/**
 * @author Nikita Martin
 * MRTNIK003
 * @version 1
 * @date April 2025
 */

#include "PGMimageProcessor.h"

//Constructors and Destructir (Big 6)
/**
* Default constructor
* Initialise an empty PGM image with zero dimensions and no components
*/
PGMimageProcessor::PGMimageProcessor(): width(0), height(0), maxVal(0), components(), fileName(""){}

/**
* Destructor
* Cleans up any allocated resources (handled by smart pointers and STL containers)
*/
PGMimageProcessor::~PGMimageProcessor(){}

/**
* Parameterized Constructor
* Attempts to load abd parse a PGM file upon construction
*/
PGMimageProcessor::PGMimageProcessor(const std::string &inputImageName): 
    width(0), 
    height(0), 
    maxVal(0),
    fileName(inputImageName),
    components()
{
    bool isPPM = isPPMFile(inputImageName);
    if(isPPM){
        if (!readPGM<true>(inputImageName)) {
            std::cerr << "Failed to read input PPM file: " << inputImageName << std::endl;
        }
    }
    else{
        if (!readPGM<false>(inputImageName)) {
            std::cerr << "Failed to read input PGM file: " << inputImageName << std::endl;
        }
    }
}

/**
* Copy Constructor
* Deep copies all fields from another processor instance
*/
PGMimageProcessor::PGMimageProcessor(const PGMimageProcessor & processor):
    width(processor.width),
    height(processor.height),
    maxVal(processor.maxVal),
    fileName(processor.fileName),
    imageData(processor.imageData),
    components(processor.components)
{}

/**
* Move Constructor
* Transfers all fields from one PGMimageProcessor instance to another
*/
PGMimageProcessor::PGMimageProcessor(PGMimageProcessor && processor):
    width(processor.width),
    height(processor.height),
    maxVal(processor.maxVal),
    fileName(std::move(processor.fileName)),
    imageData(std::move(processor.imageData)),
    components(std::move(processor.components))
{
    processor.maxVal = 0;
    processor.height = 0;
    processor.width = 0;
}

/**
* Copy Assignment Operator
* Deep copy assignment with self checking
*/
PGMimageProcessor & PGMimageProcessor::operator=(const PGMimageProcessor & processor){
    if(this!= &processor){
        width = processor.width;
        height = processor.height;
        maxVal = processor.maxVal;
        imageData = processor.imageData;
        components = processor.components;
        fileName = processor.fileName;
    }
    return *this;
}

/**
* Move Assignment Operator
* Move assignment with cleanup of the moved-from object
*/
PGMimageProcessor & PGMimageProcessor::operator=(PGMimageProcessor && processor){
    if(this != &processor){
        width = processor.width;
        height = processor.height;
        maxVal = processor.maxVal;
        fileName = std::move(processor.fileName);
        imageData = std::move(processor.imageData);
        components = std::move(processor.components);
    
        processor.width = 0;
        processor.height = 0;
        processor.maxVal = 0;
    }
    return *this;
}

//Core methods
/**
 * Extracts connected components from a grayscale image by using a given threshold.
 * Pixels >= threshold are treated as foreground (255), else background (0).
 * Uses four-neighbour Breadth First Search to label each connected as a foreground component.
 * 
 * @param threshold The intensity-threshold to separate foreground and background pixels.
 * @param minValidSize The minimum number of pixels a component must have to be considered valid.
 * @return Number of valid connected components extracted.
 */
int PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize){
    //clear existing components
    components.clear();
    
    //create a temp binary image based on the threshold
    std::vector<unsigned char> binaryImage = imageData;
    for(size_t i = 0; i<binaryImage.size(); ++i){
        if(binaryImage[i] >= threshold){
            binaryImage[i] = 255; //foreground
        }else{
            binaryImage[i] = 0; //background
        }
    }

    //use lables to track which pixels have been processed
    std::vector<int> labels(width*height, -1); //stores the connected components - checks if pixel visited
    int componentID = 0;

    //loop through each pixel in the image
    for(int y = 0; y< height; ++y){
        for(int x = 0; x< width; ++x){
            int index = y*width+x;

            if(binaryImage[index] ==255 && labels[index] == -1){ //checks if the components hasnt been processed (index == -1) and its a foreground pixel
                //create a new component
                std::vector<std::pair<int, int>> pixels;
                std::queue<std::pair<int, int>> queue; //We use bfs to search for connected pixels

                queue.push({x, y}); //add component to the queue
                labels[index] = componentID; //mark as visited
                binaryImage[index] = 0;
                pixels.push_back({x, y});

                while(!queue.empty()){
                    std::pair<int, int> current = queue.front();
                    int currX = current.first; //x-coord
                    int currY = current.second; //y-coord
                    
                    queue.pop(); //remove the first element

                    //check 4 neighbours (N, E, S, W)
                    std::vector<std::pair<int, int>> neighbours;
                    
                    //north neighbour (x, y-1)
                    neighbours.push_back(std::make_pair(currX, currY - 1));

                    //east neighbour (x+1, y)
                    neighbours.push_back(std::make_pair(currX + 1, currY));
                    
                    //south neighbour (x, y+1)
                    neighbours.push_back(std::make_pair(currX, currY + 1));
                    
                    //west neighbour (x-1, y)
                    neighbours.push_back(std::make_pair(currX - 1, currY));

                    //check each neighnour
                    for(size_t i = 0; i<neighbours.size(); ++i){
                        int nx = neighbours[i].first; //x-coord of neighbour
                        int ny = neighbours[i].second; //y-coord of neighbour

                        //check if neighbour is in image boundaries
                        if((nx >= 0 && nx < width) &&( ny >= 0 && ny < height)){
                            int neighbourIndex = ny*width + nx; //1d index

                            //check if the neighbour is a forground and hasn't been processed
                            if(binaryImage[neighbourIndex] == 255 && labels[neighbourIndex] == -1){
                                //add neighbour to queue
                                queue.push(std::make_pair(nx, ny));

                                //mark it - current component
                                labels[neighbourIndex] = componentID;
                                binaryImage[neighbourIndex] = 0;

                                //add to list of pixels in this component
                                pixels.push_back(std::make_pair(nx, ny));
                            }
                        }
                    }
                }
                //after connected pixels are processed - check if the component is big enough
                if(pixels.size() >= static_cast<size_t> (minValidSize)){
                    //create a new ConnectedComponent and add it to the component list
                    components.push_back(std::make_shared<ConnectedComponent>(componentID, pixels));
                    componentID++;
                }
            }
        }
    }
    return components.size();
}

