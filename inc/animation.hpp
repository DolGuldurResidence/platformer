#pragma once 
#include <SFML/Graphics.hpp>
#include <string>

const std::string GLOBAL_PATH = "C:\\Users\\study\\projects\\cmake-sfml-project-master\\pics";
const float FRAME_DURATION = 0.1f; 

class Animation {
private:
    sf::Texture texture;
    sf::Clock animationClock;
    sf::IntRect intRect;
    sf::Sprite sprite;
    std::string subPath;
public:
    Animation(const std::string& subPath) : sprite(texture) {
        setTexture(subPath);
    }

    void setTexture(const std::string& subPath) {
        std:: string fullPath = GLOBAL_PATH + "\\" + subPath;
        if(!texture.loadFromFile(fullPath)){
            throw std::runtime_error("Failed to load texture: " + subPath);
        }

    }

    void update(int& currentFrame, const int& totalFrames, const int& lenght,
         const int& startX, const int& startY, const int& step ){
        
        float elapsed = animationClock.getElapsedTime().asSeconds();
        if(elapsed < FRAME_DURATION){
            animationClock.restart();
            currentFrame = (currentFrame + 1) % totalFrames;
            int newLeft = startX + (currentFrame * (step));
            intRect = sf::IntRect({newLeft, startX },{startY, lenght});
            sprite.setTextureRect(intRect);
        }

    }

    sf::Sprite move(){
        int currentFrame = 0;
        update(currentFrame, 7, 85, 50, 50, 125);
    }
};
