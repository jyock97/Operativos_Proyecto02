#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstdlib>
using namespace std;

char data[100];
int sock, useSock;  //file descriptor del socket
int port = 5000;             //numero del puerto
char ipAddr[16] = "127.0.0.1";
char buffer[10];

struct sockaddr_in servAddr; //direccion del servidor y el cliente


void initClient(){

    sock = socket(AF_INET, SOCK_STREAM,0);

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(ipAddr);
    servAddr.sin_port = htons(port);
    connect(sock, (struct sockaddr*) &servAddr, sizeof(servAddr));
    printf("Se realiza conexion\n");
    useSock = sock;
}

void sedMessage(char *msg) {
    write(useSock, msg, strlen(msg));
}

int main()
{
    initClient();
    //Configuracion de la pantalla
    sf::RenderWindow window(sf::VideoMode(300, 300), "Brick Breaker");
    window.setFramerateLimit(60);
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text text;
    text.setFont(font);
    text.setString("Hola!");
    text.setCharacterSize(24);
    text.setColor(sf::Color::Red);

    bool pause = false, won = false;
    //se dibujan los bloques
    int x, y, width, length,cont = 0;
    width = 10;
    length = 30;
    vector<sf::RectangleShape> blocks(28, sf::RectangleShape(sf::Vector2f(length, width)));
    int active[28];
    x = 10;
    y = 10;
    int aux = 0;
    for(int i = 0;i<4;i++){//bloques
      for(int j = 0;j<7;j++){
        active[aux] = 1;
        blocks[aux].setPosition(x,y);
        blocks[aux].setFillColor(sf::Color::Red);
        x+=40;
        aux++;
      }
      x = 10;
      y+=15;
    }

    //se dibuja al jugador
    sf::RectangleShape player(sf::Vector2f(70, 10));
    player.setPosition(115,280);

    //se dibuja la bola
    int ballX = 145;
    int ballY = 240;
    double moveY = -1;
    double moveX = 1;
    sf::CircleShape ball(5.f);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(ballX, ballY);
    while (window.isOpen())
    {
        sf::Event event;
        sf::Vector2f playerposition = player.getPosition();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)window.close();
            if (event.type == sf::Event::KeyPressed){
              if(event.key.code==sf::Keyboard::Left){
                if(!pause&&playerposition.x>=0){player.move(-3,0);}
              }
              if(event.key.code==sf::Keyboard::Right){
                if(!pause&&playerposition.x<=235){player.move(3,0);}
              }
              if(event.key.code==sf::Keyboard::Escape){
                pause = !pause;
              }
            }
        }
        if(!pause){
          sf::FloatRect boundingBox;
          sf::FloatRect otherBox = ball.getGlobalBounds();
          for(int i = 0;i<28;i++){
            if(active[i]){
              boundingBox = blocks[i].getGlobalBounds();
              if (boundingBox.intersects(otherBox)){
                sedMessage("asd");
                blocks[i].setFillColor(sf::Color::Black);
                active[i] = 0;
                moveY*=-1;
              }
            }
          }
          sf::FloatRect playerBox = player.getGlobalBounds();
          if (otherBox.intersects(playerBox)){
            moveY*=-1;
          }

          //mueve la bola
          ball.move(moveX, moveY);
          sf::Vector2f position = ball.getPosition();
          if(position.x>= 300 || position.x<=0){moveX *=-1;}
          if(position.y<=0){moveY *=-1;}
          if(position.y>= 290){
            ballX = 145;
            ballY = 240;
            ball.setPosition(ballX, ballY);
            moveX = 1;
            moveY = -1;
            cont++;
          }
          window.clear(sf::Color::Black);//limpia
          bool noblocks = false;
          for(int i = 0;i<28;i++){
            if(active[i]){
              noblocks = true;
            }
            window.draw(blocks[i]);//dibuja los bloques
          }
          if(cont == 2){
            cont = 0;
            text.setString("Has Perdido! D:");
            window.draw(text);
            won = true;
            for(int i = 0;i<28;i++){
              active[i] = 1;
              blocks[i].setFillColor(sf::Color::Red);
            }
          }

          text.setPosition(70,100);
          window.draw(ball);//dibuja la bola
          window.draw(player);//dibuja el jugador
          if(!noblocks){
            text.setString("Has Ganado! :D");
            window.draw(text);
            won = true;
            for(int i = 0;i<28;i++){
              active[i] = 1;
              blocks[i].setFillColor(sf::Color::Red);
            }
            ballX = 145;
            ballY = 240;
            ball.setPosition(ballX, ballY);
            moveX = 1;
            moveY = -1;
          }
          window.display();
          if(won){
            sleep(2);
            won = false;
          }
        }
    }

    return 0;
}
