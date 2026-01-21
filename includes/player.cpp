#include <iostream>
#include <fstream>
#include "seciurity.h"
#include "button.h"
#include "math_const.h"

// Struktura gracza
struct Player {
	std::string name = "Unknown";

	// Statystyki podstawowe
	float total_pixels_traveled = 0.0f;
	int total_play_time_sec = 0;
	int snake_max_length = 0;
	int snake_min_length = 0;

	// Statystyki rozszerzone (Przyczyny zgonów)
	int deaths_wall = 0;
	int deaths_self = 0;

	// Postępy
	int unlocked_maps_count = 8;
	int unlocked_skins_count = 8;
};

extern Player player;

void save_player(){
    if (player.name == "Guest" || player.name == "Unknown") return;//Dla gości albo niezalogowanych nie zapisuje nic
    else{
        std::ifstream original("userdata/users_data.txt");//Otwiera plik do odczytu
        std::ofstream temp("userdata/users_data_temp.txt");//Otwiera plik do zapisu

        std::string s_name, s_dist, s_time, s_maxL, s_minL, s_wall, s_self, s_maps, s_skins;

        std::string tmp_name = player.name;
        encrypt(tmp_name,16);

        while(original>>s_name>>s_dist>>s_time>>s_maxL>>s_minL>>s_wall>>s_self>>s_maps>>s_skins) {

            // decrypt(s_name,16);//Deszyfruje odczytaną nazwę gracza

            if(s_name==tmp_name){
                //Modyfikuje dane gracza przed zapisaniem        
                // std::cout<<"    |"<<s_name<<std::endl;//Debug
                // std::cout<<player.name<<std::endl;//Debug

                // s_name = player.name;
                // encrypt(s_name,16);//Szyfruje przed zapisem
                s_dist = std::to_string(player.total_pixels_traveled);//Zamienia int na string
                s_time = std::to_string(player.total_play_time_sec);
                s_maxL = std::to_string(player.snake_max_length);
                s_minL = std::to_string(player.snake_min_length);
                s_wall = std::to_string(player.deaths_wall);
                s_self = std::to_string(player.deaths_self);
                s_maps = std::to_string(player.unlocked_maps_count);
                s_skins = std::to_string(player.unlocked_skins_count);
            }

            temp<<s_name<<" "<<s_dist<<" "<<s_time<<" "<<s_maxL<<" "<<s_minL<<" "<<s_wall<<" "<<s_self<<" "<<s_maps<<" "<<s_skins<<"\n";
        }
        original.close();
        temp.close();

        //Usuwa stary plik, 0 oznacza powodzenie 
        if (std::remove("userdata/users_data.txt") != 0) {
            // std::cout<<"Old file canceled\n";//Debug
        }

        //Zmienia nazwę nowego pliku
        if (std::rename("userdata/users_data_temp.txt", "userdata/users_data.txt") != 0) {
            // std::cout<<"Error: zmiana nazwy nie powiodła się\n"//Debug
        }

    }
}

//Odczytuje dane gracza
void read_player() {
    std::ifstream file("userdata/users_data.txt");

    if (!file.is_open()) {
        std::cerr << "Błąd: Nie można otworzyć pliku users_data.txt do odczytu!\n";
        return;
    }

    std::string s_name, s_dist, s_time, s_maxL, s_minL, s_wall, s_self, s_maps, s_skins;

    std::string target_cipher = player.name;
    encrypt(target_cipher, 16); 

    while (file >> s_name >> s_dist >> s_time >> s_maxL >> s_minL >> s_wall >> s_self >> s_maps >> s_skins) {
        
        // Jeśli zaszyfrowane imię z pliku pasuje do naszego celu
        if (s_name == target_cipher) {
            // 4. Konwertujemy stringi z pliku na liczby i zapisujemy w strukturze player
            try {
                player.total_pixels_traveled = std::stof(s_dist);
                player.total_play_time_sec = std::stoi(s_time);
                player.snake_max_length = std::stoi(s_maxL);
                player.snake_min_length = std::stoi(s_minL);
                player.deaths_wall = std::stoi(s_wall);
                player.deaths_self = std::stoi(s_self);
                player.unlocked_maps_count = std::stoi(s_maps);
                player.unlocked_skins_count = std::stoi(s_skins);
            } catch (...) {
                std::cerr << "Bład konwersji danych dla gracza: " << player.name << "\n";
            }
            break; // Znaleźliśmy właściwego gracza, wychodzimy z pętli
        }
    }

    file.close();
}


//Exp to stare player.min_lenght, bo nie ma minimalnej dlugości już
//Aktualizuje dane do exp
void update_exp(){
    player.snake_min_length = 
        player.snake_max_length*1000 +
        player.total_pixels_traveled/1000 +
        player.total_play_time_sec/6 -
        (player.deaths_self+player.deaths_wall)*5;

}


