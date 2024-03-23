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

//connected image
class ConnectedComponent {
public:
//constructor for the pixels plane
    int minX, minY, maxX, maxY;
//pixel variable
    std::vector<std::pair<int, int>> pixels;
//try to update components
    ConnectedComponent(int x, int y);
    void update(int x, int y);
};

#endif // CONNECTED_COMPONENT_H

