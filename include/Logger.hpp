/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** Logger.hpp
*/

#ifndef LOGGER_HPP_
	#define LOGGER_HPP_

#include <iostream>
#include <fstream>

class Logger {
    public:
        Logger(const std::string &filepath);
        ~Logger();

        /**
         * @brief Overload of the << operator to write in the file and in the console
         *
         * @tparam T
         * @param data
         * @return Logger&
         */
        template <typename T>
        Logger &operator<<(const T &data)
        {
            std::cout << data;
            _file << data;
            return *this;
        }

    protected:
    private:
        std::ofstream _file;
};

#endif /*LOGGER_HPP_*/