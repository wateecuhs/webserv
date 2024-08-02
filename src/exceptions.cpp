/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 13:37:57 by alermolo          #+#    #+#             */
/*   Updated: 2024/08/02 14:53:46 by alermolo         ###   ########.fr       */
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

const char *InternalServerError500::what() const throw()
{
	return "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
}

const char *BadGateway502::what() const throw()
{
	return "HTTP/1.1 502 Bad Gateway\r\nContent-Length: 0\r\n\r\n";
}
