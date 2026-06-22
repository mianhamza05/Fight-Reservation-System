// passenger.cpp  –  Passenger linked-list CRUD
#include "globals.h"

// ============================================================
//  ADD PASSENGER TO LINKED LIST  (internal helper)
// ============================================================
Passenger* createPassenger(const string& name, const string& cnic,
                           int age, Gender gender,
                           const string& flightNo, int seatNo,
                           SeatClass sc, bool isVIP) {
    Passenger* p  = new Passenger();
    p->id         = ++passengerIdCounter;
    p->name       = name;
    p->cnic       = cnic;
    p->age        = age;
    p->gender     = gender;
    p->flightNumber = flightNo;
    p->seatNumber = seatNo;
    p->seatClass  = sc;
    p->isVIP      = isVIP;
    p->confirmed  = true;
    p->next       = nullptr;

    // Prepend to linked list
    if(passengerHead == nullptr) {
        passengerHead = p;
    } else {
        p->next       = passengerHead;
        passengerHead = p;
    }
    return p;
}

// ============================================================
//  DISPLAY ONE PASSENGER
// ============================================================
void displayPassenger(const Passenger* p) {
    if(!p) return;
    printLine('-');
    cout << "ID           : " << p->id           << "\n";
    cout << "Name         : " << p->name          << "\n";
    cout << "CNIC/Passport: " << p->cnic          << "\n";
    cout << "Age          : " << p->age           << "\n";
    cout << "Gender       : " << genderName(p->gender) << "\n";
    cout << "Flight       : " << p->flightNumber  << "\n";
    cout << "Seat         : " << p->seatNumber    << "\n";
    cout << "Class        : " << seatClassName(p->seatClass) << "\n";
    cout << "VIP          : " << (p->isVIP ? "Yes" : "No") << "\n";
    cout << "Status       : " << (p->confirmed ? "Confirmed" : "Pending Confirmation") << "\n";
    printLine('-');
}

// ============================================================
//  VIEW ALL PASSENGER RECORDS
// ============================================================
void viewPassengerRecords() {
    printHeader("ALL PASSENGER RECORDS");

    if(!passengerHead) {
        cout << "No passenger records found.\n";
        pauseScreen(); return;
    }

    Passenger* cur = passengerHead;
    int count = 0;
    while(cur) {
        displayPassenger(cur);
        cur = cur->next;
        count++;
    }
    cout << "\nTotal passengers: " << count << "\n";
    pauseScreen();
}

// ============================================================
//  SEARCH PASSENGER  (by name or CNIC – linear search)
// ============================================================
void searchPassenger() {
    printHeader("SEARCH PASSENGER");
    cout << "Search by:\n  1. Name\n  2. CNIC/Passport\n  3. Passenger ID\nChoice: ";
    int ch; cin >> ch;

    bool found = false;

    if(ch == 1) {
        cin.ignore();
        string name; cout << "Enter Name: "; getline(cin, name);
        // Convert to lowercase for comparison
        string nameLower = name;
        transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

        Passenger* cur = passengerHead;
        while(cur) {
            string curLower = cur->name;
            transform(curLower.begin(), curLower.end(), curLower.begin(), ::tolower);
            if(curLower.find(nameLower) != string::npos) {
                displayPassenger(cur);
                found = true;
            }
            cur = cur->next;
        }
    } else if(ch == 2) {
        string cnic; cout << "Enter CNIC/Passport: "; cin >> cnic;
        Passenger* p = findPassengerByCNIC(cnic);
        if(p) { displayPassenger(p); found = true; }
    } else if(ch == 3) {
        int id; cout << "Enter Passenger ID: "; cin >> id;
        Passenger* p = findPassengerById(id);
        if(p) { displayPassenger(p); found = true; }
    }

    if(!found) cout << "No matching passenger found.\n";
    pauseScreen();
}

// ============================================================
//  UPDATE PASSENGER DETAILS
// ============================================================
void updatePassenger() {
    printHeader("UPDATE PASSENGER DETAILS");
    int id; cout << "Enter Passenger ID to update: "; cin >> id;

    Passenger* p = findPassengerById(id);
    if(!p) { cout << "Passenger not found!\n"; pauseScreen(); return; }

    displayPassenger(p);
    cin.ignore();

    cout << "New Name (Enter to keep [" << p->name << "]): ";
    string tmp; getline(cin, tmp);
    if(!tmp.empty()) p->name = tmp;

    cout << "New Age  (0 to keep [" << p->age << "]): ";
    int age; cin >> age;
    if(age > 0) p->age = age;

    cin.ignore();
    cout << "VIP status? (1=Yes, 0=No) [" << p->isVIP << "]: ";
    string vStr; getline(cin, vStr);
    if(vStr == "1") p->isVIP = true;
    else if(vStr == "0") p->isVIP = false;

    cout << " Passenger updated.\n";
    pauseScreen();
}

// ============================================================
//  DELETE PASSENGER RECORD
// ============================================================
void deletePassenger() {
    printHeader("DELETE PASSENGER RECORD");
    int id; cout << "Enter Passenger ID to delete: "; cin >> id;

    Passenger* cur  = passengerHead;
    Passenger* prev = nullptr;

    while(cur) {
        if(cur->id == id) {
            if(prev) prev->next = cur->next;
            else     passengerHead = cur->next;

            // Free seat if still booked
            int idx = getFlightIndex(cur->flightNumber);
            if(idx != -1 && cur->seatNumber > 0) {
                int s = cur->seatNumber - 1;
                if(s >= 0 && s < flights[idx].totalSeats) {
                    flights[idx].seats[s].isBooked    = false;
                    flights[idx].seats[s].passengerId = 0;
                    flights[idx].bookedSeats--;
                }
            }

            delete cur;
            cout << " Passenger record deleted.\n";
            pauseScreen(); return;
        }
        prev = cur;
        cur  = cur->next;
    }

    cout << "Passenger not found!\n";
    pauseScreen();
}
