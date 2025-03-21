/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wateecuhs <wateecuhs@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:20:22 by panger            #+#    #+#             */
/*   Updated: 2024/08/25 19:26:14 by wateecuhs        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <string>
#include "VirtualServer.hpp"
#include <map>

class VirtualServer;

class Location
{
	private:
		std::string							_path;
		std::string							_root;
		bool								_useCGI;
		bool								_autoindex;
		std::string							_default_file;
		std::map<std::string, std::string>	_CGI_map;
		bool								_methods[3];
		bool								_file_upload;
		std::string							_upload_path;
		std::string							_http_redirection;
		bool								_redirect;

		Location();
	public:
		Location(std::stringstream &iss, std::string word);
		Location(const Location &copy);
		Location &operator=(const Location &copy);
		~Location();

		void								addCGIExtension(std::string extension, std::string path);
		std::map<std::string, std::string>	getCGIMap() const;
		std::string							getCGIPath(std::string extension) const;
		void								setRoot(std::string root);
		std::string							getRoot() const;
		void								setUseCGI(bool useCGI);
		bool								getUseCGI() const;
		void								setMethod(std::string method);
		bool								*getMethods();
		bool								getMethod(int method) const;
		void								setAutoindex(bool autoindex);
		bool								getAutoindex() const;
		void								setDefaultFile(std::string default_file);
		std::string							getDefaultFile() const;
		void								setFileUpload(bool file_upload);
		bool								getFileUpload() const;
		void								setUploadPath(std::string upload_path);
		std::string							getUploadPath() const;
		void								setHttpRedirection(std::string http_redirection);
		std::string							getHttpRedirection() const;
		std::string							getPath() const;
		void								setPath(std::string path);
		bool								getRedirect() const;
		void								setRedirect(bool redirect);
};

#endif
