#include <iostream>
#include <SFML/Graphics.hpp>

//Tworzy ramkę wokoło okna
sf::RectangleShape build_border(unsigned int width, unsigned int height, float x, float y, int color){
    sf::RectangleShape border({(float)width,(float)height});
    border.setOrigin(border.getGeometricCenter());
    border.setPosition({width/2.0f,height/2.0f});
    border.setFillColor(sf::Color::Transparent);//Wnętrze przezroczyste
    border.setOutlineColor(sf::Color(color));
    border.setOutlineThickness(-(float)width*0.009);//Grubość ramki//-(float)width*0.008
    return border;
}