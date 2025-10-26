#pragma once 
#include <SFML/Graphics.hpp>
#include <string>
#include <stdexcept>

const std::string GLOBAL_PATH = "C:\\Users\\study\\projects\\cmake-sfml-project-master\\pics";


class Animation {
private:
    sf::Texture texture;
    sf::IntRect frameRect;
    int totalFrames = 1;
    int frameWidth = 0;
    int frameHeight = 0;
    int startX = 0;
    int startY = 0;
    int stepX = 0; 
    int currentFrame = 0; 
    float frameDurationSec = 0.1f; 
    float timer = 0.f; 

public:
    Animation() = default;

    Animation(const std::string& subPath,
        int frames,
        int frameWidth,
        int frameHeight,
        int startX,
        int startY,
        int stepX,
        float frameDurationSec) 
    : totalFrames(frames),
    frameWidth(frameWidth),
    frameHeight(frameHeight),
    startX(startX), 
    startY(startY),
    stepX(stepX), 
    frameDurationSec(frameDurationSec)
    {
    setTexture(subPath);
    frameRect = sf::IntRect({startX, startY}, {frameWidth, frameHeight});
    }
    
    void setTexture(const std::string& subPath) {
        std::string fullPath = GLOBAL_PATH + "\\" + subPath;
        if (!texture.loadFromFile(fullPath)) {
            throw std::runtime_error("Failed to load texture: " + subPath);
        }
    }

    void reset(){
        currentFrame = 0;
        timer = 0.f;
        frameRect = sf::IntRect({startX, startY}, {frameWidth, frameHeight});
    }

    void update(float dt){
        timer += dt;
        if(timer >= frameDurationSec){
            timer -= frameDurationSec;
            currentFrame = (currentFrame + 1) % totalFrames;
            int left = startX + currentFrame * stepX;
            frameRect = sf::IntRect({left, startY}, {frameWidth, frameHeight});
        }
    }

    void applyTo(sf::Sprite& sprite){
        sprite.setTexture(texture);
        sprite.setTextureRect(frameRect);
    }
};
