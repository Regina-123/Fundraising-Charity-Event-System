#include<iostream>
#include<map>
#include<set>
#include<string>
#include <fstream>
#include <vector>
#include <ctime>
#include <conio.h>
#include <algorithm>
#include <regex>
#include <iomanip>
#include <sstream>
#include <limits>
#include <cctype> 
using namespace std;

void clearScreen();
string hidePass();
void userRegister();
void userLogin();
void changeOwnPassword(const string& username);
bool isValidDateFormat(const string& date);
void createBooking();
void viewBooking();
void updateBooking();
void deleteBooking();
void updateUser();
void deleteUser();
void updateProfile(const string& username);
void deleteProfile(const string& username);
void organiser(const string& username);
void participant(const string& username);
void adminMenu();
void viewAllUsers();
void viewComplaints();
void viewIssues();
void viewPayments();
void joinEvent(const string& username);
void viewMyEvents(const string& username);
void changeEventState();
void marketingMenu();
void charityItemMenu();
void bookingMenu();
void accountMenu(const string& username);
void processPayment(const string& bookingId);
void paymentHistory();
string inputPassword();
void submitFeedback(const string& username, const string& feedbackType);
void viewFeedbackByOrganiser(const string& organiserName);
void viewFeedback(const string& username);
void linkCharityItemToBooking();
void viewBookingCharityItems();

struct User {
    string username;
    string password;
    string role;
    string securityQuestion;
    string securityAnswer;

    string toString() const {
        stringstream ss;
        ss << "Username: " << username << "\n"
            << "Password: " << password << "\n"
            << "Role: " << role << "\n"
            << "SecurityQuestion: " << securityQuestion << "\n"
            << "SecurityAnswer: " << securityAnswer << "\n"
            << "---\n";
        return ss.str();
    }
};

struct CharityItem {
    string name;
    string type;
    int quantity;
    double value;
    string status;

    bool operator==(const CharityItem& other) const {
        return name == other.name &&
            type == other.type &&
            quantity == other.quantity &&
            value == other.value &&
            status == other.status;
    }
};

struct Booking {
    string bookingId;
    string name;
    string choice;
    string date;
    string venue;
    string state;
    string paid;
    vector<string> charityItems;

    string toString() const {
        stringstream ss;
        ss << "BookingID: " << bookingId << "\n"
            << "Name: " << name << "\n"
            << "Choice: " << choice << "\n"
            << "Date: " << date << "\n"
            << "Venue: " << venue << "\n"
            << "State: " << state << "\n"
            << "Paid: " << paid << "\n";

        if (!charityItems.empty()) {
            ss << "CharityItems: ";
            for (size_t i = 0; i < charityItems.size(); i++) {
                ss << charityItems[i];
                if (i < charityItems.size() - 1) ss << ",";
            }
            ss << "\n";
        }
        ss << "---\n";
        return ss.str();
    }
};

struct Payment {
    string paymentId;
    string bookingId;
    string bookingName;
    string eventType;
    string date;
    double basePrice;
    double donation;
    double totalAmount;
    string bankAccount;
    string paymentStatus;

    string toString() const {
        stringstream ss;
        ss << "PaymentID: " << paymentId << "\n"
            << "BookingID: " << bookingId << "\n"
            << "BookingName: " << bookingName << "\n"
            << "EventType: " << eventType << "\n"
            << "Date: " << date << "\n"
            << "BasePrice: " << fixed << setprecision(2) << basePrice << "\n"
            << "Donation: " << fixed << setprecision(2) << donation << "\n"
            << "TotalAmount: " << fixed << setprecision(2) << totalAmount << "\n"
            << "BankAccount: " << bankAccount << "\n"
            << "PaymentStatus: " << paymentStatus << "\n"
            << "---\n";
        return ss.str();
    }
};

struct Promotion {
    string eventName, description, location, date, platform;
    int target;
    double allocatedBudget, spentBudget;
    int reach[2];
};

struct Feedback {
    string type;
    string username;
    string bookingId;
    string content;
    string feedbackId;
};

vector<User>& getUsers() {
    static vector<User> users;
    static bool loaded = false;

    if (!loaded) {
        ifstream file("users.txt");
        if (file) {
            User currentUser;
            string line;

            while (getline(file, line)) {
                if (line == "---") {
                    if (!currentUser.username.empty()) {
                        users.push_back(currentUser);
                    }
                    currentUser = User();
                }
                else if (line.find("Username: ") == 0) {
                    currentUser.username = line.substr(10);
                }
                else if (line.find("Password: ") == 0) {
                    currentUser.password = line.substr(10);
                }
                else if (line.find("Role: ") == 0) {
                    currentUser.role = line.substr(6);
                }
                else if (line.find("SecurityQuestion: ") == 0) {
                    currentUser.securityQuestion = line.substr(18);
                }
                else if (line.find("SecurityAnswer: ") == 0) {
                    currentUser.securityAnswer = line.substr(16);
                }
            }
            file.close();
        }
        loaded = true;
    }
    return users;
}

vector<Booking>& getBookings() {
    static vector<Booking> bookings;
    static bool loaded = false;

    if (!loaded) {
        ifstream file("bookings.txt");
        if (file) {
            Booking currentBooking;
            string line;

            while (getline(file, line)) {
                if (line == "---") {
                    if (!currentBooking.bookingId.empty()) {
                        bookings.push_back(currentBooking);
                    }
                    currentBooking = Booking();
                }
                else if (line.find("BookingID: ") == 0) {
                    currentBooking.bookingId = line.substr(11);
                }
                else if (line.find("Name: ") == 0) {
                    currentBooking.name = line.substr(6);
                }
                else if (line.find("Choice: ") == 0) {
                    currentBooking.choice = line.substr(8);
                }
                else if (line.find("Date: ") == 0) {
                    currentBooking.date = line.substr(6);
                }
                else if (line.find("Venue: ") == 0) {
                    currentBooking.venue = line.substr(7);
                }
                else if (line.find("State: ") == 0) {
                    currentBooking.state = line.substr(7);
                }
                else if (line.find("Paid: ") == 0) {
                    currentBooking.paid = line.substr(6);
                }
                else if (line.find("CharityItems: ") == 0) {
                    string itemsStr = line.substr(14);
                    stringstream ss(itemsStr);
                    string item;
                    while (getline(ss, item, ',')) {
                        currentBooking.charityItems.push_back(item);
                    }
                }
            }
            file.close();
        }
        loaded = true;
    }
    return bookings;
}

vector<Payment>& getPayments() {
    static vector<Payment> payments;
    static bool loaded = false;

    if (!loaded) {
        ifstream file("payments.txt");
        if (file) {
            Payment currentPayment;
            string line;

            while (getline(file, line)) {
                if (line == "---") {
                    if (!currentPayment.paymentId.empty()) {
                        payments.push_back(currentPayment);
                    }
                    currentPayment = Payment();
                }
                else if (line.find("PaymentID: ") == 0) {
                    currentPayment.paymentId = line.substr(11);
                }
                else if (line.find("BookingID: ") == 0) {
                    currentPayment.bookingId = line.substr(11);
                }
                else if (line.find("BookingName: ") == 0) {
                    currentPayment.bookingName = line.substr(13);
                }
                else if (line.find("EventType: ") == 0) {
                    currentPayment.eventType = line.substr(11);
                }
                else if (line.find("Date: ") == 0) {
                    currentPayment.date = line.substr(6);
                }
                else if (line.find("BasePrice: ") == 0) {
                    currentPayment.basePrice = stod(line.substr(11));
                }
                else if (line.find("Donation: ") == 0) {
                    currentPayment.donation = stod(line.substr(10));
                }
                else if (line.find("TotalAmount: ") == 0) {
                    currentPayment.totalAmount = stod(line.substr(13));
                }
                else if (line.find("BankAccount: ") == 0) {
                    currentPayment.bankAccount = line.substr(13);
                }
                else if (line.find("PaymentStatus: ") == 0) {
                    currentPayment.paymentStatus = line.substr(15);
                }
            }
            file.close();
        }
        loaded = true;
    }
    return payments;
}

vector<CharityItem>& getCharityInventory() {
    static vector<CharityItem> charityInventory;
    static bool loaded = false;

    if (!loaded) {
        ifstream file("charity_inventory.txt");
        if (file) {
            CharityItem item;
            string line;
            while (getline(file, line)) {
                if (line.empty()) continue;
                if (line == "---") {
                    charityInventory.push_back(item);
                    item = CharityItem();
                }
                else {
                    size_t colon = line.find(':');
                    if (colon != string::npos) {
                        string key = line.substr(0, colon);
                        string value = line.substr(colon + 2);

                        if (key == "Name") item.name = value;
                        else if (key == "Type") item.type = value;
                        else if (key == "Quantity") item.quantity = stoi(value);
                        else if (key == "Value") item.value = stod(value);
                        else if (key == "Status") item.status = value;
                    }
                }
            }
            file.close();
        }
        loaded = true;
    }
    return charityInventory;
}

void saveUsers() {
    auto& users = getUsers();
    ofstream file("users.txt");
    for (const auto& user : users) {
        file << user.toString();
    }
    file.close();
}

void saveBookings() {
    auto& bookings = getBookings();
    ofstream file("bookings.txt");
    for (const auto& booking : bookings) {
        file << booking.toString();
    }
    file.close();
}

void savePayments() {
    auto& payments = getPayments();
    ofstream file("payments.txt");
    for (const auto& payment : payments) {
        file << payment.toString();
    }
    file.close();
}

bool saveCharityInventory() {
    auto& charityInventory = getCharityInventory();
    ofstream file("charity_inventory.txt");

    if (!file) {
        cout << "Error: Could not open charity_inventory.txt for writing\n";
        return false;
    }

    for (const auto& item : charityInventory) {
        file << "Name: " << item.name << "\n";
        file << "Type: " << item.type << "\n";
        file << "Quantity: " << item.quantity << "\n";
        file << "Value: " << fixed << setprecision(2) << item.value << "\n";
        file << "Status: " << item.status << "\n";
        file << "---\n";
    }

    file.close();
    return true;
}

string generateBookingId() {
    auto& bookings = getBookings();
    return "B" + to_string(bookings.size() + 1);
}

string generatePaymentId() {
    auto& payments = getPayments();
    return "P" + to_string(payments.size() + 1);
}

string generateFeedbackId() {
    static int count = 0;
    ifstream file("feedback.txt");
    if (file) {
        string line;
        while (getline(file, line)) {
            if (line.find("FeedbackID: ") == 0) {
                count++;
            }
        }
        file.close();
    }
    return "F" + to_string(count + 1);
}

void pressEnterToContinue() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string findUserByUsername(const string& username) {
    auto& users = getUsers();
    for (const auto& user : users) {
        if (user.username == username) {
            return user.username;
        }
    }
    return "";
}

void clearScreen() {
    system("cls");
}

string hidePass() {
    string password;
    char ch;

    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

string inputPassword() {
    string pass;
    char ch;

    while (true) {
        ch = _getch();
        if (ch == 13) {
            cout << endl;
            break;
        }
        else if (ch == 8) {
            if (!pass.empty()) {
                pass.pop_back();
                cout << "\b \b";
            }
        }
        else {
            pass.push_back(ch);
            cout << '*';
        }
    }
    return pass;
}

bool isValidDateFormat(const string& date) {
    if (date.size() != 10) return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') return false;
        }
        else {
            if (!isdigit(date[i])) return false;
        }
    }

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    }

    return true;
}

