/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/24 17:30:01 by alermolo         ###   ########.fr       */
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
		std::map<std::string, std::string>	_cookies;
		bool								_hasCookies;
		std::string							_response;

		void								_parseRequestLine(std::string r);
		bool								_isHTTP(std::string r, int pos, int len);
		std::string							_parseURI(std::string r);
		float								_parseVersion(std::string r, int pos);
		Methods								_parseMethod(std::string r, int len);
		void								_parseCookies(std::string cookies);
		void								_parseHeaders(std::string headers);
		void								_parseChunkedBody(const std::string &body);

	public:
		Request();
		Request(std::string request);
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
		bool								pathIsDirectory() const;
		bool								hasCookies() const;
		void								setHasCookies(bool hasCookies);
		std::map<std::string, std::string>	getCookies() const;
		std::string							getCookie(std::string key) const;
		void								setCookie(std::string key, std::string value);
		std::string							getResponse() const;
		void								setResponse(std::string status, std::string content);
		void								setResponse(std::string status, std::pair<std::string, std::string> header, std::string content);
};

#endif
