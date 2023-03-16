#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <time.h>
#include <list>
#include "ball.h"
#include "bush.h"
#include "food.h"
#include "player.h"
#include "entity.h"

using namespace std;
using namespace sf;

bool meet(Food* food, Ball& part)
{
    if(sqrt(((*food).getPosition().x - part.getPosition().x) * ((*food).getPosition().x - part.getPosition().x)
       + ((*food).getPosition().y - part.getPosition().y) * ((*food).getPosition().y - part.getPosition().y)) <
       part.getRadius() + (*food).getRadius()) {
        part.changeWeight(10);
        return true;
    }
    return false;
}

bool meet(Bush* bush, Ball& part)
{
    if(sqrt(((*bush).getPosition().x - part.getPosition().x) * ((*bush).getPosition().x - part.getPosition().x)
       + ((*bush).getPosition().y - part.getPosition().y) * ((*bush).getPosition().y - part.getPosition().y)) <
       part.getRadius() + (*bush).getRadius()) {
        return true;
    }
    return false;
}

bool meet(Ball& part1, Ball& part)
{
    if(sqrt((part1.getPosition().x - part.getPosition().x) * (part1.getPosition().x - part.getPosition().x)
       + (part1.getPosition().y - part.getPosition().y) * (part1.getPosition().y - part.getPosition().y)) <
       max(part.getRadius(), part1.getRadius())) {
        return true;
    }
    return false;
}

void setBackground(float Time, RenderWindow& window, Sprite& sprite, Player player)
{
    for(int i = 0; i < 40; ++i) {
        for(int j = 0; j < 40; ++j) {
            sprite.setPosition(960 + j * 512 - player.getCenter().x, 540 + i * 512 - player.getCenter().y);
            window.draw(sprite);
        }
    }
}

void menu(RenderWindow & window)
{
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("111.png");
	menuTexture2.loadFromFile("222.png");
	menuTexture3.loadFromFile("333.png");
	aboutTexture.loadFromFile("about.png");
	menuBackground.loadFromFile("Non-splitting.jpg");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(500, 422);
	menu2.setPosition(500, 502);
	menu3.setPosition(500, 582);
	menuBg.setPosition(320, 180);

	while (isMenu)
	{
		menu1.setColor(Color::Green);
		menu2.setColor(Color::Green);
		menu3.setColor(Color::Green);
		menuNum = 0;
		window.clear(Color(242, 251, 255));

		if (IntRect(500, 422, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(500, 502, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuNum = 2; }
		if (IntRect(500, 582, 300, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Blue); menuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) {isMenu = false;}
			if (menuNum == 2) {
                window.clear(Color(195, 195, 195));
                about.setPosition(304, 166);
                window.draw(about);
                window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape));
            }
			if (menuNum == 3) {window.close(); isMenu = false;}

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);

		window.display();
	}
}

bool food[MAP_HEIGHT][MAP_WIDTH];
bool bush[MAP_HEIGHT][MAP_WIDTH];

