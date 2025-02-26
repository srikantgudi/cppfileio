#include <iostream>
#include "students.h"

using namespace std;

int main() {
    initscr();
    int mainOpt = 0;
    do {
        clear();
        attron(A_REVERSE);
        mvaddstr(2,20,"FILE I/O Demo App");
        attroff(A_REVERSE);
        mvaddstr(4,20, "s. Students Management");
        mvaddstr(6,20, "q. Quit");
        mvaddstr(8,20, "? ");
        mainOpt = getch();
        if (mainOpt == 's') {
            StudentClass studentObj;
            studentObj.menu();
        }
    } while (mainOpt != 'q');
    refresh();
    endwin();
    return 0;
}
