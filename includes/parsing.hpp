/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:50:31 by panger            #+#    #+#             */
/*   Updated: 2024/07/23 14:46:52 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

#include "Server.hpp"
#include "enums.hpp"
#include <iostream>
#include <fstream>

Server	*parse_config(std::string conf_path);
Methods	get_method(std::string request, State &state);
void parse_request_line(std::string r);

#endif