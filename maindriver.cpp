/** Thabang  sambo
 *  23/03/2024
 *  main program to run and connect compilation files
 **/

#include "PGMimageProcessor.h"

int main(int argc, char* argv[]) {
	
   //accept command line arguments
    if (argc < 6) {
        std::cerr << "Usage: " << argv[0] << " <input_imagename.pgm> <minValue> <maxValue> <threshold> <output_imagename.pgm>" << std::endl;
        return 1;
    }
	
	//DEFAULT VALUES
	//file in data folder to avoid cleaning it
	std::string imagename = "mona512.pgm"; //get the name of the file
    
	//mona512.pgm and other images are in the data folder to test it
	int minValue = 3; //default for min
	int maxValue =  std::numeric_limits<int>::max(); // default for max
	int threshold = 128;
	//variable to handle output file name
    std::string outputImagename = "binary.pgm";
	
	
	//ARGUMENTS ACCEPTED FROM COMMAND LINE
	if (argc >= 2) {
       imagename = argv[1]; //user file name
    }
    if (argc >= 3) {
        minValue = std::atoi(argv[2]); //user minimum value
    }
    if (argc >= 4) {
        maxValue = std::atoi(argv[3]); //user max value
    }
    if (argc >= 5) {
        int value = std::atoi(argv[4]); //user max threshold
		if(value<0 || value>255){
			threshold = 128;
		}
		else{
			threshold = value;
		}
    }
    if (argc >= 6) {
        outputImagename = argv[5]; //user output name
    }
	
//read the image file
	std::string filepath = "./data/"+imagename; //file path to get the tile in a folder 
    PGMimageProcessor processor(filepath);

//set the component dimesions
    int numOfcomponents = processor.extractComponents(128, 10);
	
//show the componentts after getting component data
    std::cout << "component count atfer extraction: " << numOfcomponents << std::endl;
//show the number of components after extraction
    int countFiltration = processor.filterComponentsBySize(20, std::numeric_limits<int>::max());
    std::cout << "component count after filtering: " << countFiltration << std::endl;

//if we finish writing the new binary file

    bool writeComplete = processor.writeComponents(outputImagename);
	
	//check if the condition is true
    if (writeComplete) {
        std::cout << "components written to 'binary.pgm' successfully." << std::endl;
    } else {//if failed show error messages
        std::cout << "Failed!!!!." << std::endl;
    }

//get the largest component value
    int largestSize = processor.getLargestSize();
    std::cout << "largest component: " << largestSize << std::endl;
//get the smallest component value
    int smallestSize = processor.getSmallestSize();
    std::cout << "smallest component: " << smallestSize << std::endl;
//get the connected component data
    const std::vector<std::shared_ptr<ConnectedComponent>>& components = processor.getComponents();
	
//if there are no components tell user
    if (components.empty()) {
        std::cout << "Error!!! No components found." << std::endl;
        return 0;
    }

//show the component data
    std::cout << "Component data:" << std::endl;
    for (const auto& component : components) {
        processor.printComponentData(*component);
    }

    return 0;//end main
}
