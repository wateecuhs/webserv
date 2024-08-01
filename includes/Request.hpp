/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:25:49 by panger            #+#    #+#             */
/*   Updated: 2024/08/01 14:23:26 by alermolo         ###   ########.fr       */
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
		Methods								_method;
		std::string							_path;
		bool								_pathIsDirectory;
		float								_http_version;
		std::string							_host;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		std::string							_query;

		Request();
	public:
		Request(Request &src);
		Request(std::string request);
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

		bool								pathIsDirectory() const;
};

class BadRequest: public std::exception {
	public:
		virtual const char *what() const throw();
};

#endif
