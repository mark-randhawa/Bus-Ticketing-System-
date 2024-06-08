#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <limits>

using namespace std;
//declaring the functions
void displayMenu();
void newTicket();
void viewTicket();
void availableBuses();
void saveTicketToFile(const struct Ticket& ticket);
void loadTicketsFromFile();
int getValidatedInput(int min, int max);
void displayCities();
void displayTimeSlots();
int getRandomNumber(int min, int max);

//constants
const int MAX_CITIES = 5;
const int MAX_BUSES_PER_CITY = 3;
const int MAX_TICKETS = 10;
const int TIME_SLOTS = 5;

//details of cities, buses, drivers and time slots
string cities[MAX_CITIES] = { "Karachi", "Lahore", "Islamabad", "Rawalpindi", "Multan" };
string buses[MAX_CITIES][MAX_BUSES_PER_CITY] = {
    {"Bus A", "Bus B", "Bus C"},
    {"Bus X", "Bus Y", "Bus Z"},
    {"Bus P", "Bus Q", "Bus R"},
    {"Bus M", "Bus N", "Bus O"},
    {"Bus S", "Bus T", "Bus U"}
};
string drivers[] = { "Rana", "Ijaz", "Zain", "Raja", "Gujjar" };
int busIDs[MAX_CITIES][MAX_BUSES_PER_CITY] = {
    {101, 102, 103},
    {201, 202, 203},
    {301, 302, 303},
    {401, 402, 403},
    {501, 502, 503}
};
string departureTimes[TIME_SLOTS] = { "08:00 AM", "10:00 AM", "12:00 PM", "02:00 PM", "04:00 PM" };
string reachTimes[TIME_SLOTS] = { "02:00 PM", "04:00 PM", "06:00 PM", "08:00 PM", "10:00 PM" };

//structure of ticket
struct Ticket {
    int ticketID;
    string passengerName;
    string currentCity;
    string destinationCity;
    string bus;
    string driver;
    string departureTime;
    string reachTime;
    int seatNumber;
};

//ticket array and ticket counter
Ticket tickets[MAX_TICKETS];
int totalTickets = 0;

int main()
{
    srand(static_cast<unsigned int>(time(0)));
    loadTicketsFromFile();

    int choice;
    do
    {
        //display the menu
        displayMenu();
        cout << "Enter your choice: ";
        choice = getValidatedInput(1, 4);  //check if the choice is in the valid range

        //display functions according to the valid choice
        switch (choice) 
        {
        case 1:
            newTicket();
            break;
        case 2:
            viewTicket();
            break;
        case 3:
            availableBuses();
            break;
        case 4:
            cout << "Exiting the program. Thank you!\n";
            break;
        default:
            cout << "Invalid choice. Please enter a valid option.\n";
        }
    } while (choice != 4);

    return 0;
}

void displayMenu() 
{
    cout << "\n----- Bus Ticket System Menu -----\n";
    cout << "1. New Ticket\n";
    cout << "2. View Ticket\n";
    cout << "3. Available Buses\n";
    cout << "4. Exit\n";
    cout << "----------------------------------\n";
}

//function to create new ticket
void newTicket() 
{
    if (totalTickets < MAX_TICKETS) {
        Ticket newTicket;
        newTicket.ticketID = totalTickets + 1;

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //to clear any previous input

        cout << "Enter passenger name: ";
        getline(cin, newTicket.passengerName);

        cout << "Cities:\n";
        displayCities(); // displaying the cities

        cout << "Enter the number of the current city: ";
        int currentCityIndex = getValidatedInput(1, MAX_CITIES); //checking the validity
        newTicket.currentCity = cities[currentCityIndex - 1];

        cout << "Enter the number of the destination city: ";
        int destinationCityIndex = getValidatedInput(1, MAX_CITIES);
        newTicket.destinationCity = cities[destinationCityIndex - 1];

        bool busFound = false;
        for (int i = 0; i < MAX_BUSES_PER_CITY; ++i) 
        {
            cout << i + 1 << ". " << buses[currentCityIndex - 1][i] << " (ID: " << busIDs[currentCityIndex - 1][i] << ")\n";
        }

        do 
        {
            cout << "Enter the number of the bus: ";
            int busIndex = getValidatedInput(1, MAX_BUSES_PER_CITY);
            newTicket.bus = buses[currentCityIndex - 1][busIndex - 1];
            busFound = true;
        } while (!busFound);

        //assigning a random deiver
        newTicket.driver = drivers[getRandomNumber(0, 4)];

        // display and select time slot
        displayTimeSlots();
        cout << "Select a time slot: ";
        int timeSlotIndex = getValidatedInput(1, TIME_SLOTS) - 1;
        newTicket.departureTime = departureTimes[timeSlotIndex];
        newTicket.reachTime = reachTimes[timeSlotIndex];

        //assigning random seat
        int availableSeats = getRandomNumber(1, 10);
        if (availableSeats > 0)
        {
            newTicket.seatNumber = getRandomNumber(1, availableSeats);
            cout << "Seat assigned: " << newTicket.seatNumber << "\n";
        }
        else 
        {
            cout << "Sorry, the bus is full. Cannot book a new ticket.\n";
            return;
        }

        //saving the ticket and updating the ticket counter 
        tickets[totalTickets++] = newTicket;
        saveTicketToFile(newTicket);

        cout << "\nTicket successfully booked!\n";
    }
    else
    {
        cout << "Maximum number of tickets reached. Cannot book a new ticket.\n";
    }
}

