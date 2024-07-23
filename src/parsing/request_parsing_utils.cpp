/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_parsing_utils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:59:21 by panger            #+#    #+#             */
/*   Updated: 2024/07/23 15:43:56 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <sstream>
#include <string>

// Methods get_method(std::string r)
// {
// 	std::string methods_list[] = {"GET", "POST", "DELETE"};
// 	Methods enums_list[] = {GET, POST, DELETE};

// 	std::istringstream iss(r);
// 	std::string method;

// 	iss >> method;
// 	if (!isupper(*r.begin()))
// 		throw BadRequest();
// 	for (int i = 0; i < 3; i++)
// 	{
// 		if (method == methods_list[i])
// 			return (enums_list[i]);
// 	}
// 	throw BadRequest();
// }

Methods get_method(std::string r, State &state, int len)
{
	if (r.compare(0, 3, "GET"))
		return GET;
	if (r.compare(0, 4, "POST"))
		return POST;
	if (r.compare(0, 6, "DELETE"))
		return DELETE;
	throw BadRequest();
}

std::string get_uri(std::string r)
{
	
}

void parse_request_line(std::string r, Request &request)
{
	State state = req_start;
	std::string::iterator cur_begin;

	for (std::string::iterator it = r.begin(); it != r.end(); it++)
	{
		switch (state) {
			case req_start:
				if (*it == '\r' || *it == '\n')
					break;
				if (*it < 'A' || *it > 'Z')
					throw BadRequest();
				state = req_method;
				cur_begin = it;
				break;
			case req_method:
				if (*it == ' ') {
					request._method = get_method(r, state, it - cur_begin);
					state = req_after_method;
				}
				break;
			case req_after_method:
				if (*it == '/') {
					state = req_uri;
					cur_begin = it;
					break;
				}
				else if (*it == ' ')
					throw BadRequest();
				cur_begin = it;
			case req_uri:
				if (*it == ' ')
					
		}
	}
}