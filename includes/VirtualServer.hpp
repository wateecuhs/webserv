/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wateecuhs <wateecuhs@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:29:36 by panger            #+#    #+#             */
/*   Updated: 2024/08/25 20:11:59 by wateecuhs        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALSERVER_HPP
# define VIRTUALSERVER_HPP

#include <string>
#include <map>
#include <vector>
#include "Location.hpp"
#include <sys/epoll.h>
#include "Client.hpp"

class Location;

class VirtualServer
{
	private:
		std::string							_host;
		int									_port;
		std::map<int, std::string>			_error_pages;
		std::vector<std::string>			_server_names;
		size_t								_body_size;
		std::vector<Location>				_locations;
		void								_methodHandler(Request& request, int client_fd);
		void								_handleDeleteRequest(Request &request, Location *location, int client_fd);
		void								_handleGetRequest(Request &request, Location *location, int client_fd);
		void								_handlePostRequest(Request &request, Location *location, int client_fd);
		void								_handleUpload(Request &request, Location *location, int client_fd);
		std::string							_execCGI(Request &request, Location *location);
		void								_handleCGI(Request &request, Location *location, int client_fd);
		VirtualServer();
	public:
		VirtualServer(std::stringstream &iss, std::string word);
		~VirtualServer();
		VirtualServer(const VirtualServer &copy);
		VirtualServer &operator=(const VirtualServer &copy);

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

		void								sendResponse(Request request, int client_fd);
};

#endif
