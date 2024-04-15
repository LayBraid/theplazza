/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** SafeQueue.cpp
*/

#include "MessageQueue.hpp"

plazza::MessageQueue::MessageQueue(const std::string &name, int flag) : _mq(), _queueName(name)
{
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MSG_SIZE;

    _mq = mq_open(name.c_str(), flag, MQ_MODE, &attr);
    if (_mq == -1) {
        throw MessageQueue::Error("mq_open failed");
    }
}

plazza::MessageQueue::~MessageQueue()
{
    mq_close(_mq);
}

void plazza::MessageQueue::unlinkQueue(const std::string &name)
{
    mq_unlink(name.c_str());
}

int plazza::MessageQueue::send(const plazza::Order &order, unsigned int priority)
{
    auto serializedMsg = serializeData(order);
    auto ret = mq_send(_mq, serializedMsg, MSG_SIZE, priority);
    if (ret == -1) {
        throw MessageQueue::Error("mq_send failed");
    }
    delete[] serializedMsg;
    return ret;
}

int plazza::MessageQueue::send(const std::string &message, unsigned int priority)
{
    auto ret = mq_send(_mq, message.c_str(), message.size(), priority);
    if (ret == -1) {
        throw MessageQueue::Error("mq_send failed");
    }
    return ret;
}

plazza::Order plazza::MessageQueue::receive()
{
    char buffer[MSG_SIZE] = {0};
    if (mq_receive(_mq, buffer, MSG_SIZE, nullptr) == -1 && errno != EAGAIN) {
        throw MessageQueue::Error("mq_receive failed");
    }
    return deserializeData(buffer);
}

std::string plazza::MessageQueue::receiveString()
{
    char buffer[MSG_SIZE] = {0};
    if (mq_receive(_mq, buffer, MSG_SIZE, nullptr) == -1 && errno != EAGAIN) {
        throw MessageQueue::Error("mq_receive failed");
    }
    return std::string(buffer);
}

char *plazza::MessageQueue::serializeData(const plazza::Order &order) const
{
    auto serializedData = new char[2];

    // PizzaType (4 bits)
    if (order.type == plazza::PizzaType::Fantasia) {
        serializedData[0] = 0b00000111;
    } else {
        serializedData[0] = order.type;
    }

    // PizzaSize (4 bits)
    if (order.size == plazza::PizzaSize::XXL) {
        serializedData[0] |= 0b00000000;
    } else if (order.size == plazza::PizzaSize::XL) {
        serializedData[0] |= 0b01110000;
    } else {
        serializedData[0] |= order.size << 4;
    }

    // PizzaNumber (8 bits)
    if (order.quantity > 255) {
        throw plazza::MessageQueue::Error("Invalid PizzaNumber");
    }
    serializedData[1] = static_cast<char>(order.quantity);

    return serializedData;
}

plazza::Order plazza::MessageQueue::deserializeData(const char *serializedData) const
{
    plazza::Order order = {plazza::PizzaType::Margarita, plazza::PizzaSize::S, 0};

    // PizzaType (4 bits)
    switch (serializedData[0] & 0b00001111) {
        case 0b00000001:
            order.type = plazza::PizzaType::Regina;
            break;
        case 0b00000010:
            order.type = plazza::PizzaType::Margarita;
            break;
        case 0b00000100:
            order.type = plazza::PizzaType::Americana;
            break;
        case 0b00000111:
            order.type = plazza::PizzaType::Fantasia;
            break;
        default:
            throw plazza::MessageQueue::Error("Invalid PizzaType");
    }

    // PizzaSize (4 bits)
    switch (serializedData[0] & 0b11110000) {
        case 0b00010000:
            order.size = plazza::PizzaSize::S;
            break;
        case 0b00100000:
            order.size = plazza::PizzaSize::M;
            break;
        case 0b01000000:
            order.size = plazza::PizzaSize::L;
            break;
        case 0b01110000:
            order.size = plazza::PizzaSize::XL;
            break;
        case 0b00000000:
            order.size = plazza::PizzaSize::XXL;
            break;
        default:
            throw plazza::MessageQueue::Error("Invalid PizzaSize");
    }

    // PizzaNumber (8 bits)
    if (serializedData[1]) {
        order.quantity = static_cast<int>(serializedData[1]);
    }

    return order;
}

/**
 * Error class
 */
plazza::MessageQueue::Error::Error(std::string message) noexcept
    : _message(std::move(message))
{}

plazza::MessageQueue::Error::~Error() = default;

const char *plazza::MessageQueue::Error::what() const noexcept
{
    return _message.c_str();
}
