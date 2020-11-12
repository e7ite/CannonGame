#include "level.hpp"
#include "cannon.hpp"

Level::Level(const sf::Texture &texture, float x1, float y1, float x2, float y2)
{
    this->timer = 0;
    this->xBoundaries[0] = x1;     
    this->xBoundaries[1] = x2;     
    this->yBoundaries[0] = y1;
    this->yBoundaries[1] = y2;
    this->velBorder = nullptr;
    this->velProgress = nullptr;
    this->velBarState = 0;

    this->background = sf::Sprite(texture);
    this->gameTime = sf::Clock();

    this->statText.resize(RESULTY + 1);
    for (auto &i : this->statText)
        i = nullptr;
}

Level::~Level()
{
    this->DestroyVelocityBar();
    this->DestroyPositionStats();
    this->velBorder = nullptr;
    this->velProgress = nullptr;
    this->velBarState = 0;
}

void Level::Wait(int ms)
{
    this->timer = this->gameTime.getElapsedTime().asMilliseconds() + ms;
}

bool Level::Ready()
{
    return this->gameTime.getElapsedTime().asMilliseconds() > this->timer;
}

void Level::InitVelocityBar()
{ 
    this->velBorder = std::make_unique<sf::RectangleShape>(sf::Vector2f(600.0f, 50.0f));
    this->velProgress = std::make_unique<sf::RectangleShape>(sf::Vector2f(600.0f, 50.0f));
    this->velBorder->setPosition(200.0f, 300.0f);
    this->velBorder->setOutlineThickness(2.5f);
    this->velBorder->setOutlineColor(sf::Color::Red);
    this->velBorder->setFillColor(sf::Color::Transparent);

    this->velProgress->setPosition(200.0f, 300.0f);
    this->velProgress->setFillColor(sf::Color::Red);
}

void Level::DestroyVelocityBar()
{
    if (auto *borderPtr = this->velBorder.release())
        delete borderPtr;
    if (auto *progressPtr = this->velProgress.release())
        delete progressPtr;
    this->velBarState ^- this->velBarState;
}

void Level::InitPositionStats(const sf::Font &font)
{
    for (int i = ANGLE; i <= RESULTY; i++)
    {
        sf::Text *textStat = new sf::Text;
        textStat->setFont(font);
        textStat->setCharacterSize(30);
        textStat->setFillColor(sf::Color::Black);
        textStat->setStyle(sf::Text::Regular);
        textStat->setPosition(0.0f, 35 * i);
        this->statText[i] = std::unique_ptr<sf::Text>(textStat);
    }
}

void Level::DestroyPositionStats()
{
    for (auto &i : this->statText) 
        if (sf::Text *textStat = i.release())
            delete textStat;
}

void Level::ChangeVelocityBarState()
{
    this->velBarState++;
    if (this->velBarState > 2)
        this->velBarState = 0;

    // If the state ever turns to 1, that means that we should spawn the 
    // velocity bar
    if (this->velBarState & 1)
        this->InitVelocityBar();
}

bool Level::GetVelocity(float *velocity)
{
    static float curVelocity = 50.0f;
    static float increment = 2.0f; 
    constexpr float minVel = 50.0f, maxVel = 150.0f;
    constexpr float minWid = 0.0f, maxWid = 600.0f;
    constexpr float scale = (maxWid - minWid) / (maxVel - minVel);
    
    // If the state is 0, the bar just got setup and the velocity is being
    // selected
    if (!this->velBarState)
    {
        *velocity = 0.0f;
        return false;
    }
    // If the state is 0x2, the bar has just finished selecting a velocity.
    // clean up, save the velocity and return true
    else if (this->velBarState & 0x2)
    {
        *velocity = curVelocity;
        curVelocity = 50.0f;
        increment = 2.0f;
        this->velBarState <<= 1;
        this->DestroyVelocityBar();
        return true; 
    }
    // If the state is 0x4, the bar is not spawned, so return false
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

bool Level::Collision(float x, float y) 
{
    return x < this->xBoundaries[0] || x > this->xBoundaries[1]
        || y < this->yBoundaries[0] || y > this->yBoundaries[1];
}

void Level::UpdatePositionStats(const CannonBall *projectile)
{
    // Shows the angle from where the cannon was shot
    std::wstringstream stream;
    stream << std::setprecision(0) << std::fixed
           << L"\u03B8: " << projectile->shotAngle * 180.0f / M_PI 
           << L"\u00B0";
    this->statText[ANGLE]->setString(stream.str());
    stream.str(std::wstring());

    // Shows the intial shot velocity
    stream << L"\u0076\u2080: " << projectile->shotVelocity << L"pix/sec";
    this->statText[VELOCITY]->setString(stream.str());
    stream.str(std::wstring());

    // Shows the intial shot x position 
    stream << L"x\u2080: " << projectile->shotPosition.x << L"pix";
    this->statText[POSX]->setString(stream.str());
    stream.str(std::wstring());

    // Shows the intial shot x position 
    stream << L"t: " << projectile->airTime.getElapsedTime().asSeconds()
        << L"sec";
    this->statText[TIME]->setString(stream.str());
    stream.str(std::wstring());

    stream << L"y\u2080: " << projectile->shotPosition.y << L"pix";
    this->statText[POSY]->setString(stream.str());
    stream.str(std::wstring());

    sf::Vector2f pos = projectile->shape.getPosition();
    stream << L"x(t): " << pos.x << L"pix";
    this->statText[RESULTX]->setString(stream.str());
    stream.str(std::wstring());

    stream << L"y(t): " << pos.y << L"pix";
    this->statText[RESULTY]->setString(stream.str());
    stream.str(std::wstring());
}

void Level::Render(sf::RenderWindow &window)
{
    window.draw(this->background);
    if (this->velBarState & 0x1)
    {
        if (this->velBorder)
            window.draw(*this->velBorder);
        if (this->velProgress)
            window.draw(*this->velProgress);
    }
    else if (this->velBarState & 0x4)
    {
        for (int i = ANGLE; i <= RESULTY; i++)
            if (this->statText[i])
                window.draw(*this->statText[i]);
    }
}