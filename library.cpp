#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;


//______________________________________________________________________________________________________________________

struct Credentials {
    string username;
    string password;
};

//______________________________________________________________________________________________________________________

Credentials loadCredentials(const string& filename) {
    ifstream configFile(filename);
    Credentials credentials;
    string line;

    if (configFile.is_open()) {
        while (getline(configFile, line)) {
            size_t delimiterPos = line.find('=');
            if (delimiterPos != string::npos) {
                string key = line.substr(0, delimiterPos);
                string value = line.substr(delimiterPos + 1);

                if (key == "adminUsername") {
                    credentials.username = value;
                } else if (key == "adminPassword") {
                    credentials.password = value;
                }
            }
        }
        configFile.close();
    } else {
        cerr << "Error: Unable to open configuration file." << endl;
    }

    return credentials;
}

//______________________________________________________________________________________________________________________

struct User {
    string username;
    string password;
};

//______________________________________________________________________________________________________________________

struct Book {
    string title;
    string author;
    int year;
    bool issued;
    string patron_name;
    string issue_date;
    int quantity;  
};

vector<Book> library;
vector<User> users;

//______________________________________________________________________________________________________________________

void saveLibrary(const string& filename) {
    ofstream libraryFile(filename);
    if (libraryFile.is_open()) {
        for (const Book& book : library) {
            libraryFile << book.title << "|" << book.author << "|" << book.year << "|" << book.issued << "|" << book.patron_name << "|" << book.issue_date << "|" << book.quantity << "\n";
        }
        libraryFile.close();
        cout << "Library saved to " << filename << endl;
    } else {
        cerr << "Error: Unable to open library file for writing." << endl;
    }
}

//______________________________________________________________________________________________________________________

void loadLibrary(const string& filename) {
    ifstream libraryFile(filename);
    library.clear();

    if (libraryFile.is_open()) {
        string line;
        while (getline(libraryFile, line)) {
            istringstream iss(line);
            string title, author, issue_date, patron_name;
            int year, quantity;
            bool issued;

            getline(iss, title, '|');
            getline(iss, author, '|');
            iss >> year;
            iss.ignore();
            iss >> issued;
            iss.ignore();
            getline(iss, patron_name, '|');
            getline(iss, issue_date, '|');
            iss >> quantity;

            Book book = {title, author, year, issued, patron_name, issue_date, quantity};
            library.push_back(book);
        }
        libraryFile.close();
        cout << "Library loaded from " << filename << endl;
    } else {
        cerr << "Library file not found. Starting with an empty library." << endl;
    }
}

//______________________________________________________________________________________________________________________

void loadUsers() {
    ifstream userFile("users.txt");
    string username, password;
    while (userFile >> username >> password) {
        User user = {username, password};
        users.push_back(user);
    }

    userFile.close();
}

//______________________________________________________________________________________________________________________

void saveUsers() {
    ofstream userFile("users.txt");
    for (const User& user : users) {
        userFile << user.username << " " << user.password << endl;
    }

    userFile.close();
}

//______________________________________________________________________________________________________________________

void registerUser() {
    string newUsername, newPassword;
    cout << "Enter A New UserName: ";
    cin >> newUsername;
    cout << "Enter A Password: ";
    cin >> newPassword;

    for (const User& user : users) {
        if (user.username == newUsername) {
            cout << "Username Already Exists. Use Other Name" << endl;
            return;
        }
    }

    User newUser = {newUsername, newPassword};
    users.push_back(newUser);
    saveUsers();
    cout << "Register Successful." << endl;
}

//______________________________________________________________________________________________________________________

bool authenticateUser() {
    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    for (const User& user : users) {
        if (user.username == username && user.password == password) {
            return true;
        }
    }

    cout << "Authentication Failed. Please Check Your Information" << endl;
    return false;
}

//______________________________________________________________________________________________________________________

