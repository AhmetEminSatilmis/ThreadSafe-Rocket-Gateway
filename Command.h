#pragma once
#include <iostream>

enum class Priority
{
	low,
	mid,
	high,
	critical
};

struct Command
{
	int id;
	char task[32];
	Priority level;
	
	bool operator<(const Command& cmd) const;
	bool operator==(const Command& cmd) const;
	void operator()() const;
};

std::ostream& operator<<(std::ostream& os, const Priority& pri);
std::istream& operator>>(std::istream& is, Command& cmd);
std::ostream& operator<<(std::ostream& os, const Command& cmd);