/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:49:25 by panger            #+#    #+#             */
/*   Updated: 2024/08/07 16:15:55 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/epoll.h>
#include "Socket.hpp"
#include <vector>

class Server
{
	private:
		std::vector<Socket>			_sockets;
		int							_epoll_fd;
		std::vector<epoll_event>	_epoll_events;
		std::vector<Socket>			_parseServers(std::string content);
		Server();

	public:
		Server(std::string config);
		~Server();
		Server(const Server &copy);
		Server &operator=(const Server &copy);
		void					addSocket(Socket socket);
		std::vector<Socket>		getSockets() const;
		void					setEpollFd(int epoll_fd);
		int						getEpollFd() const;
		void					startServer();
		void					listenSockets();
};

#endif