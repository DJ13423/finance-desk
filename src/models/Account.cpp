#include "../../lib/models/Account.h"

void Account::deposit(double amount) { // TODO: need to handle this error better
	if (amount <= 0) {
		std::cerr << "Deposit amount must be positive." << std::endl;
		return;
	}
	balance += amount;
}
