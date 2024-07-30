/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_parsing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:22:31 by panger            #+#    #+#             */
/*   Updated: 2024/07/30 14:28:23 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"
#include <cstring>

void parseServers(std::string content);

class InvalidConfigFile: public std::exception {
	public:
		virtual const char *what() const throw(){
			return "Config file is invalid";
		}
};



Socket *parseConfig(std::string conf_path)
{
	std::ifstream	cfstream;
	// char			buf[2048];

	cfstream.open(conf_path.c_str());
	if (!cfstream.is_open())
		throw InvalidConfigFile();
	std::string content((std::istreambuf_iterator<char>(cfstream)),
						 std::istreambuf_iterator<char>());
	// std::cout << content << std::endl;
	try {
		parseServers(content);
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return NULL;
}

void parseServers(std::string content)
{
	std::string::iterator	cur_begin;
	ConfState				state = conf_start;

	for (std::string::iterator it = content.begin(); it != content.end(); it++)
	{
		std::cout << it - content.begin() << std::endl;
		switch (state) {
			case conf_start:
				if (isspace(*it))
					break;
				if (isalpha(*it)) {
					state = conf_server;
					cur_begin = it;
					break;
				}
				throw InvalidConfigFile();

			case conf_server:
				if (isalpha(*it))
					break;
				if (isspace(*it) || *it == '{') {
					if (!content.compare(cur_begin - content.begin(), it - cur_begin, "server"))
						state = conf_brace_after_server;
					else
						throw InvalidConfigFile();
				}
				else
					throw InvalidConfigFile();

			case conf_brace_after_server:
				if (isspace(*it))
					break;
				if (*it == '{')
					state = conf_after_brace;
				break;
			
			default:
				return;
		}
	}
}