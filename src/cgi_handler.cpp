/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:13:19 by alermolo          #+#    #+#             */
/*   Updated: 2024/08/02 17:16:53 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Request.hpp"
#include "exceptions.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <sstream>

std::string	execCGI(Request &request, const Socket &socket)
{
	std::string	request_method = "REQUEST_METHOD=" + request.getMethodString();
	if (request.getQuery().empty())
		request.setQuery(request.getBody());
	std::string	query_string = "QUERY_STRING=" + request.getQuery();
	std::string	content_length = "CONTENT_LENGTH=" + strSizeToStr(request.getBody());
	std::string	extension = request.getPath().substr(request.getPath().find_last_of('.'));
	// NE COMPILE PAS CAR CGI DANS LOCATION
	// std::string cgi_handler = socket.getCgiHandler(extension);
	std::string cgi_handler = "/usr/bin/php-cgi";

	const char 	*env[4] = {request_method.c_str(), query_string.c_str(), content_length.c_str(), NULL};
	std::string	path = request.getPath();
	const char 	*argv[3] = {cgi_handler.c_str(), path.c_str(), NULL};

	pid_t 	pid;
	int 	pipe_out[2];
	int 	pipe_in[2];
	int 	status = 0;

	if (pipe(pipe_out) == -1 || pipe(pipe_in) == -1)
		throw InternalServerError500();
	pid = fork();
	if (pid == -1)
		throw InternalServerError500();

	//child process
	if (pid == 0)
	{
		close(pipe_out[0]);
		close(pipe_in[1]);
		if (dup2(pipe_out[1], STDOUT_FILENO) == -1 || dup2(pipe_in[0], STDIN_FILENO) == -1)
			throw InternalServerError500();
		close(pipe_out[1]);
		close(pipe_in[0]);

		if (access(cgi_handler.c_str(), X_OK) == -1)
			throw BadGateway502();
		if (execve(const_cast<const char*>(cgi_handler.c_str()), const_cast<char* const*>(argv), const_cast<char* const*>(env)) == -1)
			throw BadGateway502();

		std::exit(EXIT_SUCCESS);
	}

	//parent process
	else
	{
		close(pipe_out[1]);
		close(pipe_in[0]);

		//put request body into pipe for cgi handler to read
		write(pipe_in[1], request.getBody().c_str(), request.getBody().size());
		close(pipe_in[1]);

		char 				buffer[2048];
		int 				bytes_read;
		std::stringstream 	ss;

		while ((bytes_read = read(pipe_out[0], buffer, 2047)) > 0 || ss.eof() || ss.fail()){
			if (bytes_read == -1)
				throw BadGateway502();
			buffer[bytes_read] = '\0';
			ss << buffer;
		}
		close(pipe_out[0]);

		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			throw BadGateway502();

		std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + strSizeToStr(ss.str()) + "\r\n\r\n" + ss.str();
		return response;
	}
}

void	handleCGI(Request &request, const Socket &socket){
	int backup_stdin = dup(STDIN_FILENO);
	if (backup_stdin == -1)
		throw InternalServerError500();
	int backup_stdout = dup(STDOUT_FILENO);
	if (backup_stdout  == -1)
		throw InternalServerError500();

	std::string content = execCGI(request, socket);

	if (dup2(backup_stdin, STDIN_FILENO) == -1 || dup2(backup_stdout, STDOUT_FILENO) == -1)
		throw InternalServerError500();
	close(backup_stdin);
	close(backup_stdout);

	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + strSizeToStr(content) + "\r\n\r\n" + content;
	if (send(socket.getFd(), response.c_str(), response.size(), 0) == -1)
		throw InternalServerError500();
}
