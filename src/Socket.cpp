/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:26:18 by panger            #+#    #+#             */
/*   Updated: 2024/08/07 13:18:47 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
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

Socket::Socket(std::stringstream &iss, std::string word)
{
	ConfState		state = conf_new_token;
	_locations = std::vector<Location>();
	std::string		shaved_word;
	bool			trailing_semicolon;

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
					Location location(iss, word, *this);
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

int Socket::startListening(int epfd, epoll_event &ep_event)
{
	sockaddr_in server_addr;
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_port);
	if (_host == "*")
		server_addr.sin_addr.s_addr = INADDR_ANY;
	else
		inet_aton(_host.c_str(), &server_addr.sin_addr);

	bind(this->_fd, (sockaddr *)&server_addr, sizeof(server_addr));

	ep_event.data.fd = this->_fd;
	ep_event.events = EPOLLIN | EPOLLPRI;
	epoll_ctl(epfd, EPOLL_CTL_ADD, this->_fd, &ep_event);
	listen(this->_fd, 5);
	return (this->_fd);
}

Socket::~Socket()
{
}

Socket::Socket(const Socket &copy)
{
	*this = copy;
}

Socket &Socket::operator=(const Socket &copy)
{
	if (this != &copy)
	{
		this->_fd = copy._fd;
		this->_host = copy._host;
		this->_port = copy._port;
		this->_server_names = copy._server_names;
		this->_error_pages = copy._error_pages;
		this->_body_size = copy._body_size;
		this->_locations = copy._locations;
	}
	return *this;
}

std::vector<std::string> Socket::getServerNames() const
{
	return this->_server_names;
}

void Socket::addServerName(std::string name)
{
	this->_server_names.push_back(name);
}

void Socket::setHost(std::string host)
{
	this->_host = host;
}

std::string Socket::getHost() const
{
	return this->_host;
}

void Socket::setPort(int port)
{
	this->_port = port;
}

int Socket::getPort() const
{
	return this->_port;
}

void Socket::addErrorPage(int error_code, std::string path)
{
	this->_error_pages[error_code] = path;
}

std::map<int, std::string> Socket::getErrorPages() const
{
	return this->_error_pages;
}

void Socket::setBodySize(int size)
{
	this->_body_size = size;
}

int Socket::getBodySize() const
{
	return this->_body_size;
}

void Socket::addLocation(Location location)
{
	this->_locations.push_back(location);
}

std::vector<Location> Socket::getLocations() const
{
	return this->_locations;
}

void Socket::setFd(int fd)
{
	this->_fd = fd;
}

int Socket::getFd() const
{
	return this->_fd;
}
