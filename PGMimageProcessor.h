#ifndef _PGMIMAGEPROCESSOR_H
#define _PGMIMAGEPROCESSOR_H
#include "ConnectedComponent.h"

/**
 * PGMimageProcessor class
 *
 * Handles reading, writing, and processing of PGM images.
 * It supports extracting connected components, filtering them by size,
 * and accessing metadata such as image dimensions and component statistics.
 *
 * @author Nikita Martin
 * MRTNIK003
 * @version 1
 */
class PGMimageProcessor{
    protected:
        int width, height; //dimensions of the image
        int maxVal;
        std::vector<unsigned char> imageData;
        std::vector< std::shared_ptr<ConnectedComponent> > components; //list of extracted connected components
        std::string fileName;

    public:
        //Constructors and Destructir (Big 6)

        /**
         * Parameterized constructor - creates a PGMimageProcessor opject with a filename, and reads in the file
         */
        PGMimageProcessor(const std::string &inputImageName);
        
        /**
         * Default constructor
         */
        PGMimageProcessor();

        /**
         * Destructor - cleans up allocated resources
         */
        ~PGMimageProcessor();

        /**
         * Copy constructor
         */
        PGMimageProcessor(const PGMimageProcessor & processor);
        
        /**
         * Move constructor
         */
        PGMimageProcessor(PGMimageProcessor && processor);
        
        /**
         * Copy Assignment Operator
         */
        PGMimageProcessor & operator=(const PGMimageProcessor & processor);
        
        /**
         * Move Assignment Operator
         */
        PGMimageProcessor & operator=(PGMimageProcessor && processor);
        
        //Core method
        /**
         * Extracts all connected components from a binary image based on the threshold
         */
        int extractComponents(unsigned char threshold, int minValidSize);

        /**
         * Filters components based on the sized constraints
         */
        int filterComponentsBySize(int minSize, int maxSize);

        /**
         * Specialised template for PGM(gray scale) files
         * Writes components to a PGM image.
         * Each component's pixels are colored white (255,255,255).
         *
         * @tparam T Unused (kept for template specialization purposes).
         * @param outputFileName File name to save the PPM image.
         * @return True if write is successful, false otherwise.
         */
        template <typename T> bool writeComponents(const std::string & outputFileName){
            std::string outputFile = outputFileName +".pgm";
            std::ofstream out(outputFile, std::ios::binary);
            if(!out){
                std::cerr << "Error: Unable to write file " << outputFile << "\n";
                return false;
            }
        
            //Initialise output image data
            std::vector<unsigned char> outputImageData(width*height, 0);
        
            //PGM header
            out << "P5" << std::endl << width << " " << height << std::endl << 255 << std::endl;
        
            //process components and sets their pixels to white(255)
            for(size_t i = 0; i<components.size(); ++i){
                std::vector< std::pair<int, int> > pixels = components[i] ->getPixels();
                for(size_t j = 0; j< pixels.size(); ++j){
                    int x = pixels[j].first;
                    int y = pixels[j].second;
                    
                    //outputImageData[y*width+x] = 255;
                    if(x >= 0 && x < width && y >= 0 && y < height) {
                        outputImageData[y*width+x] = 255;
                    }
                }
            }
        
            //write to the file
            out.write(reinterpret_cast<char *>(outputImageData.data()), outputImageData.size());
            std::cout << "Data successfully written to file" <<std::endl;
            
            if (!out || out.fail())
            {
                std::cerr << "Error writing binary block of PGM.\n";
                return false;
            }
        
            out.close();
            return true;
        }
        
