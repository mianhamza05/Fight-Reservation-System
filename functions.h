#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// ── flight.cpp ──────────────────────────────────────────────
void addFlight();
void viewFlights();
void removeFlight();
void updateFlightSchedule();
void showFlightSchedule();
void sortFlights();

// ── passenger.cpp ───────────────────────────────────────────
void viewPassengerRecords();
void searchPassenger();
void updatePassenger();
void deletePassenger();

// ── booking.cpp ─────────────────────────────────────────────
void bookSeat();
void cancelBooking();
void vipBooking();
void familyBooking();
void viewWaitingList();
void checkWaitingListExpiry();
void processWaitingList(int flightIdx);

// ── fare.cpp ────────────────────────────────────────────────
void calculateFare();
void generateBill();

// ── notifications.cpp ───────────────────────────────────────
void viewNotifications();
void clearNotifications();

#endif // FUNCTIONS_H