//saving the ticket to file
void saveTicketToFile(const struct Ticket& ticket) 
{
    ofstream outFile("tickets.txt", ios::app);
    if (outFile.is_open()) 
    {
        outFile << ticket.ticketID << '\n'
            << ticket.passengerName << '\n'
            << ticket.currentCity << '\n'
            << ticket.destinationCity << '\n'
            << ticket.bus << '\n'
            << ticket.driver << '\n'
            << ticket.departureTime << '\n'
            << ticket.reachTime << '\n'
            << ticket.seatNumber << '\n';
        outFile.close();
    }
    else
    {
        cout << "Unable to open file to save ticket.\n";
    }
}

void loadTicketsFromFile() 
{
    ifstream inFile("tickets.txt");
    if (!inFile)
    {
        cout << "No previous tickets found. Starting fresh.\n";
        return;
    }

    //reading the ticket from file till the max limit is reached
    while (inFile && totalTickets < MAX_TICKETS) 
    {
        Ticket ticket;
        inFile >> ticket.ticketID;
        inFile.ignore();
        getline(inFile, ticket.passengerName);
        if (ticket.passengerName.empty()) break;
        getline(inFile, ticket.currentCity);
        getline(inFile, ticket.destinationCity);
        getline(inFile, ticket.bus);
        getline(inFile, ticket.driver);
        getline(inFile, ticket.departureTime);
        getline(inFile, ticket.reachTime);
        inFile >> ticket.seatNumber;
        inFile.ignore(numeric_limits<streamsize>::max(), '\n');

        tickets[totalTickets++] = ticket;
    }
    inFile.close();
}

//displaying available tickets
void viewTicket() 
{
    if (totalTickets == 0)
    {
        cout << "No tickets available.\n";
    }
    else
    {
        cout << "\nViewing Tickets:\n";
        for (int i = 0; i < totalTickets; ++i) 
        {
            cout << "Ticket ID: " << tickets[i].ticketID << "\n";
            cout << "Passenger Name: " << tickets[i].passengerName << "\n";
            cout << "Current City: " << tickets[i].currentCity << "\n";
            cout << "Destination City: " << tickets[i].destinationCity << "\n";
            cout << "Bus: " << tickets[i].bus << "\n";
            cout << "Driver: " << tickets[i].driver << "\n";
            cout << "Departure Time: " << tickets[i].departureTime << "\n";
            cout << "Reach Time: " << tickets[i].reachTime << "\n";
            cout << "Seat Number: " << tickets[i].seatNumber << "\n";
            cout << "------------------------\n";
        }
    }
}

//displaying available buses
void availableBuses()
{
    cout << "\nAvailable Buses:\n";
    for (int i = 0; i < MAX_CITIES; ++i)
    {
        cout << "From " << cities[i] << ":\n";
        for (int j = 0; j < MAX_BUSES_PER_CITY; ++j) 
        {
            cout << " - " << buses[i][j] << "\n";
        }
    }
}

int getValidatedInput(int min, int max) 
{
    int input;
    while (true) 
    {
        cin >> input;
        //input failure check
        if (cin.fail()  || input < min || input > max) 
        {
            //clearing input error
            cin.clear();
            //ignoring invalid chracters
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return input;
        }
    }
}

//displaying cities
void displayCities() 
{
    for (int i = 0; i < MAX_CITIES; ++i) 
    {
        cout << i + 1 << ". " << cities[i] << "\n";
    }
}

//displaying time slotes
void displayTimeSlots() {
    cout << "Available Time Slots:\n";
    for (int i = 0; i < TIME_SLOTS; ++i)
    {
        cout << i + 1 << ". Departure: " << departureTimes[i] << ", Reach: " << reachTimes[i] << "\n";
    }
}

//generating random number
int getRandomNumber(int min, int max)
{
    return rand() % (max - min + 1) + min;
}
