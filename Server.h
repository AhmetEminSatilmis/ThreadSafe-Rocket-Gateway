#pragma once
#include "RocketControl.h"
#include <netinet/in.h>
#include <atomic>
#include <vector>



class BaseServer
{
	protected:
		RocketControl& rc;
		int port;
		int server_fd;
		std::atomic<bool> is_running;
		struct sockaddr_in address;
		void process_message(char* buffer);
	public:
		BaseServer(RocketControl& rc_ref, int port_num);
		virtual ~BaseServer();
		virtual void run() = 0;
		virtual void stop();
};

class TCP_Server : public BaseServer
{
	private:
		int new_socket;
	public:
		TCP_Server(RocketControl& rc_ref, int port_num);
		~TCP_Server() override;
		void run() override;
		void stop() override;
};

class UDP_Server : public BaseServer
{
	public:
		UDP_Server(RocketControl& rc_ref, int port_num);
		void run() override;
};
		
	
		
		