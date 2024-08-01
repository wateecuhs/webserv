/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:37:42 by alermolo          #+#    #+#             */
/*   Updated: 2024/08/01 16:20:44 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Socket.hpp"
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <sstream>

std::string	execCGI(const Request &request, const Socket &socket);
void		handleCGI(const Request &request, const Socket &socket);

class NotFound404: public std::exception {
	public:
		virtual const char *what() const throw() {
			return "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
		}
};

class Forbidden403: public std::exception {
	public:
		virtual const char *what() const throw() {
			return "HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n";
		}
};

class InternalServerError500: public std::exception {
	public:
		virtual const char *what() const throw() {
			return "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
		}
};

void handleGetRequest(const Request &request, const Socket &socket) {
	std::string path = request.getPath();
	std::stringstream ss;

	if (request.pathIsDirectory()){
		std::string indexPath = path + "/index";
		std::string extensions[] = {".html", ".php", ".xml"};
		for (size_t i = 0; i < 3; i++) {
			std::ifstream indexFile((indexPath + extensions[i]).c_str());
			if (indexFile) {
				std::string content((std::istreambuf_iterator<char>(indexFile)),
									std::istreambuf_iterator<char>());
				ss << content.size();
				std::string size = ss.str();
				std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + size + "\r\n\r\n" + content;
				if (send(socket.getFd(), response.c_str(), response.size(), 0) == -1)
					throw InternalServerError500();
				indexFile.close();
				return;
			}
		}
		throw NotFound404();
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

	//handle CGI
	std::string	file_extension = path.substr(path.find_last_of('.'));
	if (!socket.getCgiHandler(file_extension).empty() && socket.usesCGI()){
		handleCGI(request, socket);
		// int backup_stdin = dup(STDIN_FILENO);
		// if (backup_stdin == -1)
		// 	throw InternalServerError500();
		// int backup_stdout = dup(STDOUT_FILENO);
		// if (backup_stdout  == -1)
		// 	throw InternalServerError500();
		// std::string content = execCGI(request, socket);
		// if (dup2(backup_stdin, STDIN_FILENO) || dup2(backup_stdout, STDOUT_FILENO))
		// 	throw InternalServerError500();
		// close(backup_stdin);
		// close(backup_stdout);

		// ss << content.size();
		// std::string size = ss.str();
		// std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + size + "\r\n\r\n" + content;
		// if (send(socket.getFd(), response.c_str(), response.size(), 0) == -1)
		// 	throw InternalServerError500();
		file.close();
		return ;
	}


	std::string content((std::istreambuf_iterator<char>(file)),
						 std::istreambuf_iterator<char>());
	ss << content.size();
	std::string size = ss.str();
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + size + "\r\n\r\n" + content;
	send(socket.getFd(), response.c_str(), response.size(), 0);
	file.close();
}

void handlePostRequest(const Request &request, const Socket &socket) {
	std::string path = request.getPath();
	if (request.pathIsDirectory())
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
		std::stringstream ss;
		ss << body.size();
		std::string size = ss.str();
		std::string response = "HTTP/1.1 201 Created\r\nContent-Length: " + size + "\r\n\r\n" + body;
		send(socket.getFd(), response.c_str(), response.size(), 0);
		file.close();
		return ;
	}

	//handle CGI
	std::string	file_extension = path.substr(path.find_last_of('.'));
	if (!socket.getCgiHandler(file_extension).empty() && socket.usesCGI()){
		handleCGI(request, socket);
		// int backup_stdin = dup(STDIN_FILENO);
		// if (backup_stdin == -1)
		// 	throw InternalServerError500();
		// int backup_stdout = dup(STDOUT_FILENO);
		// if (backup_stdout  == -1)
		// 	throw InternalServerError500();
		// std::string content = execCGI(request, socket);
		// if (dup2(backup_stdin, STDIN_FILENO) || dup2(backup_stdout, STDOUT_FILENO))
		// 	throw InternalServerError500();
		// close(backup_stdin);
		// close(backup_stdout);

		// std::stringstream ss;
		// ss << content.size();
		// std::string size = ss.str();
		// std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + size + "\r\n\r\n" + content;
		// if (send(socket.getFd(), response.c_str(), response.size(), 0) == -1)
		// 	throw InternalServerError500();
		file.close();
		return ;
	}

	if (access(path.c_str(), W_OK) == -1){
		file.close();
		throw Forbidden403();
	}

	std::string body = request.getBody() + "\n";
	file << body;
	std::stringstream ss;
	ss << body.size();
	std::string size = ss.str();

	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + size + "\r\n\r\n" + body;
	send(socket.getFd(), response.c_str(), response.size(), 0);
	file.close();
}

void	handleDeleteRequest(const Request &request, const Socket &socket) {
	std::string path = request.getPath();
	if (std::remove(path.c_str()) != 0)
		throw NotFound404();
	std::string response = "HTTP/1.1 204 No Content\r\nContent-Length: 0\r\n\r\n";
	send(socket.getFd(), response.c_str(), response.size(), 0);
}

void methodHandler(const Request& request, const Socket &socket) {
	switch (request.getMethod()) {
		case GET:
			handleGetRequest(request, socket);
			break;
		case POST:
			handlePostRequest(request, socket);
			break;
		case DELETE:
			handleDeleteRequest(request, socket);
			break;
		default:
			throw BadRequest();
	}
}

