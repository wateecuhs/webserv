/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 13:37:57 by alermolo          #+#    #+#             */
/*   Updated: 2024/08/05 17:11:58 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exceptions.hpp"
#include <string>
#include <fstream>
#include <sstream>

const char *Forbidden403::what() const throw()
{
	return "HTTP/1.1 403 Forbidden\r\nContent-Length: 114\r\n\r\n<html>\n<head>\n\t<title>403 Forbidden</title>\n</head>\n<body>\n\t<h1 align=\"center\">403 Forbidden</h1>\n</body>\n</html>\n";
}

const char *NotFound404::what() const throw()
{
	return "HTTP/1.1 404 Not Found\r\nContent-Length: 114\r\n\r\n<html>\n<head>\n\t<title>404 Not Found</title>\n</head>\n<body>\n\t<h1 align=\"center\">404 Not Found</h1>\n</body>\n</html>\n";
}

const char *MethodNotAllowed405::what() const throw()
{
	return "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\n\r\n";
}

const char *InternalServerError500::what() const throw()
{
	return "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
}

const char *BadGateway502::what() const throw()
{
	return "HTTP/1.1 502 Bad Gateway\r\nContent-Length: 0\r\n\r\n";
}

const char *ContentTooLarge413::what() const throw()
{
	return "HTTP/1.1 413 Content Too Large	\r\nContent-Length: 0\r\n\r\n";
}

const char *URITooLong414::what() const throw()
{
	return "HTTP/1.1 414 URI Too Long	\r\nContent-Length: 0\r\n\r\n";
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
	return "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
}
