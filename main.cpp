#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <cmath>
#include <fstream>
using namespace std;
using namespace sf;

void construct(Sprite item[], int start, int end, int amount, float x, int differenceX, int differenceY, float y)
{
    for (int i = start, j = end; i < amount; i++, j++)
       item[i].setPosition(Vector2f(x + (j * differenceX), y - (j * differenceY)));
}

void setGround(Sprite item[], int num, int width, int height, int x, int y)
{
    item[num].setTextureRect(IntRect(0, 0, width, height)); 
    item[num].setPosition(x, y);
}


struct player
{
    Texture sonicTex;
    Sprite player;
    RectangleShape PlayerColl, LeftColl, RightColl;
    Vector2f Velocity;
    int runrightanimation = 0, Idleanimation = 0, runleftanimation = 0, takingdamageanimation = 0, jumpinganimation = 0;
    int scoreValue = 0, lives = 5, hitCounter = -1, deathDealy = 0, FinalScore = 0;
    bool start = false, Running = false;
    bool on_ground = true, hitRight = false, hitLeft = false, RunningSound = false, HitAbove = false;
};


int main()
{
    // Variables
    bool isdead = false;
    bool isground = false;
    Clock gameclock;
    float delay = 0.1f;
    float deltatime = 0;
    float timer = 0;
    float acc = 0;
    int animleft = 0;
    int animright = 0;
    int jumpanim = 0;
    int idleanim = 0;
    int deadanim = 0;
    int ringanimator = 0;

    RenderWindow window(VideoMode(1940, 1080), "Sonic.exe", Style::Default);
    window.setFramerateLimit(60);

    // Structs
    // Sonic
    player sonic;
    sonic.player.setTextureRect(IntRect(48.87, 0, 48.87, 43));

    // Out Of Structs
    Vector2f Velocity;

    //Textures
    Texture spiketex;
    Texture platformtex;
    Texture Groundtex;
    Texture backgroundTexture;
    Texture groundTexture;
    Texture ringTexture;
    Texture rockTexture;
    Texture waterTexture;

    // Loading From Files
    sonic.sonicTex.loadFromFile("sonic22.png");
    platformtex.loadFromFile("Wall.png");
    Groundtex.loadFromFile("Wall.png");
    spiketex.loadFromFile("Spike.png");
    backgroundTexture.loadFromFile("background.png");
    groundTexture.loadFromFile("ground2.png");
    ringTexture.loadFromFile("ring.png");
    rockTexture.loadFromFile("rock.png");
    waterTexture.loadFromFile("water.png");

    // Sprites
    Sprite platform[20];
    Sprite Ground;
    Sprite spike[20];
    Sprite ring[100];

    // Setting Textures
    sonic.player.setTexture(sonic.sonicTex);
    for (int i = 0; i < 20; i++)
        platform[i].setTexture(platformtex);
    Ground.setTexture(Groundtex);
    for (int i = 0; i < 20; i++)
        spike[i].setTexture(spiketex);
    for (int i = 0; i < 100; i++)
        ring[i].setTexture(ringTexture);
    
    // Building
    construct(ring, 0, 0, 11, 960, 50, 0, 890);
    construct(ring, 11, 0, 22, 960, 50, 0, 828);
    construct(spike, 0, 0, 3, 2000, 200, 0, 856);
    construct(platform, 0, 0, 2, 2700, 400, 0, 650);
    construct(ring, 22, 0, 24, 2767, 400, 0, 590);
    construct(spike, 3, 0, 6, 3527, 200, 0, 856);
    construct(ring, 22, 0, 33, 4474, 50, 0, 890);
    construct(ring, 33, 0, 44, 4474, 50, 0, 828);
    construct(platform, 2, 0, 5, 6500, 350, 150, 828);
    construct(spike, 6, 0, 9, 6630, 350, 150, 735);
    construct(ring, 44, 0, 47, 6550, 350, 150, 770);
    construct(ring, 47, 0, 66, 7500, 150, 0, 890);
    construct(spike, 9, 0, 19, 7570, 300, 0, 856);

    // Sonic Position
    sonic.player.setPosition(350, 700);

    // Ground Position
    Ground.setPosition(200, 800);

    // Scaling
    sonic.player.setScale(Vector2f(2.f, 2.f));
    sonic.PlayerColl.setSize(Vector2f(45.f, 70.f));
    sonic.LeftColl.setSize(Vector2f(15.f, 70.f));
    sonic.RightColl.setSize(Vector2f(15.f, 70.f));
    for (int i = 0; i < 20; i++)
    platform[i].setScale(1.6, 1);
    Ground.setScale(11.9, 2);
    for (int i = 0; i < 20; i++)
    spike[i].setScale(0.5, 0.6);

    // Background
    Sprite background;
    background.setTexture(backgroundTexture);
    background.setTextureRect(IntRect(0, 0, 1920 * 33.33333, 1080));
    background.setPosition(Vector2f(-1000, 0));
    backgroundTexture.setRepeated(15);

    // Rocks
    Sprite rock1;
    Sprite rock3;
    rock1.setTexture(rockTexture);
    rock3.setTexture(rockTexture);
    rock1.setPosition(-690, 543);
    rock3.setPosition(-905, 747);
    rock3.setScale(0.5, 0.5);

    // Border
    RectangleShape border1(Vector2f(10, 1080));
    border1.setFillColor(Color::Transparent);

    // Ground/Water
    Sprite ground[70];
    Sprite water[70];
    for (int i = 0; i < 70; i++)
    {
        ground[i].setTexture(groundTexture);
        water[i].setTexture(waterTexture);
    }
    setGround(ground, 0, 3600, 128, -1000, 952);
    setGround(ground, 1, 3000, 128, 3400, 952);
    setGround(water, 0, 800, 128, 2600, 980);
    setGround(ground, 2, 4000, 128, 7500, 952);
    setGround(water, 1, 1500, 128, 6100, 980);
    groundTexture.setRepeated(15);
    waterTexture.setRepeated(15);

    // Fonts
    int score = 0;
    Font font;
    font.loadFromFile("font2.ttf");
    Text collector;
    collector.setFont(font);
    collector.setString("Score: " + to_string(score));
    collector.setPosition(-600, 100);
    collector.setCharacterSize(72);
    collector.setFillColor(Color::White);

  

    // Sounds
    SoundBuffer coinsoundbuffer;
    coinsoundbuffer.loadFromFile("coin.WAV");
    Sound coinsound;
    coinsound.setBuffer(coinsoundbuffer);
    SoundBuffer jumpsoundbuffer;
    jumpsoundbuffer.loadFromFile("jump.WAV");
    Sound jumpsound;
    jumpsound.setBuffer(jumpsoundbuffer);
    SoundBuffer deathsoundbuffer;
    deathsoundbuffer.loadFromFile("death2.WAV");
    Sound deathsound;
    deathsound.setBuffer(deathsoundbuffer);

    View cam(Vector2f(0.f, 0.f), Vector2f(1920.f, 1080.f));

    while (window.isOpen())
    {
        cam.setCenter(sonic.player.getPosition());
        cam.setCenter(Vector2f(sonic.player.getPosition().x, 600));
        window.setView(cam);
        gameclock.restart();
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::KeyPressed)
            {
                if ((event.key.code == Keyboard::Space) && isground && !isdead)
                {
                    sonic.Velocity.y = -30;
                    isground = false;
                    jumpsound.play();
                    cout << sonic.player.getPosition().x << endl;
                }


            }
            if (event.type == Event::KeyReleased)
            {
                if ((event.key.code == Keyboard::A))
                {
                    animleft = 0;

                    sonic.Velocity.x = 0;
                    Velocity.x = 0;
                    acc = 0;
                }
                if ((event.key.code == Keyboard::D))
                {
                    animright = 0;
                    sonic.Velocity.x = 0;
                    Velocity.x = 0;
                    acc = 0;
                }

            }
        }

        sonic.player.move(sonic.Velocity.x, sonic.Velocity.y);
        collector.move(Velocity.x, Velocity.y);

        // collision system and gravity system
        if (!isdead)
        {
            for (int i = 0; i < 65; i++)
            {
                //if sonic on the ground:
                if (sonic.player.getGlobalBounds().intersects(ground[i].getGlobalBounds()))
                {
                    //if not:
                    if ((sonic.player.getPosition().x >= ground[i].getPosition().x + 64000)) {}
                    else if ((sonic.PlayerColl.getPosition().x + 40 <= ground[i].getPosition().x)) {}

                    //then:
                    else
                    {
                        sonic.player.setPosition(sonic.player.getPosition().x, ground[i].getPosition().y - 75);
                        sonic.Velocity.y = -0.01;
                        isground = true;
                    }

                }
                //if sonic is above the ground:
                else
                {
                    //gravity:
                    sonic.Velocity.y += 0.02;
                }
            }


            for (int i = 0; i < 20; i++)
            {
                if (sonic.player.getGlobalBounds().intersects(platform[i].getGlobalBounds()))
                {

                    if ((sonic.player.getPosition().x >= platform[i].getPosition().x + 240))
                    {
                        sonic.player.setPosition(sonic.player.getPosition().x + 15, sonic.player.getPosition().y);

                        collector.setPosition(collector.getPosition().x + 15, collector.getPosition().y);
                        isground = false;
                    }
                    else if ((sonic.PlayerColl.getPosition().x + 15 <= platform[i].getPosition().x))
                    {
                        sonic.player.setPosition(sonic.player.getPosition().x - 15, sonic.player.getPosition().y);

                        collector.setPosition(collector.getPosition().x - 15, collector.getPosition().y);
                        isground = false;
                    }
                    else if ((sonic.PlayerColl.getPosition().y > platform[i].getPosition().y + 28))
                    {

                        sonic.player.setPosition(sonic.player.getPosition().x, sonic.player.getPosition().y + (40));
                        isground = false;
                        sonic.Velocity.y += 5;
                        collector.move(0, 0);
                    }
                    else
                    {
                        sonic.player.setPosition(sonic.player.getPosition().x, platform[i].getPosition().y - 75);
                        sonic.Velocity.y = -0.01;
                        isground = true;
                    }

                }
            }





            //jump animation
            if (!isground && !isdead)
            {
                if (timer < 0)
                {
                    jumpanim++;
                    jumpanim = jumpanim % 16;

                    sonic.player.setTextureRect(IntRect((jumpanim * 48.87), 2 * 59.4, 48.87, 46));
                    timer = delay;
                }
                else
                    timer -= deltatime;
            }




            //player collision
            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                Velocity.x = -8 + acc;
                sonic.Velocity.x = -8 + acc;

                if (isground && !isdead)
                {
                    if (timer < 0)
                    {
                        animleft++;
                        animleft = animleft % 23;
                        if (animleft == 22)
                        {


                            if (timer < 0)
                            {
                                animleft = 17;
                                animleft = animleft % 23;
                                animleft++;
                                acc -= 0.5;

                                sonic.player.setTextureRect(IntRect((animleft * 48.87), 1 * 59.4, 48.87, 46));
                                timer = delay;
                            }
                            else
                                timer -= deltatime;
                        }
                        else
                        {
                            sonic.player.setTextureRect(IntRect((animleft * 48.87), 1 * 59.4, 48.87, 46));
                            timer = 0.07;
                        }
                    }
                    else
                        timer -= deltatime;
                }


                sonic.player.setScale(-2, 2);

                sonic.player.setOrigin(0, 0);

            }


            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                sonic.Velocity.x = 8 + acc;
                Velocity.x = 8 + acc;
                if (isground && !isdead)
                {
                    if (timer < 0)
                    {
                        animright++;
                        animright = animright % 23;
                        if (animright == 22)
                        {

                            if (timer < 0)
                            {
                                animright = 17;
                                animright = animright % 23;
                                animright++;
                                acc += 1;

                                sonic.player.setTextureRect(IntRect((animright * 48.87), 1 * 59.4, 48.87, 46));
                                timer = 0.07;
                            }
                            else
                                timer -= deltatime;
                        }
                        else
                        {
                            sonic.player.setTextureRect(IntRect((animright * 48.87), 1 * 59.4, 48.87, 46));
                            timer = delay;
                        }
                    }
                    else
                        timer -= deltatime;
                }



                sonic.player.setScale(2, 2);

                sonic.player.setOrigin(sonic.player.getLocalBounds().width, 0);



            }
            if (isground && !(Keyboard::isKeyPressed(Keyboard::D)) && !(Keyboard::isKeyPressed(Keyboard::A)) && !(Keyboard::isKeyPressed(Keyboard::Space)))
            {
                if (timer < 0)
                {
                    idleanim++;
                    idleanim = idleanim % 8;

                    sonic.player.setTextureRect(IntRect((idleanim * 48.87), 0, 48.87, 41));
                    timer = delay;
                }
                else
                    timer -= deltatime;
            }
            
            for (int i = 0; i < 16; i++)
            {
                if (sonic.PlayerColl.getGlobalBounds().intersects(spike[i].getGlobalBounds()) && isground)
                {

                    if ((sonic.PlayerColl.getPosition().x >= spike[i].getPosition().x + 65))
                    {
                        isground = false;
                        sonic.player.setPosition(sonic.player.getPosition().x, sonic.player.getPosition().y - 200);
                        isdead = true;
                        deathsound.play();


                    }
                    else if ((sonic.PlayerColl.getPosition().x + 38 <= spike[i].getPosition().x))
                    {
                        isground = false;
                        sonic.player.setPosition(sonic.player.getPosition().x, sonic.player.getPosition().y - 200);
                        isdead = true;
                        deathsound.play();

                    }
                    else
                    {
                        isground = false;
                        sonic.Velocity.y = -0.01;
                        sonic.player.setPosition(sonic.player.getPosition().x, sonic.player.getPosition().y - 200);
                        isdead = true;
                        deathsound.play();
                    }

                }
            }

        }
        if (isdead)
        {

            for (int deadanim = 6; deadanim <= 7; deadanim++)
            {
                if (timer < 0)
                {
                    sonic.Velocity.y += 1;
                    sonic.Velocity.x = 0;
                    Velocity.x = 0;

                    sonic.player.setTextureRect(IntRect((deadanim * 45.5), 4 * 55, 47, 50));
                    timer = 0.7;
                }
                else
                    timer -= deltatime;
            }

        }
        if (sonic.player.getGlobalBounds().intersects(border1.getGlobalBounds()))
        {
            sonic.player.setPosition(sonic.player.getPosition().x + 15, sonic.player.getPosition().y);
            collector.setPosition(collector.getPosition().x + 15, collector.getPosition().y);
        }

        //Rings Disappearing When Collision
        for (int i = 0; i < 100; i++)
        {
            if (sonic.player.getGlobalBounds().intersects(ring[i].getGlobalBounds()) && !isdead)
            {
                ring[i].setScale(Vector2f(0, 0));
                score++;
                coinsound.play();
            }
        }




        //Rings Animation
        ringanimator++;
        ringanimator %= 10 * 4;
        for (int i = 0; i < 100; i++)
        {
            ring[i].setTextureRect(IntRect((ringanimator / 4) * 64, 0, 64, 62));
        }
        if (sonic.PlayerColl.getPosition().y > window.getSize().y)
        {
            sonic.player.setPosition(300, 720);
            collector.setPosition(-600, 100);
            isdead = false;
            isground = true;
        }


        collector.setString("Score: " + to_string(score));

        //position of sonic collision
        sonic.PlayerColl.setPosition(sonic.player.getPosition().x - 70, sonic.player.getPosition().y + 10);
        sonic.LeftColl.setPosition(sonic.player.getPosition().x - 70, sonic.player.getPosition().y + 10);
        sonic.RightColl.setPosition(sonic.player.getPosition().x - 40, sonic.player.getPosition().y + 10);

        //Update
        window.clear();

        //Draw
        window.draw(collector);
        window.draw(background);
        for (int i = 0; i < 2; i++)
            window.draw(water[i]);
        for (int i = 0; i < 3; i++)
        window.draw(ground[i]);
        window.draw(border1);
        for (int i = 0; i < 100; i++)
            window.draw(ring[i]);
        window.draw(rock1);
        window.draw(rock3);
        window.draw(sonic.player);
        window.draw(collector);
        for (int i = 0; i < 20; i++)
        window.draw(platform[i]);
        for (int i = 0; i < 20; i++)
        window.draw(spike[i]);
        window.draw(collector);

        //Display
        window.display();
        deltatime = gameclock.getElapsedTime().asSeconds();

    }
}
//
//struct player
//{
//	Texture sonicTex;
//	Sprite player;
//	RectangleShape PlayerColl, LeftColl, RightColl;
//	Vector2f Velocity;
//	int runrightanimation = 0, Idleanimation = 0, runleftanimation = 0, takingdamageanimation = 0, jumpinganimation = 0;
//	int scoreValue = 0, lives = 5, hitCounter = -1, deathDealy = 0, FinalScore = 0;
//	bool start = false, Running = false;
//	bool on_ground = true, hitRight = false, hitLeft = false, RunningSound = false, HitAbove = false;
//};
//
//
//int main()
//{
//
//    bool isground = false;
//    Clock gameclock;
//    float delay = 0.1f;
//    float deltatime = 0;
//    float timer = 0;
//
//    int animleft = 0;
//    int animright = 0;
//    int jumpanim = 0;
//    int idleanim = 0;
//
//    RenderWindow window(VideoMode(1920, 1080), "Sonic.exe", Style::Default);
//    window.setFramerateLimit(60);
//
//    RectangleShape ground(Vector2f(1500, 80));
//    ground.setPosition(200, 800);
//    RectangleShape block(Vector2f(200, 30));
//    block.setPosition(1000, 520);
//
//
//
//    sonic
//    player sonic;
//    set texture;
//    sonic.player.setTexture(sonic.sonicTex);
//    load images
//    sonic.sonicTex.loadFromFile("sonic22.png");
//     scaling
//    sonic.player.setScale(Vector2f(2.f, 2.f));
//    sonic.player.setTextureRect(IntRect(48.87, 0, 48.87, 43));
//    sonic.player.setPosition(250, 700);
//     collision scaling
//    sonic.PlayerColl.setSize(Vector2f(45.f, 70.f));
//    sonic.LeftColl.setSize(Vector2f(15.f, 70.f));
//    sonic.RightColl.setSize(Vector2f(15.f, 70.f));
//
//    Textures
//	Texture sonicTexture;
//	Texture backgroundTexture;
//	Texture groundTexture;
//	Texture ringTexture;
//	Texture rockTexture;
//	Texture treeTexture;
//	Texture spikeTexture;
//	Texture blockTexture;
//
//
//	Textures Loading
//	backgroundTexture.loadFromFile("background.png");
//	groundTexture.loadFromFile("ground2.png");
//	sonicTexture.loadFromFile("sonic.png");
//	ringTexture.loadFromFile("ring.png");
//	rockTexture.loadFromFile("rock.png");
//	spikeTexture.loadFromFile("spike.png");
//	treeTexture.loadFromFile("tree.png");
//	blockTexture.loadFromFile("block.png");
//
//    Sprite background[15];
//    background[0].setTexture(backgroundTexture);
//    background[0].setPosition(Vector2f(-1920, 0));
//    for (int i = 1, j = 0; i <= 15; i++, j++)
//    {
//        background[i].setTexture(backgroundTexture);
//        background[i].setPosition(Vector2f((j * 1920), 0));
//    }
//
//
//    
//    while (window.isOpen())
//    {
//        gameclock.restart();
//        Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == Event::Closed)
//            {
//                window.close();
//            }
//            if (event.type == Event::KeyPressed)
//            {
//                if ((event.key.code == Keyboard::Space) && isground)
//                {
//                    sonic.Velocity.y = -30;
//                    isground = false;
//                }
//
//
//            }
//            if (event.type == Event::KeyReleased)
//            {
//                if ((event.key.code == Keyboard::A))
//                {
//                    animleft = 0;
//                }
//                if ((event.key.code == Keyboard::D))
//                {
//                    animright = 0;
//                }
//
//            }
//        }
//        sonic.player.move(0, sonic.Velocity.y);
//
//
//        if (sonic.player.getGlobalBounds().intersects(ground.getGlobalBounds()))
//        {
//            if ((sonic.player.getPosition().x >= ground.getPosition().x + 1550))
//            {
//                sonic.player.setPosition(sonic.player.getPosition().x + 15, sonic.player.getPosition().y);
//                isground = false;
//
//            }
//            else if ((sonic.PlayerColl.getPosition().x + 40 <= ground.getPosition().x))
//            {
//                sonic.player.setPosition(sonic.player.getPosition().x - 15, sonic.player.getPosition().y);
//                isground = false;
//            }
//            else
//            {
//                sonic.player.setPosition(sonic.player.getPosition().x, ground.getPosition().y - 80);
//                sonic.Velocity.y = -0.01;
//                isground = true;
//            }
//
//        }
//        else if (sonic.player.getGlobalBounds().intersects(block.getGlobalBounds()))
//        {
//            if ((sonic.player.getPosition().x >= block.getPosition().x + 260))
//            {
//                sonic.player.setPosition(sonic.player.getPosition().x + 15, sonic.player.getPosition().y - 5);
//
//
//            }
//            else if ((sonic.PlayerColl.getPosition().x + 40 <= block.getPosition().x))
//            {
//                sonic.player.setPosition(sonic.player.getPosition().x - 15, sonic.player.getPosition().y - 5);
//
//            }
//            else if ((sonic.PlayerColl.getPosition().y > block.getPosition().y + 30))
//            {
//
//                sonic.player.setPosition(sonic.player.getPosition().x, sonic.player.getPosition().y + (40));
//
//            }
//            else
//            {
//                sonic.player.setPosition(sonic.player.getPosition().x, block.getPosition().y - 80);
//                sonic.Velocity.y = -0.01;
//                isground = true;
//            }
//
//        }
//        else
//        {
//            isground = false;
//            sonic.Velocity.y += 1.5;
//
//        }
//        if (!isground)
//        {
//            if (timer < 0)
//            {
//                jumpanim++;
//                jumpanim = jumpanim % 16;
//
//                sonic.player.setTextureRect(IntRect((jumpanim * 48.87), 2 * 59.4, 48.87, 46));
//                timer = delay;
//            }
//            else
//                timer -= deltatime;
//        }
//
//
//
//
//        player collision
//        if (Keyboard::isKeyPressed(Keyboard::A))
//        {
//
//            if (isground)
//            {
//                if (timer < 0)
//                {
//                    animleft++;
//                    animleft = animleft % 23;
//                    if (animleft == 22)
//                    {
//
//
//                        if (timer < 0)
//                        {
//                            animleft = 17;
//                            animleft = animleft % 23;
//                            animleft++;
//
//                            sonic.player.setTextureRect(IntRect((animleft * 48.87), 1 * 59.4, 48.87, 46));
//                            timer = delay;
//                        }
//                        else
//                            timer -= deltatime;
//                    }
//                    else
//                    {
//                        sonic.player.setTextureRect(IntRect((animleft * 48.87), 1 * 59.4, 48.87, 46));
//                        timer = delay;
//                    }
//                }
//                else
//                    timer -= deltatime;
//            }
//
//            sonic.player.setScale(-2, 2);
//            sonic.player.move(-5, 0);
//            sonic.player.setOrigin(0, 0);
//        }
//
//
//        if (Keyboard::isKeyPressed(Keyboard::D))
//        {
//
//
//
//            if (isground)
//            {
//                if (timer < 0)
//                {
//                    animright++;
//                    animright = animright % 23;
//                    if (animright == 22)
//                    {
//
//                        if (timer < 0)
//                        {
//                            animright = 17;
//                            animright = animright % 23;
//                            animright++;
//
//                            sonic.player.setTextureRect(IntRect((animright * 48.87), 1 * 59.4, 48.87, 46));
//                            timer = delay;
//                        }
//                        else
//                            timer -= deltatime;
//                    }
//                    else
//                    {
//                        sonic.player.setTextureRect(IntRect((animright * 48.87), 1 * 59.4, 48.87, 46));
//                        timer = delay;
//                    }
//                }
//                else
//                    timer -= deltatime;
//            }
//
//            sonic.player.setScale(2, 2);
//            sonic.player.move(5, 0);
//
//            sonic.player.setOrigin(sonic.player.getLocalBounds().width, 0);
//        }
//        if (isground && !(Keyboard::isKeyPressed(Keyboard::D)) && !(Keyboard::isKeyPressed(Keyboard::A)) && !(Keyboard::isKeyPressed(Keyboard::Space)))
//        {
//            if (timer < 0)
//            {
//                idleanim++;
//                idleanim = idleanim % 8;
//
//                sonic.player.setTextureRect(IntRect((idleanim * 48.87), 0, 48.87, 41));
//                timer = delay;
//            }
//            else
//                timer -= deltatime;
//        }
//
//
//
//
//        sonic.PlayerColl.setPosition(sonic.player.getPosition().x - 70, sonic.player.getPosition().y + 30);
//        sonic.LeftColl.setPosition(sonic.player.getPosition().x - 70, sonic.player.getPosition().y + 30);
//        sonic.RightColl.setPosition(sonic.player.getPosition().x - 40, sonic.player.getPosition().y + 30);
//
//        UPDATE
//
//        window.clear();
//
//
//
//        DRAW
//        window.draw(background[0]);
//        for (int i = 1; i <= 14; i++)
//            window.draw(background[i]);
//        window.draw(sonic.player);
//        window.draw(ground);
//        window.draw(block);
//       
//
//
//
//
//        DISPLAY
//        window.display();
//        deltatime = gameclock.getElapsedTime().asSeconds();
//
//    }
//}

	//CircleShape circ(150);

	//CircleShape square(300, 4);

	//circ.setRadius(50);

	//circ.setPointCount(300);



	//SoundBuffer walkBuffer;

	//walkBuffer.loadFromFile("Walk.wav");

	//Sound walkSound;

	//walkSound.setBuffer(walkBuffer);


	//ConvexShape randomshape(3);

	//randomshape.setPoint(0, Vector2f(40, 70));
	//randomshape.setPoint(1, Vector2f(60, 70));
	//randomshape.setPoint(2, Vector2f(200, 200));

	//RectangleShape loine(Vector2f(10, 400));

	//Vertex line[] =
	//{
	//	Vertex(Vector2f(25,10)),
	//	Vertex(Vector2f(400, 100))
	//};

	//Vertex point;

	//point.position = Vector2f(300, 300);
	//point.color = Color::Red;

	//VertexArray lines(Lines,2);

	//lines[0].position = Vector2f(200, 150);
	//lines[0].color = Color::Red;

	//lines[1].position = Vector2f(400, 90);
	//lines[1].color = Color::Green;

	//VertexArray triangle(Triangles,3); (if you want to draw a triangle)
	//VertexArray quad(Quads,4); (if you want to draw a quad)
	//VertexArray linesstrip(LinesStrip,3); (if you want to draw >)
	//VertexArray trianglestrip(TriangleStrip,4) (if you want to draw <>)
	//VertexArray trianglesfan(TriangleFan,4) (if you want to draw some random shape)
	//continue just like you're drawing a line (line 66, line 69)

	//VertexArray quad(Quads, 4);

	//quad[0].position = Vector2f(5, 5);
	//quad[1].position = Vector2f(200, 5);
	//quad[2].position = Vector2f(200, 200);
	//quad[3].position = Vector2f(5, 200);

	//quad[0].texCoords = Vector2f(0, 0);
	//quad[1].texCoords = Vector2f(50, 0);
	//quad[2].texCoords = Vector2f(50, 100);
	//quad[3].texCoords = Vector2f(0, 100);

		// how to resize the window for a bigger picture than the window and see the rest of it:

		//while (window.pollEvent(event))
		//{
		//	if (event.type == Event::Resized)
		//	{
		//		FloatRect visibleArea(0, 0, event.size.width, event.size.height);
		//		window.setView(View(visibleArea));
		//	}
		//}

		//Keyboard Events//

		/*Event e;

		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case Event::Closed:
			{
				window.close();
				break;
			}
			case Event::KeyPressed:
			{
				cout << "KeyPressed" << endl;
				break;
			}
			case Event::KeyReleased:
			{
				cout << "KeyReleased" << endl;
				if (e.key.code == Keyboard::J)
				{
					cout << "J RELEASED" << endl;
				}
				break;
			}

			}
			switch (e.key.code)
			{
			case Keyboard::B:
			{
				cout << "B" << endl;
				break;
			}

			}
		}*/

		//Mouse Events//

		/*Event e;

		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
				break;
			}

			if (e.type == Event::MouseButtonPressed)
			{
				cout << "mousepressed" << endl;
				if (e.key.code == Mouse::Left)
				{
					cout << "mouseleft" << endl;
				}
			}

			if (e.type == Event::MouseButtonReleased)
			{
				cout << "MouseReleased" << endl;
				if (e.key.code == Mouse::Right)
				{
					cout << "R Released" << endl;
				}
			}

			if (e.type == Event::MouseWheelMoved)
			{
				cout << "mouse wheel moved" << endl;
				cout << e.mouseWheel.delta << endl;
			}

			if (e.type == Event::MouseMoved)
			{
				cout << "MouseMoved" << endl;
				cout << e.mouseMove.x << endl;
			}

			if (e.type == Event::MouseEntered)
			{
				cout << "MouseEntered" << endl;
			}

			if (e.type == Event::MouseLeft)
			{
				cout << "MouseLeft" << endl;
			}

		}
*/

