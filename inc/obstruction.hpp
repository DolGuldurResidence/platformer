#pragma once
#include <SFML/Graphics.hpp>

class Obstruction  {
    private:
        sf::RectangleShape shape;
        float posX, posY;
        float width, height;
    public:
        Obstruction(float x, float y, float w, float h) {
            posX = x;
            posY = y;
            width = w;
            height = h;
            shape.setPosition(sf::Vector2f(posX, posY));
            shape.setSize(sf::Vector2f(width, height));
        }

        sf::RectangleShape& getShape() { return shape; }
        float getPositionX() const { return posX; }
        float getPositionY() const { return posY; }
        void setPosition(float x, float y) {
            posX = x;
            posY = y;
            shape.setPosition(sf::Vector2f(posX, posY));
        }
};
