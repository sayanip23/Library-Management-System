#include "Faculty.h"
#include "Student.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>

Faculty::Faculty(const std::string &uname, const std::string &pwd, int max_overdue, const std::vector<Book> &books, const std::vector<Book> &history)
    : User(uname, pwd), max_overdue(max_overdue), booksBorrowed(books), Borrow_History(history) {}
Faculty::Faculty() : User("", ""), max_overdue(0) {}

std::vector<Faculty> Faculty::readFacultyData()
{
    std::vector<Book> books = Book::readBookData();
    std::vector<Faculty> faculties;
    std::ifstream file("faculty.csv");
    if (!file.is_open())
    {
        std::cerr << "Unable to open faculty.csv file!" << std::endl;
        return faculties;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string username, password, token;
        std::string token2;
        int maxoverdue;
        std::vector<Book> booksBorrowed;
        std::vector<Book> borrowHistory;

        if (!std::getline(ss, username, ','))
            continue;

        if (!std::getline(ss, password, ','))
            continue;

        if (!std::getline(ss, token, ','))
            continue;

        if (token != "null")
        {
            maxoverdue = std::stoi(token);
        }

        if (!std::getline(ss, token, ','))
            continue;
        token = token.substr(1, token.length() - 2); // Remove [ and ]

        std::stringstream borrowedSS(token);
        std::string bookName;
        while (std::getline(borrowedSS, bookName, ';'))
        {
            if (!bookName.empty())
            {
                for (auto book : books)
                {
                    if (book.getName() == bookName)
                    {
                        booksBorrowed.push_back(book);
                        break;
                    }
                }
            }
        }

        if (!std::getline(ss, token))
            continue;
        token = token.substr(1, token.length() - 2); // Remove [ and ]

        std::stringstream historySS(token);
        while (std::getline(historySS, bookName, ';'))
        {
            if (!bookName.empty())
            {
                for (auto book : books)
                {
                    if (book.getName() == bookName)
                    {
                        borrowHistory.push_back(book);
                        break;
                    }
                }
            }
        }

        // Create and add the student
        faculties.emplace_back(username, password, maxoverdue, booksBorrowed, borrowHistory);
    }

    file.close();
    return faculties;
}

void Faculty::writeFacultyData(std::vector<Faculty> &faculties)
{
    std::ofstream file("faculty.csv");
    if (!file.is_open())
    {
        std::cerr << "Unable to open faculty.csv file for writing!" << std::endl;
        return;
    }
    for (auto &faculty : faculties)
    {
        file << faculty.getUsername() << "," << faculty.getPassword() << "," << faculty.getMaxOverdue() << ",";

        // Write booksBorrowed array
        file << "[";
        auto booksBorrowed = faculty.getBooksBorrowed();
        if (!booksBorrowed.empty())
        {
            file << booksBorrowed[0].getName();
            for (int i = 1; i < booksBorrowed.size(); i++)
            {
                file << ";" << booksBorrowed[i].getName();
            }
        }
        file << "],";

        // Write borrowHistory array
        file << "[";
        auto borrowHistory = faculty.getBorrowHistory();
        if (!borrowHistory.empty())
        {
            file << borrowHistory[0].getName();
            for (int i = 1; i < borrowHistory.size(); ++i)
            {
                file << ";" << borrowHistory[i].getName();
            }
        }
        file << "]\n";
    }
    file.close();
}

int Faculty::getMaxOverdue()
{
    for (auto books : booksBorrowed)
    {
        if (books.getBorrowDate() != 0)
        {
            time_t now = time(0);
            int diff = difftime(now, books.getBorrowDate()) / (3600 * 24); // Convert seconds to days
            if (diff > max_overdue)
            {
                max_overdue = diff;
            }
        }
    }
    return max_overdue;
}

std::vector<Book> Faculty::getBooksBorrowed() const
{
    return booksBorrowed;
}

std::vector<Book> Faculty::getBorrowHistory() const
{
    return Borrow_History;
}
void Faculty::updateFacultyData(const Faculty &updatedfaculty)
{
    std::vector<Faculty> faculties = Faculty::readFacultyData();
    for (auto &faculty : faculties)
    {
        if (faculty.getUsername() == updatedfaculty.getUsername())
        {
            faculty = updatedfaculty;
            Faculty::writeFacultyData(faculties);
            return;
        }
    }
    std::cerr << "Faculty not found." << std::endl;
}
void Faculty::borrowBook(const std::string &bookname)
{
    std::vector<Book> books = Book::readBookData();
    for (auto &book : books)
    {
        if (book.getName() == bookname)
        {
            if (booksBorrowed.size() == 5)
            {
                std::cout << "You have already borrowed 5 books." << std::endl;
                return;
            }
            if (book.getAvailable() == 1 && booksBorrowed.size() < 5 && max_overdue < 60)
            {
                std::cout << "Book borrowed successfully!" << std::endl;
                book.setAvailable(0);
                book.setBorrowDate(time(0));
                book.setToWhom(getUsername());
                booksBorrowed.push_back(book);
                Book::writeBookData(books);
                Faculty::updateFacultyData(*this);
                return;
            }
            else
            {
                std::cout << "Book is not available for borrowing." << std::endl;
                return;
            }
        }
    }
}

void Faculty::returnBook(const std::string &bookname)
{
    std::vector<Book> books = Book::readBookData();
    for (auto &book : books)
    {
        if (book.getName() == bookname && book.getToWhom() == getUsername())
        {
            if (time(0) - book.getBorrowDate() > 30 * 24 * 3600) // Corrected return date calculation
            {
                std::cout << "You have exceeded the return date." << std::endl;
            }
            book.setAvailable(1);
            book.setBorrowDate(0);
            book.setToWhom("null");
            for (auto it = booksBorrowed.begin(); it != booksBorrowed.end(); ++it)
            {
                if (it->getName() == bookname)
                {
                    booksBorrowed.erase(it);
                    break;
                }
            }
            std::cout << "Successfully returned book." << std::endl;
            Borrow_History.push_back(book);
            Book::writeBookData(books);
            Faculty::updateFacultyData(*this);
            return;
        }
    }
    std::cout << "Unable to return the book. Invalid Input" << std::endl;
}

std::string formatDate6(time_t rawTime)
{
    struct tm *timeInfo;
    char buffer[11]; // Buffer to hold the formatted date

    timeInfo = localtime(&rawTime);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeInfo);

    return std::string(buffer);
}
void Faculty::displayBorrowedBooks() const
{
    for (const auto &book : booksBorrowed)
    {
        std::cout << "Book Name: " << book.getName() << std::endl;
        std::cout << "Author: " << book.getAuthor() << std::endl;
        std::cout << "Publisher: " << book.getPublisher() << std::endl;
        std::cout << "Year: " << book.getYear() << std::endl;
        std::cout << "ISBN: " << book.getIsbn() << std::endl;
        std::cout << "Borrowed Date: " << formatDate6(book.getBorrowDate()) << std::endl;
        std::cout << "-------------------" << std::endl;
    }
}

void Faculty::displayBorrowHistory() const
{
    for (const auto &book : Borrow_History)
    {
        std::cout << "Book Name: " << book.getName() << std::endl;
        std::cout << "Author: " << book.getAuthor() << std::endl;
        std::cout << "Publisher: " << book.getPublisher() << std::endl;
        std::cout << "Year: " << book.getYear() << std::endl;
        std::cout << "ISBN: " << book.getIsbn() << std::endl;
        std::cout << "Borrowed Date: " << formatDate6(book.getBorrowDate()) << std::endl;
        std::cout << "-------------------" << std::endl;
    }
}
