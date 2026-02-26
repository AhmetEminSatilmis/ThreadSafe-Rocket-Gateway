#include "Client.h"
#include <iostream>
#include <thread>
#define IP "127.0.0.1"
#include <csignal>
#include <vector> 

std::vector<BaseClient*> g_clients;

void signal_handler(int signum)
{
	std::cout << "[SİGNAL INTERRUPT] Signal (" << signum << ") received. Shutting down securely\n";
	
	for (BaseClient* client : g_clients)
	{
		if (client)
		{
			client->stop();
		}
	}
}


int main()
{
	std::cout << "----[CLİENT] Rocket Telemetry System Started-----\n";
	
	TCP_Client tcp_client(IP, 8080);
	UDP_Client udp_client(IP, 8081);
	
	g_clients.push_back(&tcp_client);
	g_clients.push_back(&udp_client);
	std::signal(SIGINT, signal_handler);
	
	std::thread tcp_thread(&TCP_Client::run, &tcp_client);
	std::thread udp_thread(&UDP_Client::run, &udp_client);
	
	tcp_thread.join();
	udp_thread.join();
	return 0;
}
	