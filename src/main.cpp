/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:49:25 by panger            #+#    #+#             */
/*   Updated: 2024/07/29 14:25:24 by panger           ###   ########.fr       */
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

void methodHandler(const Request& request, int socket);

int	init_socket(int epfd, unsigned int port, epoll_event &ep_event)
{
	int	socketfd = socket(AF_INET, SOCK_STREAM, 0);

	// Supposed to make the socket reusable instantly after the program closes but doesnt work
	setsockopt(socketfd, IPPROTO_TCP, SO_REUSEADDR, (void *)1, 1);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_aton("127.0.0.1", &server_addr.sin_addr);

	bind(socketfd, (sockaddr *)&server_addr, sizeof(server_addr));

	ep_event.data.fd = socketfd;
	ep_event.events = EPOLLIN | EPOLLPRI;
	epoll_ctl(epfd, EPOLL_CTL_ADD, socketfd, &ep_event);

	return (socketfd);
}

int main(int argc, char **argv)
{
	(void)argv, (void)argc;

	int			fds[2];
	char		buf[2048] = {0};
	int			client_socket;
	int			epfd;
	epoll_event	ep_events[2];
	int			triggered_events;

	epfd = epoll_create(1);
	fds[0] = init_socket(epfd, 3000, ep_events[0]);
	fds[1] = init_socket(epfd, 8000, ep_events[1]);
	listen(fds[0], 5);
	listen(fds[1], 5);
	while (true)
	{
		triggered_events = epoll_wait(epfd, ep_events, 2, 0);
		if (triggered_events > 0)
		{
			client_socket = -1;
			for (int i = 0; i < triggered_events; i++)
				client_socket = accept(ep_events[i].data.fd, (sockaddr *)NULL, (socklen_t *)NULL);
			if (client_socket != -1)
			{
				recv(client_socket, buf, sizeof(buf), 0);
				try {
					std::cout << std::endl << buf << std::endl;
					Request rq(buf);
					methodHandler(rq, client_socket);
				}
				catch (const std::exception &e) {
					send(client_socket, e.what(), strlen(e.what()), 0);
					close(client_socket);
					std::cout << e.what() << std::endl;
				}
			}
		}
	}
}