void userRegister() {
    auto& users = getUsers();
    User newUser;

    while (true) {
        cout << "Enter username: ";
        getline(cin, newUser.username);
        if (newUser.username.empty()) {
            cout << "Username cannot be empty!\n";
            continue;
        }
        if (findUserByUsername(newUser.username) != "") {
            cout << "Username already exists!\n";
            continue;
        }
        break;
    }

    while (true) {
        cout << "Enter password (at least 4 chars, must include uppercase, lowercase and number): ";
        getline(cin, newUser.password);

        if (newUser.password.empty()) {
            cout << "Password cannot be empty!\n";
            continue;
        }

        if (!regex_match(newUser.password, regex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[A-Za-z\\d]{4,}$"))) {
            cout << "Password invalid! Must be at least 4 characters, include uppercase, lowercase and number.\n";
            continue;
        }
        break;
    }

    while (true) {
        cout << "Enter role (o = organiser, p = participant): ";
        getline(cin, newUser.role);

        if (newUser.role.empty()) {
            cout << "Role cannot be empty!\n";
            continue;
        }

        newUser.role.erase(0, newUser.role.find_first_not_of(" \t\n\r\f\v"));
        newUser.role.erase(newUser.role.find_last_not_of(" \t\n\r\f\v") + 1);

        if (newUser.role.length() != 1) {
            cout << "Invalid input! Please enter only one character (o or p).\n";
            continue;
        }

        char roleChar = tolower(newUser.role[0]);
        if (roleChar == 'o') {
            newUser.role = "organiser";
            break;
        }
        else if (roleChar == 'p') {
            newUser.role = "participant";
            break;
        }
        else {
            cout << "Invalid role! Please enter 'o' for organiser or 'p' for participant.\n";
        }
    }

    vector<string> questions = {
        "What is your pet's name?",
        "What is your favourite food?",
        "What was the name of your secondary school?",
        "What is your favourite color?",
        "What city were you born in?"
    };

    cout << "\nChoose a security question: \n";
    for (int i = 0; i < (int)questions.size(); i++) {
        cout << i + 1 << ". " << questions[i] << endl;
    }

    int choice;
    cout << "Enter the number of your choice: ";
    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number: ";
    }
    cin.ignore();

    if (choice < 1 || choice >(int)questions.size()) {
        cout << "Invalid choice. Defaulting to first question.\n";
        choice = 1;
    }

    newUser.securityQuestion = questions[choice - 1];

    do {
        cout << "Enter your answer: ";
        getline(cin, newUser.securityAnswer);
        if (newUser.securityAnswer.empty()) {
            cout << "Answer cannot be empty. Please try again.\n";
        }
    } while (newUser.securityAnswer.empty());

    users.push_back(newUser);
    saveUsers();

    if (newUser.role == "organiser") {
        string bankAcc, bankPass;

        cout << "Enter bank account number (8 digits): ";
        getline(cin, bankAcc);
        while (!regex_match(bankAcc, regex("^[0-9]{8}$"))) {
            cout << "Invalid account number! Must be 8 digits: ";
            getline(cin, bankAcc);
        }

        cout << "Enter password (at least 6 chars, must include uppercase, lowercase and number): ";
        getline(cin, bankPass);
        while (!regex_match(bankPass, regex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[A-Za-z\\d]{6,}$"))) {
            cout << "Password invalid! Enter again: ";
            getline(cin, bankPass);
        }

        ofstream bankFile("bank_accounts.txt", ios::app);
        bankFile << bankAcc << " " << bankPass << endl;
        bankFile.close();

        cout << "Bank account created successfully!\n";
    }

    cout << "Registration successful!\n";
}

void userLogin() {
    auto& users = getUsers();

    string username;
    cout << "Enter username: ";
    getline(cin, username);

    // admin
    if (username == "admin") {
        cout << "Enter password: ";
        string password = hidePass();
        if (password == "Admin789") {
            cout << "\nAdmin login successful!\n";
            adminMenu();
            return;
        }
        else {
            cout << "Invalid admin password!\n";
            return;
        }
    }

    string foundUsername = findUserByUsername(username);
    if (foundUsername == "") {
        cout << "Username not found. Please register first or check the username.\n";
        return;
    }

    User* userPtr = nullptr;
    for (auto& user : users) {
        if (user.username == username) {
            userPtr = &user;
            break;
        }
    }

    if (!userPtr) {
        cout << "User not found!\n";
        return;
    }

    User& user = *userPtr;

    int attempts = 0;
    while (attempts < 3) {
        cout << "Enter password: ";
        string password = hidePass();

        if (password == user.password) {
            cout << "\nLogin successful!\n";
            if (user.role == "organiser") {
                organiser(user.username);
            }
            else if (user.role == "participant") {
                participant(user.username);
            }
            else {
                cout << "Unknown role in file!\n";
            }
            return;
        }
        else {
            attempts++;
            if (attempts < 3) {
                cout << "Invalid password! Attempts left: " << (3 - attempts) << "\n";
            }
        }
    }

    char opt;
    cout << "\nYou entered wrong password 3 times.\n";
    cout << "Do you want to change your password? (y/n): ";
    cin >> opt;
    cin.ignore();

    if (opt == 'y' || opt == 'Y') {
        cout << "Security Question: " << user.securityQuestion << endl;
        cout << "Your answer: ";
        string answer;
        getline(cin, answer);

        if (answer == user.securityAnswer) {
            changeOwnPassword(username);
        }
        else {
            cout << "Incorrect security answer. Cannot reset password.\n";
        }
    }
    else {
        cout << "Returning to main menu.\n";
    }
}

void changeOwnPassword(const string& username) {
    auto& users = getUsers();

    User* userPtr = nullptr;
    for (auto& user : users) {
        if (user.username == username) {
            userPtr = &user;
            break;
        }
    }

    if (!userPtr) {
        cout << "No such user.\n";
        pressEnterToContinue();
        return;
    }

    User& user = *userPtr;

    cout << "Enter new password: ";
    string newPass = hidePass();

    if (newPass.empty()) {
        cout << "Password cannot be empty.\n";
        pressEnterToContinue();
        return;
    }

    user.password = newPass;
    saveUsers();

    cout << "Password updated successfully!\n";
    pressEnterToContinue();
}

void updateUser() {
    auto& users = getUsers();
    if (users.empty()) {
        cout << "No users found!\n";
        pressEnterToContinue();
        return;
    }

    cout << "\n--- Users ---\n";
    for (int i = 0; i < (int)users.size(); i++) {
        cout << i + 1 << ". " << users[i].username << " (" << users[i].role << ")\n";
    }

    cout << "Select user to update (0 cancel): ";
    int opt;
    while (true) {
        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.Numbers only\n";
            continue;
        }
        else if (opt < 0 || opt >(int)users.size()) {
            cout << "Invalid choice.\n";
            continue;
        }
        else if (opt == 0) {
            cout << "Cancelled.\n";
            return;
        }
        else {
            break;
        }
    }
    cin.ignore();

    User& user = users[opt - 1];

    cout << "Enter new username (leave blank to keep current: " << user.username << "): ";
    string newU;
    getline(cin, newU);
    if (!newU.empty()) {
        bool usernameExists = false;
        for (const auto& u : users) {
            if (u.username == newU && u.username != user.username) {
                usernameExists = true;
                break;
            }
        }

        if (usernameExists) {
            cout << "Username already exists. Skip username change.\n";
        }
        else {
            user.username = newU;
        }
    }

    cout << "To change password, answer security question first:\n";
    cout << user.securityQuestion << "\n";
    cout << "Your answer: ";
    string ans;
    getline(cin, ans);
    if (!ans.empty() && ans == user.securityAnswer) {
        cout << "Enter new password: ";
        string np = hidePass();
        if (!np.empty()) user.password = np;
    }
    else {
        cout << "Incorrect security answer. Password not changed.\n";
    }

    cout << "Enter new role (o = organiser, p = participant, leave blank to keep current: " << user.role << "): ";
    string newR;
    getline(cin, newR);
    if (!newR.empty()) {
        char r = tolower(newR[0]);
        if (r == 'o') user.role = "organiser";
        else if (r == 'p') user.role = "participant";
        else cout << "Invalid role input - keeping old role.\n";
    }

    saveUsers();
    cout << "User updated successfully!\n";
    pressEnterToContinue();
}

void deleteUser() {
    auto& users = getUsers();
    if (users.empty()) {
        cout << "No users found!\n";
        pressEnterToContinue();
        return;
    }

    cout << "\n--- Users ---\n";
    for (int i = 0; i < (int)users.size(); i++) {
        cout << i + 1 << ". " << users[i].username << " (" << users[i].role << ")\n";
    }

    cout << "Select user to delete (0 cancel): ";
    int opt;
    while (true) {
        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.Numbers only\n";
            continue;
        }
        else if (opt < 0 || opt >(int)users.size()) {
            cout << "Invalid choice.\n";
            continue;
        }
        else if (opt == 0) {
            cout << "Cancelled.\n";
            return;
        }
        else {
            break;
        }
    }
    cin.ignore();

    string usernameToDelete = users[opt - 1].username;
    users.erase(users.begin() + opt - 1);

    saveUsers();
    cout << "User " << usernameToDelete << " deleted successfully!\n";
    pressEnterToContinue();
}

void updateProfile(const string& username) {
    auto& users = getUsers();

    bool found = false;
    for (auto& usr : users) {
        if (usr.username == username) {
            found = true;
            cout << "\n--- Update Your Profile ---\n";

            string newUsername;
            cout << "Enter new username (leave blank to keep current): ";
            getline(cin, newUsername);
            if (!newUsername.empty()) usr.username = newUsername;

            string newPassword;
            cout << "Enter new password (leave blank to keep current): ";
            getline(cin, newPassword);
            if (!newPassword.empty()) usr.password = newPassword;

            break;
        }
    }

    if (!found) {
        cout << "Your profile was not found!\n";
        return;
    }

    saveUsers();
    cout << "Profile updated successfully!\n";
    pressEnterToContinue();

}

void deleteProfile(const string& username) {
    auto& users = getUsers();

    bool deleted = false;
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].username == username) {
            cout << "Deleting your account: " << users[i].username << "\n";
            users.erase(users.begin() + i);
            deleted = true;
            break;
        }
    }

    if (!deleted) {
        cout << "Your account was not found!\n";
        return;
    }

    saveUsers();
    cout << "Your account has been deleted. Logging out...\n";
    exit(0);
}