/**
 * Filters the current list of components by their size.
 * Keeps only those whose size is between [minSize, maxSize].
 * 
 * @param minSize Minimum number of pixels a component should have.
 * @param maxSize Maximum number of pixels a component should have.
 * @return Number of components that are filtered.
 */
int PGMimageProcessor::filterComponentsBySize(int minSize, int maxSize){
    typedef std::vector<std::shared_ptr<ConnectedComponent>> compVecPtr;
    compVecPtr filtered;
    compVecPtr::const_iterator i = components.begin();

    //checks if the components size is between [minSize, maxSize]
    while(i != components.end()){
        int components_size = (*i) -> getSize();
        if((components_size >= minSize) && (components_size <= maxSize)){
            filtered.push_back(*i);
        }
        ++i;
    }

    if (filtered.empty()) {
        std::cerr << "No components matched the size criteria!" << std::endl;
    }

    components = std::move(filtered);
    return components.size();
}

// Explicitly instantiate the templates that will be used:
// Specialised template for PGM(gray scale) files
template bool PGMimageProcessor::writeComponents<bool> (const std::string&);
// Specialised template for PPM(colour) files - with red boarder
template bool PGMimageProcessor::writeComponents<bool, true> (const std::string&);
// Specialised template for PPM(colour) files - without red boarder
template bool PGMimageProcessor::writeComponents<bool, false> (const std::string&);

//Specialised template to read in PPM(colour) files
template bool PGMimageProcessor::readPGM<true>(const std::string &fileName);
//Specialised template to read in PGM(gray scale) files
template bool PGMimageProcessor::readPGM<false>(const std::string &fileName);

//utility methods
/**
 * Gets the total number of connected components found in the image.
 *
 * @return The number of connected components.
 */
int PGMimageProcessor::getComponentCount(void) const{
    return components.size();
}

/**
 * Gets the size (in pixels) of the largest connected component.
 *
 * @return The number of pixels in the largest component. Returns 0 if there are no components.
 */
int PGMimageProcessor::getLargestSize(void) const{
    int maxSize = 0;
    for(size_t i = 0; i<components.size(); ++i){
        if(components[i] -> getSize() > maxSize){
            maxSize = components[i] -> getSize();
        }
    }
    return maxSize;
}

/**
 * Gets the size (in pixels) of the smallest connected component.
 *
 * @return The number of pixels in the smallest component. Returns 0 if there are no components.
 */
int PGMimageProcessor::getSmallestSize(void) const{
    if (components.empty()) {
        return 0;
    }

    int minSize = components[0] -> getSize();
    for(size_t i = 0; i<components.size(); ++i){
        if(components[i] -> getSize() < minSize){
            minSize = components[i] -> getSize();
        }
    }
    return minSize;
}

/**
 * Checks if a given file name corresponds to a PPM file (by extension).
 *
 * @param fileName The name of the file to check.
 * @return true if the file name ends with ".ppm", false otherwise.
 */
bool PGMimageProcessor::isPPMFile(const std::string & fileName){
    return fileName.substr(fileName.size() - 4) == ".ppm";
}

/**
 * Gets the width of the loaded PGM image.
 *
 * @return The width of the image in pixels.
 */
int PGMimageProcessor::getWidth() const{
    return width;
}

/**
 * Gets the height of the loaded PGM image.
 *
 * @return The height of the image in pixels.
 */
int PGMimageProcessor::getHeight() const{
    return height;
}

/**
 * Prints the data of a specific connected component.
 *
 * This delegates the printing task to the `printData()` method of the given component.
 *
 * @param theComponent The connected component to display.
 */
void PGMimageProcessor::printComponentData(const ConnectedComponent & theComponent) const{
    theComponent.printData();
}

/**
 * Prints the colourIntensity of each component
 */
void PGMimageProcessor::printComponentPixelValues() const{
    for (const auto& component : components) {
        std::cout << "Component ID: " << component->getID() << "\n";
    
        bool printed = false;
        unsigned char visitedValue = 0;

        for (const std::pair<int, int> & pixel : component->getPixels()) {
            int x = pixel.first;
            int y = pixel.second;
            int index = y * width + x;
            unsigned char value = imageData[index];

            if (value != visitedValue) {
                visitedValue = value;
                std::cout << "  Pixel value = " << static_cast<int>(value) << "\n";
            }
        }

        std::cout << "-----------------------------\n";
    }
    
}


/**
 * Gets a list of all the connected components found in the image.
 *
 * @return A vector containing shared pointers to all connected components.
 */
std::vector< std::shared_ptr<ConnectedComponent> > PGMimageProcessor::getComponents(){
    return components;
}