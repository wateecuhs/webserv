/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:26:18 by panger            #+#    #+#             */
/*   Updated: 2024/08/01 16:37:55 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket()
{
}

Socket::Socket(int fd)
{
	this->_fd = fd;
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
		this->_CGI_map = copy._CGI_map;
		this->_server_names = copy._server_names;
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

int Socket::getFd() const
{
	return this->_fd;
}

bool Socket::usesCGI() const
{
	return this->_useCGI;
}

void Socket::addCgiHandler(std::string extension, std::string path)
{
	this->_CGI_map[extension] = path;
	this->_useCGI = true;
}

std::string Socket::getCgiHandler(std::string extension) const
{
	std::map<std::string, std::string>::const_iterator it = this->_CGI_map.find(extension);
	if (it == this->_CGI_map.end())
		return "";
	return it->second;
}
