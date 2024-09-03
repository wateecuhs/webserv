/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 13:33:44 by alermolo          #+#    #+#             */
/*   Updated: 2024/08/09 16:24:41 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

#include <exception>

class Forbidden403: public std::exception {
	public:
		virtual const char *what() const throw();
};

class NotFound404: public std::exception {
	public:
		virtual const char *what() const throw();
};

class MethodNotAllowed405: public std::exception {
	public:
		virtual const char *what() const throw();
};

class RequestTimeout408: public std::exception {
	public:
		virtual const char *what() const throw();
};

class InternalServerError500: public std::exception {
	public:
		virtual const char *what() const throw();
};

class BadGateway502: public std::exception {
	public:
		virtual const char *what() const throw();
};

class InvalidConfigFile: public std::exception
{
	public:
		virtual const char *what() const throw();
};

class BadInput: public std::exception
{
	public:
		virtual const char *what() const throw();
};

class BadRequest: public std::exception {
	public:
		virtual const char *what() const throw();
};

class ContentTooLarge413: public std::exception {
	public:
		virtual const char *what() const throw();
};
class URITooLong414: public std::exception {
	public:
		virtual const char *what() const throw();
};

class GatewayTimeout504: public std::exception {
	public:
		virtual const char *what() const throw();
};

#endif
