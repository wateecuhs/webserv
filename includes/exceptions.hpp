/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 13:33:44 by alermolo          #+#    #+#             */
/*   Updated: 2024/08/02 13:38:58 by alermolo         ###   ########.fr       */
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

class InternalServerError500: public std::exception {
	public:
		virtual const char *what() const throw();
};

class BadGateway502: public std::exception {
	public:
		virtual const char *what() const throw();
};

#endif
