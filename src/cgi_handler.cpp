/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:13:19 by alermolo          #+#    #+#             */
/*   Updated: 2024/07/30 17:34:18 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Request.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>

class InternalServerError500;

class BadGateway502: public std::exception {
	public:
		virtual const char *what() const throw() {
			return "HTTP/1.1 502 Bad Gateway\r\nContent-Length: 0\r\n\r\n";
		}
};



void	handleCGI(const Request &request, const Socket &socket)
{
	std::string	request_method = "REQUEST_METHOD=" + request.getMethod();
	std::string	query_string = "QUERY_STRING=" + request.getQuery();
	std::string	content_length = "CONTENT_LENGTH=" + request.getBody().size();
	std::string	extension = request.getPath().substr(request.getPath().find_last_of('.') + 1);

	const char 	*env[4] = {request_method.c_str(), query_string.c_str(), content_length.c_str(), NULL};
	const char 	*argv[2] = {request.getPath().c_str(), NULL};

	pid_t 	pid;
	int 	pipefd[2];

	if (pipe(pipefd) == -1)
		throw InternalServerError500();
	pid = fork();
	if (pid == -1)
		throw InternalServerError500();
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		if (execve(request.getPath().c_str(), (char *const *)argv, (char *const *)env) == -1)
			throw BadGateway502();
		std::exit(EXIT_SUCCESS);
	}
	else
	{
		close(pipefd[1]);
		char 	buffer[2048];
		int 	bytes_read;
		while ((bytes_read = read(pipefd[0], buffer, 2048)) > 0)
			send(socket.getFD(), buffer, bytes_read, 0);
		close(pipefd[0]);
		waitpid(pid, NULL, 0);
	}
}
