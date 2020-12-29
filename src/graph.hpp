#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#define N 1000
typedef std::pair<int, int> pii;
typedef std::vector<int> VI;
typedef std::vector<std::pair<int, int> > VII;
typedef std::pair<int,int> pii;

class Vertex{
public:
	int id;///5
    bool isDeleted;

    std::string text1, text2;
	sf::Vector2f position;
	sf::Vector2f force;
    std::vector<int> edgesIdFrom;/////  1 2 3 4 5 6 7 8 9 10 11
    std::vector<int> edgesIdTo;

	Vertex();
	Vertex(int);
};

class Edge{
public:
    int id, idVertexFrom, idVertexTo, weight1, weight2;
    bool isHighlighted, isDeleted;

    Edge();
    Edge(int,int);
};	

class Graph {
public:
    std::vector<Vertex> vertices; //wszystkie wierzchołki
	std::vector<Edge> allEdges; //wszystkie krawędzie
    
    bool isDirected, isWeighted;

	float GravityForce(int);
	float RepulsionForce(int, int);
	float AttractionForce(int, int);//w argumentach indeksy wierzcholkow

	void CalculateForces();
	void ApplyForces();

    void AddEdge(Edge);
    void AddVertex(Vertex);
	void RemoveEdge(int);
    void RemoveVertex(int);


	void Draw(sf::RenderWindow&);
    Graph();
};

#endif