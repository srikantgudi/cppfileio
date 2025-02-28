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
    clear();
    s = newId();
    mvprintw(4, 5, "ID: %d", s.id);
    mvaddstr(5,5, "First Name: ");
    mvgetnstr(5,20, s.fname, sizeof(s.fname));
    if (!strlen(s.fname)) return false;
    mvaddstr(6,5, "Last Name: ");
    mvgetnstr(6,20,s.lname, sizeof(s.lname));
    if (!strlen(s.lname)) return false;
    if (isExisting(s, "name")) {
        mvaddstr(getcury(stdscr)-1, 40,"Duplicate name");
        return false;
    }
    mvaddstr(7,5, "Grade: ");
    mvgetnstr(7,20, s.grade, sizeof(s.grade));
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
