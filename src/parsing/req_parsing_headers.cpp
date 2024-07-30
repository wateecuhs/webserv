/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   r_parsing_headers.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 11:22:56 by panger            #+#    #+#             */
/*   Updated: 2024/07/29 14:31:21 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

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
		req.addHeader(key, value);
		start = end + 2;
		end = headers.find("\r\n", start);
	}
}