int main()
{
    srand(time(NULL));
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game", Style::Default);

    start: ;
    menu(window);

    Vector2f position;
    Clock clock;
    Player pl(1000);
    Image image;
    Texture texture;
    Sprite sprite;
    image.loadFromFile("space.bmp");
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    View view;
    view.setCenter(Vector2f(960, 540));
    view.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    list<Food*> f;
    list<Bush*> bb;
    list<Player*> pp;


    int cnt = 0, cnt1 = 0, cnt2 = 0;
    bool flag = false;

    while(window.isOpen())
    {
        double Time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        Time = Time / 16000;
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();
            if(event.type == Event::MouseWheelMoved) {
                    if(event.mouseWheel.delta > 0) {
                        view.zoom(1.0100f);
                        window.setView(view);
                    }
                    else {
                        view.zoom(0.9899f);
                        window.setView(view);
                    }
            }
        }
        position.x = Mouse::getPosition(window).x;
        position.y = Mouse::getPosition(window).y;

        window.setView(view);
        if(Keyboard::isKeyPressed(Keyboard::Space) && !flag && pl.getParts().size() <= 8) { //деление (split)
            pl.addBall(pl.getParts());
            flag = true;
        }
        if(!(Keyboard::isKeyPressed(Keyboard::Space)) && flag) {
            flag = false;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            view.zoom(1.0200f);
            window.setView(view);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            view.zoom(0.9799f);
            window.setView(view);
        }

        list<Player*> ppp;
        for(list<Player*>::iterator im = pp.begin(); im != pp.end(); ++im) {
            if((*im)->getParts().size() != 0) {ppp.push_back(*im);}
            else {--cnt2;}
        }
        pp = ppp;

        for(list<Player*>::iterator im = pp.begin(); im != pp.end(); ++im) { //деление (split)
            if((*im)->getWeight() % 30 == 0 && (*im)->getParts().size() == 1) {
                (*im)->addBall((*im)->getParts());
            }
        }


        if(cnt < 10000) { //появление новой еды
            int line, stolb;
            line = rand() % 20480;
            stolb = rand() % 20480;
            if(food[line][stolb] == false) {
                food[line][stolb] = true;
                Food* fo = new Food(line, 20480 - stolb, 10, pl.getCenter().x, pl.getCenter().y);
                f.push_back(fo);
                ++cnt;
            }
        }
        if(cnt1 < 50) { //спаун новых колючек
            int line, stolb;
            line = rand() % 20480;
            stolb = rand() % 20480;
            if(bush[line][stolb] == false) {
                bush[line][stolb] = true;
                Bush* bu = new Bush(line, 20480 - stolb, 300, pl.getCenter().x, pl.getCenter().y);
                bb.push_back(bu);
                ++cnt1;
            }
        }
        if(cnt2 < 10) { //спаун новых игроков-ботов
            Player* p = new Player(1000);
            list<Ball*> b = p->getParts();
            Vector2f coords;
            double u = rand() & 361;
            p->setDirection(cos(pi * (u / 180)), sin(pi * (u / 180)));
            for(list<Ball*>::iterator it = b.begin(); it != b.end(); ++it) {
                coords.x = (*it)->getXWindowCoordinate() + p->getDirection().x + (*it)->getRadius();
                coords.y = (*it)->getYWindowCoordinate() + p->getDirection().y + (*it)->getRadius();
                (*it)->update(Time, p->now, coords, pl.getCenter().x, pl.getCenter().y, b);
            }
            pp.push_back(p);
            ++cnt2;
        }


        list<Ball*> b = pl.getParts();
        if(b.size() == 0) { //в случае смерти игрока - выход в главное меню
            goto start;
        }
        double x = 0, y = 0, mass = 0;
        for(list<Ball*>::iterator ii = b.begin(); ii != b.end(); ++ii) { //определение центра масс игрока
            x += (*ii)->getWeight() * (*ii)->getPosition().x;
            y += (*ii)->getWeight() * (*ii)->getPosition().y;
            mass += (*ii)->getWeight();
        }
        pl.setCenter(x / mass, y / mass);
        for(list<Player*>::iterator it = pp.begin(); it != pp.end(); ++it) { //определение центра масс игроков-ботов
            double x = 0, y = 0, mass = 0;
            list<Ball*> ba = (*it)->getParts();
            for(list<Ball*>::iterator ii = ba.begin(); ii != ba.end(); ++ii) {
                x += (*ii)->getWeight() * (*ii)->getPosition().x;
                y += (*ii)->getWeight() * (*ii)->getPosition().y;
                mass += (*ii)->getWeight();
            }
            (*it)->setCenter(x / mass, y / mass);
        }

        pl.update(); //объединение (merge)
        b = pl.getParts();
        for(list<Player*>::iterator im = pp.begin(); im != pp.end(); ++im) { //объединение (merge)
            (*im)->update();
        }
        window.clear();


        setBackground(Time, window, sprite, pl);

        pl.clean();

        for(list<Ball*>::iterator ii = b.begin(); ii != b.end(); ++ii) { //цикл игрока
            (*ii)->update(Time, pl.now, position, pl.getCenter().x, pl.getCenter().y, b);
            if((*ii)->getPosition().x > MAP_WIDTH || (*ii)->getPosition().x < 0
               || (*ii)->getPosition().y > MAP_HEIGHT || (*ii)->getPosition().y < 0) { //выход за край карты
                (*ii)->isShown_ = false;
            }
            window.draw((*ii)->getBody());
            for(list<Food*>::iterator it = f.begin(); it != f.end(); ++it) { //взаимодействие и поглощение еды
                if(meet((*it), *(*ii))) {
                    food[(int)((*it)->getPosition().x)][(int)(20480 - (*it)->getPosition().y)] = false;
                    delete *it;
                    f.erase(it);
                    --cnt;
                }
                else if(ii == b.begin()) {
                    (*it)->setBody(960 + (*it)->getPosition().x - pl.getCenter().x - (*it)->getRadius(),
                           540 + (*it)->getPosition().y - pl.getCenter().y - (*it)->getRadius());
                    window.draw((*it)->getBody());
                }
            }
            /*for(list<Bush*>::iterator im = bb.begin(); im != bb.end(); ++im) { //расстановка кустов
                if((*ii)->getWeight() > (*im)->getWeight()) {
                    if(meet((*im), *(*ii))) {
                        bush[(int)((*im)->getPosition().x)][(int)(20480 - (*im)->getPosition().y)] = false;
                        delete *im;
                        //*im = NULL;
                        bb.erase(im);
                        --cnt1;
                        if(pl.getParts().size() < 16) {pl.crash(*ii);}
                        else {(*ii)->changeWeight((*im)->getWeight());}
                    }
                    else if(ii == b.begin()) {
                        (*im)->setBody(960 + (*im)->getPosition().x - pl.getCenter().x - (*im)->getRadius(),
                               540 + (*im)->getPosition().y - pl.getCenter().y - (*im)->getRadius());
                        window.draw((*im)->getBody());
                    }
                }
                else {
                    (*im)->setBody(960 + (*im)->getPosition().x - pl.getCenter().x - (*im)->getRadius(),
                               540 + (*im)->getPosition().y - pl.getCenter().y - (*im)->getRadius());
                    window.draw((*im)->getBody());
                }
            }*/
            for(list<Player*>::iterator im = pp.begin(); im != pp.end(); ++im) { //столкновение игрока и бота
                list<Ball*> ba = (*im)->getParts();
                for(list<Ball*>::iterator in = ba.begin(); in != ba.end(); ++in) {
                    if(meet(*(*ii), *(*in))) {
                        if((*ii)->getWeight() > 1.25 * (*in)->getWeight()) {
                            (*in)->isShown_ = false;
                            (*ii)->changeWeight((*in)->getWeight());
                        }
                        else if ((*in)->getWeight() > 1.25 * (*ii)->getWeight()) {
                            (*ii)->isShown_ = false;
                            (*in)->changeWeight((*in)->getWeight());
                        }
                    }
                }
            }
        }


        for(list<Player*>::iterator im = pp.begin(); im != pp.end(); ++im) { //цикл игрока-бота
            (*im)->clean();
            list<Ball*> ba = (*im)->getParts();
            Vector2f coords;
            float min_ = 1e6;
            for(list<Ball*>::iterator ii = ba.begin(); ii != ba.end(); ++ii) {
                if((*ii)->getPosition().x > MAP_WIDTH || (*ii)->getPosition().x < 0
                   || (*ii)->getPosition().y > MAP_HEIGHT || (*ii)->getPosition().y < 0) { //выход за край карты
                    (*ii)->isShown_ = false;
                }
                for(list<Food*>::iterator it = f.begin(); it != f.end(); ++it) { //взаимидеействие и поглощение еды
                    float dist = sqrt(((*im)->getCenter().x - (*it)->getPosition().x) * ((*im)->getCenter().x - (*it)->getPosition().x)
                            + ((*im)->getCenter().y - (*it)->getPosition().y) * ((*im)->getCenter().y - (*it)->getPosition().y));
                    if(meet((*it), *(*ii))) {
                        food[(int)((*it)->getPosition().x)][(int)(20480 - (*it)->getPosition().y)] = false;
                        delete *it;
                        f.erase(it);
                        --cnt;
                    }
                    else if(dist < min_) {
                        min_ = dist;
                        (*im)->setDirection((*it)->getPosition().x, (*it)->getPosition().y);
                    }
                }
                /*for(list<Bush*>::iterator ib = bb.begin(); ib != bb.end(); ++ib) { //взаимодействие с кустом
                    if((*ii)->getWeight() > (*ib)->getWeight()) {
                        if(meet((*ib), *(*ii))) {
                            bush[(int)((*ib)->getPosition().x)][(int)(20480 - (*ib)->getPosition().y)] = false;
                            delete *ib;
                            *ib = NULL;
                            bb.erase(ib);
                            --cnt1;
                            if((*im)->getParts().size() < 16) {(*im)->crash(*ii);}
                            else {(*ii)->changeWeight((*im)->getWeight());}
                        }
                    }
                }*/
            }
            for(list<Player*>::iterator in = pp.begin(); in != pp.end(); ++in) { //погоня бота за ботом
                if(im != in && abs((*in)->getCenter().x - (*im)->getCenter().x) < 880
                   && abs((*in)->getCenter().y - (*im)->getCenter().y) < 460
                   && (*in)->getWeight() * 1.25 < (*im)->getWeight()) {
                    (*im)->setDirection((*in)->getCenter().x, (*in)->getCenter().y);
                }
                else if (im != in && abs((*in)->getCenter().x - (*im)->getCenter().x) < 880
                   && abs((*in)->getCenter().y - (*im)->getCenter().y) < 460
                   && (*in)->getWeight() > (*im)->getWeight() * 1.25) {
                    float x, y;
                    if((*in)->getCenter().x < (*im)->getCenter().x) {
                        x = (*in)->getCenter().x + 2 * ((*im)->getCenter().x - (*in)->getCenter().x);
                    }
                    else {x = (*in)->getCenter().x - 2 * ((*in)->getCenter().x - (*im)->getCenter().x);}
                    if((*in)->getCenter().y < (*im)->getCenter().y) {
                        y = (*in)->getCenter().y + 2 * ((*im)->getCenter().y - (*in)->getCenter().y);
                    }
                    else {y = (*in)->getCenter().y - 2 * ((*in)->getCenter().y - (*im)->getCenter().y);}
                    (*im)->setDirection(x, y);
                }

            }
            if(abs(pl.getCenter().x - (*im)->getCenter().x) < 880
               && abs(pl.getCenter().y - (*im)->getCenter().y) < 460
               && pl.getWeight() * 1.25 < (*im)->getWeight()) {
                    (*im)->setDirection(pl.getCenter().x, pl.getCenter().y); //для бота погоня за игроком приоритетнее
            }
            else if (abs(pl.getCenter().x - (*im)->getCenter().x) < 880
                   && abs(pl.getCenter().y - (*im)->getCenter().y) < 460
                   && pl.getWeight() > (*im)->getWeight() * 1.25) {
                    float x, y;
                    if(pl.getCenter().x < (*im)->getCenter().x) {
                        x = pl.getCenter().x + 2 * ((*im)->getCenter().x - pl.getCenter().x);
                    }
                    else {x = pl.getCenter().x - 2 * (pl.getCenter().x - (*im)->getCenter().x);}
                    if(pl.getCenter().y < (*im)->getCenter().y) {
                        y = pl.getCenter().y + 2 * ((*im)->getCenter().y - pl.getCenter().y);
                    }
                    else {y = pl.getCenter().y - 2 * (pl.getCenter().y - (*im)->getCenter().y);}
                    (*im)->setDirection(x, y);
                }

            for(list<Ball*>::iterator ii = ba.begin(); ii != ba.end(); ++ii) { // передвижение двух игроков-ботов
                coords.x = (*ii)->getXWindowCoordinate() + (*im)->getDirection().x - (*ii)->getPosition().x + (*ii)->getRadius();
                coords.y = (*ii)->getYWindowCoordinate() + (*im)->getDirection().y - (*ii)->getPosition().y + (*ii)->getRadius();
                (*ii)->update(Time, (*im)->now, coords, pl.getCenter().x, pl.getCenter().y, ba);
                window.draw((*ii)->getBody());
            }
            for(list<Ball*>::iterator ii = ba.begin(); ii != ba.end(); ++ii) { //столкновение двух игроков-ботов
                for(list<Player*>::iterator id = pp.begin(); id != pp.end(); ++id) {
                    if(id != im) {
                        list<Ball*> baa = (*id)->getParts();
                        for(list<Ball*>::iterator in = baa.begin(); in != baa.end(); ++in) {
                            if(meet(*(*ii), *(*in))) {
                                if((*ii)->getWeight() > 1.25 * (*in)->getWeight()) {
                                    (*in)->isShown_ = false;
                                    (*ii)->changeWeight((*in)->getWeight());
                                }
                                else if ((*in)->getWeight() > 1.25 * (*ii)->getWeight()) {
                                    (*ii)->isShown_ = false;
                                    (*in)->changeWeight((*ii)->getWeight());
                                }
                            }
                        }
                    }
                }
            }
        }
        window.display();
    }

    return 0;
}
