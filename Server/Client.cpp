#include "Client.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <vector>
#include <cstdio>
#include <ctime>
#include <thread>
#include <chrono>


BaseClient::BaseClient(const char* ip_num, int port_num) : ip(ip_num), port(port_num), is_running(true)
{
}
BaseClient::~BaseClient()
{
	if (client_socket != -1)
	{
		close(client_socket);
	}
}

void BaseClient::stop()
{
	is_running = false;
}

TCP_Client::TCP_Client(const char* ip_num, int port_num) : BaseClient(ip_num, port_num)
{
}

void TCP_Client::run()
{
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0)
	{
		perror("TCP socket failed");
		return;
	}
	
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(port);
	
	if (inet_pton(AF_INET, ip, &client_addr.sin_addr) <= 0)
	{
		perror("TCP ip failed");
		return;
	}
	
	if (connect(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0)
	{
		perror("TCP connect failed");
		return;
	}
	
	std::vector<std::string> tcp_tasks = {"EngineIgnition", "DeployParachute", "ValveControl", "AbortSequence","CheckAvionics"};
	int tcp_id = 100;
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	
	while (is_running)
	{
		int task_index = std::rand() % tcp_tasks.size();
		std::string selected_task = tcp_tasks[task_index];
		int random_priority = std::rand() % 3 + 1;
		std::string msg = std::to_string(tcp_id) + " " + selected_task + " " + std::to_string(random_priority);
		
		if (send(client_socket, msg.c_str(), msg.size(), 0) < 0)
		{
			perror("TCP send failed");
			return;
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		tcp_id++;
	}
}

UDP_Client::UDP_Client(const char* ip_num, int port_num) : BaseClient(ip_num, port_num)
{
}

void UDP_Client::run()
{
	client_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (client_socket < 0)
	{
		perror("UDP socket failed");
		return;
	}
	
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(port);
	
	if (inet_pton(AF_INET, ip, &client_addr.sin_addr) <= 0)
	{
		perror("UDP ip failed");
		return;
	}
	
	std::vector<std::string> udp_tasks = {"Pitot_Dynamic_Pressure", "Engine_Chamber_Pressure", "Fin_Servo_Angle", "Magnetometer_Vector", "Structural_Vibration"};
	int udp_id = 500;
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	
	while (is_running)
	{
		int task_index = std::rand() % udp_tasks.size();
		std::string selected_task = udp_tasks[task_index];
		int random_priority = std::rand() % 3;
		std::string msg = std::to_string(udp_id) + " " + selected_task + " " + std::to_string(random_priority);
		
		if (sendto(client_socket, msg.c_str(), msg.size(), 0, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0)
		{
			perror("UDP send failed");
			continue;
		}
		
		udp_id++;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	