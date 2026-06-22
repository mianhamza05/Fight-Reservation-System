// flight.cpp  –  Flight management (Add / View / Update / Remove / Sort)
#include "globals.h"

// ============================================================
//  ADD FLIGHT
// ============================================================
void addFlight() {
    printHeader("ADD NEW FLIGHT");

    if(flightCount >= MAX_FLIGHTS) {
        cout << "Maximum flight capacity reached!\n";
        pauseScreen(); return;
    }

    Flight& f = flights[flightCount];
    f.active = true;

    cout << "Flight Number   : "; cin >> f.flightNumber;

    // Duplicate check
    for(int i = 0; i < flightCount; i++) {
        if(flights[i].active && flights[i].flightNumber == f.flightNumber) {
            cout << "Flight number already exists!\n";
            f.active = false;
            pauseScreen(); return;
        }
    }

    cin.ignore();
    cout << "Origin          : "; getline(cin, f.origin);
    cout << "Destination     : "; getline(cin, f.destination);
    cout << "Date (DD/MM/YY) : "; getline(cin, f.date);
    cout << "Departure Time  : "; getline(cin, f.departureTime);
    cout << "Arrival Time    : "; getline(cin, f.arrivalTime);

    cout << "Flight Type (1=Domestic, 2=International): ";
    int ft; cin >> ft;
    f.type = (ft == 2) ? INTERNATIONAL : DOMESTIC;

    cout << "Total Seats (max " << MAX_SEATS << "): ";
    cin >> f.totalSeats;
    if(f.totalSeats < 1 || f.totalSeats > MAX_SEATS) f.totalSeats = 50;

    cout << "Base Fare (PKR) : "; cin >> f.baseFare;

    // Initialize seats
    for(int s = 0; s < f.totalSeats; s++) {
        f.seats[s].number      = s + 1;
        f.seats[s].isBooked    = false;
        f.seats[s].passengerId = 0;
        // First 10% = First Class, next 20% = Business, rest = Economy
        int firstClassEnd  = max(1, f.totalSeats / 10);
        int businessEnd    = firstClassEnd + max(1, f.totalSeats / 5);
        if(s < firstClassEnd)
            f.seats[s].seatClass = FIRST_CLASS;
        else if(s < businessEnd)
            f.seats[s].seatClass = BUSINESS;
        else
            f.seats[s].seatClass = ECONOMY;
    }

    f.bookedSeats = 0;
    flightCount++;

    addNotification("New flight " + f.flightNumber + " (" + f.origin +
                    " -> " + f.destination + ") added successfully.");
    cout << "\n✅ Flight added successfully!\n";
    pauseScreen();
}

// ============================================================
//  VIEW ALL FLIGHTS
// ============================================================
void viewFlights() {
    printHeader("ALL FLIGHTS");

    if(flightCount == 0) {
        cout << "No flights available.\n";
        pauseScreen(); return;
    }

    cout << left
         << setw(10) << "Flight"
         << setw(12) << "Origin"
         << setw(14) << "Destination"
         << setw(10) << "Date"
         << setw(8)  << "Depart"
         << setw(8)  << "Arrive"
         << setw(7)  << "Type"
         << setw(7)  << "Seats"
         << setw(7)  << "Avail"
         << setw(10) << "Fare(PKR)"
         << "\n";
    printLine('-');

    for(int i = 0; i < flightCount; i++) {
        if(!flights[i].active) continue;
        Flight& f = flights[i];
        cout << left
             << setw(10) << f.flightNumber
             << setw(12) << f.origin
             << setw(14) << f.destination
             << setw(10) << f.date
             << setw(8)  << f.departureTime
             << setw(8)  << f.arrivalTime
             << setw(7)  << flightTypeName(f.type)
             << setw(7)  << f.totalSeats
             << setw(7)  << (f.totalSeats - f.bookedSeats)
             << setw(10) << fixed << setprecision(0) << f.baseFare
             << "\n";
    }
    pauseScreen();
}

// ============================================================
//  REMOVE FLIGHT
// ============================================================
void removeFlight() {
    printHeader("REMOVE FLIGHT");
    string flightNo;
    cout << "Enter Flight Number to remove: "; cin >> flightNo;

    int idx = getFlightIndex(flightNo);
    if(idx == -1) {
        cout << "Flight not found!\n";
        pauseScreen(); return;
    }

    flights[idx].active = false;
    addNotification("Flight " + flightNo + " has been removed.");
    cout << "✅ Flight removed successfully.\n";
    pauseScreen();
}

