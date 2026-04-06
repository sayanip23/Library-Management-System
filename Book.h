#pragma once
#include <string>
#include <vector>

class Book
{
private:
    std::string name;
    std::string author;
    std::string publisher;
    std::string isbn;
    int year;
    int isAvailable;
    int borrowDate;
    std::string towhom;

public:
    Book(const std::string &name, const std::string &author, const std::string &publisher,
         int year, const std::string &isbn, int available, int date, const std::string &towhom);
    Book();
    Book(const std::string &name);

    // Getter methods
    std::string getName() const;
    std::string getAuthor() const;
    std::string getPublisher() const;
    std::string getIsbn() const;
    int getYear() const;
    int getAvailable() const;
    int getBorrowDate() const;
    std::string getToWhom() const;

    // Setter methods
    void setName(const std::string &name);
    void setAuthor(const std::string &author);
    void setPublisher(const std::string &publisher);
    void setYear(int year);
    void setIsbn(const std::string &isbn);
    void setAvailable(int available);
    void setBorrowDate(int borrowDate);
    void setToWhom(const std::string &towhom);

    // File operations
    static std::vector<Book> readBookData();
    static void writeBookData(const std::vector<Book> &books);
};
