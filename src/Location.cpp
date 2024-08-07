/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waticouz <waticouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:26:13 by panger            #+#    #+#             */
/*   Updated: 2024/08/07 15:11:00 by waticouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include <sstream>
#include "parsing.hpp"
#include "exceptions.hpp"

Location::Location(std::stringstream &iss, std::string word)
{
	LocationState	state = loc_brace_open;
	std::string		shaved_word;
	bool			trailing_semicolon;

	if (validateLocation(word) == false)
		throw InvalidConfigFile();
	for (int i = 0; i < 3; i++)
		this->_methods[i] = false;
	this->_path = word;
	std::cout << "Location path: " << this->_path << std::endl;

	while (iss >> word)
	{
		trailing_semicolon = word[word.size() - 1] == ';' && word.size() > 1;
		if (trailing_semicolon)
			shaved_word = word.substr(0, word.size() - 1);
		else
			shaved_word = word;

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
				else if (word == "methods_allowed")
					state = loc_methods;
				else if (word == "auto_index")
					state = loc_autoindex;
				else if (word == "default_file")
					state = loc_default_file;
				else if (word == "file_upload")
					state = loc_file_upload;
				else if (word == "http_redirection")
					state = loc_http_redirection;
				else if (word == "}")
					return ;
				else
					throw InvalidConfigFile();
				break;

			case loc_semicolon:
				if (word != ";")
					throw InvalidConfigFile();
				state = loc_new_token;
				break;

			case loc_root:
				if (!validateRoot(shaved_word))
						throw InvalidConfigFile();
				this->setRoot(shaved_word);
				state = trailing_semicolon ? loc_new_token : loc_semicolon;
				break;

			case loc_methods:
				if (word == ";") {
					state = loc_new_token;
					break;
				}
				setMethod(shaved_word);
				if (trailing_semicolon)
					state = loc_new_token;
				break;

			case loc_autoindex:
				if (shaved_word == "on")
					this->_autoindex = true;
				else if (shaved_word == "off")
					this->_autoindex = false;
				else
					throw InvalidConfigFile();
				state = trailing_semicolon ? loc_new_token : loc_semicolon;
				break;

			case loc_default_file:
				if (!validateDefaultFile(shaved_word))
					throw InvalidConfigFile();
				this->_default_file = shaved_word;
				state = trailing_semicolon ? loc_new_token : loc_semicolon;
				break;

			case loc_file_upload:
				this->_file_upload = true;
				if (!validateUploadPath(shaved_word))
					throw InvalidConfigFile();
				this->_upload_path = shaved_word;
				state = trailing_semicolon ? loc_new_token : loc_semicolon;
				break;

			case loc_CGI_map:
				state = verifyAddCGIExtension(word, iss, *this);
				this->_useCGI = true;
				break;

			case loc_http_redirection:
				if (validateURI(shaved_word) == false)
					throw InvalidConfigFile();
				this->_http_redirection = shaved_word;
				state = trailing_semicolon ? loc_new_token : loc_semicolon;
				break;

			default:
				break;
		}
	}
	throw InvalidConfigFile();
}

Location::~Location()
{
}

Location::Location(const Location &copy)
{
	this->_path = copy.getPath();
	this->_root = copy.getRoot();
	this->_useCGI = copy.getUseCGI();
	this->_CGI_map = copy.getCGIMap();
	this->_autoindex = copy.getAutoindex();
	this->_default_file = copy.getDefaultFile();
	this->_methods[0] = copy.getMethod(0);
	this->_methods[1] = copy.getMethod(1);
	this->_methods[2] = copy.getMethod(2);
	this->_file_upload = copy.getFileUpload();
	this->_upload_path = copy.getUploadPath();
	this->_http_redirection = copy.getHttpRedirection();
}

Location &Location::operator=(const Location &copy)
{
	if (this == &copy)
		return *this;
	this->_path = copy.getPath();
	this->_root = copy.getRoot();
	this->_useCGI = copy.getUseCGI();
	this->_CGI_map = copy.getCGIMap();
	this->_autoindex = copy.getAutoindex();
	this->_default_file = copy.getDefaultFile();
	this->_methods[0] = copy.getMethod(0);
	this->_methods[1] = copy.getMethod(1);
	this->_methods[2] = copy.getMethod(2);
	this->_file_upload = copy.getFileUpload();
	this->_upload_path = copy.getUploadPath();
	this->_http_redirection = copy.getHttpRedirection();
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

void Location::setMethod(std::string method)
{
	if (method == "GET")
		this->_methods[0] = true;
	else if (method == "POST")
		this->_methods[1] = true;
	else if (method == "DELETE")
		this->_methods[2] = true;
	else
		throw InvalidConfigFile();
}

bool *Location::getMethods()
{
	return this->_methods;
}

bool Location::getMethod(int method) const
{
	if (method < 0 || method > 2)
		return false;
	return this->_methods[method];
}

void Location::setAutoindex(bool autoindex)
{
	this->_autoindex = autoindex;
}

bool Location::getAutoindex() const
{
	return this->_autoindex;
}

void Location::setDefaultFile(std::string default_file)
{
	this->_default_file = default_file;
}

std::string Location::getDefaultFile() const
{
	return this->_default_file;
}

void Location::setFileUpload(bool file_upload)
{
	this->_file_upload = file_upload;
}

bool Location::getFileUpload() const
{
	return this->_file_upload;
}

void Location::setUploadPath(std::string upload_path)
{
	this->_upload_path = upload_path;
}

std::string Location::getUploadPath() const
{
	return this->_upload_path;
}

void Location::setHttpRedirection(std::string http_redirection)
{
	this->_http_redirection = http_redirection;
}

std::string Location::getHttpRedirection() const
{
	return this->_http_redirection;
}

std::string Location::getPath() const
{
	return this->_path;
}

void Location::setPath(std::string path)
{
	this->_path = path;
}