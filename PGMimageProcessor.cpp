/** Thabang  sambo
 *  23/03/2024
 *  encapsulates the functionality you need to read
 *  in a PGM image and apply a connected component analysis to that imag and then write out the image after transformation
 **/


#include "PGMimageProcessor.h"


//read image data
PGMimageProcessor::PGMimageProcessor(const std::string& filename) {
    readPGM(filename);
}

// Copy constructor
PGMimageProcessor::PGMimageProcessor(const PGMimageProcessor& other) :
    width(other.width), height(other.height), pixels(other.pixels), components(other.components) {}

// Copy assignment operator
PGMimageProcessor& PGMimageProcessor::operator=(const PGMimageProcessor& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        pixels = other.pixels;
        components = other.components;
    }
    return *this;
}

// Move constructor
PGMimageProcessor::PGMimageProcessor(PGMimageProcessor&& other) noexcept :
    width(std::move(other.width)), height(std::move(other.height)),
    pixels(std::move(other.pixels)), components(std::move(other.components)) {}

// Move assignment operator
PGMimageProcessor& PGMimageProcessor::operator=(PGMimageProcessor&& other) noexcept {
    if (this != &other) {
        width = std::move(other.width);
        height = std::move(other.height);
        pixels = std::move(other.pixels);
        components = std::move(other.components);
    }
    return *this;
}

// Destructor
PGMimageProcessor::~PGMimageProcessor() {}

int PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize) {
    extractConnectedComponents(threshold);
    filterComponentsBySize(minValidSize, std::numeric_limits<int>::max());
    return components.size();
}

int PGMimageProcessor::filterComponentsBySize(int minSize, int maxSize) {
    components.erase(
        std::remove_if(components.begin(), components.end(), [&](const std::shared_ptr<ConnectedComponent>& component) {
            int sizeOfComponent = component->pixels.size();
            return sizeOfComponent < minSize || sizeOfComponent > maxSize;
        }),
        components.end()
    );
    return components.size();
}

bool PGMimageProcessor::writeComponents(const std::string& outputImage) {
    std::ofstream ImageData(outputImage, std::ios::binary);
    if (!ImageData.is_open()) {
        std::cout << "Error: Unable to open image" << std::endl;
        return false;
    }

    ImageData << "P5" << std::endl;
    ImageData << "# comments" << std::endl;
    ImageData << width << " " << height << std::endl;
    ImageData << 255 << std::endl;

    std::vector<std::vector<bool>> fileComponentData(width, std::vector<bool>(height, false));
    for (const auto& component : components) {
        for (const auto& pixel : component->pixels) {
            int x = pixel.first;
            int y = pixel.second;
            fileComponentData[x][y] = true;
        }
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (fileComponentData[x][y]) {
                ImageData << static_cast<char>(255);
            } else {
                ImageData << static_cast<char>(0);
            }
        }
    }
    return true;
}

int PGMimageProcessor::getComponentCount() const {
    return components.size();
}

int PGMimageProcessor::getLargestSize() const {
    int largestSize = 0;
    for (const auto& component : components) {
        largestSize = std::max(largestSize, static_cast<int>(component->pixels.size()));
    }
    return largestSize;
}

int PGMimageProcessor::getSmallestSize() const {
    if (components.empty()) {
        return 0;
    }
    int smallestSize = components[0]->pixels.size();
    for (const auto& component : components) {
        smallestSize = std::min(smallestSize, static_cast<int>(component->pixels.size()));
    }
    return smallestSize;
}

void PGMimageProcessor::printComponentData(const ConnectedComponent& componentidentity) const {
    std::cout << "Component ID: " << &componentidentity << " Number of pixels: " << componentidentity.pixels.size() << std::endl;
}

const std::vector<std::shared_ptr<ConnectedComponent>>& PGMimageProcessor::getComponents() const {
    return components;
}


//read the image file
void PGMimageProcessor::readPGM(const std::string& filename) {
    std::ifstream ImageData(filename, std::ios::binary);
    std::string magicNumber;
    getline(ImageData, magicNumber);
    if (magicNumber != "P5") {
        std::cout << "Error: Unsupported Image format." << std::endl;
        return;
    }

    std::string comment;
    getline(ImageData, comment);
    ImageData >> width >> height;

    int maximumValaue;
    ImageData >> maximumValaue; // Read the maximum pixel value

    //resize the pixel to match expected dimensions
    pixels.resize(width * height);
    ImageData.get(); // Skip newline character
    ImageData.read(reinterpret_cast<char*>(&pixels[0]), width * height);
}


//extract componets
void PGMimageProcessor::extractConnectedComponents(unsigned char threshold) {
	//create a new binary image from the input image
    std::vector<unsigned char> binaryImage(width * height, 0);
    for (int i = 0; i < width * height; ++i) {
        if (pixels[i] >= threshold) {
            binaryImage[i] = 255;
        }
    }

//initialize the root and the depth for the vectors using Union Find
    std::vector<int> elementRoot(width * height, -1);
    std::vector<int> elementDepth(width * height, 0);

//we then define the union find function
    std::function<int(int)> find = [&](int x) -> int {
        if (elementRoot[x] == -1) {
            return x;
        }
		//compress the path
        return elementRoot[x] = find(elementRoot[x]);
    };
//define the unite for UFA
    auto unite = [&](int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
			//perfom union by depth
            if (elementDepth[rootX] < elementDepth[rootY]) {
                elementRoot[rootX] = rootY;
            } else if (elementDepth[rootX] > elementDepth[rootY]) {
                elementRoot[rootY] = rootX;
            } else {
                elementRoot[rootY] = rootX;
                elementDepth[rootX]++;
            }
        }
    };
//go through the binary inage to find the connections
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int index = y * width + x;
            if (binaryImage[index] == 255) {
                int indexAbove = index - width;
                int indexLeft = index - 1;
                int indexRight = index + 1;
                int indexBelow = index + width;

//unite if there are neighbors in the connected component values
                if (binaryImage[indexAbove] == 255) {
                    unite(index, indexAbove);
                }
                if (binaryImage[indexLeft] == 255) {
                    unite(index, indexLeft);
                }
                if (binaryImage[indexRight] == 255) {
                    unite(index, indexRight);
                }
                if (binaryImage[indexBelow] == 255) {
                    unite(index, indexBelow);
                }
            }
        }
    }

//gather the pixels for the connected values found by UF alg 
 std::vector<std::vector<int>> connectedIndexs(width * height);
    for (int i = 0; i < width * height; ++i) {
        int root = find(i);
        connectedIndexs[root].push_back(i);//add
    }

//create an object for the connections
    for (const auto& indices : connectedIndexs) {
        if (!indices.empty()) {
            auto component = std::make_shared<ConnectedComponent>(width, height);
            for (int index : indices) {
                int x = index % width;
                int y = index / width;
                component->update(x, y);
            }
            components.push_back(component);
        }
    }
}
