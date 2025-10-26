#pragma once
#include <SFML/Graphics.hpp>
#include <animation.hpp>
#include <algorithm>
#include <cmath>

const float SHAPE_RADIUS = 50.f;
const float GRAVITY = 1500.f;        
const float MAX_SPEED_X = 600.f;    
const float ACCELERATION = 2000.f;   
const float FRICTION = 0.8f;    
const float JUMP_POWER = -700.f;     
const float JUMP_DURATION = 0.5f;  
const int SPRITE_WIDTH = 50;
const int SPRITE_HEIGHT = 85;

class Player {
private:

    sf::Texture texture;
    sf::Sprite sprite;

    float posX, posY;   
    float velX, velY;   
    
    float gravity;
    float maxSpeedX;
    float acceleration;
    float friction;

    float jumpPower;
    bool isOnGround;
    bool canDoubleJump;
    float jumpTimer;

    Animation walkAnim;
    Animation idleAnim;
    Animation* currentAnim = nullptr;
    bool facingRight = true;

public:

    Player(const float& startX, const float& startY) : sprite(texture) {
        setPosition(startX, startY);
        setVelocity(0,0);
        setGravity(GRAVITY);
        setMaxSpeed(MAX_SPEED_X);
        setAcceleration(ACCELERATION);
        setFriction(FRICTION);
        setJumpPower(JUMP_POWER);
        setJumpTimer(0.f);
        // init animations BEFORE using them in initSprite
        walkAnim = Animation("Walk.png", 7, SPRITE_WIDTH, SPRITE_HEIGHT, 50, 50, 125, 0.1f);
        idleAnim = Animation("Idle.png", 1, SPRITE_WIDTH, SPRITE_HEIGHT, 50, 50, 0, 0.2f);
        currentAnim = &idleAnim;
        initSprite();
    }


    void initSprite() {
        currentAnim->applyTo(sprite);
        sprite.setOrigin(sf::Vector2f(25.f, 42.5f)); // половины 50x85
        sprite.setPosition(sf::Vector2f(posX, posY));
    }


    //обработчик кнопок, dt~0.016 
    void handleInput(const float& dt) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            velX += acceleration * dt; // velx ~ 630;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            velX -= acceleration * dt;
        }
        else {
            velX *= friction;
        }
        
        if (velX > maxSpeedX) velX = maxSpeedX;
        if (velX < -maxSpeedX) velX = -maxSpeedX;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            if (isOnGround) {
                jump();
            }
            else if (canDoubleJump && jumpTimer <= 0.f) {
                jump();
                canDoubleJump = false;
            }
        }

        if (jumpTimer > 0) {
            jumpTimer -= dt;
        }
    }
        
    void update(const float& dt) {
        if (!isOnGround) {
            velY += gravity * dt;
        }

        posX += velX * dt;
        posY += velY * dt;

        sprite.setPosition(sf::Vector2f(posX, posY));

        // Выбор анимации
        if (isOnGround && std::fabs(velX) > 5.f) currentAnim = &walkAnim;
        else currentAnim = &idleAnim;

        // Обновить и применить кадр
        currentAnim->update(dt);
        currentAnim->applyTo(sprite);

        // Отразить по X при движении влево
        if (velX > 1.f) facingRight = true;
        else if (velX < -1.f) facingRight = false;
        sprite.setScale(sf::Vector2f(facingRight ? 1.f : -1.f, 1.f));
    }

    void jump() {
        velY = jumpPower;
        isOnGround = false;
        jumpTimer = JUMP_DURATION;
    }

    void applyObstructionCollision(const float& obsX, const float& obsY, 
        const float& obsW,const float& obsH) {
            
        float playerLeft = posX - SPRITE_WIDTH/2;
        float playerRight = posX + SPRITE_WIDTH/2;
        float playerTop = posY - SPRITE_HEIGHT/2;
        float playerBottom = posY + SPRITE_HEIGHT/2;
        
        float obstacleLeft = obsX;
        float obstacleRight = obsX + obsW;
        float obstacleTop = obsY;
        float obstacleBottom = obsY + obsH;
        
        if (playerRight > obstacleLeft && 
            playerLeft < obstacleRight && 
            playerBottom > obstacleTop && 
            playerTop < obstacleBottom) {

            float overlapLeft = playerRight - obstacleLeft;
            float overlapRight = obstacleRight - playerLeft;
            float overlapTop = playerBottom - obstacleTop;
            float overlapBottom = obstacleBottom - playerTop;
            
            float minOverlap = std::min(std::min(overlapLeft, overlapRight),
            std::min( overlapTop, overlapBottom));
            
            if (minOverlap == overlapTop) {
                posY = obstacleTop - SPRITE_HEIGHT/2; 
                velY = 0;
                isOnGround = true;
                canDoubleJump = true;
            }
            else if (minOverlap == overlapBottom) {
                posY = obstacleBottom + SPRITE_HEIGHT/2;
                velY = 0;
            }
            else if (minOverlap == overlapLeft) {
                posX = obstacleLeft - SPRITE_WIDTH/2;
                velX = 0;
            }
            else if (minOverlap == overlapRight) {
                posX = obstacleRight + SPRITE_WIDTH/2;
                velX = 0;
            }
        }
        else isOnGround = false; 
    }




    void applyFloorCollision(float floorHeight) {
        if (posY + SHAPE_RADIUS > floorHeight) {
            posY = floorHeight - SPRITE_HEIGHT/2;
            velY = 0;
            isOnGround = true;
            canDoubleJump = true;
        }
    }
    
    void applyWallCollision(float leftBound, float rightBound) {
        if (posX - SPRITE_WIDTH/2 < leftBound) {
            posX = leftBound + SPRITE_WIDTH/2;
            velX = 0;
        }
        else if (posX + SPRITE_WIDTH/2 > rightBound) {
            posX = rightBound - SPRITE_WIDTH/2;
            velX = 0;
        }
    }
    

    sf::Sprite& getSprite() { return sprite; }
    float getPositionX() const { return posX; }
    float getPositionY() const { return posY; }
    float getVelocityX() const { return velX; }
    float getVelocityY() const { return velY; }
    bool getIsOnGround() const { return isOnGround; }


    void setPosition(float x, float y) {
        posX = x;
        posY = y;
        sprite.setPosition(sf::Vector2f(posX, posY));
    }
    void setVelocity(float vx, float vy) {
        velX = vx;
        velY = vy;
    }

    void setGravity(float gravity){
        this->gravity = gravity;
    }

    void setAcceleration(float acceleration) {
        this->acceleration = acceleration;
    }

    void setMaxSpeed(float maxSpeed){
        this->maxSpeedX = maxSpeed;
    }

    void setFriction(float friction){
        this->friction = friction;
    }

    void setJumpPower(float jumpPower){
        this->jumpPower = jumpPower;
    }

    void setJumpTimer(float jumpTimer){
        this->jumpTimer = jumpTimer;
    } 
};