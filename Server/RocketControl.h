#pragma once
#include "Command.h"
#include <queue>
#include <condition_variable>
#include <atomic>
#include <mutex>
#include <fstream>

class RocketControl
{
	private:
		std::mutex key;
		std::condition_variable cv;
		std::priority_queue<Command> cmd_pq;
		std::atomic<bool> is_on = true;
		std::ofstream log_file;
	public:
		RocketControl();
		~RocketControl();
		void rocket_data();
		void insert_command();
		void push_command(const Command& cmd);
		void stop();
};