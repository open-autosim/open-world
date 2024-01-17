#ifndef WORLD_H
#define WORLD_H

#include "math/graph.h"
#include "primitives/envelope.h"
#include "primitives/polygon.h"
#include "items/tree.h"
#include "items/building.h"
#include <vector>
#include <algorithm> 
#include <limits>
#include <SFML/Graphics.hpp>
#include "markings/marking.h"


class World {

public:
    
    World(Graph& graph, 
        int roadWidth = 100, 
        int roadRoundness = 10,
        int buildingWidth = 150, 
        int buildingMinLength = 150,
        int spacing = 50,
        int treeSize = 160
    );

    void draw(sf::RenderWindow& window, const Point& viewPoint) const;

    void generate();
    std::vector<Building> generateBuildings();
    std::vector<Tree> generateTrees();
    std::vector<Segment> generateLaneGuides();

    // get graph
    Graph& getGraph() { return graph; }
    int getRoadWidth() const { return roadWidth; }
    std::vector<Segment> getLaneGuides() const { return laneGuides; }

    //method to add markings
    void addMarking(std::shared_ptr<Marking> marking) {
        markings.push_back(marking);
    }

    

private:
    /* data */
    Graph& graph;
    int roadWidth;
    int roadRoundness;
    int buildingWidth;
    int buildingMinLength;
    int spacing;
    int treeSize;

    std::vector<Envelope> envelopes;
    std::vector<Segment> roadBorders;
    std::vector<Building> buildings;
    std::vector<Tree> trees;
    std::vector<Segment> laneGuides;

    std::vector<std::shared_ptr<Marking>> markings;



};

#endif // WORLD_H
