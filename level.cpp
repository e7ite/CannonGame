#include "level.h"

Level::Level(float x1, float y1, float x2, float y2, const sf::Texture *texture)
{
    this->timer = 0;
    this->xBoundaries[0] = x1;     
    this->xBoundaries[1] = x2;     
    this->yBoundaries[0] = y1;
    this->yBoundaries[1] = y2;
    this->velBorder = nullptr;
    this->velProgress = nullptr;
    this->velBarState = 0;

    this->background = texture ? new sf::Sprite(*texture) : nullptr;
    this->gameTime = new sf::Clock;
    memset(this->statText, 0, sizeof(sf::Text*) * 5);
}

Level::~Level()
{
    delete this->background;
    this->background = nullptr;
    delete this->gameTime;
    this->gameTime = nullptr;
    this->DestroyVelocityBar();
    this->DestroyPositionStats();
    this->velBorder = nullptr;
    this->velProgress = nullptr;
    this->velBarState = 0;
}

void Level::Wait(int ms)
{
    this->timer = this->gameTime->getElapsedTime().asMilliseconds() + ms;
}

bool Level::Ready()
{
    return this->gameTime->getElapsedTime().asMilliseconds() > this->timer;
}

void Level::InitVelocityBar()
{ 
    this->velBorder = new sf::RectangleShape(sf::Vector2f(600.0f, 50.0f));
    this->velProgress = new sf::RectangleShape(sf::Vector2f(600.0f, 50.0f));
    this->velBorder->setPosition(200.0f, 300.0f);
    this->velBorder->setOutlineThickness(2.5f);
    this->velBorder->setOutlineColor(sf::Color::Red);
    this->velBorder->setFillColor(sf::Color::Transparent);

    this->velProgress->setPosition(200.0f, 300.0f);
    this->velProgress->setFillColor(sf::Color::Red);
}

void Level::DestroyVelocityBar()
{
    if (this->velBorder)
        delete this->velBorder;
    if (this->velProgress)
        delete this->velProgress;
    this->velBorder = nullptr;
    this->velProgress = nullptr;
}

void Level::InitPositionStats(const sf::Font &font)
{
    for (int i = ANGLE; i <= RESULTY; i++)
    {
        this->statText[i] = new sf::Text;
        this->statText[i]->setFont(font);
        this->statText[i]->setCharacterSize(30);
        this->statText[i]->setFillColor(sf::Color::Black);
        this->statText[i]->setStyle(sf::Text::Regular);
        this->statText[i]->setPosition(0.0f, 35 * i);
    }
}

void Level::DestroyPositionStats()
{
    for (int i = ANGLE; i <= RESULTY; i++)
        if (this->statText[i])
            delete this->statText[i];
}

bool Level::GetVelocity(float *velocity)
{
    static float curVelocity = 50.0f;
    static float increment = 2.0f; 
    constexpr float minVel = 50.0f, maxVel = 150.0f;
    constexpr float minWid = 0.0f, maxWid = 600.0f;
    constexpr float scale = (maxWid - minWid) / (maxVel - minVel);
    
    if (!this->velBarState)
    {
        *velocity = 0.0f;
        return false;
    }
    else if (this->velBarState & 0x2)
    {
        *velocity = curVelocity;
        curVelocity = 50.0f;
        increment = 2.0f;
        this->velBarState <<= 1;
        this->DestroyVelocityBar();
        return true; 
    }
    else if (this->velBarState & 0x4)
        return false;

    if (curVelocity >= maxVel)
        increment = -2.0f;
    if (curVelocity <= minVel)
        increment = 2.0f;
    curVelocity += increment;

    sf::Vector2f curSize = this->velProgress->getSize();
    curSize.x = maxWid - (maxVel - curVelocity) * scale;   
    this->velProgress->setSize(curSize);
    
    return false;
}

void Level::Render(sf::RenderWindow *window)
{
    if (this->background)
        window->draw(*this->background);
    if (this->velBarState & 0x1)
    {
        if (this->velBorder)
            window->draw(*this->velBorder);
        if (this->velProgress)
            window->draw(*this->velProgress);
    }
    else if (this->velBarState & 0x4)
    {
        for (int i = ANGLE; i <= RESULTY; i++)
            if (this->statText[i])
                window->draw(*this->statText[i]);
    }
}