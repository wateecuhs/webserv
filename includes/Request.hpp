/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:25:49 by panger            #+#    #+#             */
/*   Updated: 2024/07/22 17:35:08 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "enums.hpp"
#include <map>
#include <iostream>

class Request {
	private:
		methods								_method;
		std::string							_path;
		float								_http_version;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		unsigned int						_port;
		unsigned long						_host;

		Request();
	public:
		Request(Request &src);
		Request(std::string request);
		~Request();
};

#endif