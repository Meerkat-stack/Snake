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

//Przyjmuje tekst z klawiatury i przypisuje go do zmiennej aby go potem wyświetlić
void input_text(std::string& target_string, const sf::Event &ev,int limit,int active_field){
    // 1. Sprawdzamy, czy zdarzenie to "Wpisanie tekstu" (TextEntered)
    if (const auto* textEvent = ev.getIf<sf::Event::TextEntered>()) {
        
        uint32_t unicode = textEvent->unicode;

        // 2. Obsługa klawisza Backspace (usuwanie)
        if (unicode == 8) { 
            if (!target_string.empty()) {
                target_string.pop_back();
            }
        }
        // 3. Obsługa zwykłych znaków (od spacji wzwyż)
        else if (unicode >= 32 && unicode < 128) {
            // Dodajemy limit, żeby tekst nie uciekł z ekranu
            if (target_string.length() < limit) { 
                if(active_field==1) target_string +="*";
                else target_string += static_cast<char>(unicode);
            }
            // std::cout<<target_string<<std::endl;//Debug
        }
    }


}

