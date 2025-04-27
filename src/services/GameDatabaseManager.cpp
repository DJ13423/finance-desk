#include "../../lib/services/GameDatabaseManager.h"
#include "../../lib/models/Account.h"
#include "../../lib/models/Transaction.h"
#include "../../lib/models/Stock.h"
#include "../../lib/models/Loan.h"
#include "../../lib/models/Notification.h"
#include "../../lib/SQLiteDB.h"
#include "../../lib/Utils.h"
#include "../../lib/services/GameContext.h"
#include <iostream>
#include <string>
#include <vector>
#include "../../include/sqlite3.h"
#include <optional>

using namespace std;

SQLiteDB* GameDatabaseManager::db = new SQLiteDB("FinanceDesk.db");

void GameDatabaseManager::initializeDatabase() {
	db->exec(R"(
		CREATE TABLE IF NOT EXISTS accounts (
			id INTEGER PRIMARY KEY, 
			first_name TEXT, 
			last_name TEXT, 
			username TEXT UNIQUE COLLATE NOCASE, 
			password TEXT, 
			balance REAL DEFAULT 0.0,
			credit_score_percent INTEGER DEFAULT 0.5,
			created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
			is_system_account INTEGER DEFAULT 0
		);
	)");
	db->exec(R"(
		CREATE TABLE IF NOT EXISTS transactions (
			id INTEGER PRIMARY KEY, 
			from_account_id INTEGER NOT NULL, 
			to_account_id INTEGER NOT NULL, 
			amount REAL NOT NULL, 
			timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
			FOREIGN KEY (from_account_id) REFERENCES accounts(id),
			FOREIGN KEY (to_account_id) REFERENCES accounts(id)
		);
	)");
	db->exec(R"(
		CREATE TABLE IF NOT EXISTS loans (
			id INTEGER PRIMARY KEY, 
			account_id INTEGER NOT NULL, 
			amount REAL NOT NULL, 
			interest_rate REAL NOT NULL, 
			start_date DATETIME DEFAULT CURRENT_TIMESTAMP, 
			end_date DATE GENERATED ALWAYS AS (DATE(start_date, '+' || term_months || ' months')) STORED
			status TEXT DEFAULT 'active', 
			FOREIGN KEY (account_id) REFERENCES accounts(id)
		);
	)");
	db->exec(R"(
		CREATE TABLE IF NOT EXISTS notifications (
			id INTEGER PRIMARY KEY, 
			account_id INTEGER NOT NULL, 
			message TEXT NOT NULL, 
			timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, 
			is_read INTEGER DEFAULT 0, 
			FOREIGN KEY (account_id) REFERENCES accounts(id)
		);
	)");
	db->exec(R"(
		CREATE TABLE game_progress (
			last_save_timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
		);
	)");
	db->exec(R"(
		CREATE TABLE stocks (
			id INTEGER PRIMARY KEY,
			symbol TEXT NOT NULL,
			name TEXT
		);
	)");
	db->exec(R"(
		CREATE TABLE stock_price_history (
			id INTEGER PRIMARY KEY,
			stock_id INTEGER NOT NULL,
			timestamp DATETIME NOT NULL,
			price REAL NOT NULL,
			volume INTEGER,
			FOREIGN KEY (stock_id) REFERENCES stocks(id)
		);
	)");
	db->exec(R"(
		INSERT INTO accounts (first_name, last_name, username, password, balance, is_system_account)
		SELECT 'Bank', 'Account', 'bank_funds', '', 1000000.0, 1
		WHERE NOT EXISTS (
			SELECT 1 FROM accounts WHERE username = 'bank_funds'
		);
	)");
	auto opt = loginToAccount("bank_funds", "");
	if (opt.has_value()) {
		GameContext::bankMainAccount = new Account(opt.value());
	}
	else {
		GameContext::bankMainAccount = nullptr;
	}
}

