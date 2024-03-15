#include "PGMimageProcessor.h"

int main() {
    PGMimageProcessor processor("mona512.pgm");

    int componentCount = processor.extractComponents(128, 10);
    std::cout << "Number of components after extraction: " << componentCount << std::endl;

    int filteredCount = processor.filterComponentsBySize(20, std::numeric_limits<int>::max());
    std::cout << "Number of components after filtering: " << filteredCount << std::endl;

    bool success = processor.writeComponents("processed_components.pgm");
    if (success) {
        std::cout << "Processed components written to 'processed_components.pgm' successfully." << std::endl;
    } else {
        std::cout << "Failed to write processed components." << std::endl;
    }

    int largestSize = processor.getLargestSize();
    std::cout << "Number of pixels in largest component: " << largestSize << std::endl;

    int smallestSize = processor.getSmallestSize();
    std::cout << "Number of pixels in smallest component: " << smallestSize << std::endl;

    const std::vector<std::shared_ptr<ConnectedComponent>>& components = processor.getComponents();
    if (components.empty()) {
        std::cout << "No components found." << std::endl;
        return 0;
    }

    std::cout << "Component data:" << std::endl;
    for (const auto& component : components) {
        processor.printComponentData(*component);
    }

    return 0;
}
