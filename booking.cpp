// booking.cpp  –  Book seat, cancel, VIP booking, family booking
#include "globals.h"

// Forward declarations from passenger.cpp
Passenger* createPassenger(const string&, const string&, int, Gender,
                           const string&, int, SeatClass, bool);

// ============================================================
//  HELPER: Collect passenger info from user
// ============================================================
static void inputPassengerInfo(string& name, string& cnic, int& age,
                               Gender& gender, SeatClass& sc, bool& isVIP) {
    cin.ignore();
    cout << "Passenger Name    : "; getline(cin, name);
    cout << "CNIC / Passport   : "; cin >> cnic;
    cout << "Age               : "; cin >> age;
    cout << "Gender (1=M,2=F,3=Other): "; int g; cin >> g;
    gender = (g == 2) ? FEMALE : (g == 3 ? OTHER : MALE);
    cout << "Seat Class (1=Economy, 2=Business, 3=First Class): ";
    int s; cin >> s;
    sc = (s == 2) ? BUSINESS : (s == 3 ? FIRST_CLASS : ECONOMY);
    cout << "VIP passenger? (1=Yes, 0=No): "; int v; cin >> v;
    isVIP = (v == 1);
}

// ============================================================
//  HELPER: Find first available seat matching class
//          Returns seat index (0-based) or -1
// ============================================================
static int findAvailableSeat(int flightIdx, SeatClass sc) {
    Flight& f = flights[flightIdx];
    // Try requested class first
    for(int s = 0; s < f.totalSeats; s++)
        if(!f.seats[s].isBooked && f.seats[s].seatClass == sc)
            return s;
    // Fallback: any available seat
    for(int s = 0; s < f.totalSeats; s++)
        if(!f.seats[s].isBooked)
            return s;
    return -1;
}

// ============================================================
//  PROCESS WAITING LIST after a seat becomes free
// ============================================================
void processWaitingList(int flightIdx) {
    Flight& f = flights[flightIdx];
    if(waitingList.empty()) return;

    // Collect only entries for this flight
    priority_queue<WaitingEntry> temp;
    WaitingEntry best;
    bool foundOne = false;

    // Drain the queue, find highest-priority entry for this flight
    vector<WaitingEntry> others;
    while(!waitingList.empty()) {
        WaitingEntry e = waitingList.top(); waitingList.pop();
        if(!foundOne && e.flightNumber == f.flightNumber) {
            best     = e;
            foundOne = true;
        } else {
            others.push_back(e);
        }
    }
    // Restore non-selected entries
    for(auto& oe : others) waitingList.push(oe);

    if(!foundOne) return;

    // Assign a seat
    int seatIdx = findAvailableSeat(flightIdx, ECONOMY);
    if(seatIdx == -1) return;

    f.seats[seatIdx].isBooked    = true;
    f.seats[seatIdx].passengerId = best.passengerId;
    f.bookedSeats++;

    // Update passenger record
    Passenger* p = findPassengerById(best.passengerId);
    if(p) {
        p->seatNumber   = f.seats[seatIdx].number;
        p->seatClass    = f.seats[seatIdx].seatClass;
        p->confirmed    = false;        // must confirm within 10 min
        p->upgradeTime  = time(nullptr);
    }

    string msg = best.passengerName + " has been upgraded from waiting list to confirmed seat #"
                 + to_string(f.seats[seatIdx].number) + " on flight " + f.flightNumber + ".";
    addNotification(msg);
    cout << "\n " << msg << "\n";
}

