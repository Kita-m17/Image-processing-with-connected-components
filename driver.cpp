/**
 * @author Nikita Martin
 * MRTNIK003
 * @version 1
 */

#include "PGMimageProcessor.h"

/**
 * Prints usage instructions for the command-line tool.
 */
void printUsage() {
    std::cout << "Usage: findcomp [options] <inputPGMfile>\n";
    std::cout << "Options:\n";
    std::cout << "  -m <int>        Set the minimum size for valid components [default = 1]\n";
    std::cout << "  -f <int> <int>  Set min and max component sizes for filtering\n";
    std::cout << "  -t <int>        Set threshold for component detection [default = 128]\n";
    std::cout << "  -p              Print all component data\n";
    std::cout << "  -b <PPMimagename> Produce an output PPM image which is the original image with colour boxes drawn over it to show where each retained component is in the input image." <<std::endl;
    std::cout << "  -w <string>     Write retained components to a new PGM file\n";
    exit(1);
}

int main(int argc, char* argv[]){
    //ensure the input file is provided
    if(argc <2){
        printUsage();
        return 1;
    }

    //input/output filenames and options
    std::string inputFile = "", outputFile = "", ppmImageName;

    int minSize = 1;
    int maxSize = std::numeric_limits<int>::max();
    int threshold = 128;

    //various operations
    bool printComponents = false;
    bool writeOutput = false;
    bool drawBoarder = false;
    bool filterComponents = false;
    
    //parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string option = argv[i];
        if (option == "-m" && i + 1 < argc) {
            minSize = std::stoi(argv[++i]);
        } else if ((option == "-f" || option == "s") && i + 2 < argc) {
            filterComponents = true;
            minSize = std::stoi(argv[++i]);
            maxSize = std::stoi(argv[++i]);
        } else if (option == "-t" && i + 1 < argc) {
            threshold = std::stoi(argv[++i]);
        } else if (option == "-p") {
            printComponents = true;
        } else if(option == "-b" && i + 1 < argc) {
            drawBoarder = true;
            writeOutput = true; 
            ppmImageName = argv[++i];
        } else if (option == "-w" && i + 1 < argc) {
            outputFile = argv[++i];
            writeOutput = true;
        } else {
            inputFile = option;
        }
    }

    if (inputFile.empty()) {
        printUsage();
    }

    //load pgm image file
    PGMimageProcessor imageProcessor;
    
    std::cout << "Reading in file..." << std::endl;
    bool isPPM = imageProcessor.isPPMFile(inputFile);

    bool readFile;
    if(isPPM){
        readFile = imageProcessor.readPGM<true>(inputFile);
    }else{
        readFile = imageProcessor.readPGM<false>(inputFile);
    }
    if (!readFile) {
        std::cerr << "Error: Failed to load PGM file." << std::endl;
        return 1;
    }

    //extract components above the threshold and minimum size
    int numComponents = imageProcessor.extractComponents(threshold, minSize);
    std::cout << "Extracted Components: " << numComponents <<std::endl;
    
    //optionally filter components by range
    if(filterComponents){
        int filtered = imageProcessor.filterComponentsBySize( minSize, maxSize);
        std::cout << "Filtered Components: " << filtered <<std::endl;
    }

    //optionally print all the components
    if (printComponents) {
        for (const std::shared_ptr<ConnectedComponent> & component : imageProcessor.getComponents()) { 
            imageProcessor.printComponentData(*component);
        }
        std::cout << "Printed Components" <<std::endl;

    }
    
    //writes the output to a pgm file
    if (writeOutput) {
        bool success = success = imageProcessor.writeComponents<bool>(outputFile);
        
        if (!success) {
            std::cerr << "Error writing PGM output file: " << outputFile << std::endl;
        }
    }

    //writes output PPM file with bounding boxes drawn around components
    if (drawBoarder) {
        bool success = imageProcessor.writeComponents<bool, true>(ppmImageName);
        if (!success) {
            std::cerr << "Error writing PPM output file with bounding boxes: " << ppmImageName << std::endl;
        }
    }

    //print summary of analysis
    std::cout << "Components: " << imageProcessor.getComponentCount() <<std::endl;
    std::cout << "Smallest: " << imageProcessor.getSmallestSize() << std::endl;
    std::cout << "Largest: " << imageProcessor.getLargestSize() << std::endl;

    return 0;
}