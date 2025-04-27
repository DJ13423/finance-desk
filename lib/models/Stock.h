#pragma once

#include <string>

class Stock
{
public:
	int id;
	std::string name;
	std::string symbol;

	Stock(int id, const std::string& name, std::string& symbol)
		: id(id), name(name), symbol(symbol) {}
};

