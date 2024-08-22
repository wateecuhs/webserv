/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:49:25 by panger            #+#    #+#             */
/*   Updated: 2024/08/21 11:02:54 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include "Request.hpp"
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include "Server.hpp"

void methodHandler(Request& request);


int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return 1;
	}

	try {
		Server server(argv[1]);
		server.startServer();
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

}