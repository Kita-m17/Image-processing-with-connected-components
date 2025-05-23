#include "ConnectedComponent.h"
#include <limits>

/**
 * Parameterized constructor with a given ID
 * Initializes bounding box values to extremes so they can be updated correctly later.
 *
 * @author Nikita Martin
 * MRTNIK003
 * @version 1
 * @date April 2025
 */
ConnectedComponent::ConnectedComponent(int Id): 
    id(Id),
    numPixels(0),
    x_min(std::numeric_limits<int>::max()),
    y_min(std::numeric_limits<int>::max()),
    x_max(std::numeric_limits<int>::min()),
    y_max(std::numeric_limits<int>::min()),
    pixels()
{}

/**
 * Parameterized Constructor
 * initialized with a list of pixels.
 * Sets numPixels, and computes bounding box by iterating over each pixel.
 */
ConnectedComponent::ConnectedComponent(int id, std::vector<std::pair<int, int>> pixels) 
        : id(id), 
        pixels(std::move(pixels)),
        numPixels(pixels.size()),
        x_min(std::numeric_limits<int>::max()),
        y_min(std::numeric_limits<int>::max()),
        x_max(std::numeric_limits<int>::min()),
        y_max(std::numeric_limits<int>::min())
        {
            for (const std::pair<int, int> & pixel : this->pixels) {
                updateBounding(pixel.first, pixel.second); //update bounding for each pixel
            }
        }

/**
 * Copy constructor
 * Deep copies all values from another ConnectedComponent.
 */
ConnectedComponent::ConnectedComponent(const ConnectedComponent & component):
    id(component.id),
    numPixels(component.numPixels),
    x_min(component.x_min),
    y_min(component.y_min),
    x_max(component.x_max),
    y_max(component.y_max),
    pixels(component.pixels)
{}

/**
 * Move constructor - moves values from a temporary object to this object
 * Clears the original object's values to leave it in a valid state.
 */
ConnectedComponent::ConnectedComponent(ConnectedComponent && component):
    id(component.id),
    numPixels(component.numPixels),
    x_min(component.x_min),
    y_min(component.y_min),
    x_max(component.x_max),
    y_max(component.y_max),
    pixels(std::move(component.pixels))
    {
        component.id = 0;
        component.numPixels = 0;
        component.x_min = 0;
        component.y_min = 0;
        component.x_max = 0;
        component.y_max = 0;

        component.pixels.clear(); //explicitly clear the vector
    }

/**
 * Copy Assignment Operator
 * assigns one component to another via deep copy.
 */
ConnectedComponent & ConnectedComponent::operator=(const ConnectedComponent & component){
    if(this!= &component) { //avoids self assignment
        id = component.id;
        numPixels = component.numPixels;
        x_min = component.x_min;
        y_min = component.y_min;
        x_max = component.x_max;
        y_max = component.y_max;
        pixels = component.pixels;
    }
    return *this;
}

/**
 * Move Assignment Operator
 * transfers ownership from a temporary component.
 */
ConnectedComponent & ConnectedComponent::operator=(ConnectedComponent && component){
    if(this!= &component) { 
        id = component.id;
        numPixels = component.numPixels;
        x_min = component.x_min;
        y_min = component.y_min;
        x_max = component.x_max;
        y_max = component.y_max;
        pixels = std::move(component.pixels); //move pixel data

        component.id = 0;
        component.numPixels = 0;
        component.x_min = 0;
        component.y_min = 0;
        component.x_max = 0;
        component.y_max = 0;

        component.pixels.clear();
    }
    return *this;
}

/**
 * Method that updates the bounding box component based on a new pixel's (x, y) values
 */
void ConnectedComponent::updateBounding(int x, int y){
    x_min = std::min(x_min, x);
    y_min = std::min(y_min, y);
    x_max = std::max(x_max, x);
    y_max = std::max(y_max, y);
}

/**
 * Method that adds a pixel to the component, increments the pixel count, and updates bounding box.
 */
void ConnectedComponent::addPixel(int x, int y){
    pixels.push_back(std::make_pair(x, y));
    numPixels++;
    updateBounding(x, y);
}

/**
 * @return the number of pixels
 */
int ConnectedComponent::getSize() const{
    return pixels.size();
}

/**
 * @return the id of the connected component
 */
int ConnectedComponent::getID() const{
    return id;
}

/**
 * Getters for the bounding box coordinates 
 */
int ConnectedComponent::getXMin() const{
    return x_min;
}

int ConnectedComponent::getXMax() const{
    return x_max;
} 

int ConnectedComponent::getYMin() const{
    return y_min;
}

int ConnectedComponent::getYMax() const{
    return y_max;
}

/**
 * @return the bounding box (tuple of - (xmin, ymin, xmax, ymax))
 */
std::tuple<int, int, int, int> ConnectedComponent::getBoundingBox() const { 
    return {x_min, y_min, x_max, y_max}; 
}

/**
 * @return the list of pixels in the component
 */
const std::vector< std::pair<int, int> > & ConnectedComponent::getPixels() const{
    return pixels;
}

/**
 * print component data - component's ID and number of pixels.
 */
void ConnectedComponent::printData() const {
    std::cout << "Component ID: " << id << ", Size: " << numPixels << " pixels." << std::endl;
}