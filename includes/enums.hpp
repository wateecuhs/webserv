/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enums.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:18 by panger            #+#    #+#             */
/*   Updated: 2024/07/31 13:53:02 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUMS_HPP
# define ENUMS_HPP

typedef enum e_methods{
	GET,
	POST,
	DELETE
} Methods;


typedef enum e_req_state {
	req_start,
	req_method,
	req_after_method,
	req_uri,
	req_after_uri,
	req_http_HTTP,
	req_http_version,
	req_after_version
} ReqState;

typedef enum e_conf_state {
	conf_server,
	conf_server_brace,
	conf_after_server_brace,
	conf_inbetween,
	conf_token,
	conf_server_name,
	conf_listen,
	conf_location,
	conf_value_after_token,
	conf_semicolon_after_value,
} ConfState;

#endif