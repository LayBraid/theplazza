/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** MessageQueue.hpp
*/

#ifndef MESSAGEQUEUE_HPP_
	#define MESSAGEQUEUE_HPP_

#include "Pizza.hpp"
#include <string>
#include <mqueue.h>
#include <exception>

#define MQ_MODE     0644
#define MSG_SIZE    2

namespace plazza {

    class MessageQueue {
        public:
            MessageQueue(const std::string &name, int flag);
            ~MessageQueue();

            /**
             * @brief Delete the queue name, so it cannot be used again by an other process
             *
             * @param name
             */
            static void unlinkQueue(const std::string &name);

            /**
             * @brief serialize and send an order to the queue
             *
             * @param order
             * @param priority
             * @return int
             */
            int send(const Order &order, unsigned int priority);

            /**
             * @brief send a string to the queue
             *
             * @param message
             * @param priority
             * @return int
             */
            int send(const std::string &message, unsigned int priority);

            /**
             * @brief receive an order from the queue and deserialize it
             *
             * @return Order
             */
            Order receive();

            /**
             * @brief receive a string from the queue
             *
             * @return std::string
             */
            std::string receiveString();

            /**
             * @brief serialize the order to a char* (2 bytes maximum for a message that contain max 255 pizzas)
             *
             * @param order
             * @return char*
             */
            [[nodiscard]] char *serializeData(const Order &order) const;

            /**
             * @brief deserialize the char* to an order
             *
             * @param serializedData
             * @return Order
             */
            Order deserializeData(const char *serializedData) const;

            class Error: public std::exception {
                public:
                    explicit Error(std::string message) noexcept;
                    ~Error() override;

                    /**
                     * @brief return the error message
                     *
                     * @return const char*
                     */
                    [[nodiscard]] const char *what() const noexcept override;

                protected:
                private:
                    std::string _message;
            };

        protected:
        private:
            mqd_t _mq;
            std::string _queueName;
    };
}

#endif /*MESSAGEQUEUE_HPP_*/
