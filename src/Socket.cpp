/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:26:18 by panger            #+#    #+#             */
/*   Updated: 2024/07/31 13:25:49 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket()
{
}

Socket::Socket(int port)
{
	this->_port = port;
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