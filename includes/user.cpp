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


sf::Text text_label(int size, float x, float y, std::string label_text,unsigned int color,sf::Font& font){
    sf::Text text(font, label_text, size);//Ustawia dla napisu, czcionkę, tekst, rozmiar
    text.setFillColor(sf::Color(color));
    // text.setOutlineColor(sf::Color(Dark_color));
    // text.setOutlineThickness(-1.f);

    //Ustawia punkt odniesienia napisu 
    sf::FloatRect bounds = text.getLocalBounds();
    // text.setOrigin({bounds.position.x, 0.f});//Wyrównuje do lewej
    // text.setOrigin({bounds.position.x+bounds.size.x, 0.f});//Wyrównuje do prawej
    text.setOrigin({bounds.position.x+bounds.size.x/2,//Wyrównuje do środka
                    (float)text.getCharacterSize()*0.6f});//To ustawia dolny punkt odniesienia na linijkę do pisania
                    //Tak samo jak masz linijki w zeszycie to tu the same
    text.setPosition({x,y});

    return text;
}
