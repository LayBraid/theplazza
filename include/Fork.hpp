/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** Fork.hpp
*/

#ifndef FORK_HPP_
	#define FORK_HPP_

#include <unistd.h>
#include <csignal>

namespace plazza {

    class Fork {
        public:
            explicit Fork();
            ~Fork();

            /**
             * @brief Get the pid of the child process
             *
             * @return pid_t
             */
            [[nodiscard]] pid_t getPid() const;

        protected:
        private:
            pid_t _pid;
    };
}

#endif /*FORK_HPP_*/
