/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:25:49 by panger            #+#    #+#             */
/*   Updated: 2024/07/29 11:02:27 by panger           ###   ########.fr       */
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
		float								_http_version;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		std::string							_host;

		Request();
	public:
		Request(Request &src);
		Request(std::string request);
		~Request();
		Methods								getMethod() const;
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

};

class BadRequest: public std::exception {
	public:
		virtual const char *what() const throw() {
			return "400: Bad Request";
		}
};

#endif