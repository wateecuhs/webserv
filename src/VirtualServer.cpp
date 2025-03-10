/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:26:18 by panger            #+#    #+#             */
/*   Updated: 2024/08/22 13:15:51 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualServer.hpp"
#include <iostream>
#include <sstream>
#include "utils.hpp"
#include "enums.hpp"
#include "parsing.hpp"
#include "exceptions.hpp"
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <dirent.h>
#include <sys/wait.h>
#include <cstdlib>
#include <fcntl.h>

static std::string	listDirectory(const std::string &path);

VirtualServer::VirtualServer(std::stringstream &iss, std::string word): _pid(0)
{
	ConfState		state = conf_new_token;
	_locations = std::vector<Location>();
	std::string		shaved_word;
	bool			trailing_semicolon;
	this->_body_size = 4096;

	while (iss >> word)
	{
		trailing_semicolon = word[word.size() - 1] == ';' && word.size() > 1;
		if (trailing_semicolon)
			shaved_word = word.substr(0, word.size() - 1);
		else
			shaved_word = word;

		switch (state) {
			case conf_new_token:
				if (word == "listen")
					state = conf_listen;
				else if (word == "server_name")
					state = conf_server_name;
				else if (word == "location")
					state = conf_location;
				else if (word == "error_pages")
					state = conf_error_pages;
				else if (word == "body_size")
					state = conf_body_size;
				else if (word == "}")
					return ;
				else
					throw InvalidConfigFile();
				break;

			case conf_server_name:
				if (word == ";") {
					state = conf_new_token;
					break;
				}
				this->addServerName(shaved_word);
				if (trailing_semicolon)
					state = conf_new_token;
				break;

			case conf_listen:
				verifyAddHostPort(shaved_word, *this);
				state = trailing_semicolon ? conf_new_token : conf_semicolon;
				break;

			case conf_error_pages:
				state = verifyErrorPageMatch(word, iss, *this);
				break;

			case conf_body_size:
				this->_body_size = ft_strtoi(shaved_word);
				state = trailing_semicolon ? conf_new_token : conf_semicolon;
				break;

			case conf_location:
				{
					Location location(iss, word);
					this->_locations.push_back(location);
					state = conf_new_token;
				}
				break;

			case conf_semicolon:
				if (word != ";")
					throw InvalidConfigFile();
				state = conf_new_token;
				break;

			default:
				throw InvalidConfigFile();
		}
	}
	throw InvalidConfigFile();
}

VirtualServer::~VirtualServer()
{
}

VirtualServer::VirtualServer(const VirtualServer &copy): _pid(0)
{
	*this = copy;
}

VirtualServer &VirtualServer::operator=(const VirtualServer &copy)
{
	if (this != &copy)
	{
		this->_host = copy.getHost();
		this->_port = copy.getPort();
		this->_server_names.clear();
		this->_server_names = copy.getServerNames();
		this->_error_pages = copy.getErrorPages();
		this->_body_size = copy.getBodySize();
		this->_locations = copy.getLocations();
	}
	return *this;
}

std::vector<std::string> VirtualServer::getServerNames() const
{
	return this->_server_names;
}

void VirtualServer::addServerName(std::string name)
{
	this->_server_names.push_back(name);
}

void VirtualServer::setHost(std::string host)
{
	this->_host = host;
}

std::string VirtualServer::getHost() const
{
	return this->_host;
}

void VirtualServer::setPort(int port)
{
	this->_port = port;
}

int VirtualServer::getPort() const
{
	return this->_port;
}

void VirtualServer::addErrorPage(int error_code, std::string path)
{
	this->_error_pages[error_code] = path;
}

std::map<int, std::string> VirtualServer::getErrorPages() const
{
	return this->_error_pages;
}

void VirtualServer::setBodySize(int size)
{
	this->_body_size = size;
}

int VirtualServer::getBodySize() const
{
	return this->_body_size;
}

void VirtualServer::addLocation(Location location)
{
	this->_locations.push_back(location);
}

std::vector<Location> VirtualServer::getLocations() const
{
	return this->_locations;
}



void VirtualServer::sendResponse(Request request, int client_fd)
{
	int loc_idx = -1;

	try {
		_hasForked = false;
		_pid = 0;
		if (request.getPath().size() > 1000)
			throw URITooLong414();
		if (request.getBody().size() > (size_t)this->_body_size)
			throw ContentTooLarge413();
		loc_idx = _findLocation(request.getPath());
		if (loc_idx != -1)
			this->_methodHandler(request, client_fd, &this->_locations[loc_idx]);
		else
			this->_methodHandler(request, client_fd, (Location *)NULL);
		if (_hasForked && _pid == 0) {
			std::cout << request.getResponse().substr(0, request.getResponse().find("\r\n")) << " - " << request.getPath() << std::endl;
			throw ExitCleanup();
		}
		else if (!_hasForked)
			std::cout << request.getResponse().substr(0, request.getResponse().find("\r\n")) << " - " << request.getPath() << std::endl;
	}
	catch (const std::exception &e) {
		if (e.what() == std::string("Exit cleanup"))
			throw ExitCleanup();
		if (_hasForked && _pid > 0)
			return ;
		std::string response = e.what();
		int 		status_code = ft_strtoi(response.substr(0, 3));

		if (_error_pages.find(status_code) != _error_pages.end()) {
			std::ifstream file(_error_pages[status_code].c_str());
			if (!file) {
				request.setResponse(response, "");
				file.close();
				close(client_fd);
				std::cout << request.getResponse().substr(0, request.getResponse().find("\r\n")) << " - " << request.getPath() << std::endl;
				if (_hasForked && _pid == 0)
					throw ExitCleanup();
				return ;
			}
			std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			request.setResponse(response, content);
			file.close();
		}
		else
			request.setResponse(response, "");
		std::cout << request.getResponse().substr(0, request.getResponse().find("\r\n")) << " - " << request.getPath() << std::endl;

		if (send(client_fd, request.getResponse().c_str(), request.getResponse().size(), 0) == -1) {
			close(client_fd);
			if (_hasForked && _pid == 0)
				throw ExitCleanup();
			return ;
		}
		if (_hasForked && _pid == 0)
			throw ExitCleanup();
	}
}

int VirtualServer::_findLocation(std::string path)
{
	int						loc_idx = -1;
	size_t					longest_length = 0;
	std::string				tmp_path;

	for (size_t i = 0; i < this->_locations.size(); i++)
	{
		tmp_path = this->_locations[i].getPath();
		if (path.rfind(tmp_path, 0) == 0 && (tmp_path.length() > longest_length || longest_length == 0))
		{
			longest_length = tmp_path.length();
			loc_idx = i;
		}
	}
	return loc_idx;
}

void VirtualServer::_methodHandler(Request& request, int client_fd, Location *loc)
{
	switch (request.getMethod()) {
		case GET:
			if (loc && !loc->getMethod(GET))
				throw MethodNotAllowed405();
			this->_handleGetRequest(request, loc, client_fd);
			break;
		case POST:
			if (loc && !loc->getMethod(POST))
				throw MethodNotAllowed405();
			this->_handlePostRequest(request, loc, client_fd);
			break;
		case DELETE:
		if (loc && !loc->getMethod(DELETE))
				throw MethodNotAllowed405();
			this->_handleDeleteRequest(request, loc, client_fd);
			break;
		default:
			throw MethodNotAllowed405();
	}
}

void VirtualServer::_handleGetRequest(Request &request, Location *location, int client_fd)
{
	if (location && location->getRedirect()) {
		std::pair<std::string, std::string> header("Location", location->getHttpRedirection());
		request.setResponse("301 Moved Permanently", header, "");
		if (send(client_fd, request.getResponse().c_str(), request.getResponse().size(), 0) == -1)
			throw InternalServerError500();
		return ;
	}

	std::string path;
	location && !location->getRoot().empty() ? path = location->getRoot() + request.getPath().substr(location->getPath().size()) : path = request.getPath();

	if (pathIsDirectory(path)) {
		if (location){
			if (location->getAutoindex()) {
				std::string content = listDirectory(path);
				request.setResponse("200 OK", content);
				if (send(client_fd, request.getResponse().c_str(), request.getResponse().size(), 0) == -1)
					throw InternalServerError500();
				return ;
			}
			else if (!location->getDefaultFile().empty()) {
				path += "/" + location->getDefaultFile();
			}
		}
		else
			path += "/index.html";
	}

	std::ifstream file(path.c_str());
	if (!file){
		file.close();
		throw NotFound404();
	}

	if (access(path.c_str(), R_OK) == -1) {
		file.close();
		throw Forbidden403();
	}

	if (location) {
		std::string	file_extension = path.substr(path.find_last_of('.'));
		if (location->getUseCGI() && !location->getCGIPath(file_extension).empty()) {
			this->_handleCGI(request, location, client_fd);
			file.close();
			return ;
		}
	}

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	request.setResponse("200 OK", content);
	file.close();
	if (send(client_fd, request.getResponse().c_str(), request.getResponse().size(), 0) == -1)
		throw InternalServerError500();
}

void	VirtualServer::_handleUpload(Request &request, Location *location, int client_fd)
{
	std::string path;
	!location->getRoot().empty() ? path = location->getRoot() + location->getUploadPath() + request.getPath().substr(location->getPath().size()) : path = location->getUploadPath() + request.getPath();

	if (pathIsDirectory(path))
		throw BadRequest();

    std::ofstream file(path.c_str(), std::ios::out | std::ios::trunc);
    if (!file)
		throw NotFound404();

	if (access(path.c_str(), W_OK) == -1){
		file.close();
		throw Forbidden403();
	}

    std::string body = request.getBody();
    file << body;
    file.close();
	request.setResponse("201 Created", body);
	if (send(client_fd, request.getResponse().c_str(), request.getResponse().size(), 0) == -1)
		throw InternalServerError500();
}

void VirtualServer::_handlePostRequest(Request &request, Location *location, int client_fd)
{
	if (location && location->getFileUpload())
		return this->_handleUpload(request, location, client_fd);

	std::string	path;
	location && !location->getRoot().empty() ? path = location->getRoot() + request.getPath().substr(location->getPath().size()) : path = request.getPath();

	if (pathIsDirectory(path))
		path += "/uploadedData.txt";

	std::ofstream file(path.c_str(), std::ios::out | std::ios::app);
	if (!file) {
		file.open(path.c_str(), std::ios::out | std::ios::trunc);
		if (!file) {
			file.close();
			throw NotFound404();
		}
		std::string body = request.getBody();
		file << body;
		file.close();
		request.setResponse("201 Created", body);
		if (send(client_fd, request.getResponse().c_str(), request.getResponse().size(), 0) == -1)
			throw InternalServerError500();
		return ;
	}
	if (location && location->getUseCGI() && path.find_last_of('.') != std::string::npos) {
		std::string	file_extension = path.substr(path.find_last_of('.'));
		if (!location->getCGIPath(file_extension).empty()) {
			this->_handleCGI(request, location, client_fd);
			file.close();
			return ;
		}
	}

	if (access(path.c_str(), W_OK) == -1) {
		file.close();
		throw Forbidden403();
	}

	std::string body = request.getBody() + "\n";
	file << body;
	file.close();
	request.setResponse("200 OK", body);
	if (send(client_fd, request.getResponse().c_str(), request.getResponse().size(), 0) == -1)
		throw InternalServerError500();
}

void	VirtualServer::_handleDeleteRequest(Request &request, Location *location,int client_fd)
{
	std::string	path;
	location && !location->getRoot().empty() ? path = location->getRoot() + request.getPath().substr(location->getPath().size()) : path = request.getPath();

	if (std::remove(path.c_str()) != 0)
		throw NotFound404();
	request.setResponse("204 No Content", "");
	if (send(client_fd, request.getResponse().c_str(), request.getResponse().size(), 0) == -1)
		throw InternalServerError500();
}

static std::string	listDirectory(const std::string &path)
{
	std::string response = "<html><head><title>Index of " + path + "</title></head><body><h1>Index of " + path + "</h1><ul>";
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			response += "<li><a href=\"" + std::string(ent->d_name) + "\">" + std::string(ent->d_name) + "</a></li>";
		}
		closedir(dir);
	}
	response += "</ul></body></html>";
	return response;
}

std::string	VirtualServer::_execCGI(Request &request, Location *location)
{
	std::string	request_method = "REQUEST_METHOD=" + request.getMethodString();
	if (request.getQuery().empty())
		request.setQuery(request.getBody());
	std::string	query_string = "QUERY_STRING=" + request.getQuery();
	std::string	content_length = "CONTENT_LENGTH=" + strSizeToStr(request.getBody());
	std::string	extension = request.getPath().substr(request.getPath().find_last_of('.'));
	std::string cgi_handler = location->getCGIPath(extension);

	const char 	*env[4] = {request_method.c_str(), query_string.c_str(), content_length.c_str(), NULL};
	std::string	path;
	!location->getRoot().empty() ? path = location->getRoot() + request.getPath().substr(location->getPath().size()) : path = request.getPath();
	const char 	*argv[3] = {cgi_handler.c_str(), path.c_str(), NULL};

	pid_t 	pid;
	int 	pipe_out[2];
	int 	pipe_in[2];
	int 	status = 0;

	if (pipe(pipe_out) == -1 || pipe(pipe_in) == -1)
		throw InternalServerError500();
	pid = fork();
	if (pid == -1)
		throw InternalServerError500();

	//child process
	if (pid == 0) {
		close(pipe_out[0]);
		close(pipe_in[1]);
		if (dup2(pipe_out[1], STDOUT_FILENO) == -1 || dup2(pipe_in[0], STDIN_FILENO) == -1)
			throw InternalServerError500();
		close(pipe_out[1]);
		close(pipe_in[0]);

		if (access(cgi_handler.c_str(), X_OK) == -1)
			throw BadGateway502();
		if (execve(const_cast<const char*>(cgi_handler.c_str()), const_cast<char* const*>(argv), const_cast<char* const*>(env)) == -1)
			throw BadGateway502();

		std::exit(EXIT_SUCCESS);
	}

	//parent process
	else {
		close(pipe_out[1]);
		close(pipe_in[0]);

		//put request body into pipe for cgi handler to read
		write(pipe_in[1], request.getBody().c_str(), request.getBody().size());
		close(pipe_in[1]);

		char 				buffer[2048];
		int 				bytes_read;
		std::stringstream 	ss;
		time_t				start = time(NULL);
		int					epoll_fd = epoll_create1(0);

		setFDNonBlocking(pipe_out[0]);
		struct epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = pipe_out[0];
		epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pipe_out[0], &event);
		while (1) {
			if (time(NULL) - start > 1) {
				kill(pid, SIGKILL);
				close(epoll_fd);
				close(pipe_out[0]);
				throw GatewayTimeout504();
			}
			int events = epoll_wait(epoll_fd, &event, 1, 0);
			if (events == -1) {
				close(pipe_out[0]);
				close(epoll_fd);
				throw InternalServerError500();
			}
			if (events == 0)
				continue ;
			if (event.events & EPOLLIN) {
				bytes_read = read(pipe_out[0], buffer, 1);
				if (bytes_read == -1) {
					close(epoll_fd);
					close(pipe_out[0]);
					throw BadGateway502();
				}
				if (bytes_read == 0)
					break ;
				buffer[bytes_read] = '\0';
				ss << buffer;
				if (ss.str().size() > 500000) {
					kill(pid, SIGKILL);
					close(epoll_fd);
					close(pipe_out[0]);
					throw InternalServerError500();
				}
			}
			if (waitpid(pid, &status, WNOHANG) == pid)
				break ;
		}
		std::cout << "broke" << std::endl;
		close (epoll_fd);
		close(pipe_out[0]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			throw BadGateway502();

		return ss.str();
	}
}

void	VirtualServer::_handleCGI(Request &request, Location *location, int client_fd)
{
	int backup_stdin = dup(STDIN_FILENO);
	if (backup_stdin == -1)
		throw InternalServerError500();
	int backup_stdout = dup(STDOUT_FILENO);
	if (backup_stdout  == -1)
		throw InternalServerError500();

	_pid = fork();
	_hasForked = true;
	if (_pid == -1)
		throw InternalServerError500();
	if (_pid == 0) {
		try {
			std::string content = this->_execCGI(request, location);
			if (dup2(backup_stdin, STDIN_FILENO) == -1 || dup2(backup_stdout, STDOUT_FILENO) == -1)
				throw InternalServerError500();
			close(backup_stdin);
			close(backup_stdout);

			request.setResponse("200 OK", content);
			if (send(client_fd, request.getResponse().c_str(), request.getResponse().size(), 0) == -1)
				throw InternalServerError500();
		}
		catch (std::exception &e){
			close(backup_stdin);
			close(backup_stdout);
			throw;
		}
	}
	close(backup_stdin);
	close(backup_stdout);
}
