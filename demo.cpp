#include "demo.hpp"
#include <iostream>

Demo::Demo()
{
    astar = new Astar();
    rect.setSize(sf::Vector2f(5, 5));
    lastStart.x = -1;
    lastStart.y = -1;
    lastGoal.x = -1;
    lastGoal.y = -1;
    count = 0;
    avg = 0;
}

Demo::~Demo()
{
    delete astar;
    clear();
}

void Demo::clear()
{
    size.x = -1;
    size.y = -1;
    for(aNode* i : nodes)
        delete i;
    nodes.clear();
    path.clear();
}

void Demo::createNodeMap(sf::Vector2i s)
{
    clear();
    if(s.x < 0 || s.y < 0) return;
    std::cout << "creating nodes..." << std::endl;
    size = s;
    nodes.reserve(size.x * size.y);
    for(int32_t j = 0; j < size.y; ++j)
    {
        for(int32_t i = 0; i < size.x; ++i)
        {
            nodes.push_back(new aNode());
            nodes.back()->setPosition(sf::Vector2i(i, j));
            nodes.back()->setScore(1);
        }
    }
    std::cout << "placing neightbors..." << std::endl;
    for(int32_t j = 0; j < size.y; ++j)
    {
        for(int32_t i = 0; i < size.x; ++i)
        {
            for(size_t k = 0; k < 4; ++k)
            {
                sf::Vector2i n(i, j);
                switch(k)
                {
                    case 0: n.y--; break;
                    case 1: n.x++; break;
                    case 2: n.y++; break;
                    case 3: n.x--; break;
                }

                if(n.x >= 0 && n.x < size.x && n.y >= 0 && n.y < size.y)
                {
                    nodes[i+(j*size.x)]->addNeighbor(nodes[n.x+(n.y*size.x)]);
                }
            }
        }
    }
    std::cout << "done" << std::endl;
}

void Demo::placeWall(sf::Vector2i start, sf::Vector2i goal, int32_t nScore)
{
    if(start.x < 0 || start.x >= size.x || start.y < 0 || start.y >= size.y)
        return;
    if(goal.x < 0 || goal.x >= size.x || goal.y < 0 || goal.y >= size.y)
        return;

    for(int32_t j = start.y; j <= goal.y; ++j)
    {
        for(int32_t i = start.x; i <= goal.x; ++i)
        {
            nodes[i+(j*size.x)]->setScore(nScore);
        }
    }
}

void Demo::run(sf::Vector2i start, sf::Vector2i goal)
{
    if(start.x < 0 || start.x >= size.x || start.y < 0 || start.y >= size.y)
        return;
    if(goal.x < 0 || goal.x >= size.x || goal.y < 0 || goal.y >= size.y)
        return;
    std::cout << "searching path: " << start.x << "x" << start.y << " to " << goal.x << "x" << goal.y << std::endl;
    if(astar->find(nodes[start.x+(start.y*size.x)], nodes[goal.x+(goal.y*size.x)]))
    {
        std::cout << "done" << std::endl;
    }
    else
    {
        std::cout << "failed" << std::endl;
    }
}

bool Demo::runStepByStep(sf::Vector2i start, sf::Vector2i goal)
{
    if(start.x < 0 || start.x >= size.x || start.y < 0 || start.y >= size.y)
        return false;
    if(goal.x < 0 || goal.x >= size.x || goal.y < 0 || goal.y >= size.y)
        return false;
    static bool started = false;

    if(!started)
    {
        std::cout << "searching path: " << start.x << "x" << start.y << " to " << goal.x << "x" << goal.y << std::endl;
        if(!astar->startStepByStep(nodes[start.x+(start.y*size.x)], nodes[goal.x+(goal.y*size.x)]))
            return false;
        started = true;
        chrono.restart();
    }
    else
    {
        bool tmpR = astar->findStepByStep();
        if(tmpR || astar->getState() == 0)
        {
            if(!tmpR) astar->partialPath();
            sf::Time tmpT = chrono.getElapsedTime();
            avg = ((avg * count) + tmpT.asMilliseconds()) / (float)(count + 1);
            count++;
            std::cout << "done [elapsed=" << tmpT.asMilliseconds() << " ms | average=" << avg << " ms | sample=" << count << "]" << std::endl;
            started = false;
            lastResult = tmpR;
            lastStart = start;
            lastGoal = goal;
            return true;
        }
    }
    return false;
}

void Demo::draw(sf::RenderWindow& win)
{
    // map
    for(int32_t j = 0; j < size.y; ++j)
    {
        for(int32_t i = 0; i < size.x; ++i)
        {
            if(nodes[i+(j*size.x)]->getScore() > 1)
                rect.setFillColor(sf::Color(25+230*nodes[i+(j*size.x)]->getScore()/DEFAULT_WALL_SCORE, 25+230*nodes[i+(j*size.x)]->getScore()/DEFAULT_WALL_SCORE, 0));
            else
                rect.setFillColor(sf::Color(20, 20, 20));
            rect.setPosition(sf::Vector2f(6*i, 6*j));
            win.draw(rect);
        }
    }
    // path
    if(lastResult)
        rect.setFillColor(sf::Color::Red);
    else rect.setFillColor(sf::Color::Blue);
    for(size_t k = 0; k < path.size(); ++k)
    {
        rect.setPosition(sf::Vector2f(6*path[k]->getPosition().x, 6*path[k]->getPosition().y));
        win.draw(rect);
    }
    // start
    rect.setFillColor(sf::Color::Cyan);
    rect.setPosition(sf::Vector2f(6*lastStart.x, 6*lastStart.y));
    win.draw(rect);
    // goal
    rect.setFillColor(sf::Color::Green);
    rect.setPosition(sf::Vector2f(6*lastGoal.x, 6*lastGoal.y));
    win.draw(rect);
}

void Demo::updatePath()
{
    if(astar->getState() == 0)
    {
        astar->swapPath(path);
        std::cout << "path size: " << path.size() << std::endl;
    }

}

int32_t Demo::getNodeScore(sf::Vector2i p) const
{
    if(p.x < 0 || p.x >= size.x || p.y < 0 || p.y >= size.y) return -1;
    return nodes[p.x+(p.y*size.x)]->getScore();
}

Astar& Demo::getAstar()
{
    return *astar;
}
