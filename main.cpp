#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <algorithm>
#include <functional>
#include <limits>

class ConnectedComponent {
public:
    int minX, minY, maxX, maxY;
    std::vector<std::pair<int, int>> pixels;

    ConnectedComponent(int x, int y) : minX(x), minY(y), maxX(x), maxY(y) {}

    void update(int x, int y) {
        minX = std::min(minX, x);
        minY = std::min(minY, y);
        maxX = std::max(maxX, x);
        maxY = std::max(maxY, y);
        pixels.emplace_back(x, y);
    }
};

class PGMimageProcessor {
private:
    int width;
    int height;
    std::vector<unsigned char> pixels;
    std::vector<std::shared_ptr<ConnectedComponent>> components;

public:
    PGMimageProcessor(const std::string& filename) {
        readPGM(filename);
    }

    int extractComponents(unsigned char threshold, int minValidSize) {
        findConnectedComponents(threshold);
        filterComponentsBySize(minValidSize, std::numeric_limits<int>::max());
        return components.size();
    }

    int filterComponentsBySize(int minSize, int maxSize) {
        components.erase(
            std::remove_if(components.begin(), components.end(), [&](const std::shared_ptr<ConnectedComponent>& component) {
                int componentSize = component->pixels.size();
                return componentSize < minSize || componentSize > maxSize;
            }),
            components.end()
        );
        return components.size();
    }

    bool writeComponents(const std::string& outFileName) {
        std::ofstream file(outFileName, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "Error: Unable to open file for writing." << std::endl;
            return false;
        }
        file << "P5" << std::endl;
        file << "# Connected components" << std::endl;
        file << width << " " << height << std::endl;
        file << 255 << std::endl;

        std::vector<std::vector<bool>> componentImage(width, std::vector<bool>(height, false));
        for (const auto& component : components) {
            for (const auto& pixel : component->pixels) {
                int x = pixel.first;
                int y = pixel.second;
                componentImage[x][y] = true;
            }
        }

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (componentImage[x][y]) {
                    file << static_cast<char>(255);
                } else {
                    file << static_cast<char>(0);
                }
            }
        }
        return true;
    }

    int getComponentCount() const {
        return components.size();
    }

    int getLargestSize() const {
        int largestSize = 0;
        for (const auto& component : components) {
            largestSize = std::max(largestSize, static_cast<int>(component->pixels.size()));
        }
        return largestSize;
    }

    int getSmallestSize() const {
        if (components.empty()) {
            return 0;
        }
        int smallestSize = components[0]->pixels.size();
        for (const auto& component : components) {
            smallestSize = std::min(smallestSize, static_cast<int>(component->pixels.size()));
        }
        return smallestSize;
    }

    void printComponentData(const ConnectedComponent & theComponent) const {
        std::cout << "Component ID: " << &theComponent << ", Number of pixels: " << theComponent.pixels.size() << std::endl;
    }

    const std::vector<std::shared_ptr<ConnectedComponent>>& getComponents() const {
        return components;
    }

private:
    void readPGM(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        std::string magicNumber;
        getline(file, magicNumber);
        if (magicNumber != "P5") {
            std::cout << "Error: Unsupported file format." << std::endl;
            return;
        }

        std::string comment;
        getline(file, comment); // Skip comment
        file >> width >> height;

        int maxVal;
        file >> maxVal; // Read the maximum pixel value

        pixels.resize(width * height);
        file.get(); // Skip newline character
        file.read(reinterpret_cast<char*>(&pixels[0]), width * height);
    }

    void findConnectedComponents(unsigned char threshold) {
        std::vector<unsigned char> binaryImage(width * height, 0);
        for (int i = 0; i < width * height; ++i) {
            if (pixels[i] >= threshold) {
                binaryImage[i] = 255;
            }
        }

        std::vector<int> parent(width * height, -1);
        std::vector<int> rank(width * height, 0);

        std::function<int(int)> find = [&](int x) -> int {
            if (parent[x] == -1) {
                return x;
            }
            return parent[x] = find(parent[x]);
        };

        auto unite = [&](int x, int y) {
            int rootX = find(x);
            int rootY = find(y);
            if (rootX != rootY) {
                if (rank[rootX] < rank[rootY]) {
                    parent[rootX] = rootY;
                } else if (rank[rootX] > rank[rootY]) {
                    parent[rootY] = rootX;
                } else {
                    parent[rootY] = rootX;
                    rank[rootX]++;
                }
            }
        };

        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                int index = y * width + x;
                if (binaryImage[index] == 255) {
                    int indexAbove = index - width;
                    int indexLeft = index - 1;
                    int indexRight = index + 1;
                    int indexBelow = index + width;

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

        std::vector<std::vector<int>> componentsIndices(width * height);
        for (int i = 0; i < width * height; ++i) {
            int root = find(i);
            componentsIndices[root].push_back(i);
        }

        for (const auto& indices : componentsIndices) {
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
};

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