void createBooking() {
    auto& bookings = getBookings();
    Booking newBooking;

    newBooking.bookingId = "B" + to_string(bookings.size() + 1);

    cout << "\n----- Booking Events -----\n";
    cout << "> Food Truck (FT)\n";
    cout << "> Charity Run (CR)\n";
    cout << "> Charity Concert (CC)\n";

    string choice;
    while (true) {
        cout << "Choose booking event: ";
        getline(cin, choice);

        if (choice.empty()) {
            cout << "No empty! Please enter a value.\n";
            continue;
        }

        string choiceLower = choice;
        transform(choiceLower.begin(), choiceLower.end(), choiceLower.begin(), ::tolower);

        if (choiceLower == "ft" || choiceLower == "food truck") {
            newBooking.choice = "Food Truck";
            break;
        }
        else if (choiceLower == "cr" || choiceLower == "charity run") {
            newBooking.choice = "Charity Run";
            break;
        }
        else if (choiceLower == "cc" || choiceLower == "charity concert") {
            newBooking.choice = "Charity Concert";
            break;
        }
        else {
            cout << "Invalid event name! Enter FT, CR, CC, or full name.\n";
            cout << "You can use lowercase letters too.\n";
        }
    }

    do {
        cout << "Enter the name for event: ";
        getline(cin, newBooking.name);

        if (newBooking.name.empty()) {
            cout << "No empty! Please enter event name.\n";
        }
    } while (newBooking.name.empty());

    while (true) {
        cout << "Enter date (YYYY-MM-DD): ";
        getline(cin, newBooking.date);

        if (newBooking.date.empty()) {
            cout << "No empty! Please enter a date.\n";
            continue;
        }

        if (isValidDateFormat(newBooking.date)) {
            break;
        }
        else {
            cout << "Invalid date! Please enter a valid date in YYYY-MM-DD format.\n";
            cout << "Example: 2024-12-25\n";
        }
    }

    do {
        cout << "Enter venue: ";
        getline(cin, newBooking.venue);

        if (newBooking.venue.empty()) {
            cout << "No empty! Please enter venue.\n";
        }
    } while (newBooking.venue.empty());

    newBooking.state = "Pending";
    newBooking.paid = "No";

    bookings.push_back(newBooking);
    saveBookings();
    cout << "Booking " << newBooking.bookingId << " created successfully!\n";
    pressEnterToContinue();
}

void viewBooking() {
    auto& bookings = getBookings();
    if (bookings.empty()) {
        cout << "No bookings found!\n";
        pressEnterToContinue();
        return;
    }

    clearScreen();
    cout << "\n----- All Bookings -----\n";
    for (const auto& booking : bookings) {
        cout << "Booking ID: " << booking.bookingId << "\n";
        cout << "Name: " << booking.name << "\n";
        cout << "Choice: " << booking.choice << "\n";
        cout << "Date: " << booking.date << "\n";
        cout << "Venue: " << booking.venue << "\n";
        cout << "State: " << booking.state << "\n";
        cout << "Paid: " << booking.paid << "\n";

        if (!booking.charityItems.empty()) {
            cout << "Charity Items: ";
            for (size_t i = 0; i < booking.charityItems.size(); i++) {
                cout << booking.charityItems[i];
                if (i < booking.charityItems.size() - 1) cout << ", ";
            }
            cout << "\n";
        }
        cout << "-----------------------------------\n";
    }

    pressEnterToContinue();
}

void updateBooking() {
    auto& bookings = getBookings();
    if (bookings.empty()) {
        cout << "No bookings to update!\n";
        pressEnterToContinue();
        return;
    }

    cout << "\n--- Bookings ---\n";
    for (int i = 0; i < (int)bookings.size(); i++) {
        cout << i + 1 << ". " << bookings[i].bookingId << " (" << bookings[i].name << ")\n";
    }

    int choice;
    while (true) {
        cout << "Select booking to update (0 cancel): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            pressEnterToContinue();
            return;
        }
        cin.ignore();

        if (choice < 0 || choice >(int)bookings.size()) {
            cout << "Invalid choice. Please select a valid booking.\n";
            continue;
        }

        if (choice == 0) {
            cout << "Cancelled updating....\n";
            pressEnterToContinue();
            return;
        }

        Booking& booking = bookings[choice - 1];

        string newInput;
        cout << "Enter new name (blank = keep current: " << booking.name << "): ";
        getline(cin, newInput);
        if (!newInput.empty()) booking.name = newInput;

        while (true) {
            cout << "Enter new date (YYYY-MM-DD, blank = keep current: " << booking.date << "): ";
            getline(cin, newInput);
            if (newInput.empty()) break;
            if (isValidDateFormat(newInput)) {
                booking.date = newInput;
                break;
            }
            else {
                cout << "Invalid date format! Try again.\n";
            }
        }

        cout << "Enter new venue (blank = keep current: " << booking.venue << "): ";
        getline(cin, newInput);
        if (!newInput.empty()) booking.venue = newInput;

        while (true) {
            cout << "Enter new state (Pending/Ongoing/Completed/Cancel, blank = keep current: " << booking.state << "): ";
            getline(cin, newInput);
            if (newInput.empty()) break;
            if (newInput == "Pending" || newInput == "Ongoing" || newInput == "Completed" || newInput == "Cancel") {
                booking.state = newInput;
                break;
            }
            else {
                cout << "Invalid state! Try again.\n";
            }
        }

        saveBookings();
        cout << "Booking updated successfully!\n";
        pressEnterToContinue();
    }
}

void deleteBooking() {
    auto& bookings = getBookings();
    if (bookings.empty()) {
        cout << "No bookings to delete!\n";
        pressEnterToContinue();
        return;
    }

    cout << "\n--- Bookings ---\n";
    for (int i = 0; i < (int)bookings.size(); i++) {
        cout << i + 1 << ". " << bookings[i].bookingId << " (" << bookings[i].name << ")\n";
    }

    int choice;
    while (true) {
        cout << "Select booking to delete (0 cancel): ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            pressEnterToContinue();
            return;
        }
        cin.ignore();

        if (choice < 0 || choice >(int)bookings.size()) {
            cout << "Invalid choice. Please select a valid booking.\n";
            continue;
        }

        if (choice == 0) {
            cout << "Cancelled deleting....\n";
            pressEnterToContinue();
            return;
        }

        string bookingIdToDelete = bookings[choice - 1].bookingId;
        bookings.erase(bookings.begin() + choice - 1);

        saveBookings();
        cout << "Booking " << bookingIdToDelete << " deleted successfully!\n";
        pressEnterToContinue();
        return;
    }
}

void joinEvent(const string& username) {
    auto& bookings = getBookings();
    if (bookings.empty()) {
        cout << "No bookings found!\n";
        pressEnterToContinue();
        return;
    }

    clearScreen();
    cout << "\n--- Available Events ---\n";
    for (int i = 0; i < (int)bookings.size(); i++) {
        if (bookings[i].state != "Cancel") {
            cout << i + 1 << ". " << bookings[i].name
                << " | " << bookings[i].date
                << " | " << bookings[i].venue
                << " | State: " << bookings[i].state << "\n";
        }
    }

    while (true) {
        cout << "\nEnter event number to join (0 to cancel): ";
        int opt;
        cin >> opt;
        cin.ignore();

        if (opt < 0 || opt >(int)bookings.size()) {
            cout << "Invalid choice. Please enter a valid event.\n";
            continue;
        }

        if (opt == 0) {
            cout << "Cancelled participation.\n";
            pressEnterToContinue();
            return;
        }

        string bookingId = bookings[opt - 1].bookingId;

        ofstream out("participants.txt", ios::app);
        out << username << "\n";
        out << bookingId << "\n";
        out << "---\n";
        out.close();

        cout << "Successfully joined event: " << bookings[opt - 1].name << endl;
        pressEnterToContinue();
    }
}

void viewMyEvents(const string& username) {
    ifstream file("participants.txt");
    if (!file) {
        cout << "No participation records found!\n";
        pressEnterToContinue();
        return;
    }

    vector<string> joinedIds;
    string user, bookingId, sep;
    while (getline(file, user)) {
        if (user.empty() || user == "---") continue;
        getline(file, bookingId);
        getline(file, sep);

        if (user == username) {
            joinedIds.push_back(bookingId);
        }
    }
    file.close();

    if (joinedIds.empty()) {
        cout << "You have not joined any events yet.\n";
        pressEnterToContinue();
        return;
    }

    clearScreen();
    auto& bookings = getBookings();
    cout << "\n--- My Events ---\n";
    bool foundEvents = false;

    for (const auto& id : joinedIds) {
        for (const auto& booking : bookings) {
            if (booking.bookingId == id) {
                foundEvents = true;
                cout << "Event: " << booking.name << "\n"
                    << "Type: " << booking.choice << "\n"
                    << "Date: " << booking.date << "\n"
                    << "Venue: " << booking.venue << "\n"
                    << "State: " << booking.state << "\n"
                    << "Booking ID: " << booking.bookingId << "\n"
                    << "--------------------------------\n";
                break;
            }
        }
    }

    if (!foundEvents) {
        cout << "No events found for your participation records.\n";
    }
    pressEnterToContinue();
}

void changeEventState() {
    auto& bookings = getBookings();
    if (bookings.empty()) {
        cout << "No bookings found!\n";
        pressEnterToContinue();
        return;
    }

    while (true) {
        clearScreen();
        cout << "\n=== Monitoring Event Status ===\n";

        for (int i = 0; i < (int)bookings.size(); i++) {
            cout << i + 1 << ". "
                << "Name: " << bookings[i].name
                << " | Date: " << bookings[i].date
                << " | Type: " << bookings[i].choice
                << " | Status: " << bookings[i].state << "\n";
        }

        cout << "\nSelect event to update status (0 to cancel): ";
        int opt;
        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            pressEnterToContinue();
            continue;
        }
        cin.ignore();

        if (opt == 0) break;
        if (opt < 1 || opt >(int)bookings.size()) {
            cout << "Invalid choice!\n";
            pressEnterToContinue();
            continue;
        }

        Booking& booking = bookings[opt - 1];

        while (true) {
            cout << "\nSelected Event: " << booking.name << "\n";
            cout << "Current State: " << booking.state << "\n";
            cout << "Enter new state (Ongoing / Completed / Canceled): ";

            string newState;
            getline(cin, newState);

            transform(newState.begin(), newState.end(), newState.begin(), ::tolower);
            if (newState == "pending") newState = "Pending";
            else if (newState == "ongoing") newState = "Ongoing";
            else if (newState == "completed") newState = "Completed";
            else if (newState == "canceled" || newState == "cancel") newState = "Canceled";

            if (newState != "Pending" && newState != "Ongoing" &&
                newState != "Completed" && newState != "Canceled") {
                cout << "Invalid state. Try again.\n";
                continue;
            }

            if (newState == booking.state) {
                cout << "New state cannot be the same as current state. Try again.\n";
                continue;
            }

            booking.state = newState;
            cout << "Event state updated!\n";
            saveBookings();
            break;
        }

        cout << "\nDo you want to update another event? (y/n): ";
        char continueChoice;
        cin >> continueChoice;
        cin.ignore();

        if (tolower(continueChoice) != 'y') {
            pressEnterToContinue();
            break;
        }

    }
}

void loadPromotions(Promotion promotions[], int& count) {
    ifstream file("promotions.txt");
    if (!file) return;

    while (true) {
        Promotion p;
        if (!getline(file, p.eventName)) break;
        if (p.eventName.empty()) break;
        getline(file, p.description);
        getline(file, p.location);
        file >> p.target;
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(file, p.date);
        getline(file, p.platform);
        file >> p.allocatedBudget >> p.spentBudget;
        file >> p.reach[0] >> p.reach[1];
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        promotions[count++] = p;
        if (!file) break;
    }
    file.close();
}

void savePromotions(Promotion promotions[], int count) {
    ofstream file("promotions.txt");
    for (int i = 0; i < count; i++) {
        file << promotions[i].eventName << "\n"
            << promotions[i].description << "\n"
            << promotions[i].location << "\n"
            << promotions[i].target << "\n"
            << promotions[i].date << "\n"
            << promotions[i].platform << "\n"
            << promotions[i].allocatedBudget << " " << promotions[i].spentBudget << "\n"
            << promotions[i].reach[0] << " " << promotions[i].reach[1] << "\n";
    }
    file.close();
}