// ============================================================
//  BOOK SEAT
// ============================================================
void bookSeat() {
    printHeader("BOOK A SEAT");

    string flightNo;
    cout << "Enter Flight Number: "; cin >> flightNo;

    int idx = getFlightIndex(flightNo);
    if(idx == -1) { cout << "Flight not found!\n"; pauseScreen(); return; }

    Flight& f = flights[idx];

    string name, cnic;
    int age;
    Gender gender;
    SeatClass sc;
    bool isVIP;
    inputPassengerInfo(name, cnic, age, gender, sc, isVIP);

    // Prevent double booking by CNIC
    Passenger* existing = findPassengerByCNIC(cnic);
    if(existing && existing->flightNumber == flightNo) {
        cout << "This passenger is already booked on flight " << flightNo << "!\n";
        pauseScreen(); return;
    }

    if(f.bookedSeats >= f.totalSeats) {
        // Flight full → add to waiting list
        cout << "\n⚠ Flight is FULL. Adding " << name << " to the waiting list...\n";

        // Create passenger record with no seat (-1)
        Passenger* p = createPassenger(name, cnic, age, gender, flightNo, -1, sc, isVIP);

        WaitingEntry we;
        we.passengerId   = p->id;
        we.passengerName = name;
        we.flightNumber  = flightNo;
        we.isVIP         = isVIP;
        we.priority      = isVIP ? 10 : 1;
        we.requestTime   = time(nullptr);
        waitingList.push(we);

        addNotification(name + " added to waiting list for flight " + flightNo + ".");
        cout << "Waiting list position assigned. Passenger ID: " << p->id << "\n";
    } else {
        // Book a seat
        int seatIdx = findAvailableSeat(idx, sc);
        if(seatIdx == -1) {
            cout << "No seats available in that class. No seats at all!\n";
            pauseScreen(); return;
        }

        f.seats[seatIdx].isBooked    = true;
        f.bookedSeats++;

        Passenger* p = createPassenger(name, cnic, age, gender, flightNo,
                                       f.seats[seatIdx].number,
                                       f.seats[seatIdx].seatClass, isVIP);
        f.seats[seatIdx].passengerId = p->id;

        addNotification("Seat #" + to_string(f.seats[seatIdx].number) +
                        " booked for " + name + " on flight " + flightNo + ".");
        cout << "\n Booking confirmed!\n";
        cout << "  Seat Number : " << p->seatNumber << "\n";
        cout << "  Class       : " << seatClassName(p->seatClass) << "\n";
        cout << "  Passenger ID: " << p->id << "\n";
    }
    pauseScreen();
}

// ============================================================
//  CANCEL BOOKING
// ============================================================
void cancelBooking() {
    printHeader("CANCEL BOOKING");

    int id; cout << "Enter Passenger ID to cancel: "; cin >> id;
    Passenger* p = findPassengerById(id);
    if(!p) { cout << "Passenger not found!\n"; pauseScreen(); return; }

    if(p->seatNumber == -1) {
        cout << "This passenger is on the waiting list, not yet seated.\n";
        pauseScreen(); return;
    }

    int idx = getFlightIndex(p->flightNumber);
    if(idx == -1) { cout << "Associated flight not found!\n"; pauseScreen(); return; }

    Flight& f = flights[idx];
    int seatIdx = p->seatNumber - 1;

    if(seatIdx >= 0 && seatIdx < f.totalSeats) {
        f.seats[seatIdx].isBooked    = false;
        f.seats[seatIdx].passengerId = 0;
        f.bookedSeats--;
    }

    addNotification("Booking cancelled for " + p->name +
                    " (Seat #" + to_string(p->seatNumber) + ", Flight " + p->flightNumber + ").");

    p->seatNumber = -1;   // mark as no-seat

    cout << " Booking cancelled.\n";

    // Try auto-upgrade from waiting list
    processWaitingList(idx);

    pauseScreen();
}

