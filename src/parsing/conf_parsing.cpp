/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_parsing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:22:31 by panger            #+#    #+#             */
/*   Updated: 2024/08/01 17:29:45 by panger           ###   ########.fr       */
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
	std::cout << "Body size: " << socket.getBodySize() << std::endl;
	std::cout << "Error pages: " << std::endl;
	std::map<int, std::string> error_pages = socket.getErrorPages();
	for (std::map<int, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
		std::cout << "- " << it->first << " * " << it->second << std::endl;
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
					state = conf_after_server_brace;
				else
					throw InvalidConfigFile();
				break;

			case conf_after_server_brace:
				{
					Socket tmp(iss, word);
					sockets.push_back(tmp);
					printSocket(tmp);
					return sockets;
				}
			default:
				throw InvalidConfigFile();
		}
	}
	if (sockets.empty())
		throw InvalidConfigFile();
	return sockets;
}
