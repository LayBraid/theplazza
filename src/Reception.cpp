/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** Reception.cpp
*/

#include "Reception.hpp"

plazza::Reception::Reception(const double &multiplier, const int &nbCooks, const int &restockDuration) :
    _multiplier(multiplier),
    _nbCooks(nbCooks),
    _restockDuration(restockDuration),
    _kitchens(),
    _forks(),
    _pizzaTypes(),
    _pizzaSizes(),
    _logger("log.log")
{
    _pizzaSizes = {
            {"S", plazza::PizzaSize::S},
            {"M", plazza::PizzaSize::M},
            {"L", plazza::PizzaSize::L},
            {"XL", plazza::PizzaSize::XL},
            {"XXL", plazza::PizzaSize::XXL},
    };

    _pizzaTypes = {
            {"regina", plazza::PizzaType::Regina},
            {"margarita", plazza::PizzaType::Margarita},
            {"americana", plazza::PizzaType::Americana},
            {"fantasia", plazza::PizzaType::Fantasia},
    };

    _welcomePrinter();
}

plazza::Reception::~Reception() = default;

void plazza::Reception::startReception()
{
    std::string buffer;

    while (std::getline(std::cin, buffer)) {
        if (buffer == "exit") {
            break;
        }
        if (buffer == "status") {
            _checkKitchens();
            _statusCommand();
            continue;
        }
        std::vector<std::string> inputSplit = _splitInput(buffer, ';');
        auto orders = _pizzaValidator(inputSplit);
        if (orders.empty()) {
            continue;
        }
        _checkKitchens();
        _dispatchOrders(orders);
    }
}

std::vector<plazza::Order> plazza::Reception::_pizzaValidator(std::vector<std::string> &input)
{
    std::vector<plazza::Order> orders;

    for (const auto &i: input) {
        std::vector<std::string> inputSplit = _splitInput(i, ' ');

        // Input format
        if (inputSplit.size() != 3) {
            std::cerr << "Error: Invalid input use this format: [type] [size] [quantity];" << std::endl;
            return {};
        }

        // Type
        auto type = _pizzaTypes.find(inputSplit[0]);
        if (type == _pizzaTypes.end()) {
            std::cerr << "Error: Invalid pizza type" << std::endl;
            return {};
        }

        // Size
        auto size = _pizzaSizes.find(inputSplit[1]);
        if (size == _pizzaSizes.end()) {
            std::cerr << "Error: Invalid pizza size" << std::endl;
            return {};
        }

        // Quantity
        if (inputSplit[2][0] != 'x') {
            std::cerr << "Error: Invalid pizza quantity" << std::endl;
            return {};
        }
        int nbPizza = 0;
        try {
            nbPizza = std::stoi(inputSplit[2].erase(0, 1));
            if (nbPizza <= 0)
                throw std::invalid_argument("");
        } catch (const std::invalid_argument &e) {
            std::cerr << "Error: Invalid pizza quantity" << std::endl;
            return {};
        }

        Order order = {type->second, size->second, nbPizza};
        orders.push_back(order);
    }
    return orders;
}

std::vector<std::string> plazza::Reception::_splitInput(const std::string &str, char delim)
{
    std::istringstream ss(str);
    std::string token;
    std::vector<std::string> res = {};

    while (std::getline(ss, token, delim)) {
        res.push_back(token);
    }
    return res;
}

