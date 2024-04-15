/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** Kitchen.hpp
*/

#ifndef KITCHEN_HPP_
	#define KITCHEN_HPP_

#include "Reception.hpp"
#include "MessageQueue.hpp"
#include "Timer.hpp"
#include "Logger.hpp"
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <sys/fcntl.h>

namespace plazza {

    struct Order;

    class Kitchen {
        public:
            Kitchen(const pid_t &pid, const double &multiplier, const int &nbCooks, const int &restoreDuration);
            ~Kitchen();

            /**
             * @brief Start the kitchen, get the orders from the message queue and cook them
             */
             void startKitchen();

             /**
              * @brief Thread routine, cook the order and tell reception when it's done
              */
             void cookPizza(const int &id);
             void printStatus();

            int getDough() const;
            int getTomato() const;
            int getGruyere() const;
            int getHam() const;
            int getMushrooms() const;
            int getSteak() const;
            int getEggplant() const;
            int getGoatCheese() const;
            int getChiefLove() const;

            void setDough(int);
            void setTomato(int);
            void setGruyere(int);
            void setHam(int);
            void setMushrooms(int);
            void setSteak(int);
            void setEggplant(int);
            void setGoatCheese(int);
            void setChiefLove(int);

            bool canCook(PizzaType type);
            void takeIngredients(PizzaType type);
            void refillChecker();

        protected:
        private:
            pid_t _pid;
            double _multiplier;
            int _nbCooks;
            int _restoreDuration;
            Logger _logger;
            Timer _timer;
            MessageQueue _readQueue;
            MessageQueue _writeQueue;
            std::vector<std::thread> _cooks; // 1: Thread, 2: isCooking, 3: isRunning
            std::vector<bool> _cooksStatus;
            std::vector<Order> _orders;
            std::vector<Order> _cookedOrders;
            std::size_t _inCooking;
            std::mutex _mutexOrders;
            std::mutex _mutexCooked;
            std::mutex _mutexProtect;
            std::condition_variable _cv;
            bool _isRunning;
            std::map<PizzaType, std::string> _typeToString;

            int _dough;
            int _tomato;
            int _gruyere;
            int _ham;
            int _mushrooms;
            int _steak;
            int _eggplant;
            int _goatCheese;
            int _chiefLove;

    };
}

#endif /*KITCHEN_HPP_*/