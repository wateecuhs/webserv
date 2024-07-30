/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:29:36 by panger            #+#    #+#             */
/*   Updated: 2024/07/30 13:07:34 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <map>

class Socket
{
	private:
		int									port;
		unsigned long						host;
		bool								useCGI;
		std::map<std::string, std::string>	cgi_extensions;
	public:
		Socket();
		Socket(int port);
		~Socket();
};

#endif