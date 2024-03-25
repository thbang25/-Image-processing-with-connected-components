/** Thabang  sambo
 *  23/03/2024
 *  stores all the information needed to represent a component.
 *  container stores smart pointers to each ConnectedComponent
 *  
 **/

#ifndef CONNECTED_COMPONENT_H
#define CONNECTED_COMPONENT_H

#include <vector>
#include <utility>
#include <algorithm>

class ConnectedComponent {
public:

    ConnectedComponent(int x, int y);//method to try and update the bounds and pixels
    ConnectedComponent(const ConnectedComponent& other); // Copy constructor
    ConnectedComponent& operator=(const ConnectedComponent& other); // Copy assignment operator
    ConnectedComponent(ConnectedComponent&& other) noexcept; // Move constructor
    ConnectedComponent& operator=(ConnectedComponent&& other) noexcept; // Move assignment operator
    ~ConnectedComponent(); // Destructor
//try to update components
    void update(int x, int y);
    int minX, minY, maxX, maxY;
//pixel variable
    std::vector<std::pair<int, int>> pixels;
};

#endif // CONNECTED_COMPONENT_H