optional<Account> GameDatabaseManager::loginToAccount(const string &username, const string &password) {
	SQLiteStatement* statement = GameDatabaseManager::db->prepareStatement(R"(
                SELECT id, first_name, last_name, username, balance
                FROM accounts WHERE username = ? AND password = ?;
            )");
	statement->bindText(1, username);
	statement->bindText(2, password);
	if (statement->step() == SQLITE_ROW) {
		return Account(statement->getColumnInt(0), statement->getColumnTextStr(1), statement->getColumnTextStr(2), statement->getColumnTextStr(3), statement->getColumnDouble(4));
	}
	else {
		return nullopt;
	}
}

Transaction* GameDatabaseManager::TransferMoney(int fromAccountID, int toAccountID, double amount) {
	db->exec("BEGIN TRANSACTION;");
	SQLiteStatement* updateFromStatement = db->prepareStatement("UPDATE accounts SET balance = balance - ? WHERE id = ?;");
	updateFromStatement->bindDouble(1, amount);
	updateFromStatement->bindInt(2, fromAccountID);
	if (updateFromStatement->step() != SQLITE_DONE) {
		cerr << "Error updating from account: " << sqlite3_errmsg(db->db) << endl;
		db->exec("ROLLBACK;");
		return nullptr;
	}

	SQLiteStatement* updateToStatement = db->prepareStatement("UPDATE accounts SET balance = balance + ? WHERE id = ?;");
	updateToStatement->bindDouble(1, amount);
	updateToStatement->bindInt(2, toAccountID);
	if (updateToStatement->step() != SQLITE_DONE) {
		cerr << "Error updating to account: " << sqlite3_errmsg(db->db) << endl;
		db->exec("ROLLBACK;");
		return nullptr;
	}

	// Create a new transaction entry
	SQLiteStatement* insertStatement = db->prepareStatement("INSERT INTO transactions (from_account_id, to_account_id, amount) VALUES (?, ?, ?);");
	insertStatement->bindInt(1, fromAccountID);
	insertStatement->bindInt(2, toAccountID);
	insertStatement->bindDouble(3, amount);
	if (insertStatement->step() != SQLITE_DONE) {
		cerr << "Error inserting transaction: " << sqlite3_errmsg(db->db) << endl;
		db->exec("ROLLBACK;");
		return nullptr;
	}
	
// Get the transaction ID
SQLiteStatement* transactionIDStatement = db->prepareStatement("SELECT id, timestamp FROM transactions WHERE id = last_insert_rowid();");
	if (transactionIDStatement->step() != SQLITE_ROW) {
		cerr << "Error retrieving transaction ID: " << sqlite3_errmsg(db->db) << endl;
		db->exec("ROLLBACK;");
		return nullptr;
	}
	int transactionID = transactionIDStatement->getColumnInt(0);
	string timestamp = transactionIDStatement->getColumnTextStr(1);

	Transaction* transaction = new Transaction(transactionID, fromAccountID, toAccountID, amount, timestamp);
	if (!transaction) {
		db->exec("ROLLBACK;");
		return nullptr;
	}

	db->exec("COMMIT;");
	return transaction;
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

Account* GameDatabaseManager::getAccount(const string &username) {
	SQLiteStatement* statement = GameDatabaseManager::db->prepareStatement("SELECT id, first_name, last_name, username, balance FROM accounts WHERE username = ?;");
	statement->bindText(1, username);
	if (statement->step() == SQLITE_ROW) {
		int accountID = statement->getColumnInt(0);
		string firstName = statement->getColumnTextStr(1);
		string lastName = statement->getColumnTextStr(2);
		string usernameFromDB = statement->getColumnTextStr(3);
		double balance = statement->getColumnDouble(4);
		return new Account( accountID, firstName, lastName, usernameFromDB, balance );
	}
	else {
		cout << "Account not found!" << endl;
	}
}

Account* GameDatabaseManager::getAccount(int accountNumber) {
	SQLiteStatement* statement = GameDatabaseManager::db->prepareStatement("SELECT id, first_name, last_name, username, balance FROM accounts WHERE id = ?;");
	statement->bindInt(1, accountNumber);
	if (statement->step() == SQLITE_ROW) {
		int accountID = statement->getColumnInt(0);
		string firstName = statement->getColumnTextStr(1);
		string lastName = statement->getColumnTextStr(2);
		string username = statement->getColumnTextStr(3);
		double balance = statement->getColumnDouble(4);
		return new Account{ accountID, firstName, lastName, username, balance };
	}
	else {
		cout << "Account not found!" << endl;
	}
}

void GameDatabaseManager::saveAccount(Account account) {
	SQLiteStatement* statement = GameDatabaseManager::db->prepareStatement("UPDATE accounts SET balance = ? WHERE id = ?;");
	statement->bindDouble(1, account.balance);
	statement->bindInt(2, account.accountNumber);
	if (statement->step() != SQLITE_DONE) {
		cout << "Error saving account: " << sqlite3_errmsg(GameDatabaseManager::db->db) << endl;
		return;
	}
	cout << "Account saved successfully!" << endl;
}

bool GameDatabaseManager::createAccount(const string &firstName, const string &lastName, const string &username, const string &password) {
	SQLiteStatement* statement = GameDatabaseManager::db->prepareStatement(R"(
				INSERT INTO accounts (first_name, last_name, username, password)
				VALUES (?, ?, ?, ?);
			)");
	statement->bindText(1, firstName);
	statement->bindText(2, lastName);
	statement->bindText(3, username);
	statement->bindText(4, password); // needs to be hashed in the future
	if (statement->step() == SQLITE_DONE) {
		return true;
	}
	else {
		return false;
	}
}

vector<Transaction*> GameDatabaseManager::getTransactions(int accountID) {
	SQLiteStatement* statement = GameDatabaseManager::db->prepareStatement("SELECT id, from_account_id, to_account_id, amount, timestamp FROM transactions WHERE from_account_id = ? OR to_account_id = ?;");
	statement->bindInt(1, accountID);
	statement->bindInt(2, accountID);
	vector<Transaction*> transactions;
	while (statement->step() == SQLITE_ROW) {
		int id = statement->getColumnInt(0);
		int fromAccountID = statement->getColumnInt(1);
		int toAccountID = statement->getColumnInt(2);
		double amount = statement->getColumnDouble(3);
		string timestamp = statement->getColumnTextStr(4);
		transactions.push_back(new Transaction(id, fromAccountID, toAccountID, amount, timestamp));
	}
	return transactions;
}

vector<Loan*> GameDatabaseManager::getLoans(int accountID) {
	SQLiteStatement* statement = GameDatabaseManager::db->prepareStatement("SELECT id, account_id, amount, interest_rate, start_date, end_date, status FROM loans WHERE account_id = ?;");
	statement->bindInt(1, accountID);
	vector<Loan*> loans;
	while (statement->step() == SQLITE_ROW) {
		int id = statement->getColumnInt(0);
		int accountID = statement->getColumnInt(1);
		double amount = statement->getColumnDouble(2);
		double interestRate = statement->getColumnDouble(3);
		string startDate = statement->getColumnTextStr(4);
		string endDate = statement->getColumnTextStr(5);
		string status = statement->getColumnTextStr(6);
		loans.push_back(new Loan(id, accountID, amount, interestRate, startDate, endDate, status));
	}
	return loans;
}

vector<Notification*> GameDatabaseManager::getNotifications(int accountID) {
	SQLiteStatement* statement = GameDatabaseManager::db->prepareStatement("SELECT id, account_id, message, timestamp, is_read FROM notifications WHERE account_id = ?;");
	statement->bindInt(1, accountID);
	vector<Notification*> notifications;
	while (statement->step() == SQLITE_ROW) {
		int id = statement->getColumnInt(0);
		int accountID = statement->getColumnInt(1);
		string message = statement->getColumnTextStr(2);
		string timestamp = statement->getColumnTextStr(3);
		bool isRead = statement->getColumnInt(4) == 1;
		notifications.push_back(new Notification(id, accountID, message, timestamp, isRead));
	}
	return notifications;
}
