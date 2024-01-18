#include "markings/marking.h"

class Crossing : public Marking {
public: 
    // Using the constructor of Marking
    Crossing(const Point& center, const Point& directionVector, int width, int height)
        : Marking(center, directionVector, width, height) {
            
            borders = { poly.segments[0], poly.segments[2] } ;
        }

    //get type
    std::string getType() const override { return "Crossing"; }

    void draw(sf::RenderWindow& window) const override {
        
        Point perp = Utils::perpendicular(directionVector);
        Segment line = Segment(
            std::make_shared<Point>(Utils::add(center, Utils::scale(perp, width/2))),
            std::make_shared<Point>(Utils::add(center, Utils::scale(perp, -width/2)))
        );
        line.draw(window, height, sf::Color::White, true);

        
        line.draw(window, height, sf::Color::White, true, 11, 11);
        
    }

private:
    
    std::vector<Segment> borders;
    
};


