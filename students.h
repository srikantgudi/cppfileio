#ifndef __STUDENTS_H
#define __STUDENTS_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <ncurses.h>

using namespace std;

const int RECORD_VALID = 0;
const int RECORD_EXISTS = 1;
const int RECORD_INVALID = 2;
const int RECORD_DUP_NAME = 3;

struct StudentModel {
    int id;
    char fname[30];
    char lname[30];
    char grade[10];

    string fullname() {
        string n = "";
        n.append(fname);
        n.append(" ");
        n.append(lname);
        return n;
    }
};

class StudentFile {
    StudentModel studentRec;
    string filename;
    long recno;
    int f;
public:
    StudentFile();
    ~StudentFile();
    long reccount();
    long recsize();
    void goTop();
    StudentModel newId();
    void goRec(long _recno);
    long readRow(StudentModel& s);
    void writeNew(const StudentModel& s);
    bool input(StudentModel & s);
    bool isExisting(const StudentModel& s, string = "id");
    void list();
};

#endif