// ============================================================
//  VIP BOOKING  (separate flow with VIP flag forced)
// ============================================================
void vipBooking() {
    printHeader("VIP BOOKING");

    string flightNo;
    cout << "Enter Flight Number: "; cin >> flightNo;

    int idx = getFlightIndex(flightNo);
    if(idx == -1) { cout << "Flight not found!\n"; pauseScreen(); return; }

    Flight& f = flights[idx];

    string name, cnic;
    int age;
    Gender gender;
    SeatClass sc;
    (void)0;   // VIP flag is forced true inline below

    cin.ignore();
    cout << "Passenger Name    : "; getline(cin, name);
    cout << "CNIC / Passport   : "; cin >> cnic;
    cout << "Age               : "; cin >> age;
    cout << "Gender (1=M,2=F,3=Other): "; int g; cin >> g;
    gender = (g == 2) ? FEMALE : (g == 3 ? OTHER : MALE);
    cout << "Preferred Class (1=Economy, 2=Business, 3=First Class): ";
    int s; cin >> s;
    sc = (s == 2) ? BUSINESS : (s == 3 ? FIRST_CLASS : ECONOMY);

    // Double-booking check
    Passenger* existing = findPassengerByCNIC(cnic);
    if(existing && existing->flightNumber == flightNo) {
        cout << "This passenger is already booked on this flight!\n";
        pauseScreen(); return;
    }

    if(f.bookedSeats >= f.totalSeats) {
        cout << "\n⚠ Flight full. Adding VIP " << name << " to PRIORITY waiting list.\n";

        Passenger* p = createPassenger(name, cnic, age, gender, flightNo, -1, sc, true);

        WaitingEntry we;
        we.passengerId   = p->id;
        we.passengerName = name;
        we.flightNumber  = flightNo;
        we.isVIP         = true;
        we.priority      = 10;
        we.requestTime   = time(nullptr);
        waitingList.push(we);

        addNotification("VIP " + name + " added to priority waiting list for flight " + flightNo);
        cout << " VIP placed in PRIORITY waiting list. Passenger ID: " << p->id << "\n";
    } else {
        // Prefer First Class for VIP
        int seatIdx = findAvailableSeat(idx, FIRST_CLASS);
        if(seatIdx == -1) seatIdx = findAvailableSeat(idx, sc);
        if(seatIdx == -1) { cout << "No seats available!\n"; pauseScreen(); return; }

        f.seats[seatIdx].isBooked    = true;
        f.bookedSeats++;

        Passenger* p = createPassenger(name, cnic, age, gender, flightNo,
                                       f.seats[seatIdx].number,
                                       f.seats[seatIdx].seatClass, true);
        f.seats[seatIdx].passengerId = p->id;

        addNotification("VIP Seat #" + to_string(p->seatNumber) +
                        " booked for " + name + " on flight " + flightNo);
        cout << "\n VIP Booking confirmed!\n";
        cout << "   Seat   : " << p->seatNumber << "\n";
        cout << "   Class  : " << seatClassName(p->seatClass) << "\n";
        cout << "   ID     : " << p->id << "\n";
    }
    pauseScreen();
}

