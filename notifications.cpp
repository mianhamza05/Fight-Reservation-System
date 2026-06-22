// notifications.cpp  –  Notification display and management
#include "globals.h"

// ============================================================
//  VIEW ALL NOTIFICATIONS
// ============================================================
void viewNotifications() {
    printHeader("SYSTEM NOTIFICATIONS");

    if(notifications.empty()) {
        cout << "No notifications yet.\n";
        pauseScreen(); return;
    }

    cout << left << setw(5) << "No."
         << setw(22) << "Timestamp"
         << "Message\n";
    printLine('-');

    for(int i = (int)notifications.size() - 1; i >= 0; i--) {
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S",
                 localtime(&notifications[i].timestamp));
        cout << left << setw(5) << (notifications.size() - i)
             << setw(22) << buf
             << notifications[i].message << "\n";
    }

    cout << "\nTotal notifications: " << notifications.size() << "\n";
    pauseScreen();
}

// ============================================================
//  CLEAR NOTIFICATIONS
// ============================================================
void clearNotifications() {
    printHeader("CLEAR NOTIFICATIONS");
    cout << "Are you sure you want to clear all notifications? (1=Yes, 0=No): ";
    int ch; cin >> ch;
    if(ch == 1) {
        notifications.clear();
        cout << " All notifications cleared.\n";
    } else {
        cout << "Operation cancelled.\n";
    }
    pauseScreen();
}
