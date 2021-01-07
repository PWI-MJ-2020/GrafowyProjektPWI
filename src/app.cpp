#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "graph.hpp"
#include "app.hpp"
#include "utils.hpp"
#include "algorithms/algorithms.hpp"

#include <cassert>
#define assertm(exp, msg) assert(((void)msg, exp))
void *sG;
//assert(zeroV || G->vertices.size() > 0);

std::mt19937 rnd2(1234);
int los0(int mi,int mx) {return rnd2()%(mx-mi+1)+mi;}

/*void Test(Application &app,Button &thisButton,sf::Event &event) {
    std::cout<<"test\n"<<std::endl;
    return;
}*/
void ChooseVertexInit(Application &app)
{
    app.aktualnyStan = chooseVertex;
    app.chosenVertices.clear();
}
void Application::ClearSelected() {
    if (firstVertexId != -1) {
        G.vertices[firstVertexId].isBeingChosen = false;
        firstVertexId = -1;
    }

    if (secondVertexId != -1) {
        G.vertices[secondVertexId].isBeingChosen = false;
        secondVertexId = -1;
    }
}
void ButtonRemoveVertex(Application &app,Button &thisButton,sf::Event &event) {app.aktualnyStan = removeV;}
void ButtonAddVertex(Application    &app,Button &thisButton,sf::Event &event) {app.aktualnyStan = addV;}
void ButtonAddEdge(Application      &app,Button &thisButton,sf::Event &event) {app.aktualnyStan = addE;}
void ButtonEditEdge(Application     &app,Button &thisButton,sf::Event &event) {app.aktualnyStan = editE;}
void ButtonRemoveEdge(Application   &app,Button &thisButton,sf::Event &event) {app.aktualnyStan = removeE;}
void ButtonMoveVertex(Application   &app,Button &thisButton,sf::Event &event) {app.aktualnyStan = movingV;}
void ButtonSimulate(Application     &app,Button &thisButton,sf::Event &event) {app.simulateForces = !app.simulateForces;}
void ButtonAlgorithm(Application    &app,Button &thisButton,sf::Event &event) {
    app.simulateForces = false;
    app.runningForward = false;
    app.aktualnyStan = algorithmC; 
   }
void PlayAlgorithm(Application    &app,Button &thisButton,sf::Event &event) {
    app.runningForward = true;
    app.runningBack = false;
    app.lastStep = clock();
    app.lastStep /= CLOCKS_PER_SEC; 
   }
void PlayBackAlgorithm(Application    &app,Button &thisButton,sf::Event &event) {
    app.runningBack = true;
    app.runningForward = false;
    app.lastStep = clock();
    app.lastStep /= CLOCKS_PER_SEC; 
   }
void StopPlayingAlgorithm(Application    &app,Button &thisButton,sf::Event &event) {
    app.runningForward = false;
    app.runningBack = false; 
   }
void ButtonReadFile(Application     &app,Button &thisButton,sf::Event &event) {
    app.aktualnyStan = readFile;
    app.textEntered.clear(); 
   
    }
void ButtonSaveFile(Application     &app,Button &thisButton,sf::Event &event) {
    app.aktualnyStan = saveFile;
    app.textEntered.clear(); 
    }
void ButtonReturnToGraphEdit(Application       &app,Button &thisButton,sf::Event &event) {
    app.aktualnyStan = nothing;
    app.holdingVertexId = -1;
    app.firstVertexId = -1;    
    app.secondVertexId = -1; 
   }
void ButtonReturnToAlgChoose(Application    &app,Button &thisButton,sf::Event &event) {
    app.runningForward = false;
    app.runningBack = false;
    app.aktualnyStan = algorithmC;
    for (Vertex &v: app.G.vertices)
        v.color = sf::Color::Red;
    for (Edge &e: app.G.allEdges)
        e.color = sf::Color::Black;
    } 
void ButtonStepRight(Application    &app,Button &thisButton,sf::Event &event) {
    app.stepLista.GoRight();}
void ButtonStepLeft(Application     &app,Button &thisButton,sf::Event &event) {
    app.stepLista.GoLeft();}
