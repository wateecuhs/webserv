/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:42:30 by panger            #+#    #+#             */
/*   Updated: 2024/08/02 17:12:41 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Socket.hpp"
#include "exceptions.hpp"

int ft_strtoi(std::string str)
{
	int ret = 0.0;
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (*it < '0' || *it > '9')
			throw BadInput();
		ret = ret * 10 + *it - '0';
	}
	return ret;
}