//Window Events//

/*Event e;

while (window.pollEvent(e))
{
	if (e.type == Event::Closed)
	{
		window.close();
	}

	if (e.type == Event::Resized)
	{
		cout << "WindowResized" << endl;
		cout << e.size.width << ":" << e.size.height << endl;
	}

	if (e.type == Event::GainedFocus)
	{
		cout << "GainedFocus" << endl;
	}

	if (e.type == Event::LostFocus)
	{
		cout << "LostFocus" << endl;
	}
}*/

//Text Events//

/*Event e;

while (window.pollEvent(e))
{

	if (e.type == Event::Closed)
	{
		window.close();
	}

	if (e.type == Event::TextEntered)
	{
		cout << "Text has been entered." << endl;
		if (e.text.unicode == 65)
		{
			cout << "CAPA" << endl;
		}
	}

}*/

//Live Keyboard Input//

/*if (Keyboard::isKeyPressed(Keyboard::A))
{
	cout << "LeftMovement" << endl;
}
if (Keyboard::isKeyPressed(Keyboard::D))
{
	cout << "RightMovement" << endl;
}*/

//Live Mouse Input//

/*if (Mouse::isButtonPressed(Mouse::Left))
{
	cout << "Fire!" << endl;
}

cout << Mouse::getPosition(window).x << endl;

Mouse::setPosition(Vector2i(40, 50), window);*/

