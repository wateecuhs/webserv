/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wateecuhs <wateecuhs@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:56:28 by waticouz          #+#    #+#             */
/*   Updated: 2024/08/25 19:27:51 by wateecuhs        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <sys/socket.h>
#include "Request.hpp"
#include <unistd.h>
#include "exceptions.hpp"
#include <fcntl.h>

Client::Client(): _fd(-1), _isReady(false), _lastRequestTime(-1), _hasRequestedBefore(false)
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

#include <string.h>
#include <errno.h>

int Client::readRequest()
{
	char buf[4096] = {0};
	std::string request;
	int bytes_read = 0;

	_lastRequestTime = time(NULL);
	while (1)
	{
		bytes_read = recv(_fd, buf, sizeof(buf) - 1, O_NONBLOCK);
		if (bytes_read < 0)
			return 0;
		if (bytes_read == 0) {
			close(_fd);
			_fd = -1;
			return 0;
		}
		request.append(buf, bytes_read);
		if (request.find("\r\n\r\n") != std::string::npos)
			break;
	}
	_request = Request(std::string(buf));
	_isReady = true;
	_hasRequestedBefore = true;
	return (request.length());
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

bool Client::isTimedOut()
{
	if (_hasRequestedBefore == false)
		return (false);
	if (time(NULL) - _lastRequestTime > 5)
		return (true);
	return (false);
}