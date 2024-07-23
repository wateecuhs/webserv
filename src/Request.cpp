/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:25:42 by panger            #+#    #+#             */
/*   Updated: 2024/07/23 15:59:28 by panger           ###   ########.fr       */
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

std::string Request::getPath() const
{
	return this->_path;
}

float Request::getHTTPVersion() const
{
	return this->_http_version;
}

std::map<std::string, std::string>	Request::getHeaders() const
{
	return this->_headers;
}

std::string Request::getBody() const
{
	return this->_body;
}

unsigned int Request::getPort() const
{
	return this->_port;
}

unsigned long Request::getHost() const
{
	return this->_host;
}