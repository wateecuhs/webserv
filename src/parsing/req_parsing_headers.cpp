/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req_parsing_headers.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 11:22:56 by panger            #+#    #+#             */
/*   Updated: 2024/08/09 17:40:08 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "exceptions.hpp"

void	parseCookies(std::string cookies, Request &req)
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
		req.setCookie(key, value);
		start = end + 2;
		end = cookies.find("; ", start);
	}
	req.setHasCookies(true);
}

void parseHeaders(std::string headers, Request &req)
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
			parseCookies(value, req);
		else
			req.addHeader(key, value);
		start = end + 2;
		end = headers.find("\r\n", start);
	}
}
