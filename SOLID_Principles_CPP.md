# SOLID Principles - C++ Examples 🎯

SOLID is an acronym for five design principles that make software designs more understandable, flexible, and maintainable.

---

## 1. **S - Single Responsibility Principle (SRP)** 

### Definition
> "A class should have only ONE reason to change" - Each class should have only ONE job or responsibility.

### Why it matters
When a class does multiple things, changes to one responsibility can affect the others, making the code fragile and hard to maintain.

### ❌ Bad Example (Violates SRP)

```cpp
#include <iostream>
#include <string>
using namespace std;

class User {
private:
    string name;
    string email;
    
public:
    // Responsibility 1: User data management
    void setName(const string& n) {
        name = n;
    }
    
    string getName() const {
        return name;
    }
    
    void setEmail(const string& e) {
        email = e;
    }
    
    string getEmail() const {
        return email;
    }
    
    // Responsibility 2: Validation
    bool validateEmail() {
        return email.find("@") != string::npos;
    }
    
    // Responsibility 3: Database operations
    void saveToDatabase() {
        cout << "Saving " << name << " to database..." << endl;
    }
    
    // Responsibility 4: Email notifications
    void sendWelcomeEmail() {
        cout << "Sending welcome email to " << email << endl;
    }
};
```

**Problem:** This class has 4 different responsibilities! If email validation rules change, or database logic changes, or email service changes - we have to modify the User class.

### ✅ Good Example (Follows SRP)

```cpp
#include <iostream>
#include <string>
using namespace std;

// Responsibility 1: User data management ONLY
class User {
private:
    string name;
    string email;
    
public:
    User(const string& n, const string& e) : name(n), email(e) {}
    
    string getName() const { return name; }
    void setName(const string& n) { name = n; }
    
    string getEmail() const { return email; }
    void setEmail(const string& e) { email = e; }
};

// Responsibility 2: Validation
class UserValidator {
public:
    bool validateEmail(const string& email) {
        return !email.empty() && email.find("@") != string::npos;
    }
    
    bool validateName(const string& name) {
        return !name.empty() && name.length() > 0;
    }
};

// Responsibility 3: Database operations
class UserRepository {
public:
    void save(const User& user) {
        cout << "Saving user " << user.getName() << " to database..." << endl;
    }
    
    User* findByEmail(const string& email) {
        // Database query logic
        return nullptr;
    }
};

// Responsibility 4: Email notifications
class EmailService {
public:
    void sendWelcomeEmail(const User& user) {
        cout << "Sending welcome email to: " << user.getEmail() << endl;
    }
};

// Usage
int main() {
    User user("John Doe", "john@example.com");
    
    UserValidator validator;
    if (validator.validateEmail(user.getEmail())) {
        UserRepository repo;
        repo.save(user);
        
        EmailService emailService;
        emailService.sendWelcomeEmail(user);
    }
    
    return 0;
}
```

**Benefit:** Now each class has ONE clear purpose. Changes are isolated!

---

## 2. **O - Open/Closed Principle (OCP)**

### Definition
> "Software entities should be OPEN for extension but CLOSED for modification"

You should be able to add new functionality WITHOUT changing existing code.

### Why it matters
Modifying existing code can introduce bugs. It's safer to extend functionality through new code.

### ❌ Bad Example (Violates OCP)

```cpp
#include <iostream>
#include <string>
using namespace std;

class PaymentProcessor {
public:
    void processPayment(const string& paymentType, double amount) {
        if (paymentType == "CreditCard") {
            cout << "Processing credit card payment: $" << amount << endl;
            // Credit card logic
        } 
        else if (paymentType == "PayPal") {
            cout << "Processing PayPal payment: $" << amount << endl;
            // PayPal logic
        }
        else if (paymentType == "Bitcoin") {
            cout << "Processing Bitcoin payment: $" << amount << endl;
            // Bitcoin logic
        }
        // Every new payment method requires modifying this class!
    }
};

int main() {
    PaymentProcessor processor;
    processor.processPayment("CreditCard", 100.0);
    processor.processPayment("PayPal", 200.0);
    return 0;
}
```

**Problem:** Every time you add a new payment method (Google Pay, Apple Pay), you must MODIFY this class.

### ✅ Good Example (Follows OCP)

