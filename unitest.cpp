#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ConnectedComponent.h"
#include "PGMimageProcessor.h"

TEST_CASE("PGMimageProcessor extractComponents") {
    // we are using mona512 from using the defaukt values from
    PGMimageProcessor Processor("./data/mona512.pgm");

    // Extract components with a threshold of 128
    int componentCount = Processor.extractComponents(128, 10, std::numeric_limits<int>::max());

    // Check if the correct number of components is extracted
    REQUIRE(componentCount == 70);

    // Check if the smallest and largest sizes are correct
    REQUIRE(Processor.getSmallestSize() == 10);
    REQUIRE(Processor.getLargestSize() == 214095);
}

TEST_CASE("PGMimageProcessor writeComponents") {
    PGMimageProcessor Processor("./data/mona512.pgm");
    Processor.extractComponents(128, 10, std::numeric_limits<int>::max());

    // Write components to an output image file
    bool success = Processor.writeComponents("output_image.pgm");

    // Check if writing to file was successful
    REQUIRE(success == true);

}



