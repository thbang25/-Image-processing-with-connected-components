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

//use PGM to process images
class PGMimageProcessor {
private:
//private variables
    int width;
    int height;
    std::vector<unsigned char> pixels;
    std::vector<std::shared_ptr<ConnectedComponent>> components;
//public method declarations
public:
    PGMimageProcessor(const std::string& filename); //read and initialize

    int extractComponents(unsigned char threshold, int minValidSize);//extract the connected componets in the file

    int filterComponentsBySize(int minSize, int maxSize);//go through the components by given size

    bool writeComponents(const std::string& outputImage);//write the component data to the image file

    int getComponentCount() const; //count the number of components

    int getLargestSize() const; //get the biggest component size value

    int getSmallestSize() const; //get the smallest component size value

    void printComponentData(const ConnectedComponent& theComponent) const; //print the componet data

    const std::vector<std::shared_ptr<ConnectedComponent>>& getComponents() const; //creating a vector componets

private:
//read our image pgm file
    void readPGM(const std::string& filename);
//extract the connected componennts
    void extractConnectedComponents(unsigned char threshold);
};

#endif // PGM_IMAGE_PROCESSOR_H
