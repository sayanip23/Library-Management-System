# Library-Management-System

# Introduction
This program implements a basic library management system
where students, faculty and librarians can perform various tasks related to
borrowing books, creating profiles, paying fines, returning books etc. Users
interact with the system through a console-based interface.

# Key Features
# User Authentication
Librarians, Students and Teachers can log in using their
usernames and passwords. New users can register. The system verifies user
credentials before granting access to the functionalities.
Student Functions: Students can-
1) Return Books
2) Borrow Books
3) Check Fines
4) Pay Fines
5) Check Borrowed Books
6) View Borrowing History
7) View Available Books
# Employee Functions
Employees have similar functionalities as customers but
with additional privileges like no fines. In addition, they can view the maximum
overdue period of any of their books.
# Librarian Functions
Librarians have administrative privileges to manage users
and books. They can register new users, update and add books, delete books
and users, view database details
# Data Persistence
Book, Librarian, Faculty, and Student data are stored in external
.csv files (faculty.csv, books.csv, librarians.csv, students.csv) to maintain
persistence between program executions.
# Error Handling
The program provides error messages and handles invalid
inputs gracefully. Users are prompted with clear instructions throughout the
interaction.
# Usage
# Compilation
Compile the program using a C++ compiler, such as clang:
clang++ -std=c++11 main.cpp User.cpp Book.cpp Librarian.cpp Faculty.cpp
Student.cpp -o lms
# Execution
Run the compiled executable:
./lms
Follow the prompts on the console to navigate through different functionalities.
Enter any integer other than the ones displayed to exit the program.
