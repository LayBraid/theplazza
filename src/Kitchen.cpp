/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** Kitchen.cpp
*/

#include "Kitchen.hpp"

plazza::Kitchen::Kitchen(const pid_t &pid, const double &multiplier, const int &nbCooks, const int &restoreDuration) :
    _pid(pid),
    _multiplier(multiplier),
    _nbCooks(nbCooks),
    _restoreDuration(restoreDuration),
    _logger("log.log"),
    _timer(restoreDuration),
    _readQueue("/1" + std::to_string(pid), O_CREAT | O_RDONLY | O_NONBLOCK),
    _writeQueue("/2" + std::to_string(pid), O_CREAT | O_WRONLY | O_NONBLOCK),
    _cooks(),
    _orders(),
    _cookedOrders(),
    _inCooking(0),
    _mutexOrders(),
    _mutexCooked(),
    _mutexProtect(),
    _cv(),
    _isRunning(true),
    _dough(5),
    _tomato(5),
    _gruyere(5),
    _ham(5),
    _mushrooms(5),
    _steak(5),
    _eggplant(5),
    _goatCheese(5),
    _chiefLove(5)
{
    for (int i = 0; i < _nbCooks; i++) {
        auto cook = std::thread(&plazza::Kitchen::cookPizza, this, i);
        _cooks.emplace_back(std::move(cook));
        _cooksStatus.emplace_back(false);
    }
    _typeToString[plazza::Margarita] = "margarita";
    _typeToString[plazza::Regina] = "regina";
    _typeToString[plazza::Americana] = "americana";
    _typeToString[plazza::Fantasia] = "fantasia";
}

plazza::Kitchen::~Kitchen()
{
    for (int i = 0; i < _nbCooks; i++) {
        if (_cooks[i].joinable())
            _cooks[i].join();
    }
}

bool plazza::Kitchen::canCook(PizzaType type)
{
    if (type == plazza::Margarita) {
        std::lock_guard<std::mutex> lock(_mutexProtect);
        if (getDough() >= 1 && getTomato() >= 1 && getGruyere() >= 1)
            return true;
    }
    if (type == plazza::Regina) {
        std::lock_guard<std::mutex> lock(_mutexProtect);
        if (getDough() >= 1 && getTomato() >= 1 && getGruyere() >= 1 && getHam() >= 1 &&
            getMushrooms() >= 1)
            return true;
    }
    if (type == plazza::Americana) {
        std::lock_guard<std::mutex> lock(_mutexProtect);
        if (getDough() >= 1 && getTomato() >= 1 && getGruyere() >= 1 && getSteak() >= 1)
            return true;
    }
    if (type == plazza::Fantasia) {
        std::lock_guard<std::mutex> lock(_mutexProtect);
        if (getDough() >= 1 && getTomato() >= 1 && getEggplant() >= 1 && getGoatCheese() >= 1 &&
            getChiefLove() >= 1)
            return true;
    }
    return false;
}

void plazza::Kitchen::takeIngredients(PizzaType type)
{
    if (type == plazza::Margarita) {
        std::lock_guard<std::mutex> lock(_mutexProtect);
        setDough(getDough() - 1);
        setTomato(getTomato() - 1);
        setGruyere(getGruyere() - 1);
    }
    if (type == plazza::Regina) {
        std::lock_guard<std::mutex> lock(_mutexProtect);
        setDough(getDough() - 1);
        setTomato(getTomato() - 1);
        setGruyere(getGruyere() - 1);
        setHam(getHam() - 1);
        setMushrooms(getMushrooms() - 1);
    }
    if (type == plazza::Americana) {
        std::lock_guard<std::mutex> lock(_mutexProtect);
        setDough(getDough() - 1);
        setTomato(getTomato() - 1);
        setGruyere(getGruyere() - 1);
        setSteak(getSteak() - 1);
    }
    if (type == plazza::Fantasia) {
        std::lock_guard<std::mutex> lock(_mutexProtect);
        setDough(getDough() - 1);
        setTomato(getTomato() - 1);
        setEggplant(getEggplant() - 1);
        setGoatCheese(getGoatCheese() - 1);
        setChiefLove(getChiefLove() - 1);
    }
}

void plazza::Kitchen::cookPizza(const int &id)
{
    while (_isRunning) {
        usleep(50);
        plazza::Order order{};

        { // Get an order
            std::lock_guard<std::mutex> lock(_mutexOrders);

            if (_orders.empty())
                continue;
            order = _orders.front();
            _orders.erase(_orders.begin());
        }

        if (!canCook(order.type)) {
            std::lock_guard<std::mutex> lock(_mutexOrders);
            _orders.emplace_back(order);
            continue;
        }

        takeIngredients(order.type);
        _cooksStatus[id] = true;
        double cookingTime = (order.type == plazza::Regina ? 1 : (order.type == plazza::Margarita ? 2 : (order.type == plazza::Americana ? 2 : 4))) * _multiplier;
        std::this_thread::sleep_for(std::chrono::seconds(static_cast<int>(cookingTime)));

        { // Finish order and place it in cooked orders
            std::lock_guard<std::mutex> lock(_mutexCooked);

            _cookedOrders.emplace_back(order);
            _inCooking--;
            _cooksStatus[id] = false;
        }
    }
}

