/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wateecuhs <wateecuhs@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:28:07 by panger            #+#    #+#             */
/*   Updated: 2024/08/25 19:26:14 by wateecuhs        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include "VirtualServer.hpp"
#include "utils.hpp"
#include "enums.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "exceptions.hpp"

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

void verifyAddHostPort(std::string content, VirtualServer &VirtualServer)
{
	if (content.find(":") != std::string::npos)
	{
		std::string host = content.substr(0, content.find(":"));
		std::string port = content.substr(content.find(":") + 1);
		if (verifyPort(port) == 0 && verifyHost(host) == 0)
			throw BadInput();
		VirtualServer.setHost(host);
		VirtualServer.setPort(ft_strtoi(port));
	}
	else if (content.find(".") != std::string::npos) {
		if (verifyHost(content) == 0)
			throw BadInput();
		VirtualServer.setHost(content);
		VirtualServer.setPort(80);
	}
	else {
		if (verifyPort(content) == 0)
			throw BadInput();
		VirtualServer.setPort(ft_strtoi(content));
		VirtualServer.setHost("*");
	}
}

ConfState verifyErrorPageMatch(std::string word, std::stringstream &iss, VirtualServer &VirtualServer)
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
	VirtualServer.addErrorPage(ft_strtoi(error_code), path);
	return word[word.size() - 1] == ';' ? conf_new_token : conf_semicolon;
}

bool validateLocation(std::string path)
{	
	if (path == "/")
		return true;
	if (path.size() < 2 || path[0] != '/' || path[path.size() - 1] != '/')
		return false;
	for (std::string::iterator it = path.begin() + 1; it != path.end() - 1; it++)
	{
		if (!isalnum(*it) && *it != '/' && *it != '_' &&  *it != '-' && *it != '.')
			return false;
	}
	return true;
}

bool validateRoot(std::string path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0) {
		if (!(s.st_mode & S_IFDIR) || access(path.c_str(), R_OK) == -1)
			return false;
	}
	else
		return false;
	return true;
}

bool validateDefaultFile(std::string path)
{
	if (path.find("/") != std::string::npos || path.find("..") != std::string::npos || path.size() < 1)
		return false;
	return true;
}

bool validateUploadPath(std::string path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0) {
		if (!(s.st_mode & S_IFDIR) || access(path.c_str(), R_OK) == -1)
			return false;
	}
	else
		return false;
	return true;
}

bool validateCGIPath(std::string path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0) {
		if (!(s.st_mode & S_IFREG) || access(path.c_str(), R_OK) == -1)
			return false;
	}
	else
		return false;
	return true;
}

LocationState verifyAddCGIExtension(std::string word, std::stringstream &iss, Location &location)
{
	std::string extension = word;
	std::string cgi_path;

	if (extension.size() < 2 || extension[0] != '.' || extension[extension.size() - 1] == ';')
		throw BadInput();
	for (std::string::iterator it = extension.begin() + 1; it != extension.end(); it++)
	{
		if (!isalpha(*it))
			throw BadInput();
	}
	iss >> word;
	if (word[word.size() - 1] == ';')
		cgi_path = word.substr(0, word.size() - 1);
	else
		cgi_path = word;
	if (!validateCGIPath(cgi_path))
		throw BadInput();
	location.addCGIExtension(extension, cgi_path);
	return word[word.size() - 1] == ';' ? loc_new_token : loc_semicolon;
}

bool validateURI(const std::string& uri)
{
	if (uri.empty()) {
		return false;
	}

	size_t colonPos = uri.find(':');
	if (colonPos == std::string::npos) {
		return false;
	}

	std::string scheme = uri.substr(0, colonPos);
	if (scheme.empty()) {
		return false;
	}
	for (size_t i = 0; i < scheme.length(); ++i) {
		char c = scheme[i];
		if (!(isalpha(c) || (i > 0 && (isdigit(c) || c == '+' || c == '-' || c == '.')))) {
			return false;
		}
	}

	std::string remaining = uri.substr(colonPos + 1);
	if (remaining.substr(0, 2) == "//") {
		size_t authorityEnd = remaining.find('/', 2);
		if (authorityEnd == std::string::npos) {
			authorityEnd = remaining.length();
		}
		std::string authority = remaining.substr(2, authorityEnd - 2);
		
		for (size_t i = 0; i < authority.length(); ++i) {
			char c = authority[i];
			if (!(isalnum(c) || c == '.' || c == '-' || c == '_' || c == '~' || c == '%' || c == '!' || 
				  c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' || c == '*' || c == '+' || 
				  c == ',' || c == ';' || c == '=' || c == ':' || c == '@')) {
				return false;
			}
		}
		remaining = remaining.substr(authorityEnd);
	}

	for (size_t i = 0; i < remaining.length(); ++i) {
		char c = remaining[i];
		if (!(isalnum(c) || c == '/' || c == '?' || c == '#' || c == '[' || c == ']' || c == '@' || 
			  c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' || c == '*' || 
			  c == '+' || c == ',' || c == ';' || c == '=' || c == '-' || c == '.' || c == '_' || 
			  c == '~' || c == '%')) {
			return false;
		}
	}

	return true;
}