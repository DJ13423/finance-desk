#pragma once

#include <string>
#include "../SQLiteDB.h"

class Transaction
{
public:
	int transactionID;
	int fromAccountID;
	int toAccountID;
	double amount;
	std::string timestamp;

	Transaction(int transactionID, int fromAccountID, int toAccountID, double amount, const std::string& timestamp)
		: transactionID(transactionID), fromAccountID(fromAccountID), toAccountID(toAccountID), amount(amount), timestamp(timestamp) {}
};