//Funkcja budująca wizualizację statystyk
void build_player_stat(sf::Text* statistic, sf::Font& font, sf::Font& math_font,float width, float height, unsigned int color){
    int size = (int)(1.5*(0.5f*0.037f*height+3));//Polepszyć jakość tak jak przy innych rozmiarach
    update_exp();
    
    std::string labels[9];
    labels[0] = "Player:\t" + player.name;
    labels[1] = "Total play time: " + std::to_string(player.total_play_time_sec/60) + " min";
    labels[2] = "Distance traveled: " + std::to_string((int)player.total_pixels_traveled) + " px";
    labels[3] = "Max snake length: " + std::to_string(player.snake_max_length);
    labels[4] = "Exp: " + std::to_string(player.snake_min_length);
    labels[5] = "Deaths by wall: " + std::to_string(player.deaths_wall);
    labels[6] = "Deaths by tail: " + std::to_string(player.deaths_self);
    labels[7] = "Unlocked maps:\t";
    sf::String math_symbols[] = { 
        L"\u03C0", L"e", L"\u03B3", L"\u03B6(3)", L"\u03D5", L"\u221A2", L"\u221A3", L"\u221A5" 
    };    
    for(int i=0;i<player.unlocked_maps_count;i++){
        labels[8] += math_symbols[i];
        labels[8] += " ";
    }

    //Ustawia kolor wszyskic na jednakowy, ustawia punkt odniesienia na lewo i ewentualnie grubośc krawędzi, pozycje
    for(int i=0;i<9;i++){
        if(i==8){
            statistic[i] = sf::Text(math_font,labels[i],size*4);
            statistic[i].setStyle(sf::Text::Bold);//Pogrubia symbole
            // statistic[i].setCharacterSize(4*size);//Ustawia większą czcionkę
            statistic[i].setScale({0.25f,0.25f});//Skaluje symbole
            // statistic[i].setOutlineThickness(0.25);
            // statistic[i].setOutlineColor(sf::Color(color));
        }
        else{
            statistic[i] = sf::Text(font,labels[i],size);//Tekst pola
        }
        statistic[i].setFillColor(sf::Color(color));
        //Ewentualnie grubość krawędzi
        sf::FloatRect bounds = statistic[i].getLocalBounds();
        statistic[i].setOrigin({bounds.position.x, 0.f});
        

        //Ustawienie pozycji
        if(i==0)//Nazwa
            statistic[i].setPosition({width/8.0f,height/12.0f + i*0.07f*height});//Ustawia odstep między linijkami
        else if(i==1||i==2||i==3)//Pierwsza kolumna
            statistic[i].setPosition({width/8.0f+0.02f*width,height/12.0f + i*0.07f*height});
        else if(i==4||i==5||i==6)//Druga kolumna
            statistic[i].setPosition({width/8.0f+0.02f*width+0.4f*width,height/12.0f + (i-3)*0.07f*height});
        else if(i==7)//Etykieta dla stałych
            statistic[i].setPosition({width/8.0f,height/12.0f + (i-3)*0.075f*height});
        else if(i==8)//Stałe
            statistic[i].setPosition({width/8.0f+0.2f*width,height/12.0f + (i-4)*0.075f*height});

    }
}

//Funkcja aktualizująca statystyki za pomoca sf::setString("");
void update_player_stat(sf::Text* statistic){

    int size = (int)(1.5*(0.5f*0.037f*740+3));//Polepszyć jakość tak jak przy innych rozmiarach

    update_exp();
    
    statistic[0].setString("Player:\t" + player.name);
    statistic[1].setString("Total play time: " + std::to_string(player.total_play_time_sec/60) + " min");
    statistic[2].setString("Distance traveled: " + std::to_string((int)player.total_pixels_traveled) + " px");
    statistic[3].setString("Max snake length: " + std::to_string(player.snake_max_length));
    statistic[4].setString("Exp: " + std::to_string(player.snake_min_length));
    statistic[5].setString("Deaths by wall: " + std::to_string(player.deaths_wall));
    statistic[6].setString("Deaths by tail: " + std::to_string(player.deaths_self));
    statistic[7].setString("Unlocked maps:");

    sf::String tmp_label = "";
    sf::String math_symbols[] = { 
        L"\u03C0", L"e", L"\u03B3", L"\u03B6(3)", L"\u03D5", L"\u221A2", L"\u221A3", L"\u221A5" 
    };    
    for(int i=0;i<player.unlocked_maps_count;i++){
        tmp_label += math_symbols[i];
        tmp_label += "\t";
    }

    statistic[8].setString(tmp_label);//Wyświetla stałe jakie osiągnieto

    for(int i = 0; i < 9; i++) {
        sf::FloatRect bounds = statistic[i].getLocalBounds();
        statistic[i].setOrigin({bounds.position.x, 0.f}); // Tylko jeśli chcesz wyrównanie do lewej po zmianie tekstu
        statistic[i].setCharacterSize(4*size);//Ustawia większą czcionkę
        statistic[i].setScale({0.25f,0.25f});//Skaluje symbole

    }

}

