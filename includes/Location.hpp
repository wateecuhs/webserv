/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: panger <panger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:20:22 by panger            #+#    #+#             */
/*   Updated: 2024/08/01 17:39:44 by panger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <string>
#include <map>

class Location
{
	private:
		std::string							_path;
		std::string							_root;
		bool								_useCGI;
		std::map<std::string, std::string>	_CGI_map;
		int									_body_size;
		Location();
	public:
		Location(std::stringstream &iss, std::string word);
		~Location();
		Location(const Location &copy);
		Location &operator=(const Location &copy);
	
		void								addCGIExtension(std::string extension, std::string path);
		std::map<std::string, std::string>	getCGIMap() const;
		void								setRoot(std::string root);
		std::string							getRoot() const;
		void								setUseCGI(bool useCGI);
		bool								getUseCGI() const;


};

#endif