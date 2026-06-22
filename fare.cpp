// fare.cpp  –  Fare calculation and bill generation
#include "globals.h"

// ============================================================
//  CALCULATE FARE
// ============================================================
void calculateFare() {
    printHeader("FARE CALCULATION");

    string flightNo;
    cout << "Enter Flight Number: "; cin >> flightNo;

    int idx = getFlightIndex(flightNo);
    if(idx == -1) { cout << "Flight not found!\n"; pauseScreen(); return; }

    Flight& f = flights[idx];

    cout << "\nSeat Class:\n";
    cout << "  1. Economy     (base fare)\n";
    cout << "  2. Business    (base x 2.0)\n";
    cout << "  3. First Class (base x 3.5)\n";
    cout << "Choice: ";
    int cls; cin >> cls;
    SeatClass sc = (cls == 2) ? BUSINESS : (cls == 3 ? FIRST_CLASS : ECONOMY);

    cout << "VIP passenger? (1=Yes, 0=No): ";
    int vip; cin >> vip;

    // Multipliers
    double classMultiplier = (sc == FIRST_CLASS) ? 3.5 :
                             (sc == BUSINESS)    ? 2.0 : 1.0;
    double typeMultiplier  = (f.type == INTERNATIONAL) ? 2.5 : 1.0;
    double vipDiscount     = (vip == 1) ? 0.90 : 1.0;  // 10% discount for VIP

    double fare = f.baseFare * classMultiplier * typeMultiplier * vipDiscount;

    double tax  = fare * 0.15;  // 15% government tax
    double total = fare + tax;

    printLine('=');
    cout << "FARE BREAKDOWN\n";
    printLine('-');
    cout << "Flight          : " << f.flightNumber   << "\n";
    cout << "Route           : " << f.origin << " → " << f.destination << "\n";
    cout << "Flight Type     : " << flightTypeName(f.type) << "\n";
    cout << "Seat Class      : " << seatClassName(sc) << "\n";
    cout << "Base Fare       : PKR " << fixed << setprecision(2) << f.baseFare << "\n";
    cout << "Class Multiplier: x" << classMultiplier << "\n";
    cout << "Route Multiplier: x" << typeMultiplier  << "\n";
    if(vip == 1)
    cout << "VIP Discount    : -10%\n";
    cout << "Subtotal        : PKR " << fixed << setprecision(2) << fare  << "\n";
    cout << "Tax (15%)       : PKR " << fixed << setprecision(2) << tax   << "\n";
    printLine('-');
    cout << "TOTAL FARE      : PKR " << fixed << setprecision(2) << total << "\n";
    printLine('=');

    pauseScreen();
}

// ============================================================
//  GENERATE PASSENGER BILL
// ============================================================
void generateBill() {
    printHeader("GENERATE PASSENGER BILL");

    int id; cout << "Enter Passenger ID: "; cin >> id;
    Passenger* p = findPassengerById(id);
    if(!p) { cout << "Passenger not found!\n"; pauseScreen(); return; }

    int idx = getFlightIndex(p->flightNumber);
    if(idx == -1) { cout << "Flight not found!\n"; pauseScreen(); return; }

    Flight& f = flights[idx];

    double classMultiplier = (p->seatClass == FIRST_CLASS) ? 3.5 :
                             (p->seatClass == BUSINESS)    ? 2.0 : 1.0;
    double typeMultiplier  = (f.type == INTERNATIONAL) ? 2.5 : 1.0;
    double vipDiscount     = p->isVIP ? 0.90 : 1.0;

    double fare  = f.baseFare * classMultiplier * typeMultiplier * vipDiscount;
    double tax   = fare * 0.15;
    double total = fare + tax;

    printLine('=');
    cout << "          AIRLINE BOOKING RECEIPT\n";
    printLine('=');
    cout << "Date             : " << currentTimestamp() << "\n";
    printLine('-');
    cout << "Passenger Name   : " << p->name           << "\n";
    cout << "CNIC/Passport    : " << p->cnic            << "\n";
    cout << "Passenger ID     : " << p->id              << "\n";
    cout << "Flight Number    : " << p->flightNumber    << "\n";
    cout << "Route            : " << f.origin << " → " << f.destination << "\n";
    cout << "Date             : " << f.date             << "\n";
    cout << "Departure        : " << f.departureTime    << "\n";
    cout << "Seat Number      : " << p->seatNumber      << "\n";
    cout << "Seat Class       : " << seatClassName(p->seatClass) << "\n";
    cout << "VIP              : " << (p->isVIP ? "Yes" : "No") << "\n";
    printLine('-');
    cout << "Base Fare        : PKR " << fixed << setprecision(2) << f.baseFare << "\n";
    cout << "Subtotal         : PKR " << fixed << setprecision(2) << fare  << "\n";
    cout << "Tax (15%)        : PKR " << fixed << setprecision(2) << tax   << "\n";
    printLine('-');
    cout << "TOTAL            : PKR " << fixed << setprecision(2) << total << "\n";
    printLine('=');
    cout << "     Thank you for flying with us! ✈\n";
    printLine('=');

    pauseScreen();
}
