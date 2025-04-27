#include "../../lib/ui/IntroScreen.h"
#include "../../lib/ui/ANSI.h"
#include "../../lib/Utils.h"
#include <thread>

using namespace std;

int showIntro() {
    const int width = 120;
    const int height = 30;

    ansi::hideCursor();

    string title = R"(
Welcome to the Bank!
FinanceDesk v1
* $100 |                 _        /  *
*  $90 |             ___/ \      /   *
*  $80 |            /      \_   /    *
*  $70 |       _   /         \_/     *
*  $60 |   ___/ \_/                  *
*  $50 |__/                          *
*      |---------------------------- *
)";
    //TODO: add fake loading bar

    vector splitTitle = Utils::splitString(title, '\n');

    for (int y = 1; y < height; y++) {
        ansi::moveCursor(0, y);

        bool isTitleLine = false;
        string title;

        for (int i = 0; i < splitTitle.size(); i++) {
            // make vertically centered
            if (y == height / 2 - splitTitle.size() / 2 + i) {
                cout << string(width / 2 - splitTitle[i].length() / 2, '*') + splitTitle[i] + string(width / 2 - splitTitle[i].length() / 2, '*');
                title = string(width / 2 - splitTitle[i].length() / 2, ' ') + splitTitle[i] + string(width / 2 - splitTitle[i].length() / 2, ' ');
                isTitleLine = true;
            }
        }

        if (!isTitleLine)
            cout << string(width, '*');


        //cout.flush();
        this_thread::sleep_for(chrono::milliseconds(1000 / 60));

        ansi::moveCursor(0, y);
        ansi::clearLine();

        if (isTitleLine) {
            cout << title;
        }
    }

    ansi::moveCursor(0, height + 2);
    ansi::showCursor();

    return 0;
}
