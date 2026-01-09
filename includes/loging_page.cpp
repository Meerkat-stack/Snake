#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>

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
void input_text(std::string& target_string,std::string& hide_target_string, const sf::Event &ev,int limit,int active_field){
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
        else if (unicode > 32 && unicode < 128) {
            // Dodajemy limit, żeby tekst nie uciekł z ekranu
            if (target_string.length() < limit) { 
                if(active_field==1) {hide_target_string +="*";}
                target_string += static_cast<char>(unicode);
            }
            // std::cout<<target_string<<std::endl;//Debug
        }
    }


}

//sprawdza dane logowania
int verify_user(std::string& username, std::string& password){
    //zwraca 0 gdy błędne dane logowania
    //zwraca 1 gdy zalogowano
    //zwraca -1 gdy nie otwarto pliku

    //Wczytanie pliku z użytkownikami
    std::ifstream file("userdata/users_log.txt");
    //Sprawdza czy napewno plik został otwarty
    if(!file.is_open()){
        std::cout<<"Cannot open users_log.txt file to verify user.\n";
        return -1;
    }
    else{
        std::string u, p;//u to odczytany login/ p to odczytane hasło
        while (file >> u >> p) {
            std::cout<<u<<std::endl;//Debug
            std::cout<<p<<std::endl;//Debug
            // W każdej pętli 'u' to login, a 'p' to hasło z jednej linijki
            if (u == username && p == password) {
                file.close();//Zamyka plik
                return 1;//Zweryfikowany i zalogowany
            }
        }
        file.close();//Zamyka plik
        return 0;//Błędne dane logowania
    }

}

void log(int& state, int& error_log,std::string& login_input,std::string& password_input,std::string& hide_password_input,sf::Text& login_input_text, sf::Text& password_input_text){
    int loging_status = verify_user(login_input,password_input);
    std::cout<<login_input<<std::endl;//Debug
    std::cout<<password_input<<std::endl;//Debug
            
    //Kasuje dane logowania
    login_input = "";
    password_input = "";
    hide_password_input = "";
    login_input_text.setString("");
    password_input_text.setString("");

    switch (loging_status){
        case -1:
            state = 0;//Jesli brak pliku z haslami przechodzi do menu startowego
            break;
        case 0:
            //logowanie niepowiodło się dane mają się usunąć, pojawić ramka czerwona i komunikat
            state = 1;//Potem 4
            error_log = -1;//Zmienna informuje że trzeba wyświetlić ostrzeżenie
            std::cout<<"Brak dostępu\n";//Debug
            break;
        case 1:
            //Przejście do następnej strony, dostęp przyznany
            state = 5;
            error_log = 1;//Brak problemów z logowaniem
            std::cout<<"Zalogowano\n";

    }
}