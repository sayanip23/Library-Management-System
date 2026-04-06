#pragma once
#include "User.h"
#include "Book.h"
#include <vector>

class Faculty : public User
{
private:
    int max_overdue;
    std::vector<Book> booksBorrowed;
    std::vector<Book> Borrow_History;

public:
    Faculty(const std::string &uname, const std::string &pwd, int max_overdue = 0, const std::vector<Book> &books = std::vector<Book>(), const std::vector<Book> &history = std::vector<Book>());
    Faculty();

    std::vector<Book> getBooksBorrowed() const;
    std::vector<Book> getBorrowHistory() const;
    int getMaxOverdue();
    static std::vector<Faculty> readFacultyData();
    static void writeFacultyData(std::vector<Faculty> &faculty);
    void updateFacultyData(const Faculty &updatedfaculty);

    void borrowBook(const std::string &bookname);
    void returnBook(const std::string &bookname);
    void displayBorrowedBooks() const;
    void displayBorrowHistory() const;
};
