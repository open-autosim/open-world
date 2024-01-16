#include "world.h"


World::World(Graph& graph, int roadWidth, int roadRoundness, int buildingWidth, int buildingMinLength, int spacing, int treeSize) : 
    graph(graph), 
    roadWidth(roadWidth), 
    roadRoundness(roadRoundness), 
    buildingWidth(buildingWidth), 
    buildingMinLength(buildingMinLength), 
    spacing(spacing),
    treeSize(treeSize) 
{
    
}

void World::generate() {
    
    envelopes.clear();

    // check if graph is empty
    if (graph.getSegments().size() == 0) {
        roadBorders.clear();
        return;
    }

    for (auto& seg : graph.getSegments()) {
        envelopes.push_back(Envelope(seg, roadWidth, roadRoundness));
    }
        
    std::vector<Polygon> polygons;
    for (auto& envelope : envelopes) {
        polygons.push_back(envelope.polygon);
    }
    roadBorders = Polygon::unionPolygons(polygons);

    buildings = generateBuildings();

    trees = generateTrees();
}

std::vector<Point> World::generateTrees() {

    std::vector<Point> points;

    for (const auto& seg : roadBorders) {
        points.push_back(*(seg.p1));
        points.push_back(*(seg.p2));
    }

    for (const auto& building : buildings) {
        for (const auto& pointPtr : building.points) {
            points.push_back(*pointPtr);
        }
    }

    float left = std::numeric_limits<float>::max();
    float right = std::numeric_limits<float>::lowest();
    float top = std::numeric_limits<float>::max();
    float bottom = std::numeric_limits<float>::lowest();

    for (const auto& point : points) {
        left = std::min(left, point.x);
        right = std::max(right, point.x);
        top = std::min(top, point.y);
        bottom = std::max(bottom, point.y);
    }

    std::vector<Polygon> illegalPolys;

    for (const auto& building : buildings) {
        illegalPolys.push_back(building);
    }
    for (const auto& env : envelopes) {
        illegalPolys.push_back(env.polygon);
    }

    std::vector<Point> trees;
    int tryCount = 0;
    while (tryCount<100) {
        Point p = Point(
            Utils::lerp(left, right, Utils::getRand()), 
            Utils::lerp(top, bottom, Utils::getRand())
        );

        // check if point is in a building or too close to a building
        bool keep = true;
        for (const auto& poly : illegalPolys) {
            if (poly.containsPoint(p) || poly.distanceToPoint(p) < treeSize / 2) {
                keep = false;
                break;
            }
        }

        // check if point is too close to another tree
        if (keep) {
            for (const auto& tree : trees) {
                if (Utils::distance(tree, p) < treeSize) {
                    keep = false;
                    break;
                }
            }
        }

        // check if point is close to a road
        if (keep) {
            bool closeToSomething = false;
            for (const auto& poly : illegalPolys) {
                if (poly.distanceToPoint(p) < treeSize * 2) {
                    closeToSomething = true;
                    break;
                }
            }
            keep = closeToSomething;
        }

        if (keep) {
            trees.push_back(p);
            tryCount = 0;
        }
        tryCount++;
    }
    return trees;
}

std::vector<Polygon> World::generateBuildings() {
    
    std::vector<Envelope> tempEnvelopes;

    for (auto& seg : graph.getSegments()) {
        tempEnvelopes.push_back(Envelope(seg, roadWidth + buildingWidth + 2*spacing, roadRoundness));
    }

    //call union ont he polygons of the envelopes
    std::vector<Polygon> polygons;
    for (auto& env : tempEnvelopes) {
        polygons.push_back(env.polygon);
    }
    std::vector<Segment> guides = Polygon::unionPolygons(polygons);

    auto it = guides.begin();
    while (it != guides.end()) {
        if (it->length() < buildingMinLength) {
            it = guides.erase(it);
        } else {
            ++it;
        }
    }

    std::vector<Segment> supports;
    for (auto& seg : guides) {
        float len = seg.length() + spacing;
        float buildingCount = floor(len / (buildingMinLength + spacing));
        float buildingLength = len / buildingCount - spacing;

        Point dir = seg.directionVector();

        Point q1 = *seg.p1;
        Point q2 = Utils::add(q1, Utils::scale(dir, buildingLength));
        supports.push_back(Segment(std::make_shared<Point>(q1), std::make_shared<Point>(q2)));
    
        for (int i = 2; i <= buildingCount; i++) {
            q1 = Utils::add(q2, Utils::scale(dir, spacing));
            q2 = Utils::add(q1, Utils::scale(dir, buildingLength));
            supports.push_back(Segment(std::make_shared<Point>(q1), std::make_shared<Point>(q2)));
        }
    }

    std::vector<Polygon> bases;

    for (auto& seg : supports) {
        bases.push_back(Envelope(seg, buildingWidth).polygon);
    }

    float eps = 0.0001;
    for (auto it1 = bases.begin(); it1 != bases.end(); ++it1) {
        auto it2 = it1;
        ++it2; // Start from the next element
        while (it2 != bases.end()) {
            if (it1->intersectsPoly(*it2) || it1->distanceToPoly(*it2) < spacing - eps) {
                it2 = bases.erase(it2);
            } else {
                ++it2;
            }
        }
    }

    return bases;
}



void World::draw(sf::RenderWindow& window) const {
    for (auto& envelope : envelopes) {
        envelope.draw(window, sf::Color(187, 187, 187), 7, sf::Color(187, 187, 187));
    }

    for (auto& seg : graph.getSegments()) {
        seg.draw(window, 4, sf::Color::White, true);
    }

    for (auto& segment : roadBorders) {
        segment.draw(window, 4, sf::Color::White);
    }
    
    for (auto& tree : trees) {
        tree.draw(window, treeSize, sf::Color(0,0,0,50));
    }

    for (auto& building : buildings) {
        building.draw(window);
    }

}