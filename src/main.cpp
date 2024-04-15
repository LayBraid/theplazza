/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** main
*/

#include <iostream>
#include "Reception.hpp"

int main(int ac, char **av)
{
    if (ac != 4) {
        std::cerr << "Usage: ./plazza [multiplier] [cooks] [time]" << std::endl;
        return 84;
    }
    auto reception = plazza::Reception(std::stod(av[1]), std::stoi(av[2]), std::stoi(av[3]));
    reception.startReception();
    return 0;
}
