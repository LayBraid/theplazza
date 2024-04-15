/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** testSafeQueue.cpp
*/

#include <criterion/criterion.h>
#include <iostream>
#include "MessageQueue.hpp"

Test (MessageQueue, testSend)
{
    try {
        plazza::MessageQueue queue("/test", O_CREAT | O_RDWR | O_NONBLOCK);
        plazza::Order order = {plazza::PizzaType::Fantasia, plazza::PizzaSize::L, 1};
        queue.send(order, 0);
        plazza::Order received = queue.receive();
        cr_assert_eq(received.type, plazza::PizzaType::Fantasia);
        cr_assert_eq(received.size, plazza::PizzaSize::L);
        cr_assert_eq(received.quantity, 1);
    } catch (const plazza::MessageQueue::Error &e) {
        std::cerr << e.what() << std::endl;
        cr_assert_fail();
    }
}

Test (MessageQueue, throwCtor)
{
    try {
        plazza::MessageQueue queue("", -1);
    } catch (const plazza::MessageQueue::Error &e) {
        cr_assert_str_eq(e.what(), "mq_open failed");
    }
}

Test (MessageQueue_Error, what)
{
    try {
        throw plazza::MessageQueue::Error("test error");
    } catch (const plazza::MessageQueue::Error &e) {
        cr_assert_str_eq(e.what(), "test error");
    }
}
