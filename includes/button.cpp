#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

//Tworzy przycisk, czytaj taki prostokąt
sf::RectangleShape build_button(float a, float b, float x, float y, unsigned int button_color, unsigned int border_color){
    sf::RectangleShape button({a,b});
    button.setOrigin(button.getSize()/2.0f);
    button.setPosition({x,y});
    button.setFillColor(sf::Color(button_color));
    button.setOutlineColor(sf::Color(border_color));
    button.setOutlineThickness(-a*0.02);//-a*0.01

    return button;
}

sf::Text label(int size, float x, float y, std::string label_text,unsigned int color,sf::Font& font){

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

//akcja guzika, zwraca 1 gdy kliknięty
int button_action(const sf::RectangleShape &button, const sf::Event &ev, sf::RenderWindow &window){
    if(ev.is<sf::Event::MouseButtonPressed>()){
        const auto* mousePress = ev.getIf<sf::Event::MouseButtonPressed>();
        if(mousePress->button == sf::Mouse::Button::Left){
            sf::Vector2i pixelPos = mousePress->position;
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            if(button.getGlobalBounds().contains(worldPos)) return 1;
        }
    }
    return 0;
}

//animacja guzika
//bez inline kompilator się gubi, ale czm?
inline void button_animation(sf::RectangleShape &button, sf::Text &text, sf::RenderWindow &window)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    bool hovered = button.getGlobalBounds().contains(worldPos);
    if(hovered){ 
        button.setScale({1.15f, 1.15f});
        text.setScale({1.15f, 1.15f});
    }
    else{ 
        button.setScale({1.0f, 1.0f});
        text.setScale({1.0f, 1.0f});
    }
}


//Tworzy przysciski na starcie
inline void build_start_bttons(unsigned int width, unsigned int height, unsigned int button_color, unsigned int text_color,sf::RectangleShape* buttons,sf::Text* buttons_labels, sf::Font& font){
    buttons[0] = build_button(0.12f*width,0.05f*height,width/2.0f,height/2.0f-0.09f*height,button_color,text_color);//sign in
    buttons[1] = build_button(0.12f*width,0.05f*height,width/2.0f,height/2.0f,button_color,text_color);//sign up
    buttons[2] = build_button(0.12f*width,0.05f*height,width/2.0f,height/2.0f+0.09f*height,button_color,text_color);//guest

    buttons_labels[0] = label(0.035f*height,width/2.0f,height/2.0f-0.09f*height,"Sign in",text_color,font);
    buttons_labels[1] = label(0.035f*height,width/2.0f,height/2.0f,"Sign up",text_color,font);
    buttons_labels[2] = label(0.035f*height,width/2.0f,height/2.0f+0.09f*height,"Guest",text_color,font);
    
}

