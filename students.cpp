#include <iostream>
#include "students.h"

const int ERR_ROW = 15;

StudentClass::StudentClass() {
    record_num = 0;
    filename = "students.dat";
    studf = open64(filename.c_str(),O_RDWR | O_CREAT, 0755);
}
StudentClass::~StudentClass() {
    wrefresh(mainw);
    endwin();
    close(studf);
}
int StudentClass::input(StudentRecord &s) {
    wclear(mainw);
    char rollno[5];
    wattron(mainw,A_REVERSE|A_BOLD);
    mvwaddstr(mainw,1,10,"Add New Student");
    wattroff(mainw,A_REVERSE|A_BOLD);
    mvwaddstr(mainw,3,10,"Roll No.");
    mvwaddstr(mainw,3,40,"(0 to exit)");
    mvwgetstr(mainw,3,30,rollno);
    if (!strlen(rollno)) {
        return RECORD_INVALID;
    } else {
        s.rollno = stoi(rollno);
        if (!s.rollno) {
            return RECORD_INVALID;
        }
    }
    if (isExisting(s)) {
        return RECORD_EXISTS;
    }
    mvwaddstr(mainw,5,10,"First name: ");
    mvwgetstr(mainw,5,30,s.fname);
    if (!strlen(s.fname)) {
        return RECORD_INVALID;
    }
    mvwaddstr(mainw, 7,10,"Last name: ");
    mvwgetstr(mainw, 7,30, s.lname);
    if (!strlen(s.lname)) {
        return RECORD_INVALID;
    }
    mvwaddstr(mainw,9,10, "Grade: ");
    mvwgetstr(mainw,9,30, s.grade);
    if (!strlen(s.grade)) {
        return RECORD_INVALID;
    }
    writeToFile(s);
    return RECORD_VALID;
}
void StudentClass::addRecord() {
    wclear(mainw);
    int status = input(student);
    if (status == RECORD_VALID) {
        mvwaddstr(mainw,ERR_ROW,10,"Added new record..");
    } else {
        if (status == RECORD_EXISTS) {
            mvwaddstr(mainw,ERR_ROW,10,"Record already exists..");
        } else if (status == RECORD_INVALID) {
            mvwaddstr(mainw,ERR_ROW,10,"Invalid input..");
        }
    }
    mvwaddstr(mainw,ERR_ROW+1,10,"Press a key to continue..");
    wrefresh(mainw);
    getch();
    wclear(mainw);
}
bool StudentClass::isExisting(const StudentRecord & s) {
    StudentRecord tmp;
    rewind();
    bool found = false;
    while (readFromFile(tmp) && !found) {
        if (tmp.rollno == s.rollno) {
            found = true;
            record_num = lseek64(studf,0,SEEK_CUR)-sizeof(s);
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
    wclear(mainw);
    wattron(mainw, A_REVERSE|A_BOLD);
    mvwaddstr(mainw, 1, 5, "Students List");
    wattroff(mainw, A_REVERSE|A_BOLD);
    wmove(mainw,3,0);
    while (readFromFile(student) > 0) {
        wprintw(mainw, "%10d %-30s %-30s %-10s\n", student.rollno, student.fname, student.lname, student.grade);
    }
    waddstr(mainw, "Press a key to continue..");
    wgetch(mainw);
}
void StudentClass::menu() {
    initscr();
    mainw = newwin(60,80, 1,1);
    box(mainw,0,0);
    keypad(mainw, true);
    wrefresh(mainw);
    char opt;
    while (opt != 'q') {
        wclear(mainw);
        wattron(mainw, A_REVERSE|A_BOLD);
        mvwaddstr(mainw,2,10,"Students Management System");
        wattroff(mainw, A_REVERSE|A_BOLD);
        mvwaddstr(mainw,4,10,"a. Add");
        mvwaddstr(mainw,5,10,"l. List");
        mvwaddstr(mainw,6,10,"q. Quit");
        mvwaddstr(mainw,8,10,"?");
        wrefresh(mainw);
        opt = wgetch(mainw);
        if (opt == 'a') {
            addRecord();
        } else if (opt == 'l') {
            showList();
        }
    }
    cout << endl << "Thank you for using the app" << endl;
}
