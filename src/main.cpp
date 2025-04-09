#include <iostream>
#include <string>
#include <optional>
#include "../lib/Authentication.h"
#include "../lib/SQLiteDB.h"
#include "../lib/Account.h"
#include "../lib/Transaction.h"
#include "../lib/GameDatabaseManager.h"
#include "../include/sqlite3.h"
#include "../lib/ThrowFatalError.h"

using namespace std;


// database contains:
/*
-- accounts --
account_id
username
password
balance
created_at

-- transactions --
transaction_id
from_account_id
to_account_id
amount
transaction_type
timestamp

-- game_progress --
last_save_timestamp

-- stocks --
id
name
symbol

-- stock_price_history --
id
stock_id
timestamp
price
volume
*/

/*

CREATE TABLE accounts (
	id INTEGER PRIMARY KEY,
	username TEXT NOT NULL,
	password TEXT NOT NULL,
	balance REAL DEFAULT 0.0,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE transactions (
	transaction_id INTEGER PRIMARY KEY,
	from_account_id INTEGER NOT NULL,
	to_account_id INTEGER NOT NULL,
	amount REAL NOT NULL,
	transaction_type TEXT NOT NULL,
	timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
	FOREIGN KEY (from_account_id) REFERENCES accounts(account_id),
	FOREIGN KEY (to_account_id) REFERENCES accounts(account_id)
);

CREATE TABLE game_progress (
	last_save_timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE stocks (
    id INTEGER PRIMARY KEY,
    symbol TEXT NOT NULL,
    name TEXT
);

CREATE TABLE stock_price_history (
    id INTEGER PRIMARY KEY,
    stock_id INTEGER NOT NULL,
    timestamp DATETIME NOT NULL,
    price REAL NOT NULL,
    volume INTEGER,
    FOREIGN KEY (stock_id) REFERENCES stocks(id)
);

*/


void getUserLoginInfo()
{
    cout << "Please enter your credentials: " << endl;
    string username;
    string password;

    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    // authenticate and return the accountNumber
}

class UserPrompt {
public:
    void showLoginModal() {
		cout << "Login Modal" << endl;
	}

    void showSignupModal() {
		cout << "Signup Modal" << endl;
	}
};

template <typename... Args>
void printChoices(Args... args) {
    int index = 1;
    ((std::cout << index++ << ". " << args << std::endl), ...); // what C++ wizardry is this?
}

optional<Account> loginToAccount(string username, string password) {
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

bool createAccount(string firstName, string lastName, string username, string password) {
	SQLiteStatement* statement = GameDatabaseManager::db->prepareStatement(R"(
				INSERT INTO accounts (first_name, last_name, username, password)
				VALUES (?, ?, ?, ?);
			)");
    statement->bindText(1, firstName);
    statement->bindText(2, lastName);
    statement->bindText(3, username);
    statement->bindText(4, password);
    if (statement->step() == SQLITE_DONE) {
		return true;
	}
    else {
		return false;
	}
}

#include <stdexcept>
SQLiteDB* GameDatabaseManager::db = new SQLiteDB("Test.db");
int main() {
    //GameDatabaseManager::initializeDatabase();


    /*SQLiteStatement* statement = db.prepareStatement("SELECT * FROM Users;");

    for (int i = 0; i < statement->getColumnCount(); i++) {
        cout << statement->getColumnName(i) << "   ";
    }
    cout << endl;
    while (statement->step() == SQLITE_ROW) {
        for (int i = 0; i < statement->getColumnCount(); i++) {
            cout << statement->getColumnText(i) << "    ";
        }
        cout << endl;
    }*/
    GameDatabaseManager::initializeDatabase();
    while (true) {
        cout << "Welcome to the Bank!" << endl;
        int choice;
        printChoices("Login", "Signup");
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Enter username: ";
            string username;
            cin >> username;
            cout << "Enter password: ";
            string password;
            cin >> password;
            optional account = loginToAccount(username, password);
            if (!account) {
                cout << "Invalid username or password." << endl;
                break;
            }
            cout << "Welcome, " << account->firstName << "!" << endl;
            cout << "Your balance is: $" << account->balance << endl;
            printChoices("View Transactions", "Deposit", "Withdraw", "Transfer", "Logout");
            int action;
            cin >> action;
            switch (action) {

            }
            break;
        }
        case 2: {
            string firstName, lastName, username, password;
            cout << "Enter first name: ";
            cin >> firstName;
            cout << "Enter last name: ";
            cin >> lastName;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (createAccount(firstName, lastName, username, password)) {
                cout << "Account created successfully!" << endl;
            }
            else {
                cout << "Error creating account." << endl;
            }
            break;
        }
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }

    return 0;
}
