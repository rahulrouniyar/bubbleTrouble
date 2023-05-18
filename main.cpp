#include<SFML/Graphics.hpp>
#include<iostream>
#include<cmath>

sf::RenderWindow window(sf::VideoMode(800, 600), "BUBBLE TROUBLE");
sf::RectangleShape rectUp, rectDown, rectLeft, rectRight, bullet, charRect;
int count1, count2, count3, count4;
bool LevelOne = true, LevelTwo = false, LevelThree = false, LevelFour = false;
bool shootBulletAgain = true, playGame = true;
int calcDecr(float);
void initRect(sf::RectangleShape&, sf::Vector2f const&, sf::Vector2f const&, sf::Color const&);

class Bubble1;
class Bubble2;
class Bubble3;

class Bubble
{
protected:
	int pos_y;
	int incr, decr;
	bool senseBubble;
	bool movBubUp, movBubDown, movBubLeft, movBubRight, controlSplitBallsFirst, movSplitBalls;
public:
	sf::CircleShape circle;
	Bubble(int a, bool b, bool c, bool d, bool e)
	{
		pos_y = a;
		movBubUp = b;
		movBubDown = c;
		movBubLeft = d;
		movBubRight = e;
		senseBubble = true;
		controlSplitBallsFirst = false;
		movSplitBalls = false;
		circle.setRadius(40.f);
		circle.setFillColor(sf::Color::Magenta);
	}
	void initCirc(float size, sf::Vector2f const& pos, sf::Color const& color);
	void immediateControl();
	void mainControl();
	void setDecr();
	void direction(bool, bool, bool, bool);
};
void Bubble::initCirc(float size, sf::Vector2f const& pos, sf::Color const& color)
{
	circle.setRadius(size);
	circle.setPosition(pos);
	circle.setFillColor(color);
}
void Bubble::immediateControl()
{
	sf::Vector2f bubblePos = circle.getPosition();
	if (bubblePos.y == pos_y)
	{
		movBubUp = false;
		movBubDown = true;
	}
	if (circle.getGlobalBounds().intersects(rectLeft.getGlobalBounds()))
	{
		movBubLeft = false;
		movBubRight = true;
	}
	if (circle.getGlobalBounds().intersects(rectRight.getGlobalBounds()))
	{
		movBubLeft = true;
		movBubRight = false;
	}
	if (movBubUp)
		bubblePos.y -= 0.1 * decr--;
	if (movBubLeft)
		bubblePos.x--;
	if (movBubRight)
		bubblePos.x++;
	circle.setPosition(sf::Vector2f(bubblePos.x, bubblePos.y));
}
void Bubble::mainControl()
{
	sf::Vector2f bubblePos = circle.getPosition();
	if (circle.getGlobalBounds().intersects(rectDown.getGlobalBounds()))
	{
		movBubUp = true;
		movBubDown = false;
		decr = calcDecr(600 - pos_y - 20 - circle.getRadius());
	}
	if (bubblePos.y == pos_y)
	{
		movBubUp = false;
		movBubDown = true;
		incr = 1;
	}
	if (circle.getGlobalBounds().intersects(rectLeft.getGlobalBounds()))
	{
		movBubLeft = false;
		movBubRight = true;
	}
	if (circle.getGlobalBounds().intersects(rectRight.getGlobalBounds()))
	{
		movBubLeft = true;
		movBubRight = false;
	}
	if (movBubUp)
		bubblePos.y -= 0.1 * decr--;
	if (movBubDown)
		bubblePos.y += 0.1 * incr++;
	if (movBubLeft)
		bubblePos.x--;
	if (movBubRight)
		bubblePos.x++;
	circle.setPosition(sf::Vector2f(bubblePos.x, bubblePos.y));
}
void Bubble::setDecr()
{
	sf::Vector2f position = circle.getPosition();
	float height = position.y - pos_y;
	decr = calcDecr(height);
}
void Bubble::direction(bool a, bool b, bool c, bool d)
{
	movBubUp = a;
	movBubDown = b;
	movBubLeft = c;
	movBubRight = d;
}
class Bubble3 :public Bubble
{
public:
	Bubble3(int a, bool b, bool c, bool d, bool e) :Bubble(a, b, c, d, e)
	{}
	void controlBubble();
	void replay();
};
void Bubble3::controlBubble()
{
	if (circle.getGlobalBounds().intersects(bullet.getGlobalBounds()) && senseBubble)
	{
		shootBulletAgain = false;
		senseBubble = false;
		sf::Vector2f charPos = charRect.getPosition();
		bullet.setPosition(sf::Vector2f(charPos.x + 4, charPos.y+40));
		circle.setRadius(0);
		//circle.setPosition(sf::Vector2f(900, 200));
		count1++;
		count2++;
		count3++;
		count4++;
	}
	if (circle.getGlobalBounds().intersects(charRect.getGlobalBounds()) && senseBubble)
		playGame = false;
}
void Bubble3::replay()
{
	if (!playGame)
	{
		senseBubble = true;
		movBubUp = true;
		movBubDown = false;
		circle.setRadius(10);
	}
}

