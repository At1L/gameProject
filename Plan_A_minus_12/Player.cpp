#include "Player.h"
#include <iostream>
#include "Math.h"
#include <algorithm>


Player::Player() :
    playerSpeed(0.2f), hp(100),
    bulletMaxFireRate(100.0f), bulletFireRateTimer(0),
    skeletonMaxDamadgeRate(1000.0f), skeletonDamageRate(0)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
    boundingRectangle.setFillColor(sf::Color::Transparent); 
    boundingRectangle.setOutlineColor(sf::Color::Red); 
    boundingRectangle.setOutlineThickness(1);
    hp = 100;
    size = sf::Vector2i(24, 32);  
}

void Player::Load()
{
    
    if (texture.loadFromFile("Assets/Player/Textures/FumikoSpritesheet.png")) {
        std::cout << "Hero Loaded\n"; 
        
        if (font.loadFromFile("Assets/Fonts/arial.ttf"))
        {
            std::cout << "Font loaded\n";
            healthText.setFont(font);
            healthText.setString(std::to_string(hp));
        }
        
        int XIndex = 4;
        int YIndex = 2;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(XIndex * size.x, YIndex * size.y, size.x, size.y));
        sprite.setPosition(sf::Vector2f(100, 100));

        sprite.scale(sf::Vector2f(2, 2));
        boundingRectangle.setSize(sf::Vector2f(size.x * sprite.getScale().x, size.y * sprite.getScale().y));
    }
    else
        std::cout << "ERROR\n";
}

void Player::Update(double deltaTime, sf::Vector2f& mousePosition)
{

    //Moving up
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        //take current position of image
        sf::Vector2f position = sprite.getPosition();
        //add pixels to coordinate to move it
        sprite.setPosition(position + sf::Vector2f(0, -1) * playerSpeed * (float)deltaTime);
    }
    //Moving down
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        sf::Vector2f position = sprite.getPosition();
        sprite.setPosition(position + sf::Vector2f(0, 1) * playerSpeed * (float)deltaTime);
    }
    //Moving left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        sf::Vector2f position = sprite.getPosition();
        sprite.setPosition(position + sf::Vector2f(-1, 0) * playerSpeed * (float)deltaTime);
    }
    //Moving right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        sf::Vector2f position = sprite.getPosition();
        sprite.setPosition(position + sf::Vector2f(1, 0) * playerSpeed * (float)deltaTime);
    }
    //--------------------------BULLET--------------------------
    bulletFireRateTimer += deltaTime;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && bulletFireRateTimer >= bulletMaxFireRate)
    {
        bullets.push_back(new Bullet());
        int i = bullets.size() - 1;
        bullets[i]->Initialize(sprite.getPosition(), mousePosition, 0.5f);
        bullets[i]->Load();
        bulletFireRateTimer = 0;
    }
    for (size_t i = 0; i < bullets.size(); ++i)
    {
        bullets[i]->Update(deltaTime);
        
    }
    healthText.setString(std::to_string(hp));
    healthText.setPosition(sprite.getPosition());
    boundingRectangle.setPosition(sprite.getPosition());
}

void Player::UpdateSkeleton(double deltaTime, Skeleton*& skeleton)
{
    for (size_t i = 0; i < bullets.size(); ++i)
    {
        //bullets[i]->Update(deltaTime);
        if (skeleton->health > 0) {
            if (Math::DidRectCollide(bullets[i]->GetGlobalBounds(), skeleton->sprite.getGlobalBounds())) 
            {
                skeleton->ChangeHealth(-10); 
                bullets.erase(bullets.begin() + i); 
                std::cout << "Collision" << "\n";
            }
        }
    }
    //--------------------------BULLET--------------------------
    //--------------------------SKELETON------------------------
    boundingRectangle.setPosition(sprite.getPosition());
    skeletonDamageRate += deltaTime;
    if (Math::DidRectCollide(skeleton->sprite.getGlobalBounds(), this->sprite.getGlobalBounds()) && skeletonDamageRate >= skeletonMaxDamadgeRate)
    {
        hp--;
        skeletonDamageRate = 0;
        std::cout << "Collision" << "\n";
    }
    healthText.setString(std::to_string(hp));
    healthText.setPosition(sprite.getPosition());
    //--------------------------SKELETON------------------------
}

void Player::Draw(sf::RenderWindow& window)
{
    window.draw(sprite); 
    window.draw(boundingRectangle);
    window.draw(healthText);
    for (size_t i = 0; i < bullets.size(); ++i) 
        bullets[i]->Draw(window); 

}