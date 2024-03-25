/** Thabang  sambo
 *  23/03/2024
 *  encapsulates the functionality you need to read
 *  in a PGM image and apply a connected component analysis to that imag and then write out the image after transformation
 **/

#ifndef PGM_IMAGE_PROCESSOR_H
#define PGM_IMAGE_PROCESSOR_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <algorithm>
#include <functional>
#include <limits>
#include "ConnectedComponent.h"

class PGMimageProcessor {
public:
    PGMimageProcessor(const std::string& filename);
    PGMimageProcessor(const PGMimageProcessor& other); // Copy constructor
    PGMimageProcessor& operator=(const PGMimageProcessor& other); // Copy assignment operator
    PGMimageProcessor(PGMimageProcessor&& other) noexcept; // Move constructor
    PGMimageProcessor& operator=(PGMimageProcessor&& other) noexcept; // Move assignment operator
    ~PGMimageProcessor(); // Destructor

    int extractComponents(unsigned char threshold, int minValidSize);//extract the connected componets in the file
    int filterComponentsBySize(int minSize, int maxSize);//go through the components by given size
    bool writeComponents(const std::string& outputImage);//write the component data to the image file
    int getComponentCount() const;//count the number of components
    int getLargestSize() const; //get the biggest component size value
    int getSmallestSize() const; //get the smallest component size value
    void printComponentData(const ConnectedComponent& theComponent) const; //print the componet data
    const std::vector<std::shared_ptr<ConnectedComponent>>& getComponents() const; //creating a vector componets

private:
    void readPGM(const std::string& filename); //read our image pgm file
    void extractConnectedComponents(unsigned char threshold); //extract the connected componennts

private:
//private variables
    int width;
    int height;
    std::vector<unsigned char> pixels;
    std::vector<std::shared_ptr<ConnectedComponent>> components;
};

#endif // PGM_IMAGE_PROCESSOR_H
