#ifndef _CONNECTEDCOMPONENT_H
#define _CONNECTEDCOMPONENT_H
#include <vector>
#include <array>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <limits>
#include <queue>

/**
 * Represents a Connected Component in a binary image.
 *
 * A Connected Component is a group of pixels that are connected and share the same value (white(255) or black(0) in a binary image).
 */
class ConnectedComponent{
    private:
        int id; //unique identifier for the component
        int numPixels; //no. of pixels in the component
        std::vector< std::pair<int, int> > pixels; //list of pixels of the component
        int x_min, y_min, x_max, y_max; //these represent the bounding box coordinates of the Connected Component
    
    public:
        //Constructors and Destructor - Big 6
        ConnectedComponent(int id); //Parameteized Constructor which initializes an empty component with a given ID
        ConnectedComponent(int id, std::vector<std::pair<int, int>> pixels); //Parameterized Constructor which initializes a component with a given ID and a list of pixels

        //Big 6
        //Default constructor - initialises an empty component with default values
        ConnectedComponent() = default;

        //Destructor - clears the component data and allocated resources 
        ~ConnectedComponent() = default;

        //Copy constructor
        ConnectedComponent(const ConnectedComponent & component);
        
        //Move constructor
        ConnectedComponent(ConnectedComponent && component);
        
        //Copy assignment operator
        ConnectedComponent & operator=(const ConnectedComponent & component);
        
        //Move assignment operator
        ConnectedComponent & operator=(ConnectedComponent && component);


        //Adds a pixel to a component and updates the bounding box
        void addPixel(int x, int y);

        //Returns the number of pixels in the component
        int getSize() const;

        //Returns the unique ID of the component
        int getID() const;

        //Bounding box getter methods
        int getXMin() const;
        int getYMin() const;
        int getXMax() const;
        int getYMax() const;

        //Returns the bounding box as a tuple: (x_min, y_min, x_max, y_max)
        std::tuple<int, int, int, int> getBoundingBox() const;
        
        //Returns a constant reference to the vector of pixels
        const std::vector< std::pair<int, int> > & getPixels() const;
        
        //Updates the bounding box coordinates of the new pixel
        void updateBounding(int x, int y);

        //Prints the component's ID and size
        void printData() const;
};

#endif