/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waticouz <waticouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:56:28 by waticouz          #+#    #+#             */
/*   Updated: 2024/08/07 15:56:29 by waticouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <sys/socket.h>
#include "Request.hpp"
#include <unistd.h>
#include "exceptions.hpp"
#include <fcntl.h>

Client::Client(): _fd(-1), _isReady(false)
{
}

Client::Client(int client_fd): _isReady(false)
{
	this->_fd = client_fd;
}

Client::~Client() {}

Client::Client(const Client &copy)
{
	*this = copy;
}

Client &Client::operator=(const Client &copy)
{
	if (this != &copy)
	{
		this->_fd = copy.getFd();
		this->_request = copy.getRequest();
		this->_isReady = copy._isReady;
	}
	return (*this);
}

int Client::getFd() const
{
	return (this->_fd);
}

Request Client::getRequest() const
{
	return (this->_request);
}

void Client::readRequest()
{
	char buf[4096] = {0};
	size_t cr = recv(_fd, buf, sizeof(buf) - 1, O_NONBLOCK);
	if (cr == 0)
		throw InternalServerError500();
	_request = Request(std::string(buf));
	_isReady = true;
}

bool Client::isReady()
{
	if (_isReady)
	{
		_isReady = false;
		return (true);
	}
	return (this->_isReady);
}

void Client::setReady(bool ready)
{
	this->_isReady = ready;
}

void Client::setFd(int fd)
{
	this->_fd = fd;
}

