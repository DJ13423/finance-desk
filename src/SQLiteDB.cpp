#include "../lib/SQLiteDB.h"
#include <iostream>
#include <string>
#include "../include/sqlite3.h"
#include "../lib/Utils.h"


SQLiteDB::SQLiteDB(const std::string& filename) {
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }
}

SQLiteDB::~SQLiteDB() {
    if (db) {
        sqlite3_close(db);
    }
}

void SQLiteDB::exec(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}
SQLiteStatement* SQLiteDB::prepareStatement(const std::string& sql) {
    stmt = new SQLiteStatement();
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt->stmt, nullptr) != SQLITE_OK) {
		Utils::ThrowFatalError("Failed to prepare statement:", sqlite3_errmsg(db));
		return nullptr;
	}
	return stmt;
}

// SQLiteStatement

SQLiteStatement::~SQLiteStatement() {
	if (stmt) {
		sqlite3_finalize(stmt);
	}
}

int SQLiteStatement::step() {
    return sqlite3_step(stmt);
}

int SQLiteStatement::getColumnCount() {
	return sqlite3_column_count(stmt);
}

int SQLiteStatement::getColumnType(int col) {
    return sqlite3_column_type(stmt, col);
}

const unsigned char* SQLiteStatement::getColumnText(int col) {
	return sqlite3_column_text(stmt, col);
}

std::string SQLiteStatement::getColumnTextStr(int col) {
	return reinterpret_cast<const char*>(sqlite3_column_text(stmt, col));
}

const char* SQLiteStatement::getColumnName(int col) {
	return sqlite3_column_name(stmt, col);
}

int SQLiteStatement::getColumnInt(int col) {
	return sqlite3_column_int(stmt, col);
}

double SQLiteStatement::getColumnDouble(int col) {
	return sqlite3_column_double(stmt, col);
}

int SQLiteStatement::bindInt(int index, int value) {
	return sqlite3_bind_int(stmt, index, value);
}

int SQLiteStatement::bindDouble(int index, double value) {
	return sqlite3_bind_double(stmt, index, value);
}

int SQLiteStatement::bindText(int index, const std::string& value) {
	if (!stmt) {
		std::cerr << "Error: stmt is null!" << std::endl;
		return SQLITE_ERROR;
	}
	return sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_STATIC);
}

int SQLiteStatement::bindNull(int index) {
	return sqlite3_bind_null(stmt, index);
}

int SQLiteStatement::finalize() {
	return sqlite3_finalize(stmt);
}