void ButtonRunDFS(Application       &app,Button &thisButton,sf::Event &event) {
    app.stepLista.ClearStates();
    ChooseVertexInit(app);
    app.algorithmId = 0;
    app.stepLista.GoRight();
}
void ButtonRunBFS(Application       &app,Button &thisButton,sf::Event &event) {
    app.stepLista.ClearStates();
    ChooseVertexInit(app);
    app.algorithmId = 1;
    app.stepLista.GoRight();
}
void ButtonRunDIJKSTRA(Application       &app,Button &thisButton,sf::Event &event) {
    app.stepLista.ClearStates();
    ChooseVertexInit(app);
    app.algorithmId = 2;
    app.stepLista.GoRight();
}
void ButtonRunSCC(Application       &app,Button &thisButton,sf::Event &event) {
    app.stepLista.ClearStates();
    ChooseVertexInit(app);
    app.algorithmId = 3;
    app.stepLista.GoRight();
}

void ButtonRunPostorder(Application       &app,Button &thisButton,sf::Event &event) {
    app.stepLista.ClearStates();
    ChooseVertexInit(app);
    app.algorithmId = 4;
    app.stepLista.GoRight();
}

void ButtonRunColors(Application    &app,Button &thisButton,sf::Event &event) {
    app.stepLista.ClearStates();
    ChooseVertexInit(app);
    app.algorithmId = 5;
    app.stepLista.GoRight();
}
void ButtonNothing(Application &app,Button &thisButton,sf::Event &event){}
//void SetTextToMousePosition(Application &app,Button &thisButton,sf::Event &event) {   
//    thisButton.text.setString(std::to_string(event.mouseButton.x) + "x "+ std::to_string(event.mouseButton.y)+ "y");}
void ButtonIncreaseAlgSpeed(Application    &app,Button &thisButton,sf::Event &event) {
    app.timeStep *= 2; 
}
void ButtonDecreaseAlgSpeed(Application    &app,Button &thisButton,sf::Event &event) {
    app.timeStep /= 2; 
}
Application::Application()
{
    if (!font.loadFromFile("Fonts/OpenSans-Regular.ttf"))
		throw("NIE MA CZCIONKI\n");
   
    
        
    G = Graph(&font);
    stepLista = StepList(&G);
    sG = &G;
    aktualnyStan    = addV;
    firstVertexId   = -1;
    secondVertexId  = -1;    
    holdingVertexId = -1;
    simulateForces = false;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    
    runningForward = false;
    runningBack = false;
    timeStep = (1.f/64.f); //czas
    lastStep = 0;
    
    selectedEdgeId = -1;
    
    sf::Text textAddVertex( L"Dodaj wierzchołek", font , 20);
    sf::Text textRemoveVertex( L"Usuń wierzchołek", font , 20);
    sf::Text textAddEdge( L"Dodaj krawedź", font , 20);
    sf::Text textEdgeEdit( L"Edycja krawędzi", font , 20);
    sf::Text textDeleteEdge( L"Usuń krawędź", font , 20);
    sf::Text textForceSimulation( L"Symulacja sił", font , 20);
    sf::Text textMoveVertex( L"Przesuń\nwierzchołek", font , 20);
    sf::Text textReturn( L"Powrót", font , 20);

    buttons.push_back(Button(50,24,195,45,textAddVertex,         &font,ButtonAddVertex));
    buttons.push_back(Button(250,24,195,45,textRemoveVertex,     &font,ButtonRemoveVertex));
    buttons.push_back(Button(450,24,145,45,textAddEdge,          &font,ButtonAddEdge));
    buttons.push_back(Button(900,24,145,45,textEdgeEdit,         &font,ButtonEditEdge));
    buttons.push_back(Button(800,24,145,45,textDeleteEdge,       &font,ButtonRemoveEdge));
    buttons.push_back(Button(900,24,145,45,textForceSimulation,  &font,ButtonSimulate));
    buttons.push_back(Button(1050,24,145,45,"Odczyt z pliku",    &font,ButtonReadFile));
    buttons.push_back(Button(1200,24,145,45,"Zapis do pliku",    &font,ButtonSaveFile));
    //buttons.push_back(Button(1350,24,150,45,"Koordy",            &font,SetTextToMousePosition));
    buttons.push_back(Button(1350,24,150,45,textMoveVertex,      &font,ButtonMoveVertex));
    buttons.push_back(Button(1350,24,150,45,"Wybierz\nAlgorytm", &font,ButtonAlgorithm));

    algorithms.push_back(DFS);   
    buttonsAlg.push_back(Button(50,24,50,45,"DFS", &font,ButtonRunDFS));

    algorithms.push_back(BFS);   
    buttonsAlg.push_back(Button(50,24,50,45,"BFS", &font,ButtonRunBFS));
    
    algorithms.push_back(DIJKSTRA);   
    buttonsAlg.push_back(Button(50,24,50,45,"Dijkstra", &font,ButtonRunDIJKSTRA));

    algorithms.push_back(SCC);   
    buttonsAlg.push_back(Button(50,24,50,45,"SCC", &font,ButtonRunSCC));

    algorithms.push_back(POSTORDER);   
    buttonsAlg.push_back(Button(50,24,50,45,"POSTORDER", &font,ButtonRunPostorder));

    algorithms.push_back(ColorsAlgorithm);
    buttonsAlg.push_back(Button(190,24,80,45,"Kolory",&font,ButtonRunColors));

    buttonsAlg.push_back(Button(190,24,150,45, textReturn, &font,ButtonReturnToGraphEdit));

    
    buttonsAlgR.push_back(Button(30,24,50,45,"<",                &font,PlayBackAlgorithm));
    buttonsAlgR.push_back(Button(100,24,50,45,"||",              &font,StopPlayingAlgorithm));
    buttonsAlgR.push_back(Button(170,24,50,45,">",               &font,PlayAlgorithm));
    buttonsAlgR.push_back(Button(240,24,50,45,"->",              &font,ButtonStepRight));
    buttonsAlgR.push_back(Button(310,24,50,45,"<-",              &font,ButtonStepLeft));
    buttonsAlgR.push_back(Button(380,24,150,45, textReturn,      &font,ButtonReturnToAlgChoose));
    buttonsAlgR.push_back(Button(550,24,50,45,"-",               &font,ButtonDecreaseAlgSpeed));
    //textBoxAlgR.push_back(TextBox(620,24,50,45,"x1",             &font));
    buttonsAlgR.push_back(Button(690,24,50,45,"+",               &font,ButtonIncreaseAlgSpeed));
    buttonsAlgR.push_back(Button(690,24,60,45,"",                &font,ButtonNothing));
    buttonsAlgR[8].ifThisIsTextBox = true;

    buttonsChooseVertex.push_back(Button(190,24,150,45, textReturn, &font,ButtonReturnToAlgChoose));

    for (int i = 1; i< buttons.size();++i) {
        buttons[i].x = buttons[i-1].x + buttons[i-1].width + BUTTON_SPACING;
        buttons[i].Relocate();
    }
    for (int i = 1; i< buttonsAlg.size();++i) {
        buttonsAlg[i].x = buttonsAlg[i-1].x + buttonsAlg[i-1].width + BUTTON_SPACING;
        buttonsAlg[i].Relocate();
    }

    window.create(sf::VideoMode(1920, 1080), "Projekt PWI",sf::Style::Default,settings);
    window.setFramerateLimit(100);
}

