/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** Pizza.hpp
*/

#ifndef PIZZA_HPP_
#define PIZZA_HPP_

#include <iostream>
#include <map>
#include <tuple>

namespace plazza
{
    enum PizzaType
    {
        Regina = 1,
        Margarita = 2,
        Americana = 4,
        Fantasia = 8
    };

    enum PizzaSize
    {
        S = 1,
        M = 2,
        L = 4,
        XL = 8,
        XXL = 16
    };

    struct Order {
        PizzaType type;
        PizzaSize size;
        int quantity;
    };
}

#endif /* PIZZA_HPP_ */
