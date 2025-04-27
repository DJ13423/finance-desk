#pragma once

#include <string>

class Loan
{
	public:
	int loanID;
	int accountID;
	double amount;
	double interestRate;
	std::string startDate;
	std::string endDate;
	std::string status;

	Loan(int loanID, int accountID, double amount, double interestRate, const std::string& startDate, const std::string& endDate, std::string status = "active")
		: loanID(loanID), accountID(accountID), amount(amount), interestRate(interestRate), startDate(startDate), endDate(endDate), status(status) {}
};

