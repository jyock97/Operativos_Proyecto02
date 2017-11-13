#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{
    //Configuracion de la pantalla
    sf::RenderWindow window(sf::VideoMode(800, 600), "TRON");
    window.setFramerateLimit(60);
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text text;
    text.setFont(font);
    text.setString("Hola!");
    text.setCharacterSize(24);
    text.setColor(sf::Color::Red);
    int width = 5;
    vector<sf::RectangleShape> player1(500, sf::RectangleShape(sf::Vector2f(width, width)));
    player1[0].setPosition(300,300);
    vector<sf::RectangleShape> player2(500, sf::RectangleShape(sf::Vector2f(width, width)));
    player2[0].setPosition(500,300);
    for(int i = 0;i<300;i++){
      player1[i].setFillColor(sf::Color::Blue);
      player2[i].setFillColor(sf::Color::Green);
    }
    int len = 1;
    int moveX = 0, move2X = 0;
    int moveY = -1, move2Y = -1;
    char direction = 'u', direction2 = 'u';
    bool won = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)window.close();
            if (event.type == sf::Event::KeyPressed){
              if(event.key.code==sf::Keyboard::A && direction != 'r'){
                direction = 'l';
                moveX = -1;moveY = 0;
              }
              if(event.key.code==sf::Keyboard::D && direction != 'l'){
                direction = 'r';
                moveX = 1;moveY = 0;
              }
              if(event.key.code==sf::Keyboard::W && direction != 'd'){
                direction = 'u';
                moveX = 0;moveY = -1;
              }
              if(event.key.code==sf::Keyboard::S && direction != 'u'){
                direction = 'd';
                moveX = 0;moveY = 1;
              }
              if(event.key.code==sf::Keyboard::Left && direction2 != 'r'){
                direction2 = 'l';
                move2X = -1;move2Y = 0;
              }
              if(event.key.code==sf::Keyboard::Right && direction2 != 'l'){
                direction2 = 'r';
                move2X = 1;move2Y = 0;
              }
              if(event.key.code==sf::Keyboard::Up && direction2 != 'd'){
                direction2 = 'u';
                move2X = 0;move2Y = -1;
              }
              if(event.key.code==sf::Keyboard::Down && direction2 != 'u'){
                direction2 = 'd';
                move2X = 0;move2Y = 1;
              }

            }
        }

        won = false;
        window.clear(sf::Color::Black);//limpia
        //jugador 1
        for(int i = 0;i<(len<300?len:300);i++){
          window.draw(player1[i]);
        }
        sf::Vector2f player1position = player1[((len-1)%300)].getPosition();
        player1[len%300].setPosition(player1position.x+(moveX*(width/2)),player1position.y+(moveY*(width/2)));

        //jugador 2
        for(int i = 0;i<(len<300?len:300);i++){
          window.draw(player2[i]);
        }
        sf::Vector2f player2position = player2[((len-1)%300)].getPosition();
        player2[len%300].setPosition(player2position.x+(move2X*(width/2)),player2position.y+(move2Y*(width/2)));
        sf::FloatRect player1head = player1[(len-1)%300].getGlobalBounds();
        sf::FloatRect player2head = player2[(len-1)%300].getGlobalBounds();
        for(int i = 0;i<(len<300?len:300);i++){
          sf::FloatRect player1box = player1[i].getGlobalBounds();
          if (player1box.intersects(player2head)){
            player1[0].setPosition(300,300);
            player2[0].setPosition(500,300);
            len = 0;
            direction = 'u';
            moveX = 0;moveY = -1;
            direction2 = 'u';
            move2X = 0;move2Y = -1;
            text.setString("Jugador Azul gana!");

            won = true;
          }
        }
        for(int j = 0;j<(len<300?len:300);j++){
          sf::FloatRect player2box = player2[j].getGlobalBounds();
          if (player2box.intersects(player1head)){
            player1[0].setPosition(300,300);
            player2[0].setPosition(500,300);
            len = 0;
            direction = 'u';
            moveX = 0;moveY = -1;
            direction2 = 'u';
            move2X = 0;move2Y = -1;
            text.setString("Jugador Verde gana!");
            won = true;
          }
        }
        len++;
        window.draw(text);
        window.display();
        if(won){
          sleep(2);
        }

    }

    return 0;
}