void plazza::Reception::_welcomePrinter()
{
    int flagWidth = 42;
    int flagHeight = 6;

    std::string green = "\033[0;32m";
    std::string white = "\033[0;37m";
    std::string red = "\033[0;31m";
    std::string bold = "\033[1m";
    std::string reset = "\033[0m";

    for (int i = 0; i < flagHeight; i++) {
        for (int j = 0; j < flagWidth; j++) {
            if (j < flagWidth / 3) {
                std::cout << green << "*";
            } else if (j >= flagWidth / 3 && j < (2 * flagWidth) / 3) {
                std::cout << white << "*";
            } else {
                std::cout << red << "*";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << white << "Benvenuti al ristorante italiano ";
    std::cout << green << bold << "Plazza" << white << "!" << reset << std::endl;
}

void plazza::Reception::_statusCommand() const
{
    if (_kitchens.empty()) {
        return;
    }

    std::cout << "****** Status ******" << std::endl;
    for (auto &kitchen : _kitchens) {
        std::cout << "Kitchen " << kitchen.first << ":" << std::endl;
        std::cout << "\tCooking time multiplier: " << _multiplier << std::endl;
        std::cout << "\tNumber of cooks: " << _nbCooks << std::endl;
        std::cout << "\tRestock time: " << _restockDuration << std::endl;
        auto writeQueue = plazza::MessageQueue("/2" + std::to_string(kitchen.first), O_CREAT | O_WRONLY | O_NONBLOCK);
        writeQueue.send("st", 0);
        usleep(1000);
        std::cout << "********************" << std::endl;
    }
}

int plazza::Reception::_getNbKitchens(const std::vector<Order> &orders) const
{
    int nbAvailablePlaces = 0;
    for (auto &kitchen : _kitchens) {
        nbAvailablePlaces += kitchen.second;
    }

    int nbPizzas = 0;
    for (auto &order : orders) {
        nbPizzas += order.quantity;
    }

    int newPlaces = nbPizzas - nbAvailablePlaces;
    if (newPlaces <= 0) {
        return 0;
    }
    return newPlaces % (2 * _nbCooks) == 0 ? newPlaces / (2 * _nbCooks) : newPlaces / (2 * _nbCooks) + 1;
}

void plazza::Reception::_checkKitchens()
{
    std::vector<pid_t> pidsToRemove = {};

    for (auto &it : _kitchens) {
        auto readQueue = plazza::MessageQueue("/2" + std::to_string(it.first), O_CREAT | O_RDONLY | O_NONBLOCK);
        std::string msg = readQueue.receiveString();

        if (msg == "cl") {
            pidsToRemove.push_back(it.first);
        }
    }
    for (auto &pid : pidsToRemove) {
        plazza::MessageQueue::unlinkQueue("/" + std::to_string(pid));
        _forks.erase(pid); // Kill child process by calling it destructor
        _kitchens.erase(pid);
    }
}

void plazza::Reception::_dispatchOrders(std::vector<Order> &orders)
{
    int nbKitchens = _getNbKitchens(orders) - static_cast<int>(_kitchens.size());

    for (int i = 0; i < nbKitchens; i++) {
        auto fork = std::make_shared<plazza::Fork>();
        auto pid = (*fork).getPid();
        _forks.emplace(pid, std::move(fork));
        if (pid == -1) {
            std::cerr << "Error: fork failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            auto kitchen = new plazza::Kitchen(getpid(), _multiplier, _nbCooks, _restockDuration);
            kitchen->startKitchen();
            delete kitchen; // Quit kitchen (but not child process)
            exit(EXIT_SUCCESS); // Exit child program
        } else {
            _kitchens.emplace(pid, 2 * _nbCooks);
        }
    }
    _assignPizzas(orders);
}

void plazza::Reception::_assignPizzas(std::vector<Order> &orders)
{
    for (auto &kitchen : _kitchens) {
        if (orders.empty()) {
            break;
        }
        auto writeQueue = plazza::MessageQueue("/1" + std::to_string(kitchen.first), O_CREAT | O_WRONLY | O_NONBLOCK);
        while (kitchen.second > 0 && !orders.empty()) {
            plazza::Order order{};
            if (kitchen.second - orders.front().quantity < 0) {
                order = plazza::Order{orders.front().type, orders.front().size, kitchen.second};
                orders.front().quantity -= kitchen.second;
            } else {
                order = orders.front();
                orders.erase(orders.begin());
            }
            writeQueue.send(order, 0);
            kitchen.second -= order.quantity;
        }
    }
}
