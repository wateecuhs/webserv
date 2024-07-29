/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:50:31 by panger            #+#    #+#             */
/*   Updated: 2024/07/29 14:32:50 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

#include "Server.hpp"
#include "enums.hpp"
#include <iostream>
#include <fstream>
#include "Request.hpp"

Server	*parseConfig(std::string conf_path);
void	parseRequestLine(std::string r, Request &request);
void	parseHeaders(std::string r, Request &req);

#endif