        /**
         * Specialised template for PPM(colour) files with optional bounding boxes
         * Writes components to a PPM (colour) image.
         * Each component's pixels are coloured white (255,255,255).
         * If drawBoundingBoxes is true, then red rectangles are drawn around each component.
         * 
         * @tparam T Unused (kept for template specialization purposes).
         * @tparam drawBoundingBoxes to check if red bounding boxes should be drawn on the components of the output image.
         * @param outputFileName File name to save the PPM image.
         * @return True if write is successful, false otherwise.
         */
        template <typename T, bool drawBoundingBoxes> bool writeComponents(const std::string & outputFileName){
            std::string outputFile = outputFileName + ".ppm";
            std::ofstream out(outputFile, std::ios::binary);
            if(!out){
                std::cerr << "Error: Unable to write file " << outputFile << "\n";
                return false;
            }
        
            //initialise the colour image data
            std::vector<unsigned char> outputImageData;
        
            //PPM header
            out << "P6" << std::endl << width << " " << height << std::endl << 255 << std::endl;
            outputImageData.resize(width*height*3, 0);
        
            //if drawing bpunding boxes, start with original image converted to colour
            if(drawBoundingBoxes){
                //convert original grayscale to colour
                for(int y = 0; y<height; ++y){
                    for(int x = 0; x<width; ++x){
                        int index = y*width+x;
                        unsigned char grayValue = imageData[index];
                        outputImageData[index * 3] = grayValue;     // R
                        outputImageData[index * 3 + 1] = grayValue; // G
                        outputImageData[index * 3 + 2] = grayValue; // B
                    }
                }
            }
            
            //process components and set their pixels to white
            for(size_t i = 0; i<components.size(); ++i){
                std::vector< std::pair<int, int> > pixels = components[i] ->getPixels();
                for(size_t j = 0; j< pixels.size(); ++j){
                    int x = pixels[j].first;
                    int y = pixels[j].second;
        
                    if(x >= 0 && x < width && y >= 0 && y < height){
                        int index = y*width+x;
                        if(!drawBoundingBoxes){
                            //set to white if not drawing bounding boxes
                            outputImageData[index * 3] = 255;
                            outputImageData[index * 3 + 1] = 255;
                            outputImageData[index * 3 + 2] = 255;
                        }
                    }
                }
            }
        
            if(drawBoundingBoxes) {
            //draw bounding boxes if requested and its a PPM image
                for(size_t i = 0; i<components.size(); ++i){
                    std::tuple<int, int, int, int> boundingBox = components[i] -> getBoundingBox();
                    int x_min = std::get<0>(boundingBox);
                    int y_min = std::get<1>(boundingBox);
                    int x_max = std::get<2>(boundingBox);
                    int y_max = std::get<3>(boundingBox);
        
        
                    // Add bounds checking to ensure values are within image dimensions
                    x_min = std::max(0, std::min(x_min, width-1));
                    y_min = std::max(0, std::min(y_min, height-1));
                    x_max = std::max(0, std::min(x_max, width-1));
                    y_max = std::max(0, std::min(y_max, height-1));
        
                    // Also ensure min <= max
                    if (x_min > x_max) std::swap(x_min, x_max);
                    if (y_min > y_max) std::swap(y_min, y_max);
                    
                    std::cout << "Component " << i << " bounding box: " << "Xmin: " << x_min << ", Xmax: " << x_max << ", Ymin: " << y_min << ", Ymax: " << y_max << std::endl;
        
                    //draw horizontal bounding box lines (top and bottom)
                    for(int x = x_min; x <= x_max; ++x){
                        //top horizontal line
                        int topIndex = (y_min * width + x) * 3;
                        int bottomIndex = (y_max * width + x) * 3;
        
                        if (topIndex >= 0 && topIndex < static_cast<int>(outputImageData.size()-2)) {
                            outputImageData[topIndex] = 255;
                            outputImageData[topIndex + 1] = 0;
                            outputImageData[topIndex + 2] = 0;
                        }
        
                        if (bottomIndex >= 0 && bottomIndex < static_cast<int>(outputImageData.size()-2)) {
                            outputImageData[bottomIndex] = 255;
                            outputImageData[bottomIndex + 1] = 0;
                            outputImageData[bottomIndex + 2] = 0;
                        }
                    }
        
                    //draw vertical bounding box lines (left and right)
                    for(int y = y_min +1; y < y_max; ++y){
                        //left vertical line
                        int leftIndex = (y * width + x_min) * 3;
                        int rightIndex = (y * width + x_max) * 3;
                        if (leftIndex >= 0 && leftIndex < static_cast<int>(outputImageData.size()-2)) {
                            outputImageData[leftIndex] = 255;     // Red
                            outputImageData[leftIndex + 1] = 0;   // Green
                            outputImageData[leftIndex + 2] = 0;   // Blue
                        }
        
                        //right vertical line
                        if (rightIndex >= 0 && rightIndex < static_cast<int>(outputImageData.size()-2)) {
                            outputImageData[rightIndex] = 255;     // Red
                            outputImageData[rightIndex + 1] = 0;   // Green
                            outputImageData[rightIndex + 2] = 0;   // Blue
                        }
                    }
                }
            }
        
            out.write(reinterpret_cast<char *>(outputImageData.data()), outputImageData.size());
            std::cout << "Data successfully written to file" <<std::endl;
            
            if (!out)
            {
                std::cerr << "Error writing binary block of PGM.\n";
                return false;
            }
        
            if (!out || out.fail())
            {
                std::cerr << "Error writing binary block of PPM.\n";
                return false;
            }
        
            out.close();
            return true;
        }        
        
