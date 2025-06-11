#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cstdlib>
using namespace std;

// ======================
// OOP CLASSES IMPLEMENTATION
// ======================

class BookingManager; // Forward declaration for friend class

class Passenger {
protected:
    bool isInfant;
    string seat;
    int ageCategory;
    string meal;
    string name;

public:
    Passenger() {}

    void inputPassenger(int travelClass, int index);
    void assignSeat(int baseRow, int offset);
    void printDetails(int labelWidth) const;

    int getAgeCategory() const { return ageCategory; }
    bool getIsInfant() const { return isInfant; }
    string getSeat() const { return seat; }
    string getMeal() const { return meal; }
    string getName() const { return name; }

    void setAgeCategory(int age) { ageCategory = age; isInfant = (age == 3); }
    void setSeat(string s) { seat = s; }
    void setMeal(string m) { meal = m; }
    void setName(string n) { name = n; }

    friend void showPassengerInfo(const Passenger& p);
    friend class BookingManager;
};

void showPassengerInfo(const Passenger& p) {
    cout << "Passenger Info -> Seat: " << p.getSeat() << ", Meal: " << p.getMeal() << endl;
}

class Flight {
protected:
    string airline;
    string flightNumber;
    string departureCity;
    string arrivalCity;
    string date;
    string time;
    int price;

public:
    Flight() {}
    Flight(string air, string num, string dep, string arr, string d, string t, int p)
        : airline(air), flightNumber(num), departureCity(dep), arrivalCity(arr), date(d), time(t), price(p) {
    }

    virtual void printDetails() const {
        cout << airline << " " << flightNumber << " | " << date << " " << time << " | " << price << " PKR";
    }

    string getDate() const { return date; }
    int getPrice() const { return price; }
    string getFlightNumber() const { return flightNumber; }
    string getAirline() const { return airline; }
    string getTime() const { return time; }
    string getDepartureCity() const { return departureCity; }
    string getArrivalCity() const { return arrivalCity; }
    void setPrice(int p) { price = p; }
    void applyMultiplier(double multiplier) { price = static_cast<int>(price * multiplier); }
};

class BusinessFlight : public Flight { // ? Inheritance
public:
    BusinessFlight(string air, string num, string dep, string arr, string d, string t, int p) : Flight(air, num, dep, arr, d, t, p) {
    }

    void printDetails() const {
        cout << "[Business Class] " << airline << " " << flightNumber
            << " | " << date << " " << time << " | " << price << " PKR";
    }
};

class BookingManager { // ? Friend Class of Passenger
public:
    static void assignPassengerSeat(Passenger& p, int baseRow, int offset) {
        string seats[] = { "A", "B", "C", "D", "E", "F", "G" };
        if (!p.getIsInfant()) {
            p.getSeat() = to_string(baseRow) + seats[offset % 7];
        }
        else {
            p.getSeat() = "No Seat";
        }
    }
};

string globalLoggedInName = "";
bool accountExists(string username, string password) {
    ifstream file("users.txt");
    string user, pass, fullNamePart, fullName;
    while (file >> user >> pass) {
        getline(file, fullNamePart);
        fullName = fullNamePart;
        if (user == username && pass == password) {
            globalLoggedInName = fullName.substr(1); // remove leading space
            return true;
        }
    }
    return false;
}

void clearScreen() {
    system("cls");
}

void printHeader(const string& title) {
    clearScreen();
    int totalWidth = max(30, (int)title.size() + 10);
    string border(totalWidth, '=');
    int space = (totalWidth - title.size()) / 2;
    cout << border << endl;
    cout << string(space, ' ') << title << endl;
    cout << border << endl << endl;
}

void availableDates(string dates[], int size) {
    string fixedDates[11] = {
        "12-06-2025", "13-06-2025", "14-06-2025", "15-06-2025", "16-06-2025",
        "17-06-2025", "18-06-2025", "19-06-2025", "20-06-2025", "21-06-2025", "22-06-2025"
    };
    for (int i = 0; i < size && i < 11; ++i) dates[i] = fixedDates[i];
}


void createAccount();
void bookingSystem();

