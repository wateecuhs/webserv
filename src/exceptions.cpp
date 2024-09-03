/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 13:37:57 by alermolo          #+#    #+#             */
/*   Updated: 2024/08/22 17:04:52 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exceptions.hpp"
#include <string>
#include <fstream>
#include <sstream>

const char *Forbidden403::what() const throw()
{
	return "403 Forbidden";
}

const char *NotFound404::what() const throw()
{
	return "404 Not Found";
}

const char *MethodNotAllowed405::what() const throw()
{
	return "405 Method Not Allowed";
}

const char *RequestTimeout408::what() const throw()
{
	return "408 Request Timeout";
}

const char *InternalServerError500::what() const throw()
{
	return "500 Internal Server Error";
}

const char *BadGateway502::what() const throw()
{
	return "502 Bad Gateway";
}

const char *ContentTooLarge413::what() const throw()
{
	return "413 Content Too Large";
}

const char *URITooLong414::what() const throw()
{
	return "414 URI Too Long";
}

const char *InvalidConfigFile::what() const throw()
{
	return "Config file is invalid";
}

const char *BadInput::what() const throw()
{
	return "Invalid input";
}

const char *BadRequest::what() const throw()
{
	return "400 Bad Request";
}

const char *GatewayTimeout504::what() const throw()
{
	return "504 Gateway Timeout";
}