```cpp
#include <iostream>
#include <memory>
using namespace std;

// Abstract interface - closed for modification
class PaymentMethod {
public:
    virtual void processPayment(double amount) = 0;
    virtual ~PaymentMethod() = default;
};

// Open for extension - add new payment types without changing existing code
class CreditCardPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        cout << "Processing credit card payment: $" << amount << endl;
    }
};

class PayPalPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        cout << "Processing PayPal payment: $" << amount << endl;
    }
};

class BitcoinPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        cout << "Processing Bitcoin payment: $" << amount << endl;
    }
};

// New payment method - just add a new class!
class GooglePayPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        cout << "Processing Google Pay payment: $" << amount << endl;
    }
};

// Payment processor doesn't need to change
class PaymentProcessor {
public:
    void process(PaymentMethod* paymentMethod, double amount) {
        paymentMethod->processPayment(amount);
    }
};

int main() {
    PaymentProcessor processor;
    
    // Use different payment methods
    CreditCardPayment creditCard;
    processor.process(&creditCard, 100.0);
    
    PayPalPayment paypal;
    processor.process(&paypal, 200.0);
    
    GooglePayPayment googlePay;
    processor.process(&googlePay, 300.0);
    
    return 0;
}
```

**Benefit:** Add new payment methods by creating new classes, not modifying existing ones!

---

## 3. **L - Liskov Substitution Principle (LSP)**

### Definition
> "Objects of a superclass should be replaceable with objects of its subclasses without breaking the application"

If class B is a subtype of class A, we should be able to replace A with B without disrupting the behavior.

### Why it matters
Violating LSP leads to unexpected behavior and breaks polymorphism.

### ❌ Bad Example (Violates LSP)

```cpp
#include <iostream>
using namespace std;

class Rectangle {
protected:
    int width;
    int height;
    
public:
    Rectangle() : width(0), height(0) {}
    
    virtual void setWidth(int w) {
        width = w;
    }
    
    virtual void setHeight(int h) {
        height = h;
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    int getArea() const {
        return width * height;
    }
};

class Square : public Rectangle {
public:
    void setWidth(int w) override {
        width = w;
        height = w;  // Square must have equal sides
    }
    
    void setHeight(int h) override {
        width = h;
        height = h;  // Square must have equal sides
    }
};

// This breaks!
void testRectangle(Rectangle* rect) {
    rect->setWidth(5);
    rect->setHeight(4);
    cout << "Expected area: 20, Actual: " << rect->getArea() << endl;
    // If rect is a Square, area will be 16, not 20!
}

int main() {
    Rectangle rect;
    testRectangle(&rect);  // Works: area = 20
    
    Square square;
    testRectangle(&square);  // Breaks: area = 16 (expected 20)
    
    return 0;
}
```

**Problem:** Substituting Rectangle with Square breaks the expected behavior.

### ✅ Good Example (Follows LSP)

```cpp
#include <iostream>
using namespace std;

// Abstract base
class Shape {
public:
    virtual int getArea() const = 0;
    virtual ~Shape() = default;
};

class Rectangle : public Shape {
private:
    int width;
    int height;
    
public:
    Rectangle(int w, int h) : width(w), height(h) {}
    
    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    int getArea() const override {
        return width * height;
    }
};

class Square : public Shape {
private:
    int side;
    
public:
    Square(int s) : side(s) {}
    
    void setSide(int s) { side = s; }
    int getSide() const { return side; }
    
    int getArea() const override {
        return side * side;
    }
};

// Now both can be used without surprises
void printArea(Shape* shape) {
    cout << "Area: " << shape->getArea() << endl;
}

int main() {
    Rectangle rect(5, 4);
    Square square(4);
    
    printArea(&rect);    // Area: 20
    printArea(&square);  // Area: 16
    
    return 0;
}
```

**Benefit:** No inheritance relationship where it doesn't make sense. Both are just shapes with their own rules.

---

## 4. **I - Interface Segregation Principle (ISP)**

### Definition
> "No client should be forced to depend on methods it does not use"

Create specific, focused interfaces instead of one large, general-purpose interface.

### Why it matters
Large interfaces force classes to implement methods they don't need, leading to bloated and confusing code.

### ❌ Bad Example (Violates ISP)

