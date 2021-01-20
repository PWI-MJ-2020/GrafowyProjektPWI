## Projekt GraphVisualizer

#### Wstęp
Program oferuje możliwość tworzenia, edycji, wczytywania oraz zapisu grafu. Pozwala na wizualizację krok po kroku działania szeregu algorytmów takich jak DFS, BFS, algorytm Dijkstry, wyświetlanie wierzchołków w kolejności postorder, podział grafu na silnie spójne składowe. Rozbudowany silnik fizyczny umożliwia w zautomatyzowany sposób estetycznie rozmieścić wierzchołki i krawędzie w polu edycji grafu. Zapis oraz odczyt do pliku pozwalają na kontynuację pracy nawet po przerwaniu wykonywania programu. Zmiany kolorów zarówno wierzchołków jak i krawędzi podczas wykonywania kolejnych kroków algorytmów pozwalają na przejrzystą obserwację sposobu działania algorytmu. Interaktywne poruszanie się po liście kroków algorytmu, ze szczególnym uzwględnieniem w pełni działającej możliwości cofania tych dotychczasowo wykonanych, pozwala na dogłębną analizę i pełne zrozumienie podstaw algorytmiki. Program może być świetnym dopełnieniem klasycznej formy nauczania algorytmów, w szczególności dla zawodników oraz potencjalnych zawodników Olimpiady Informatycznej, Olimpiady Informatycznej Juniorów oraz innych osób zaczynających swoją przygodę w świecie algorytmiki konkursowej. 


#### Instalacja niezbędnych zależności
Zalecane jest uruchomienie programu na systemach z rodziny Linux. Pozwala on na łatwą instalację potrzebnych pakietów:
`sudo apt-get install libsfml-dev`
`sudo apt install build-essential`

#### Uruchomienie programu
Dzięki utworzonemu plikowi `makefile` kompilacja jest prosta i intuicyjna. Polega na wejściu w lokalizację pobranego repozytorium oraz wpisaniu polecenia:
`make`
Powstanie wtedy plik wykonywalny `GraphVisualizer`, który należy wykonać poleceniem
`./GraphVisualizer`

Jednak w przypadku, gdy powyższy sposób okaże się nieskuteczny, przewidziana jest również druga,
zastępcza metoda kompilacji i uruchomienia. Po wejściu do katalogu repozytorium w terminalu należy wpisać:
`g++ -c src/*.cpp src/algorithms/*.cpp src/graphics/*.cpp`
`g++ *.o -o GraphVisualizer -lsfml-graphics -lsfml-window -lsfml-system`


#### Wykorzystane bibloteki
SFML 2.5.1
Język programowania C++ w standardzie 17

#### Autorzy
Mikołaj Jaszcza
Michał Kierul
Marcin Wróbel
Marcin Sarnecki
