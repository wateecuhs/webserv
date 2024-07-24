/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_parsing_utils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:59:21 by panger            #+#    #+#             */
/*   Updated: 2024/07/24 14:44:08 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <sstream>
#include <string>
#include <cstdlib>


Methods parse_method(std::string r, int len)
{
	switch (len) {
		case 3:
			if (r.compare(0, 3, "GET") == 0)
				return GET;
			throw BadRequest();
		case 4:
			if (r.compare(0, 4, "POST") == 0)
				return POST;
			throw BadRequest();
		case 6:
			if (r.compare(0, 6, "DELETE") == 0)
				return DELETE;
			throw BadRequest();
		default:
			throw BadRequest();
	}
}

float parse_version(std::string r, int pos)
{
	float version;

	if (!(isdigit(r[pos]) && r[pos + 1] == '.' && isdigit(r[pos + 2]))) {
		throw BadRequest();
	}
	std::string v = r.substr(pos, 3);
	version = strtof(v.c_str(), NULL);
	if (version != 1.1f && version != 1.0f && version != 0.9f)
		throw BadRequest();
	return version;
}

std::string parse_uri(std::string r, int pos, int len)
{
	return r.substr(pos, len);
}

bool is_http(std::string r, int pos, int len)
{
	if (r.compare(pos, len, "HTTP/") == 0)
		return true;
	return false;
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
					request.setMethod(parse_method(r, it - cur_begin));
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
			case req_uri:
				if (*it == ' ') {
					request.setPath(parse_uri(r, cur_begin - r.begin(), it - cur_begin));
					state = req_after_uri;
				}
				break;
			case req_after_uri:
				if (*it == 'H') {
					cur_begin = it;
					state = req_http_HTTP;
					break;
				}
				throw BadRequest();
			case req_http_HTTP:
				if (*it == 'T' || *it == 'P')
					break;
				if (*it == '/') {
					if (!is_http(r, cur_begin - r.begin(), it - cur_begin + 1))
						throw BadRequest();
					state = req_http_version;
					break;
				}
				throw BadRequest();
			case req_http_version:
				request.setHTTPVersion(parse_version(r, it - r.begin()));
				state = req_after_version;
			default:
				break;
		}
	}
}