#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
using namespace std;

int placed[9];

int check(){
  int count = 0;
  for(int i = 0;i<7;i){//revisa horizontales
    for(int j = 0;j<3;j++){
      if(placed[i+j]==1){count++;}
      if(placed[i+j]==2){count--;}
    }
    if(count == 3){return 1;}
    if(count == -3){return 2;}
    i+=3;
    count = 0;
  }

  for(int i = 0;i<3;i++){//revisa verticales
    for(int j = 0;j<7;j){
      if(placed[i+j]==1){count++;}
      if(placed[i+j]==2){count--;}
      j+=3;
    }
    if(count == 3){return 1;}
    if(count == -3){return 2;}
    count = 0;
  }

  for(int i = 0;i<9;i){//revisa diagonal 1
    if(placed[i]==1){count++;}
    if(placed[i]==2){count--;}
    i+=4;
  }
  if(count == 3){return 1;}
  if(count == -3){return 2;}
  count = 0;

  for(int i = 2;i<7;i){//revisa diagonal 2
    if(placed[i]==1){count++;}
    if(placed[i]==2){count--;}
    i+=2;
  }
  if(count == 3){return 1;}
  if(count == -3){return 2;}
  return 0;
}

int main()
{
    //Configuracion de la pantalla
    sf::RenderWindow window(sf::VideoMode(300, 300), "Brick Breaker");
    window.setFramerateLimit(60);
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text text, text2;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setPosition(60,190);

    text2.setFont(font);
    text2.setCharacterSize(30);
    text2.setPosition(100,72);
    text2.setString("GANA");

    vector<sf::Text> x(5);
    vector<sf::Text> y(5);

    for(int i = 0;i<9;i++){
      placed[i] = 0;
    }

    for(int i = 0;i<5;i++){
      x[i].setFont(font);
      x[i].setString("X");
      x[i].setCharacterSize(60);

      y[i].setFont(font);
      y[i].setString("O");
      y[i].setCharacterSize(60);
    }

    sf::RectangleShape barx1(sf::Vector2f(280, 30));
    sf::RectangleShape barx2(sf::Vector2f(280, 30));
    sf::RectangleShape bary1(sf::Vector2f(30, 280));
    sf::RectangleShape bary2(sf::Vector2f(30, 280));

    barx1.setPosition(10,75);
    barx2.setPosition(10,195);
    bary1.setPosition(75,10);
    bary2.setPosition(195,10);

    window.clear(sf::Color::Black);//limpia
    window.draw(barx1);
    window.draw(barx2);
    window.draw(bary1);
    window.draw(bary2);

    int count1 = 0;
    int count2 = 0;

    bool player1 = true;
    bool player2 = false;
    bool played = false;
    int plays = 0;
    while (window.isOpen()){
        sf::Event event;

        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)window.close();
            if (event.type == sf::Event::KeyPressed){
              played = false;
              if(event.key.code==sf::Keyboard::Numpad7 && !placed[0]){
                played = true;
                if(player1){
                  placed[0] = 1;
                  x[count1].setPosition(15,5);
                }else{
                  placed[0] = 2;
                  y[count2].setPosition(15,5);
                }
              }
              if(event.key.code==sf::Keyboard::Numpad8 && !placed[1]){
                played = true;
                if(player1){
                  placed[1] = 1;
                  x[count1].setPosition(125,5);
                }else{
                  placed[1] = 2;
                  y[count2].setPosition(125,5);
                }
              }
              if(event.key.code==sf::Keyboard::Numpad9 && !placed[2]){
                played = true;
                if(player1){
                  placed[2] = 1;
                  x[count1].setPosition(235,5);
                }else{
                  placed[2] = 2;
                  y[count2].setPosition(235,5);
                }
              }
              if(event.key.code==sf::Keyboard::Numpad4 && !placed[3]){
                played = true;
                if(player1){
                  placed[3] = 1;
                  x[count1].setPosition(15,110);
                }else{
                  placed[3] = 2;
                  y[count2].setPosition(15,110);
                }
              }
              if(event.key.code==sf::Keyboard::Numpad5 && !placed[4]){
                played = true;
                if(player1){
                  placed[4] = 1;
                  x[count1].setPosition(125,110);
                }else{
                  placed[4] = 2;
                  y[count2].setPosition(125,110);
                }
              }
              if(event.key.code==sf::Keyboard::Numpad6 && !placed[5]){
                played = true;
                if(player1){
                  placed[5] = 1;
                  x[count1].setPosition(235,110);
                }else{
                  placed[5] = 2;
                  y[count2].setPosition(235,110);
                }
              }
              if(event.key.code==sf::Keyboard::Numpad1 && !placed[6]){
                played = true;
                if(player1){
                  placed[6] = 1;
                  x[count1].setPosition(15,220);
                }else{
                  placed[6] = 2;
                  y[count2].setPosition(15,220);
                }
              }
              if(event.key.code==sf::Keyboard::Numpad2 && !placed[7]){
                played = true;
                if(player1){
                  placed[7] = 1;
                  x[count1].setPosition(125,220);
                }else{
                  placed[7] = 2;
                  y[count2].setPosition(125,220);
                }
              }
              if(event.key.code==sf::Keyboard::Numpad3 && !placed[8]){
                played = true;
                if(player1){
                  placed[8] = 1;
                  x[count1].setPosition(235,220);
                }else{
                  placed[8] = 2;
                  y[count2].setPosition(235,220);
                }
              }
              if(played){
                plays++;
                if(player1){
                  window.draw(x[count1]);
                  count1++;
                }else{
                  window.draw(y[count2]);
                  count2++;
                }
                player1 = !player1;
                player2 = !player2;
              }
            }
        }
        int end = 0;
        end = check();
        if(end == 1){
          text.setColor(sf::Color::Red);
          text.setString("JUGADOR 1");
          text2.setColor(sf::Color::Red);
          window.draw(text);
          window.draw(text2);
        }
        if(end == 2){
          text.setColor(sf::Color::Blue);
          text.setString("JUGADOR 2");
          text2.setColor(sf::Color::Blue);
          window.draw(text);
          window.draw(text2);
        }
        if(plays == 9){
          end = 1;
          text.setColor(sf::Color::Green);
          text.setString("   EMPATE");
          window.draw(text);
        }
        window.display();
        if(end){
          sleep(4);
          for(int i = 0;i<9;i++){
            placed[i] = 0;
          }
          window.clear(sf::Color::Black);
          window.draw(barx1);
          window.draw(barx2);
          window.draw(bary1);
          window.draw(bary2);
          player1 = true;
          player2 = false;
          count1 = 0;
          count2 = 0;
          plays = 0;
        }
    }

    return 0;
}