bool login(const string& adminUsername, const string& adminPassword) {
    string user_type;
    while (true) {
        cout << "Write 'Exit' to close the program." << endl;
        cout << "Are you a user or an administrator? (user/admin): ";
        cin >> user_type;

        if (user_type == "Exit") {
            exit(0);
        } else if (user_type == "user") {
            return false;
        } else if (user_type == "admin") {
            string username, password;
            cout << "Enter admin username: ";
            cin >> username;
            cout << "Enter admin password: ";
            cin >> password;

            if (username == adminUsername && password == adminPassword) {
                return true;
            } else {
                cout << "Authentication failed for admin. Please check your credentials." << endl;
            }
        } else {
            cout << "Invalid input. Please enter 'user' or 'admin'." << endl;
        }
    }
}

//______________________________________________________________________________________________________________________

void admin_menu() {
    cout << "Administrator Menu:" << endl;
    cout << "   1. Add a new book to the library." << endl;
    cout << "   2. See the list of borrowed books." << endl;
    cout << "   3. Remove a quantity of a book from the library." << endl;
    cout << "   4. Issue a book to a person." << endl;
    cout << "   5. List all the books in the library." << endl;
    cout << "   6. Return a book." << endl;
    cout << "   0. Return" << endl;
}

//______________________________________________________________________________________________________________________

void user_menu() {
    cout << "" << endl;
    cout << "User Menu:" << endl;
    cout << "   1. Search for a book by title or author." << endl;
    cout << "   2. Return A Requested Book." << endl;
    cout << "   3. List all the books in the library." << endl;
    cout << "   4. Request A Book."  << endl;
    cout << "   0. Return" << endl;
}

//______________________________________________________________________________________________________________________

//PERMITE BUSCAR UN LIBRO


void search_book() {
    int search_option;
    cout << "Enter search option:\n1. Title\n2. Author " << std::endl << "Write the option: ";
    cin >> search_option;
    string search_string;
    int search_year;
    switch (search_option) {
        case 1:
            cout << "Enter book title: ";
            cin.ignore();
            getline(cin, search_string);

            for (const auto& book : library) {
                if (book.title == search_string) {
                    cout << book.title << " by " << book.author << " (" << book.year << ")\n";
                    return;
                }
            }
            cout << "Book not found\n";
            break;

        case 2:
            cout << "Enter book author: ";
            cin.ignore();
            getline(cin, search_string);
            for (const auto& book : library) {
                if (book.author == search_string) {
                    cout << book.title << " by " << book.author << " (" << book.year << ")\n";
                    return;
                }
            }
            cout << "Book not found\n";
            break;

        default:
            cout << "Invalid option\n";
            break;
    }
}


//______________________________________________________________________________________________________________________

//PERMITE AGREGAR UN LIBRO A LA LISTA Y PERMITE QUITAR LA CANTIDAD DE LIBROS INDEPENDIENTEMENTE LA CANTIDAD PUEDEN SER
//DESDE TODOS A SOLAMENTE UNO 

void add_book() {
    Book newBook;
    cout << "Enter book title: ";
    cin.ignore();
    getline(cin, newBook.title);
    cout << "Enter book author: ";
    getline(cin, newBook.author);
    cout << "Enter book year: ";
    cin >> newBook.year;

    bool bookExists = false;
    int existingBookIndex = -1;

    for (int i = 0; i < library.size(); i++) {
        if (library[i].title == newBook.title && library[i].author == newBook.author) {
            bookExists = true;
            existingBookIndex = i;
            break;
        }
    }

    if (bookExists) {
        int additionalQuantity;
        cout << "Enter the additional quantity of this book: ";
        cin >> additionalQuantity;

        library[existingBookIndex].quantity += additionalQuantity;
        cout << "Quantity added to existing book in the library" << endl;
    } else {
        newBook.issued = false;
        cout << "Enter the quantity of this book: ";
        cin >> newBook.quantity;

        library.push_back(newBook);
        cout << "New book added to library" << endl;
    }
}

void remove_quantity_book() {
    int book_index;
    cout << "Enter index of book to remove quantity from: ";
    cin >> book_index;

    if (book_index >= 0 && book_index < library.size()) {
        int quantity;
        cout << "Enter the quantity to remove: ";
        cin >> quantity;

        if (quantity <= library[book_index].quantity) {
            library[book_index].quantity -= quantity;
            cout << "Quantity removed from the library\n";

            if (library[book_index].quantity == 0) {
                library.erase(library.begin() + book_index);
                cout << "Book removed from the library\n";
            }
        } else {
            cout << "Invalid quantity. There are only " << library[book_index].quantity << " left.\n";
        }
    } else {
        cout << "Invalid index\n";
    }
}


