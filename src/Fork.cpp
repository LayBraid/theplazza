/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** Fork.cpp
*/

#include "Fork.hpp"

plazza::Fork::Fork() : _pid(fork())
{}

plazza::Fork::~Fork()
{
    if (_pid > 0) {
        kill(_pid, SIGTERM);
    }
}

pid_t plazza::Fork::getPid() const
{
    return _pid;
}