// ============================================================
//  UPDATE FLIGHT SCHEDULE
// ============================================================
void updateFlightSchedule() {
    printHeader("UPDATE FLIGHT SCHEDULE");
    string flightNo;
    cout << "Enter Flight Number: "; cin >> flightNo;

    int idx = getFlightIndex(flightNo);
    if(idx == -1) {
        cout << "Flight not found!\n";
        pauseScreen(); return;
    }

    Flight& f = flights[idx];
    cin.ignore();
    cout << "New Date (or Enter to keep [" << f.date << "]): ";
    string tmp; getline(cin, tmp);
    if(!tmp.empty()) f.date = tmp;

    cout << "New Departure Time (or Enter to keep [" << f.departureTime << "]): ";
    getline(cin, tmp);
    if(!tmp.empty()) f.departureTime = tmp;

    cout << "New Arrival Time (or Enter to keep [" << f.arrivalTime << "]): ";
    getline(cin, tmp);
    if(!tmp.empty()) f.arrivalTime = tmp;

    addNotification("Schedule updated for flight " + flightNo + ".");
    cout << "✅ Schedule updated.\n";
    pauseScreen();
}

// ============================================================
//  SHOW FLIGHT SCHEDULE (single flight detail)
// ============================================================
void showFlightSchedule() {
    printHeader("FLIGHT SCHEDULE DETAIL");
    string flightNo;
    cout << "Enter Flight Number: "; cin >> flightNo;

    int idx = getFlightIndex(flightNo);
    if(idx == -1) {
        cout << "Flight not found!\n";
        pauseScreen(); return;
    }

    Flight& f = flights[idx];
    printLine('-');
    cout << "Flight Number  : " << f.flightNumber      << "\n";
    cout << "Origin         : " << f.origin             << "\n";
    cout << "Destination    : " << f.destination        << "\n";
    cout << "Date           : " << f.date               << "\n";
    cout << "Departure      : " << f.departureTime      << "\n";
    cout << "Arrival        : " << f.arrivalTime        << "\n";
    cout << "Type           : " << flightTypeName(f.type) << "\n";
    cout << "Total Seats    : " << f.totalSeats         << "\n";
    cout << "Available      : " << (f.totalSeats - f.bookedSeats) << "\n";
    cout << "Base Fare      : PKR " << fixed << setprecision(2) << f.baseFare << "\n";
    printLine('-');

    // Seat map
    cout << "\nSEAT MAP  (B=Booked, FC=First Class, BZ=Business, EC=Economy)\n\n";
    for(int s = 0; s < f.totalSeats; s++) {
        Seat& seat = f.seats[s];
        string label = "";
        if(seat.isBooked) label = " [X]";
        else {
            if(seat.seatClass == FIRST_CLASS)  label = " [FC]";
            else if(seat.seatClass == BUSINESS) label = " [BZ]";
            else                                label = " [EC]";
        }
        cout << setw(3) << seat.number << label;
        if((s + 1) % 6 == 0) cout << "\n";
    }
    cout << "\n";
    pauseScreen();
}

// ============================================================
//  SORT FLIGHTS
// ============================================================
void sortFlights() {
    printHeader("SORT FLIGHTS");
    cout << "Sort by:\n";
    cout << "  1. Fare (ascending)\n";
    cout << "  2. Departure Time\n";
    cout << "  3. Destination (alphabetical)\n";
    cout << "Choice: ";
    int ch; cin >> ch;

    // Collect active flights indices
    vector<int> idx;
    for(int i = 0; i < flightCount; i++)
        if(flights[i].active) idx.push_back(i);

    if(idx.empty()) { cout << "No flights to sort.\n"; pauseScreen(); return; }

    // Simple insertion sort on the indices
    for(int i = 1; i < (int)idx.size(); i++) {
        int key = idx[i];
        int j   = i - 1;
        bool swap_cond = false;
        while(j >= 0) {
            if(ch == 1)
                swap_cond = flights[idx[j]].baseFare > flights[key].baseFare;
            else if(ch == 2)
                swap_cond = flights[idx[j]].departureTime > flights[key].departureTime;
            else
                swap_cond = flights[idx[j]].destination > flights[key].destination;

            if(swap_cond) { idx[j+1] = idx[j]; j--; }
            else break;
        }
        idx[j+1] = key;
    }

    cout << "\n--- Sorted Flights ---\n";
    cout << left << setw(10) << "Flight"
         << setw(14) << "Destination"
         << setw(8)  << "Depart"
         << setw(10) << "Fare(PKR)" << "\n";
    printLine('-');
    for(int i : idx) {
        cout << left
             << setw(10) << flights[i].flightNumber
             << setw(14) << flights[i].destination
             << setw(8)  << flights[i].departureTime
             << setw(10) << fixed << setprecision(0) << flights[i].baseFare
             << "\n";
    }
    pauseScreen();
}
