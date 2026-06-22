// main.cpp  –  Entry point, global definitions, main menu
// ============================================================
//   Smart Flight Reservation & Waiting Management System
//   Language  : C++
//   Concepts  : Arrays, Linked List, Queue, Priority Queue,
//               Sorting (Insertion Sort), Searching (Linear)
// ============================================================

#include "globals.h"
#include "functions.h"

// ============================================================
//  GLOBAL VARIABLE DEFINITIONS
// ============================================================
Flight       flights[MAX_FLIGHTS];
int          flightCount       = 0;
Passenger*   passengerHead     = nullptr;
int          passengerIdCounter = 0;
priority_queue<WaitingEntry> waitingList;
vector<Notification>         notifications;

// ============================================================
//  SEED DEMO DATA  (so system is usable right away)
// ============================================================
static void seedDemoData() {
    // Flight 1
    flights[0].active        = true;
    flights[0].flightNumber  = "PK-302";
    flights[0].origin        = "Karachi";
    flights[0].destination   = "Lahore";
    flights[0].date          = "25/07/25";
    flights[0].departureTime = "08:00";
    flights[0].arrivalTime   = "09:30";
    flights[0].type          = DOMESTIC;
    flights[0].totalSeats    = 10;   // small for easy demo
    flights[0].bookedSeats   = 0;
    flights[0].baseFare      = 8000.0;
    for(int s = 0; s < 10; s++) {
        flights[0].seats[s].number      = s + 1;
        flights[0].seats[s].isBooked    = false;
        flights[0].seats[s].passengerId = 0;
        flights[0].seats[s].seatClass   = (s < 1) ? FIRST_CLASS :
                                          (s < 3) ? BUSINESS : ECONOMY;
    }

    // Flight 2
    flights[1].active        = true;
    flights[1].flightNumber  = "EK-601";
    flights[1].origin        = "Islamabad";
    flights[1].destination   = "Dubai";
    flights[1].date          = "26/07/25";
    flights[1].departureTime = "14:00";
    flights[1].arrivalTime   = "16:30";
    flights[1].type          = INTERNATIONAL;
    flights[1].totalSeats    = 15;
    flights[1].bookedSeats   = 0;
    flights[1].baseFare      = 25000.0;
    for(int s = 0; s < 15; s++) {
        flights[1].seats[s].number      = s + 1;
        flights[1].seats[s].isBooked    = false;
        flights[1].seats[s].passengerId = 0;
        flights[1].seats[s].seatClass   = (s < 2) ? FIRST_CLASS :
                                          (s < 5) ? BUSINESS : ECONOMY;
    }

    // Flight 3
    flights[2].active        = true;
    flights[2].flightNumber  = "PF-105";
    flights[2].origin        = "Lahore";
    flights[2].destination   = "Quetta";
    flights[2].date          = "27/07/25";
    flights[2].departureTime = "10:30";
    flights[2].arrivalTime   = "12:00";
    flights[2].type          = DOMESTIC;
    flights[2].totalSeats    = 8;
    flights[2].bookedSeats   = 0;
    flights[2].baseFare      = 6500.0;
    for(int s = 0; s < 8; s++) {
        flights[2].seats[s].number      = s + 1;
        flights[2].seats[s].isBooked    = false;
        flights[2].seats[s].passengerId = 0;
        flights[2].seats[s].seatClass   = (s < 1) ? FIRST_CLASS :
                                          (s < 2) ? BUSINESS : ECONOMY;
    }

    flightCount = 3;
    addNotification("System initialized with 3 demo flights.");
}

// ============================================================
//  PASSENGER RECORDS SUB-MENU
// ============================================================
static void passengerMenu() {
    int ch;
    do {
        printHeader("PASSENGER RECORDS");
        cout << "  1. View All Passengers\n";
        cout << "  2. Search Passenger\n";
        cout << "  3. Update Passenger\n";
        cout << "  4. Delete Passenger\n";
        cout << "  0. Back\n";
        printLine('-');
        cout << "Choice: "; cin >> ch;
        switch(ch) {
            case 1: viewPassengerRecords(); break;
            case 2: searchPassenger();      break;
            case 3: updatePassenger();      break;
            case 4: deletePassenger();      break;
        }
    } while(ch != 0);
}

