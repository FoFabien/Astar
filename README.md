# A* search algorithm
Implementation in C++ of Wikipedia's pseudocode of the A* search algorithm.
https://en.wikipedia.org/wiki/A*_search_algorithm
Slight modifications was done with a 2D tile-based game map in mind (but it can be used with others types of graph or easily modified).
Also, the code uses c++11 but you can make it compatible with older versions if you need to.

# Astar (.cpp and .hpp)
The class aNode represents a node/point/tile of your graph/map.
You can assign it a 2D position and a score (higher score means the node is more difficult to pass through).

The Astar class is the search algorithm.
Gives the starting and ending nodes to the find function and it will try to find a valid path between the two.
The goal of the StepByStep functions is to allow you to do the same thing without blocking your application (for example, calling the findStepByStep function in your main loop will search a bit and then your loop will resume until next call).
The searchArea and minimalScore settings allow you to customize the search.
The first excludes nodes out of range (to reduce the CPU usage if you have a big graph. However, if there is no valid path in the area, the search will fail, obviously).
The second excludes nodes with a score equal or higher than the specified minimum (in the demo, it used to make impassable walls).

# Demo
main.cpp, demo.cpp and demo.hpp form a rough demo, for example purpose.
It uses SFML to render the result.