void Application::CheckPodswietlenie(sf::Vector2i mousePosition) {
    for (Button &button : buttons) {
        if( button.ifThisIsTextBox )
            continue;
        if (button.rectangle.getGlobalBounds().contains(mousePosition.x,mousePosition.y) ) {//czy myszka jest w prostokacie przycisku
            button.SetColor(sf::Color::Blue);
        }
        else {
            button.SetColor(sf::Color::Black);
        }
    }
    for (Button &button : buttonsAlg) {
        if( button.ifThisIsTextBox )
            continue;
        if (button.rectangle.getGlobalBounds().contains(mousePosition.x,mousePosition.y) ) {//czy myszka jest w prostokacie przycisku
            button.SetColor(sf::Color::Blue);
        }
        else {
            button.SetColor(sf::Color::Black);
        }
    }
    for (Button &button : buttonsAlgR) {
        if( button.ifThisIsTextBox )
            continue;
        if (button.rectangle.getGlobalBounds().contains(mousePosition.x,mousePosition.y) ) {//czy myszka jest w prostokacie przycisku
            button.SetColor(sf::Color::Blue);
        }
        else {
            button.SetColor(sf::Color::Black);
        }
    }
    for (Button &button : buttonsChooseVertex) {
        if( button.ifThisIsTextBox )
            continue;
        if (button.rectangle.getGlobalBounds().contains(mousePosition.x,mousePosition.y) ) {//czy myszka jest w prostokacie przycisku
            button.SetColor(sf::Color::Blue);
        }
        else {
            button.SetColor(sf::Color::Black);
        }
    }

}

