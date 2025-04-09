#include "../lib/GameDatabaseManager.h"
#include "../lib/SQLiteDB.h"
#include "../lib/Transaction.h"
#include <iostream>
#include <string>
#include "../lib/Account.h"
#include "../include/sqlite3.h"

using namespace std;

//SQLiteDB* GameDatabaseManager::db = new SQLiteDB("Test.db");

void GameDatabaseManager::initializeDatabase() {
	db->exec("CREATE TABLE IF NOT EXISTS accounts (id INTEGER PRIMARY KEY, first_name TEXT, last_name TEXT, username TEXT, password TEXT, balance REAL);");
	db->exec("CREATE TABLE IF NOT EXISTS transactions (id INTEGER PRIMARY KEY, fromAccountID INTEGER, toAccountID INTEGER, amount REAL, transactionType TEXT, timestamp TEXT);");
}

Transaction* GameDatabaseManager::createNewTransactionEntry(int fromAccountID, int toAccountID, double amount, const string& transactionType)
{
	SQLiteStatement* intsertStatement = db->prepareStatement("INSERT INTO transactions (fromAccountID, toAccountID, amount, transactionType) VALUES (?, ?, ?, ?, ?, ?)");
	intsertStatement->bindInt(1, fromAccountID);
	intsertStatement->bindInt(2, toAccountID);
	intsertStatement->bindDouble(3, amount);
	intsertStatement->bindText(4, transactionType);
	if (intsertStatement->step() == SQLITE_ROW) {
		SQLiteStatement* selectStatement = db->prepareStatement("SELECT id, timestamp FROM users WHERE id = last_insert_rowid();");
		if (selectStatement->step() == SQLITE_ROW) {
			int id = selectStatement->getColumnInt(0);
			string timestamp = selectStatement->getColumnTextStr(1);
			return new Transaction(id, fromAccountID, toAccountID, amount, transactionType, timestamp);
		}
		else {
			cerr << "Error retrieving transaction ID: " << sqlite3_errmsg(db->db) << endl;
		}
	}
}
//
//struct TransactionSelection {
//	int id;
//	int fromAccountID;
//	int toAccountID;
//	double amount;
//	string transactionType;
//	string timestamp;
//};
//
//Transaction* GameDatabaseManager::selectTransactions(string& sql) {
//	SQLiteStatement* statement = db->prepareStatement("SELECT id, fromAccountID, toAccountID, amount, transactionType, timestamp FROM transactions WHERE id = ?;");
//	statement->bindInt(1, transactionSelection.id);
//	if (statement->step() == SQLITE_ROW) {
//		int id = statement->getColumnInt(0);
//		int fromAccountID = statement->getColumnInt(1);
//		int toAccountID = statement->getColumnInt(2);
//		double amount = statement->getColumnDouble(3);
//		string transactionType = statement->getColumnTextStr(4);
//		string timestamp = statement->getColumnTextStr(5);
//		return new Transaction(id, fromAccountID, toAccountID, amount, transactionType, timestamp);
//	}
//	else {
//		cerr << "Transaction not found!" << endl;
//	}
//
//}

Account GameDatabaseManager::getAccount(string username, string password) {
	SQLiteDB db("Test.db");
	SQLiteStatement* statement = db.prepareStatement("SELECT id, first_name, last_name, username, balance FROM Users WHERE username = ? AND password = ?;");
	statement->bindText(1, username);
	statement->bindText(2, password);
	if (statement->step() == SQLITE_ROW) {
		int accountID = statement->getColumnInt(0);
		string firstName = statement->getColumnTextStr(1);
		string lastName = statement->getColumnTextStr(2);
		string username = statement->getColumnTextStr(3);
		double balance = statement->getColumnDouble(4);
		return Account{ accountID, firstName, lastName, username, balance };
	}
	else {
		cout << "Invalid credentials!" << endl;
	}
}

void GameDatabaseManager::saveAccount(Account account) {
	SQLiteDB db("Test.db");
	SQLiteStatement* statement = db.prepareStatement("UPDATE users SET balance = ? WHERE id = ?;");
	statement->bindDouble(1, account.balance);
	statement->bindInt(2, account.accountNumber);
	if (statement->step() != SQLITE_DONE) {
		cout << "Error saving account: " << sqlite3_errmsg(db.db) << endl;
		return;
	}
	cout << "Account saved successfully!" << endl;
}

void GameDatabaseManager::createAccount(string username, string password) {
	SQLiteDB db("Test.db");
	SQLiteStatement* statement = db.prepareStatement("INSERT INTO users (username, password) VALUES (?, ?);");
	statement->bindText(1, username);
	statement->bindText(2, password); // password needs to be hashed in the future
	if (statement->step() != SQLITE_DONE) {
		cout << "Error creating account: " << sqlite3_errmsg(db.db) << endl;
		return;
	}
	cout << "Account created successfully!" << endl;
}