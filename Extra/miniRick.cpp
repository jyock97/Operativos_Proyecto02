#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <iostream>

using namespace std;
int main(){
  sf::Texture texture;
  texture.loadFromFile("rick.png");
  sf::Texture texture2;
  texture2.loadFromFile("morty.png");

  sf::RenderWindow window(sf::VideoMode(1920, 1080), "Tiny Rick");
  window.setFramerateLimit(60);

  sf::Text text, endText;
  sf::Font font;
  font.loadFromFile("arial.ttf");
  text.setFont(font);
  text.setColor(sf::Color::Red);
  text.setString("Tiny Rick");
  text.setCharacterSize(40);
  text.setPosition(840, 50);
  endText.setFont(font);
  endText.setColor(sf::Color::Red);
  endText.setCharacterSize(80);
  endText.setPosition(640, 450);

  sf::Sprite player;
  player.setTexture(texture2);
  sf::Vector2f scale1 = player.getScale();
  player.setScale(scale1.x * 0.2, scale1.y * 0.2);
  player.setPosition(900, 800);
  int playerlife = 3;

  sf::Sprite boss;
  boss.setTexture(texture);
  boss.setPosition(850, 300);
  sf::Vector2f scale = boss.getScale();
  boss.setScale(scale.x * 0.4, scale.y * 0.4);

  std::vector<sf::RectangleShape> bullets(5, sf::RectangleShape(sf::Vector2f(10,20)));
  int activeBullets[5];
  int lastBullet;
  for(int i = 0; i<5;i++)
    activeBullets[i] = 0;

  int bossX = 3;
  int life = 30;

  std::vector<sf::RectangleShape> lifeBar(life, sf::RectangleShape(sf::Vector2f(30,10)));
  int x = 510, y = 100;

  for(int i = 0; i<life;i++){
    lifeBar[i].setFillColor(sf::Color::Red);
    lifeBar[i].setPosition(x,y);
    x+=30;
  }

  std::vector<sf::RectangleShape> bossBullets(500, sf::RectangleShape(sf::Vector2f(10,20)));
  int activeBossBullets[500];
  float direction[500][2];
  int lastBossBullet;
  for(int i = 0; i<500;i++){
    activeBossBullets[i] = 0;
    bossBullets[i].setFillColor(sf::Color::Red);
  }

  clock_t startTime = clock(); //Start timer
  double secondsPassed;
  double secondsToDelay = 30000;

  lastBullet = 0;
  lastBossBullet = 0;
  bool end = false;
  while (window.isOpen()){
    if(!end){
      secondsPassed = (clock() - startTime);
      if(secondsPassed >= secondsToDelay){
        cout << "YES";
        startTime = clock();
        sf::Vector2f loc = boss.getPosition();
        activeBossBullets[lastBossBullet%500] = 1;
        bossBullets[lastBossBullet%500].setPosition(loc.x+80,loc.y+40);
        double temp = (rand()%2)-1;
        double temp2 = (1+rand()%2)-0.7;
        direction[lastBossBullet%500][0] = ((float) rand()) / (float) RAND_MAX;
        direction[lastBossBullet%500][1] = ((float) rand()) / (float) RAND_MAX;
        direction[lastBossBullet%500][0]*=temp;
        direction[lastBossBullet%500][1]*=temp2;
        lastBossBullet++;
      }
      sf::Event event;
      sf::Vector2f plaloc = player.getPosition();
      while (window.pollEvent(event)){
          if (event.type == sf::Event::Closed)window.close();
          if(plaloc.y>400){

            if (plaloc.x>20 && event.key.code==sf::Keyboard::W && event.key.code==sf::Keyboard::A)player.move(-10,-10);
            if (plaloc.x<1800 && event.key.code==sf::Keyboard::W && event.key.code==sf::Keyboard::D)player.move(10,-10);
            if (event.key.code==sf::Keyboard::W)player.move(0,-10);
          }
          if(plaloc.x>20){

            if (event.key.code==sf::Keyboard::A)player.move(-10,0);
          }
          if(plaloc.y<1020){

            if (plaloc.x>20 && event.key.code==sf::Keyboard::S && event.key.code==sf::Keyboard::A)player.move(-10,10);
            if (plaloc.x<1800 && event.key.code==sf::Keyboard::S && event.key.code==sf::Keyboard::D)player.move(10,10);
            if (event.key.code==sf::Keyboard::S)player.move(0,10);
        }
          if(plaloc.x<1800)
            if (event.key.code==sf::Keyboard::D)player.move(10,0);
          if (event.key.code==sf::Keyboard::Space){
            sf::Vector2f loc = player.getPosition();
            activeBullets[lastBullet%5] = 1;
            bullets[lastBullet%5].setPosition(loc.x+30, loc.y);
            lastBullet++;
          }
      }
      sf::Vector2f loc = boss.getPosition();
      if(loc.x>1600||loc.x<120){
        bossX*=-1;
      }
      boss.move(bossX, 0);

      window.clear(sf::Color::Black);

      sf::FloatRect bossColl = boss.getGlobalBounds();
      for(int i = 0;i<5;i++){
        if(activeBullets[i]){
          sf::FloatRect bulletColl = bullets[i].getGlobalBounds();
          bullets[i].move(0,-5);
          if(bulletColl.intersects(bossColl)){
            life--;
            activeBullets[i] = 0;
          }
          window.draw(bullets[i]);
        }
      }

      for(int i = 0; i<life;i++){
        window.draw(lifeBar[i]);
      }

      sf::FloatRect playerColl = player.getGlobalBounds();
      for(int i =0;i<500;i++){
        if(activeBossBullets[i]){
          sf::FloatRect bulletColl = bossBullets[i].getGlobalBounds();
          if(playerColl.intersects(bulletColl)){
            playerlife--;
            activeBossBullets[i] = 0;
          }
          sf::Vector2f loc = boss.getPosition();
          bossBullets[i].move(5*direction[i][0],5*direction[i][1]);
          window.draw(bossBullets[i]);
          if(loc.y>1100){
            activeBossBullets[i] = 0;
          }
        }
      }

      if(playerlife == 0){
        endText.setString("HAS PERDIDO");
        window.draw(endText);
        end = true;
      }
      if(life <= 0){
        endText.setString("HAS GANADO :D");
        endText.setColor(sf::Color::Green);
        window.draw(endText);
        end = true;
      }
      window.draw(boss);
      window.draw(player);
      window.draw(text);
    }
    window.display();
  }


  return 0;
}
