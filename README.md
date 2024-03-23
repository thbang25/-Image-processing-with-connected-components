# -Image-processing-with-connected-components

*ConnectedComponents.cpp*

Constructor (ConnectedComponent::ConnectedComponent(int x, int y)): Initializes the minX, minY, maxX, and maxY coordinates of the connected component with the given x and y coordinates. The pixels container holds all the coordinates of the pixels that belong to this connected component.

Copy Constructor (ConnectedComponent::ConnectedComponent(const ConnectedComponent& other)) and Copy Assignment Operator (ConnectedComponent::operator=): These methods enable the creation of a copy of a ConnectedComponent object. They perform a deep copy of the pixels container and duplicate the bounding box coordinates (minX, minY, maxX, maxY).

Move Constructor (ConnectedComponent::ConnectedComponent(ConnectedComponent&& other) noexcept) and Move Assignment Operator (ConnectedComponent::operator=(ConnectedComponent&& other) noexcept): The following methods are used to move the content of one ConnectedComponent object to another, which can be more efficient than copying, especially for large objects. They utilize move semantics to transfer the ownership of the pixels container and the bounding box coordinates.

Destructor (ConnectedComponent::~ConnectedComponent()): This is the destructor of the ConnectedComponent class. In this case, it does not perform any actions since there are no dynamically allocated resources to clean up.

update Method (void ConnectedComponent::update(int x, int y)): This method updates the bounding box coordinates (minX, minY, maxX, maxY) based on the input x and y coordinates. It also adds the input coordinates to the pixels container.

Class Definition: The ConnectedComponent class is defined, which encapsulates the information and functionality related to a connected component in an image.

Constructor: The class has a constructor ConnectedComponent(int x, int y) that initializes a connected component with the given x and y coordinates.

Copy Constructor and Copy Assignment Operator: These methods allow creation of copies of ConnectedComponent objects. They ensure that deep copies are made so that the copied object has its independent data.

Move Constructor and Move Assignment Operator: These methods implement move semantics, allowing efficient transfer of resources from one ConnectedComponent object to another. The class includes a destructor ~ConnectedComponent() that handles cleanup tasks. However, no cleanup is necessary in this case as there are no dynamically allocated resources.

The update method modifies the bounding box coordinates of the connected component based on the input x and y coordinates. It also adds the input coordinates to the pixel vector.

As for data members, the class contains public data members minX, minY, maxX, and maxY, which represent the bounding box coordinates of the connected component. Additionally, it includes a std::vector<std::pair<int, int>> pixels to store the coordinates of pixels belonging to the connected component.

*PGMimageProcessor*

Class Definition: The PGMImageProcessor class is defined, which encapsulates functionality related to processing PGM (Portable Gray Map) images.
Constructor: The class has a constructor PGMImageProcessor(const std::string& filename) which takes the filename of the PGM image to be processed.
The PGMImageProcessor class includes the following methods:

Copy Constructor and Copy Assignment Operator: These methods enable the creation of copies of PGMImageProcessor objects. They ensure that deep copies are made, so the copied object has its independent data.

Move Constructor and Move Assignment Operator: These methods implement move semantics, allowing for the efficient transfer of resources from one PGMImageProcessor object to another.

Destructor: The class has a destructor ~PGMImageProcessor() which performs cleanup tasks. In this case, it doesn't need to perform any cleanup because there are no dynamically allocated resources.
Public Member Functions:
extractComponents(unsigned char threshold, int minValidSize): Extracts connected components from the image based on a threshold and minimum valid size.
filterComponentsBySize(int minSize, int maxSize): Filters components based on their size.
writeComponents(const std::string& outputImage): Writes the components to an output image file.
getComponentCount() const: Returns the count of components.
getLargestSize() const: Returns the size of the largest component.
getSmallestSize() const: Returns the size of the smallest component.
printComponentData(const ConnectedComponent& theComponent) const: Prints data about a specific component.
getComponents() const: Returns a constant reference to the vector of shared pointers to ConnectedComponent objects.
Private Member Functions:
readPGM(const std::string& filename): Reads the PGM image file.
extractConnectedComponents(unsigned char threshold): Extracts connected components from the image.
Private Data Members:
int width: Width of the image.
int height: Height of the image.
std::vector<unsigned char> pixels: Vector to store pixel values of the image.
std::vector<std::shared_ptr<ConnectedComponent>> components: Vector to store shared pointers to ConnectedComponent objects representing connected components extracted from the image.

Constructor (PGMimageProcessor::PGMimageProcessor(const std::string& filename)): This method reads the PGM image data from the specified file using the readPGM function.

Copy Constructor (PGMimageProcessor::PGMimageProcessor(const PGMimageProcessor& other)) and Copy Assignment Operator (PGMimageProcessor::operator=(const PGMimageProcessor& other)): These methods perform a deep copy of the PGMimageProcessor object, including its width, height, pixels, and components.

Move Constructor (PGMimageProcessor::PGMimageProcessor(PGMimageProcessor&& other) noexcept) and Move Assignment Operator (PGMimageProcessor::operator=(PGMimageProcessor&& other) noexcept): These methods efficiently move the resources (width, height, pixels, and components) from one PGMimageProcessor object to another.

Destructor (PGMimageProcessor::~PGMimageProcessor()): The destructor doesn't do anything specific because there are no dynamically allocated resources to clean up.

extractComponents(unsigned char threshold, int minValidSize): This method extracts connected components from the image based on a threshold value and filters out components smaller than minValidSize.

filterComponentsBySize(int minSize, int maxSize): This method filters components based on their size. It removes components whose size is not within the specified range.

writeComponents(const std::string& outputImage): This method writes the connected components to an output image file. It creates a binary image based on the components and writes it to the specified file.

getComponentCount(), getLargestSize(), getSmallestSize(), printComponentData(), and getComponents(): These methods provide various information about the connected components, such as the count, size, and data of the components.

readPGM(const std::string& filename): This method reads the PGM image file and stores the image data (width, height, and pixel values) in the respective member variables.

extractConnectedComponents(unsigned char threshold): This method extracts connected components from the binary image obtained after applying a threshold. It utilizes the Union-Find algorithm to efficiently find connected components and gather their pixels.