//Set Sprite Texture Rect Size//

/*Texture t;

if (!t.loadFromFile("sonic.png"))
{
	cout << "Error loading sonic texture ):" << endl;
}

t.setRepeated(true);

Sprite sprite(t);

sprite.scale(0.09, 0.09);

sprite.setTextureRect(IntRect(0, 0, 600, 600));
)

Event event;

while (window.pollEvent(event))
{
	if (event.type == Event::Closed)
	{
		window.close();
	}
}

window.draw(sprite);

while (window.isOpen())
{*/

//Transform trans;

//trans.rotate(45, Vector2f(100, 100));

//RenderStates states;

//states.transform = trans;

//states.texture = &groundTexture;

//Views

//View view(FloatRect(0, 0, 300, 300));

//window.setView(view);

//view.move(Vector2f(300, 300));

//view.setCenter(Vector2f(300, 300));

//view.rotate(25);

//view.setRotation(25);

//window.setView(view);

//view.zoom(0.5); //zoom in

//view.zoom(2.0); //zoom out

//view.setSize(Vector2f(4000,2000));

	//font
//int rings = 0;
//Font font;
//font.loadFromFile("font.ttf");
//Text ringsCollected;
//ringsCollected.setFont(font);
//ringsCollected.setString("Rings: " + to_string(rings));
//ringsCollected.setCharacterSize(72);
//ringsCollected.setFillColor(Color::Black);


