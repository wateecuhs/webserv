/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:22:31 by panger            #+#    #+#             */
/*   Updated: 2024/07/11 15:33:57 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

Server *parse_config(std::string conf_path)
{
	// std::ifstream cfstream(conf_path);
	std::string config(conf_path);

	// while (!cfstream.eof())
		// cfstream >> config;
	std::cout << config << std::endl;
	return NULL;
}
