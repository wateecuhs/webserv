/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:25:42 by panger            #+#    #+#             */
/*   Updated: 2024/08/09 17:52:59 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "parsing.hpp"
#include <sstream>
#include <cstring>
#include <sys/stat.h>
#include "exceptions.hpp"
#include "utils.hpp"

Request::Request(std::string request, Socket &socket): _socket(socket)
{
	size_t					headers_start;
	size_t					headers_end;
	size_t					longest_length = 0;
	std::vector<Location>	locations;
	std::string				tmp_path;

	this->_location = NULL;
	headers_start = request.find("\r\n");
	headers_end = request.find("\r\n\r\n");
	if (headers_start == std::string::npos)
		throw BadRequest();
	headers_start += 2;
	if (headers_start == std::string::npos || headers_end == std::string::npos)
		throw BadRequest();

	parseRequestLine(request.substr(0, headers_start), *this);
	parseHeaders(request.substr(headers_start, headers_end), *this);
	setHost(this->_headers["Host"]);

	locations = this->_socket.getLocations();
	for (size_t i = 0; i < locations.size(); i++)
	{
		tmp_path = locations[i].getPath();
		if (this->_path.rfind(tmp_path, 0) == 0 && tmp_path.length() > longest_length)
		{
			longest_length = tmp_path.length();
			this->_location = new Location(locations[i]);
		}
	}
	setBody(request.substr(headers_end + 4));
}

Request::~Request()
{
	if (this->_location)
		delete this->_location;
}

Request::Request(Request &src): _socket(src._socket)
{
	*this = src;
}

Request &Request::operator=(Request &src)
{
	this->_method = src.getMethod();
	this->_path = src.getPath();
	this->_pathIsDirectory = src.pathIsDirectory();
	this->_http_version = src.getHTTPVersion();
	this->_headers = src.getHeaders();
	this->_body = src.getBody();
	this->_host = src.getHost();
	this->_query = src.getQuery();
	this->_hasCookies = src.hasCookies();
	this->_cookies = src._cookies;
	if (this->_location)
		delete this->_location;
	this->_location = new Location(*src.getLocation());
	return *this;
}

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

void Request::setLocation(Location *location)
{
	this->_location = location;
}

Location *Request::getLocation() const
{
	return this->_location;
}

Socket &Request::getSocket() const
{
	return this->_socket;
}

bool Request::hasCookies() const
{
	return this->_hasCookies;
}

void Request::setHasCookies(bool hasCookies)
{
	this->_hasCookies = hasCookies;
}

std::string Request::getCookie(std::string key) const
{
	if (this->_cookies.find(key) == this->_cookies.end())
		return "";
	return this->_cookies.at(key);
}

void Request::setCookie(std::string key, std::string value)
{
	this->_cookies[key] = value;
	this->_hasCookies = true;
}

std::string Request::getResponse() const
{
	return this->_response;
}

void Request::setResponse(std::string status, std::string content)
{
	std::string response = "HTTP/1.1 " + status;

	if (this->hasCookies())
	{
		response += "\r\nSet-Cookie: ";
		for (std::map<std::string, std::string>::iterator it = this->_cookies.begin(); it != this->_cookies.end(); it++)
			response += it->first + "=" + it->second + "; ";
		response.pop_back();
	}
	response += "\r\nContent-Length: " + strSizeToStr(content) + "\r\n\r\n" + content;
	this->_response = response;
}
