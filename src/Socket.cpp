/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:26:18 by panger            #+#    #+#             */
/*   Updated: 2024/08/26 14:15:28 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "VirtualServer.hpp"
#include "utils.hpp"
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
#include <dirent.h>
#include <sys/wait.h>
#include <cstdlib>
#include <fcntl.h>

Socket::Socket(std::string host, int port): _host(host), _port(port)
{
}

Socket::~Socket() {}

Socket::Socket(const Socket &copy)
{
	*this = copy;
}

Socket &Socket::operator=(const Socket &copy)
{
	if (this != &copy)
	{
		this->_fd = copy.getFd();
		this->_host = copy.getHost();
		this->_port = copy.getPort();
		this->_virtual_servers = copy.getVirtualServers();
		this->_clients = copy.getClients();
	}
	return (*this);
}

void Socket::setFd(int fd)
{
	this->_fd = fd;
}

int Socket::getFd() const
{
	return (this->_fd);
}

void Socket::setHost(std::string host)
{
	this->_host = host;
}

std::string Socket::getHost() const
{
	return (this->_host);
}

void Socket::setPort(int port)
{
	this->_port = port;
}

int Socket::getPort() const
{
	return (this->_port);
}

void Socket::addVirtualServer(VirtualServer VirtualServer)
{
	this->_virtual_servers.push_back(VirtualServer);
}

std::vector<VirtualServer> Socket::getVirtualServers() const
{
	return (this->_virtual_servers);
}

int Socket::startListening(int epfd)
{
	int			option = 1;
	sockaddr_in	server_addr;
	epoll_event event;

	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd == -1)
		throw std::runtime_error("Failed to create VirtualServer");
	setFDNonBlocking(this->_fd);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_port);
	if (_host == "*")
		server_addr.sin_addr.s_addr = INADDR_ANY;
	else
	{
		if (inet_aton(_host.c_str(), &server_addr.sin_addr) == -1)
			throw std::runtime_error("Failed to convert host to network address");
	}

	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
		throw std::runtime_error("Failed to set VirtualServer options");
	if (bind(this->_fd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		throw std::runtime_error("Failed to bind VirtualServer");

	event.data.fd = this->_fd;
	event.events = EPOLLIN | EPOLLPRI;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, this->_fd, &event) == -1)
		throw std::runtime_error("Failed to add VirtualServer to epoll");
	if (listen(this->_fd, 5) == -1)
		throw std::runtime_error("Failed to listen on VirtualServer");
	return (this->_fd);
}

void Socket::sendResponse(Request request, int client_fd)
{
	std::string host;
	std::vector<std::string> server_names;
	try {
		host = request.getHeaders().find("Host")->second;
	}
	catch (...) {}

	for (std::vector<VirtualServer>::iterator it = _virtual_servers.begin(); it != _virtual_servers.end(); it++)
	{
		server_names = it->getServerNames();
		for (std::vector<std::string>::iterator it2 = server_names.begin(); it2 != server_names.end(); it2++)
		{
			if (host == *it2)
			{
				it->sendResponse(request, client_fd);
				return ;
			}
		}
	}
	_virtual_servers.begin()->sendResponse(request, client_fd);
}


int Socket::acceptConnection(int event_fd)
{
	int client_socket = accept(event_fd, (sockaddr *)NULL, (socklen_t *)NULL);

	if (client_socket == -1)
		throw std::runtime_error("Failed to accept client connection");
	setFDNonBlocking(client_socket);
	_clients[client_socket] = Client(client_socket);
	return client_socket;
}

std::map<int, Client> &Socket::getClientsRef()
{
	return _clients;
}

std::map<int, Client> Socket::getClients() const
{
	return _clients;
}
