/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:29:36 by panger            #+#    #+#             */
/*   Updated: 2024/08/21 16:10:55 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <string>
#include <map>
#include <vector>
#include "Location.hpp"
#include <sys/epoll.h>
#include "Client.hpp"

// class Location;

class Socket
{
	private:
		int									_fd;
		std::string							_host;
		int									_port;
		std::map<int, std::string>			_error_pages;
		std::vector<std::string>			_server_names;
		size_t								_body_size;
		std::vector<Location>				_locations;
		int									_epoll_fd;
		struct epoll_event					_event;
		struct epoll_event					_events[10];
		std::map<int, Client>				_clients;
		void								_methodHandler(Request& request, int client_fd);
		void								_handleDeleteRequest(Request &request, Location *location, int client_fd);
		void								_handleGetRequest(Request &request, Location *location, int client_fd);
		void								_handlePostRequest(Request &request, Location *location, int client_fd);
		void								_handleUpload(Request &request, Location *location, int client_fd);
		std::string							_execCGI(Request &request, Location *location);
		void								_handleCGI(Request &request, Location *location, int client_fd);
		Socket();
	public:
		Socket(std::stringstream &iss, std::string word);
		~Socket();
		Socket(const Socket &copy);
		Socket &operator=(const Socket &copy);

		void								setFd(int fd);
		int									getFd() const;
		void								addServerName(std::string name);
		std::vector<std::string>			getServerNames() const;
		void								setHost(std::string host);
		std::string							getHost() const;
		void								setPort(int port);
		int									getPort() const;
		void								addErrorPage(int error_code, std::string path);
		std::map<int, std::string>			getErrorPages() const;
		void								setBodySize(int size);
		int									getBodySize() const;
		void								addLocation(Location location);
		std::vector<Location>				getLocations() const;
		int									startListening(int epfd);
		void								httpListen();
		int									acceptConnection(int event_fd);
		std::map<int, Client>				&getClients();
		void								sendResponse(Request request, int client_fd);
};

#endif
