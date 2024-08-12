/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:49:25 by panger            #+#    #+#             */
/*   Updated: 2024/08/07 16:15:55 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include "parsing.hpp"
#include "Request.hpp"
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

void methodHandler(Request& request);

// void handleNewClients()
// {
// 	for (std::vector<Socket>::iterator it = servers.begin(); it != servers.end(); it++) {
// 		if (it->getFd() == event_fd) {
// 			client_socket = accept(event_fd, (sockaddr *)NULL, (socklen_t *)NULL);
// 			if (client_socket == -1) {
// 				perror("accept");
// 				continue;
// 			}
// 			client_event.data.fd = client_socket;
// 			client_event.events = EPOLLIN | EPOLLOUT;
// 			if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &client_event) == -1) {
// 				perror("epoll_ctl: client_socket");
// 				close(client_socket);
// 				continue;
// 			}
// 			client_sockets.push_back(client_socket);
// 			break;
// 		}
// 	}
// }

void startSockets(std::vector<Socket> servers)
{
	int									epfd;
	epoll_event							ep_events[servers.size() * 5];
	int									client_socket;
	int									triggered_events;
	char								buf[2048] = {0};
	std::vector< std::pair<int, bool> >	client_sockets;
	std::vector<Request>				requests;
	epoll_event							client_event;
	int									event_fd;

	epfd = epoll_create(servers.size());
	for (std::vector<Socket>::iterator it = servers.begin(); it != servers.end(); it++)
		it->startListening(epfd, ep_events[it - servers.begin()]);
	while (true)
	{
		triggered_events = epoll_wait(epfd, ep_events, servers.size(), 0);
		for (int i = 0; i < triggered_events; i++) {
			event_fd = ep_events[i].data.fd;
			uint32_t events = ep_events[i].events;

			for (std::vector<Socket>::iterator it = servers.begin(); it != servers.end(); it++) {
				if (it->getFd() == event_fd) {
					client_socket = accept(event_fd, (sockaddr *)NULL, (socklen_t *)NULL);
					if (client_socket == -1) {
						perror("accept");
						continue;
					}
					client_event.data.fd = client_socket;
					client_event.events = EPOLLIN | EPOLLOUT;
					if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &client_event) == -1) {
						perror("epoll_ctl: client_socket");
						close(client_socket);
						continue;
					}
					client_sockets.push_back(std::pair<int, bool>(client_socket, false));
					continue;
				}
			}
			for (std::vector<std::pair<int, bool> >::iterator it = client_sockets.begin(); it != client_sockets.end(); it++) {
				if (it->first == event_fd) {
					if (events & EPOLLIN) {
						recv(event_fd, buf, sizeof(buf), 0);
						try {
							for (std::vector<Socket>::iterator itt = servers.begin(); itt != servers.end(); itt++)
							{
								if (itt->getFd() == ep_events[itt - servers.begin()].data.fd)
								{
									requests.push_back(Request(buf, *itt, it->first));
									break;
								}
							}
						}
						catch (const std::exception &e) {
							send(event_fd, e.what(), strlen(e.what()), 0);
							close(event_fd);
							std::cout << e.what() << std::endl;
						}
					}
					else if (events & EPOLLOUT) {
						for (std::vector<Request>::iterator itt = requests.begin(); itt != requests.end(); itt++) {
							if (itt->getConfd() == event_fd) {
								try {
									methodHandler(*itt);
								}
								catch (const std::exception &e) {
									send(event_fd, e.what(), strlen(e.what()), 0);
									close(event_fd);
								}
								requests.erase(itt);
								break;
							}
						}
					}
				}
			}
		}
	}
}



int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return 1;
	}

	std::vector<Socket> servers;
	try {
		servers = parseConfig(argv[1]);
		startSockets(servers);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

}