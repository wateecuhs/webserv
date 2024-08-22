/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:49:25 by panger            #+#    #+#             */
/*   Updated: 2024/08/22 14:48:55 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>
#include "parsing.hpp"
#include "exceptions.hpp"
#include <unistd.h>
#include <signal.h>
#include <cstring>

int sig = 0;

Server::Server(std::string config): _epoll_events(10)
{
	std::ifstream	cfstream;

	cfstream.open(config.c_str());
	if (!cfstream.is_open())
		throw InvalidConfigFile();
	std::string content((std::istreambuf_iterator<char>(cfstream)),
						 std::istreambuf_iterator<char>());
	this->_sockets = _parseServers(content);
	this->_epoll_fd = epoll_create(1);
}

Server::~Server() {}

Server::Server(const Server &copy)
{
	*this = copy;
}

Server &Server::operator=(const Server &copy)
{
	if (this != &copy)
	{
		this->_sockets = copy.getSockets();
		this->_epoll_fd = copy.getEpollFd();
	}
	return (*this);
}

void Server::addSocket(Socket socket)
{
	this->_sockets.push_back(socket);
}

std::vector<Socket> Server::getSockets() const
{
	return (this->_sockets);
}

void Server::setEpollFd(int epoll_fd)
{
	this->_epoll_fd = epoll_fd;
}

int Server::getEpollFd() const
{
	return (this->_epoll_fd);
}

void Server::listenSockets()
{
	for (std::vector<Socket>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++)
		it->startListening(this->_epoll_fd);
}

void sigHandler(int sig_code)
{
	sig = sig_code;
}

void Server::startServer()
{
	std::map<int, Client>	clients;
	struct sigaction		action;

	std::memset(&action, 0, sizeof(action));
	action.sa_handler = sigHandler;
	action.sa_flags = SA_SIGINFO;
	if (sigaction(SIGINT, &action, NULL))
		throw std::runtime_error("Initializing signal handler failed");

	this->listenSockets();
	while (sig == 0)
	{
		int nfds = epoll_wait(_epoll_fd, _epoll_events.data(), _epoll_events.size(), 0);
		for (int i = 0; i < nfds; i++)
		{
			int event_fd = this->_epoll_events[i].data.fd;
			uint32_t events = this->_epoll_events[i].events;

			for (std::vector<Socket>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++)
			{
				if (it->getFd() == event_fd) {
					epoll_event _event;
					int client_socket = it->acceptConnection(event_fd);
					_event.data.fd = client_socket;
					_event.events = EPOLLIN | EPOLLOUT;
					epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_socket, &_event);
				}
				else
				{
					for (std::map<int, Client>::iterator it_client = it->getClients().begin(); it_client != it->getClients().end(); it_client++)
					{
						if (it_client->first == event_fd)
						{
							if (events & EPOLLIN)
							{
								try {
									it_client->second.readRequest();
								}
								catch (std::exception &e) {
									it->getClients().erase(event_fd);
									close(event_fd);
									break;
								}
							}
							if (events & EPOLLOUT && it_client->second.isReady())
							{
								it->sendResponse(it_client->second.getRequest(), it_client->first);
							}
						}
					}
				}
			}
		}
	}
	std::cout << "\b\bSignal received, closing server.." << std::endl;
}

std::vector<Socket> Server::_parseServers(std::string content)
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
				else if (word == "server{"){
					Socket tmp(iss, word);
					sockets.push_back(tmp);
				}
				else
					throw InvalidConfigFile();
				break;

			case conf_server_brace:
				if (word == "{") {
					Socket tmp(iss, word);
					sockets.push_back(tmp);
					state = conf_server;
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
