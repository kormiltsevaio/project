#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Интерфейс наблюдателя
class Observer {
public:
    virtual void update(const string& message) = 0; // Обновление данных
    virtual ~Observer() {}
};

// Интерфейс субъекта (класс, который уведомляет наблюдателей)
class Subject {
public:
    virtual void addObserver(Observer* observer) = 0;
    virtual void removeObserver(Observer* observer) = 0;
    virtual void notifyObservers(const string& message) = 0;
    virtual ~Subject() {}
};

// Общий интерфейс для авиакомпаний
class Airline {
public:
    virtual ~Airline() {}

    virtual string searchFlights(const string& fromCity, const string& toCity) = 0; // Поиск рейсов
    virtual string bookTicket(const string& flightId) = 0; // Бронирование билета
    virtual string refundTicket(const string& ticketId) = 0; // Возврат билета
};

// Авиакомпания A
class AirlineA : public Airline {
public:
    string searchFlights(const string& fromCity, const string& toCity) {
        return "Поиск рейсов в Airline A из " + fromCity + " в " + toCity;
    }

    string bookTicket(const string& flightId) {
        return "Бронирование билета на рейс " + flightId + " в Airline A";
    }

    string refundTicket(const string& ticketId) {
        return "Возврат билета с номером " + ticketId + " в Airline A";
    }
};

// Авиакомпания B
class AirlineB : public Airline {
public:
    string searchFlights(const string& fromCity, const string& toCity) {
        return "Поиск рейсов в Airline B из " + fromCity + " в " + toCity;
    }

    string bookTicket(const string& flightId) {
        return "Бронирование билета на рейс " + flightId + " в Airline B";
    }

    string refundTicket(const string& ticketId) {
        return "Возврат билета с номером " + ticketId + " в Airline B";
    }
};

// Наблюдатель для уведомления пользователя
class UserObserver : public Observer {
public:
    void update(const string& message) {
        cout << "Уведомление пользователю: " << message << endl;
    }
};

// Наблюдатель для логирования событий
class LoggerObserver : public Observer {
public:
    void update(const string& message) {
        cout << "Логирование: " << message << endl;
    }
};

// Фасад, управляющий бронированием
class BookingFacade : public Subject {
private:
    AirlineA airlineA;
    AirlineB airlineB;
    vector<Observer*> observers; // Список наблюдателей

public:
    // Поиск и бронирование
    void searchAndBook(const string& fromCity, const string& toCity, const string& airlineChoice) {
        string message;
        if (airlineChoice == "A") {
            string flightInfo = airlineA.searchFlights(fromCity, toCity);
            cout << flightInfo << endl;
            string flightId = "A123";  
            string bookingInfo = airlineA.bookTicket(flightId);
            cout << bookingInfo << endl;
            message = "Бронирование успешно завершено в Airline A на рейс " + flightId;
        } else if (airlineChoice == "B") {
            string flightInfo = airlineB.searchFlights(fromCity, toCity);
            cout << flightInfo << endl;
            string flightId = "B456";  
            string bookingInfo = airlineB.bookTicket(flightId);
            cout << bookingInfo << endl;
            message = "Бронирование успешно завершено в Airline B на рейс " + flightId;
        } else {
            cout << "Неверный выбор авиакомпании" << endl;
            return;
        }
        
        // Уведомление всех наблюдателей
        notifyObservers(message);
    }

    // Возврат билета
    void refundTicket(const string& airlineChoice) {
        string userInput;
        cout << "Вы хотите вернуть билет? (Да/Нет): ";
        cin >> userInput;

        if (userInput == "Да" || userInput == "да") {
            cout << "Возврат билета..." << endl;
            string message;
            if (airlineChoice == "A") {
                message = airlineA.refundTicket("A123");
            } else if (airlineChoice == "B") {
                message = airlineB.refundTicket("B456");
            }
            cout << message << endl;
            // Уведомление всех наблюдателей
            notifyObservers(message);
        } else if (userInput == "Нет" || userInput == "нет") {
            cout << "Хорошего полёта!" << endl;
        } else {
            cout << "Неверный ввод. Пожалуйста, введите 'Да' или 'Нет'." << endl;
        }
    }

    // Реализация методов Subject
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void removeObserver(Observer* observer) {
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notifyObservers(const string& message) {
        for (int i = 0; i < observers.size(); ++i) {
            observers[i]->update(message); // Оповещение каждого наблюдателя
        }
    }
};

int main() {
    srand(time(0));  
    BookingFacade facade;
    
    // Создание наблюдателей
    UserObserver userObserver;
    LoggerObserver loggerObserver;

    // Добавление наблюдателей в фасад
    facade.addObserver(&userObserver);
    facade.addObserver(&loggerObserver);

    string fromCity, toCity, airlineChoice;
    cout << "Введите город отправления: ";
    cin >> fromCity;
    cout << "Введите город назначения: ";
    cin >> toCity;
    cout << "Выберите авиакомпанию (A или B): ";
    cin >> airlineChoice;

    // Поиск и бронирование
    facade.searchAndBook(fromCity, toCity, airlineChoice);

    // Возврат билета
    facade.refundTicket(airlineChoice);

    return 0;
}
