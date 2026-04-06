#include "Student.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>

// Helper function to convert time_t to ddmmyyyy format
std::string formatDate(time_t rawTime)
{
    struct tm *timeInfo;
    char buffer[11]; // Buffer to hold the formatted date

    timeInfo = localtime(&rawTime);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeInfo);

    return std::string(buffer);
}

Student::Student(const std::string &uname, const std::string &pwd, int fine, long long int date, const std::vector<Book> &books, const std::vector<Book> &history)
    : User(uname, pwd), fineDue(fine), dateofcheck(date), booksBorrowed(books), borrowHistory(history) {}

Student::Student() : User("", ""), fineDue(0), dateofcheck(0) {}

int Student::getFineDue() const
{
    return fineDue;
}

std::vector<Book> Student::getBooksBorrowed() const
{
    return booksBorrowed;
}

std::vector<Book> Student::getBorrowHistory() const
{
    return borrowHistory;
}

void Student::writeStudentData(const std::vector<Student> &students)
{
    std::ofstream file("students.csv");
    if (!file.is_open())
    {
        std::cerr << "Unable to open students.csv file for writing!" << std::endl;
        return;
    }

    for (const auto &student : students)
    {
        file << student.getUsername() << "," << student.getPassword() << ","
             << student.getFineDue() << "," << student.getdateofcheck() << ",";

        // Write booksBorrowed array
        file << "[";
        auto booksBorrowed = student.getBooksBorrowed();
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
        auto borrowHistory = student.getBorrowHistory();
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

std::vector<Student> Student::readStudentData()
{
    std::vector<Book> books = Book::readBookData();
    std::vector<Student> students;
    std::ifstream file("students.csv");
    if (!file.is_open())
    {
        std::cerr << "Unable to open students.csv file!" << std::endl;
        return students;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string username, password, token;
        int fine;
        long long int date;
        std::vector<Book> booksBorrowed;
        std::vector<Book> borrowHistory;

        // Read username
        if (!std::getline(ss, username, ','))
            continue;

        // Read password
        if (!std::getline(ss, password, ','))
            continue;

        // Read fine
        if (!std::getline(ss, token, ','))
            continue;
        fine = std::stoi(token);

        // Read date
        std::string dateStr;
        if (!std::getline(ss, dateStr, ','))
            continue;
        if (dateStr == "[]")
        {
            date = 0;
        }
        else
        {
            date = std::stoll(dateStr);
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

        // Read borrow history
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
        students.emplace_back(username, password, fine, date, booksBorrowed, borrowHistory);
    }

    file.close();
    return students;
}
void Student::updateStudentData(const Student &updatedStudent)
{

    std::vector<Student> students = readStudentData();
    bool found = false;
    for (auto &student : students)
    {
        if (student.getUsername() == updatedStudent.getUsername())
        {
            student = updatedStudent;
            found = true;
            break;
        }
    }

    if (!found)
    {
        std::cerr << "Student with username " << updatedStudent.getUsername() << " not found!" << std::endl;
        return;
    }

    Student::writeStudentData(students);
    return;
}

void Student::borrowBook(const std::string &bookname)
{
    std::vector<Book> books = Book::readBookData();
    for (auto &book : books)
    {
        if (book.getName() == bookname)
        {
            if (booksBorrowed.size() == 3)
            {
                std::cout << "You have already borrowed 3 books." << std::endl;
                return;
            }
            if (book.getAvailable() == 1 && booksBorrowed.size() < 3 && fineDue == 0)
            {
                book.setAvailable(0);
                book.setBorrowDate(time(0));
                book.setToWhom(getUsername());
                booksBorrowed.push_back(book);
                updateStudentData(*this);
                Book::writeBookData(books);

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

void Student::returnBook(const std::string &bookname)
{
    std::vector<Book> books = Book::readBookData();
    for (auto &book : books)
    {
        if (book.getName() == bookname && book.getToWhom() == getUsername())
        {
            book.setAvailable(1);

            book.setToWhom("null");
            for (auto it = booksBorrowed.begin(); it != booksBorrowed.end(); ++it)
            {
                if (it->getName() == bookname)
                {
                    booksBorrowed.erase(it);
                    break;
                }
            }
            borrowHistory.push_back(book);
            Book::writeBookData(books);
            Student::updateStudentData(*this);
            book.setBorrowDate(0);
            return;
        }
    }
    std::cout << "Book is not borrowed." << std::endl;
}

void Student::displayBorrowedBooks() const
{
    for (const auto &book : booksBorrowed)
    {
        std::cout << "Book Name: " << book.getName() << std::endl;
        std::cout << "Author: " << book.getAuthor() << std::endl;
        std::cout << "Publisher: " << book.getPublisher() << std::endl;
        std::cout << "Year: " << book.getYear() << std::endl;
        std::cout << "ISBN: " << book.getIsbn() << std::endl;
        std::cout << "Borrowed Date: (in ddmmyyyy)" << formatDate(book.getBorrowDate()) << std::endl;
        std::cout << "Return Date: (in ddmmyyyy)" << formatDate(book.getBorrowDate() + 604800) << std::endl; // Corrected return date calculation
        std::cout << "-------------------" << std::endl;
    }
}

void Student::displayBorrowHistory() const
{
    for (const auto &book : borrowHistory)
    {
        std::cout << "Book Name: " << book.getName() << std::endl;
        std::cout << "Author: " << book.getAuthor() << std::endl;
        std::cout << "Publisher: " << book.getPublisher() << std::endl;
        std::cout << "Year: " << book.getYear() << std::endl;
        std::cout << "ISBN: " << book.getIsbn() << std::endl;
        std::cout << "Borrowed Date: (in ddmmyyyy)" << formatDate(book.getBorrowDate()) << std::endl;
        // std::cout << "Return Date: " << formatDate(book.getBorrowDate() + 604800) << std::endl; // Corrected return date calculation
        std::cout << "-------------------" << std::endl;
    }
}

void Student::payFine()
{
    fineDue = 0;
    Student::updateStudentData(*this);
}

void Student::setFineDue(int fine)
{
    fineDue = fine;
    Student::updateStudentData(*this);
}

void Student::setdateofcheck(long long int date)
{
    dateofcheck = date;
    Student::updateStudentData(*this);
}

long long int Student::getdateofcheck() const
{
    return dateofcheck;
}

int Student::calculateFine()
{
    int fine = getFineDue();
    time_t currentTime = time(0);

    for (const auto &book : booksBorrowed)
    {
        long long int t1 = getdateofcheck();

        time_t elapsedSeconds = currentTime - t1;

        int daysElapsed = elapsedSeconds / (3600 * 24);

        if (daysElapsed > 15)
        {
            int extraDays = daysElapsed - 15;
            fine += 10 * extraDays;
        }
    }
    setdateofcheck(currentTime);
    setFineDue(fine);
    updateStudentData(*this);
    return fine;
}
