#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctime>
#include <fcntl.h>

using namespace std;

sf::Font font;
int sock, newSock, useSock;  //file descriptor del socket
socklen_t clientLen;
int port = 5000;             //numero del puerto
int charReadWriteSize;
char ipAddr[16];
char clientIP[16];
char buffer[10];

struct sockaddr_in servAddr, cliAddr; //direccion del servidor y el cliente
struct hostent *server;

int retval;
struct timeval timeOut;
fd_set fdList;


void initServer(){
    fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    printf("Socket creando\n");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;   //direccion ip de la maquina
    servAddr.sin_port = htons(port);

    retval = bind(sock, (struct sockaddr*) &servAddr, sizeof(servAddr));

    printf("Binding hecho\n");

    listen(sock,2);     //listo para recibir conexiones

    clientLen = sizeof(cliAddr);
    newSock = accept(sock, (struct sockaddr*) &cliAddr, &clientLen);

    inet_ntop(AF_INET, &(cliAddr.sin_addr), clientIP, sizeof(clientIP));
    printf("Se realiza la conexion con el cliente: %s\n", clientIP);
    useSock = newSock;
}

sf::Text setText(){
  sf::Text text;
  text.setFont(font);
  text.setColor(sf::Color::Red);
  text.setString("Hola!");
  text.setCharacterSize(24);
  return text;
}


int main()
{
    initServer();
    int tempPos[2];
    // bind the socket to a port
    //Configuracion de la pantalla
    sf::RenderWindow window(sf::VideoMode(800, 600), "TRON");
    window.setFramerateLimit(60);
    font.loadFromFile("arial.ttf");

    sf::Text text = setText();

    int width = 5;
    //Obstaculo
    sf::RectangleShape wall(sf::Vector2f(25, 25));
    wall.setFillColor(sf::Color::Red);
    wall.setPosition(1000,1000);
    //crea los jugadores
    vector<sf::RectangleShape> player1(5000, sf::RectangleShape(sf::Vector2f(width, width)));
    player1[0].setPosition(300,300);
    vector<sf::RectangleShape> player2(5000, sf::RectangleShape(sf::Vector2f(width, width)));
    player2[0].setPosition(500,300);

    //pinta los jugadores
    for(int i = 0;i<5000;i++){
      player1[i].setFillColor(sf::Color::Blue);
      player2[i].setFillColor(sf::Color::Green);
    }
    //variables de direccion
    int len = 1;
    int moveX = 0, move2X = 0;
    int moveY = -1, move2Y = -1;
    char direction = 'u', direction2 = 'u';
    bool won = false;
    time_t first = time(0);
    while (window.isOpen())
    {
        time_t now = time(0);
        //if(now<first+1){
          if(read(useSock, buffer, sizeof(buffer)) > 0){
            int i = (rand()%700)+50;
            int j = (rand()%500)+50;
            wall.setPosition(i, j);
          }
          //first = now;
        //}
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
        for(int i = 0;i<(len<1600?len:1600);i++){
          window.draw(player1[i]);
        }
        sf::Vector2f player1position = player1[((len-1)%1600)].getPosition();
        player1[len%1600].setPosition(player1position.x+(moveX*(width/2)),player1position.y+(moveY*(width/2)));

        //jugador 2
        for(int i = 0;i<(len<1600?len:1600);i++){
          window.draw(player2[i]);
        }
        sf::Vector2f player2position = player2[((len-1)%1600)].getPosition();
        player2[len%1600].setPosition(player2position.x+(move2X*(width/2)),player2position.y+(move2Y*(width/2)));
        sf::FloatRect player1head = player1[(len-1)%1600].getGlobalBounds();
        sf::FloatRect player2head = player2[(len-1)%1600].getGlobalBounds();
        sf::FloatRect wallBox = wall.getGlobalBounds();
        for(int i = 0;i<(len<1600?len:1600);i++){
          sf::FloatRect player1box = player1[i].getGlobalBounds();
          sf::FloatRect player2box = player2[i].getGlobalBounds();
          if (player2head.intersects(wallBox)||player1box.intersects(player2head)||(player2box.intersects(player2head)&&i<(len-5)%1600)){
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

        for(int j = 0;j<(len<1600?len:1600);j++){
          sf::FloatRect player1box = player1[j].getGlobalBounds();
          sf::FloatRect player2box = player2[j].getGlobalBounds();
          if (player1head.intersects(wallBox)||player2box.intersects(player1head)||(player1box.intersects(player1head)&&j<(len-5)%1600)){
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
        window.draw(wall);
        window.display();
        if(won){
          sleep(2);
        }

    }

    return 0;
}
/*
const char s[2] = " ";
char *token = (char*)malloc(4* sizeof(char));
int i;

token = strtok(data, s);


while( token != NULL )
{
  tempPos[i++] = atoi(token);
  token = strtok(NULL, s);
}
cout << tempPos[0] << tempPos[1]<<endl;
*/
