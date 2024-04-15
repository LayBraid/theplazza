/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** Logger.cpp
*/

#include "Logger.hpp"

Logger::Logger(const std::string &filepath)
    : _file()
{
    _file.open(filepath, std::ios::app);
    if (!_file.is_open()) {
        std::cerr << "Error: could not open file " << filepath << std::endl;
        exit(84);
    }
}

Logger::~Logger()
{
    _file.close();
}