void Application::Run() {    
    for(Vertex v2: G.vertices) {
        std::cerr<<"V: "<<v2.id<<std::endl;
    }    
    std::cerr<<"edges:"<<std::endl;
    for(Edge e2:G.allEdges) {
        std::cerr<<"E: "<<e2.id<<" "<<e2.idVertexFrom<<", "<<e2.idVertexTo<<std::endl;}

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {HandleEvent(event);}

        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        //stepLista.currentStep                
        //std::cerr<<"steplista: "<<stepLista.G->vertices.size()<<" <-> "<< stepLista.G <<"\n";
        //std::cerr<<(stepLista.G == sG);
        //assert((zeroV) || (    (stepLista.G->vertices[0].id >= 0 && stepLista.G->vertices[0].id <= 2) && stepLista.currentStep >= -1   ));
        
        if(simulateForces)
        {
            G.CalculateForces(window.getSize().x,window.getSize().y-TOOLBAR_HEIGHT);//to wtedy i tlyko wtedy gdy aktualny stan na symulacje sily
            G.ApplyForces(window.getSize().x,window.getSize().y-TOOLBAR_HEIGHT);
        } else {
            for (Vertex &v: G.vertices) {
                v.KeepInGraphArea(window.getSize().x,window.getSize().y-TOOLBAR_HEIGHT);
            }
        }
        if( runningForward && aktualnyStan == algorithmR ){
            long double tt= clock();
            tt /= CLOCKS_PER_SEC;
            if( tt >= lastStep + timeStep ){
                lastStep = tt;
                stepLista.GoRight();
            }
        }
        if( runningBack && aktualnyStan == algorithmR ){
            long double tt= clock();
            tt /= CLOCKS_PER_SEC;
            if( tt >= lastStep + timeStep ){
                lastStep = tt;
                stepLista.GoLeft();
            }
        }
        CheckPodswietlenie(mousePosition);
        Render();
    }
}

void Application::Render() {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    buttons[0].text.setString(std::to_string(mousePosition.x) + " " + std::to_string(mousePosition.y));
    //buttons[1].text.setString(std::to_string(window.getSize().x));
    //buttons[2].text.setString(std::to_string(aktualnyStan));
    //buttons[3].text.setString(std::to_string(GraphArea.getMaximumAntialiasingLevel()));
    window.clear(sf::Color::White);        
    RenderGraphArea();
    RenderToolBar();
    window.display();
}

void Application::RenderGraphArea(){
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    sf::RenderTexture GraphArea;
    GraphArea.create(window.getSize().x,window.getSize().y-TOOLBAR_HEIGHT,settings);
    GraphArea.clear(sf::Color::Blue);

    G.Draw(GraphArea,aktualnyStan != algorithmR);
     
    GraphArea.display();
    sf::Sprite GraphAreaSprite;
    GraphAreaSprite.setTexture(GraphArea.getTexture());
    GraphAreaSprite.setPosition(0,TOOLBAR_HEIGHT);
    window.draw(GraphAreaSprite);    
}

int TotalLength( std::vector<Button>* buttonsv )
{
    int totalLength;
    for (int i=0; i<buttonsv->size(); ++i) {
        totalLength += (*buttonsv)[i].width + BUTTON_SPACING;
    }
    if( buttonsv->size() > 0 )
        totalLength += BUTTON_SPACING;
    return totalLength;
}

