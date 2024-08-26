/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wateecuhs <wateecuhs@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:29:36 by panger            #+#    #+#             */
/*   Updated: 2024/08/25 20:14:15 by wateecuhs        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <string>
#include <vector>
#include "VirtualServer.hpp"
#include "Request.hpp"

class VirtualServer;
class Request;

class Socket
{
	private:
		int									_fd;
		std::string							_host;
		int									_port;
		std::vector<VirtualServer>			_virtual_servers;
		std::map<int, Client>				_clients;
		Socket();
	public:
		int									startListening(int epfd);
		void 								sendResponse(Request request, int client_fd);
		int									acceptConnection(int event_fd);
		std::map<int, Client>				&getClientsRef();
		std::map<int, Client>				getClients() const;
		Socket(std::string host, int port);
		~Socket();
		Socket(const Socket &copy);
		Socket &operator=(const Socket &copy);

		void								setFd(int fd);
		int									getFd() const;
		void								setHost(std::string host);
		std::string							getHost() const;
		void								setPort(int port);
		int									getPort() const;
		void								addVirtualServer(VirtualServer VirtualServer);
		std::vector<VirtualServer>			getVirtualServers() const;
		
};

#endif