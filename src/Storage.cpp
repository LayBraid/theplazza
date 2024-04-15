/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** Storage.cpp
*/

#include "Storage.hpp"

plazza::Storage::Storage() : _ingredients()
{
    _ingredients = {
            .tomato = 5,
            .gruyere = 5,
            .ham = 5,
            .mushroom = 5,
            .steak = 5,
            .eggplant = 5,
            .goatCheese = 5,
            .chiefLove = 5
    };
}

plazza::Storage::~Storage() = default;

plazza::Ingredients plazza::Storage::getIngredients() const
{
    return _ingredients;
}

void plazza::Storage::restock()
{
    _ingredients = {
            .tomato = 5,
            .gruyere = 5,
            .ham = 5,
            .mushroom = 5,
            .steak = 5,
            .eggplant = 5,
            .goatCheese = 5,
            .chiefLove = 5
    };
}
