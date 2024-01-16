#ifndef SEGMENT_H
#define SEGMENT_H

#include "primitives/point.h"
#include <memory>
#include <functional>
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include "math/utils.h"

class Segment {
public:
    std::shared_ptr<Point> p1;
    std::shared_ptr<Point> p2;

    Segment(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2);
    // default constructor
    Segment() : p1(nullptr), p2(nullptr) {}

    bool equals(const Segment& seg) const;
    bool includes(const Point& point) const;
    void draw(sf::RenderWindow& window, float width = 2, sf::Color color = sf::Color::Black, bool dash = false) const;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(p1, p2); // Serialize the start and end points
    }

    float length() const {
        return Utils::distance(*p1, *p2);
    }

    Point directionVector() const {
        return Utils::normalize(Utils::subtract(*p2, *p1));
    }

    float distanceToPoint(const Point& point) const;

    Utils::IntersectionResult projectPoint(const Point& point) const; 
};


#endif // SEGMENT_H