void plazza::Kitchen::refillChecker()
{
    if (_timer.refillStorage()) {
        if (getDough() < 5)
            setDough(getDough() + 1);
        if (getTomato() < 5)
            setTomato(getTomato() + 1);
        if (getGruyere() < 5)
            setGruyere(getGruyere() + 1);
        if (getHam() < 5)
            setHam(getHam() + 1);
        if (getMushrooms() < 5)
            setMushrooms(getMushrooms() + 1);
        if (getSteak() < 5)
            setSteak(getSteak() + 1);
        if (getEggplant() < 5)
            setEggplant(getEggplant() + 1);
        if (getGoatCheese() < 5)
            setGoatCheese(getGoatCheese() + 1);
        if (getChiefLove() < 5)
            setChiefLove(getChiefLove() + 1);
    }
}

void plazza::Kitchen::printStatus()
{
    std::lock_guard<std::mutex> lock(_mutexProtect);
    std::cout << "\tCooks State:" << std::endl;
    for (int i = 0; i < _nbCooks; i++) {
        std::cout << "\t\tCook " << i << ": ";
        if (_cooksStatus[i])
            std::cout << "Cooking" << std::endl;
        else
            std::cout << "Waiting" << std::endl;
    }
    std::cout << "\tKitchen Stock:" << std::endl;
    std::cout << "\t\tDough: " << getDough() << std::endl;
    std::cout << "\t\tTomato: " << getTomato() << std::endl;
    std::cout << "\t\tGruyere: " << getGruyere() << std::endl;
    std::cout << "\t\tHam: " << getHam() << std::endl;
    std::cout << "\t\tMushrooms: " << getMushrooms() << std::endl;
    std::cout << "\t\tSteak: " << getSteak() << std::endl;
    std::cout << "\t\tEggplant: " << getEggplant() << std::endl;
    std::cout << "\t\tGoat Cheese: " << getGoatCheese() << std::endl;
    std::cout << "\t\tChief Love: " << getChiefLove() << std::endl;
}

void plazza::Kitchen::startKitchen()
{
    while (!_timer.closeKitchen()) {
        { // Check if there is a refill to do
            std::lock_guard<std::mutex> lock(_mutexOrders);
            refillChecker();
        }
        try {
            auto order = _readQueue.receive();

            { // Add new orders to the orders list available for cooks
                std::lock_guard<std::mutex> lock(_mutexOrders);
                for (int i = 0; i < order.quantity; i++) {
                    _orders.emplace_back(order.type, order.size, 1);
                }
                _inCooking += order.quantity;
            }

            _timer.resetKitchenTime();
        } catch (const plazza::MessageQueue::Error &e) {
            try {
                auto readQueue = plazza::MessageQueue("/2" + std::to_string(_pid), O_CREAT | O_RDONLY | O_NONBLOCK);
                std::string msg = readQueue.receiveString();
                if (msg == "st") {
                    printStatus();
                }
            } catch (const plazza::MessageQueue::Error &e) {
                std::cerr << e.what() << std::endl;
                throw plazza::MessageQueue::Error("Error while reading from queue");
            }
            _cv.notify_all();

            { // Send cooked orders to reception and reset timer if needed
                std::lock_guard<std::mutex> lock(_mutexCooked);
                if (_inCooking > 0) {
                    _timer.resetKitchenTime();
                }
                for (auto &order : _cookedOrders) {
                    _logger << "A pizza " << _typeToString[order.type] << " has been cooked by " << _pid << "\n";
                }
                _cookedOrders.clear();
            }
        }
    }
    _isRunning = false; // Close threads routines
    _writeQueue.send("cl", 0);
}

int plazza::Kitchen::getDough() const
{
    return _dough;
}

int plazza::Kitchen::getTomato() const
{
    return _tomato;
}

int plazza::Kitchen::getGruyere() const
{
    return _gruyere;
}

int plazza::Kitchen::getHam() const
{
    return _ham;
}

int plazza::Kitchen::getMushrooms() const
{
    return _mushrooms;
}

int plazza::Kitchen::getSteak() const
{
    return _steak;
}

int plazza::Kitchen::getEggplant() const
{
    return _eggplant;
}

int plazza::Kitchen::getGoatCheese() const
{
    return _goatCheese;
}

int plazza::Kitchen::getChiefLove() const
{
    return _chiefLove;
}

void plazza::Kitchen::setDough(int doughNew)
{
    _dough = doughNew;
}

void plazza::Kitchen::setTomato(int tomatoNew)
{
    _tomato = tomatoNew;
}

void plazza::Kitchen::setGruyere(int gruyereNew)
{
    _gruyere = gruyereNew;
}

void plazza::Kitchen::setHam(int hamNew)
{
    _ham = hamNew;
}

void plazza::Kitchen::setMushrooms(int mushroomsNew)
{
    _mushrooms = mushroomsNew;
}

void plazza::Kitchen::setSteak(int steakNew)
{
    _steak = steakNew;
}

void plazza::Kitchen::setEggplant(int eggplantNew)
{
    _eggplant = eggplantNew;
}

void plazza::Kitchen::setGoatCheese(int goatCheeseNew)
{
    _goatCheese = goatCheeseNew;
}

void plazza::Kitchen::setChiefLove(int chiefLoveNew)
{
    _chiefLove = chiefLoveNew;
}
