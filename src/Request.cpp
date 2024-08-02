/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:25:42 by panger            #+#    #+#             */
/*   Updated: 2024/08/02 17:12:31 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "parsing.hpp"
#include <sstream>
#include <cstring>
#include <sys/stat.h>
#include "exceptions.hpp"

Request::Request() {}

Request::Request(std::string request)
{
	size_t headers_start;
	size_t headers_end;
	headers_start = request.find("\r\n");
	if (headers_start == std::string::npos)
		throw BadRequest();
	parseRequestLine(request.substr(0, headers_start), *this);

	std::cout << "Method found -> " << this->_method << std::endl;
	std::cout << "Path found -> " << this->_path << std::endl;
	std::cout << "Version found -> " << this->_http_version << std::endl;

	headers_start += 2;
	headers_end = request.find("\r\n\r\n");
	if (headers_start == std::string::npos || headers_end == std::string::npos)
		throw BadRequest();
	parseHeaders(request.substr(headers_start, headers_end), *this);

	std::cout << "Headers found -> " << std::endl;
	for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
	setHost(this->_headers["Host"]);
	setBody(request.substr(headers_end + 4));
	std::cout << "Body found -> " << this->_body << std::endl;
}

Request::Request(Request &src)
{
	*this = src;
}

Request::~Request() {}

void Request::setMethod(Methods method)
{
	this->_method = method;
}

Methods Request::getMethod() const
{
	return this->_method;
}

std::string	Request::getMethodString() const
{
	switch (this->_method)
	{
		case GET:
			return "GET";
		case POST:
			return "POST";
		case DELETE:
			return "DELETE";
		default:
			return "UNKNOWN";
	}
}

void Request::setPath(std::string path)
{
	this->_path = path;
	this->_pathIsDirectory = false;
	struct stat s;

	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
			this->_pathIsDirectory = true;
		else
			this->_pathIsDirectory = false;
	}
}

bool Request::pathIsDirectory() const
{
	return this->_pathIsDirectory;
}

std::string Request::getPath() const
{
	return this->_path;
}

void Request::setHTTPVersion(float version)
{
	this->_http_version = version;
}

float Request::getHTTPVersion() const
{
	return this->_http_version;
}

void Request::addHeader(std::string key, std::string value)
{
	this->_headers[key] = value;
}

void Request::setHeaders(std::map<std::string, std::string> headers)
{
	this->_headers = headers;
}

std::map<std::string, std::string>	Request::getHeaders() const
{
	return this->_headers;
}

void Request::setBody(std::string body)
{
	this->_body = body;
}

std::string Request::getBody() const
{
	return this->_body;
}

void Request::setHost(std::string host)
{
	this->_host = host;
}

std::string Request::getHost() const
{
	return this->_host;
}

void Request::setQuery(std::string query)
{
	this->_query = query;
}

std::string Request::getQuery() const
{
	return this->_query;
}