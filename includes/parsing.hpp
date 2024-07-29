/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:50:31 by panger            #+#    #+#             */
/*   Updated: 2024/07/29 11:45:00 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

#include "Server.hpp"
#include "enums.hpp"
#include <iostream>
#include <fstream>
#include "Request.hpp"

Server	*parse_config(std::string conf_path);
void	parse_request_line(std::string r, Request &request);
void	parse_headers(std::string r, Request &req);

#endif