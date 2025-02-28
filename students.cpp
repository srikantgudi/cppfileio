#include "students.h"
#include <cstdio>
#include <cstring>
#include <ncurses.h>

using namespace std;

StudentFile::StudentFile() {
    filename = "students.dat";
    f = open(filename.c_str(), O_RDWR | O_CREAT);
}
StudentFile::~StudentFile() {
    close(f);
}
long StudentFile::reccount() {
    return lseek(f, 0, SEEK_END) / sizeof(StudentModel);
}
long StudentFile::recsize() {
    return sizeof(StudentModel);
}

StudentModel StudentFile::newId() {
    StudentModel s;
    lseek(f, (reccount()-1)*recsize(), SEEK_SET);
    read(f, &s, recsize());
    s.id++;
    strcpy(s.fname,"");
    strcpy(s.lname,"");
    return s;
}
void StudentFile::goTop() {
    lseek(f, 0, SEEK_SET);
}
void StudentFile::goRec(long _recno) {
    lseek(f, (_recno) * recsize(), SEEK_SET);
}
long StudentFile::readRow(StudentModel& s) {
    return read(f,&s,recsize());
}
void StudentFile::writeNew(const StudentModel& s) {
    lseek(f,0,SEEK_END);
    write(f, &s, recsize());
}

bool StudentFile::input(StudentModel & s) {
    char idnum[20];
    s = newId();
    mvprintw(4,10, "ID: %d", s.id);
    mvaddstr(5,10, "First Name: ");
    mvgetnstr(5,30, s.fname, sizeof(s.fname));
    if (!strlen(s.fname)) return false;
    mvaddstr(6,10, "Last Name: ");
    mvgetnstr(6,30,s.lname, sizeof(s.lname));
    if (!strlen(s.lname)) return false;
    if (isExisting(s, "name")) {
        mvaddstr(getcury(stdscr)-1, 40,"Duplicate name");
        return false;
    }
    mvaddstr(7,10, "Grade: ");
    mvgetnstr(7,30, s.grade, sizeof(s.grade));
    if (!strlen(s.grade)) return false;
    mvaddstr(getcury(stdscr)+2, 10, "a) Add | c) Cancel");
    int ok = getch();
    return ok == 'a';
}

bool StudentFile::isExisting(const StudentModel& s, string what) {
    StudentModel ns;
    lseek(f, 0, SEEK_SET);
    bool found = false;
    while (readRow(ns)) {
        if (what == "id") {
            if (ns.id == s.id) {
                found = true;
                break;
            }
        } else if (what == "name") {
            if (!strcmp(ns.fname,s.fname) && !strcmp(ns.lname,s.lname)) {
                found = true;
                break;
            }
        }
    }
    return found;
}
void StudentFile::list() {
    StudentFile stf;
    StudentModel stm;
    int opt;
    bool filtered = false;
    char lname[30] {" "};
    char grade[10] {" "};
    
    while (opt != 'q') {
        clear();
        attron(A_REVERSE|A_BOLD);
        mvaddstr(1,10,"Data list");
        attroff(A_REVERSE|A_BOLD);
        mvaddstr(2,10,"Filter: Leave both the fields blank for all");
        mvaddstr(3,10,"Last name: ");
        mvaddstr(4,10,"Grade: ");
        mvgetstr(3,25,lname);
        mvgetstr(4,25,grade);
        stf.goTop();
        while (stf.readRow(stm)) {
            if (strcasestr(stm.lname,lname) != nullptr && strcasestr(stm.grade,grade) != nullptr) {
                printw("%8d %-30s %-30s %-10s\n", stm.id, stm.fname, stm.lname, stm.grade);
            }
        }
        mvaddstr(getcury(stdscr)+2,10,"Press a key to continue...(q) to Quit");
        opt = getch();
    }
}