void showAirports() {
    printHeader("Airport Selection");
    string airports[] = {
        "1. Islamabad", "2. Lahore", "3. Karachi", "4. Peshawar", "5. Quetta",
        "6. Multan", "7. Skardu", "8. Faisalabad", "9. Sialkot"
    };
    for (const string& airport : airports)
        cout << airport << endl;
    cout << endl;
}

vector<Flight> generateFlights(string dep, string arr) {
    vector<Flight> flights;
    string dates[11]; availableDates(dates, 11);
    string times[] = { "06:00 AM", "10:00 AM", "02:00 PM", "06:00 PM" };

    string airlines[] = { "PIA", "Airblue", "SereneAir", "Fly Jinnah" };
    string airlineCodes[] = { "PK", "AB", "SA", "FJ" };

    for (int a = 0; a < 4; ++a) {
        for (int i = 0; i < 11; ++i) {
            for (int j = 0; j < 4; ++j) {
                flights.push_back({
                    airlines[a],
                    airlineCodes[a] + to_string(100 + a * 100 + i * 4 + j),
                    dep,
                    arr,
                    dates[i],
                    times[j],
                    28000 + j * 2500 + a * 1000  // Slightly vary prices by airline
                    });
            }
        }
    }

    return flights;
}

void printTicket(int depIndex, int arrIndex, const Flight& outboundFlight, Passenger* passengers, int numTickets, int travelClass, int ticketType, const Flight* returnFlight, string bookingName, string bookingRef)
{
    string cities[] = { "", "Islamabad", "Lahore", "Karachi", "Peshawar", "Quetta", "Multan", "Skardu", "Faisalabad", "Sialkot" };
    printHeader("BOOKING SUMMARY - " + bookingName + " | Ref#: " + bookingRef);

    const int labelWidth = 20;
    const int valueWidth = 40;

    cout << left << setw(labelWidth) << "From" << ": " << cities[depIndex] << "\n";
    cout << left << setw(labelWidth) << "To" << ": " << cities[arrIndex] << "\n";
    cout << left << setw(labelWidth) << "Travel Class" << ": " << (travelClass == 1 ? "Economy" : "Business") << "\n";
    cout << left << setw(labelWidth) << "Ticket Type" << ": " << (ticketType == 1 ? "One-Way" : "Return") << "\n\n";

    cout << "----------- Outbound Flight Details -----------\n";
    cout << left << setw(labelWidth) << "Airline" << ": " << outboundFlight.getAirline() << "\n";
    cout << left << setw(labelWidth) << "Flight Number" << ": " << outboundFlight.getFlightNumber() << "\n";
    cout << left << setw(labelWidth) << "Date" << ": " << outboundFlight.getDate() << "\n";
    cout << left << setw(labelWidth) << "Time" << ": " << outboundFlight.getTime() << "\n";
    cout << left << setw(labelWidth) << "Price per Ticket" << ": " << outboundFlight.getPrice() << " PKR\n\n";

    if (ticketType == 2 && returnFlight != nullptr) {
        cout << "----------- Return Flight Details -------------\n";
        cout << left << setw(labelWidth) << "Airline" << ": " << returnFlight->getAirline() << "\n";
        cout << left << setw(labelWidth) << "Flight Number" << ": " << returnFlight->getFlightNumber() << "\n";
        cout << left << setw(labelWidth) << "Date" << ": " << returnFlight->getDate() << "\n";
        cout << left << setw(labelWidth) << "Time" << ": " << returnFlight->getTime() << "\n";
        cout << left << setw(labelWidth) << "Price per Ticket" << ": " << returnFlight->getPrice() << " PKR\n\n";
    }

    cout << "-------------- Passenger Details --------------\n";
    int totalPrice = 0;
    for (int i = 0; i < numTickets; ++i) {
        cout << "Passenger " << i + 1 << ":\n";
        int ageCat = passengers[i].getAgeCategory();
        string ageType = (ageCat == 1) ? "Adult" : (ageCat == 2) ? "Child" : "Infant";

        cout << left << setw(labelWidth) << "  Age Category" << ": " << ageType << "\n";
        cout << left << setw(labelWidth) << "  Seat Preference" << ": " << passengers[i].getSeat() << "\n";
        cout << left << setw(labelWidth) << "  Meal Preference" << ": " << passengers[i].getMeal() << "\n";

        int indivFare = 0;
        if (ageCat == 3)
            indivFare = static_cast<int>(outboundFlight.getPrice() * 0.2);
        else if (ageCat == 2)
            indivFare = static_cast<int>(outboundFlight.getPrice() * 0.75);
        else
            indivFare = outboundFlight.getPrice();

        if (ticketType == 2 && returnFlight != nullptr) {
            if (ageCat == 3)
                indivFare += static_cast<int>(returnFlight->getPrice() * 0.2);
            else if (ageCat == 2)
                indivFare += static_cast<int>(returnFlight->getPrice() * 0.75);
            else
                indivFare += returnFlight->getPrice();
        }

        totalPrice += indivFare;
        cout << left << setw(labelWidth) << "  Ticket Price" << ": " << indivFare << " PKR\n\n\n";
    }

    cout << left << setw(labelWidth) << "Total Tickets" << ": " << numTickets << "\n";
    cout << left << setw(labelWidth) << "Total Price" << ": " << totalPrice << " PKR\n";

    cout << "======================================================\n\n";
}


