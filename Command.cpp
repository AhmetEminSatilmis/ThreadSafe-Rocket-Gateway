#include "Command.h"
#include <string>
#include <cstring>

bool Command::operator<(const Command& cmd) const
{
	return static_cast<int>(level) < static_cast<int>(cmd.level);
}
	
bool Command::operator==(const Command& cmd) const
{
	return id == cmd.id;
}
	
void Command::operator()() const
{
	std::cout << "---[ROCKET] Check Command---" << task << " Works Succesfully\n"; 
}

std::ostream& operator<<(std::ostream& os, const Priority& pri)
{
	switch(pri)
	{
		case Priority::low:
			return os << "Low";
		case Priority::mid:
			return os << "Mid";
		case Priority::high:
			return os << "High";
		case Priority::critical:
			return os << "Critical";
		default:
			return os << "Unknown";
	}
}

std::istream& operator>>(std::istream& is, Command& cmd)
{
	is >> cmd.id;
	if (cmd.id == 0 || cmd.id == -1)
	{
		return is;
	}
	std::string temp_str;
	int temp_p;
	is >> temp_str >> temp_p;
	strncpy(cmd.task, temp_str.c_str(), sizeof(cmd.task) - 1);
	cmd.task[sizeof(cmd.task) - 1] = '\0';
	cmd.level = static_cast<Priority>(temp_p);
	return is;
}

std::ostream& operator<<(std::ostream& os, const Command& cmd)
{
	os << "ID: " << cmd.id << "---Task: " << cmd.task << "---Priority: " << cmd.level << std::endl;
	return os;
}