class Bubble2 :public Bubble
{
private:
	Bubble3 tertBubble[2] = { Bubble3(250, true, false, false, true),Bubble3(250, true, false, true, false) };
public:
	Bubble2(int a, bool b, bool c, bool d, bool e) :Bubble(a, b, c, d, e)
	{
	}
	void controlBubble();
	void replay();
};
void Bubble2::controlBubble()
{

	if (bullet.getGlobalBounds().intersects(circle.getGlobalBounds()) && senseBubble)
	{
		shootBulletAgain = false;
		movSplitBalls = true;
		senseBubble = false;
		controlSplitBallsFirst = true;
		sf::Vector2f charPos = charRect.getPosition();
		bullet.setPosition(sf::Vector2f(charPos.x + 4, charPos.y+40));
		circle.setRadius(0);
		sf::Vector2f bubblePos = circle.getPosition();
		tertBubble[0].initCirc(10, sf::Vector2f(bubblePos.x + 5, bubblePos.y), sf::Color::Yellow);
		tertBubble[1].initCirc(10, sf::Vector2f(bubblePos.x - 5, bubblePos.y), sf::Color::Yellow);
		circle.setPosition(sf::Vector2f(100, 200));
		tertBubble[0].setDecr();
		tertBubble[1].setDecr();
	}
	if (movSplitBalls)
	{
		if (controlSplitBallsFirst)
		{
			tertBubble[0].immediateControl();
			tertBubble[1].immediateControl();
			controlSplitBallsFirst = false;
		}
		else
		{
			tertBubble[0].mainControl();
			tertBubble[1].mainControl();
		}
		window.draw(tertBubble[0].circle);
		window.draw(tertBubble[1].circle);
		tertBubble[0].controlBubble();
		tertBubble[1].controlBubble();
	}
	if (circle.getGlobalBounds().intersects(charRect.getGlobalBounds()) && senseBubble)
		playGame = false;
}
void Bubble2::replay()
{
	if (!playGame)
	{
		circle.setRadius(20);
		tertBubble[0].replay();
		tertBubble[1].replay();
		senseBubble = true;
		controlSplitBallsFirst = true;
		movSplitBalls = false;
		movBubUp = true;
		movBubDown = false;
		tertBubble[0].direction(true, false, false, true);
		tertBubble[1].direction(true, false, true, false);
	}
}
class Bubble1 :public Bubble
{
private:
	Bubble2 secBubble[2] = { Bubble2(300, true, false, false, true),Bubble2(300, true, false, true, false) };
public:
	Bubble1(int a, bool b, bool c, bool d, bool e) :Bubble(a, b, c, d, e)
	{
	}
	void controlBubble();
	void replay(int);
};
//controls bubbles during usual circumstances
void Bubble1::controlBubble()
{
	if (senseBubble)
	{
		window.draw(circle);
		std::cout << 1;
	}
	Bubble::mainControl();
	if (bullet.getGlobalBounds().intersects(circle.getGlobalBounds()) && senseBubble)
	{
		shootBulletAgain = false;
		movSplitBalls = true;
		senseBubble = false;
		controlSplitBallsFirst = true;
		sf::Vector2f charPos = charRect.getPosition();
		bullet.setPosition(sf::Vector2f(charPos.x + 4, charPos.y+40));
		//circle.setRadius(0);
		sf::Vector2f bubblePos = circle.getPosition();
		secBubble[0].initCirc(20, sf::Vector2f(bubblePos.x + 10, bubblePos.y), sf::Color::Yellow);
		secBubble[1].initCirc(20, sf::Vector2f(bubblePos.x - 10, bubblePos.y), sf::Color::Yellow);
		secBubble[0].setDecr();
		secBubble[1].setDecr();
	}
	if (movSplitBalls)
	{
		if (controlSplitBallsFirst)
		{
			secBubble[0].immediateControl();
			secBubble[1].immediateControl();
			controlSplitBallsFirst = false;
		}
		else
		{
			secBubble[0].mainControl();
			secBubble[1].mainControl();
		}
		window.draw(secBubble[0].circle);
		window.draw(secBubble[1].circle);
		std::cout << 2;
		secBubble[0].controlBubble();
		secBubble[1].controlBubble();
	}
	if (circle.getGlobalBounds().intersects(charRect.getGlobalBounds())&&senseBubble)
		playGame = false;
}
void Bubble1::replay(int x)
{
	if (!playGame)
	{
		initCirc(40, sf::Vector2f(x, 400), sf::Color::Red);
		secBubble[0].replay();
		secBubble[1].replay();
		senseBubble = true;
		controlSplitBallsFirst = true;
		movSplitBalls = false;
		movBubLeft = false;
		movBubRight = true;
		movBubUp = false;
		movBubDown = true;
		secBubble[0].direction(true, false, false, true);
		secBubble[1].direction(true, false, true, false);
		count1 = count2 = count3 = count4 = 0;
	}
}

