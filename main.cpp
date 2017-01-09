#include "demo.hpp"
#include <SFML/Graphics.hpp>
#include <stdlib.h>

Demo demo; // global for demo purpose
bool thread_running; // contains if the thread is searching a path
bool demo_running; // contains if the app is running
sf::Vector2i demo_start;
sf::Vector2i demo_goal;

#define DEMO_SIZE_X 100
#define DEMO_SIZE_Y 100

// randomize the start and goal points, and start a new search
void newDemoRun()
{
    do
    {
        demo_start.x = rand() % DEMO_SIZE_X;
        demo_start.y = rand() % DEMO_SIZE_Y;
    }while(demo.getNodeScore(demo_start) != 1);

    do
    {
        demo_goal.x = rand() % DEMO_SIZE_X;
        demo_goal.y = rand() % DEMO_SIZE_Y;
    }while(demo.getNodeScore(demo_goal) != 1 || demo_goal == demo_start);

    sf::IntRect area; // testing the limited area
    if(demo_start.x <= demo_goal.x)
    {
        area.left = demo_start.x - 5;
        area.width = demo_goal.x + 5;
    }
    else
    {
        area.width = demo_start.x + 5;
        area.left = demo_goal.x - 5;
    }
    if(demo_start.y <= demo_goal.y)
    {
        area.top = demo_start.y - 5;
        area.height = demo_goal.y + 5;
    }
    else
    {
        area.height = demo_start.y + 5;
        area.top = demo_goal.y - 5;
    }
    demo.getAstar().enableSearchArea(area);
    thread_running = true;
}

// the function running in a thread. will search the path or call newDemoRun()
void thread_func()
{
    srand(time(NULL));
    while(demo_running)
    {
        if(thread_running)
        {
            if(demo.runStepByStep(demo_start, demo_goal))
            {
                demo.updatePath();
                thread_running = false;
                //sf::sleep(sf::seconds(1));
            }
        }
        else
        {
            newDemoRun();
        }
    }
}

// main function initializing stuffs and managing the SFML window
int main()
{
    // for testing
    // creating a map
    demo.createNodeMap(sf::Vector2i(DEMO_SIZE_X, DEMO_SIZE_Y));
    // adding obstacles
    demo.placeWall(sf::Vector2i(0, 0), sf::Vector2i(DEMO_SIZE_X-1, 0));
    demo.placeWall(sf::Vector2i(0, DEMO_SIZE_Y-1), sf::Vector2i(DEMO_SIZE_X-1, DEMO_SIZE_Y-1));
    demo.placeWall(sf::Vector2i(0, 0), sf::Vector2i(0, DEMO_SIZE_Y-1));
    demo.placeWall(sf::Vector2i(DEMO_SIZE_X-1, 0), sf::Vector2i(DEMO_SIZE_X-1, DEMO_SIZE_Y-1));
    demo.placeWall(sf::Vector2i(20, 15), sf::Vector2i(45, 25), 50);
    demo.placeWall(sf::Vector2i(23, 38), sf::Vector2i(34, 48), 30);
    demo.placeWall(sf::Vector2i(40, 1), sf::Vector2i(47, 23), 10);
    demo.placeWall(sf::Vector2i(10, 10), sf::Vector2i(25, 13));
    demo.placeWall(sf::Vector2i(36, 20), sf::Vector2i(39, 44), 20);
    demo.placeWall(sf::Vector2i(15, 36), sf::Vector2i(27, 45));
    demo.placeWall(sf::Vector2i(25, 25), sf::Vector2i(27, 30), 40);
    demo.placeWall(sf::Vector2i(30, 24), sf::Vector2i(10, 32));
    demo.placeWall(sf::Vector2i(1, 15), sf::Vector2i(15, 20));
    demo.placeWall(sf::Vector2i(0, 2), sf::Vector2i(20, 2));
    demo.placeWall(sf::Vector2i(25, 2), sf::Vector2i(35, 2));

    demo.placeWall(sf::Vector2i(63, 48), sf::Vector2i(89, 58), 20);
    demo.placeWall(sf::Vector2i(45, 10), sf::Vector2i(85, 10));
    demo.placeWall(sf::Vector2i(65, 13), sf::Vector2i(89, 18));
    demo.placeWall(sf::Vector2i(77, 32), sf::Vector2i(81, 48));
    demo.placeWall(sf::Vector2i(45, 52), sf::Vector2i(63, 56));
    demo.placeWall(sf::Vector2i(3, 58), sf::Vector2i(69, 88), 10);
    demo.placeWall(sf::Vector2i(15, 63), sf::Vector2i(39, 78));
    demo.placeWall(sf::Vector2i(85, 73), sf::Vector2i(89, 88));
    demo.placeWall(sf::Vector2i(25, 90), sf::Vector2i(85, 90));

    // forbid walls
    demo.getAstar().forbidMinScore(DEFAULT_WALL_SCORE);
    // creating the thread
    sf::Thread thread(&thread_func);
    // creating the window
    sf::RenderWindow window(sf::VideoMode(6*DEMO_SIZE_X, 6*DEMO_SIZE_Y), "Demo A*");
    window.setFramerateLimit(30);
    // launching the thread
    demo_running = true;
    thread.launch();
    // main loop
    while(demo_running)
    {
        if(!window.isOpen())
        {
            demo_running = false;
            continue;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        demo.draw(window);
        window.display();
    }

    thread.wait();
    return 0;
}
