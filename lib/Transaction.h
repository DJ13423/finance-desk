#pragma once

#include <string>
#include "SQLiteDB.h"

class Transaction
{
public:
	int transactionID;
	int fromAccountID;
	int toAccountID;
	double amount;
	std::string transactionType; // "deposit", "withdrawal", "transfer"
	std::string timestamp;

	Transaction(int transactionID, int fromAccountID, int toAccountID, double amount, const std::string& transactionType, const std::string& timestamp)
		: transactionID(transactionID), fromAccountID(fromAccountID), toAccountID(toAccountID), amount(amount), transactionType(transactionType), timestamp(timestamp) {}
};