#include "graph.hpp"
Graph::Graph() {
	graphics.font.loadFromFile("Fonts/ABeeZee-Regular.ttf"); 
}

void Graph::AddEdge(int v,int w) {
    Edge edge;
    edge.v1 = vertices.begin() + v - 1;
    edge.v2 = vertices.begin() + w - 1;
    edges.push_back(edge);
    vertices[v-1].edgesOf.push_back(edges.end()-1);
    vertices[w-1].edgesOf.push_back(edges.end()-1);
}
#include <random>
std::mt19937 rnd;
int los(int mi,int mx) {return rnd() % (mx-mi+1)+mi;}
void Graph::AddVertex(int v) {
    Vertex vertex;
    vertex.value1 = v;
    vertex.position = sf::Vector2f(los(0,1500),los(0,700));
    vertices.push_back(vertex);
}
void Graph::Draw(sf::RenderWindow& window) {
    sf::CircleShape shape(20.f,100);
    shape.setFillColor(sf::Color::Green);
    for (Vertex vertex:vertices) {
        shape.setPosition(vertex.position);
        window.draw(shape);
    }
}