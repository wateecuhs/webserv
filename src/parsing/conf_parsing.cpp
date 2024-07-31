/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_parsing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:22:31 by panger            #+#    #+#             */
/*   Updated: 2024/07/31 13:53:36 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"
#include <sstream>
#include <cstring>

void parseServers(std::string content);
void parseTokens(std::string content, std::string::iterator it, ConfState state, Socket &socket);
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
/* 
void parseServers(std::string content)
{
	std::string::iterator	cur_begin;
	ConfState				state = conf_start;
	Socket					socket;

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
					state = conf_inbetween;
				break;
			case conf_inbetween:
				parseTokens(content, it, state, socket);
			default:
				return;
		}
	}
}

void parseTokens(std::string content, std::string::iterator it, ConfState state, Socket &socket)
{

	std::string::iterator cur_begin;
	std::string token;
	int keyword;

	while (it != content.end()) 
	{
		switch (state) {
			case conf_inbetween:
				if (isspace(*it))
					break;
				if (isalpha(*it)) {
					state = conf_token;
					cur_begin = it;
					break;
				}
				throw InvalidConfigFile();
			case conf_token:
				if (isalpha(*it) || (*it) == '_')
					break;
				if (isspace(*it)) {
					token = std::string(cur_begin, it);
					state = conf_value_after_token;
					break;
				}
				throw InvalidConfigFile();
			case conf_value_after_token:
				if (isspace(*it))
					break;
				if (isalpha(*it) || *it == '_') {
					keyword = inKeywords(token);
					if (keyword == 0)
						throw InvalidConfigFile();
					std::cout << "Keyword: " << keyword << std::endl;
					switch (keyword) {
						case 1:
							state = conf_server_name;
							break;
						case 2:
							state = conf_listen;
							break;
						case 3:
							state = conf_location;
							break;
						default:
							throw InvalidConfigFile();
					}
					cur_begin = it;
					break;
				}
			case conf_server_name:
				// if (isspace(*it) && isspace(*(it - 1)))
					// break;
				if (isalnum(*it) || *it == '_' || *it == '.')
					break;
				if (*it == ';') {
					std::cout << "Here\n";
					if (*(it--) != ' ')
						socket.addServerName(std::string(cur_begin, it));
					std::vector<std::string> sn = socket.getServerNames();
					for (std::vector<std::string>::iterator ite = sn.begin(); ite != sn.end(); ite++)
						std::cout << *ite << std::endl;
					state = conf_inbetween;
					break;
				}
				if (isspace(*it)) {
					socket.addServerName(std::string(cur_begin, it));
					cur_begin = it;
					break;
				}
				throw InvalidConfigFile();
			default:
				return;
		}
		it++;
	}
}

 */

void parseServers(std::string content)
{
	std::stringstream iss(content);
	std::string line;
	ConfState state = conf_server;

	iss >> line;
	while (!iss.eof())
	{
		std::cout << line << std::endl;
		switch (state) {
			case conf_server:
				if (!line.compare("server")) {
					state = conf_server_brace;
					break;
				}
				if (!line.compare("server{")) {
					state = conf_after_server_brace;
					break;
				}
				else
					throw InvalidConfigFile();
			case conf_server_brace:
				if (!line.compare("{")) {
					state = conf_after_server_brace;
					break;
				}
				throw InvalidConfigFile();
			case conf_after_server_brace:
				
			default:
				break;
		}
		iss >> line;
	}
}