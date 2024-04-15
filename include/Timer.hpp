/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** Timer.hpp
*/

#ifndef TIMER_HPP_
	#define TIMER_HPP_

#include <iostream>
#include <chrono>
#include "Storage.hpp"

namespace plazza {

    class Timer {
        public:
            Timer();
            explicit Timer(const std::size_t &);
            ~Timer();

            /**
             * @brief Reset the kitchen time
             */
            void resetKitchenTime();

            /**
             * @brief Reset the refill time
             */
            void resetRefillTime();

            /**
             * @brief Get the kitchen time
             *
             * @return std::size_t
             */
            [[nodiscard]] std::size_t getRefillDelay() const;

            /**
             * @brief Set the refill delay
             *
             * @param delay
             */
            void setRefillDelay(const std::size_t &delay);

            bool refillStorage();
            bool closeKitchen();
        protected:
        private:
            std::chrono::steady_clock::time_point _kitchenTime;
            std::chrono::steady_clock::time_point _refillTime;
            std::size_t _refillDelay;
    };
}

#endif /*TIMER_HPP_*/