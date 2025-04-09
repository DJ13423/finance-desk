#pragma once
#include <string>
#include "SQLiteDB.h"
#include "Transaction.h"
#include "Account.h"
#include "../include/sqlite3.h"

using namespace std;

class GameDatabaseManager
{
public:
	static SQLiteDB* db;
	static void initializeDatabase();
	static Transaction* createNewTransactionEntry(int fromAccountID, int toAccountID, double amount, const std::string& transactionType);
	static Account getAccount(string username, string password);
	static void saveAccount(Account account);
	static void createAccount(string username, string password);
};