void SetPositionsForButtons( std::vector<Button>* buttonsv, Application* app )
{
    long double scale, xdb, ydb, btspdb, hdb;
    int l;
    if( TotalLength(buttonsv) <= app->window.getSize().x ){
        (*buttonsv)[0].x = BUTTON_SPACING;
        (*buttonsv)[0].scale = 1;
        (*buttonsv)[0].y = 25;
        for (int i = 1; i< buttonsv->size();++i) {
            (*buttonsv)[i].x = (*buttonsv)[i-1].x + (*buttonsv)[i-1].width + BUTTON_SPACING;
            (*buttonsv)[i].y = 25;
            (*buttonsv)[i].scale = 1;
        }
    }
    else{
        int i = 0;
        l = BUTTON_SPACING;
        for( ; l <= app->window.getSize().x && i < buttonsv->size(); ){
            l += (*buttonsv)[i].width + BUTTON_SPACING;
            i++;
        }
        if( l > app->window.getSize().x ){
            i--;
            l -= (*buttonsv)[i].width + BUTTON_SPACING;
        }
        l = BUTTON_SPACING;
        for( ; l <= app->window.getSize().x && i < buttonsv->size(); ){
            l += (*buttonsv)[i].width + BUTTON_SPACING;
            i++;
        }
        if( l <= app->window.getSize().x ){
            i = 0, l = BUTTON_SPACING;
            for( ; l <= app->window.getSize().x && i < buttonsv->size(); ){
                (*buttonsv)[i].x = l;
                (*buttonsv)[i].y = 2;
                (*buttonsv)[i].scale = 1;
                l += (*buttonsv)[i].width + BUTTON_SPACING;
                i++;
            }
            if( l > app->window.getSize().x ){
                i--;
            }
            l = BUTTON_SPACING;
            for( ; l <= app->window.getSize().x && i < buttonsv->size(); ){
                (*buttonsv)[i].x = l;
                (*buttonsv)[i].y = 52;
                (*buttonsv)[i].scale = 1;
                l += (*buttonsv)[i].width + BUTTON_SPACING;
                i++;
            }
        }else{
            for( int k = 3; k < 100; k++ ){
                i = 0;
                scale = 2;
                scale /= k;
                hdb = BUTTON_HEIGHT;
                hdb *= scale;
                btspdb = BUTTON_SPACING;
                btspdb *= scale;
                for( int j = 0; j < k; j++ )
                {
                    l = btspdb;
                    for( ; l <= app->window.getSize().x && i < buttonsv->size(); ){
                        xdb = (*buttonsv)[i].width;
                        xdb *= scale;
                        l += xdb + btspdb;
                        i++;
                    }
                    if( l > app->window.getSize().x ){
                        i--;
                        xdb = (*buttonsv)[i].width;
                        xdb *= scale;
                        l -= xdb + btspdb;
                    }
                }
                if( i == buttonsv->size() ){
                    i = 0;
                    for( int j = 0; j < k; j++ )
                    {
                        l = btspdb;
                        ydb = 2 + ( hdb + 2.5 ) * j;
                        for( ; l <= app->window.getSize().x && i < buttonsv->size(); ){
                            (*buttonsv)[i].x = l;
                            (*buttonsv)[i].y = ydb;
                            (*buttonsv)[i].scale = scale;
                            xdb = (*buttonsv)[i].width;
                            xdb *= scale;
                            l += xdb + btspdb;
                            i++;
                        }
                        if( l > app->window.getSize().x ){
                            i--;
                            xdb = (*buttonsv)[i].width;
                            xdb *= scale;
                            l -= xdb + btspdb;
                        }
                    }
                    break;
                }
            }
        }
    }
}

void Application::RenderToolBar() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    sf::RenderTexture toolBar;
    toolBar.create(window.getSize().x,TOOLBAR_HEIGHT,settings);
    toolBar.clear(sf::Color::Green);

    sf::RectangleShape shape(sf::Vector2f(toolBar.getSize().x,TOOLBAR_HEIGHT));
    shape.setFillColor(sf::Color::Green);
    toolBar.draw(shape);
    std::stringstream stringStream;
    std::string stringSpeed;
    switch( aktualnyStan )
    {
        case algorithmC:
            SetPositionsForButtons( &buttonsAlg, this );
            for (int i=0; i<buttonsAlg.size(); ++i) {
                buttonsAlg[i].draw(toolBar);
            }
            break;
        case algorithmR:
            stringStream << std::fixed << std::setprecision(3) << std::round(timeStep * 64000)/1000;
            stringSpeed = stringStream.str();
            buttonsAlgR[8].text.setString(stringSpeed);
            SetPositionsForButtons( &buttonsAlgR, this );
            for (int i=0; i<buttonsAlgR.size(); ++i) {
                buttonsAlgR[i].draw(toolBar);
            }
            /*
            for (TextBox &tb :textBoxAlgR) {
                tb.Draw(toolBar);
            }*/
            break;
        case chooseVertex:
            SetPositionsForButtons( &buttonsChooseVertex, this );
            for (int i=0; i<buttonsChooseVertex.size(); ++i) {
                buttonsChooseVertex[i].draw(toolBar);
            }
            break;
        default:
            SetPositionsForButtons( &buttons, this );
            for (int i=0; i<buttons.size(); ++i) {
                buttons[i].draw(toolBar);
            }        
            break;
    }
    toolBar.display();
    sf::Sprite toolBarSprite;
    toolBarSprite.setTexture(toolBar.getTexture());
    toolBarSprite.setPosition(0,0);
    window.draw(toolBarSprite);    
}