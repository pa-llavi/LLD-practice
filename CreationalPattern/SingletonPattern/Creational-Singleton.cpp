#include<iostream>
using namespace std;

class Logger {
    Logger() {
        cout<<"Logger instance created...\n";
    }

    //delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const string& message) {
        cout<<"[LOG]: "<<message<<endl;
    }

    ~Logger() {
        cout<<"Logger destroyed!\n";
    }
};

int main() {
    Logger& logger1 = Logger::getInstance();
    Logger& logger2 = Logger::getInstance();

    logger1.log("First message");
    logger2.log("Second message");

    //if same instance or not
    cout<<"Same instance? "<<(&logger1==&logger2 ? "Yes" : "No")<<endl;

    return 0;
}