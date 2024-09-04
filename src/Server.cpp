/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wateecuhs <wateecuhs@student.42.fr>        +#+  +:+       +#+        */
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
#include <sys/socket.h>

int sig = 0;

Server::Server(std::string config): _epoll_events(20)
{
	std::ifstream	cfstream;

	cfstream.open(config.c_str());
	if (!cfstream.is_open())
		throw InvalidConfigFile();
	std::string content((std::istreambuf_iterator<char>(cfstream)),
						 std::istreambuf_iterator<char>());
	_parseServers(content);
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
		this->_epoll_events = copy._epoll_events;
		this->_sockets = copy.getSockets();
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
	try {
		for (std::vector<Socket>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++)
			it->startListening(this->_epoll_fd);
	}
	catch (std::exception &e) {
		for (std::vector<Socket>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++) {
			if (it->getFd() != -1)
				close(it->getFd());
		}
		if (this->_epoll_fd != -1)
			close(this->_epoll_fd);
		throw ;
	}
}

void sigHandler(int sig_code)
{
	sig = sig_code;
}

void Server::startServer()
{
	std::map<int, Client>			clients;
	struct sigaction				action;
	std::map<int, Client>::iterator	tmp;
	Client							client;
	int								nfds = 0;
	int								event_fd = 0;

	std::memset(&action, 0, sizeof(action));
	action.sa_handler = sigHandler;
	action.sa_flags = SA_SIGINFO;
	if (sigaction(SIGINT, &action, NULL))
		throw std::runtime_error("Initializing signal handler failed");

	this->listenSockets();
	while (sig == 0)
	{
		nfds = epoll_wait(_epoll_fd, _epoll_events.data(), _epoll_events.size(), 0);
		for (int i = 0; i < nfds; i++) {
			event_fd = this->_epoll_events[i].data.fd;
			uint32_t events = this->_epoll_events[i].events;

			for (std::vector<Socket>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++) {
				for (std::map<int, Client>::iterator itc = clients.begin(); itc != clients.end(); itc++) {
					if (itc->second.isTimedOut()) {
						close(itc->first);
						it->getClientsRef().erase(itc);
					}
				}
				if (it->getFd() == event_fd) {
					epoll_event _event;
					int client_socket = it->acceptConnection(event_fd);
					_event.data.fd = client_socket;
					_event.events = EPOLLIN | EPOLLOUT;
					epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_socket, &_event);
					break;
				}
				clients = it->getClients();
				if (clients.find(event_fd) == clients.end())
					continue;
				client = clients.find(event_fd)->second;
				if (events & EPOLLIN) {
					try {
						if (client.readRequest() == 0) {
							close(event_fd);
							it->getClientsRef().erase(event_fd);
							continue;
						}
					}
					catch (std::exception &e) {
						std::cout << "HTTP/1.1 400 Bad Request" << std::endl;
						send(event_fd, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n", 45, 0);
						close(event_fd);
						it->getClientsRef().erase(event_fd);
						continue;
					}
				}
				if (events & EPOLLOUT && client.isReady()) {
					try {
						it->sendResponse(client.getRequest(), event_fd);
						client.setReady(false);
					}
					catch (std::exception &e){
						if (std::string(e.what()) == "Exit cleanup") {
							for (std::vector<Socket>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++) {
								std::map<int, Client> clients = it->getClients();
								for (std::map<int, Client>::iterator itc = clients.begin(); itc != clients.end(); itc++)
									close(itc->first);
								close(it->getFd());
							}
							close(this->_epoll_fd);
							return ;
						}
					}
				}
			}
		}
	}
	std::cout << "\b\bSignal received, closing server.." << std::endl;
	for (std::vector<Socket>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++) {
		std::map<int, Client> clients = it->getClients();
		for (std::map<int, Client>::iterator itc = clients.begin(); itc != clients.end(); itc++)
			close(itc->first);
		close(it->getFd());
	}
	close(this->_epoll_fd);
}

void Server::_parseServers(std::string content)
{
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
					VirtualServer tmp(iss, word);
					this->addVirtualServer(tmp);
				}
				else
					throw InvalidConfigFile();
				break;

			case conf_server_brace:
				if (word == "{") {
					VirtualServer tmp(iss, word);
					this->addVirtualServer(tmp);
					state = conf_server;
				}
				else {
					throw InvalidConfigFile();
				}
				break;
			default:
				throw InvalidConfigFile();
		}
	}
	if (this->_sockets.empty())
		throw InvalidConfigFile();
}

void Server::addVirtualServer(VirtualServer VirtualServer)
{
	for (std::vector<Socket>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++) {
		if (it->getHost() == VirtualServer.getHost() && it->getPort() == VirtualServer.getPort())
		{
			it->addVirtualServer(VirtualServer);
			return ;
		}
	}
	Socket socket(VirtualServer.getHost(), VirtualServer.getPort());
	socket.addVirtualServer(VirtualServer);
	this->_sockets.push_back(socket);
}
