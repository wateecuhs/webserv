/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:25:42 by panger            #+#    #+#             */
/*   Updated: 2024/07/25 17:33:42 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "parsing.hpp"
#include <sstream>
#include <cstring>

Request::Request() {}

Request::Request(std::string request)
{
	parse_request_line(request, *this);
	std::cout << "Method found -> " << this->_method << std::endl;
	std::cout << "Path found -> " << this->_path << std::endl;
	std::cout << "Version found -> " << this->_http_version << std::endl;
	std::cout << std::endl;
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

void Request::setPath(std::string path)
{
	this->_path = path;
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

// void Request::setSocket(int socket)
// {
// 	this->_socket = socket;
// }

// int Request::getSocket() const
// {
// 	return this->_socket;
// }

const char *BadRequest::what() const throw()
{
	return "400: Bad Request";
}