string generateSeat(int passengerNum, int travelClass) {
    // Basic logic to generate 2-3-2 style seat like 14A, 14C etc.
    string seats[] = { "A", "B", "C", "D", "E", "F", "G" }; // 2-3-2 has 7 seats per row
    int baseRow = (travelClass == 1) ? 14 : 1; // Economy starts at row 14
    int row = baseRow + (rand() % 20);
    string letter = seats[passengerNum % 7]; // Assign seat A to G
    return to_string(row) + letter;
}


void bookingSystem() {
    string cities[] = { "", "Islamabad", "Lahore", "Karachi", "Peshawar", "Quetta",
                        "Multan", "Skardu", "Faisalabad", "Sialkot" };

    int depIndex, arrIndex;

    while (true) {
        showAirports();
        cout << "\nEnter Departure Airport Number: "; cin >> depIndex;
        cout << "Enter Arrival Airport Number: "; cin >> arrIndex;

        if (depIndex < 1 || depIndex > 9 || arrIndex < 1 || arrIndex > 9) {
            cout << "\nInvalid Airport Number(s)! Please try again.\n";
            continue;
        }

        if (depIndex == arrIndex) {
            cout << "\nError: Departure and Arrival Airports cannot be the same.\n";
            cout << "1. Exit Program\n2. Reselect Airports\nEnter choice: ";
            int choice; cin >> choice;
            if (choice == 1) return;
            else continue;
        }
        break;
    }

    printHeader("Ticket Type and Class Selection");
    int ticketType;
    cout << "Select Ticket Type:\n1. One-Way\n2. Return\nChoice: ";
    cin >> ticketType;
    while (ticketType != 1 && ticketType != 2) {
        cout << "Invalid choice. Enter 1 or 2: ";
        cin >> ticketType;
    }

    int travelClass;
    cout << "\nSelect Travel Class:\n1. Economy\n2. Business\nChoice: ";
    cin >> travelClass;
    while (travelClass != 1 && travelClass != 2) {
        cout << "Invalid choice. Enter 1 or 2: ";
        cin >> travelClass;
    }

    double classMultiplier = (travelClass == 1) ? 1.0 : 1.5;
    vector<Flight> flights = generateFlights(cities[depIndex], cities[arrIndex]);
    for (auto& f : flights) f.setPrice(static_cast<int>(f.getPrice() * classMultiplier));

    string dates[11]; availableDates(dates, 11);
    printHeader("Outbound Date Selection");
    for (int i = 0; i < 11; ++i) cout << i + 1 << ". " << dates[i] << endl;
    int dateChoice;
    cout << "\nSelect Date Number: "; cin >> dateChoice;
    if (dateChoice < 1 || dateChoice > 11) return;

    cout << "\nSort Flights By:\n1. Closest Date\n2. Lowest Price\n3. Highest Price\nChoice: ";
    int sortChoice; cin >> sortChoice;
    if (sortChoice == 2)
        sort(flights.begin(), flights.end(), [](Flight a, Flight b) { return a.getPrice() < b.getPrice(); });
    else if (sortChoice == 3)
        sort(flights.begin(), flights.end(), [](Flight a, Flight b) { return a.getPrice() > b.getPrice(); });

    vector<Flight> filteredFlights;
    for (const auto& f : flights)
        if (f.getDate() == dates[dateChoice - 1])
            filteredFlights.push_back(f);

    printHeader("Flight Selection (Outbound)");
    cout << left << setw(5) << "Sr#"
        << setw(15) << "Airline"
        << setw(12) << "Flight#"
        << setw(15) << "Time"
        << setw(20) << "Date"
        << setw(15) << "Price (PKR)" << endl;
    cout << string(82, '-') << endl;

    int displayCount = min(5, (int)filteredFlights.size());
    for (int i = 0; i < displayCount; ++i) {
        const Flight& f = filteredFlights[i];
        cout << left << setw(5) << (i + 1)
            << setw(15) << f.getAirline()
            << setw(12) << f.getFlightNumber()
            << setw(15) << f.getTime()
            << setw(20) << f.getDate()
            << setw(15) << f.getPrice() << endl;
    }

    int flightChoice;
    cout << "\nSelect Flight Serial Number (1 - " << displayCount << "): ";
    cin >> flightChoice;
    if (flightChoice < 1 || flightChoice > displayCount) return;
    Flight chosen = filteredFlights[flightChoice - 1];

    Flight returnFlight;
    if (ticketType == 2) {
        vector<Flight> returnFlights = generateFlights(cities[arrIndex], cities[depIndex]);
        for (auto& f : returnFlights)
            f.setPrice(static_cast<int>(f.getPrice() * classMultiplier));

        printHeader("Return Date Selection");
        for (int i = dateChoice; i < 11; ++i)
            cout << i - dateChoice + 1 << ". " << dates[i] << endl;

        int retDateChoice;
        cout << "\nSelect Return Date Number: ";
        cin >> retDateChoice;
        retDateChoice += dateChoice - 1;
        if (retDateChoice < dateChoice || retDateChoice > 10) return;

        cout << "\nSort Return Flights By:\n1. Closest Date\n2. Lowest Price\n3. Highest Price\nChoice: ";
        int retSortChoice; cin >> retSortChoice;
        if (retSortChoice == 2)
            sort(returnFlights.begin(), returnFlights.end(), [](Flight a, Flight b) { return a.getPrice() < b.getPrice(); });
        else if (retSortChoice == 3)
            sort(returnFlights.begin(), returnFlights.end(), [](Flight a, Flight b) { return a.getPrice() > b.getPrice(); });

        vector<Flight> filteredReturnFlights;
        for (const auto& f : returnFlights)
            if (f.getDate() == dates[retDateChoice])
                filteredReturnFlights.push_back(f);

        printHeader("Return Flight Selection");
        cout << left << setw(5) << "Sr#"
            << setw(15) << "Airline"
            << setw(12) << "Flight#"
            << setw(15) << "Time"
            << setw(20) << "Date"
            << setw(15) << "Price (PKR)" << endl;
        cout << string(82, '-') << endl;

        int retDisplayCount = min(5, (int)filteredReturnFlights.size());
        for (int i = 0; i < retDisplayCount; ++i) {
            const Flight& f = filteredReturnFlights[i];
            cout << left << setw(5) << (i + 1)
                << setw(15) << f.getAirline()
                << setw(12) << f.getFlightNumber()
                << setw(15) << f.getTime()
                << setw(20) << f.getDate()
                << setw(15) << f.getPrice() << endl;
        }

        int returnChoice;
        cout << "\nSelect Return Flight Serial Number (1–" << retDisplayCount << "): ";
        cin >> returnChoice;

        if (returnChoice < 1 || returnChoice > filteredReturnFlights.size()) return;
        returnFlight = filteredReturnFlights[returnChoice - 1];
    }

    int numTickets;
    cout << "\nEnter Number of Tickets: "; cin >> numTickets;
    Passenger* passengers = new Passenger[numTickets];

    int baseRow = (travelClass == 1) ? 14 + rand() % 10 : 1 + rand() % 5;
    string seats[] = { "A", "B", "C", "D", "E", "F", "G" };
    int seatIndex = rand() % 4;

    for (int i = 0; i < numTickets; ++i) {
        printHeader("Passenger " + to_string(i + 1) + " Information");
        int ageOpt;
        cout << "1. Adult\n2. Child (2–11)\n3. Infant (Under 2)\nSelect Age Category: "; cin >> ageOpt;
        while (ageOpt < 1 || ageOpt > 3) { cout << "Invalid. Select 1–3: "; cin >> ageOpt; }
        passengers[i].setAgeCategory(ageOpt);

        if (!passengers[i].getIsInfant()) {
            int seatOpt, mealOpt;

            cout << "\n1. Window\n2. Middle\n3. Aisle\nSelect Seat Preference: "; cin >> seatOpt;
            while (seatOpt < 1 || seatOpt > 3) { cout << "Invalid. Select 1–3: "; cin >> seatOpt; }
            string seatPref = (seatOpt == 1) ? "Window" : (seatOpt == 2) ? "Middle" : "Aisle";
            passengers[i].setSeat(seatPref);

            cout << "\n1. Vegetarian\n2. Non-Vegetarian\n3. Diabetic\n4. Gluten-Free\n5. No Preference\nSelect Meal Preference: "; cin >> mealOpt;
            while (mealOpt < 1 || mealOpt > 5) { cout << "Invalid. Select 1–5: "; cin >> mealOpt; }
            string mealStr[] = { "Vegetarian", "Non-Vegetarian", "Diabetic", "Gluten-Free", "No Preference" };
            passengers[i].setMeal(mealStr[mealOpt - 1]);

            string assignedSeat = to_string(baseRow) + seats[(seatIndex + i) % 7];
            passengers[i].setSeat(assignedSeat);
        }
        else {
            passengers[i].setSeat("No Seat");
            passengers[i].setMeal("N/A");
        }
    }

    string bookingRef = "HR" + to_string(rand() % 9000 + 1000);
    printTicket(depIndex, arrIndex, chosen, passengers, numTickets, travelClass, ticketType, (ticketType == 2 ? &returnFlight : nullptr), globalLoggedInName, bookingRef);

    ofstream bfile("bookings.txt", ios::app);
    bfile << "BookingRef: " << bookingRef << endl;
    bfile << "Full Name: " << globalLoggedInName << endl;
    bfile << "From: " << cities[depIndex] << endl;
    bfile << "To: " << cities[arrIndex] << endl;
    bfile << "Flight Date: " << chosen.getDate() << endl;
    bfile << "Flight#: " << chosen.getFlightNumber() << endl;
    bfile << "Airline: " << chosen.getAirline() << endl;
    bfile << "Tickets: " << numTickets << endl;

    delete[] passengers;
}




