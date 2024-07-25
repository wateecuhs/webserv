/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:37:42 by alermolo          #+#    #+#             */
/*   Updated: 2024/07/25 16:59:46 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>

void handleGetRequest(const Request& request) {
	std::string path = request.getPath();
	std::ifstream file(path);

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
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(content.size()) + "\r\n\r\n" + content;
	send(request.getSocket(), response.c_str(), response.size(), 0);
	// std::cout << response;
	file.close();
}

void handlePostRequest(const Request& request) {
	std::string path = request.getPath();
	std::ofstream file(path, std::ios::out | std::ios::app); // Open file in append mode

	if (!file){
		file.open(path, std::ios::out | std::ios::trunc); // Create file if it doesn't exist
		if (!file){
			file.close();
			throw InternalServerError500();
		}
		std::string body = request.getBody();
		file << body;
		std::string response = "HTTP/1.1 201 Created\r\nContent-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body;
		send(request.getSocket(), response.c_str(), response.size(), 0);
		file.close();
		return ;
	}
	if (access(path.c_str(), W_OK) == -1){
		file.close();
		throw Forbidden403();
	}

	std::string body = request.getBody();
	file << body;

	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body;
	send(request.getSocket(), response.c_str(), response.size(), 0);
	// std::cout << response;
	file.close();
}

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
