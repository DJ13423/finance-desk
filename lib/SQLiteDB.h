#pragma once

#include <string>
#include "../include/sqlite3.h"
#include <iostream>

class SQLiteStatement;

class SQLiteDB {
public:
    sqlite3* db;
    SQLiteStatement* stmt;

    SQLiteDB(const std::string& filename);
    ~SQLiteDB();

    void exec(const std::string& sql);
    SQLiteStatement* prepareStatement(const std::string& sql);
};

class SQLiteStatement {
public:
    sqlite3_stmt* stmt;

    ~SQLiteStatement();
    int step();
    int getColumnCount();
    int getColumnType(int col);
    const unsigned char* getColumnText(int col);
    std::string getColumnTextStr(int col);
    const char* getColumnName(int col);
    int getColumnInt(int col);
    double getColumnDouble(int col);
    int bindInt(int index, int value);
    int bindDouble(int index, double value);
    int bindText(int index, const std::string& value);
    int bindNull(int index);
    int finalize();
};

enum class SQLiteTypes {
    INTEGER = SQLITE_INTEGER,
    FLOAT = SQLITE_FLOAT,
    BLOB = SQLITE_BLOB,
    NULL_TYPE = SQLITE_NULL,
    TEXT = SQLITE3_TEXT
};

