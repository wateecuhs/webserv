/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:50:31 by panger            #+#    #+#             */
/*   Updated: 2024/08/02 16:14:07 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

#include "Socket.hpp"
#include "enums.hpp"
#include <iostream>
#include <fstream>
#include "Request.hpp"

Socket			*parseConfig(std::string conf_path);
void			parseRequestLine(std::string r, Request &request);
void			parseHeaders(std::string r, Request &req);
void			verifyAddHostPort(std::string content, Socket &socket);
ConfState		verifyErrorPageMatch(std::string word, std::stringstream &iss, Socket &socket);
bool			validateLocation(std::string path);
bool			validateRoot(std::string path);
bool			validateDefaultFile(std::string path);
bool			validateUploadPath(std::string path);
bool			validateCGIPath(std::string path);
LocationState	verifyAddCGIExtension(std::string word, std::stringstream &iss, Location &location);
bool			validateURI(const std::string& uri);

#endif