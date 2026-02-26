#pragma once
#include <atomic>
#include <arpa/inet.h>


class BaseClient
{
	protected:
		const char* ip;
		int port;
		int client_socket;
		std::atomic<bool> is_running;
		struct sockaddr_in client_addr;
	public:
		BaseClient(const char* ip_num, int port_num);
		virtual ~BaseClient();
		virtual void run() = 0;
		void stop();
};

class TCP_Client : public BaseClient
{
	public:
		TCP_Client(const char* ip_num, int port_num);
		void run() override;
};

class UDP_Client : public BaseClient
{
	public:
		UDP_Client(const char* ip_num, int port_num);
		void run() override;
};
		