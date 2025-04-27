#include "../../lib/services/GameContext.h"
#include "../../lib/models/Account.h"

std::optional<Account> GameContext::currentLoggedInAccount = std::nullopt;
Account* GameContext::bankMainAccount = nullptr;
