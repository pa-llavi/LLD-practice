#pragma once

class PaymentProcessor
{
public:
    virtual ~PaymentProcessor() = default;
    virtual bool processPayment(double amount) = 0;
};

class CashPayment : public PaymentProcessor
{
public:
    bool processPayment(double amount) override;
};

class CardPayment : public PaymentProcessor
{
public:
    bool processPayment(double amount) override;
};