int main()
{
	window.setFramerateLimit(60);//60 frames per second

	Bubble1 Level1[1] = { Bubble1(400, false, true, false, true) };
	Bubble1 Level2[2] = { Level1[0],Level1[0] };
	Bubble1 Level3[3] = { Level1[0],Level1[0],Level1[0] };
	Bubble1 Level4[4] = { Level1[0],Level1[0],Level1[0],Level1[0] };

	Level1[0].initCirc(40, sf::Vector2f(400, 400), sf::Color::Red);

	Level2[0].initCirc(40, sf::Vector2f(267, 400), sf::Color::Red);
	Level2[1].initCirc(40, sf::Vector2f(534, 400), sf::Color::Red);

	Level3[0].initCirc(40, sf::Vector2f(200, 400), sf::Color::Red);
	Level3[1].initCirc(40, sf::Vector2f(400, 400), sf::Color::Red);
	Level3[2].initCirc(40, sf::Vector2f(600, 400), sf::Color::Red);

	Level4[0].initCirc(40, sf::Vector2f(160, 400), sf::Color::Red);
	Level4[1].initCirc(40, sf::Vector2f(320, 400), sf::Color::Red);
	Level4[2].initCirc(40, sf::Vector2f(480, 400), sf::Color::Red);
	Level4[3].initCirc(40, sf::Vector2f(640, 400), sf::Color::Red);

	initRect(charRect, sf::Vector2f(10, 20), sf::Vector2f(395, 560), sf::Color::Black);
	initRect(rectUp, sf::Vector2f(800, 20), sf::Vector2f(0, 0), sf::Color::Yellow);
	initRect(rectDown, sf::Vector2f(800, 20), sf::Vector2f(0, 580), sf::Color::Yellow);
	initRect(rectLeft, sf::Vector2f(20, 600), sf::Vector2f(0, 0), sf::Color::Yellow);
	initRect(rectRight, sf::Vector2f(20, 600), sf::Vector2f(780, 0), sf::Color::Yellow);
	initRect(bullet, sf::Vector2f(2, 600), sf::Vector2f(399, 600), sf::Color::Black);

	bool movCharRight = false, movCharLeft = false;
	bool shootBullet = false;

	//input handling
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Key::Right:
					movCharRight = true;
					movCharLeft = false;
					shootBullet = false;//prevents shooting while moving
					break;
				case sf::Keyboard::Key::Left:
					movCharRight = false;
					movCharLeft = true;
					shootBullet = false;//prevents shooting while moving
					break;
				case sf::Keyboard::Key::Up:
					shootBullet = true;
					break;
				}
			}
			if (event.type == sf::Event::EventType::KeyReleased)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Key::Right:
					movCharRight = false;
					break;
				case sf::Keyboard::Key::Left:
					movCharLeft = false;
					break;
				case sf::Keyboard::Key::Up:
					shootBullet = false;
					shootBulletAgain = true;
					sf::Vector2f charPos = charRect.getPosition();
					bullet.setPosition(sf::Vector2f(charPos.x + 4, charPos.y+40));
					break;
				}
			}
			if (event.type == sf::Event::EventType::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			movCharLeft = false;
			movCharRight = false;
		}

		window.clear(sf::Color::Magenta);
		window.draw(rectUp);
		window.draw(rectRight);
		window.draw(rectLeft);


		if (playGame)
		{
			if (LevelOne)
			{
				Level1[0].controlBubble();
				if (count1 == 4)
				{
					LevelOne = false;
					LevelTwo = true;
				}
			}
			else if (LevelTwo)
			{
				Level2[0].controlBubble();
				Level2[1].controlBubble();
				if (count2 == 12)
				{
					LevelTwo = false;
					LevelThree = true;
				}
			}
			else if (LevelThree)
			{
				Level3[0].controlBubble();
				Level3[1].controlBubble();
				Level3[2].controlBubble();
				if (count3 == 24)
				{
					LevelThree = false;
					LevelFour = true;
				}
			}
			else if (LevelFour)
			{
				Level4[0].controlBubble();
				Level4[1].controlBubble();
				Level4[2].controlBubble();
				Level4[3].controlBubble();
				if (count4 == 40)
				{
					LevelFour = false;
				}
			}
		}

		//                    BULLET
