/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:22:31 by panger            #+#    #+#             */
/*   Updated: 2024/07/29 14:32:50 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

Server *parseConfig(std::string conf_path)
{
	// std::ifstream cfstream(conf_path);
	std::string config(conf_path);

	// while (!cfstream.eof())
		// cfstream >> config;
	std::cout << config << std::endl;
	return NULL;
}
