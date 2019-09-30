#include "cannon.h" 

Cannon::Cannon(float x, float y, float width, float height)
    : x(x), y(y), health(100)
{
    this->wheel = new sf::CircleShape(width / 2.0f);
    this->wheel->setPosition(this->x, this->y);
    this->wheel->setFillColor(sf::Color::Black);
    this->projectile = nullptr;
    this->launcher = new sf::RectangleShape();
    this->launcher->setSize(sf::Vector2f(width, height));
    this->launcher->setPosition(this->x + 25.0f, this->y + 25.0f);
    this->launcher->setOrigin(0, width / 4.0f);
}

Cannon::~Cannon()
{
    this->DestroyProjectile();
    if (this->launcher)
        delete this->launcher;
    if (this->wheel)
        delete this->wheel;
    this->launcher = nullptr;
    this->wheel = nullptr;
}

void Cannon::InitProjectile(float velocity)
{
     this->projectile = new CannonBall(this, velocity);
}

void Cannon::DestroyProjectile()
{
    if (this->projectile)
        delete this->projectile;
    this->projectile = nullptr;
}

void Cannon::Render(sf::RenderWindow *window)
{
    if (this->projectile)
        window->draw(*this->projectile->shape);
    window->draw(*this->launcher);
    window->draw(*this->wheel);
}

CannonBall::CannonBall(const Cannon *cannon, float velocity)
{
    this->shotVelocity = velocity;
    this->shape = new sf::CircleShape(10.0f);
    this->airTime = new sf::Clock();
    this->velocity = sf::Vector2f(velocity, velocity);
    this->shape->setOrigin(10.0f, 10.0f);
    this->shape->setFillColor(sf::Color::Black);
    this->shotAngle = cannon->launcher->getRotation() * M_PI / 180.0f;

    sf::Vector2f cannonPos = cannon->launcher->getPosition();
    float hyp = cannon->launcher->getSize().x;
    float adj = cos(this->shotAngle);
    float opp = sin(this->shotAngle);
    this->velocity.x *= adj;
    this->velocity.y *= opp;
    this->shape->setPosition(cannonPos.x + adj * hyp, cannonPos.y + opp * hyp);
    this->shotPosition = this->shape->getPosition();
}

CannonBall::~CannonBall()
{
    delete this->shape;
    this->shape = nullptr;
    delete this->airTime;
    this->airTime = nullptr;
}

void HandleControls(Level *level, Cannon *cannon)
{
    if (level->Ready())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            cannon->launcher->rotate(-0.5f);
            level->Wait(8);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            cannon->launcher->rotate(0.5f);
            level->Wait(8);
        }

        if (!cannon->projectile && sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            level->velBarState++;
            if (level->velBarState > 2)
            level->velBarState = 0;

            if (level->velBarState & 1)
                level->InitVelocityBar();

            level->Wait(100);
        }
    }
}

void CannonBall::UpdateTrajectory(Level *level, Cannon *cannon)
{
    //Position equation = X(t) = 0.5at^2 + Vit + Xi
    float seconds = this->airTime->getElapsedTime().asSeconds() * 4.0f;
    this->shape->setPosition(
        /*0.5f * 0 * pow(seconds, 2) + */
            this->velocity.x * seconds + this->shotPosition.x,
        0.5f * 9.8f * pow(seconds, 2) 
            + this->velocity.y * seconds + this->shotPosition.y
    );

    sf::Vector2f position = this->shape->getPosition();
    if (position.x < level->xBoundaries[0] || position.x > level->xBoundaries[1]
        || position.y < level->yBoundaries[0] || position.y > level->yBoundaries[1])
        {
            cannon->DestroyProjectile();
            level->DestroyPositionStats();
            level->velBarState ^= level->velBarState;
        }
}


void Level::UpdatePositionStats(const CannonBall *projectile)
{
    if (!projectile)
        return;

    std::wstringstream stream;
    stream << std::setprecision(0) << std::fixed
        << L"\u03B8: " << projectile->shotAngle * 180.0f / M_PI 
        << L"\u00B0";
    this->statText[ANGLE]->setString(stream.str());
    stream.str(std::wstring());

    stream << L"\u0076\u2080: " << projectile->shotVelocity << L"pix/sec";
    this->statText[VELOCITY]->setString(stream.str());
    stream.str(std::wstring());

    stream << L"x\u2080: " << projectile->shotPosition.x << L"pix";
    this->statText[POSX]->setString(stream.str());
    stream.str(std::wstring());

    stream << L"t: " << projectile->airTime->getElapsedTime().asSeconds()
        << L"sec";
    this->statText[TIME]->setString(stream.str());
    stream.str(std::wstring());

    stream << L"y\u2080: " << projectile->shotPosition.y << L"pix";
    this->statText[POSY]->setString(stream.str());
    stream.str(std::wstring());

    sf::Vector2f pos = projectile->shape->getPosition();
    stream << L"x(t): " << pos.x << L"pix";
    this->statText[RESULTX]->setString(stream.str());
    stream.str(std::wstring());

    stream << L"y(t): " << pos.y << L"pix";
    this->statText[RESULTY]->setString(stream.str());
    stream.str(std::wstring());
}
