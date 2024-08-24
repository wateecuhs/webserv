/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:42:30 by panger            #+#    #+#             */
/*   Updated: 2024/08/24 17:46:15 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Socket.hpp"
#include "exceptions.hpp"
#include <sys/stat.h>

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

size_t ft_strtoul_hex(std::string str)
{
	size_t ret = 0.0;
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (*it >= '0' && *it <= '9')
			ret = ret * 16 + *it - '0';
		else if (*it >= 'a' && *it <= 'f')
			ret = ret * 16 + *it - 'a' + 10;
		else if (*it >= 'A' && *it <= 'F')
			ret = ret * 16 + *it - 'A' + 10;
		else
			throw BadInput();
	}
	return ret;
}

std::string	strSizeToStr(std::string str)
{
	std::stringstream ss;
	ss << str.size();
	if (ss.fail())
		throw InternalServerError500();
	return ss.str();
}

bool pathIsDirectory(const std::string &path)
{
	struct stat sb;

	if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
		return true;
	return false;
}
