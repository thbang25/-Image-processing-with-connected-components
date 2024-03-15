#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <algorithm>
#include <limits>

using namespace std;

class ConnectedComponent {
public:
    int minX, minY, maxX, maxY;
    vector<pair<int, int>> pixels;

    ConnectedComponent(int x, int y) : minX(x), minY(y), maxX(x), maxY(y) {}

    void update(int x, int y) {
        minX = min(minX, x);
        minY = min(minY, y);
        maxX = max(maxX, x);
        maxY = max(maxY, y);
        pixels.emplace_back(x, y);
    }
};

class PGMimageProcessor {
private:
    int width;
    int height;
    vector<unsigned char> pixels;
    vector<shared_ptr<ConnectedComponent>> components;

public:
    PGMimageProcessor(const string& filename) {
        readPGM(filename);
    }

    ~PGMimageProcessor() {
        components.clear();
    }

    int extractComponents(unsigned char threshold, int minValidSize) {
        findConnectedComponents(threshold);
        filterComponentsBySize(minValidSize, numeric_limits<int>::max());
        return components.size();
    }

    int filterComponentsBySize(int minSize, int maxSize) {
        components.erase(
            remove_if(components.begin(), components.end(), [&](const shared_ptr<ConnectedComponent>& component) {
                int componentSize = component->pixels.size();
                return componentSize < minSize || componentSize > maxSize;
            }),
            components.end()
        );
        return components.size();
    }

    bool writeComponents(const string& outFileName) {
        ofstream file(outFileName, ios::binary);
        if (!file.is_open()) {
            cout << "Error: Unable to open file for writing." << endl;
            return false;
        }
        file << "P5" << endl;
        file << "# Connected components" << endl;
        file << width << " " << height << endl;
        file << 255 << endl;

        vector<vector<bool>> componentImage(width, vector<bool>(height, false));
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

private:
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

        pixels.resize(width * height);
        file.get(); // Skip newline character
        file.read(reinterpret_cast<char*>(&pixels[0]), width * height);
    }

void findConnectedComponents(unsigned char threshold) {
    vector<unsigned char> binaryImage(width * height, 0);
    for (int i = 0; i < width * height; ++i) {
        if (pixels[i] >= threshold) {
            binaryImage[i] = 255;
        }
    }

    vector<int> parent(width * height, -1);
    vector<int> rank(width * height, 0);

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

    vector<vector<int>> componentsIndices(width * height);
    for (int i = 0; i < width * height; ++i) {
        int root = find(i);
        componentsIndices[root].push_back(i);
    }

    for (const auto& indices : componentsIndices) {
        if (!indices.empty()) {
            auto component = make_shared<ConnectedComponent>(width, height);
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
    cout << "Number of components after extraction: " << componentCount << endl;

    int filteredCount = processor.filterComponentsBySize(20, numeric_limits<int>::max());
    cout << "Number of components after filtering: " << filteredCount << endl;

    bool success = processor.writeComponents("processed_components.pgm");
    if (success) {
        cout << "Processed components written to 'processed_components.pgm' successfully." << endl;
    } else {
        cout << "Failed to write processed components." << endl;
    }

    return 0;
}

