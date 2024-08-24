/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waticouz <waticouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:56:28 by waticouz          #+#    #+#             */
/*   Updated: 2024/08/07 15:56:29 by waticouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include "Request.hpp"
#include <vector>
#include <time.h>

class Client
{
	private:
		int						_fd;
		Request					_request;
		bool					_isReady;
		time_t					_lastRequestTime;

	public:
		Client();
		Client(int client_fd);
		~Client();
		Client(const Client &copy);
		Client &operator=(const Client &copy);

		void					setFd(int fd);
		int						getFd() const;
		Request					getRequest() const;
		int						readRequest();
		bool					isReady();
		void					setReady(bool ready);
		bool					isTimedOut();

};

#endif