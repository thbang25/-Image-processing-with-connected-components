/** Thabang  sambo
 *  23/03/2024
 *  stores all the information needed to represent a component.
 *  container stores smart pointers to each ConnectedComponent
 *  
 **/

#include "ConnectedComponent.h"

//co-ordinates for the pixels, initialize 
ConnectedComponent::ConnectedComponent(int x, int y) : minX(x), minY(y), maxX(x), maxY(y) {}

// Copy constructor
ConnectedComponent::ConnectedComponent(const ConnectedComponent& other) : 
    minX(other.minX), minY(other.minY), maxX(other.maxX), maxY(other.maxY), pixels(other.pixels) {}

// Copy assignment operator
ConnectedComponent& ConnectedComponent::operator=(const ConnectedComponent& other) {
    if (this != &other) {
        minX = other.minX;
        minY = other.minY;
        maxX = other.maxX;
        maxY = other.maxY;
        pixels = other.pixels;
    }
    return *this;
}

// Move constructor
ConnectedComponent::ConnectedComponent(ConnectedComponent&& other) noexcept :
    minX(std::move(other.minX)), minY(std::move(other.minY)), 
    maxX(std::move(other.maxX)), maxY(std::move(other.maxY)), pixels(std::move(other.pixels)) {}

// Move assignment operator
ConnectedComponent& ConnectedComponent::operator=(ConnectedComponent&& other) noexcept {
    if (this != &other) {
        minX = std::move(other.minX);
        minY = std::move(other.minY);
        maxX = std::move(other.maxX);
        maxY = std::move(other.maxY);
        pixels = std::move(other.pixels);
    }
    return *this;
}

// Destructor
ConnectedComponent::~ConnectedComponent() {}

//method to try and update the bounds and pixels
void ConnectedComponent::update(int x, int y) {
    minX = std::min(minX, x);
    minY = std::min(minY, y);
    maxX = std::max(maxX, x);
    maxY = std::max(maxY, y);
    pixels.emplace_back(x, y);
}