// ============================================================
//  FLIGHT SCHEDULE SUB-MENU
// ============================================================
static void flightScheduleMenu() {
    int ch;
    do {
        printHeader("FLIGHT SCHEDULE");
        cout << "  1. View Flight Detail / Seat Map\n";
        cout << "  2. Update Schedule\n";
        cout << "  3. Remove Flight\n";
        cout << "  0. Back\n";
        printLine('-');
        cout << "Choice: "; cin >> ch;
        switch(ch) {
            case 1: showFlightSchedule();    break;
            case 2: updateFlightSchedule();  break;
            case 3: removeFlight();          break;
        }
    } while(ch != 0);
}

// ============================================================
//  NOTIFICATIONS SUB-MENU
// ============================================================
static void notificationsMenu() {
    int ch;
    do {
        printHeader("NOTIFICATIONS");
        cout << "  1. View All Notifications\n";
        cout << "  2. Clear Notifications\n";
        cout << "  0. Back\n";
        printLine('-');
        cout << "Choice: "; cin >> ch;
        switch(ch) {
            case 1: viewNotifications();  break;
            case 2: clearNotifications(); break;
        }
    } while(ch != 0);
}

// ============================================================
//  FARE SUB-MENU
// ============================================================
static void fareMenu() {
    int ch;
    do {
        printHeader("FARE & BILLING");
        cout << "  1. Calculate Fare\n";
        cout << "  2. Generate Passenger Bill\n";
        cout << "  0. Back\n";
        printLine('-');
        cout << "Choice: "; cin >> ch;
        switch(ch) {
            case 1: calculateFare(); break;
            case 2: generateBill();  break;
        }
    } while(ch != 0);
}

// ============================================================
//  MAIN MENU
// ============================================================
static void showBanner() {
    cout << "\n";
    printLine('*');
    cout << "*   SMART FLIGHT RESERVATION & WAITING MANAGEMENT SYSTEM   *\n";
    printLine('*');
    cout << "\n";
}

static void mainMenu() {
    int choice;
    do {
        showBanner();
        cout << "  MAIN MENU\n";
        printLine('-');
        cout << "  1.  Add Flight\n";
        cout << "  2.  View All Flights\n";
        cout << "  3.  Book a Seat\n";
        cout << "  4.  Cancel Booking\n";
        cout << "  5.  View Waiting List\n";
        cout << "  6.  VIP Booking\n";
        cout << "  7.  Passenger Records\n";
        cout << "  8.  Search Passenger\n";
        cout << "  9.  Fare Calculation\n";
        cout << "  10. Flight Schedule\n";
        cout << "  11. Family Group Booking\n";
        cout << "  12. Notifications\n";
        cout << "  13. Sort Flights\n";
        cout << "  14. Check Confirmation Expiry\n";
        cout << "  0.  Exit\n";
        printLine('-');
        cout << "  Enter choice: ";
        cin  >> choice;

        switch(choice) {
            case 1:  addFlight();             break;
            case 2:  viewFlights();           break;
            case 3:  bookSeat();              break;
            case 4:  cancelBooking();         break;
            case 5:  viewWaitingList();       break;
            case 6:  vipBooking();            break;
            case 7:  passengerMenu();         break;
            case 8:  searchPassenger();       break;
            case 9:  fareMenu();              break;
            case 10: flightScheduleMenu();    break;
            case 11: familyBooking();         break;
            case 12: notificationsMenu();     break;
            case 13: sortFlights();           break;
            case 14: checkWaitingListExpiry(); break;
            case 0:
                cout << "\n✈  Thank you for using the Flight Reservation System. Goodbye!\n\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                pauseScreen();
        }
    } while(choice != 0);
}

// ============================================================
//  PROGRAM ENTRY
// ============================================================
int main() {
    seedDemoData();
    mainMenu();

    // Clean up linked list
    Passenger* cur = passengerHead;
    while(cur) {
        Passenger* next = cur->next;
        delete cur;
        cur = next;
    }
    return 0;
}
