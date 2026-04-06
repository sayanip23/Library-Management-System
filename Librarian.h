#pragma once
#include "User.h"
#include "Book.h"
#include "Student.h"
#include "Faculty.h"

class Librarian : public User
{
public:
    Librarian(const std::string &uname, const std::string &pwd);
    Librarian();

    static std::vector<Librarian> readLibrarianData();
    static void writeLibrarianData(const std::vector<Librarian> &librarian);
    void registerNewStudent() const;
    void registerNewFaculty() const;
    void DeleteBook(const std::string &bookname);
    void deleteStudent(const std::string &username);
    void deleteFaculty(const std::string &username);
    void addBook(const std::string &name, const std::string &author, const std::string &publisher,
                 int year, const std::string &isbn, int available, int date, const std::string &towhom);
    void updateBook(const std::string &bookname, const std::string &newname, const std::string &newauthor, const std::string &newpublisher,
                    int newyear, const std::string &newisbn, int newavailable, int newdate, const std::string &newtowhom);
    void displayAllBooks() const;
    void displayAllStudents() const;
    void displayAllFaculty() const;
};