///Old Code


//int main()
//{
//
//	RenderWindow window(VideoMode(1920, 1080), "SFML Works");
//	
//	//Textures
//	Texture sonicTexture;
//	Texture backgroundTexture;
//	Texture groundTexture;
//	Texture ringTexture;
//	Texture rockTexture;
//	Texture treeTexture;
//	Texture spikeTexture;
//	Texture blockTexture;
//
//
//	//Textures Loading
//	backgroundTexture.loadFromFile("background.png");
//	groundTexture.loadFromFile("ground2.png");
//	sonicTexture.loadFromFile("sonic.png");
//	ringTexture.loadFromFile("ring.png");
//	rockTexture.loadFromFile("rock.png");
//	spikeTexture.loadFromFile("spike.png");
//	treeTexture.loadFromFile("tree.png");
//	blockTexture.loadFromFile("block.png");
//
//	//Sonic
//	Sprite sonic;
//	sonic.setTexture(sonicTexture);
//	sonic.setOrigin(100, 128);
//	sonic.setPosition(Vector2f(200, 890));
//	sonic.setScale(0.5, 0.5);
//
//	//Background
//	Sprite background[15];
//	background[0].setTexture(backgroundTexture);
//	background[0].setPosition(Vector2f(-1920, 0));
//	for (int i = 1, j = 0; i <= 15; i++, j++)
//	{
//		background[i].setTexture(backgroundTexture);
//		background[i].setPosition(Vector2f((j * 1920), 0));
//	}
//
//	//Rocks
//	Sprite rock1;
//	Sprite rock2;
//	Sprite rock3;
//	Sprite rock4;
//	rock1.setTexture(rockTexture);
//	rock2.setTexture(rockTexture);
//	rock3.setTexture(rockTexture);
//	rock4.setTexture(rockTexture);
//	rock1.setPosition(-690, 543);
//	rock2.setPosition(7652, 543);
//	rock3.setPosition(-905, 747);
//	rock4.setPosition(8217, 747);
//	rock3.setScale(0.5, 0.5);
//	rock4.setScale(0.5, 0.5);
//
//	//Blocks
//	Sprite block[100];
//	for (int i = 0; i < 100; i++)
//	{
//		block[i].setTexture(blockTexture);
//		block[i].setScale(Vector2f(2, 1));
//	}
//	
//
//
//
//	//Borders
//	RectangleShape border1(Vector2f(10, 1080));
//	RectangleShape border2(Vector2f(10, 1080));
//	border1.setFillColor(Color::Transparent);
//	border2.setFillColor(Color::Transparent);
//	border2.setPosition(Vector2f(7670, 0));
//
//	//Ground
//	Sprite ground[1000];
//	//Ground Behind Sonic
//	for (int i = 0, j = -1; i < 8; i++, j--)
//	{
//		ground[i].setScale(Vector2f(2, 2));
//		ground[i].setTexture(groundTexture);
//		ground[i].setPosition(Vector2f((j * 256), 952));
//	}
//	//Ground Infront Of Sonic
//	for (int i = 8, j = 0; i < 100; i++, j++)
//	{
//		ground[i].setScale(Vector2f(2, 2));
//		ground[i].setTexture(groundTexture);
//		ground[i].setPosition(Vector2f((j * 256), 952));
//	}
//
//	//Rings
//	Sprite ring[100];
//	int ringanimator = 0;
//	for (int i = 0; i < 11; i++)
//	{
//		ring[i].setTexture(ringTexture);
//		ring[i].setPosition(Vector2f(960 + (i * 50), 890));
//	}
//	for (int i = 11, j = 0; i < 22; i++, j++)
//	{
//		ring[i].setTexture(ringTexture);
//		ring[i].setPosition(Vector2f(960 + (j * 50), 890 - 62));
//	}
//
//	//Spikes
//	Sprite spike[100];
//	for (int i = 0; i < 100; i++)
//	{
//		spike[i].setTexture(spikeTexture);
//	}
//
//	spike[0].setPosition(Vector2f(1700, 822));
//	spike[1].setPosition(Vector2f(2100, 822));
//	spike[2].setPosition(Vector2f(2500, 822));
//
//
//	//Views
//	View cam(Vector2f(0.f, 0.f), Vector2f(1920.f, 1080.f));
//	
//	while (window.isOpen())
//	{
//		//View
//		cam.setCenter(sonic.getPosition());
//		cam.setCenter(Vector2f(sonic.getPosition().x, 600));
//		window.setView(cam);
//
//		//Close
//		Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == Event::Closed)
//			{
//				window.close();
//			}
//		}
//
//		//Right Movement
//		if (Keyboard::isKeyPressed(Keyboard::D))
//		{
//			sonic.move(Vector2f(5, 0));
//			sonic.setScale(Vector2f(0.5,0.5));
//			sonic.setOrigin(Vector2f(100, 128));
//		}
//
//		//Left Movement
//		if (Keyboard::isKeyPressed(Keyboard::A))
//		{
//			sonic.move(Vector2f(-5, 0));
//			sonic.setScale(Vector2f(-0.5, 0.5));
//			sonic.setOrigin(Vector2f(100, 128));
//		}
//
//		//Mouse Position
//		if (Mouse::isButtonPressed(Mouse::Left))
//		{
//			cout << "X: " << Mouse::getPosition().x << endl;
//			cout << "Y: " << Mouse::getPosition().y << endl;
//		}
//
//		//Border Collisions
//		if (sonic.getGlobalBounds().intersects(border1.getGlobalBounds()))
//		{
//			sonic.move(Vector2f(5, 0));
//		}
//
//		//Rings Disappearing When Collision
//		for (int i = 0; i < 22; i++)
//		{
//			if (sonic.getGlobalBounds().intersects(ring[i].getGlobalBounds()))
//			{
//				ring[i].setScale(Vector2f(0, 0));
//			}
//		}
//
//		cout << sonic.getPosition().x << endl;
//
//		//Rings Animation
//		ringanimator++;
//		ringanimator %= 10;
//		for (int i = 0; i < 22; i++)
//		{
//			ring[i].setTextureRect(IntRect((ringanimator * 64), 0, 64, 62));
//		}
//
//		//Display
//		window.clear();
//		for (int i = 0; i <= 14; i++)
//			window.draw(background[i]);
//		window.draw(sonic);
//		for (int i = 0; i < 65; i++)
//			window.draw(ground[i]);
//		window.draw(border1);
//		for (int i = 0; i < 22; i++)
//			window.draw(ring[i]);
//		window.draw(rock1);
//		window.draw(rock3);
//		for (int i = 0; i < 3; i++)
//		window.draw(spike[i]);
//		window.display();
//	}
//}
