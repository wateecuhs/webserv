/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:25:42 by panger            #+#    #+#             */
/*   Updated: 2024/08/21 16:59:11 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <sstream>
#include <cstring>
#include <sys/stat.h>
#include "exceptions.hpp"
#include "utils.hpp"
#include <cstdlib>

Request::Request() {}

Request::Request(std::string request)
{
	size_t					headers_start;
	size_t					headers_end;

	headers_start = request.find("\r\n");
	headers_end = request.find("\r\n\r\n");
	if (headers_start == std::string::npos)
		throw BadRequest();
	headers_start += 2;
	if (headers_start == std::string::npos || headers_end == std::string::npos)
		throw BadRequest();

	_parseRequestLine(request.substr(0, headers_start));
	_parseHeaders(request.substr(headers_start, headers_end));
	setHost(this->_headers["Host"]);

	setBody(request.substr(headers_end + 4));
}

Request::~Request()
{
}

Request::Request(Request &src)
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
	this->_cookies = src.getCookies();
	this->_hasCookies = src.hasCookies();
	this->_response = src.getResponse();
	return *this;
}

Request &Request::operator=(const Request &src)
{
	this->_method = src.getMethod();
	this->_path = src.getPath();
	this->_pathIsDirectory = src.pathIsDirectory();
	this->_http_version = src.getHTTPVersion();
	this->_headers = src.getHeaders();
	this->_body = src.getBody();
	this->_host = src.getHost();
	this->_query = src.getQuery();
	this->_cookies = src.getCookies();
	this->_hasCookies = src.hasCookies();
	this->_response = src.getResponse();
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

Request::Request(const Request &src)
{
	this->_method = src.getMethod();
	this->_path = src.getPath();
	this->_pathIsDirectory = src.pathIsDirectory();
	this->_http_version = src.getHTTPVersion();
	this->_headers = src.getHeaders();
	this->_body = src.getBody();
	this->_host = src.getHost();
	this->_query = src.getQuery();
	this->_cookies = src.getCookies();
	this->_hasCookies = src.hasCookies();
	this->_response = src.getResponse();
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

bool Request::hasCookies() const
{
	return this->_hasCookies;
}

void Request::setHasCookies(bool hasCookies)
{
	this->_hasCookies = hasCookies;
}

std::map<std::string, std::string> Request::getCookies() const
{
	return this->_cookies;
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
		response = response.substr(0, response.size() - 2);
	}
	response += "\r\nContent-Length: " + strSizeToStr(content) + "\r\n\r\n" + content;
	this->_response = response;
}

void Request::setResponse(std::string status, std::pair<std::string, std::string> header, std::string content)
{
	std::string response = "HTTP/1.1 " + status;

	if (this->hasCookies())
	{
		response += "\r\nSet-Cookie: ";
		for (std::map<std::string, std::string>::iterator it = this->_cookies.begin(); it != this->_cookies.end(); it++)
			response += it->first + "=" + it->second + "; ";
		response = response.substr(0, response.size() - 2);
	}
	response += "\r\n" + header.first + ": " + header.second;
	response += "\r\nContent-Length: " + strSizeToStr(content) + "\r\n\r\n" + content;
	this->_response = response;
}

void Request::_parseRequestLine(std::string r)
{
	ReqState state = req_start;
	std::string::iterator cur_begin;

	for (std::string::iterator it = r.begin(); it != r.end(); it++)
	{
		switch (state) {
			case req_start:
				if (*it == '\r' || *it == '\n')
					break;
				if (*it < 'A' || *it > 'Z')
					throw BadRequest();
				state = req_method;
				cur_begin = it;
				break;
			case req_method:
				if (*it == ' ') {
					setMethod(_parseMethod(r, it - cur_begin));
					state = req_after_method;
				}
				break;
			case req_after_method:
				if (*it == '/') {
					state = req_uri;
					cur_begin = it;
					break;
				}
				else
					throw BadRequest();
			case req_uri:
				if (*it == ' ') {
					setPath(_parseURI(r.substr(cur_begin - r.begin(), it - cur_begin)));
					state = req_after_uri;
				}
				break;
			case req_after_uri:
				if (*it == 'H') {
					cur_begin = it;
					state = req_http_HTTP;
					break;
				}
				throw BadRequest();
			case req_http_HTTP:
				if (*it == 'T' || *it == 'P')
					break;
				if (*it == '/') {
					if (!_isHTTP(r, cur_begin - r.begin(), it - cur_begin + 1))
						throw BadRequest();
					state = req_http_version;
					break;
				}
				throw BadRequest();
			case req_http_version:
				setHTTPVersion(_parseVersion(r, it - r.begin()));
				state = req_after_version;
				it++;
				it++;
				break;
			case req_after_version:
				if (*it == '\r' && *(it + 1) == '\n')
					return;
				throw BadRequest();
			default:
				break;
		}
	}
}


Methods Request::_parseMethod(std::string r, int len)
{
	switch (len) {
		case 3:
			if (r.compare(0, 3, "GET") == 0)
				return GET;
			throw BadRequest();
		case 4:
			if (r.compare(0, 4, "POST") == 0)
				return POST;
			throw BadRequest();
		case 6:
			if (r.compare(0, 6, "DELETE") == 0)
				return DELETE;
			throw BadRequest();
		default:
			throw BadRequest();
	}
}

float Request::_parseVersion(std::string r, int pos)
{
	float version;

	if (!(isdigit(r[pos]) && r[pos + 1] == '.' && isdigit(r[pos + 2]))) {
		throw BadRequest();
	}
	std::string v = r.substr(pos, 3);
	version = strtof(v.c_str(), NULL);
	if (version != 1.1f && version != 1.0f && version != 0.9f)
		throw BadRequest();
	return version;
}

std::string Request::_parseURI(std::string r)
{
	if (r.find('?') != std::string::npos)
	{
		size_t pos = r.find('?');
		setQuery(r.substr(pos + 1));
		return r.substr(0, pos);
	}
	return r;
}

bool Request::_isHTTP(std::string r, int pos, int len)
{
	if (r.compare(pos, len, "HTTP/") == 0)
		return true;
	return false;
}

void Request::_parseCookies(std::string cookies)
{
	size_t start = 0;
	size_t end = cookies.find("; ");
	while (end != std::string::npos && end != start)
	{
		std::string cookie = cookies.substr(start, end - start);
		size_t sep = cookie.find("=");
		if (sep == std::string::npos)
			throw BadRequest();
		std::string key = cookie.substr(0, sep);
		std::string value = cookie.substr(sep + 1);
		setCookie(key, value);
		start = end + 2;
		end = cookies.find("; ", start);
	}
	setHasCookies(true);
}

void Request::_parseHeaders(std::string headers)
{
	size_t start = 0;
	size_t end = headers.find("\r\n");
	while (end != std::string::npos && end != start)
	{
		std::string header = headers.substr(start, end - start);
		size_t sep = header.find(": ");
		if (sep == std::string::npos)
			throw BadRequest();
		std::string key = header.substr(0, sep);
		std::string value = header.substr(sep + 2);
		if (key == "Cookie")
			_parseCookies(value);
		else
			addHeader(key, value);
		start = end + 2;
		end = headers.find("\r\n", start);
	}
}