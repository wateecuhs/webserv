/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:26:18 by panger            #+#    #+#             */
/*   Updated: 2024/08/02 14:03:19 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include <iostream>
#include <sstream>
#include "utils.hpp"
#include "enums.hpp"
#include "parsing.hpp"


Socket::Socket(std::stringstream &iss, std::string word)
{
	ConfState state = conf_new_token;

	while (!iss.eof())
	{
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
				else 
					throw InvalidConfigFile();
				break;

			case conf_server_name:
				if (word == ";")
				state = conf_new_token;
				else if (word[word.size() - 1] == ';' && word.size() > 1) {
					this->addServerName(word.substr(0, word.size() - 1));
					state = conf_new_token;
				}
				else
					this->addServerName(word);
				break;
			case conf_listen:
				if (word == ";")
					state = conf_new_token;
				if (word[word.size() - 1] == ';' && word.size() > 1) {
					verifyAddHostPort(word.substr(0, word.size() - 1), *this);
					state = conf_new_token;
					break;
				}
				else {
					verifyAddHostPort(word, *this);
					state = conf_semicolon;
				}
				break;
			case conf_error_pages:
				state = verifyErrorPageMatch(word, iss, *this);
				break;
			case conf_location:
				{
					Location location(iss, word);
					this->_locations.push_back(location);
				}
				break;
			case conf_semicolon:
				if (word != ";")
					throw InvalidConfigFile();
				state = conf_new_token;
				break;
			case conf_body_size:
				if (word[word.size() - 1] == ';' && word.size() > 1) {
					this->_body_size = ft_strtoi(word.substr(0, word.size() - 1));
					state = conf_new_token;
					break;
				}
				else
					this->_body_size = ft_strtoi(word);
				state = conf_semicolon;
				break;
			default:
				return ;
		}
		iss >> word;
	}
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
		this->_useCGI = copy._useCGI;
		this->_server_names = copy._server_names;
		this->_error_pages = copy._error_pages;
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

const char *InvalidConfigFile::what() const throw()
{
	return "Config file is invalid";
}

const char *BadInput::what() const throw()
{
	return "Invalid input";
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