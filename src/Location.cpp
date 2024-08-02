/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:26:13 by panger            #+#    #+#             */
/*   Updated: 2024/08/02 14:01:58 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include <sstream>
#include "parsing.hpp"

Location::Location(std::stringstream &iss, std::string word)
{
	if (validateLocation(word) == false)
		throw InvalidConfigFile();
	this->_path = word;
	LocationState state = loc_brace_open;
	while (iss >> word)
	{
		switch (state) {
			case loc_brace_open:
				if (word != "{")
					throw InvalidConfigFile();
				state = loc_new_token;
				break;
			case loc_new_token:
				if (word == "root")
					state = loc_root;
				else if (word == "cgi_extension")
					state = loc_CGI_map;
				else if (word == "}")
					state = loc_brace_close;
				else
					throw InvalidConfigFile();
				break;
			case loc_root:
				if (word[word.size() - 1] == ';')
				{
					this->setRoot(word.substr(0, word.size() - 1));
					state = loc_new_token;
				}
		}
	}
}

Location::~Location()
{
}

Location::Location(const Location &copy)
{
	this->_path = copy._path;
	this->_root = copy._root;
	this->_useCGI = copy._useCGI;
	this->_CGI_map = copy._CGI_map;
	this->_body_size = copy._body_size;
}

Location &Location::operator=(const Location &copy)
{
	if (this == &copy)
		return *this;
	this->_path = copy._path;
	this->_root = copy._root;
	this->_useCGI = copy._useCGI;
	this->_CGI_map = copy._CGI_map;
	this->_body_size = copy._body_size;
	return *this;
}

void Location::setRoot(std::string root)
{
	this->_root = root;
}

std::string Location::getRoot() const
{
	return this->_root;
}

void Location::setUseCGI(bool useCGI)
{
	this->_useCGI = useCGI;
}

bool Location::getUseCGI() const
{
	return this->_useCGI;
}

void Location::addCGIExtension(std::string extension, std::string path)
{
	this->_CGI_map[extension] = path;
}

std::map<std::string, std::string> Location::getCGIMap() const
{
	return this->_CGI_map;
}
