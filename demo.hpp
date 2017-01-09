#ifndef DEMO_HPP
#define DEMO_HPP

#include "astar.hpp"
#include <SFML/Graphics.hpp>

#define DEFAULT_WALL_SCORE 200

class Demo
{
    public:
        Demo();
        virtual ~Demo();
        void clear();
        void createNodeMap(sf::Vector2i s); // create a 2D map (sort like a tile-based map) and store it into nodes
        void placeWall(sf::Vector2i start, sf::Vector2i goal, int32_t nScore = DEFAULT_WALL_SCORE); // change the score values of some nodes to simulate wall/obstacles
        void run(sf::Vector2i start, sf::Vector2i goal);
        bool runStepByStep(sf::Vector2i start, sf::Vector2i goal);
        void draw(sf::RenderWindow& win);
        void updatePath();
        int32_t getNodeScore(sf::Vector2i p) const;
        Astar& getAstar();
    protected:
        Astar *astar;
        std::vector<aNode*> nodes;
        std::vector<aNode*> path;
        sf::Vector2i size;
        sf::RectangleShape rect;
        bool lastResult;
        sf::Vector2i lastStart;
        sf::Vector2i lastGoal;
        sf::Clock chrono;
        float avg;
        size_t count;
};

#endif // DEMO_HPP
