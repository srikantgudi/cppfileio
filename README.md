# C++ low level file i/o demo

### This is a C++ application to demonstrate data manipulation using low level files.
### Uses `ncurses` library for console I/O

#### creates file `students.dat` for storing data

### Include files:
- `fcntl.h` for `open` / `create`
- `unistd.h` for `close`

### To build the app

```
make
```
- runs the command `g++ -std=c++23 students.cpp main.cpp -o main -lncurses`
- `-o main` creates the executable file

### To run the app
```
./main
```

### Listing records

To filter records, enter values for `Last name` and/or `Grade` search fields.
Leave blank to show all

### Screen shots
- [FileIO Main Screen](./screenshots/fileio_demo.png)
- [Data List Screen](./screenshots/data_list.png)
- [Data List filtered](./screenshots/data_list_filtered.png)
- [New Data Screen](./screenshots/fileio_newdata.png)