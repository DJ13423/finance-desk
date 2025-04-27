#pragma once

#include <string>

class Notification
{
public:
	int id;
	int accountID;
	std::string message;
	std::string timestamp;
	bool isRead;

	Notification(int id, int accountID, const std::string& message, const std::string& timestamp, bool isRead)
		: id(id), accountID(accountID), message(message), timestamp(timestamp), isRead(isRead) {}
};

