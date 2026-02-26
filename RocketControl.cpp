#include "RocketControl.h"
#include <iostream>

RocketControl::RocketControl()
{
	log_file.open("telemetry_log.txt", std::ios::app);
	if (log_file.is_open())
	{
		log_file << "----[SYSTEM] Rocket Data log Started----\n";
	}
	else
	{
		std::cerr << "---[ERROR] Could not Open telemetry_log.txt for Writing-----\n";
	}
}
RocketControl::~RocketControl()
{
	if (log_file.is_open())
	{
		log_file << "----[SYSTEM] Rocket Data log Terminated-----\n";
		log_file.close();
	}
}

void RocketControl::rocket_data()
{
	while(true)
	{
		std::unique_lock<std::mutex> lock(key);
		cv.wait(lock, [this]()
		{
			return !cmd_pq.empty() || !is_on;
		});
				
		if (cmd_pq.empty() && !is_on)
		{
			lock.unlock();
			break;
		}
				
		if (!cmd_pq.empty())
		{
			Command current_cmd = cmd_pq.top();
			cmd_pq.pop();
			std::cout << current_cmd;
			if (log_file.is_open() && current_cmd.level == Priority::critical)
			{
				log_file << current_cmd;
			}
			lock.unlock();
		}
	}
}
		
void RocketControl::insert_command()
{
	Command cmd;	
	while(is_on)
	{
		{
			std::lock_guard<std::mutex> lock(key);
			std::cout << "Enter New Command(Press 0 to Exit, Press -1 to Check Command in Process)" << std::endl;
		}
		std::cin >> cmd;
		std::unique_lock<std::mutex> lock(key);
		if (cmd.id == 0)
		{
			lock.unlock();
			stop();
					
		}
		else if (cmd.id == -1)
		{
			cmd();
			lock.unlock();
		}
		else
		{
			cmd_pq.push(cmd);
			lock.unlock();
			cv.notify_one();
		}
	}
}

void RocketControl::push_command(const Command& cmd)
{
	{
		std::lock_guard<std::mutex> lock(key);
		cmd_pq.push(cmd);
		std::cout << "[NETWORK INPUT] Command Added via Network.\n";
	}
	cv.notify_one();
}

void RocketControl::stop()
{
is_on = false;
cv.notify_all();
}
