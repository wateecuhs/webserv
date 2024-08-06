/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waticouz <waticouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:29:36 by panger            #+#    #+#             */
/*   Updated: 2024/08/06 12:16:57 by waticouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <string>
#include <map>
#include <vector>
#include "Location.hpp"
#include <sys/epoll.h>

class Location;

class Socket
{
	private:
		int									_fd;
		std::string							_host;
		int									_port;
		std::map<int, std::string>			_error_pages;
		std::vector<std::string>			_server_names;
		int									_body_size;
		std::vector<Location>				_locations;
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
		int 								startListening(int epfd, epoll_event &ep_event);
};

#endif
