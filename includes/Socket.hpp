/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 10:29:36 by panger            #+#    #+#             */
/*   Updated: 2024/07/31 13:25:36 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <string>
#include <map>
#include <vector>

class Socket
{
	private:
		int									_fd;
		unsigned long						_host;
		int									_port;
		bool								_useCGI;
		std::map<std::string, std::string>	_CGI_map;
		std::vector<std::string>			_server_names;

	public:
		Socket();
		Socket(int port);
		~Socket();
		Socket(const Socket &copy);
		Socket &operator=(const Socket &copy);
		void addServerName(std::string name);
		std::vector<std::string> getServerNames() const;
		void addCGIExtension(std::string extension, std::string path);
};

#endif