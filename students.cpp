#include <iostream>
#include <ncurses.h>
#include "students.h"

using namespace std;

const int ERR_ROW = 15;

StudentClass::StudentClass() {
    record_num = 0;
    filename = "students.dat";
    studf = open64(filename.c_str(),O_RDWR | O_CREAT, 0755);
}
StudentClass::~StudentClass() {
    refresh();
    endwin();
    close(studf);
}
int StudentClass::input(StudentRecord &s) {
    clear();
    char rollno[5];
    attron(A_REVERSE|A_BOLD);
    mvaddstr(1,10,"Add New Student");
    attroff(A_REVERSE|A_BOLD);
    mvaddstr(3,10,"Roll No.");
    mvaddstr(3,40,"(0 to exit)");
    mvgetstr(3,30,rollno);
    if (!strlen(rollno)) {
        return RECORD_INVALID;
    } else {
        s.rollno = stoi(rollno);
        if (!s.rollno) {
            return RECORD_INVALID;
        }
    }
    if (isExisting(s, "rollno")) {
        return RECORD_EXISTS;
    }
    mvaddstr(5,10,"First name: ");
    mvgetstr(5,30,s.fname);
    if (!strlen(s.fname)) {
        return RECORD_INVALID;
    }
    mvaddstr(7,10,"Last name: ");
    mvgetstr(7,30, s.lname);
    if (!strlen(s.lname)) {
        return RECORD_INVALID;
    }
    if (isExisting(s,"name")) {
        return RECORD_DUP_NAME;
    }
    mvaddstr(9,10, "Grade: ");
    mvgetstr(9,30, s.grade);
    if (!strlen(s.grade)) {
        return RECORD_INVALID;
    }
    writeToFile(s);
    return RECORD_VALID;
}
void StudentClass::addRecord() {
    clear();
    int status = input(student);
    if (status == RECORD_VALID) {
        mvaddstr(ERR_ROW,10,"Added new record..");
    } else {
        if (status == RECORD_EXISTS) {
            mvaddstr(ERR_ROW,10,"Record already exists..");
        } else if (status == RECORD_DUP_NAME) {
            mvaddstr(ERR_ROW,10,"Duplicate name encountered..");
        } else if (status == RECORD_INVALID) {
            mvaddstr(ERR_ROW,10,"Invalid input..");
        }
    }
    mvaddstr(ERR_ROW+1,10,"Press a key to continue..");
    refresh();
    getch();
    clear();
}
bool StudentClass::isExisting(const StudentRecord & s, const string & col="rollno") {
    StudentRecord tmp;
    rewind();
    bool found = false;
    while (readFromFile(tmp) && !found) {
        if (col == "rollno") {
            if (tmp.rollno == s.rollno) {
                found = true;
                record_num = lseek64(studf,0,SEEK_CUR)-sizeof(s);
            }
        } else if (col == "name") {
            if (!strcmp(tmp.fname, s.fname) && !strcmp(tmp.lname,s.lname)) {
                found = true;
                record_num = lseek64(studf,0,SEEK_CUR)-sizeof(s);
            }
        }
    }
    return found;
}
void StudentClass::writeToFile(const StudentRecord & s) {
    lseek64(studf,0,SEEK_END);
    write(studf, &s, sizeof(s));
}
void StudentClass::rewind() {
    lseek(studf,0,SEEK_SET);
}
long StudentClass::readFromFile(StudentRecord & s) {
    return read(studf, &s,sizeof(s));
}
void StudentClass::showList() {
    rewind();
    clear();
    attron(A_REVERSE|A_BOLD);
    mvaddstr(1, 5, "Students List");
    attroff(A_REVERSE|A_BOLD);
    move(3,0);
    bool show = true;
    while (readFromFile(student) > 0) {
        printw("%10d %-30s %-30s %-10s\n", student.rollno, student.fname, student.lname, student.grade);
    }
    addstr("Press a key to continue..");
    getch();
}
void  StudentClass::filterList() {
    char opt;
    char lname[30], grade[10];
    StudentRecord st;
    do {
        clear();
        attron(A_REVERSE|A_BOLD);
        mvprintw(1,20,"Filter List");
        attroff(A_REVERSE|A_BOLD);
        mvprintw(2,10,"l. Last name | g. Grade | q. Quit");
        opt = getch();
        switch (opt) {
            case 'l':
                mvprintw(3,10,"Last name to filter: ");
                getnstr(lname, 30);
                rewind();
                while (readFromFile(st)) {
                    if (!strcmp(st.lname, lname)) {
                        printw("%10d | %-30s | %-30s | %-10s\n",st.rollno,st.fname,st.lname,st.grade);
                    }
                }
                addstr("Done!");
                getch();
                break;
            case 'g':
                mvprintw(3,10,"Grade to filter: ");
                getnstr(grade, 10);
                rewind();
                while (readFromFile(st)) {
                    if (!strcmp(st.grade, grade)) {
                        printw("%10d | %-30s | %-30s | %-10s\n",st.rollno,st.fname,st.lname,st.grade);
                    }
                }
                addstr("Done!");
                getch();
                break;
            default:
                break;
        }
    } while (opt != 'q');
}
void StudentClass::menu() {
    initscr();
    keypad(stdscr, true);
    refresh();
    char opt;
    while (opt != 'q') {
        clear();
        attron(A_REVERSE|A_BOLD);
        mvaddstr(2,10,"Students Management System");
        attroff(A_REVERSE|A_BOLD);
        mvaddstr(4,10,"a. Add");
        mvaddstr(5,10,"l. List");
        mvaddstr(6,10,"f. Filter");
        mvaddstr(7,10,"q. Quit");
        mvaddstr(8,10,"?");
        refresh();
        opt = getch();
        switch (opt) {
            case 'a':
                addRecord(); break;
            case 'l':
                showList(); break;
            case 'f':
                filterList(); break;
        }
    }
    cout << endl << "Thank you for using the app" << endl;
}
