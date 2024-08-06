/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:37:42 by alermolo          #+#    #+#             */
/*   Updated: 2024/08/06 23:38:04 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Socket.hpp"
#include "exceptions.hpp"
#include "utils.hpp"
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <sstream>
#include <dirent.h>

void	handleCGI(Request &request);

static std::string	listDirectory(const std::string &path) {
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

static void handleGetRequest(Request &request) {
	Location	*location = request.getLocation();
	Socket		socket = request.getSocket();

	if (location && !location->getHttpRedirection().empty()){
		std::string response = "HTTP/1.1 301 Moved Permanently\r\nLocation: " + location->getHttpRedirection() + "\r\nContent-Length: 0\r\n\r\n";
		if (send(socket.getFd(), response.c_str(), response.size(), 0) == -1)
			throw InternalServerError500();
		return ;
	}

	std::string path;
	location ? path = location->getPath() + location->getRoot() + request.getPath() : path = request.getPath();

	if (pathIsDirectory(path)){
		std::string indexPath = path;

		if (location && !location->getDefaultFile().empty())
			indexPath += "/" + location->getDefaultFile();
		else if (location && location->getAutoindex())
		{
			indexPath += "/index.html";

			std::ifstream indexFile(indexPath.c_str());
			if (indexFile){
				std::string content((std::istreambuf_iterator<char>(indexFile)), std::istreambuf_iterator<char>());
				std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + strSizeToStr(content) + "\r\n\r\n" + content;
				if (send(socket.getFd(), response.c_str(), response.size(), 0) == -1)
					throw InternalServerError500();
			}
			else{
				std::string content = listDirectory(path);
				std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + strSizeToStr(content) + "\r\n\r\n" + content;
				if (send(socket.getFd(), response.c_str(), response.size(), 0) == -1)
					throw InternalServerError500();
			}
			indexFile.close();
			return;
		}
		else
			throw BadRequest();
	}

	std::ifstream file(path.c_str());
	if (!file){
		file.close();
		throw NotFound404();
	}

	if (access(path.c_str(), R_OK) == -1){
		file.close();
		throw Forbidden403();
	}

	if (location){
		std::string	file_extension = path.substr(path.find_last_of('.'));
		if (location->getUseCGI() && !location->getCGIPath(file_extension).empty()){
			handleCGI(request);
			file.close();
			return ;
		}
	}

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + strSizeToStr(content) + "\r\n\r\n" + content;
	file.close();
	if (send(socket.getFd(), response.c_str(), response.size(), 0) == -1)
		throw InternalServerError500();
}

static void	handleUpload(Request &request) {
	Location 	*location = request.getLocation();
	Socket 		socket = request.getSocket();
	std::string path = location->getPath() + location->getRoot() + request.getPath();

	if (pathIsDirectory(path))
		throw BadRequest();

    std::ofstream file(path.c_str(), std::ios::out | std::ios::trunc);
    if (!file)
        throw InternalServerError500();

	if (access(path.c_str(), W_OK) == -1){
		file.close();
		throw Forbidden403();
	}

    std::string body = request.getBody();
    file << body;
    file.close();
    std::string response = "HTTP/1.1 201 Created\r\nContent-Length: " + strSizeToStr(body) + "\r\n\r\n" + body;
    if (send(socket.getFd(), response.c_str(), response.size(), 0) == -1)
		throw InternalServerError500();
}

static void handlePostRequest(Request &request) {
	Location 	*location = request.getLocation();
	Socket 		socket = request.getSocket();

	if (location && location->getFileUpload())
		return handleUpload(request);

	std::string	path;
	location ? path = location->getPath() + location->getRoot() + request.getPath() : path = request.getPath();

	if (pathIsDirectory(path))
		path += "/uploadedData.txt";

	std::ofstream file(path.c_str(), std::ios::out | std::ios::app);

	if (!file){
		file.open(path.c_str(), std::ios::out | std::ios::trunc);
		if (!file){
			file.close();
			throw InternalServerError500();
		}
		std::string body = request.getBody();
		file << body;
		file.close();
		std::string response = "HTTP/1.1 201 Created\r\nContent-Length: " + strSizeToStr(body) + "\r\n\r\n" + body;
		if (send(socket.getFd(), response.c_str(), response.size(), 0) == -1)
			throw InternalServerError500();
		return ;
	}

	if (location){
		std::string	file_extension = path.substr(path.find_last_of('.'));
		if (location->getUseCGI() && !location->getCGIPath(file_extension).empty()){
			handleCGI(request);
			file.close();
			return ;
		}
	}

	if (access(path.c_str(), W_OK) == -1){
		file.close();
		throw Forbidden403();
	}

	std::string body = request.getBody() + "\n";
	file << body;
	file.close();
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + strSizeToStr(body) + "\r\n\r\n" + body;
	if (send(socket.getFd(), response.c_str(), response.size(), 0) == -1)
		throw InternalServerError500();
}

static void	handleDeleteRequest(Request &request) {
	Socket 		socket = request.getSocket();
	Location 	*location = request.getLocation();

	std::string	path;
	location ? path = location->getPath() + location->getRoot() + request.getPath() : path = request.getPath();

	if (std::remove(path.c_str()) != 0)
		throw NotFound404();
	std::string response = "HTTP/1.1 204 No Content\r\nContent-Length: 0\r\n\r\n";
	if (send(socket.getFd(), response.c_str(), response.size(), 0) == -1)
		throw InternalServerError500();
}

void methodHandler(Request& request) {
	bool	*allowed_methods = request.getLocation()->getMethods();

	switch (request.getMethod()) {
		case GET:
			if (!allowed_methods[GET])
				throw MethodNotAllowed405();
			handleGetRequest(request);
			break;
		case POST:
			if (!allowed_methods[POST])
				throw MethodNotAllowed405();
			handlePostRequest(request);
			break;
		case DELETE:
			if (!allowed_methods[DELETE])
				throw MethodNotAllowed405();
			handleDeleteRequest(request);
			break;
		default:
			throw BadRequest();
	}
}