//prevents bullet from being produced once it touches top and Up key is still being pressed
		if (bullet.getGlobalBounds().intersects(rectUp.getGlobalBounds()))
		{
			shootBulletAgain = false;
			//bullet.setPosition(sf::Vector2f(charPos.x + 4, charPos.y+40));
		}


		//produce bullet
		if (shootBullet && shootBulletAgain && playGame)
		{

			window.draw(bullet);
			bullet.move(sf::Vector2f(0, -5));
		}

		window.draw(rectDown);//ensures bullet doesn't overlap lower rectangle


		//                   CHARACTER
		sf::Vector2f charPos = charRect.getPosition();
		if (charPos.x >= 770)
			movCharRight = false;
		if (charPos.x <= 20)
			movCharLeft = false;
		if (movCharRight && playGame)
		{
			charRect.move(sf::Vector2f(4, 0));
			sf::Vector2f charPos = charRect.getPosition();
			bullet.setPosition(charPos.x + 4, charPos.y+40);
			//bullet.setPosition(sf::Vector2f(charPos.x + 4, charPos.y+40));//updates base position of bullet with movement of character
		}
		if (movCharLeft && playGame)
		{
			charRect.move(sf::Vector2f(-4, 0));
			sf::Vector2f charPos = charRect.getPosition();
			bullet.setPosition(charPos.x + 4, charPos.y+40);
			//bullet.setPosition(sf::Vector2f(charPos.x + 4, charPos.y+40));//updates base position of bullet with movement of character
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
		{
			LevelOne = true;
			LevelTwo = false;
			LevelThree = false;
			LevelFour = false;
			Level1[0].replay(400);

			Level2[0].replay(267);
			Level2[1].replay(534);

			Level3[0].replay(200);
			Level3[1].replay(400);
			Level3[2].replay(600);

			Level4[0].replay(160);
			Level4[1].replay(320);
			Level4[2].replay(480);
			Level4[3].replay(640);
			charRect.setPosition(sf::Vector2f(395, 560));
			playGame = true;
		}
		window.draw(charRect);
		window.display();
	}
	return 0;
}
int calcDecr(float height)
{
	float discriminant = 1 - 4 * 1 * 20 * (-height);
	float root1 = (-1 + sqrt(discriminant)) / 2;
	float root2 = (-1 - sqrt(discriminant)) / 2;
	return (root1 > 0) ? root1 : root2;
}
void initRect(sf::RectangleShape& rect, sf::Vector2f const& size, sf::Vector2f const& pos, sf::Color const& color)
{
	rect.setSize(size);
	rect.setFillColor(color);
	rect.setPosition(pos);    }

