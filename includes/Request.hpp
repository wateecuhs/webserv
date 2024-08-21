/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/21 15:33:52 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "enums.hpp"
#include <map>
#include <iostream>
#include <exception>

class Request {
	private:
		// int									_confd;
		Methods								_method;
		std::string							_path;
		bool								_pathIsDirectory;
		float								_http_version;
		std::string							_host;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		std::string							_query;
		std::map<std::string, std::string>	_cookies;
		bool								_hasCookies;
		std::string							_response;

		// Location							*_location;
		// Socket								&_socket;

	public:
		Request();
		Request(std::string request);
		// Request(std::string request, Socket &socket, int confd);
		Request(Request &src);
		Request &operator=(Request &src);
		Request &operator=(const Request &src);
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
		// void								setLocation(Location *location);
		// Location							*getLocation() const;
		// Socket								&getSocket() const;
		// int									getConfd() const;
		// void								setConfd(int confd);

		bool								pathIsDirectory() const;
		bool								hasCookies() const;
		void								setHasCookies(bool hasCookies);
		std::map<std::string, std::string>	getCookies() const;
		std::string							getCookie(std::string key) const;
		void								setCookie(std::string key, std::string value);
		std::string							getResponse() const;
		void								setResponse(std::string status, std::string content);
};

#endif
