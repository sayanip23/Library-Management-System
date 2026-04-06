#include "Book.h"
#include <iostream>
#include <fstream>
#include <sstream>

Book::Book(const std::string &name, const std::string &author, const std::string &publisher,
           int year, const std::string &isbn, int available, int borrowDate, const std::string &towhom)
    : name(name), author(author), publisher(publisher), year(year), isbn(isbn), isAvailable(available), borrowDate(borrowDate), towhom(towhom) {}

Book::Book() {}
Book::Book(const std::string &name) : name(name) {}

std::string Book::getName() const
{
    return name;
}

std::string Book::getAuthor() const
{
    return author;
}

std::string Book::getPublisher() const
{
    return publisher;
}

int Book::getYear() const
{
    return year;
}

std::string Book::getIsbn() const
{
    return isbn;
}

int Book::getAvailable() const
{
    return isAvailable;
}

int Book::getBorrowDate() const
{
    return borrowDate;
}

std::string Book::getToWhom() const
{
    return towhom;
}

void Book::setName(const std::string &n)
{
    name = n;
}

void Book::setAuthor(const std::string &a)
{
    author = a;
}

void Book::setPublisher(const std::string &p)
{
    publisher = p;
}

void Book::setYear(int y)
{
    year = y;
}

void Book::setIsbn(const std::string &i)
{
    isbn = i;
}

void Book::setAvailable(int a)
{
    isAvailable = a;
}

void Book::setBorrowDate(int d)
{
    borrowDate = d;
}

void Book::setToWhom(const std::string &t)
{
    towhom = t;
}

std::vector<Book> Book::readBookData()
{
    std::vector<Book> books;
    std::ifstream file("books.csv");

    if (!file.is_open())
    {
        std::cerr << "Unable to open books.csv file!" << std::endl;
        return books;
    }

    std::string line;
    // Skip header line if it exists
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string name, author, publisher, towhom, isbn;
        int year, isAvailable, borrowDate;

        // Parse CSV fields according to Book class structure
        if (std::getline(ss, name, ',') &&
            std::getline(ss, author, ',') &&
            std::getline(ss, publisher, ',') &&
            ss >> year && ss.ignore() &&
            std::getline(ss, isbn, ',') &&
            ss >> isAvailable && ss.ignore() &&
            ss >> borrowDate && ss.ignore() &&
            std::getline(ss, towhom))
        {
            // Create Book object with the parsed data
            Book book(name, author, publisher, year, isbn, isAvailable, borrowDate, towhom);
            books.push_back(book);
        }
    }

    file.close();
    return books;
}

// Static function to write book data to a CSV file
void Book::writeBookData(const std::vector<Book> &books)
{
    std::ofstream file("books.csv");

    if (!file.is_open())
    {
        std::cerr << "Unable to open books.csv file for writing!" << std::endl;
        return;
    }

    // Write header based on Book class structure
    file << "Name,Author,Publisher,Year,ISBN,IsAvailable,BorrowDate,ToWhom" << std::endl;

    // Write each book's data
    for (const auto &book : books)
    {
        // Assuming Book class has getter methods for these properties
        file << book.getName() << ","
             << book.getAuthor() << ","
             << book.getPublisher() << ","
             << book.getYear() << ","
             << book.getIsbn() << ","
             << book.getAvailable() << ","
             << book.getBorrowDate() << ","
             << book.getToWhom() << std::endl;
    }

    file.close();
    // std::cout << "Book data has been successfully written to books.csv" << std::endl;
}
