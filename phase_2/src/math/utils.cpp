#include <cmath>
#include "math/utils.h"

// Calculate the Euclidean distance between two points
float Utils::distance(const Point& p1, const Point& p2) {
    return std::hypot(p1.x - p2.x, p1.y - p2.y);
}

// Find the nearest point to a given location within a threshold distance
std::shared_ptr<Point> Utils::getNearestPoint(const Point& loc, const std::vector<std::shared_ptr<Point>>& points, float threshold) {
    float minDist = std::numeric_limits<float>::max();
    std::shared_ptr<Point> nearest = nullptr;

    for (const auto& point : points) {
        float dist = distance(*point, loc);
        if (dist < minDist && dist < threshold) {
            minDist = dist;
            nearest = point;
        }
    }
    return nearest;
}

Point Utils::subtract(const Point& p1, const Point& p2) {
    return Point(p1.x - p2.x, p1.y - p2.y);
}

Point Utils::add(const Point& p1, const Point& p2) {
    return Point(p1.x + p2.x, p1.y + p2.y);
}

Point Utils::scale(const Point& p, float scaler) {
    return Point(p.x * scaler, p.y * scaler);
}

Point Utils::average(const Point& p1, const Point& p2) {
    return Point((p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0);
}

Point Utils::translate(const Point& loc, float angle, float offset) {
    return Point(
        loc.x + std::cos(angle) * offset,
        loc.y + std::sin(angle) * offset
    );
}

float Utils::angle(const Point& p) {
    return std::atan2(p.y, p.x);
}

Point Utils::normalize(const Point& p) {
    return scale(p, 1.0 / magnitude(p));
}

float Utils::magnitude(const Point& p) {
    return std::hypot(p.x, p.y);
}

float Utils::dot(const Point& p1, const Point& p2) {
    return p1.x * p2.x + p1.y * p2.y;
}

std::optional<Utils::IntersectionResult> Utils::getIntersection(const Point& A, const Point& B, const Point& C, const Point& D) {
    float tTop = (D.x - C.x) * (A.y - C.y) - (D.y - C.y) * (A.x - C.x);
    float uTop = (C.y - A.y) * (A.x - B.x) - (C.x - A.x) * (A.y - B.y);
    float bottom = (D.y - C.y) * (B.x - A.x) - (D.x - C.x) * (B.y - A.y);

    float eps = 0.0001;

    if (std::abs(bottom) > eps) {
        float t = tTop / bottom;
        float u = uTop / bottom;
        if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
            // Found an intersection
            return IntersectionResult{Point{lerp(A.x, B.x, t), lerp(A.y, B.y, t)}, t};
        }
    }

    return std::nullopt; 
}



float Utils::lerp(float a, float b, float t) {
    return a + (b - a) * t;
}


struct Color {
    float r, g, b;
};

sf::Color Utils::getRandomColor() {
    // Generate random RGB values
    sf::Uint8 red = static_cast<sf::Uint8>(rand() % 256);   // Random red (0-255)
    sf::Uint8 green = static_cast<sf::Uint8>(rand() % 256); // Random green (0-255)
    sf::Uint8 blue = static_cast<sf::Uint8>(rand() % 256);  // Random blue (0-255)

    // Return the random color
    return sf::Color(red, green, blue);
}

// sf::Color Utils::getRandomColor() {
//     // Generate a random hue value between 290 and 550
//     float hue = 290.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 260.0f));

//     // Convert HSL to RGB (assuming Saturation = 100%, Lightness = 60%)
//     float S = 1.0f; // Saturation
//     float L = 0.6f; // Lightness
//     float C = (1.0f - std::fabs(2.0f * L - 1.0f)) * S;
//     float X = C * (1.0f - std::fabs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
//     float m = L - C / 2.0f;
//     float r, g, b;

//     if (hue >= 0 && hue < 60) {
//         r = C, g = X, b = 0;
//     } else if (hue >= 60 && hue < 120) {
//         r = X, g = C, b = 0;
//     } else if (hue >= 120 && hue < 180) {
//         r = 0, g = C, b = X;
//     } else if (hue >= 180 && hue < 240) {
//         r = 0, g = X, b = C;
//     } else if (hue >= 240 && hue < 300) {
//         r = X, g = 0, b = C;
//     } else {
//         r = C, g = 0, b = X;
//     }

//     // Adjust the result and convert to 8-bit integers
//     r = (r + m) * 255;
//     g = (g + m) * 255;
//     b = (b + m) * 255;

//     return sf::Color(static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b));
// }