```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

class Machine {
public:
    virtual void print() = 0;
    virtual void scan() = 0;
    virtual void fax() = 0;
    virtual void staple() = 0;
    virtual ~Machine() = default;
};

// Old printer can do everything
class MultiFunctionPrinter : public Machine {
public:
    void print() override { 
        cout << "Printing..." << endl; 
    }
    
    void scan() override { 
        cout << "Scanning..." << endl; 
    }
    
    void fax() override { 
        cout << "Faxing..." << endl; 
    }
    
    void staple() override { 
        cout << "Stapling..." << endl; 
    }
};

// Simple printer is forced to implement methods it doesn't support
class SimplePrinter : public Machine {
public:
    void print() override { 
        cout << "Printing..." << endl; 
    }
    
    void scan() override { 
        throw runtime_error("Scan not supported");
    }
    
    void fax() override { 
        throw runtime_error("Fax not supported");
    }
    
    void staple() override { 
        throw runtime_error("Staple not supported");
    }
};

int main() {
    SimplePrinter printer;
    printer.print();  // Works
    // printer.scan(); // Would throw exception!
    
    return 0;
}
```

**Problem:** SimplePrinter is forced to implement methods it doesn't support!

### ✅ Good Example (Follows ISP)

```cpp
#include <iostream>
using namespace std;

// Small, focused interfaces
class Printer {
public:
    virtual void print() = 0;
    virtual ~Printer() = default;
};

class Scanner {
public:
    virtual void scan() = 0;
    virtual ~Scanner() = default;
};

class Fax {
public:
    virtual void fax() = 0;
    virtual ~Fax() = default;
};

class Stapler {
public:
    virtual void staple() = 0;
    virtual ~Stapler() = default;
};

// SimplePrinter only implements what it needs
class SimplePrinter : public Printer {
public:
    void print() override {
        cout << "Printing..." << endl;
    }
};

// MultiFunctionPrinter implements multiple interfaces
class MultiFunctionPrinter : public Printer, public Scanner, public Fax, public Stapler {
public:
    void print() override { 
        cout << "Printing..." << endl; 
    }
    
    void scan() override { 
        cout << "Scanning..." << endl; 
    }
    
    void fax() override { 
        cout << "Faxing..." << endl; 
    }
    
    void staple() override { 
        cout << "Stapling..." << endl; 
    }
};

// Scanner-only device
class StandaloneScanner : public Scanner {
public:
    void scan() override {
        cout << "Scanning..." << endl;
    }
};

int main() {
    SimplePrinter simplePrinter;
    simplePrinter.print();
    
    MultiFunctionPrinter mfp;
    mfp.print();
    mfp.scan();
    mfp.fax();
    mfp.staple();
    
    StandaloneScanner scanner;
    scanner.scan();
    
    return 0;
}
```

**Benefit:** Each class implements only what it needs. No dummy implementations or exceptions!

---

## 5. **D - Dependency Inversion Principle (DIP)**

### Definition
> "High-level modules should not depend on low-level modules. Both should depend on abstractions (interfaces)"
> 
> "Abstractions should not depend on details. Details should depend on abstractions"

### Why it matters
Direct dependencies on concrete classes make code rigid and hard to test/change.

### ❌ Bad Example (Violates DIP)

```cpp
#include <iostream>
#include <string>
using namespace std;

// Low-level module
class MySQLDatabase {
public:
    void save(const string& data) {
        cout << "Saving to MySQL: " << data << endl;
    }
};

// High-level module directly depends on low-level concrete class
class UserService {
private:
    MySQLDatabase* database;  // Direct dependency on concrete class
    
public:
    UserService() {
        database = new MySQLDatabase();  // Tightly coupled
    }
    
    ~UserService() {
        delete database;
    }
    
    void saveUser(const string& userData) {
        database->save(userData);
    }
};

int main() {
    UserService service;
    service.saveUser("John Doe");
    // Can't easily switch to PostgreSQL or MongoDB!
    return 0;
}
```

**Problem:** 
- UserService is tightly coupled to MySQLDatabase
- Can't switch to PostgreSQL or MongoDB without changing UserService
- Hard to test - can't mock the database

### ✅ Good Example (Follows DIP)

```cpp
#include <iostream>
#include <string>
#include <memory>
using namespace std;

// Abstraction (interface)
class Database {
public:
    virtual void save(const string& data) = 0;
    virtual ~Database() = default;
};

// Low-level modules implement the abstraction
class MySQLDatabase : public Database {
public:
    void save(const string& data) override {
        cout << "Saving to MySQL: " << data << endl;
    }
};

class PostgreSQLDatabase : public Database {
public:
    void save(const string& data) override {
        cout << "Saving to PostgreSQL: " << data << endl;
    }
};

class MongoDatabase : public Database {
public:
    void save(const string& data) override {
        cout << "Saving to MongoDB: " << data << endl;
    }
};

// High-level module depends on abstraction
class UserService {
private:
    Database* database;  // Depends on interface, not concrete class
    
public:
    // Dependency injection via constructor
    UserService(Database* db) : database(db) {}
    
    void saveUser(const string& userData) {
        database->save(userData);
    }
};

int main() {
    // Easy to switch databases!
    MySQLDatabase mysqlDb;
    UserService mysqlService(&mysqlDb);
    mysqlService.saveUser("John Doe");
    
    // Switch to MongoDB without changing UserService
    MongoDatabase mongoDb;
    UserService mongoService(&mongoDb);
    mongoService.saveUser("Jane Doe");
    
    // Switch to PostgreSQL
    PostgreSQLDatabase postgresDb;
    UserService postgresService(&postgresDb);
    postgresService.saveUser("Bob Smith");
    
    return 0;
}
```

