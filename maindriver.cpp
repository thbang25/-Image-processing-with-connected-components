#include "PGMimageProcessor.h"
#include <iostream>

using namespace std;

int main() {
    PGMimageProcessor processor;
    processor.readPGM("mona512.pgm");
    processor.findConnectedComponents();
    processor.writeSelectedComponents("output.pgm");
    std::cout << "Processing complete." << std::endl;
    return 0;
}
