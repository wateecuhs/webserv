/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_parsing_utils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:59:21 by panger            #+#    #+#             */
/*   Updated: 2024/07/23 15:51:04 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <sstream>
#include <string>

Methods parse_method(std::string r, State &state, int len)
{
	if (r.compare(0, 3, "GET"))
		return GET;
	if (r.compare(0, 4, "POST"))
		return POST;
	if (r.compare(0, 6, "DELETE"))
		return DELETE;
	throw BadRequest();
}

std::string parse_uri(std::string r, State &state, int len)
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
					request.setMethod(parse_method(r, state, it - cur_begin));
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
				{
					parse_uri(r, state, it - cur_begin); // wont work right now
				}
		}
	}
}