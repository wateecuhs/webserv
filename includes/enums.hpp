/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enums.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:18 by panger            #+#    #+#             */
/*   Updated: 2024/07/23 15:15:53 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUMS_HPP
# define ENUMS_HPP

typedef enum e_methods{
	GET,
	POST,
	DELETE
} Methods;


typedef enum e_state {
	req_start,
	req_method,
	req_after_method,
	req_uri
} State;

#endif