/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** Timer.cpp
*/

#include "Timer.hpp"

plazza::Timer::Timer() = default;

plazza::Timer::Timer(const std::size_t &refill) : _refillDelay(refill)
{
    _kitchenTime = std::chrono::steady_clock::now();
    _refillTime = std::chrono::steady_clock::now();
}

plazza::Timer::~Timer() = default;

void plazza::Timer::resetKitchenTime()
{
    _kitchenTime = std::chrono::steady_clock::now();
}

void plazza::Timer::resetRefillTime()
{
    _refillTime = std::chrono::steady_clock::now();
}

std::size_t plazza::Timer::getRefillDelay() const
{
    return _refillDelay;
}

void plazza::Timer::setRefillDelay(const std::size_t &delay)
{
    _refillDelay = delay;
}

bool plazza::Timer::refillStorage()
{
    auto now = std::chrono::steady_clock::now();
    unsigned long diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - _refillTime).count();

    if (diff >= _refillDelay) {
        resetRefillTime();
        return true;
    }
    return false;
}

bool plazza::Timer::closeKitchen()
{
    auto now = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - _kitchenTime).count();

    if (diff >= 5) {
        resetKitchenTime();
        return true;
    }
    return false;
}
