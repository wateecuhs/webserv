/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:49:25 by panger            #+#    #+#             */
/*   Updated: 2024/07/22 15:14:24 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include "parsing.hpp"
#include <netinet/in.h>
#include <poll.h>
#include <sys/epoll.h>

int	init_socket(unsigned int port)
{
	int	socketfd = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	bind(socketfd, (sockaddr *)&server_addr, sizeof(server_addr));
	return (socketfd);
}

int main(int argc, char **argv)
{
	(void)argv, (void)argc;

	int			fds[2];
	char		buf[2048];
	int			client_socket;
	int			epfd;
	epoll_event	ep_events[2];
	int			triggered_events;

	fds[0] = init_socket(3000);
	fds[1] = init_socket(8000);
	ep_events[0].data.fd = fds[0];
	ep_events[0].events = EPOLLIN | EPOLLPRI;
	ep_events[1].data.fd = fds[1];
	ep_events[1].events = EPOLLIN | EPOLLPRI;
	listen(fds[0], 5);
	listen(fds[1], 5);
	epfd = epoll_create(1);
	epoll_ctl(epfd, EPOLL_CTL_ADD, fds[0], &ep_events[0]);
	epoll_ctl(epfd, EPOLL_CTL_ADD, fds[1], &ep_events[1]);
	while (true)
	{
		triggered_events = epoll_wait(epfd, ep_events, 2, 0);
		if (triggered_events > 0)
		{
			client_socket = -1;
			for (int i = 0; i < triggered_events; i++)
			{
				client_socket = accept(ep_events[i].data.fd, (sockaddr *)NULL, (socklen_t *)NULL);
			}
			if (client_socket != -1)
			{
				recv(client_socket, buf, sizeof(buf), 0);
				std::cout << "Message from client:" << std::endl << buf;
				send(client_socket, "SALUT\n", 6, 0);
			}
		}
	}
}
