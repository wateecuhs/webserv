/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:25:42 by panger            #+#    #+#             */
/*   Updated: 2024/07/23 14:51:49 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "parsing.hpp"
#include <sstream>
#include <cstring>

Request::Request() {}

Request::Request(std::string request)
{
	parse_request_line(request);
	std::cout << "Method found -> " << this->_method << std::endl;
}

Request::Request(Request &src)
{
	*this = src;
}

Request::~Request() {}