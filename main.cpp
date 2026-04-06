#include "User.h"
#include "Book.h"
#include "Student.h"
#include "Faculty.h"
#include "Librarian.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <ctime>

std::string formatDate3(time_t rawTime)
{
    struct tm *timeInfo;
    char buffer[11]; // Buffer to hold the formatted date

    timeInfo = localtime(&rawTime);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeInfo);

    return std::string(buffer);
}

int main()
{
    std::vector<Book> books = Book::readBookData();
    std::vector<Student> students = Student::readStudentData();
    std::vector<Faculty> faculty = Faculty::readFacultyData();
    std::vector<Librarian> librarian = Librarian::readLibrarianData();

    while (1)
    {
        bool returnToMainPrompt = false;

        std::cout << "Enter 1 if you are a Student, 2 if you are a Faculty, and 3 if you are the Librarian and any other number for ending: ";
        int verification;

        if (!(std::cin >> verification))
        {
            std::cout << "Invalid input. Please enter an integer.\n";

            // Clear the error state and ignore invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue; // Restart the loop
        }

        if (verification != 1 && verification != 2 && verification != 3)
        {
            std::cout << "Good bye";
            return 0; // This properly breaks out of the entire program
        }

        if (verification == 1) // Student
        {
            std::cout << "Are you already registered? (Type YES if already registered or NO) ";
            std::string answer;

            while (true)
            {
                std::cin >> answer;
                transform(answer.begin(), answer.end(), answer.begin(), ::toupper);
                if (answer == "YES" || answer == "NO")
                {
                    // Valid input, exit the loop
                    break;
                }
                else
                {
                    std::cout << "Invalid input. Please type YES or NO: ";
                    // Clear the input buffer to prevent infinite loop
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }

            if (answer == "YES") // Student Login
            {
                std::string username;
                std::cout << "Enter username: ";
                std::cin >> username;

                auto it = find_if(students.begin(), students.end(),
                                  [&username](const Student &c)
                                  { return c.getUsername() == username; });

                if (it != students.end())
                {
                    std::cout << "Enter password: ";
                    std::string password;
                    std::cin >> password;
                    if (it->authenticate(password))
                    {
                        std::cout << "Login successful! Welcome, " << it->getUsername() << "!\n";

                        // Student Main Menu
                        while (true)
                        {
                            std::cout << "You're at the Main Menu.\n Press 1, to Return Book(s)\n Press 2, to Borrow Book(s)\n Press 3, to Check Fines\n Press 4, to Pay Fines\n Press 5, to Check Borrowed Books\n Press 6, to View Borrowing History\n Press 7, to View Available Books\n Press any other number else to Exit\n";
                            int input;
                            if (!(std::cin >> input))
                            {
                                std::cout << "Invalid input. Please enter an integer.\n";

                                // Clear the error state and ignore invalid input
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                continue; // Restart the loop
                            }

                            switch (input)
                            {

                            case 1: // Return Book
                            {
                                std::vector<Book> borrowedBooks = it->getBooksBorrowed();

                                if (borrowedBooks.empty())
                                {
                                    std::cout << "You have no books to return.\n";
                                }
                                else
                                {
                                    std::cout << "Your borrowed books:\n";
                                    for (size_t i = 0; i < borrowedBooks.size(); ++i)
                                    {
                                        std::cout << i + 1 << ". " << borrowedBooks[i].getName() << "\n";
                                    }

                                    std::cout << "Enter the name of the book you want to return: ";
                                    std::string book_name;
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                    std::getline(std::cin, book_name);

                                    it->returnBook(book_name);
                                    break;
                                }
                            }

                            case 2: // Borrow Book
                            {
                                // std::cout << "Hello";
                                if (it->getFineDue() > 0)
                                {
                                    std::cout << "You have a fine of " << it->getFineDue() << " rupees. Please pay the fine to borrow books.\n";
                                    break;
                                }
                                std::vector<Book> books = Book::readBookData();
                                std::cout << "Available Books:\n";
                                for (auto &book : books)
                                {
                                    if (book.getAvailable() == 1)
                                    {
                                        std::cout << "Title: " << book.getName() << ", Author: " << book.getAuthor()
                                                  << ", ISBN: " << book.getIsbn() << "\n";
                                    }
                                }
                                std::cout << "Enter the name of the book you want to borrow: ";
                                std::string book_name;
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                std::getline(std::cin, book_name);
                                it->borrowBook(book_name);
                                break;
                            }

                            case 3: // Check Fines
                            {
                                std::cout << "You have to pay " << it->calculateFine() << " rupees as fine\n";
                                break;
                            }

                            case 4: // Pay Fines
                            {
                                it->payFine();
                                std::cout << "Fine paid successfully\n";
                                break;
                            }

                            case 5: // Check Borrowed Books
                            {
                                it->displayBorrowedBooks();
                                break;
                            }

                            case 6: // View Borrowing History
                            {
                                it->displayBorrowHistory();
                                break;
                            }

                            case 7: // View Available Books
                            {
                                std::vector<Book> books = Book::readBookData();
                                std::cout << "Available Books:\n";
                                for (const auto &book : books)
                                {
                                    if (book.getAvailable() == 1)
                                    {
                                        std::cout << "Title: " << book.getName() << ", Author: " << book.getAuthor() << "\n";
                                    }
                                }
                                break;
                            }

                            case 8:
                            {
                                returnToMainPrompt = true;
                                break;
                            }

                            default:
                            {
                                std::cout << "Good bye";
                                return 0;
                            }
                            }
                            if (returnToMainPrompt)
                            {
                                break;
                            }
                        }
                        if (returnToMainPrompt)
                        {
                            continue;
                        }
                    }
                    else
                    {
                        std::cout << "Invalid password.\n";
                    }
                }
                else
                {
                    std::cout << "Username not found.\n";
                }
            }
            else if (answer == "NO") // Student Registration
            {
                std::string username, password, confirmPassword;
                std::cout << "Enter username: ";
                std::cin >> username;
                bool k = false;
                for (auto students : students)
                {
                    if (students.getUsername() == username)
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
                    std::cout << "Confirm password: ";
                    std::cin >> confirmPassword;

                    if (password == confirmPassword)
                    {
                        Student newStudent(username, password, 0, time(0), std::vector<Book>(), std::vector<Book>());
                        students.push_back(newStudent);
                        Student::writeStudentData(students);
                        std::cout << "Student registration successful! Please log in again.\n";
                    }
                    else
                    {
                        std::cout << "Password and Confirm Password do not match. Registration failed.\n";
                    }
                }
            }
        }

        else if (verification == 2) // Faculty
        {
            std::cout << "Are you already registered? (Type YES if already registered or NO) ";
            std::string answer;

            while (true)
            {
                std::cin >> answer;
                transform(answer.begin(), answer.end(), answer.begin(), ::toupper);
                if (answer == "YES" || answer == "NO")
                {
                    // Valid input, exit the loop
                    break;
                }
                else
                {
                    std::cout << "Invalid input. Please type YES or NO: ";
                    // Clear the input buffer to prevent infinite loop
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }

            if (answer == "YES") // Faculty Login
            {
                std::string username;
                std::cout << "Enter username: ";
                std::cin >> username;

                auto it = find_if(faculty.begin(), faculty.end(),
                                  [&username](const Faculty &c)
                                  { return c.getUsername() == username; });

                if (it != faculty.end())
                {
                    std::cout << "Enter password: ";
                    std::string password;
                    std::cin >> password;
                    if (it->authenticate(password))
                    {
                        std::cout << "Login successful! Welcome, " << it->getUsername() << "!\n";

                        // Faculty Main Menu
                        while (true)
                        {
                            std::cout << "You're at the Main Menu.\n Press 1, to Return Book(s)\n Press 2, to Borrow Book(s)\n Press 3, to Check Maximum Overdue\n Press 4, to Check Borrowed Books\n Press 5, to View Borrowing History\n Press 6, to View Available Books\n Press anything else to Exit\n";
                            int input;
                            if (!(std::cin >> input))
                            {
                                std::cout << "Invalid input. Please enter an integer.\n";

                                // Clear the error state and ignore invalid input
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                continue; // Restart the loop
                            }

                            switch (input)
                            {
                            case 1: // Return Book
                            {
                                std::vector<Book> borrowedBooks = it->getBooksBorrowed();

                                if (borrowedBooks.empty())
                                {
                                    std::cout << "You have no books to return.\n";
                                }
                                else
                                {
                                    std::cout << "Your borrowed books:\n";
                                    for (size_t i = 0; i < borrowedBooks.size(); ++i)
                                    {
                                        std::cout << i + 1 << ". " << borrowedBooks[i].getName() << "\n";
                                    }

                                    std::cout << "Enter the name of the book you want to return: ";
                                    std::string book_name;
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                    std::getline(std::cin, book_name);

                                    it->returnBook(book_name);
                                }
                                break;
                            }

                            case 2: // Borrow Book
                            {
                                if (it->getMaxOverdue() > 60)
                                {
                                    std::cout << "You have a maximum overdue of " << it->getMaxOverdue() << " days. Please return the book(s) to borrow books.\n";
                                    break;
                                }
                                std::vector<Book> books = Book::readBookData();
                                std::cout << "Available Books:\n";
                                for (auto &book : books)
                                {
                                    if (book.getAvailable() == 1)
                                    {
                                        std::cout << "Title: " << book.getName() << ", Author: " << book.getAuthor()
                                                  << ", ISBN: " << book.getIsbn() << "\n";
                                    }
                                }
                                std::cout << "Enter the name of the book you want to borrow: ";
                                std::string book_name;
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                std::getline(std::cin, book_name);
                                it->borrowBook(book_name);
                                break;
                            }

                            case 3: // Check Max Overdue
                            {
                                std::cout << "You have a maximum overdue of " << it->getMaxOverdue() << " days. \n";
                                break;
                            }

                            case 4: // Check Borrowed Books
                            {
                                it->displayBorrowedBooks();
                                break;
                            }

                            case 5: // Check Borrowing History
                            {
                                it->displayBorrowHistory();
                                break;
                            }

                            case 6: // View Available Books
                            {
                                std::vector<Book> books = Book::readBookData();
                                std::cout << "Available Books:\n";
                                for (const auto &book : books)
                                {
                                    if (book.getAvailable() == 1)
                                    {
                                        std::cout << "Title: " << book.getName() << ", Author: " << book.getAuthor() << "\n";
                                    }
                                }
                                break;
                            }

                            default:
                            {
                                std::cout << "Good bye";
                                return 0;
                            }
                            }
                        }
                    }
                    else
                    {
                        std::cout << "Invalid password.\n";
                    }
                }
                else
                {
                    std::cout << "Username not found.\n";
                }
            }
            else if (answer == "NO") // Faculty Registration
            {
                std::string username, password, confirmPassword;
                std::cout << "Enter username: ";
                std::cin >> username;
                bool k = false;
                for (auto faculty : faculty)
                {
                    if (faculty.getUsername() == username)
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
                    std::cout << "Confirm password: ";
                    std::cin >> confirmPassword;

                    if (password == confirmPassword)
                    {
                        Faculty newFaculty(username, password, 0, std::vector<Book>(), std::vector<Book>());
                        faculty.push_back(newFaculty);
                        Faculty::writeFacultyData(faculty);
                        std::cout << "Faculty registration successful! Please log in again.\n";
                    }
                    else
                    {
                        std::cout << "Password and Confirm Password do not match. Registration failed.\n";
                    }
                }
            }
        }
        else if (verification == 3) // Librarian
        {
            std::cout << "Are you already registered? (Type YES if already registered or NO) ";
            std::string answer;

            while (true)
            {
                std::cin >> answer;
                transform(answer.begin(), answer.end(), answer.begin(), ::toupper);
                if (answer == "YES" || answer == "NO")
                {
                    // Valid input, exit the loop
                    break;
                }
                else
                {
                    std::cout << "Invalid input. Please type YES or NO: ";
                    // Clear the input buffer to prevent infinite loop
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }

            if (answer == "YES") // Librarian Login
            {
                std::string username;
                std::cout << "Enter username: ";
                std::cin >> username;

                auto it = find_if(librarian.begin(), librarian.end(),
                                  [&username](const Librarian &c)
                                  { return c.getUsername() == username; });

                if (it != librarian.end())
                {
                    std::cout << "Enter password: ";
                    std::string password;
                    std::cin >> password;
                    if (it->authenticate(password))
                    {
                        std::cout << "Login successful! Welcome, " << it->getUsername() << "!\n";

                        // Librarian Main Menu
                        while (true)
                        {
                            std::cout << "You're at the Main Menu.\n Press 1, to Register New Student\n Press 2, to Register New Faculty\n Press 3, to Delete Book\n Press 4, to Delete Student\n Press 5, to Delete Faculty\n Press 6, to Add Book\n Press 7, to Update Book\n Press 8, to Display all Books\n Press 9, to Display all Faculty\n Press 10, to Display all Students\n Press 11, to go back to the Start\n Press anything else, to Exit\n";
                            int input;
                            if (!(std::cin >> input))
                            {
                                std::cout << "Invalid input. Please enter an integer.\n";

                                // Clear the error state and ignore invalid input
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                continue; // Restart the loop
                            }

                            switch (input)
                            {
                            case 1: // Register New Student
                            {
                                it->registerNewStudent();
                                break;
                            }

                            case 2: // Register New Faculty
                            {
                                it->registerNewFaculty();
                                break;
                            }

                            case 3: // Delete Books
                            {
                                std::string bookName;
                                std::cout << "Enter the name of the book to delete: ";
                                std::cin.ignore(); // Clear any previous input
                                std::getline(std::cin, bookName);
                                it->DeleteBook(bookName);
                                break;
                            }

                            case 4: // Delete Students
                            {
                                std::string username;
                                std::cout << "Enter the username of the student to delete: ";
                                std::cin.ignore(); // Clear any previous input
                                std::getline(std::cin, username);
                                it->deleteStudent(username);
                                break;
                            }

                            case 5: // Delete Faculty
                            {
                                std::string username;
                                std::cout << "Enter the username of the faculty to delete: ";
                                std::cin.ignore(); // Clear any previous input
                                std::getline(std::cin, username);
                                it->deleteFaculty(username);
                                break;
                            }

                            case 6: // Add Books
                            {
                                std::string name, author, publisher, toWhom, isbn;
                                int year, available = 1, date = 0;

                                std::cin.ignore(); // Clear previous input
                                std::cout << "Enter book name: ";
                                std::getline(std::cin, name);

                                std::cout << "Enter author: ";
                                std::getline(std::cin, author);

                                std::cout << "Enter publisher: ";
                                std::getline(std::cin, publisher);

                                std::cout << "Enter ISBN: ";
                                std::getline(std::cin, isbn);

                                std::cout << "Enter year: ";
                                std::cin >> year;

                                toWhom = "null"; // Initially no one has borrowed the book

                                it->addBook(name, author, publisher, year, isbn, available, date, toWhom);
                                // std::cout << "Added";
                                break;
                            }

                            case 7: // Update Books
                            {
                                std::string bookName, newName, newAuthor, newPublisher, newToWhom, newIsbn;
                                int newYear;
                                long long int newDate;
                                bool newAvailable;

                                std::cin.ignore(); // Clear previous input
                                std::cout << "Enter current book name: ";
                                std::getline(std::cin, bookName);
                                bool found = false;

                                for (auto book : books)
                                {
                                    if (book.getName() == bookName)
                                    {
                                        // std::cout << "Book found";
                                        found = true;
                                        std::cout << "Current Book Attributes" << std::endl;
                                        std::cout << "Book name" << book.getName() << std::endl;
                                        std::cout << "Book author" << book.getAuthor() << std::endl;
                                        std::cout << "Book publisher" << book.getPublisher() << std::endl;
                                        std::cout << "Book year" << book.getYear() << std::endl;
                                        std::cout << "Book isbn" << book.getIsbn() << std::endl;
                                        std::cout << "Book available" << book.getAvailable() << std::endl;
                                        std::cout << "Book date" << formatDate3(book.getBorrowDate()) << std::endl;
                                        std::cout << "Book towhom" << book.getToWhom() << std::endl;
                                        break;
                                    }
                                }

                                if (found)
                                {
                                    std::cout << "Enter new name: ";
                                    std::getline(std::cin, newName);

                                    std::cout << "Enter new author: ";
                                    std::getline(std::cin, newAuthor);

                                    std::cout << "Enter new publisher: ";
                                    std::getline(std::cin, newPublisher);

                                    std::cout << "Enter new ISBN: ";
                                    std::getline(std::cin, newIsbn);

                                    std::cout << "Enter new year: ";
                                    std::cin >> newYear;

                                    std::cout << "Is book available? (1 for yes, 0 for no): ";
                                    std::cin >> newAvailable;

                                    std::cin.ignore();
                                    std::cout << "Enter new borrow date (ddmmyyyy format): ";
                                    std::string dateStr;
                                    std::getline(std::cin, dateStr);

                                    struct tm tm = {0};
                                    strptime(dateStr.c_str(), "%d%m%Y", &tm);
                                    newDate = mktime(&tm);

                                    std::cin.ignore(); // Clear buffer
                                    std::cout << "Enter new borrower (empty if not borrowed): ";
                                    std::getline(std::cin, newToWhom);

                                    it->updateBook(bookName, newName, newAuthor, newPublisher, newYear, newIsbn, newAvailable, newDate, newToWhom);
                                    break;
                                }
                                else
                                {
                                    std::cout << "Book not found";
                                }
                            }

                            case 8: // Display all Books
                            {
                                // std::cout << "Hello";
                                it->displayAllBooks();
                                break;
                            }

                            case 9: // Display all Faculty
                            {
                                it->displayAllFaculty();
                                break;
                            }

                            case 10: // Display all Students
                            {
                                it->displayAllStudents();
                                break;
                            }

                            default:
                            {
                                std::cout << "Good bye";
                                return 0;
                            }
                            }
                        }
                    }
                    else
                    {
                        std::cout << "Invalid password.\n";
                    }
                }
                else
                {
                    std::cout << "Librarian not found.\n";
                }
            }
            else if (answer == "NO") // Librarian Registration
            {
                std::string username, password, confirmPassword;
                std::cout << "Enter username: ";
                std::cin >> username;
                bool k = false;
                for (auto librarian : librarian)
                {
                    if (librarian.getUsername() == username)
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
                    std::cout << "Confirm password: ";
                    std::cin >> confirmPassword;

                    if (password == confirmPassword)
                    {
                        Librarian newLibrarian(username, password);
                        librarian.push_back(newLibrarian);
                        Librarian::writeLibrarianData(librarian);
                        std::cout << "Librarian registration successful! Please log in again.\n";
                    }
                    else
                    {
                        std::cout << "Password and Confirm Password do not match. Registration failed.\n";
                    }
                }
            }
        }
    }
    return 0;
}
