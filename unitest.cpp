/** Thabang  sambo
 *  23/03/2024
 *  unit testing methods
 *  
 **/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ConnectedComponent.h"
#include "PGMimageProcessor.h"


//test if we actually extract the image components
TEST_CASE("PGMimageProcessor extractComponents") {
    // we are using mona512 from using the defaukt values from
    PGMimageProcessor Processor("./data/mona512.pgm");

    // Extract components with a threshold of 128
    int componentCount = Processor.extractComponents(128, 10);

    // Check if the correct number of components is extracted
    REQUIRE(componentCount == 70);

    // Check if the smallest and largest sizes are correct
    REQUIRE(Processor.getSmallestSize() == 10);//should be
    REQUIRE(Processor.getLargestSize() == 214095);
}

//testing if the program write components is correct
TEST_CASE("PGMimageProcessor writeComponents") {
    PGMimageProcessor Processor("./data/mona512.pgm");
    Processor.extractComponents(128, 3);//my default values

    // Write components to an output image file
    bool success = Processor.writeComponents("output_image.pgm");

    // Check if writing to file was successful
    REQUIRE(success == true);

}

//test the behavior pf the extractions from
TEST_CASE("Different type of component test") {
    //created a pgm with no components
    PGMimageProcessor Processor("./data/binary_image.pgm");//removed all components
    int count = Processor.extractComponents(128, 3);//my default values
    REQUIRE(count == 0); // No components should be detected in this image

}

//test that we can actually print the components data
TEST_CASE("Test printComponentData") {
    // mona512 has components that we already know about
    PGMimageProcessor Processor("./data/mona512.pgm");
    Processor.extractComponents(128, 3);//my default values

    const auto& components = Processor.getComponents();
    REQUIRE(!components.empty()); // we should have some components

    SECTION("Test if we can print the components data") {
        for (const auto& component : components) {
            CAPTURE(component.get()); // use capture for the pointer
            Processor.printComponentData(*component);

        }
    }
}

//test if we actually filter through the components
TEST_CASE("Test filterComponentsBySize") {
    PGMimageProcessor Processor("./data/mona512.pgm");
    Processor.extractComponents(128, 3); //my default values

//see if we filter by the size
    SECTION("Filter components by size") {
        int initalCount = Processor.getComponentCount(); //the initial count
        int minSize = 20; // minimum size
        int maxSize = 50; //insert designated size
        int currentCount = Processor.filterComponentsBySize(minSize, maxSize);//the cuurent count of the components
        REQUIRE(currentCount < initalCount);
        REQUIRE(Processor.getComponentCount() == currentCount); //check the count
    }
}

//try to check if the constructor works
TEST_CASE("Test PGMimageProcessor Constructor") {
    PGMimageProcessor Processor("./data/mona512.pgm"); //use the same file image
    REQUIRE(Processor.getComponentCount() == 0); //my default values
}

