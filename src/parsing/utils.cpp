/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:47:27 by panger            #+#    #+#             */
/*   Updated: 2024/07/30 14:59:27 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

int inKeywords(std::string s)
{
	if (!s.compare("server_name"))
		return 1;
	if (!s.compare("listen"))
		return 2;
	if (!s.compare("location"))
		return 3;
	return 0;
}