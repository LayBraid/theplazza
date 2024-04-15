/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** Reception.hpp
*/

#ifndef RECEPTION_HPP_
	#define RECEPTION_HPP_

#include "Pizza.hpp"
#include "Kitchen.hpp"
#include "MessageQueue.hpp"
#include "Fork.hpp"
#include "Logger.hpp"
#include <memory>
#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <map>
#include <algorithm>
#include <sys/wait.h>

namespace plazza {

    class Kitchen;

    class MessageQueue;

    class Reception {
        public:
            Reception(const double &multiplier, const int &nbCooks, const int &restockDuration);
            ~Reception();

            /**
             * @brief Start the reception (shell)
             */
            void startReception();

        protected:
        private:
            double _multiplier;
            int _nbCooks;
            int _restockDuration;
            std::map<pid_t, int> _kitchens;
            std::map<pid_t, std::shared_ptr<Fork>> _forks;
            std::map<std::string, PizzaType> _pizzaTypes;
            std::map<std::string, PizzaSize> _pizzaSizes;
            Logger _logger;

            /**
             * @brief Print the welcome message
             */
            void _welcomePrinter();

            /**
             * @brief split string by delim
             *
             * @param str
             * @param delim
             * @return std::vector<std::string>
             */
            std::vector<std::string> _splitInput(const std::string &str, char delim);

            /**
             * @brief Get and print the status of all kitchens
             */
            void _statusCommand() const;

            /**
             * @brief Calculate the number of kitchens needed
             *
             * @param orders
             * @return std::size_t
             */
            [[nodiscard]] int _getNbKitchens(const std::vector<Order> &orders) const;

            /**
             * @brief Get messages from kitchens
             */
            void _checkKitchens();

            /**
             * @brief Create a new kitchen and assign orders to it
             *
             * @param orders
             */
            void _dispatchOrders(std::vector<Order> &orders);

            /**
             * @brief Assign orders to a kitchen
             *
             * @param orders
             */
            void _assignPizzas(std::vector<Order> &orders);

            /**
             * @brief check if the input is valid
             *
             * @param input
             * @return std::vector<Order>
             */
            std::vector<Order> _pizzaValidator(std::vector<std::string> &input);
    };
}

#endif /*RECEPTION_HPP_*/