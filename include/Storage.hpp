/*
** EPITECH PROJECT, 2023
** B-CCP-400-LYN-4-1-theplazza-clement.loeuillet
** File description:
** Storage.hpp
*/

#ifndef STORAGE_HPP_
	#define STORAGE_HPP_

namespace plazza {

    struct Ingredients {
        int tomato;
        int gruyere;
        int ham;
        int mushroom;
        int steak;
        int eggplant;
        int goatCheese;
        int chiefLove;
    };

    class Storage {
        public:
            Storage();
            ~Storage();

            [[nodiscard]] Ingredients getIngredients() const;

            void restock();

        protected:
        private:
            Ingredients _ingredients;
    };
}

#endif /* STORAGE_HPP_ */
