/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waticouz <waticouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:49:25 by panger            #+#    #+#             */
/*   Updated: 2024/08/07 15:41:44 by waticouz         ###   ########.fr       */
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

void	methodHandler(Request &request, const Socket &socket);
void	handleCGI(const Request &request, const Socket &socket);

void startSockets(std::vector<Socket> servers)
{
	int			epfd;
	epoll_event	ep_events[servers.size()];
	int			client_socket;
	int			triggered_events;
	char		buf[2048] = {0};

	for (std::vector<Socket>::iterator it = servers.begin(); it != servers.end(); it++)
		it->startListening();
	while (true)
	{
		// triggered_events = epoll_wait(epfd, ep_events, servers.size(), 50);
		// std::cout << "triggered_events: " << triggered_events << std::endl;
		// if (triggered_events > 0)
		// {
		// 	client_socket = accept(ep_events[0].data.fd, (sockaddr *)NULL, (socklen_t *)NULL);
		// 	if (client_socket != -1)
		// 	{
		// 		recv(client_socket, buf, sizeof(buf), 0);
		// 		try {
		// 			std::cout << std::endl << buf << std::endl;
		// 			for (std::vector<Socket>::iterator it = servers.begin(); it != servers.end(); it++)
		// 			{
		// 				if (it->getFd() == ep_events[it - servers.begin()].data.fd)
		// 				{
		// 					Request rq(buf, *it);
		// 					break;
		// 				}
		// 			}
		// 		}
		// 		catch (const std::exception &e) {
		// 			send(client_socket, e.what(), strlen(e.what()), 0);
		// 			close(client_socket);
		// 			std::cout << e.what() << std::endl;
		// 		}
		// 	}
		// }
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