#pragma once
#include "../models/Account.h"
#include <optional>

class GameContext {
public:
    static std::optional<Account> currentLoggedInAccount;
    static Account* bankMainAccount;
};