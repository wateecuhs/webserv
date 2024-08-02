/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:28:07 by panger            #+#    #+#             */
/*   Updated: 2024/08/02 13:28:05 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include "Socket.hpp"
#include "utils.hpp"
#include "enums.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int verifyPort(std::string content)
{
	float value = ft_strtoi(content);
	if (value > 65535 || value < 0 || value != (int)value)
		throw BadInput();
	return value;
}

int verifyHost(std::string content)
{
	float	value;
	int		dot_count = 0;
	size_t next_dot = content.find(".");
	size_t prev_dot = 0;

	while (next_dot != std::string::npos)
	{
		value = ft_strtoi(content.substr(prev_dot, next_dot - prev_dot));
		if (value > 255 || value < 0 || next_dot - prev_dot > 4 || next_dot - prev_dot < 1)
			throw BadInput();
		dot_count++;
		prev_dot = next_dot + 1;
		next_dot = content.find(".", prev_dot);
	}
	value = ft_strtoi(content.substr(prev_dot, 10));
	if (value > 255 || value < 0 || dot_count != 3)
		throw BadInput();
	return 1;
}

void verifyAddHostPort(std::string content, Socket &socket)
{
	if (content.find(":") != std::string::npos)
	{
		std::string host = content.substr(0, content.find(":"));
		std::string port = content.substr(content.find(":") + 1);
		if (verifyPort(port) == 0 && verifyHost(host) == 0)
			throw BadInput();
		socket.setHost(host);
		socket.setPort(ft_strtoi(port));
	}
	else if (content.find(".") != std::string::npos) {
		if (verifyHost(content) == 0)
			throw BadInput();
		socket.setHost(content);
		socket.setPort(80);
	}
	else {
		if (verifyPort(content) == 0)
			throw BadInput();
		socket.setPort(ft_strtoi(content));
		socket.setHost("*");
	}
}

ConfState verifyErrorPageMatch(std::string word, std::stringstream &iss, Socket &socket)
{
	struct stat s;
	std::string path;
	std::string error_code;

	if (word.size() != 3 || ft_strtoi(word) == 0 || word[word.size() - 1] == ';')
		throw BadInput();
	error_code = word;
	iss >> word;
	if (word[word.size() - 1] == ';')
		path = word.substr(0, word.size() - 1);
	else
		path = word;
	if (stat(path.c_str(), &s) == 0) {
		if (!(s.st_mode & S_IFREG) || access(path.c_str(), R_OK) == -1)
			throw BadInput();
	}
	else
		throw BadInput();
	socket.addErrorPage(ft_strtoi(error_code), path);
	return word[word.size() - 1] == ';' ? conf_new_token : conf_semicolon;
}

bool validateLocation(std::string path)
{	
	if (path.size() < 2 || path[0] != '/' || path[path.size() - 1] != '/')
		return false;
	for (std::string::iterator it = path.begin() + 1; it != path.end() - 1; it++)
	{
		if (!isalnum(*it) && *it != '/' && *it != '_' &&  *it != '-' && *it != '.')
			return false;
	}
	return true;
}