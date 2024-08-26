/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wateecuhs <wateecuhs@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:49:25 by panger            #+#    #+#             */
/*   Updated: 2024/08/25 20:00:40 by wateecuhs        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/epoll.h>
#include "VirtualServer.hpp"
#include "Socket.hpp"
#include <vector>

class Socket;

class Server
{
	private:
		std::vector<Socket>			_sockets;
		int							_epoll_fd;
		std::vector<epoll_event>	_epoll_events;
		void						_parseServers(std::string content);
		Server();

	public:
		Server(std::string config);
		~Server();
		Server(const Server &copy);
		Server &operator=(const Server &copy);
		void								addSocket(Socket socket);
		std::vector<Socket>					getSockets() const;
		void								setEpollFd(int epoll_fd);
		int									getEpollFd() const;
		void								startServer();
		void								listenSockets();
		void								addVirtualServer(VirtualServer VirtualServer);
};

#endif