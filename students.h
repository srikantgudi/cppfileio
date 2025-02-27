#ifndef __STUDENTS_H
#define __STUDENTS_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

using namespace std;

const int RECORD_VALID = 0;
const int RECORD_EXISTS = 1;
const int RECORD_INVALID = 2;
const int RECORD_DUP_NAME = 3;

struct StudentRecord {
    int rollno;
    char fname[30];
    char lname[30];
    char grade[10];
};

class StudentClass {
    StudentRecord student;
    int studf;
    string filename;
    long record_num;
public:
    StudentClass();
    ~StudentClass();
    int input(StudentRecord &s);
    void addRecord();
    bool isExisting(const StudentRecord & s, const string & str);
    void writeToFile(const StudentRecord & s);
    void rewind();
    long readFromFile(StudentRecord & s);
    void showList();
    void filterList();
    void menu();
};
#endif