//______________________________________________________________________________________________________________________

//ESTO PERMITE PRESTAR UN LIBRO 

void loanBook(int book_index, const string& patron_name, const string& contact_number) {
    if (book_index >= 0 && book_index < library.size()) {
        if (!library[book_index].issued) {
            library[book_index].patron_name = patron_name;
            library[book_index].issued = true;

            // Obtener la fecha actual
            time_t now = time(0);
            tm* date = localtime(&now);
            string issue_date = to_string(date->tm_year + 1900) + "-" +
                               to_string(date->tm_mon + 1) + "-" +
                               to_string(date->tm_mday);

            library[book_index].issue_date = issue_date;

            if (library[book_index].quantity > 0) {
                library[book_index].quantity--;
            }

            // Guardar la información de préstamo en un archivo (puedes personalizar esto)
            ofstream loanInfoFile("infoprestamo.txt", ios::app);
            if (loanInfoFile.is_open()) {
                loanInfoFile << "Title: " << library[book_index].title << endl;
                loanInfoFile << "Patron: " << patron_name << endl;
                loanInfoFile << "Date: " << issue_date << endl;
                loanInfoFile << "Contact Number: " << contact_number << endl;
                loanInfoFile << "Index: " << book_index << endl;
                loanInfoFile << "-----------------------------" << endl;
                loanInfoFile.close();
            } else {
                cerr << "Error: Unable to open loan information file." << endl;
            }

            cout << "Book issued to " << patron_name << endl;
        } else {
            cout << "Book already issued" << endl;
        }
    } else {
        cout << "Invalid book index" << endl;
    }
}


//______________________________________________________________________________________________________________________

//PERMITE RETORNAR LIBROS PRESTADOS

void return_book(int book_index) { 
    if (book_index >= 0 && book_index < library.size()) {
        if (library[book_index].issued) {
            

            ofstream loanInfoFile("infoprestamo.txt", ios::app);
            if (loanInfoFile.is_open()) {
                loanInfoFile << "Title: " << library[book_index].title << endl;
                loanInfoFile << "User: " << library[book_index].patron_name << endl;

                
                time_t now = time(0);
                tm* date = localtime(&now);
                string return_date = to_string(date->tm_year + 1900) + "-" +
                                   to_string(date->tm_mon + 1) + "-" +
                                   to_string(date->tm_mday);
                loanInfoFile << "Return Date: " << return_date << endl;
                loanInfoFile << "Index: " << book_index << endl; 
                loanInfoFile << "-----------------------------" << endl;
                loanInfoFile.close();
            } else {
                cerr << "Error: Unable to open loan information file." << endl;
            }

            library[book_index].issued = false;
            library[book_index].patron_name = "";
            library[book_index].issue_date = "";

            
            library[book_index].quantity++;

            cout << "Book returned to library" << endl;
        } else {
            cout << "Book not issued" << endl;
        }
    } else {
        cout << "Invalid book index" << endl;
    }
}


void requestBook(int book_index, const string& username) {
    if (book_index >= 0 && book_index < library.size()) {
        if (!library[book_index].issued) {
            string contact_number;
            cout << "Enter your contact number: ";
            cin >> contact_number;
            
            loanBook(book_index, username, contact_number);
        } else {
            cout << "Book already issued" << endl;
        }
    } else {
        cout << "Invalid book index" << endl;
    }
}

void returnRequestedBook(int book_index, const string& username) {
    if (book_index >= 0 && book_index < library.size()) {
        if (library[book_index].issued && library[book_index].patron_name == username) {
            return_book(book_index);
        } else {
            cout << "You can only return books that you have borrowed." << endl;
        }
    } else {
        cout << "Invalid book index" << endl;
    }
}
//______________________________________________________________________________________________________________________