void addPromotion(Promotion promotions[], int& count) {
    Promotion p;

    while (true) {
        cout << "Enter Event Name: ";
        getline(cin, p.eventName);
        if (!p.eventName.empty()) break;
        cout << "Event name cannot be empty!\n";
    }

    while (true) {
        cout << "Enter Description: ";
        getline(cin, p.description);
        if (!p.description.empty()) break;
        cout << "Description cannot be empty!\n";
    }

    while (true) {
        cout << "Enter Location: ";
        getline(cin, p.location);
        if (!p.location.empty()) break;
        cout << "Location cannot be empty!\n";
    }

    while (true) {
        cout << "Enter Target Audience: ";
        if (cin >> p.target) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    while (true) {
        cout << "Enter Event Date (YYYY-MM-DD): ";
        if (!getline(cin, p.date)) {
            cin.clear();
            p.date.clear();
        }

        p.date.erase(remove_if(p.date.begin(), p.date.end(), [](unsigned char c) { return isspace(c); }), p.date.end());

        if (isValidDateFormat(p.date)) break;
        cout << "Invalid date! Please enter a valid date with format YYYY-MM-DD.\n";
    }

    while (true) {
        cout << "Enter Promotional Platform: ";
        getline(cin, p.platform);
        if (!p.platform.empty()) break;
        cout << "Promotional Platform cannot be empty!\n";
    }

    while (true) {
        cout << "Allocated Budget: ";
        if (cin >> p.allocatedBudget) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    while (true) {
        cout << "Spent Budget: ";
        if (cin >> p.spentBudget) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    while (true) {
        cout << "Enter Male Reach: ";
        if (cin >> p.reach[0]) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    while (true) {
        cout << "Enter Female Reach: ";
        if (cin >> p.reach[1]) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    promotions[count++] = p;
    cout << "\nPromotion added!\nTotal Audience Reached: " << (p.reach[0] + p.reach[1]) << "\n";
}

void viewPromotions(Promotion promotions[], int count) {
    if (count == 0) {
        cout << "No promotions.\n";
        return;
    }
    cout << left
        << setw(5) << "No" << setw(20) << "Event" << setw(23) << "Description" << setw(12) << "Location"
        << setw(15) << "Platform" << setw(10) << "Budget" << setw(10) << "Spent"
        << setw(8) << "Male" << setw(8) << "Female" << setw(10) << "Total" << "\n";

    for (int i = 0; i < count; i++) {
        int male = promotions[i].reach[0], female = promotions[i].reach[1];
        cout << setw(5) << i + 1
            << setw(20) << promotions[i].eventName
            << setw(23) << promotions[i].description
            << setw(12) << promotions[i].location
            << setw(15) << promotions[i].platform
            << setw(10) << promotions[i].allocatedBudget
            << setw(10) << promotions[i].spentBudget
            << setw(8) << male
            << setw(8) << female
            << setw(10) << (male + female) << "\n";
    }
}

void editPromotion(Promotion promotions[], int count) {
    int idx;
    viewPromotions(promotions, count);
    cout << "Enter number: ";
    cin >> idx;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (idx < 1 || idx > count) {
        cout << "Invalid.\n";
        return;
    }
    Promotion& p = promotions[idx - 1];
    cout << "Editing " << p.eventName << "\n";

    string input;
    cout << "New Description (" << p.description << "): ";
    getline(cin, input);
    if (!input.empty()) p.description = input;

    cout << "New Location (" << p.location << "): ";
    getline(cin, input);
    if (!input.empty()) p.location = input;

    cout << "New Spent Budget (" << p.spentBudget << "): ";
    getline(cin, input);
    if (!input.empty()) {
        try {
            p.spentBudget = stoi(input);
        }
        catch (...) {
            cout << "Invalid input, keeping old value.\n";
        }
    }

    cout << "New Male Reach (" << p.reach[0] << "): ";
    getline(cin, input);
    if (!input.empty()) {
        try {
            p.reach[0] = stoi(input);
        }
        catch (...) {
            cout << "Invalid input, keeping old value.\n";
        }
    }

    cout << "New Female Reach (" << p.reach[1] << "): ";
    getline(cin, input);
    if (!input.empty()) {
        try {
            p.reach[1] = stoi(input);
        }
        catch (...) {
            cout << "Invalid input, keeping old value.\n";
        }
    }

    cout << "Updated!\n";
}

void deletePromotion(Promotion promotions[], int& count) {
    int idx;
    viewPromotions(promotions, count);
    cout << "Enter number: ";
    cin >> idx;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (idx < 1 || idx > count) {
        cout << "Invalid.\n";
        return;
    }
    for (int i = idx - 1; i < count - 1; i++) {
        promotions[i] = promotions[i + 1];
    }
    count--;
    cout << "Deleted!\n";
}

void searchPromotion(Promotion promotions[], int count) {
    string key;
    cout << "Enter keyword: ";
    getline(cin, key);

    bool any = false;
    for (int i = 0; i < count; i++) {
        if (promotions[i].eventName.find(key) != string::npos ||
            promotions[i].description.find(key) != string::npos)
        {
            any = true;
            cout << "Found: " << promotions[i].eventName
                << " (" << promotions[i].description << ")"
                << " on " << promotions[i].platform
                << " (Total reach " << promotions[i].reach[0] + promotions[i].reach[1] << ")\n";
        }
    }
    if (!any) cout << "No matching promotions.\n";
}

void addItem() {
    auto& charityInventory = getCharityInventory();
    const int MAX_ITEMS = 100;
    const int MAX_QUANTITY = 100000;
    const double MAX_VALUE = 100000.0;

    if (charityInventory.size() >= MAX_ITEMS) {
        cout << "Item list is full.\n";
        pressEnterToContinue();
        return;
    }

    CharityItem newItem;

    while (true) {
        cout << "Enter Item Name: ";
        getline(cin, newItem.name);
        if (!newItem.name.empty()) {
            bool itemExists = false;
            for (const auto& item : charityInventory) {
                if (item.name == newItem.name) {
                    itemExists = true;
                    break;
                }
            }
            if (itemExists) {
                cout << "Item with this name already exists! Please use a different name.\n";
                continue;
            }
            break;
        }
        cout << "Item Name cannot be empty!\n";
    }

    while (true) {
        cout << "Enter Item Type: ";
        getline(cin, newItem.type);
        if (!newItem.type.empty()) break;
        cout << "Item Type cannot be empty!\n";
    }

    while (true) {
        cout << "Enter Item Quantity (0-" << MAX_QUANTITY << "): ";
        string input;
        getline(cin, input);

        if (input.empty()) {
            cout << "Quantity cannot be empty!\n";
            continue;
        }

        try {
            newItem.quantity = stoi(input);
            if (newItem.quantity >= 0 && newItem.quantity <= MAX_QUANTITY) {
                break;
            }
            cout << "Quantity must be between 0 and " << MAX_QUANTITY << "!\n";
        }
        catch (...) {
            cout << "Invalid input! Please enter a valid number.\n";
        }
    }

    while (true) {
        cout << "Enter Item Value (RM 0.00 - " << MAX_VALUE << "): ";
        string input;
        getline(cin, input);

        if (input.empty()) {
            cout << "Value cannot be empty!\n";
            continue;
        }

        try {
            newItem.value = stod(input);
            if (newItem.value >= 0 && newItem.value <= MAX_VALUE) {
                break;
            }
            cout << "Value must be between RM 0.00 and RM " << MAX_VALUE << "!\n";
        }
        catch (...) {
            cout << "Invalid input! Please enter a valid number.\n";
        }
    }

    if (newItem.quantity == 0) {
        newItem.status = "Out Stock";
    }
    else if (newItem.quantity < 10) {
        newItem.status = "Low Stock";
    }
    else {
        newItem.status = "Available";
    }

    charityInventory.push_back(newItem);

    if (saveCharityInventory()) {
        double totalValue = newItem.value * newItem.quantity;
        cout << "Item added successfully!\n";
        cout << "Total value: RM " << fixed << setprecision(2) << totalValue << endl;
        cout << "Status: " << newItem.status << endl;
    }
    else {
        cout << "Error saving item to file!\n";
    }

    pressEnterToContinue();
}

void viewItems() {
    auto& charityInventory = getCharityInventory();
    if (charityInventory.empty()) {
        cout << "No charity items available.\n";
        pressEnterToContinue();
        return;
    }

    cout << "\n===================================== Charity Items Inventory =====================================\n";
    cout << left << setw(5) << "No"
        << setw(25) << "Name"
        << setw(15) << "Type"
        << setw(15) << "Quantity"
        << setw(12) << "Value"
        << setw(15) << "Status"
        << setw(15) << "Total Value" << "\n";
    cout << string(99, '-') << "\n";

    double grandTotal = 0.0;
    for (int i = 0; i < (int)charityInventory.size(); i++) {
        const auto& item = charityInventory[i];
        double itemTotalValue = item.value * item.quantity;
        grandTotal += itemTotalValue;

        cout << setw(5) << i + 1
            << setw(25) << (item.name.length() > 24 ? item.name.substr(0, 24) + "." : item.name)
            << setw(15) << item.type
            << setw(15) << item.quantity
            << setw(12) << fixed << setprecision(2) << item.value
            << setw(15) << item.status
            << setw(15) << fixed << setprecision(2) << itemTotalValue << "\n";
    }

    cout << string(99, '-') << "\n";
    cout << "Total Items: " << charityInventory.size()
        << " | Grand Total Value: RM " << fixed << setprecision(2) << grandTotal << "\n";

    pressEnterToContinue();
}

void updateItem() {
    auto& charityInventory = getCharityInventory();
    if (charityInventory.empty()) {
        cout << "No items available to update.\n";
        pressEnterToContinue();
        return;
    }

    viewItems();

    cout << "Enter item number to update (0 to cancel): ";
    string input;
    getline(cin, input);

    if (input.empty() || input == "0") {
        cout << "Update cancelled.\n";
        pressEnterToContinue();
        return;
    }

    int idx;
    try {
        idx = stoi(input);
        if (idx < 1 || idx >(int)charityInventory.size()) {
            cout << "Invalid item number.\n";
            pressEnterToContinue();
            return;
        }
    }
    catch (...) {
        cout << "Invalid input! Please enter a valid number.\n";
        pressEnterToContinue();
        return;
    }

    CharityItem& item = charityInventory[idx - 1];

    cout << "\nUpdating item: " << item.name << "\n";

    cout << "Enter new Name (leave blank to keep current: '" << item.name << "'): ";
    string newName;
    getline(cin, newName);
    if (!newName.empty()) {
        bool nameExists = false;
        for (const auto& existingItem : charityInventory) {
            if (existingItem.name == newName && &existingItem != &item) {
                nameExists = true;
                break;
            }
        }
        if (nameExists) {
            cout << "Item with this name already exists! Name not changed.\n";
        }
        else {
            item.name = newName;
        }
    }

    cout << "Enter new Type (leave blank to keep current: '" << item.type << "'): ";
    string newType;
    getline(cin, newType);
    if (!newType.empty()) {
        item.type = newType;
    }

    cout << "Enter new Quantity (current: " << item.quantity << ", enter -1 to keep current): ";
    string qtyInput;
    getline(cin, qtyInput);
    if (!qtyInput.empty() && qtyInput != "-1") {
        try {
            int newQty = stoi(qtyInput);
            if (newQty >= 0 && newQty <= 10000) {
                item.quantity = newQty;
            }
            else {
                cout << "Invalid quantity! Must be between 0 and 10000. Quantity not changed.\n";
            }
        }
        catch (...) {
            cout << "Invalid input! Quantity not changed.\n";
        }
    }

    cout << "Enter new Value (current: RM " << fixed << setprecision(2) << item.value << ", enter -1 to keep current): ";
    string valueInput;
    getline(cin, valueInput);
    if (!valueInput.empty() && valueInput != "-1") {
        try {
            double newValue = stod(valueInput);
            if (newValue >= 0 && newValue <= 1000000.0) {
                item.value = newValue;
            }
            else {
                cout << "Invalid value! Must be between RM 0.00 and RM 1,000,000.00. Value not changed.\n";
            }
        }
        catch (...) {
            cout << "Invalid input! Value not changed.\n";
        }
    }

    if (item.quantity == 0) {
        item.status = "Out Stock";
    }
    else if (item.quantity < 10) {
        item.status = "Low Stock";
    }
    else {
        item.status = "Available";
    }

    if (saveCharityInventory()) {
        cout << "Item updated successfully!\n";
        cout << "New status: " << item.status << endl;
    }
    else {
        cout << "Error saving changes!\n";
    }

    pressEnterToContinue();
}

void deleteItem() {
    auto& charityInventory = getCharityInventory();
    if (charityInventory.empty()) {
        cout << "No items available to delete.\n";
        pressEnterToContinue();
        return;
    }

    viewItems();

    cout << "Enter item number to delete (0 to cancel): ";
    string input;
    getline(cin, input);

    if (input.empty() || input == "0") {
        cout << "Deletion cancelled.\n";
        pressEnterToContinue();
        return;
    }

    int idx;
    try {
        idx = stoi(input);
        if (idx < 1 || idx >(int)charityInventory.size()) {
            cout << "Invalid item number.\n";
            pressEnterToContinue();
            return;
        }
    }
    catch (...) {
        cout << "Invalid input! Please enter a valid number.\n";
        pressEnterToContinue();
        return;
    }

    string itemName = charityInventory[idx - 1].name;

    cout << "Are you sure you want to delete '" << itemName << "'? (y/n): ";
    string confirm;
    getline(cin, confirm);

    if (confirm == "y" || confirm == "Y") {
        charityInventory.erase(charityInventory.begin() + idx - 1);
        if (saveCharityInventory()) {
            cout << "Item '" << itemName << "' deleted successfully!\n";
        }
        else {
            cout << "Error saving changes!\n";
        }
    }
    else {
        cout << "Deletion cancelled.\n";
    }

    pressEnterToContinue();
}

void submitFeedback(const string& username, const string& feedbackType) {
    auto& bookings = getBookings();

    if (bookings.empty()) {
        cout << "No events found!\n";
        pressEnterToContinue();
        return;
    }

    cout << "\n--- Available Events ---\n";
    for (int i = 0; i < (int)bookings.size(); i++) {
        cout << i + 1 << ". " << bookings[i].bookingId << " - " << bookings[i].name
            << " (" << bookings[i].date << ")\n";
    }

    int opt;
    while (true) {
        cout << "\nSelect event number to submit " << feedbackType << " (0 to cancel): ";
        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        if (opt == 0) {
            cout << "Cancelled.\n";
            pressEnterToContinue();
            return;
        }
        if (opt >= 1 && opt <= (int)bookings.size()) {
            break;
        }
        cout << "Invalid choice! Please select a valid event.\n";
    }
    cin.ignore();

    string bookingId = bookings[opt - 1].bookingId;
    string feedbackContent;
    while (true) {
        cout << "Enter your " << feedbackType << " for event " << bookingId << ": ";
        getline(cin, feedbackContent);
        if (!feedbackContent.empty()) break;
        cout << feedbackType << " cannot be empty! Please try again.\n";
    }

    ofstream out("feedback.txt", ios::app);
    out << "Type: " << feedbackType << "\n";
    out << "User: " << username << "\n";
    out << "BookingID: " << bookingId << "\n";
    out << "Content: " << feedbackContent << "\n";
    out << "---\n";
    out.close();

    cout << feedbackType << " submitted successfully for event: " << bookingId << endl;
    pressEnterToContinue();
}

void deleteFeedback() {
    auto& bookings = getBookings();

    cout << "\n=== Available Events ===\n";
    for (int i = 0; i < (int)bookings.size(); i++) {
        cout << i + 1 << ". " << bookings[i].name << " (" << bookings[i].bookingId << ")\n";
    }

    cout << "Select event (0 to cancel): ";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice <= 0 || choice > (int)bookings.size()) {
        cout << "Cancelled.\n";
        return;
    }

    string bookingId = bookings[choice - 1].bookingId;

    ifstream file("feedback.txt");
    vector<string> feedbackLines;
    vector<int> feedbackIndices;
    string line;
    int index = 0;
    bool isTargetFeedback = false;
    int feedbackCount = 0;

    while (getline(file, line)) {
        feedbackLines.push_back(line);

        if (line.find("BookingID: " + bookingId) == 0) {
            isTargetFeedback = true;
        }

        if (line == "---") {
            if (isTargetFeedback) {
                feedbackIndices.push_back(index - 3);
                feedbackCount++;
            }
            isTargetFeedback = false;
        }
        index++;
    }
    file.close();

    cout << "\n=== Feedback for Selected Event ===\n";
    for (int i = 0; i < feedbackIndices.size(); i++) {
        int start = feedbackIndices[i];
        cout << i + 1 << ". " << feedbackLines[start] << "\n";
        cout << "   " << feedbackLines[start + 1] << "\n";
        cout << "   " << feedbackLines[start + 2] << "\n";
        cout << "--------------------------------\n";
    }

    cout << "Select feedback to delete (0 to cancel): ";
    int fbChoice;
    cin >> fbChoice;
    cin.ignore();

    if (fbChoice > 0 && fbChoice <= feedbackIndices.size()) {
        int startIndex = feedbackIndices[fbChoice - 1];
        feedbackLines.erase(feedbackLines.begin() + startIndex, feedbackLines.begin() + startIndex + 4);

        ofstream out("feedback.txt");
        for (const auto& l : feedbackLines) {
            out << l << "\n";
        }
        out.close();
        cout << "Feedback deleted successfully!\n";
    }
    else {
        cout << "Cancelled.\n";
    }
    pressEnterToContinue();
}

void viewFeedbackByOrganiser(const string& organiserName) {
    auto& bookings = getBookings();
    if (bookings.empty()) {
        cout << "No events found!\n";
        pressEnterToContinue();
        return;
    }

    cout << "\n=== Your Events ===\n";
    vector<Booking> organiserBookings;
    int count = 0;

    for (int i = 0; i < (int)bookings.size(); i++) {
        cout << i + 1 << ". " << bookings[i].name
            << " (" << bookings[i].bookingId << ")"
            << " - " << bookings[i].state << "\n";
    }

    cout << "\nSelect event to view feedback (0 to cancel): ";
    int eventChoice;
    if (!(cin >> eventChoice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        pressEnterToContinue();
        return;
    }
    cin.ignore();

    if (eventChoice <= 0 || eventChoice > (int)bookings.size()) {
        cout << "Cancelled or invalid choice.\n";
        pressEnterToContinue();
        return;
    }

    string selectedBookingId = bookings[eventChoice - 1].bookingId;
    string selectedEventName = bookings[eventChoice - 1].name;

    ifstream file("feedback.txt");
    if (!file) {
        cout << "No feedback records found!\n";
        pressEnterToContinue();
        return;
    }

    vector<Feedback> eventFeedback;
    string line, currentType, currentUser, currentBookingId, currentContent;

    while (getline(file, line)) {
        if (line == "---") {
            if (!currentBookingId.empty() && currentBookingId == selectedBookingId) {
                eventFeedback.push_back({
                    currentType, currentUser, currentBookingId, currentContent
                    });
            }
            currentType.clear();
            currentUser.clear();
            currentBookingId.clear();
            currentContent.clear();
        }
        else if (line.find("Type: ") == 0) {
            currentType = line.substr(6);
        }
        else if (line.find("User: ") == 0) {
            currentUser = line.substr(6);
        }
        else if (line.find("BookingID: ") == 0) {
            currentBookingId = line.substr(11);
        }
        else if (line.find("Content: ") == 0) {
            currentContent = line.substr(9);
        }
    }
    file.close();

    if (eventFeedback.empty()) {
        cout << "\nNo feedback found for event: " << selectedEventName << "\n";
        pressEnterToContinue();
        return;
    }

    int complaints = 0, issues = 0, suggestions = 0;
    for (const auto& fb : eventFeedback) {
        if (fb.type == "Complaint") complaints++;
        else if (fb.type == "Issue") issues++;
    }

    clearScreen();
    cout << "\n+===================================================+\n";
    cout << "|              FEEDBACK FOR EVENT                 |\n";
    cout << "+===================================================+\n";
    cout << "| Event: " << setw(40) << left
        << (selectedEventName.length() > 38 ? selectedEventName.substr(0, 38) + ".." : selectedEventName) << " |\n";
    cout << "| Booking ID: " << setw(35) << left << selectedBookingId << " |\n";
    cout << "+---------------------------------------------------+\n";
    cout << "| Feedback Statistics:                             |\n";
    cout << "|   Complaints: " << setw(32) << left << complaints << " |\n";
    cout << "|   Issues: " << setw(37) << left << issues << " |\n";
    cout << "|   Total: " << setw(38) << left << eventFeedback.size() << " |\n";
    cout << "+===================================================+\n\n";

    cout << "=== Detailed Feedback ===\n";
    for (int i = 0; i < (int)eventFeedback.size(); i++) {
        const auto& feedback = eventFeedback[i];
        cout << "\n[" << i + 1 << "] " << feedback.type << " from " << feedback.username << "\n";
        cout << "Content: " << feedback.content << "\n";
        cout << string(50, '-') << "\n";
    }

    cout << "\n=== Filter Options ===\n";
    cout << "1. View Complaints only\n";
    cout << "2. View Issues only\n";
    cout << "3. View All feedback\n";
    cout << "0. Return to menu\n";
    cout << "Choice: ";

    int filterChoice;
    if (!(cin >> filterChoice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Returning to menu...\n";
        pressEnterToContinue();
        return;
    }
    cin.ignore();

    if (filterChoice == 1 || filterChoice == 2) {
        string filterType = (filterChoice == 1) ? "Complaint" : "Issue";

        clearScreen();
        cout << "\n=== " << filterType << "s for Event: " << selectedEventName << " ===\n";

        int count = 0;
        for (const auto& feedback : eventFeedback) {
            if (feedback.type == filterType) {
                count++;
                cout << "\n[" << count << "] From: " << feedback.username << "\n";
                cout << "Content: " << feedback.content << "\n";
                cout << string(50, '-') << "\n";
            }
        }

        if (count == 0) {
            cout << "No " << filterType << "s found for this event.\n";
        }
        else {
            cout << "\nTotal " << filterType << "s: " << count << "\n";
        }
    }

    pressEnterToContinue();
}


void viewFeedback(const string& username) {
    ifstream file("feedback.txt");
    if (!file) {
        cout << "No feedback records found!\n";
        pressEnterToContinue();
        return;
    }

    vector<Feedback> myFeedback;
    auto& bookings = getBookings();

    string line, currentType, currentUser, currentBookingId, currentContent;
    Feedback currentFeedback;

    while (getline(file, line)) {
        if (line == "---") {
            if (currentUser == username) {
                myFeedback.push_back({
                    currentType, currentUser, currentBookingId, currentContent
                    });
            }
            currentType.clear();
            currentUser.clear();
            currentBookingId.clear();
            currentContent.clear();
        }
        else if (line.find("Type: ") == 0) {
            currentType = line.substr(6);
        }
        else if (line.find("User: ") == 0) {
            currentUser = line.substr(6);
        }
        else if (line.find("BookingID: ") == 0) {
            currentBookingId = line.substr(11);
        }
        else if (line.find("Content: ") == 0) {
            currentContent = line.substr(9);
        }
    }
    file.close();

    if (myFeedback.empty()) {
        cout << "You have not submitted any feedback yet.\n";
        pressEnterToContinue();
        return;
    }

    cout << "\n=== My Feedback ===\n";
    for (const auto& feedback : myFeedback) {
        cout << "Booking ID: " << feedback.bookingId;

        string bookingName = "Unknown Event";
        for (const auto& booking : bookings) {
            if (booking.bookingId == feedback.bookingId) {
                bookingName = booking.name;
                break;
            }
        }
        cout << " (" << bookingName << ")\n";

        cout << "Type: " << feedback.type << "\n";
        cout << "Content: " << feedback.content << "\n";
        cout << "--------------------------------\n";
    }
    pressEnterToContinue();
}

void linkCharityItemToBooking() {
    auto& charityInventory = getCharityInventory();
    auto& bookings = getBookings();

    if (charityInventory.empty()) {
        cout << "No charity items available. Please create items first.\n";
        pressEnterToContinue();
        return;
    }

    if (bookings.empty()) {
        cout << "No bookings available. Please create bookings first.\n";
        pressEnterToContinue();
        return;
    }

    cout << "\n=== Available Bookings ===\n";
    for (int i = 0; i < (int)bookings.size(); i++) {
        cout << i + 1 << ". " << bookings[i].bookingId << " - " << bookings[i].name
            << " (" << bookings[i].date << ")\n";
    }

    cout << "Select booking to add charity items (0 to cancel): ";
    int bookingChoice;
    cin >> bookingChoice;
    cin.ignore();

    if (bookingChoice <= 0 || bookingChoice > (int)bookings.size()) {
        cout << "Cancelled or invalid choice.\n";
        pressEnterToContinue();
        return;
    }

    string selectedBookingId = bookings[bookingChoice - 1].bookingId;
    string bookingName = bookings[bookingChoice - 1].name;

    cout << "\n=== Available Charity Items ===\n";
    viewItems();

    cout << "Select item number to add to booking '" << bookingName << "' (0 to cancel): ";
    int itemChoice;
    cin >> itemChoice;
    cin.ignore();

    if (itemChoice <= 0 || itemChoice > (int)charityInventory.size()) {
        cout << "Cancelled or invalid choice.\n";
        pressEnterToContinue();
        return;
    }

    CharityItem& selectedItem = charityInventory[itemChoice - 1];

    int quantityToAdd;
    while (true) {
        cout << "Enter quantity to add (available: " << selectedItem.quantity << "): ";
        if (cin >> quantityToAdd) {
            if (quantityToAdd > 0 && quantityToAdd <= selectedItem.quantity) {
                break;
            }
            else {
                cout << "Invalid quantity! Must be between 1 and " << selectedItem.quantity << ".\n";
            }
        }
        else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    cin.ignore();

    selectedItem.quantity -= quantityToAdd;

    if (selectedItem.quantity == 0) {
        selectedItem.status = "Out Stock";
    }
    else if (selectedItem.quantity < 10) {
        selectedItem.status = "Low Stock";
    }
    else {
        selectedItem.status = "Available";
    }

    ofstream out("booking_charity_items.txt", ios::app);
    out << "BookingID: " << selectedBookingId << "\n";
    out << "ItemName: " << selectedItem.name << "\n";
    out << "ItemType: " << selectedItem.type << "\n";
    out << "Quantity: " << quantityToAdd << "\n";
    out << "Value: " << selectedItem.value << "\n";
    out << "Status: " << selectedItem.status << "\n";
    out << "---\n";
    out.close();

    saveCharityInventory();

    cout << "\nSuccessfully added " << quantityToAdd << " x " << selectedItem.name
        << " to booking '" << bookingName << "'\n";
    pressEnterToContinue();
}

void viewBookingCharityItems() {
    auto& bookings = getBookings();

    if (bookings.empty()) {
        cout << "No bookings available.\n";
        pressEnterToContinue();
        return;
    }

    cout << "\n=== Available Bookings ===\n";
    for (int i = 0; i < (int)bookings.size(); i++) {
        cout << i + 1 << ". " << bookings[i].bookingId << " - " << bookings[i].name
            << " (" << bookings[i].date << ")\n";
    }

    cout << "Select booking to view charity items (0 to cancel): ";
    int bookingChoice;
    cin >> bookingChoice;
    cin.ignore();

    if (bookingChoice <= 0 || bookingChoice > (int)bookings.size()) {
        cout << "Cancelled or invalid choice.\n";
        pressEnterToContinue();
        return;
    }

    string selectedBookingId = bookings[bookingChoice - 1].bookingId;
    string bookingName = bookings[bookingChoice - 1].name;

    ifstream file("booking_charity_items.txt");
    if (!file) {
        cout << "No charity items found for this booking.\n";
        pressEnterToContinue();
        return;
    }

    vector<vector<string>> bookingItems;
    vector<string> currentItem;
    string line;
    double totalValue = 0;
    int totalQuantity = 0;

    while (getline(file, line)) {
        if (line == "---") {
            if (!currentItem.empty() && currentItem[0] == selectedBookingId) {
                bookingItems.push_back(currentItem);
                totalQuantity += stoi(currentItem[3]);
                totalValue += stod(currentItem[4]) * stoi(currentItem[3]);
            }
            currentItem.clear();
        }
        else if (line.find("BookingID: ") == 0) {
            currentItem.push_back(line.substr(11));
        }
        else if (line.find("ItemName: ") == 0) {
            currentItem.push_back(line.substr(10));
        }
        else if (line.find("ItemType: ") == 0) {
            currentItem.push_back(line.substr(10));
        }
        else if (line.find("Quantity: ") == 0) {
            currentItem.push_back(line.substr(10));
        }
        else if (line.find("Value: ") == 0) {
            currentItem.push_back(line.substr(7));
        }
        else if (line.find("Status: ") == 0) {
            currentItem.push_back(line.substr(8));
        }
    }
    file.close();

    if (bookingItems.empty()) {
        cout << "No charity items found for booking '" << bookingName << "'\n";
    }
    else {
        cout << "\n=== Charity Items for Booking: " << bookingName << " ===\n";
        cout << left << setw(5) << "No" << setw(25) << "Item Name" << setw(15) << "Type"
            << setw(10) << "Qty" << setw(12) << "Value" << setw(15) << "Status" << "\n";
        cout << string(82, '-') << "\n";

        for (int i = 0; i < (int)bookingItems.size(); i++) {
            const auto& item = bookingItems[i];
            cout << setw(5) << i + 1
                << setw(25) << (item[1].length() > 24 ? item[1].substr(0, 24) + "." : item[1])
                << setw(15) << item[2]
                << setw(10) << item[3]
                << setw(12) << fixed << setprecision(2) << stod(item[4])
                << setw(15) << item[5] << "\n";
        }

        cout << string(82, '-') << "\n";
        cout << "Total Items: " << bookingItems.size() << " | ";
        cout << "Total Quantity: " << totalQuantity << " | ";
        cout << "Total Value: RM " << fixed << setprecision(2) << totalValue << "\n";
    }
    pressEnterToContinue();
}

void processPayment(const string& bookingId) {
    auto& bookings = getBookings();

    Booking* bookingPtr = nullptr;
    for (auto& booking : bookings) {
        if (booking.bookingId == bookingId) {
            bookingPtr = &booking;
            break;
        }
    }

    if (!bookingPtr) {
        cout << "Booking not found!\n";
        pressEnterToContinue();
        return;
    }

    Booking& booking = *bookingPtr;

    cout << "\n=== Process Payment for Booking ===\n";
    cout << "Booking ID: " << booking.bookingId << "\n";
    cout << "Event: " << booking.name << "\n";
    cout << "Type: " << booking.choice << "\n";
    cout << "Date: " << booking.date << "\n";

    double basePrice = 0.0;
    if (booking.choice == "Food Truck") basePrice = 500.0;
    else if (booking.choice == "Charity Run") basePrice = 300.0;
    else if (booking.choice == "Charity Concert") basePrice = 1000.0;

    cout << "Base Price: RM " << fixed << setprecision(2) << basePrice << "\n";

    double donation = 0.0;
    cout << "Enter donation amount: RM ";
    cin >> donation;
    cin.ignore();

    if (donation < 0) {
        cout << "Donation cannot be negative!\n";
        pressEnterToContinue();
        return;
    }

    double totalAmount = basePrice + donation;

    cout << "Total Amount: RM " << fixed << setprecision(2) << totalAmount << "\n";

    string bankAccount;
    cout << "Enter bank account number (8 digits): ";
    getline(cin, bankAccount);

    if (!regex_match(bankAccount, regex("^[0-9]{8}$"))) {
        cout << "Invalid bank account number!\n";
        pressEnterToContinue();
        return;
    }

    Payment newPayment;
    newPayment.paymentId = generatePaymentId();
    newPayment.bookingId = bookingId;
    newPayment.bookingName = booking.name;
    newPayment.eventType = booking.choice;
    newPayment.date = booking.date;
    newPayment.basePrice = basePrice;
    newPayment.donation = donation;
    newPayment.totalAmount = totalAmount;
    newPayment.bankAccount = bankAccount;
    newPayment.paymentStatus = "Completed";

    auto& payments = getPayments();
    payments.push_back(newPayment);
    savePayments();

    booking.paid = "Yes";
    saveBookings();

    cout << "\nPayment processed successfully!\n";
    cout << "Payment ID: " << newPayment.paymentId << "\n";
    pressEnterToContinue();
}

void paymentHistory() {
    auto& payments = getPayments();

    if (payments.empty()) {
        cout << "No payment records found!\n";
        pressEnterToContinue();
        return;
    }

    cout << "\n=== Payment History ===\n";
    cout << left << setw(12) << "Payment ID"
        << setw(12) << "Booking ID"
        << setw(20) << "Event Name"
        << setw(15) << "Total Amount"
        << setw(12) << "Status" << "\n";
    cout << string(71, '-') << "\n";

    for (const auto& payment : payments) {
        cout << setw(12) << payment.paymentId
            << setw(12) << payment.bookingId
            << setw(20) << (payment.bookingName.length() > 19 ? payment.bookingName.substr(0, 19) + "." : payment.bookingName)
            << setw(15) << fixed << setprecision(2) << payment.totalAmount
            << setw(12) << payment.paymentStatus << "\n";
    }

    pressEnterToContinue();
}

void viewPayments() {
    ifstream file("booking_payments.txt");
    if (!file) {
        cout << "No payment records found.\n";
        return;
    }

    string line;
    cout << "\n--- Payments ---\n";
    while (getline(file, line)) {
        if (line == "---") {
            cout << "-------------------------\n";
        }
        else {
            cout << line << "\n";
        }
    }
    file.close();
}

void generateEventReport() {
    auto& bookings = getBookings();
    auto& payments = getPayments();
    auto& charityInventory = getCharityInventory();

    vector<Booking> eligibleBookings;
    for (const auto& booking : bookings) {
        if (booking.state == "Completed") {
            bool hasSuccessfulPayment = false;
            for (const auto& payment : payments) {
                if (payment.bookingId == booking.bookingId && payment.paymentStatus == "Completed") {
                    hasSuccessfulPayment = true;
                    break;
                }
            }
            if (hasSuccessfulPayment) {
                eligibleBookings.push_back(booking);
            }
        }
    }

    if (eligibleBookings.empty()) {
        cout << "\n+==================================================+\n";
        cout << "|           NO EVENTS AVAILABLE            |\n";
        cout << "|  No completed events with successful     |\n";
        cout << "|      payment available for reporting     |\n";
        cout << "+==================================================+\n";
        pressEnterToContinue();
        return;
    }

    cout << "\n+==============================================================+\n";
    cout << "|              EVENTS AVAILABLE FOR REPORTING          |\n";
    cout << "+==============================================================+\n";
    for (int i = 0; i < (int)eligibleBookings.size(); i++) {
        cout << "| " << setw(2) << i + 1 << ". " << setw(40) << left
            << (eligibleBookings[i].name.length() > 37 ?
                eligibleBookings[i].name.substr(0, 37) + "..." : eligibleBookings[i].name)
            << " (" << setw(10) << left << eligibleBookings[i].choice.substr(0, 10) << ") |\n";
    }
    cout << "+==============================================================+\n";

    int choice;
    cout << "\nSelect event to generate report (0 to cancel): ";
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        pressEnterToContinue();
        return;
    }
    cin.ignore();

    if (choice <= 0 || choice > (int)eligibleBookings.size()) {
        cout << "Cancelled or invalid choice.\n";
        pressEnterToContinue();
        return;
    }

    Booking& selectedBooking = eligibleBookings[choice - 1];

    vector<Feedback> issues, complaints;
    ifstream feedbackFile("feedback.txt");
    if (feedbackFile) {
        string line, currentType, currentUser, currentBookingId, currentContent;

        while (getline(feedbackFile, line)) {
            if (line == "---") {
                if (currentBookingId == selectedBooking.bookingId) {
                    if (currentType == "Issue") {
                        issues.push_back({ currentType, currentUser, currentBookingId, currentContent });
                    }
                    else if (currentType == "Complaint") {
                        complaints.push_back({ currentType, currentUser, currentBookingId, currentContent });
                    }
                }
                currentType.clear();
                currentUser.clear();
                currentBookingId.clear();
                currentContent.clear();
            }
            else if (line.find("Type: ") == 0) {
                currentType = line.substr(6);
            }
            else if (line.find("User: ") == 0) {
                currentUser = line.substr(6);
            }
            else if (line.find("BookingID: ") == 0) {
                currentBookingId = line.substr(11);
            }
            else if (line.find("Content: ") == 0) {
                currentContent = line.substr(9);
            }
        }
        feedbackFile.close();
    }

    vector<vector<string>> charityItems;
    ifstream charityFile("booking_charity_items.txt");
    if (charityFile) {
        vector<string> currentItem;
        string line;

        while (getline(charityFile, line)) {
            if (line == "---") {
                if (!currentItem.empty() && currentItem[0] == selectedBooking.bookingId) {
                    charityItems.push_back(currentItem);
                }
                currentItem.clear();
            }
            else if (line.find("BookingID: ") == 0) {
                currentItem.push_back(line.substr(11));
            }
            else if (line.find("ItemName: ") == 0) {
                currentItem.push_back(line.substr(10));
            }
            else if (line.find("ItemType: ") == 0) {
                currentItem.push_back(line.substr(10));
            }
            else if (line.find("Quantity: ") == 0) {
                currentItem.push_back(line.substr(10));
            }
            else if (line.find("Value: ") == 0) {
                currentItem.push_back(line.substr(7));
            }
            else if (line.find("Status: ") == 0) {
                currentItem.push_back(line.substr(8));
            }
        }
        charityFile.close();
    }

    clearScreen();

    const int BOX_WIDTH = 80;

    cout << "+" << string(BOX_WIDTH - 2, '=') << "+\n";
    cout << "|" << setw((BOX_WIDTH - 34) / 2) << "" << "EVENT COMPLETION REPORT"
        << setw((BOX_WIDTH - 34) / 2 + (BOX_WIDTH - 34) % 2) << "" << "|\n";
    cout << "+" << string(BOX_WIDTH - 2, '=') << "+\n";

    cout << "|" << string(BOX_WIDTH - 2, ' ') << "|\n";
    cout << "|" << setw((BOX_WIDTH - 32) / 2) << "" << "EVENT BASIC INFORMATION"
        << setw((BOX_WIDTH - 32) / 2 + (BOX_WIDTH - 32) % 2) << "" << "|\n";
    cout << "|" << string(BOX_WIDTH - 2, ' ') << "|\n";
    cout << "+" << string(BOX_WIDTH - 2, '-') << "+\n";

    cout << "| " << setw(20) << left << "Booking ID:" << setw(BOX_WIDTH - 23) << selectedBooking.bookingId << "|\n";
    cout << "| " << setw(20) << "Event Name:" << setw(BOX_WIDTH - 23)
        << (selectedBooking.name.length() > BOX_WIDTH - 23 ? selectedBooking.name.substr(0, BOX_WIDTH - 26) + "..." : selectedBooking.name) << "|\n";
    cout << "| " << setw(20) << "Event Type:" << setw(BOX_WIDTH - 23) << selectedBooking.choice << "|\n";
    cout << "| " << setw(20) << "Date:" << setw(BOX_WIDTH - 23) << selectedBooking.date << "|\n";
    cout << "| " << setw(20) << "Venue:" << setw(BOX_WIDTH - 23)
        << (selectedBooking.venue.length() > BOX_WIDTH - 23 ? selectedBooking.venue.substr(0, BOX_WIDTH - 26) + "..." : selectedBooking.venue) << "|\n";
    cout << "| " << setw(20) << "Status:" << setw(BOX_WIDTH - 23) << selectedBooking.state << "|\n";
    cout << "| " << setw(20) << "Payment Status:" << setw(BOX_WIDTH - 23) << selectedBooking.paid << "|\n";
    cout << "|" << string(BOX_WIDTH - 2, ' ') << "|\n";

    cout << "+" << string(BOX_WIDTH - 2, '-') << "+\n";
    cout << "|" << setw((BOX_WIDTH - 26) / 2) << "" << "CHARITY ITEMS DETAILS"
        << setw((BOX_WIDTH - 26) / 2 + (BOX_WIDTH - 26) % 2) << "" << "|\n";
    cout << "+" << string(BOX_WIDTH - 2, '-') << "+\n";

    if (charityItems.empty()) {
        cout << "| " << setw(BOX_WIDTH - 3) << "No charity items linked to this event." << "|\n";
    }
    else {
        double totalValue = 0;
        int totalQuantity = 0;

        cout << "| " << setw(3) << "No" << setw(22) << "Item Name"
            << setw(15) << "Type" << setw(8) << "Qty"
            << setw(10) << "Value" << setw(12) << "Status" << " |\n";
        cout << "+" << string(BOX_WIDTH - 2, '-') << "+\n";

        for (int i = 0; i < (int)charityItems.size(); i++) {
            const auto& item = charityItems[i];
            double itemValue = stod(item[4]) * stoi(item[3]);
            totalValue += itemValue;
            totalQuantity += stoi(item[3]);

            string itemName = item[1];
            if (itemName.length() > 20) {
                itemName = itemName.substr(0, 20) + "...";
            }

            cout << "| " << setw(3) << i + 1
                << setw(22) << itemName
                << setw(15) << (item[2].length() > 13 ? item[2].substr(0, 13) + "..." : item[2])
                << setw(8) << item[3]
                << "RM" << setw(8) << fixed << setprecision(2) << stod(item[4])
                << setw(12) << (item[5].length() > 10 ? item[5].substr(0, 10) + "..." : item[5]) << " |\n";
        }

        cout << "+" << string(BOX_WIDTH - 2, '-') << "+\n";
        stringstream totalLine;
        totalLine << "TOTAL: " << charityItems.size() << " items, "
            << totalQuantity << " units, RM " << fixed << setprecision(2) << totalValue;
        cout << "| " << setw(BOX_WIDTH - 3) << left << totalLine.str() << "|\n";
    }
    cout << "|" << string(BOX_WIDTH - 2, ' ') << "|\n";

    cout << "+" << string(BOX_WIDTH - 2, '-') << "+\n";
    cout << "|" << setw((BOX_WIDTH - 18) / 2) << "" << "ISSUES REPORTED"
        << setw((BOX_WIDTH - 18) / 2 + (BOX_WIDTH - 18) % 2) << "" << "|\n";
    cout << "+" << string(BOX_WIDTH - 2, '-') << "+\n";

    if (issues.empty()) {
        cout << "| " << setw(BOX_WIDTH - 3) << "No issues reported for this event." << "|\n";
    }
    else {
        cout << "| " << setw(BOX_WIDTH - 3) << "Total Issues: " + to_string(issues.size()) << "|\n";
        cout << "+" << string(BOX_WIDTH - 2, '-') << "+\n";

        for (int i = 0; i < (int)issues.size(); i++) {
            stringstream issueLine;
            issueLine << i + 1 << ". [" << issues[i].username << "]: ";

            string content = issues[i].content;
            int contentWidth = BOX_WIDTH - issueLine.str().length() - 3;
            if (content.length() > contentWidth) {
                content = content.substr(0, contentWidth) + "...";
            }
            issueLine << content;

            cout << "| " << setw(BOX_WIDTH - 3) << left << issueLine.str() << "|\n";
        }
    }
    cout << "|" << string(BOX_WIDTH - 2, ' ') << "|\n";

    cout << "+" << string(BOX_WIDTH - 2, '-') << "+\n";
    cout << "|" << setw((BOX_WIDTH - 21) / 2) << "" << "COMPLAINTS RECEIVED"
        << setw((BOX_WIDTH - 21) / 2 + (BOX_WIDTH - 21) % 2) << "" << "|\n";
    cout << "+" << string(BOX_WIDTH - 2, '-') << "+\n";

    if (complaints.empty()) {
        cout << "| " << setw(BOX_WIDTH - 3) << "No complaints received for this event." << "|\n";
    }
    else {
        cout << "| " << setw(BOX_WIDTH - 3) << "Total Complaints: " + to_string(complaints.size()) << "|\n";
        cout << "+" << string(BOX_WIDTH - 2, '-') << "+\n";

        for (int i = 0; i < (int)complaints.size(); i++) {
            stringstream complaintLine;
            complaintLine << i + 1 << ". [" << complaints[i].username << "]: ";

            string content = complaints[i].content;
            int contentWidth = BOX_WIDTH - complaintLine.str().length() - 3;
            if (content.length() > contentWidth) {
                content = content.substr(0, contentWidth) + "...";
            }
            complaintLine << content;

            cout << "| " << setw(BOX_WIDTH - 3) << left << complaintLine.str() << "|\n";
        }
    }

    cout << "|" << string(BOX_WIDTH - 2, ' ') << "|\n";
    cout << "+" << string(BOX_WIDTH - 2, '-') << "+\n";

    string timestamp = "REPORT GENERATED ON: " + string(__DATE__) + " " + string(__TIME__);
    cout << "| " << setw(BOX_WIDTH - 3) << timestamp << "|\n";
    cout << "+" << string(BOX_WIDTH - 2, '=') << "+\n";

    pressEnterToContinue();
}

void marketingMenu() {
    Promotion promotions[100];
    int count = 0;
    loadPromotions(promotions, count);
    int choice;
    do {
        cout << "\n=== Marketing Module ===\n"
            << "1. Add\n2. View\n3. Edit\n4. Delete\n5. Search Promotion\n6. Exit\nChoice: ";
        cin >> choice;
        cin.ignore(1000, '\n');
        switch (choice) {
        case 1: addPromotion(promotions, count); break;
        case 2: viewPromotions(promotions, count); break;
        case 3: editPromotion(promotions, count); break;
        case 4: deletePromotion(promotions, count); break;
        case 5: searchPromotion(promotions, count); break;
        case 6:
            savePromotions(promotions, count);
            cout << "Exiting Marketing...\n";
            break;
        default: cout << "Invalid choice.\n"; break;
        }
    } while (choice != 6);
}

void charityItemMenu() {
    int choice;
    do {
        clearScreen();
        cout << "\n=== Charity Item Management ===\n";
        cout << "1. Add New Item\n";
        cout << "2. View All Items\n";
        cout << "3. Update Item\n";
        cout << "4. Delete Item\n";
        cout << "5. Link Item to Booking\n";
        cout << "6. View Booking Items\n";
        cout << "0. Return to Main Menu\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pressEnterToContinue();
            continue;
        }
        cin.ignore();

        switch (choice) {
        case 1: addItem(); break;
        case 2: viewItems(); break;
        case 3: updateItem(); break;
        case 4: deleteItem(); break;
        case 5: linkCharityItemToBooking(); break;
        case 6: viewBookingCharityItems(); break;
        case 0:
            cout << "Returning to organizer menu...\n";
            break;
        default:
            cout << "Invalid choice! Please enter 0-6.\n";
            pressEnterToContinue();
        }
    } while (choice != 0);
}

void bookingMenu() {
    int opt;
    do {
        clearScreen();
        cout << "\n----- Booking Operation -----\n";
        cout << "1. Create Booking\n";
        cout << "2. View Booking\n";
        cout << "3. Update Booking\n";
        cout << "4. Delete Booking\n";
        cout << "5. Process Payment\n";
        cout << "6. View Payment History\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number (0-4).\n";
            continue;
        }
        cin.ignore();

        switch (opt) {
        case 1: createBooking(); break;
        case 2: viewBooking(); break;
        case 3: updateBooking(); break;
        case 4: deleteBooking(); break;
        case 5:
        {
            auto& bookings = getBookings();
            if (bookings.empty()) {
                cout << "No bookings found!\n";
                pressEnterToContinue();
                break;
            }
            cout << "\n--- Select Booking for Payment ---\n";
            for (int i = 0; i < (int)bookings.size(); i++) {
                cout << i + 1 << ". " << bookings[i].bookingId << " - " << bookings[i].name << "\n";
            }
            cout << "Select booking number: ";
            int choice;
            cin >> choice;
            cin.ignore();
            if (choice > 0 && choice <= (int)bookings.size()) {
                processPayment(bookings[choice - 1].bookingId);
            }
            else {
                cout << "Invalid selection!\n";
                pressEnterToContinue();
            }
        }
        break;
        case 6: paymentHistory(); break;
        case 0: cout << "Exiting booking menu...\n"; break;
        default:
            cout << "Invalid choice! Please enter 0-6.\n";
            pressEnterToContinue();
        }
    } while (opt != 0);
}

void accountMenu(const string& username) {
    int opt;
    do {
        clearScreen();
        cout << "\n----- Account Management -----\n";
        cout << "1. Update profile\n";
        cout << "2. Delete profile\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        if (!(cin >> opt)) {
            cout << "Invalid input! Please enter a number (0-).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opt) {
        case 1: updateProfile(username); break;
        case 2: deleteProfile(username); break;
        case 0: cout << "Exiting account management\n"; break;
        default:
            cout << "Invalid choice! Please enter 0-3.\n";
            pressEnterToContinue();
        }
    } while (opt != 0);
}

void viewAllUsers() {
    ifstream file("users.txt");
    if (!file) {
        cout << "No users found.\n";
        return;
    }

    cout << "\n--- Registered Users ---\n";

    string line;
    User currentUser;

    while (getline(file, line)) {
        if (line == "---") {
            if (!currentUser.username.empty()) {
                cout << "Username: " << currentUser.username
                    << " | Role: " << currentUser.role << "\n";
            }
            currentUser = User();
        }
        else if (line.find("Username: ") == 0) {
            currentUser.username = line.substr(10);
        }
        else if (line.find("Password: ") == 0) {
            currentUser.password = line.substr(10);
        }
        else if (line.find("Role: ") == 0) {
            currentUser.role = line.substr(6);
        }
        else if (line.find("SecurityQuestion: ") == 0) {
            currentUser.securityQuestion = line.substr(18);
        }
        else if (line.find("SecurityAnswer: ") == 0) {
            currentUser.securityAnswer = line.substr(16);
        }
    }

    if (!currentUser.username.empty()) {
        cout << "Username: " << currentUser.username
            << " | Role: " << currentUser.role << "\n";
    }

    file.close();
    pressEnterToContinue();
    return;
}

void viewComplaints() {
    ifstream file("complaints.txt");
    if (!file) {
        cout << "No complaints found.\n";
        return;
    }

    string user, event, complaint, line;
    cout << "\n--- Complaints ---\n";
    while (getline(file, user)) {
        if (user.empty() || user == "---")
            continue;
        getline(file, event);
        getline(file, complaint);
        getline(file, line);
        cout << "User: " << user << " | Event: " << event << "\nComplaint: " << complaint << "\n\n";
    }
    file.close();
}

void viewIssues() {
    ifstream file("issues.txt");
    if (!file) {
        cout << "No issues found.\n";
        return;
    }

    string organiser, event, issue, line;
    cout << "\n--- Issues ---\n";
    while (getline(file, organiser)) {
        if (organiser.empty() || organiser == "---")
            continue;
        getline(file, event);
        getline(file, issue);
        getline(file, line);
        cout << "Organiser: " << organiser << " | Event: " << event << "\nIssue: " << issue << "\n\n";
    }
    file.close();
}

void adminMenu() {
    int choice;
    do {
        clearScreen();
        cout << "\n---- Admin Menu ----\n";
        cout << "1. View All Users\n";
        cout << "2. Update Users\n";
        cout << "3. Delete User\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            viewAllUsers();
            break;
        case 2:
            updateUser();
            break;
        case 3:
            deleteUser();
            break;
        case 0:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void organiser(const string& username) {
    int choice;
    do {
        clearScreen();
        cout << "\n----- Organiser Menu -----\n";
        cout << "1. Booking operation\n";
        cout << "2. Account management\n";
        cout << "3. Change state\n";
        cout << "4. Submit Issue\n";
        cout << "5. View All Feedback\n";
        cout << "6. Deleted Feedback\n";
        cout << "7. Promotional Marketing\n";
        cout << "8. Charity Item Management\n";
        cout << "9. Generate Event Report\n";
        cout << "0. Exit\n";

        cout << "Choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: bookingMenu(); break;
        case 2: accountMenu(username); break;
        case 3: changeEventState(); break;
        case 4: submitFeedback(username, "Issue"); break;
        case 5: viewFeedbackByOrganiser(username); break;
        case 6: deleteFeedback(); break;
        case 7: marketingMenu(); break;
        case 8: charityItemMenu(); break;
        case 9: generateEventReport(); break;
        case 0: cout << "Logging out...\n"; break;
        default:
            cout << "Invalid choice! Please enter 0-8.\n";
            pressEnterToContinue();
        }
    } while (choice != 0);
}


void participant(const string& username) {
    int choice;
    do {
        clearScreen();
        cout << "\n--- Participant Menu ---\n";
        cout << "1. View All Events\n";
        cout << "2. Join an Event\n";
        cout << "3. View My Events\n";
        cout << "4. Submit Complaint\n";
        cout << "5. View My Feedback\n";
        cout << "6. Account Management\n";
        cout << "0. Exit\n";

        while (true) {
            cout << "Choice: ";
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Please enter a number (0-6).\n";
                continue;
            }
            else if (choice < 0 || choice > 6) {
                cout << "Invalid choice! Please enter 0-6.\n";
                continue;
            }
            else {
                cin.ignore(1000, '\n');
                break;
            }
        }

        switch (choice) {
        case 1: viewBooking(); break;
        case 2: joinEvent(username); break;
        case 3: viewMyEvents(username); break;
        case 4: submitFeedback(username, "Complaint"); break;
        case 5: viewFeedback(username); break;
        case 6: accountMenu(username); break;
        case 0: cout << "Logging out...\n"; break;
        default: cout << "Invalid choice! Please enter 0-5.\n"; break;
        }
    } while (choice != 0);
}

int main() {
    int choice;
    do {
        cout << R"ascii(
        _____         __     __           _____        ___________        ____________    ________    ________   ______   _____  
   _____\    \_      /  \   /  \        /      |_      \          \      /            \  /        \  /        \ |\     \ |     | 
  /     /|     |    /   /| |\   \      /         \      \    /\    \    |\___/\  \\___/||\         \/         /|\ \     \|     | 
 /     / /____/|   /   //   \\   \    |     /\    \      |   \_\    |    \|____\  \___|/| \            /\____/ | \ \           | 
|     | |____|/   /    \_____/    \   |    |  |    \     |      ___/           |  |     |  \______/\   \     | |  \ \____      | 
|     |  _____   /    /\_____/\    \  |     \/      \    |      \  ____   __  /   / __   \ |      | \   \____|/    \|___/     /| 
|\     \|\    \ /    //\_____/\\    \ |\      /\     \  /     /\ \/    \ /  \/   /_/  |   \|______|  \   \             /     / | 
| \_____\|    |/____/ |       | \____\| \_____\ \_____\/_____/ |\______||____________/|            \  \___\           /_____/  / 
| |     /____/||    | |       | |    || |     | |     ||     | | |     ||           | /             \ |   |           |     | /  
 \|_____|    |||____|/         \|____| \|_____|\|_____||_____|/ \|_____||___________|/               \|___|           |_____|/   
        |____|/                                                                                                                  
)ascii";
        cout << "\n----- Main Menu -----\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";

        while (true) {
            cout << "Enter your choice: ";
            if (!(cin >> choice)) {
                cout << "Invalid input! Please enter a number (1-3).\n";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            if (choice < 1 || choice > 3) {
                cout << "Invalid choice. Please enter 1, 2 or 3.\n";
                continue;
            }
            cin.ignore();
            break;
        }

        switch (choice) {
        case 1:
            userRegister();
            break;
        case 2:
            userLogin();
            break;
        case 3:
            cout << "Thank You ! See You Next Time !\n";
            break;
        }
    } while (choice != 3);
    return 0;
}
