/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:37:42 by alermolo          #+#    #+#             */
/*   Updated: 2024/07/27 15:35:25 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <sstream>

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

void handleGetRequest(const Request& request, int socket) {
	std::string path = request.getPath();
	std::stringstream ss;

	// if (!file){
	if (request.pathIsDirectory()){
		std::string indexPath = path + "/index";
		std::string extensions[] = {".html", ".php", ".xml"}; // Add more extensions if needed
		for (size_t i = 0; i < extensions->size(); i++) {
			std::ifstream indexFile((indexPath + extensions[i]).c_str());
			if (indexFile) {
				std::string content((std::istreambuf_iterator<char>(indexFile)),
									std::istreambuf_iterator<char>());
				ss << content.size();
				std::string size = ss.str();
				std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + size + "\r\n\r\n" + content;
				send(socket, response.c_str(), response.size(), 0);
				indexFile.close();
				return;
			}
		}
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

	std::string content((std::istreambuf_iterator<char>(file)),
						 std::istreambuf_iterator<char>());
	ss << content.size();
	std::string size = ss.str();
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + size + "\r\n\r\n" + content;
	send(socket, response.c_str(), response.size(), 0);
	// std::cout << response;
	file.close();
}

void handlePostRequest(const Request& request, int socket) {
	std::string path = request.getPath();
	std::ofstream file(path.c_str(), std::ios::out | std::ios::app); // Open file in append mode

	if (!file){
		file.open(path.c_str(), std::ios::out | std::ios::trunc); // Create file if it doesn't exist
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
		send(socket, response.c_str(), response.size(), 0);
		file.close();
		return ;
	}
	if (access(path.c_str(), W_OK) == -1){
		file.close();
		throw Forbidden403();
	}

	std::string body = request.getBody();
	file << body;
	std::stringstream ss;
	ss << body.size();
	std::string size = ss.str();

	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + size + "\r\n\r\n" + body;
	send(socket, response.c_str(), response.size(), 0);
	// std::cout << response;
	file.close();
}

void methodHandler(const Request& request, int socket) {
	switch (request.getMethod()) {
		case GET:
			handleGetRequest(request, socket);
			break;
		case POST:
			handlePostRequest(request, socket);
			break;
		default:
			throw BadRequest();
	}
}

