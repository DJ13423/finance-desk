#include "../include/sqlite3.h"
#include "../lib/models/Account.h"
#include "../lib/ui/ANSI.h"
#include "../lib/services/Authentication.h"
#include "../lib/services/GameDatabaseManager.h"
#include "../lib/SQLiteDB.h"
#include "../lib/models/Transaction.h"
#include <chrono>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include "../lib/Utils.h"
#include "../lib/ui/ApplicationPages.h"
#include "../lib/ui/IntroScreen.h"

using namespace std;

// TODO: implement back button. Via '-' or 'exit'
int main() {
    GameDatabaseManager::initializeDatabase();
    showIntro();
    while (true) {
        ApplicationPages::StartPage();
        switch (Utils::getIntCInput()) {
            case 1: {
                ApplicationPages::LoginPage();
                break;
            }
            case 2: {
                ApplicationPages::SignupPage();
                break;
            }
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
    }

    return 0;
}

