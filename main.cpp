#define MAP_WIDTH			20
#define MAP_HEIGHT			20

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <list>


int mainWindowHeight = 800;
int mainWindowWidth = 800;
int currentMap[MAP_WIDTH][MAP_HEIGHT];
int startPosition[2] = { 0, 0};
int endPosition[2] = { 19, 19};
int startCell = (startPosition[0]*MAP_HEIGHT)+startPosition[1];
int endCell = (endPosition[0]*MAP_HEIGHT)+endPosition[1];


class Cells
{
public:
    int positionX, positionY, parentCellId, cummulativeCost, cellId;
    double costToGoal, totalCostGuess;
    bool visited = false;
    bool obstacle = false;
    std::list<int> neighbours;
};


double dist(double x1, double y1, double x2, double y2)
{
    double x = x1 - x2; //calculating number to square in next step
    double y = y1 - y2;
    double dist;
    dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
    dist = sqrt(dist);
    return dist;
}

Cells cellsList[MAP_HEIGHT*MAP_WIDTH];

int main()
{
    bool focus = true;
    bool mousePressedLeft = false;
    bool stateHasChanged = true;

    sf::Event event;
    sf::RenderWindow window(sf::VideoMode(mainWindowHeight, mainWindowWidth), "A Star");
    window.setFramerateLimit(60);

    sf::RectangleShape cellShape;
    cellShape.setSize(sf::Vector2f( mainWindowWidth/MAP_WIDTH, mainWindowHeight/MAP_HEIGHT));
    cellShape.setFillColor(sf::Color(255,255,255));
    cellShape.setOutlineThickness(-1);
    cellShape.setOutlineColor(sf::Color(0,0,0));

    for(int i = 0; i < MAP_WIDTH; i++)
    {
        for(int j = 0; j < MAP_HEIGHT; j++)
        {
            currentMap[i][j] = 0;
        }
    }

    while(window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::LostFocus:
                focus = false;
                break;
            case sf::Event::GainedFocus:
                focus = true;
                break;
            }
        }

        //Mouse interaction
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mousePressedLeft && focus)
        {
            stateHasChanged = true;
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            if( currentMap[mousePosition.x/(mainWindowWidth/MAP_WIDTH)][mousePosition.y/(mainWindowHeight/MAP_HEIGHT)] == 0)
            {
                currentMap[mousePosition.x/(mainWindowWidth/MAP_WIDTH)][mousePosition.y/(mainWindowHeight/MAP_HEIGHT)] = 1;
            }
            else
            {
                currentMap[mousePosition.x/(mainWindowWidth/MAP_WIDTH)][mousePosition.y/(mainWindowHeight/MAP_HEIGHT)] = 0;
            }
            mousePressedLeft = true;
        }
        else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            mousePressedLeft = false;
        }

        if(stateHasChanged)
        {
            std::list<Cells*> listToCheck;
            int n = 0;
            for(int i = 0; i < MAP_WIDTH; i++)
            {
                for(int j = 0; j < MAP_HEIGHT; j++)
                {
                    cellsList[n].positionX = i;
                    cellsList[n].positionY = j;
                    if(currentMap[i][j] == 1)
                    {
                        cellsList[n].obstacle = true;
                    }
                    else
                    {
                        cellsList[n].obstacle = false;
                    }
                    cellsList[n].costToGoal = NULL;
                    cellsList[n].visited = false;
                    cellsList[n].parentCellId = NULL;
                    cellsList[n].cummulativeCost = NULL;
                    cellsList[n].totalCostGuess = NULL;
                    cellsList[n].neighbours.clear();
                    cellsList[n].cellId = n;
                    n++;
                }
            }

            for(int i = 0; i < (MAP_HEIGHT*MAP_WIDTH) ; i++)
            {
                if(cellsList[i].positionX > 0)
                {
                    //de cell links kan toegevoegd worden
                    if(!cellsList[i-MAP_HEIGHT].obstacle)
                    {
                        cellsList[i].neighbours.push_back(i-MAP_HEIGHT);
                    }
                }

                if(cellsList[i].positionX < MAP_WIDTH-1)
                {
                    //de cell rechts kan toegevoegd worden
                    if(!cellsList[i+MAP_HEIGHT].obstacle)
                    {
                        cellsList[i].neighbours.push_back(i+MAP_HEIGHT);
                    }
                }

                if(cellsList[i].positionY > 0)
                {
                    //de cell erboven kan toegevogd worden
                    if(!cellsList[i-1].obstacle)
                    {
                        cellsList[i].neighbours.push_back(i-1);
                    }
                }

                if(cellsList[i].positionY != MAP_HEIGHT-1)
                {
                    //de cell eronder kan toegevoegd worden
                    if(!cellsList[i+1].obstacle)
                    {
                        cellsList[i].neighbours.push_back(i+1);
                    }
                }

                //schuin gaan...
                if(cellsList[i].positionY != MAP_HEIGHT-1 && cellsList[i].positionX < MAP_WIDTH-1)
                {
                    //de cell rechtsonder kan toegevoegd worden
                    if(!cellsList[i+1+MAP_HEIGHT].obstacle )
                    {
                        if(cellsList[i+1].obstacle  && cellsList[i+MAP_HEIGHT].obstacle)
                        {
                            //Dit hokje wordt door de twee buur hokjes geblokkeerd!
                        }
                        else
                        {
                            cellsList[i].neighbours.push_back(i+1+MAP_HEIGHT);
                        }
                    }
                }
                if(cellsList[i].positionY >0 && cellsList[i].positionX < MAP_WIDTH-1)
                {
                    //de cell rechtsboven kan toegevoegd worden
                    if(!cellsList[i-1+MAP_HEIGHT].obstacle)
                    {
                        if(cellsList[i-1].obstacle && cellsList[i+MAP_HEIGHT].obstacle)
                        {
                            //Dit hokje wordt door de twee buur hokjes geblokkeerd!
                        }
                        else
                        {
                            cellsList[i].neighbours.push_back(i-1+MAP_HEIGHT);
                        }
                    }
                }
                if(cellsList[i].positionY != MAP_HEIGHT-1 && cellsList[i].positionX > 0)
                {
                    //de cell linksonder kan toegevoegd worden
                    if(!cellsList[i+1-MAP_HEIGHT].obstacle)
                    {
                        if(cellsList[i+1].obstacle && cellsList[i-MAP_HEIGHT].obstacle)
                        {
                            //Dit hokje wordt door de twee buur hokjes geblokkeerd!
                        }
                        else
                        {
                            cellsList[i].neighbours.push_back(i+1-MAP_HEIGHT);
                        }
                    }
                }
                if(cellsList[i].positionY >0 && cellsList[i].positionX > 0)
                {
                    //de cell rechtsboven kan toegevoegd worden
                    if(!cellsList[i-1-MAP_HEIGHT].obstacle)
                    {
                        if(cellsList[i-MAP_HEIGHT].obstacle && cellsList[i-1].obstacle)
                        {
                            //Dit hokje wordt door de twee buur hokjes geblokkeerd!
                        }
                        else
                        {
                            cellsList[i].neighbours.push_back(i-1-MAP_HEIGHT);
                        }
                    }
                }




            }

            listToCheck.push_back(&cellsList[startCell]);
            cellsList[startCell].visited = true;

            bool pathFound = false;
            while(!listToCheck.empty())
            {
                //sorteer de lijst en zet de cell met de laagste cost to goal bovenaan om het eerst te testen
                listToCheck.sort([](const Cells* f, const Cells* s)
                {
                    return f->totalCostGuess < s->totalCostGuess;
                });
                //Check of de te checken cell het doel is. Als dat zo is zijn we klaar
                if((*listToCheck.front()).cellId == endCell)
                {
                    listToCheck.clear();
                    pathFound = true;
                }
                else
                {
                    for (std::list<int>::const_iterator iterator =  (*listToCheck.front()).neighbours.begin(), end =  (*listToCheck.front()).neighbours.end(); iterator != end; ++iterator)
                    {
                        //We have found neighbours!
                        //check if neighbours was found before
                        if(!cellsList[*iterator].visited)
                        {
                            //Deze cell heeft geen parent is is dus nooit eerder gevonden!
                            //De cell waarvan we de neighbours onderzoeken is dus automagisch tot nu toe de kortste route hiernaartoe
                            cellsList[*iterator].parentCellId = (*listToCheck.front()).cellId;
                            //Nu moeten de kosten voor de route hiernatoe uitgerekend worden (Dit zijn de kosten van naar de buurman gaan +1
                            cellsList[*iterator].cummulativeCost = (*listToCheck.front()).cummulativeCost+1;
                            //Als laatste zetten we de cell in de lijst met cellen die gecheckt moet worden
                            listToCheck.push_back(&cellsList[*iterator]);
                            //Bereken de afstand naar het doel
                            cellsList[*iterator].costToGoal = dist(cellsList[*iterator].positionX,cellsList[*iterator].positionY,cellsList[endCell].positionX,cellsList[endCell].positionY);
                            cellsList[*iterator].totalCostGuess = cellsList[*iterator].costToGoal + cellsList[*iterator].cummulativeCost;
                            cellsList[*iterator].visited = true;
                        }
                        else
                        {
                            //Deze cell is al eerder gevonden, staat dus als in de te checken cell lijst
                            if((*listToCheck.front()).cummulativeCost+1 < cellsList[*iterator].cummulativeCost)
                            {
                                //Er is een kortere route naar deze cell! Pas de parent cell dus aan en geef een nieuwe cummulative Cost;
                                cellsList[*iterator].parentCellId = (*listToCheck.front()).cellId;
                                cellsList[*iterator].cummulativeCost = (*listToCheck.front()).cummulativeCost+1;
                                cellsList[*iterator].totalCostGuess = cellsList[*iterator].costToGoal + cellsList[*iterator].cummulativeCost;
                            }
                        }
                    }
                    listToCheck.pop_front();
                }

            }
            stateHasChanged = false;
        }

        //Draw stuff:
        window.clear();
        for(int i = 0; i < MAP_WIDTH; i++)
        {
            for(int j = 0; j < MAP_HEIGHT; j++)
            {
                if(currentMap[i][j] == 1)
                {
                    cellShape.setFillColor(sf::Color(40,40,40));
                    cellShape.setPosition((mainWindowWidth/MAP_WIDTH)*i, (mainWindowHeight/MAP_HEIGHT)*j);
                    window.draw(cellShape);
                    cellShape.setFillColor(sf::Color(255,255,255));
                }
                else
                {
                    cellShape.setPosition((mainWindowWidth/MAP_WIDTH)*i, (mainWindowHeight/MAP_HEIGHT)*j);
                    window.draw(cellShape);
                }
            }
        }





        //Draw path
        //Create a list with the end as starting point
        std::list<Cells> route;
        route.push_back(cellsList[endCell]);
        bool endReached = false;
        while(!endReached)
        {
            route.push_front(cellsList[route.front().parentCellId]);
            cellShape.setFillColor(sf::Color(0,0,255));
            cellShape.setPosition((mainWindowWidth/MAP_WIDTH)* route.front().positionX, (mainWindowHeight/MAP_HEIGHT)*route.front().positionY);
            window.draw(cellShape);
            route.pop_back();
            if(route.front().parentCellId == startCell)
            {
                endReached = true;
            }
        }


        cellShape.setFillColor(sf::Color(0,255,0));
        cellShape.setPosition((mainWindowWidth/MAP_WIDTH)*startPosition[0], (mainWindowHeight/MAP_HEIGHT)*startPosition[1]);
        window.draw(cellShape);
        cellShape.setFillColor(sf::Color(255,0,0));
        cellShape.setPosition((mainWindowWidth/MAP_WIDTH)*endPosition[0], (mainWindowHeight/MAP_HEIGHT)*endPosition[1]);
        window.draw(cellShape);
        cellShape.setFillColor(sf::Color(255,255,255));
        window.display();

    }
    return 0;
}
