/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wateecuhs <wateecuhs@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:50:31 by panger            #+#    #+#             */
/*   Updated: 2024/08/25 19:45:47 by wateecuhs        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <string>

int 		ft_strtoi(std::string str);
std::string	strSizeToStr(std::string str);
bool		pathIsDirectory(const std::string &path);
void		setFDNonBlocking(int fd);

#endif
