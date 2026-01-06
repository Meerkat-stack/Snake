#include <iostream>
#include <SFML/Graphics.hpp>

//Tworzy rakme na tekst
sf::RectangleShape text_frame(float a, float b, float x, float y, unsigned int border_color){
    sf::RectangleShape button({a,b});
    button.setOrigin(button.getSize()/2.0f);
    button.setPosition({x,y});
    button.setFillColor(sf::Color::Transparent);
    button.setOutlineColor(sf::Color(border_color));
    button.setOutlineThickness(-a*0.01);//-a*0.01

    return button;
}

//Tworzy tło za ramką na tekst
sf::RectangleShape text_frame_bg(float a, float b, float x, float y, unsigned int bg_color ,unsigned int border_color){
    sf::RectangleShape button({a,b});
    button.setOrigin(button.getSize()/2.0f);
    button.setPosition({x,y});
    button.setFillColor(sf::Color(bg_color));
    button.setOutlineColor(sf::Color(border_color));
    button.setOutlineThickness(-a*0.01);//-a*0.01

    return button;
}



