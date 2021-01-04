#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include "graph.hpp"
#include "graphics/button.hpp"
#include "steps.hpp"

enum stan {
        addV, removeV, 
        addE, removeE, 
        movingV,
        simulateForce,
        algorithmC,
        algorithmR,
        readFile,
        saveFile,
        chooseVertex
};

class Application{
public:
    sf::RenderWindow window;
    sf::Font font;
    Graph G;
    StepList stepLista;
    /*enum buttonsEnum {
        buttonAddV, buttonRemoveV,
        buttonAddE, buttonRemoveE,
        buttonSymulacjaSily,
        buttonOdczyt,
        buttonZapis,
        buttonDfs,
        buttonPokazKlik, buttonPokazPozycje,
        buttonBfs
    };*///i wszystkie pozostale algorytmy tez maja button
    std::vector<Button> buttons;
    std::vector<Button> buttonsAlg;
    std::vector<Button> buttonsAlgR;
    std::vector<Button> buttonsChooseVertex;
    std::vector<int> chosenVertices;
    stan aktualnyStan;//wszystkie stany
    int holdingVertexId;
    int firstVertexId, secondVertexId;
    int algorithmId;

    std::vector<std::function<void(Graph *,StepList*, std::vector<int> &)> > algorithms;
    void Run();
    void CheckPodswietlenie(sf::Vector2i);
    void Render();
    void RenderToolBar();
    void RenderGraphArea();
    Application();

    void HandleEvent(sf::Event &event);
    void HandleMouseButtonPressed(sf::Event &event);
    void HandleMouseButtonReleased(sf::Event &event);
    void HandleMouseMoved(sf::Event &event);
};





#endif