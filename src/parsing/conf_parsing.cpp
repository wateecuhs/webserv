/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_parsing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:22:31 by panger            #+#    #+#             */
/*   Updated: 2024/08/02 16:10:12 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"
#include <sstream>
#include <cstring>
#include "Socket.hpp"
#include "utils.hpp"

void printSocket(Socket &socket)
{
	std::cout << "Host: " << socket.getHost() << std::endl;
	std::cout << "Port: " << socket.getPort() << std::endl;
	std::cout << "Server names: " << std::endl;
	std::vector<std::string> server_names = socket.getServerNames();
	for (std::vector<std::string>::iterator it = server_names.begin(); it != server_names.end(); it++)
		std::cout << "- " << *it << std::endl;
	std::cout << "Body size: " << socket.getBodySize() << std::endl;
	std::cout << "Error pages: " << std::endl;
	std::map<int, std::string> error_pages = socket.getErrorPages();
	for (std::map<int, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
		std::cout << "- " << it->first << " " << it->second << std::endl;
	std::cout << "Locations: " << std::endl;
	std::vector<Location> locations = socket.getLocations();
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		std::cout << "\tLocation: " << it->getPath() << std::endl;
		std::cout << "\tRoot: " << it->getRoot() << std::endl;
		std::cout << "\tDefault file: " << it->getDefaultFile() << std::endl;
		std::cout << "\tAutoindex: " << it->getAutoindex() << std::endl;
		std::cout << "\tUse CGI: " << it->getUseCGI() << std::endl;
		std::cout << "\tMethods: " << std::endl;
		bool *methods = it->getMethods();
		std::cout << "\t- GET: " << methods[0] << std::endl;
		std::cout << "\t- POST: " << methods[1] << std::endl;
		std::cout << "\t- DELETE: " << methods[2] << std::endl;
		std::cout << "\tFile upload: " << it->getFileUpload() << std::endl;
		std::cout << "\tUpload path: " << it->getUploadPath() << std::endl;
		std::cout << "\tCGI extensions: " << std::endl;
		std::map<std::string, std::string> cgi_extensions = it->getCGIMap();
		for (std::map<std::string, std::string>::iterator it = cgi_extensions.begin(); it != cgi_extensions.end(); it++)
			std::cout << "\t- " << it->first << " " << it->second << std::endl;
		std::cout << "\tHTTP redirection: " << it->getHttpRedirection() << std::endl;
		std::cout << std::endl;
	}
}

std::vector<Socket> parseServers(std::string content);
Socket parseServerBlock(std::stringstream &iss, std::string word);

Socket *parseConfig(std::string conf_path)
{
	std::ifstream	cfstream;

	cfstream.open(conf_path.c_str());
	if (!cfstream.is_open())
		throw InvalidConfigFile();
	std::string content((std::istreambuf_iterator<char>(cfstream)),
						 std::istreambuf_iterator<char>());
	try {
		parseServers(content);
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return NULL;
}

std::vector<Socket> parseServers(std::string content)
{
	std::vector<Socket>	sockets;
	std::stringstream	iss(content);
	std::string			word;
	ConfState			state = conf_server;

	while (iss >> word)
	{
		switch (state) {
			case conf_server:
				if (word == "server")
					state = conf_server_brace;
				else if (word == "server{")
					state = conf_after_server_brace;
				else
					throw InvalidConfigFile();
				break;

			case conf_server_brace:
				if (word == "{")
				{
					Socket tmp(iss, word);
					sockets.push_back(tmp);
					printSocket(tmp);
					state = conf_after_server_brace;
					return sockets;
				}
				else
					throw InvalidConfigFile();
				break;

			default:
				throw InvalidConfigFile();
		}
	}
	if (sockets.empty())
		throw InvalidConfigFile();
	return sockets;
}