// ============================================================
//  FAMILY GROUP BOOKING
// ============================================================
void familyBooking() {
    printHeader("FAMILY GROUP BOOKING");

    string flightNo;
    cout << "Enter Flight Number: "; cin >> flightNo;

    int idx = getFlightIndex(flightNo);
    if(idx == -1) { cout << "Flight not found!\n"; pauseScreen(); return; }

    Flight& f = flights[idx];

    int count;
    cout << "Number of family members (max " << MAX_FAMILY << "): "; cin >> count;
    if(count < 1 || count > MAX_FAMILY) { cout << "Invalid count.\n"; pauseScreen(); return; }

    int available = f.totalSeats - f.bookedSeats;
    if(available < count) {
        cout << "Only " << available << " seats available. Cannot seat all " << count << " members together.\n";
        pauseScreen(); return;
    }

    // Find 'count' consecutive available seats (arrays + linear search)
    int startSeat = -1;
    for(int s = 0; s <= f.totalSeats - count; s++) {
        bool allFree = true;
        for(int k = 0; k < count; k++) {
            if(f.seats[s + k].isBooked) { allFree = false; break; }
        }
        if(allFree) { startSeat = s; break; }
    }

    if(startSeat == -1) {
        cout << "Could not find " << count << " consecutive seats.\n";
        cout << "Booking members in any available seats...\n\n";
    }

    cout << "\nEnter details for each family member:\n";
    for(int m = 0; m < count; m++) {
        cout << "\n--- Member " << (m+1) << " ---\n";
        string name, cnic;
        int age;
        Gender gender;

        cin.ignore();
        cout << "Name            : "; getline(cin, name);
        cout << "CNIC/Passport   : "; cin >> cnic;
        cout << "Age             : "; cin >> age;
        cout << "Gender (1=M,2=F,3=Other): "; int g; cin >> g;
        gender = (g == 2) ? FEMALE : (g == 3 ? OTHER : MALE);

        // Assign seat
        int seatIdx = (startSeat != -1) ? (startSeat + m) : findAvailableSeat(idx, ECONOMY);
        if(seatIdx == -1 || f.seats[seatIdx].isBooked)
            seatIdx = findAvailableSeat(idx, ECONOMY);

        if(seatIdx == -1) {
            cout << "No more seats available for member " << (m+1) << "!\n";
            break;
        }

        f.seats[seatIdx].isBooked = true;
        f.bookedSeats++;

        Passenger* p = createPassenger(name, cnic, age, gender, flightNo,
                                       f.seats[seatIdx].number,
                                       f.seats[seatIdx].seatClass, false);
        f.seats[seatIdx].passengerId = p->id;

        cout << "✅ " << name << " → Seat #" << p->seatNumber
             << " (" << seatClassName(p->seatClass) << ")  ID: " << p->id << "\n";
    }

    addNotification("Family group of " + to_string(count) +
                    " booked on flight " + flightNo + ".");
    pauseScreen();
}

// ============================================================
//  VIEW WAITING LIST
// ============================================================
void viewWaitingList() {
    printHeader("WAITING LIST");

    if(waitingList.empty()) {
        cout << "Waiting list is empty.\n";
        pauseScreen(); return;
    }

    // Copy to temp to display without destroying
    priority_queue<WaitingEntry> temp = waitingList;
    int pos = 1;
    cout << left << setw(5) << "Pos"
         << setw(20) << "Name"
         << setw(12) << "Flight"
         << setw(8)  << "VIP"
         << setw(10) << "Priority"
         << "\n";
    printLine('-');
    while(!temp.empty()) {
        WaitingEntry e = temp.top(); temp.pop();
        cout << left << setw(5) << pos++
             << setw(20) << e.passengerName
             << setw(12) << e.flightNumber
             << setw(8)  << (e.isVIP ? "YES" : "No")
             << setw(10) << e.priority
             << "\n";
    }
    pauseScreen();
}

// ============================================================
//  CHECK WAITING LIST EXPIRY (10-minute rule)
// ============================================================
void checkWaitingListExpiry() {
    printHeader("CHECK CONFIRMATION EXPIRY");

    const int TIMEOUT_SECS = 600; // 10 minutes
    time_t now = time(nullptr);
    bool found = false;

    Passenger* cur = passengerHead;
    while(cur) {
        if(!cur->confirmed && cur->upgradeTime != 0) {
            double elapsed = difftime(now, cur->upgradeTime);
            int remaining  = TIMEOUT_SECS - (int)elapsed;

            cout << "Passenger: " << cur->name << " | Flight: " << cur->flightNumber
                 << " | Seat #" << cur->seatNumber << "\n";

            if(remaining <= 0) {
                cout << "  ⌛ EXPIRED – Seat reassigned to next in queue.\n";

                // Free the seat
                int idx = getFlightIndex(cur->flightNumber);
                if(idx != -1 && cur->seatNumber > 0) {
                    int s = cur->seatNumber - 1;
                    flights[idx].seats[s].isBooked    = false;
                    flights[idx].seats[s].passengerId = 0;
                    flights[idx].bookedSeats--;
                    processWaitingList(idx);
                }
                cur->seatNumber = -1;
            } else {
                cout << "  ⏳ " << remaining << " seconds left to confirm.\n";
            }
            found = true;
        }
        cur = cur->next;
    }

    if(!found) cout << "No pending confirmations.\n";
    pauseScreen();
}
