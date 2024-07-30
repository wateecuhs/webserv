/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:29:36 by panger            #+#    #+#             */
/*   Updated: 2024/07/30 14:36:58 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <string>
#include <map>

class Socket
{
	private:
		int									fd;
		unsigned long						host;
		int									port;
		bool								useCGI;
		std::map<std::string, std::string>	cgi_extensions;

	public:
		Socket();
		Socket(int port);
		~Socket();
};

#endif