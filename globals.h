#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <sstream>
using namespace std;

// ============================================================
//  CONSTANTS
// ============================================================
const int MAX_FLIGHTS   = 20;
const int MAX_SEATS     = 60;   // seats per flight
const int MAX_FAMILY    = 10;

// ============================================================
//  ENUMERATIONS
// ============================================================
enum SeatClass  { ECONOMY = 1, BUSINESS, FIRST_CLASS };
enum FlightType { DOMESTIC = 1, INTERNATIONAL };
enum Gender     { MALE = 1, FEMALE, OTHER };

// ============================================================
//  PASSENGER NODE  (Linked List)
// ============================================================
struct Passenger {
    int    id;
    string name;
    string cnic;          // CNIC or Passport
    int    age;
    Gender gender;
    string flightNumber;
    int    seatNumber;
    SeatClass seatClass;
    bool   isVIP;
    bool   confirmed;     // confirmed after waiting-list upgrade
    time_t upgradeTime;   // when the upgrade offer was made
    Passenger* next;

    Passenger() : id(0), age(0), gender(MALE), seatNumber(-1),
                  seatClass(ECONOMY), isVIP(false), confirmed(true),
                  upgradeTime(0), next(nullptr) {}
};

// ============================================================
//  SEAT
// ============================================================
struct Seat {
    int    number;
    bool   isBooked;
    int    passengerId;   // 0 = empty
    SeatClass seatClass;

    Seat() : number(0), isBooked(false), passengerId(0), seatClass(ECONOMY) {}
};

// ============================================================
//  FLIGHT
// ============================================================
struct Flight {
    string flightNumber;
    string origin;
    string destination;
    string departureTime;
    string arrivalTime;
    string date;
    FlightType type;
    int    totalSeats;
    int    bookedSeats;
    double baseFare;      // base fare in PKR
    Seat   seats[MAX_SEATS];
    bool   active;

    Flight() : type(DOMESTIC), totalSeats(50), bookedSeats(0),
               baseFare(5000.0), active(false) {}
};

// ============================================================
//  WAITING LIST ENTRY
// ============================================================
struct WaitingEntry {
    int    passengerId;
    string passengerName;
    string flightNumber;
    bool   isVIP;
    int    priority;      // higher = served first; VIP gets 10, normal 1
    time_t requestTime;

    // For priority_queue (max-heap)
    bool operator<(const WaitingEntry& o) const {
        return priority < o.priority;   // lower priority = popped later
    }
};

// ============================================================
//  NOTIFICATION
// ============================================================
struct Notification {
    string message;
    time_t timestamp;
};

// ============================================================
//  GLOBAL STORES  (defined in main.cpp)
// ============================================================
extern Flight       flights[MAX_FLIGHTS];
extern int          flightCount;
extern Passenger*   passengerHead;       // head of linked list
extern int          passengerIdCounter;
extern priority_queue<WaitingEntry> waitingList;   // VIP-aware
extern vector<Notification> notifications;

// ============================================================
//  UTILITY HELPERS
// ============================================================
string currentTimestamp();
string seatClassName(SeatClass sc);
string flightTypeName(FlightType ft);
string genderName(Gender g);
void   printLine(char c = '-', int w = 60);
void   printHeader(const string& title);
void   pauseScreen();
int    getFlightIndex(const string& flightNo);
Passenger* findPassengerById(int id);
Passenger* findPassengerByCNIC(const string& cnic);
void   addNotification(const string& msg);

#endif // GLOBALS_H
