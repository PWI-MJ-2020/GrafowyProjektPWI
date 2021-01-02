#ifndef EDGE_HPP
#define EDGE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include "graph.hpp"

class Edge{
public:
    int id, idVertexFrom, idVertexTo;
    int weight1, weight2;
    bool isHighlighted;
    sf::Text t1, t2;
    sf::Color color;
    void SetColor(sf::Color);
    Edge();
    Edge(int,int,int,int,sf::Font*);
};	


#endif
