#include "Server.h"
#include "Command.h"
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>


void BaseServer::process_message(char* buffer)
{
	Command cmd;
	int priority_temp;
	int scan_result = sscanf(buffer, "%d %s %d", &cmd.id, cmd.task, &priority_temp);
	if (scan_result == 3)
	{
		cmd.level = static_cast<Priority>(priority_temp);
		rc.push_command(cmd);
	}
}

BaseServer::BaseServer(RocketControl& rc_ref, int port_num) : rc(rc_ref), port(port_num), server_fd(-1), is_running(true)
{
}

BaseServer::~BaseServer()
{
	if (server_fd != -1)
	{
		close(server_fd);
	}
}

void BaseServer::stop()
{
	is_running = false;
	if (server_fd != -1)
	{
		shutdown(server_fd, SHUT_RDWR);
	}
}

TCP_Server::TCP_Server(RocketControl& rc_ref, int port_num) : BaseServer(rc_ref, port_num), new_socket(-1)
{
}

TCP_Server::~TCP_Server()
{
	if (server_fd != -1)
	{
		close(server_fd);
	}
	if (new_socket != -1)
	{
		close(new_socket);
	}
}

void TCP_Server::stop()
{
	BaseServer::stop();
	if (new_socket != -1)
	{
		shutdown(new_socket, SHUT_RDWR);
	}
}

void TCP_Server::run()
{
	int opt = 1;
	char buffer[1024] = {0};
	int addrlen = sizeof(address);
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("TCP socket failed");
		return;
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
	{
		perror("TCP setsockopt failed");
		return;
	}
	
	memset(&address, 0, addrlen);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	
	if (bind(server_fd, (struct sockaddr*)&address, addrlen) < 0)
	{
		perror("TCP bind failed");
		return;
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("TCP listen failed");
		return;
	}
	
	new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	if (new_socket < 0)
	{
		perror("TCP accept failed");
		return;
	}
	
	while (is_running)
	{
		memset(buffer, 0, 1024);
		int valread = read(new_socket, buffer, 1024);
		if (valread > 0)
		{
			process_message(buffer);
		}
		else if (valread == 0)
		{
			break;
		}
		else
		{
			perror("TCP read failed");
			return;
		}
	}
}

UDP_Server::UDP_Server(RocketControl& rc_ref, int port_num) : BaseServer(rc_ref, port_num)
{
}

void UDP_Server::run()
{
	int opt = 1;
	char buffer[1024] = {0};
	int addrlen = sizeof(address);
	
	server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (server_fd < 0)
	{
		perror("UDP socket failed");
		return;
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
	{
		perror("UDP setsockopt failed");
		return;
	}
	
	memset(&address, 0, addrlen);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	
	if (bind(server_fd, (struct sockaddr*)&address, addrlen) < 0)
	{
		perror("UDP bind failed");
		return;
	}
	
	struct sockaddr_in client_addr;
	std::cout << "----[SERVER] UDP Port " << port << " is Read for Incoming Telemetry---\n";
	
	while(is_running)
	{
		socklen_t client_len = sizeof(client_addr);
		memset(buffer, 0, 1024);
		int valread = recvfrom(server_fd, buffer, 1024, 0, (struct sockaddr*)&client_addr, &client_len);
		if (valread > 0)
		{
			process_message(buffer);
		}
		else if (valread < 0)
		{
			perror("UDP recvfrom failed");
			continue;
		}
	}
}
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
		
	






























		