        //utility methods
        /**
         * @return the number of components currently saved
         */
        std::vector< std::shared_ptr<ConnectedComponent> > getComponents();
        
        /**
         * @return the number of conponents currently saved
         */
        int getComponentCount(void) const;

        /**
         * @return the size of the largest component
         */
        int getLargestSize(void) const;

        /**
         * @return the size of the smallest component
         */
        int getSmallestSize(void) const;

        /**
         * @return the width of the image
         */
        int getWidth() const;

        /**
         * @return the height of the image
         */
        int getHeight() const;

        /**
         * prints the colourIntensity of each component
         */
        void printComponentPixelValues() const;
        
        /**
         * prints the metadata for a specific connected component
         */
        void printComponentData(const ConnectedComponent & theComponent) const;

        /**
         * Checks is the file being read is a ppm or pmg
         * @return true is the file being read is a ppm file, else false
         */
        bool isPPMFile(const std::string & fileName);

        /**
        * Reads a PGM (Portable Gray Map) image from a file.
        * This method opens a PGM file in "P5" (pgm file) or "P6" (ppm file) format, and then extracts the image's width, height, and max value, and stores the grayscale pixel data in imageData.
        * It ensures the image is valid, handles comments in the header, and checks for errors during parsing or I/O.
        * @param inputImageName The file path to the PGM image to be read.
        * @return true if the file is successfully read and image data is loaded; false if otherwise. 
        */
        template <bool isPPM = false> bool readPGM(const std::string &fileName){
            std::ifstream in(fileName, std::ios::binary);
            if(!in){
                std::cerr << "Failed to open file for read: " << fileName << std::endl;
                return false;
            }
        
            //check if valid file/magic number
            std::string magicNumber;
            in >> magicNumber;

            if(!isPPM && magicNumber != "P5"){
                    std::cerr << "Invalid PGM file: " << fileName << std::endl;
                    return false;
            } else if ( isPPM && magicNumber != "P6"){
                    std::cerr << "Invalid PPM file: " << fileName << std::endl;
                    return false;
            }
        
            //Skip the comments
            char ch;
            while(in>>ch){
                if(ch == '#'){
                    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }else{
                    in.putback(ch);
                    break;
                }
            }
        
            //read width, height and the max value
            in >> width >> height >>maxVal;
            if (width <= 0 || height <= 0) {
                std::cerr<<"Invalid image dimensions: " << width << "x" << height << std::endl;
                return false;
            }
        
            if (maxVal != 255) {
                std::cerr << "Unsupported max value: " << maxVal << std::endl;
                return false;
            }
        
            //consume the newline character after the header
            in.get();

            imageData.resize(width*height);

            if(!isPPM){
                //read a gray scale image
                in.read(reinterpret_cast<char *>(imageData.data()), width * height);
            }else{
                //read colour image (3 bytes per pixel)
                std::vector<std::array<unsigned char, 3>> colourData(width * height);
                in.read(reinterpret_cast<char *>(colourData.data()), width * height * 3);
                
                //ensure full read
                if (!in) {
                    std::cerr << "Error reading PPM image data!" << std::endl;
                    return false;
                }
        
                //convert the ppm to grayscale and store in imageData
                for (size_t i = 0; i< width*height; ++i){
                    //I = 0.299 ∗ R + 0.587 ∗ G + 0.114 ∗ B, where (R, G, B) are the channel intensities for your colour pixel.
                    imageData[i] = static_cast<unsigned char>( 
                        0.299 * colourData[i][0] 
                        + 0.587 * colourData[i][1] 
                        + 0.114 * colourData[i][2]
                    );
                }
            }
            
            //ensure that the data was fully read
            if (!in) {
                std::cerr << "Error reading image data!" << std::endl;
                return false;
            }
        
            return true;
        }
        
};

#endif