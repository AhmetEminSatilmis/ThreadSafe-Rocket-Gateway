#include "RocketControl.h"
#include "Command.h"
#include "Server.h"
#include <iostream>
#include <thread>
#include <csignal>

std::vector<BaseServer*> g_baseservers;

void signal_handler(int signum)
{
	std::cout << "[SİGNAL INTERRUPT] Signal (" << signum << ") received. Shutting down securely\n";
	
	for (BaseServer* server : g_baseservers)
	{
		if (server)
		{
			server->stop();
		}
	}
}

int main()
{
	std::cout << "----[SYSTEM] Rocket Data System Initiated----" << std::endl;
	RocketControl rc;
	TCP_Server tcp_server(rc, 8080);
	UDP_Server udp_server(rc, 8081);
	
	g_baseservers.push_back(&tcp_server);
	g_baseservers.push_back(&udp_server);
	std::signal(SIGINT, signal_handler);
	
	std::thread insert_command_thread(&RocketControl::insert_command, &rc);
	std::thread rocket_data_thread(&RocketControl::rocket_data, &rc);
	std::thread tcp_server_thread(&TCP_Server::run, &tcp_server);
	std::thread udp_server_thread(&UDP_Server::run, &udp_server);
	
	insert_command_thread.join();
	rocket_data_thread.join();
	tcp_server_thread.join();
	udp_server_thread.join();
	
	std::cout << "----[SYSTEM] Rocket Data System Terminated----" << std::endl;
	return 0;
}