//PERMITE LA LISTA DE LIBROS DE LA BIBLIOTECA DISPONIBLES Y LA LISTA DE LIBROS QUE SE ENCUENTRAN PRESTADOS

void list_books() {
    int i = 0;
    for (const auto& book : library) {
        cout << i++ << ". " << book.title << " by " << book.author << " (" << book.year << ")";

        if (book.issued) {
            cout << " (issued to " << book.patron_name << " on " << book.issue_date << ")";
        }

        cout << ", Quantity: " << book.quantity << endl;
    }
}

void printLoanInformation() {
    ifstream loanInfoFile("infoprestamo.txt");
    string line;

    if (loanInfoFile.is_open()) {
        cout << "Loan Information:" << endl;
        while (getline(loanInfoFile, line)) {
            cout << line << endl;
        }
        loanInfoFile.close();
    } else {
        cerr << "Error: Unable to open loan information file." << endl;
    }
}


//______________________________________________________________________________________________________________________



int main() {
    Credentials adminCredentials = loadCredentials("config.txt");

    const string adminUsername = adminCredentials.username;
    const string adminPassword = adminCredentials.password;

    loadUsers();
    loadLibrary("library.txt"); 

    cout << "========" << endl;
    cout << " WELCOME"  << endl;
    cout << "========" << endl;

    bool continueProgram = true; 
    string currentUsername = "";

    while (continueProgram) {

        bool is_admin = login(adminUsername, adminPassword); 
        bool isAuthenticated = false;

        if (!is_admin) {

            cout << "1. Log in\n2. Register\nChoose an option: ";
            int authOption;
            cin >> authOption;

            string username = "";
            string newUsername = "";

            if (authOption == 1) {
                isAuthenticated = authenticateUser();
                if(isAuthenticated){
                    cout << "Enter A Personalized Username: ";
                    cin>>username;
                    currentUsername=username;
                }
            } else if (authOption == 2) {
                registerUser();
                cout << "Enter A New UserName: ";
                cin >> newUsername;
                currentUsername = newUsername;
            } else {
                cout << "Invalid choice" << endl;
            }
        }

        int option;

        do {
            if (!is_admin && !isAuthenticated) {
                break; 
            }

            if (is_admin) {
                admin_menu();
            } else {
                user_menu();
            }

            cout << "Choose an option: ";
            cin >> option;

            switch (option) {

                case 0:
                    saveLibrary("library.txt"); 
                    break;

                case 1:
                    if (is_admin) {
                        add_book();
                    } else {
                        search_book();
                    }
                    break;

                case 2:
                    if (is_admin) {

                        cout << " " << endl;
                        cout << "List Of Borrowed Books. " << endl;
                        cout << " " << endl;
                        printLoanInformation();
                        
                    } else {
                        list_books();
                        int book_index;
                        cout << "Enter Index Of Requested Book To Return: ";
                        cin >> book_index;
                        returnRequestedBook(book_index, currentUsername);
                        
                    }
                    break;

                case 3:
                    if (is_admin) {
                        remove_quantity_book();
                    } else {
                        list_books();
                    }
                    break;

                case 4:
                    if (is_admin) {
                    int book_index;
                    cout << "Enter index of book to issue: ";
                    cin >> book_index;
                    string patron_name; // No lo solicites aquí
                    string contact_number;
                    cout << "Enter contact number: ";
                    cin >> contact_number;
                    loanBook(book_index, patron_name, contact_number);

                    } else {
                        list_books();
                        int book_index;
                        cout << "Enter Index Of Book To Request: ";
                        cin >> book_index;
                        requestBook(book_index, currentUsername);
                    }
                    break;

                case 5:
                    if (is_admin) {
                        list_books();

                    } else {
                        cout << " ";
                    }
                    break;

                case 6:
                if(is_admin){
                    int book_index;
                    cout << "Enter index of book to return: ";
                    cin >> book_index;
                    return_book(book_index);
                }else{
                    cout << "Invalid Choice" << endl;
                }


                break;



                default:
                    cout << "Invalid choice" << endl;
                    break;
            }
        } while (option != 0);
    }
    return 0;
}



