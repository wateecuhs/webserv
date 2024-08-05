/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:37:42 by alermolo          #+#    #+#             */
/*   Updated: 2024/08/05 18:47:36 by alermolo         ###   ########.fr       */
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

std::string	execCGI(Request &request);
void		handleCGI(Request &request);

std::string	listDirectory(const std::string &path) {
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

// void handleGetRequest(Request &request, const Socket &socket) {
void handleGetRequest(Request &request) {
	std::string 	path = request.getPath();
	Location 		*location = request.getLocation();
	Socket 			socket = request.getSocket();

	if (request.pathIsDirectory()){
		std::string indexPath = path;

		// location->getDefaultFile().empty() ? indexPath += "/index.html" : indexPath += "/" + location->getDefaultFile();

		// std::string extensions[] = {".html", ".php", ".xml"};
		// for (size_t i = 0; i < 3; i++) {
		// std::ifstream indexFile((indexPath + extensions[i]).c_str());
		if (!location->getDefaultFile().empty())
			indexPath += "/" + location->getDefaultFile();
		else if (location->getAutoindex())
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

	std::string	file_extension = path.substr(path.find_last_of('.'));
	// NE COMPILE PAS CAR LES CGI SE TROUVENT DANS LOCATION
	if (location->getUseCGI() && !location->getCGIPath(file_extension).empty()){
		handleCGI(request);
		file.close();
		return ;
	}

	std::string content((std::istreambuf_iterator<char>(file)),
						 std::istreambuf_iterator<char>());
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + strSizeToStr(content) + "\r\n\r\n" + content;
	send(socket.getFd(), response.c_str(), response.size(), 0);
	file.close();
}

// void handlePostRequest(Request &request, const Socket &socket) {
void handlePostRequest(Request &request) {
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
		std::string response = "HTTP/1.1 201 Created\r\nContent-Length: " + strSizeToStr(body) + "\r\n\r\n" + body;
		send(socket.getFd(), response.c_str(), response.size(), 0);
		file.close();
		return ;
	}

	std::string	file_extension = path.substr(path.find_last_of('.'));
	// NE COMPILE PAS CAR LES CGI SE TROUVENT DANS LOCATION
	// if (!socket.getCgiHandler(file_extension).empty() && socket.usesCGI()){
	// 	handleCGI(request, socket);
	// 	file.close();
	// 	return ;
	// }

	if (access(path.c_str(), W_OK) == -1){
		file.close();
		throw Forbidden403();
	}

	std::string body = request.getBody() + "\n";
	file << body;
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + strSizeToStr(body) + "\r\n\r\n" + body;
	send(socket.getFd(), response.c_str(), response.size(), 0);
	file.close();
}

// void	handleDeleteRequest(const Request &request, const Socket &socket) {
void	handleDeleteRequest(Request &request) {
	std::string path = request.getPath();
	if (std::remove(path.c_str()) != 0)
		throw NotFound404();
	std::string response = "HTTP/1.1 204 No Content\r\nContent-Length: 0\r\n\r\n";
	send(socket.getFd(), response.c_str(), response.size(), 0);
}

// void methodHandler(Request& request, const Socket &socket) {
void methodHandler(Request& request) {
	Location 	*location = request.getLocation();
	bool		*allowed_methods = location->getMethods();

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

