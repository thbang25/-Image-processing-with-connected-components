#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>

using namespace std;

// Class to represent a connected component
class ConnectedComponent {
public:
    int minX, minY, maxX, maxY;

    ConnectedComponent(int x, int y, int width, int height) : minX(x), minY(y), maxX(x), maxY(y) {}

    void updateBounds(int x, int y) {
        minX = min(minX, x);
        minY = min(minY, y);
        maxX = max(maxX, x);
        maxY = max(maxY, y);
    }
};

// Class to represent a PGM image processor
class PGMimageProcessor {
private:
    int width;
    int height;
    vector<unsigned char> pixels;
    vector<shared_ptr<ConnectedComponent>> components;

public:
    // Method to read a PGM image
    void readPGM(const string& filename) {
        ifstream file(filename, ios::binary);
        string magicNumber;
        getline(file, magicNumber);
        if (magicNumber != "P5") {
            cout << "Error: Unsupported file format." << endl;
            return;
        }

        string comment;
        getline(file, comment); // Skip comment
        file >> width >> height;

        int maxVal;
        file >> maxVal; // Read the maximum pixel value

        // Read pixel data
        pixels.resize(width * height);
        file.get(); // Skip newline character
        file.read(reinterpret_cast<char*>(&pixels[0]), width * height);
    }

    // Method to find connected components
    void findConnectedComponents() {
        // Convert the image to binary
        vector<unsigned char> binaryImage(width * height, 0); // Initialize all pixels to 0

        // Convert grayscale image to binary image using a threshold value (e.g., 128)
        for (int i = 0; i < width * height; ++i) {
            if (pixels[i] >= 128) {
                binaryImage[i] = 255; // Foreground pixel
            }
        }

// Perform connected component labeling using Union-Find algorithm
vector<int> parent(width * height, -1); // Parent array for Union-Find
vector<int> rank(width * height, 0);   // Rank array for Union-Find

function<int(int)> find = [&](int x) -> int {
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

        // Perform connected component labeling
        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                int index = y * width + x;
                if (binaryImage[index] == 255) { // If the pixel is foreground
                    // Check neighboring pixels
                    int indexAbove = index - width;
                    int indexLeft = index - 1;
                    int indexRight = index + 1;
                    int indexBelow = index + width;

                    if (binaryImage[indexAbove] == 255) { // Top neighbor
                        unite(index, indexAbove);
                    }
                    if (binaryImage[indexLeft] == 255) { // Left neighbor
                        unite(index, indexLeft);
                    }
                    if (binaryImage[indexRight] == 255) { // Right neighbor
                        unite(index, indexRight);
                    }
                    if (binaryImage[indexBelow] == 255) { // Bottom neighbor
                        unite(index, indexBelow);
                    }
                }
            }
        }

        // Traverse the image to extract connected components
        vector<vector<int>> componentsIndices(width * height);
        for (int i = 0; i < width * height; ++i) {
            int root = find(i);
            componentsIndices[root].push_back(i);
        }

        // Create ConnectedComponent objects and populate the 'components' vector
        for (const auto& indices : componentsIndices) {
            if (!indices.empty()) {
                int minX = width, minY = height, maxX = 0, maxY = 0;
                for (int index : indices) {
                    int x = index % width;
                    int y = index / width;
                    minX = min(minX, x);
                    minY = min(minY, y);
                    maxX = max(maxX, x);
                    maxY = max(maxY, y);
                }
                components.push_back(make_shared<ConnectedComponent>(minX, minY, maxX, maxY));
            }
        }
    }

    // Method to write a PGM image with selected components
    void writeSelectedComponents(const string& filename) {
        ofstream file(filename, ios::binary);
        file << "P5" << endl;
        file << "# Selected components" << endl;
        file << width << " " << height << endl;
        file << 255 << endl; // Maximum pixel value

        // Write pixel data with selected components
        for (int i = 0; i < width * height; ++i) {
            bool selected = false;
            for (const auto& component : components) {
                if (i % width >= component->minX && i % width <= component->maxX &&
                    i / width >= component->minY && i / width <= component->maxY) {
                    selected = true;
                    break;
                }
            }
            if (selected)
                file << static_cast<char>(255);
            else
                file << static_cast<char>(0);
        }
    }
};

int main() {
    PGMimageProcessor processor;

    // Read the input image
    processor.readPGM("mona512.pgm");

    // Find connected components
    processor.findConnectedComponents();

    // Write the selected components to a new PGM file
    processor.writeSelectedComponents("selected_components.pgm");

    cout << "Selected components saved successfully." << endl;

    return 0;
}
