#pragma once
#include "User.h"
#include "Book.h" // Assuming Book class is defined in Book.h
#include <vector>

class Student : public User
{
private:
    int fineDue;
    long long int dateofcheck;
    std::vector<Book> booksBorrowed;
    std::vector<Book> borrowHistory;

public:
    Student(const std::string &uname, const std::string &pwd, int fine = 0, long long int date = time(0), const std::vector<Book> &books = std::vector<Book>(), const std::vector<Book> &history = std::vector<Book>());
    Student();

    int getFineDue() const;
    long long int getdateofcheck() const;
    void setFineDue(int fine);
    void setdateofcheck(long long int date);
    std::vector<Book> getBooksBorrowed() const;
    std::vector<Book> getBorrowHistory() const;
    static std::vector<Student> readStudentData();
    static void writeStudentData(const std::vector<Student> &students);
    void updateStudentData(const Student &updatedStudent);
    void borrowBook(const std::string &bookname);
    void returnBook(const std::string &bookname);
    void displayBorrowedBooks() const;
    void displayBorrowHistory() const;
    void payFine();
    int calculateFine();
};
