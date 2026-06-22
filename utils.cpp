// utils.cpp  –  Helper / utility functions
#include "globals.h"

// --------------------------------------------------------
string currentTimestamp() {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

// --------------------------------------------------------
string seatClassName(SeatClass sc) {
    switch(sc) {
        case ECONOMY:     return "Economy";
        case BUSINESS:    return "Business";
        case FIRST_CLASS: return "First Class";
    }
    return "Unknown";
}

// --------------------------------------------------------
string flightTypeName(FlightType ft) {
    return (ft == DOMESTIC) ? "Domestic" : "International";
}

// --------------------------------------------------------
string genderName(Gender g) {
    switch(g) {
        case MALE:   return "Male";
        case FEMALE: return "Female";
        case OTHER:  return "Other";
    }
    return "Unknown";
}

// --------------------------------------------------------
void printLine(char c, int w) {
    cout << string(w, c) << "\n";
}

// --------------------------------------------------------
void printHeader(const string& title) {
    printLine('=');
    int padding = (60 - (int)title.size()) / 2;
    if(padding < 0) padding = 0;
    cout << string(padding, ' ') << title << "\n";
    printLine('=');
}

// --------------------------------------------------------
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(1000, '\n');
    cin.get();
}

// --------------------------------------------------------
// Returns index in flights[] or -1 if not found
int getFlightIndex(const string& flightNo) {
    for(int i = 0; i < flightCount; i++) {
        if(flights[i].active && flights[i].flightNumber == flightNo)
            return i;
    }
    return -1;
}

// --------------------------------------------------------
Passenger* findPassengerById(int id) {
    Passenger* cur = passengerHead;
    while(cur) {
        if(cur->id == id) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// --------------------------------------------------------
Passenger* findPassengerByCNIC(const string& cnic) {
    Passenger* cur = passengerHead;
    while(cur) {
        if(cur->cnic == cnic) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// --------------------------------------------------------
void addNotification(const string& msg) {
    Notification n;
    n.message   = msg;
    n.timestamp = time(nullptr);
    notifications.push_back(n);
    cout << "\n[NOTIFICATION] " << msg << "\n";
}