void postAccountMenu() {
    int option;
    cout << "\n1. Sign In\n2. Exit\n\nEnter Choice: ";
    cin >> option;

    if (option == 1) {
        printHeader("Sign In");
        string username, password;
        cout << "Enter Username: "; cin >> username;
        cout << "Enter Password: "; cin >> password;
        if (accountExists(username, password)) {
            bookingSystem();
            cout << "\nThank you for booking with HAN RENTALS AIRLINES. Goodbye!\n";
            exit(0);
        }
        else {
            cout << "\nInvalid credentials!\n";
        }
    }
    else {
        cout << "\nThank you for using HAN RENTALS AIRLINES. Goodbye!\n";
        exit(0);
    }
}

void createAccount() {
    printHeader("Create Account");
    string name, username, password;
    cout << "Enter Full Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Create Username: ";
    cin >> username;
    cout << "Create Password: ";
    cin >> password;

    ofstream file("users.txt", ios::app);
    file << username << " " << password << " " << name << endl;


    printHeader("Account Created");
    cout << "Your account has been created successfully!\n";
    postAccountMenu();
}

void checkBooking() {
    printHeader("CHECK EXISTING BOOKING");
    string ref, name;

    cout << "Enter Booking Reference Number (e.g., HR1234): ";
    cin >> ref;
    cin.ignore(); // Clear newline from input buffer

    cout << "Enter Full Name (used during booking): ";
    getline(cin, name);

    ifstream file("bookings.txt");
    if (!file.is_open()) {
        cout << "\nCould not open bookings file.\n";
        system("pause");
        return;
    }

    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find("BookingRef:") != string::npos && line.find(ref) != string::npos) {
            string tempBlock = line + "\n";
            bool nameMatched = false;

            for (int i = 0; i < 10 && getline(file, line); ++i) {
                tempBlock += line + "\n";
                if (line.find("Full Name:") != string::npos && line.find(name) != string::npos) {
                    nameMatched = true;
                }
            }

            if (nameMatched) {
                printHeader("WAITING TO HOST YOU EAGERLY!");

                istringstream blockStream(tempBlock);
                string label, line;
                string bookingRef, fullName, from, to, date, flightNum, airline, tickets;
                int totalCost = 0;

                while (getline(blockStream, line)) {
                    size_t pos = line.find(":");
                    if (pos != string::npos) {
                        label = line.substr(0, pos);
                        string value = line.substr(pos + 1);
                        value.erase(0, value.find_first_not_of(" \t"));

                        if (label.find("BookingRef") != string::npos) bookingRef = value;
                        else if (label.find("Full Name") != string::npos) fullName = value;
                        else if (label.find("From") != string::npos) from = value;
                        else if (label.find("To") != string::npos) to = value;
                        else if (label.find("Flight Date") != string::npos) date = value;
                        else if (label.find("Flight#") != string::npos) flightNum = value;
                        else if (label.find("Airline") != string::npos) airline = value;
                        else if (label.find("Tickets") != string::npos) tickets = value;
                    }
                }

                // Display aligned output
                cout << left << setw(20) << "Booking Ref" << ": " << bookingRef << endl;
                cout << left << setw(20) << "Full Name" << ": " << fullName << endl;
                cout << left << setw(20) << "From" << ": " << from << endl;
                cout << left << setw(20) << "To" << ": " << to << endl;
                cout << left << setw(20) << "Flight Date" << ": " << date << endl;
                cout << left << setw(20) << "Flight #" << ": " << flightNum << endl;
                cout << left << setw(20) << "Airline" << ": " << airline << endl;
                cout << left << setw(20) << "Tickets" << ": " << tickets << endl;
                cout << "\n" << string(45, '=') << "\n\n";

                found = true;
                break;
            }

        }
    }

    if (!found) {
        cout << "\nNo matching booking found for that reference number and full name.\n";
    }

    system("pause"); // ? So output stays on screen before returning
}




