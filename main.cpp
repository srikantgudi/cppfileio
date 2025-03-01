#include <cstdio>
#include <iostream>
#include <ncurses.h>
#include "students.h"

using namespace std;

int main() {
    initscr();
    refresh();
    keypad(stdscr, true);
    int opt = '?';
    StudentFile stf;
    StudentModel stm;
    do {
        clear();
        mvaddstr(1, 10, "Data management System");
        mvprintw(3, 10, "n) New row | l) List | q) Quit ? ");
        opt = getch();
        if (opt == 'n') {
            clear();
            mvaddstr(2,20,"Add new data");
            if (stf.input(stm)) {
                mvprintw(getcury(stdscr)+2, 10, "%d %s %s %s", stm.id, stm.fname, stm.lname, stm.grade);
                stf.writeNew(stm);
            } else {
                mvprintw(getcury(stdscr)+2, 10, "Incomplete data");
            }
        } else if (opt == 'l') {
            clear();
            stf.list();
        }
    } while (opt != 'q');
    endwin();
    return 0;
}
