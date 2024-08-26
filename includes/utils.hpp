/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:50:31 by panger            #+#    #+#             */
/*   Updated: 2024/08/26 14:12:47 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <string>

int 		ft_strtoi(std::string str);
size_t		ft_strtoul_hex(std::string str);
std::string	strSizeToStr(std::string str);
bool		pathIsDirectory(const std::string &path);
void		setFDNonBlocking(int fd);

#endif