int main() {
    srand(time(0)); // seed randomness once
    while (true) {
        printHeader("HAN RENTALS AIRLINES");
        cout << "Welcome! What would you like to do?\n";
        cout << "1. Create New Booking\n";
        cout << "2. Check Existing Booking\n";
        cout << "3. Exit\n\nEnter Choice: ";
        int choice;
        cin >> choice;
        if (choice == 1) {
            while (true) {
                printHeader("MAIN MENU");
                cout << "1. Create Account\n2. Sign In\n3. Exit\n\nEnter Choice: ";
                int choice;
                cin >> choice;
                if (choice == 1) {
                    createAccount();
                }
                else if (choice == 2) {
                    printHeader("Sign In");
                    string username, password;
                    cout << "Enter Username: "; cin >> username;
                    cout << "Enter Password: "; cin >> password;
                    if (accountExists(username, password)) {
                        bookingSystem();
                        cout << "\nThank you for booking with HAN RENTALS AIRLINES. Goodbye!\n";
                        exit(0);
                    }
                    else {
                        cout << "\nInvalid credentials!\n";
                    }
                }
                else {
                    cout << "\nExiting...\n";
                    break;
                }
            }
        }
        else if (choice == 2) {
            checkBooking();
        }
        else if ((choice == 3)) {
            cout << "\nThank you for using HAN RENTALS AIRLINES. Goodbye!\n";
            break;
        }
        else {
            cout << "\nERROR! Invalid Input" << endl << "1. Retry\n2. Exit\n\nEnter Choice: ";
            int choice;
            cin >> choice;
            if (choice == 1) {
                main();
            }
            else {
                cout << "Exiting...";
            }
        }
    }
    return 0;
}