**Using Smart Pointers (Modern C++):**

```cpp
#include <iostream>
#include <string>
#include <memory>
using namespace std;

// Abstraction (interface)
class Database {
public:
    virtual void save(const string& data) = 0;
    virtual ~Database() = default;
};

// Implementations
class MySQLDatabase : public Database {
public:
    void save(const string& data) override {
        cout << "Saving to MySQL: " << data << endl;
    }
};

class MongoDatabase : public Database {
public:
    void save(const string& data) override {
        cout << "Saving to MongoDB: " << data << endl;
    }
};

// High-level module
class UserService {
private:
    shared_ptr<Database> database;
    
public:
    UserService(shared_ptr<Database> db) : database(db) {}
    
    void saveUser(const string& userData) {
        database->save(userData);
    }
};

int main() {
    // Using smart pointers for automatic memory management
    auto mysqlDb = make_shared<MySQLDatabase>();
    UserService mysqlService(mysqlDb);
    mysqlService.saveUser("John Doe");
    
    auto mongoDb = make_shared<MongoDatabase>();
    UserService mongoService(mongoDb);
    mongoService.saveUser("Jane Doe");
    
    return 0;
}
```

**Benefit:** 
- Loose coupling
- Easy to switch implementations
- Easy to test (can inject mock database)
- Flexible and maintainable

---

## 🎯 Quick Summary

| Principle | Key Question | Solution |
|-----------|-------------|----------|
| **SRP** | Does this class do too many things? | One class = One responsibility |
| **OCP** | Do I modify code to add features? | Extend through new classes, not modifications |
| **LSP** | Can I substitute child for parent safely? | Subtypes must be valid replacements |
| **ISP** | Are there unused methods in my interface? | Split into smaller, focused interfaces |
| **DIP** | Do I depend on concrete classes? | Depend on abstractions (interfaces) |

---

## 🌟 Real-World Analogy

Think of SOLID like organizing a restaurant:

- **SRP**: Chef cooks, waiter serves, cashier handles payment (not one person doing all)
- **OCP**: Add new menu items without changing kitchen equipment
- **LSP**: Any chef can work the station without breaking the workflow  
- **ISP**: Delivery driver doesn't need to know how to cook
- **DIP**: Menu depends on "food supplier" concept, not specific farm

---

## 💡 Benefits of Following SOLID

1. ✅ **Maintainable** - Easy to understand and modify
2. ✅ **Testable** - Can mock dependencies easily
3. ✅ **Flexible** - Easy to extend functionality
4. ✅ **Scalable** - Handles growth without major rewrites
5. ✅ **Readable** - Clear responsibilities and structure

---

## 🔧 C++ Specific Tips

1. **Use Virtual Destructors**: Always make destructors virtual in base classes
2. **Smart Pointers**: Use `shared_ptr`, `unique_ptr` for automatic memory management
3. **Abstract Classes**: Use pure virtual functions (`= 0`) for interfaces
4. **Multiple Inheritance**: C++ allows multiple inheritance for ISP (use carefully)
5. **RAII**: Follow Resource Acquisition Is Initialization principle
6. **Const Correctness**: Mark methods that don't modify state as `const`

---

## 📚 Practice Exercise

Try implementing these systems in C++ following SOLID principles:

1. **Logger System**
   - Console logger, File logger, Database logger
   - Apply: OCP, DIP

2. **Shape Calculator**
   - Different shapes with area calculation
   - Apply: SRP, LSP, OCP

3. **Notification System**
   - Email, SMS, Push notifications
   - Apply: ISP, DIP, OCP

4. **E-commerce Discount System**
   - Different discount strategies
   - Apply: OCP, SRP

---

Hope this helps! Practice applying these principles in your C++ code, and they'll become second nature! 🚀

