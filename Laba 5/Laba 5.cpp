#include <iostream>
#include <ctime>
#include <cmath>
#include <SFML/Graphics.hpp>

struct Vel
{
    float x, y;
};

sf::Color randomColor()
{
    int random = rand() % 7;
    switch (random)
    {
    case 0:
        return sf::Color::White;
        break;
    case 1:
        return sf::Color::Red;
        break;
    case 2:
        return sf::Color::Yellow;
        break;
    case 3:
        return sf::Color::Green;
        break;
    case 4:
        return sf::Color::Cyan;
        break;
    case 5:
        return sf::Color::Magenta;
        break;
    case 6:
        return sf::Color::Blue;
        break;
    default:
        break;
    }
}

void calcCentres(float &centre1x, float &centre2x, float &centre1y, float &centre2y, 
        const sf::CircleShape &circ1, const sf::CircleShape &circ2, const int &size, const float &rad)
{
    centre1x = circ1.getPosition().x + rad * size;
    centre2x = circ2.getPosition().x + rad * size;
    centre1y = circ1.getPosition().y + rad * size;
    centre2y = circ2.getPosition().y + rad * size;
}

int main()
{
    srand(time(NULL));
    const float rad = 25.f;
    int number_of_circles, size_of_circles;
    std::cout << "Enter the number of balls: ";
    std::cin >> number_of_circles; 
    while (true)
    {
        std::cout << "Enter the size of balls: ";
        std::cin >> size_of_circles;
        if (size_of_circles > 10) std::cout << "Try to decrease number of balls\n";
        else break;
    }
    sf::CircleShape *shapes = new sf::CircleShape[number_of_circles];
    Vel* shapes_vel = new Vel[number_of_circles];
    sf::RenderWindow window(sf::VideoMode(0, 0), "random title", sf::Style::Fullscreen);
    system("cls");
    if (number_of_circles > window.getSize().x / (2 * rad * size_of_circles))
    {
        number_of_circles = window.getSize().x / (2 * rad * size_of_circles) - 1;
        std::cout << "The number of circles for this ball size is too big, it was changed with " << 
        static_cast<int>(window.getSize().x / (2 * rad * size_of_circles)) - 1 << " circles\n";
    }
    for (int i = 0; i < number_of_circles; ++i)
    {
        int tmp_width = window.getSize().x / number_of_circles - 2 * rad * size_of_circles;
        shapes_vel[i].x = 2.0f / (rand() % 10 + 1);
        shapes_vel[i].y = 2.0f / (rand() % 10 + 1);
        shapes[i].setRadius(rad * size_of_circles);
        int x, y;
        x = rand() % tmp_width + (window.getSize().x / number_of_circles) * i;
        y = rand() % (window.getSize().y - 2 * static_cast<int>(rad) * size_of_circles);
        shapes[i].setPosition(x, y);
        shapes[i].setFillColor(randomColor());
    }
    sf::Clock clock;
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        sf::Event event;
        while (window.pollEvent(event));
        if (event.type == sf::Event::Closed)
            window.close();
        for (int i = 0; i < number_of_circles; ++i)
        {
            window.draw(shapes[i]);
            if (shapes[i].getPosition().x <= 0 || shapes[i].getPosition().x >= window.getSize().x - 2 * rad * size_of_circles)
                shapes_vel[i].x *= -1;
            if (shapes[i].getPosition().y <= 0 || shapes[i].getPosition().y >= window.getSize().y - 2 * rad * size_of_circles)
                shapes_vel[i].y *= -1;
            shapes[i].setPosition(sf::Vector2f(shapes[i].getPosition().x + shapes_vel[i].x, shapes[i].getPosition().y + shapes_vel[i].y));
            for (int j = i + 1; j < number_of_circles; ++j)
            {
                float centre1x, centre1y, centre2x, centre2y;
                calcCentres(centre1x, centre2x, centre1y, centre2y, shapes[i], shapes[j], size_of_circles, rad);
                if (sqrt((centre2x - centre1x) * (centre2x - centre1x) + (centre2y - centre1y) * (centre2y - centre1y)) <= 2 * rad * (1.0f * size_of_circles))
                {
                    while (sqrt((centre2x - centre1x) * (centre2x - centre1x) + (centre2y - centre1y) * (centre2y - centre1y)) <= 2 * rad * (1.0f * size_of_circles))
                    {
                        shapes[i].setPosition(sf::Vector2f(shapes[i].getPosition().x - shapes_vel[i].x, shapes[i].getPosition().y - shapes_vel[i].y));
                        shapes[j].setPosition(sf::Vector2f(shapes[j].getPosition().x - shapes_vel[j].x, shapes[j].getPosition().y - shapes_vel[j].y));
                        calcCentres(centre1x, centre2x, centre1y, centre2y, shapes[i], shapes[j], size_of_circles, rad);
                    }
                    float v1x, v1y, v2x, v2y;
                    v1x = shapes_vel[i].x - (centre1x - centre2x) * ((shapes_vel[i].x - shapes_vel[j].x) * (centre1x - centre2x) + (shapes_vel[i].y - shapes_vel[j].y) * (centre1y - centre2y)) / ((centre1x - centre2x) * (centre1x - centre2x) + (centre1y - centre2y) * (centre1y - centre2y));
                    v1y = shapes_vel[i].y - (centre1y - centre2y) * ((shapes_vel[i].x - shapes_vel[j].x) * (centre1x - centre2x) + (shapes_vel[i].y - shapes_vel[j].y) * (centre1y - centre2y)) / ((centre1x - centre2x) * (centre1x - centre2x) + (centre1y - centre2y) * (centre1y - centre2y));
                    v2x = shapes_vel[j].x - (centre2x - centre1x) * ((shapes_vel[j].x - shapes_vel[i].x) * (centre2x - centre1x) + (shapes_vel[j].y - shapes_vel[i].y) * (centre2y - centre1y)) / ((centre1x - centre2x) * (centre1x - centre2x) + (centre1y - centre2y) * (centre1y - centre2y));
                    v2y = shapes_vel[j].y - (centre2y - centre1y) * ((shapes_vel[j].x - shapes_vel[i].x) * (centre2x - centre1x) + (shapes_vel[j].y - shapes_vel[i].y) * (centre2y - centre1y)) / ((centre1x - centre2x) * (centre1x - centre2x) + (centre1y - centre2y) * (centre1y - centre2y));
                    shapes_vel[i].x = v1x, shapes_vel[j].x = v2x, shapes_vel[i].y = v1y, shapes_vel[j].y = v2y;
                }
            }
        }
        window.display();
        for (int i = 0; i < number_of_circles; ++i)
        {
            float centrex, centrey;
            centrex = shapes[i].getPosition().x + rad * size_of_circles;
            centrey = shapes[i].getPosition().y + rad * size_of_circles;
            if ((sf::Mouse::getPosition().x - centrex) * (sf::Mouse::getPosition().x - centrex) + (sf::Mouse::getPosition().y - centrey) * (sf::Mouse::getPosition().y - centrey) <= rad * rad * size_of_circles * size_of_circles)
            {
                window.close();
                delete[] shapes;
                delete[] shapes_vel;
            }
        }
    }
    std::cout << "You've survived " << clock.getElapsedTime().asSeconds() << " seconds\n";
	return 0;
}