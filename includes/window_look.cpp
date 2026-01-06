#include <iostream>
#include <SFML/Graphics.hpp>

//Tworzy ramkę wokoło okna
sf::RectangleShape build_border(unsigned int width, unsigned int height,unsigned int color){
    sf::RectangleShape border({(float)width,(float)height});
    border.setFillColor(sf::Color::Transparent);//Wnętrze przezroczyste
    border.setOutlineColor(sf::Color(color));
    border.setOutlineThickness(-(float)width*0.009);//Grubość ramki//-(float)width*0.008
    return border;
}