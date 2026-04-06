#include "Librarian.h"
#include "User.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Librarian::Librarian(const std::string &uname, const std::string &pwd)
    : User(uname, pwd) {}
Librarian::Librarian() : User("", "") {}

std::vector<Librarian> Librarian::readLibrarianData()
{
    std::vector<Librarian> librarians;
    std::ifstream file("librarians.csv");
    if (!file.is_open())
    {
        std::cerr << "Unable to open " << "librarians.csv" << " file!" << std::endl;
        return librarians;
    }
    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        Librarian librarian;
        std::string username, password;
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        librarian = Librarian(username, password);
        librarians.push_back(librarian);
    }
    file.close();
    return librarians;
}

std::string formatDate4(time_t rawTime)
{
    struct tm *timeInfo;
    char buffer[11]; // Buffer to hold the formatted date

    timeInfo = localtime(&rawTime);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeInfo);

    return std::string(buffer);
}

void Librarian::writeLibrarianData(const std::vector<Librarian> &librarians)
{
    std::ofstream file("librarians.csv");
    if (!file.is_open())
    {
        std::cerr << "Unable to open librarians.csv file for writing!" << std::endl;
        return;
    }
    for (size_t i = 0; i < librarians.size(); ++i)
    {
        file << librarians[i].getUsername() << "," << librarians[i].getPassword();
        if (i != librarians.size() - 1)
        {
            file << "\n";
        }
    }
    file.close();
}

void Librarian::registerNewStudent() const
{
    std::vector<Student> students = Student::readStudentData();
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    bool k = false;
    for (auto student : students)
    {
        if (student.getUsername() == username)
        {
            std::cout << "Username already exists. Please try again.\n";
            k = true;
            break;
        }
    }
    if (k == false)
    {
        std::cout << "Enter password: ";
        std::cin >> password;

        std::ofstream file("students.csv", std::ios::app);
        if (!file.is_open())
        {
            std::cerr << "Error opening file." << std::endl;
            return;
        }

        file << username << "," << password << ",0," << time(0) << ",[],[]\n";
        file.close();
    }
}

void Librarian::registerNewFaculty() const
{
    std::vector<Faculty> faculties = Faculty::readFacultyData();
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    bool k = false;
    for (auto faculty : faculties)
    {
        if (faculty.getUsername() == username)
        {
            std::cout << "Username already exists. Please try again.\n";
            k = true;
            return;
        }
    }
    if (k == false)
    {
        std::cout << "Enter password: ";
        std::cin >> password;

        std::ofstream file("faculty.csv", std::ios::app);
        if (!file.is_open())
        {
            std::cerr << "Error opening file." << std::endl;
            return;
        }

        file << username << "," << password << ",0,[],[]\n";
        file.close();
    }
}

void Librarian::DeleteBook(const std::string &bookname)
{
    std::vector<Book> books = Book::readBookData();
    auto it = std::find_if(books.begin(), books.end(), [&bookname](const Book &book)
                           { return book.getName() == bookname; });

    if (it != books.end())
    {
        books.erase(it);
        Book::writeBookData(books);
        std::cout << "Book deleted successfully." << std::endl;
    }
    else
    {
        std::cout << "Book not found." << std::endl;
    }
}

void Librarian::deleteStudent(const std::string &username)
{
    std::vector<Student> students = Student::readStudentData();
    auto it = std::find_if(students.begin(), students.end(), [&username](const Student &student)
                           { return student.getUsername() == username; });

    if (it != students.end())
    {
        students.erase(it);
        Student::writeStudentData(students);
        std::cout << "Student deleted successfully." << std::endl;
    }
    else
    {
        std::cout << "Student not found." << std::endl;
    }
}

void Librarian::deleteFaculty(const std::string &username)
{
    std::vector<Faculty> faculties = Faculty::readFacultyData();
    auto it = std::find_if(faculties.begin(), faculties.end(), [&username](const Faculty &faculty)
                           { return faculty.getUsername() == username; });

    if (it != faculties.end())
    {
        faculties.erase(it);
        Faculty::writeFacultyData(faculties);
        std::cout << "Faculty deleted successfully." << std::endl;
    }
    else
    {
        std::cout << "Faculty not found." << std::endl;
    }
}

void Librarian::addBook(const std::string &name, const std::string &author, const std::string &publisher,
                        int year, const std::string &isbn, int available, int date, const std::string &towhom)
{
    std::vector<Book> books = Book::readBookData();
    Book book(name, author, publisher, year, isbn, available, date, towhom);
    books.push_back(book);
    Book::writeBookData(books);
    std::cout << "Book added successfully." << std::endl;
}

void Librarian::updateBook(const std::string &bookname, const std::string &newname, const std::string &newauthor, const std::string &newpublisher,
                           int newyear, const std::string &newisbn, int newavailable, int newdate, const std::string &newtowhom)
{
    std::vector<Book> books = Book::readBookData();
    auto it = std::find_if(books.begin(), books.end(), [&bookname](const Book &book)
                           { return book.getName() == bookname; });

    if (it != books.end())
    {
        it->setName(newname);
        it->setAuthor(newauthor);
        it->setPublisher(newpublisher);
        it->setYear(newyear);
        it->setIsbn(newisbn);
        it->setAvailable(newavailable);
        it->setBorrowDate(newdate);
        it->setToWhom(newtowhom);
        Book::writeBookData(books);
        std::cout << "Book updated successfully." << std::endl;
    }
    else
    {
        std::cout << "Book not found." << std::endl;
    }
}

void Librarian::displayAllBooks() const
{
    std::vector<Book> books = Book::readBookData();
    for (const auto &book : books)
    {
        std::cout << "Name: " << book.getName() << std::endl;
        std::cout << "Author: " << book.getAuthor() << std::endl;
        std::cout << "Publisher: " << book.getPublisher() << std::endl;
        std::cout << "Year: " << book.getYear() << std::endl;
        std::cout << "ISBN: " << book.getIsbn() << std::endl;
        std::cout << "Available: " << book.getAvailable() << std::endl;
        std::cout << "Borrow Date: " << formatDate4(book.getBorrowDate()) << std::endl;
        std::cout << "To Whom: " << book.getToWhom() << std::endl;
        std::cout << "-------------------" << std::endl;
    }
}

void Librarian::displayAllStudents() const
{
    std::vector<Student> students = Student::readStudentData();
    for (const auto &student : students)
    {
        std::cout << "Username: " << student.getUsername() << std::endl;
        std::cout << "-------------------" << std::endl;
    }
}

void Librarian::displayAllFaculty() const
{
    std::vector<Faculty> faculties = Faculty::readFacultyData();
    for (const auto &faculty : faculties)
    {
        std::cout << "Username: " << faculty.getUsername() << std::endl;
        std::cout << "-------------------" << std::endl;
    }
}
