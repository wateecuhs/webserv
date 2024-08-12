/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/09 15:48:56 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "enums.hpp"
#include "Location.hpp"
#include "Socket.hpp"
#include <map>
#include <iostream>
#include <exception>

class Location;
class Socket;

class Request {
	private:
		int									_confd;
		Methods								_method;
		std::string							_path;
		bool								_pathIsDirectory;
		float								_http_version;
		std::string							_host;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		std::string							_query;

		Location							*_location;
		Socket								&_socket;
		Request();

	public:
		Request(std::string request, Socket &socket, int confd);
		Request(Request &src);
		Request &operator=(Request &src);
		Request(const Request &src);
		~Request();
		Methods								getMethod() const;
		std::string							getMethodString() const;
		void								setMethod(Methods method);
		std::string							getPath() const;
		void								setPath(std::string path);
		void								setHTTPVersion(float version);
		float								getHTTPVersion() const;
		void								addHeader(std::string key, std::string value);
		void								setHeaders(std::map<std::string, std::string>);
		std::map<std::string, std::string>	getHeaders() const;
		void								setBody(std::string body);
		std::string							getBody() const;
		void								setHost(std::string host);
		std::string							getHost() const;
		void								setQuery(std::string query);
		std::string							getQuery() const;
		void								setLocation(Location *location);
		Location							*getLocation() const;
		Socket								&getSocket() const;
		int									getConfd() const;
		void								setConfd(int confd);

		bool								pathIsDirectory() const;
};

#endif