void build_stat_bg(sf::RectangleShape* bg,unsigned int width, unsigned int height, unsigned int bg_color, unsigned int front_color){
    sf::RectangleShape statistic_bg({width*0.85f,height*0.4f});
    statistic_bg.setOrigin(statistic_bg.getGeometricCenter());
    statistic_bg.setPosition({width/2.0f,height/4.0f*1.04f});
    statistic_bg.setFillColor(sf::Color(bg_color));
    statistic_bg.setOutlineColor(sf::Color(front_color));
    statistic_bg.setOutlineThickness(4);

    bg[0] = statistic_bg;

    bg[1] = sf::RectangleShape({2.0f,height*0.4f*0.565f});
    bg[2] = sf::RectangleShape({width*0.85f,2.0f});
    bg[3] = sf::RectangleShape({width*0.85f,2.0f});

    for(int i=0;i<3;i++){
        bg[i+1].setFillColor(sf::Color(front_color));
        bg[i+1].setOrigin(bg[i+1].getGeometricCenter());
    }

    bg[1].setPosition({width/2.0f,height/4.0f*(1.45f+0.55f)/2.0f});
    bg[2].setPosition({width/2.0f,height/4.0f*1.45f});
    bg[3].setPosition({width/2.0f,height/4.0f*0.55f});

}

void builda_gamemode_buttons(sf::RectangleShape* buttons,unsigned int width, unsigned int height, unsigned int button_color, unsigned int border_color){
    for(int i=0;i<4;i++){
        buttons[i] = build_button(
            width*0.85f/2.2f, height*0.05f,//Wymiary pojedynczego przycisku
            width/3.73f, height/1.85f + i*height*0.1,//Pozycja kolejnych przycisków
            button_color,border_color
        );
        buttons[i].setOutlineThickness(4);
    }

    for(int i=4;i<8;i++){
        buttons[i] = build_button(
            width*0.85f/2.2f, height*0.05f,//Wymiary pojedynczego przycisku
            3.0f*width/4.098f, height/1.85f + (i-4)*height*0.1,//Pozycja kolejnych przycisków
            button_color,border_color
        );
        buttons[i].setOutlineThickness(4);
    }
}

//Funkcja tworząca etykiety dla przycisków
void build_gamemode_buttons_labels(sf::Text* buttons_labels,sf::Font& font, unsigned int width, unsigned int height, unsigned int color){

    int sclae = height*0.03f*1.0f;//Rozmiar czcionki
    int n = 13;//Liczba wypisywanych cyfr

    //Lista symboli
    sf::String symbols[8] = {        
        PI_CONST.symbol,
        E_CONST.symbol,
        GAMMA_CONST.symbol,
        ZETA_CONST.symbol,
        PHI_CONST.symbol,
        SQRT2_CONST.symbol,
        SQRT3_CONST.symbol,
        SQRT5_CONST.symbol
};
    //Lista cyfr
    sf::String digits[8] = {
        PI_CONST.digits.substr(0,n),
        E_CONST.digits.substr(0,n),
        GAMMA_CONST.digits.substr(0,n),
        ZETA_CONST.digits.substr(0,n),
        PHI_CONST.digits.substr(0,n),
        SQRT2_CONST.digits.substr(0,n),
        SQRT3_CONST.digits.substr(0,n),
        SQRT5_CONST.digits.substr(0,n)
    };
    
    for(int i=0;i<8;i++){
        //Buduje napis
        sf::String tmp = symbols[i];
        tmp += L" \u2248 ";
        tmp += digits[i];
        tmp += L"...";

        //Ustawia tekst, rozmiar i czcionkęs
        buttons_labels[i] = sf::Text(font,tmp,sclae);
        //Skaluje napisy
        // buttons_labels[i].setScale({0.25,0.25});
        //Ustawia punkt odniesienia na środek napisu
        sf::FloatRect bounds = buttons_labels[i].getLocalBounds();//auto?
        buttons_labels[i].setOrigin({
            bounds.position.x + bounds.size.x / 2.0f, // Odpowiednik left + width/2
            bounds.position.y + bounds.size.y / 2.0f  // Odpowiednik top + height/2
        });

        //Ustawia kolor etykiet
        buttons_labels[i].setFillColor(sf::Color(color));
        buttons_labels[i].setStyle(sf::Text::Bold);

        //Ustala pozycje
        if(i<4){
            buttons_labels[i].setPosition({width/3.73f, height/1.85f + i*height*0.1f});
        }
        else if(i>=4){
            buttons_labels[i].setPosition({3.0f*width/4.098f, height/1.85f + (i-4)*height*0.1f});
        }
    }


}
