#include "../include/Payment.h"

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

// ================= CashPayment =================

bool CashPayment::processPayment(double amount)
{
    cout << fixed << setprecision(2);
    cout << "Processing payment of Rupees " << amount << endl;

    this_thread::sleep_for(chrono::milliseconds(500));

    cout << "Cash payment successful!" << endl;
    return true;
}

// ================= CardPayment =================

bool CardPayment::processPayment(double amount)
{
    cout << fixed << setprecision(2);
    cout << "Processing payment of Rupees " << amount << endl;

    this_thread::sleep_for(chrono::milliseconds(500));

    cout << "Card payment successful!" << endl;
    return true;
}
