#include "graph.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <math.h>
#define pb push_back
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define M_RAD 57.2957

float getAngleByCoordinates(float x, float y)//zwraca kat miedzy osia y+ zgodnie z ruchem wskazowek zegara do punku (x,y)
{
	if (x == 0) {
		if (y > 0) return 0.f;
		else return 180.f;
	}
	if (y == 0) {
		if (x > 0) return 90.f;
		else return 270.f;
	}
	if (x > 0 && y > 0) return 90.f - atan(fabs(y / x)) * M_RAD;
	if (x > 0 && y < 0) return 90.f + atan(fabs(y / x)) * M_RAD;
	if (x < 0 && y < 0) return 270.f - atan(fabs(y / x)) * M_RAD;
	if (x < 0 && y > 0) return 270.f + atan(fabs(y / x)) * M_RAD;
}

float getLenght(sf::Vector2f p1, sf::Vector2f p2) {//odleglosc miedzy dwoma punktami w przestrzeni
	return sqrtf(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void Graph::RemoveVertex(int id) {
    
    for (int w: vertices[id].edgesIdFrom) RemoveEdge(w);
    for (int w: vertices[id].edgesIdTo) RemoveEdge(w);
    
    std::swap(vertices[id], vertices.back());
    vertices.pop_back();
    if (id == 0) return;
    vertices[id].id = id;
    for (int w: vertices[id].edgesIdFrom) {
        if (allEdges[w].idVertexFrom == vertices.size()) allEdges[w].idVertexFrom = id;
        if (allEdges[w].idVertexTo   == vertices.size()) allEdges[w].idVertexTo   = id;
    }
    for (int w: vertices[id].edgesIdTo) {
        if (allEdges[w].idVertexFrom == vertices.size()) allEdges[w].idVertexFrom = id;
        if (allEdges[w].idVertexTo   == vertices.size()) allEdges[w].idVertexTo   = id;
    }
}

void Graph::AddVertex(sf::Vector2f position){
    int newId = vertices.size();
    std::string text1 = "";

    Vertex vertexToAdd = Vertex(position, newId, text1);
    vertices.push_back(vertexToAdd);
}


Vertex::Vertex(sf::Vector2f position, int id, std::string text1){
    position = position;
    text1 = text1;
    force = sf::Vector2f(0.f,0.f);
}



Edge::Edge(){
    idVertexFrom = idVertexTo = weight1 = weight2 = -1;
    isHighlighted = isDeleted = false;
}

Edge::Edge(int v, int u, int w1, int w2) {
    idVertexFrom = v;
    idVertexTo = u;
    weight1 = w1;
    weight2 = w2;
    isHighlighted = isDeleted = false;
}

void Graph::AddEdge(Edge edge) {
    vertices[edge.idVertexFrom].edgesIdTo.pb(edge.idVertexTo);
    vertices[edge.idVertexTo].edgesIdFrom.pb(edge.idVertexFrom);
    allEdges.pb(edge);
}

void Graph::RemoveEdgeFromVertex(int id,int v) {
    for (int i = 0; i < vertices[v].edgesIdFrom.size(); ++i) {
        if (vertices[v].edgesIdFrom[i] == id) {
            std::swap(vertices[v].edgesIdFrom[i],vertices[v].edgesIdFrom.back());
            return;
        }
    }
    for (int i = 0; i < vertices[v].edgesIdTo.size(); ++i) {
        if (vertices[v].edgesIdTo[i] == id) {
            std::swap(vertices[v].edgesIdTo[i],vertices[v].edgesIdTo.back());
            return;
        }
    }
}
void Graph::RemoveEdge(int id) {
    RemoveEdgeFromVertex(id,allEdges[id].idVertexFrom);
    RemoveEdgeFromVertex(id,allEdges[id].idVertexTo);    
}

void Graph::CalculateForces() {
	rep(i, 0, vertices.size()-1) vertices[i].force = sf::Vector2f(0.f,0.f);
	//przyciaganie do srodka 800 400
	rep(i, 0, vertices.size()-1) {
		float distance = getLenght(sf::Vector2f(800, 400), vertices[i].position);
		float forceValue = GravityForce(distance);
		sf::Vector2f forceVector = sf::Vector2f(800, 400) - vertices[i].position;
		float angle = getAngleByCoordinates(vertices[i].position.x - 800, vertices[i].position.y - 400);
		vertices[i].force -= sf::Vector2f(forceValue * sin(angle / M_RAD), forceValue * cos(angle / M_RAD));//- a nie + bo grawitacja przyciaga FIZYKA 
	}

	//odpychanie sie wierzcholkow
	rep(i, 0, vertices.size()-1)
		rep(j, i+1, vertices.size()-1) {
		float distance = getLenght(vertices[i].position, vertices[j].position);
		float forceValue = RepulsionForce(distance);
		sf::Vector2f forceVector_i_to_j = vertices[i].position - vertices[j].position;
		sf::Vector2f forceVector_j_to_i = vertices[j].position - vertices[i].position;
		float angle1 = getAngleByCoordinates(vertices[i].position.x - vertices[j].position.x, vertices[i].position.y - vertices[j].position.y);
		float angle2 = (angle1 + 180);
		if (angle2 > 360) angle2 -= 180;
		vertices[i].force += sf::Vector2f(forceValue * sin(angle1 / M_RAD), forceValue * cos(angle1 / M_RAD));
		vertices[i].force += sf::Vector2f(forceValue * sin(angle2 / M_RAD), forceValue * cos(angle2 / M_RAD));
	}

	//przyciaganie sie na krawedziach
	rep(i, 0, vertices.size()-1)
		rep(j, i+1, vertices.size()-1) {
		bool czyJest = 0;
		for (auto it : vertices[i].edgesIdTo) if (it == j) czyJest = 1;
        for (auto it : vertices[i].edgesIdFrom) if (it == j) czyJest = 1;
		if (czyJest == 0) continue;
		float distance = getLenght(vertices[i].position, vertices[j].position);
		float forceValue = AttractionForce(distance);
		sf::Vector2f forceVector_i_to_j = vertices[i].position - vertices[j].position;
		sf::Vector2f forceVector_j_to_i = vertices[j].position - vertices[i].position;
		float angle1 = getAngleByCoordinates(vertices[i].position.x - vertices[j].position.x, vertices[i].position.y - vertices[j].position.y);
		float angle2 = (angle1 + 180);
		if (angle2 > 360) angle2 -= 180;
		force[i] -= sf::Vector2f(forceValue * sin(angle1 / M_RAD), forceValue * cos(angle1 / M_RAD));
		force[j] -= sf::Vector2f(forceValue * sin(angle2 / M_RAD), forceValue * cos(angle2 / M_RAD));
	}
}

void Graph::ApplyForces() {
	rep(i, 0, vertices.size()-1) {
		sf::Vector2f delta = sf::Vector2f(vertices[i].x * 0.01, vertices[i].y * 0.01);
		if (delta.x < 0.05) delta.x = 0;
		if (delta.y < 0.05) delta.y = 0;

		vertices[i].position += delta;
		vertices[i].circle.setPosition(grafika[i].circle.getPosition() + delta);
		vertices[i].text.setPosition(grafika[i].text.getPosition() + delta);
	}
}

float Graph::RepulsionForce(float distance) {
	if (distance >= 100) return 0;
	return ((distance - 100) * (distance - 100) / 1 + 50.f);
}

float Graph::AttractionForce(float distance) {
	if (distance <= 100) return 0;
	return ((distance - 100) * (distance - 100) / 5 + 50.f);
}

float Graph::GravityForce() {
	return 100;
}
