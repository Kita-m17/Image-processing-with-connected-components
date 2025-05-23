#include "PGMimageProcessor.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

/**
 * Unit tests for ConnectedComponent class.
 *
 * @author Nikita Martin
 * MRTNIK003
 * @version 1
 * @date April 2025
 */
TEST_CASE("ConnectedComponents class Unit Test"){
    std::cout << "Testing the ConnectedComponent class" << std::endl;
    
    /**
     * Tests default constructor.
     * ConnectedComponent should initialize with ID = 0 and size = 0.
     */
    SECTION("Default Constructor"){
        ConnectedComponent c;

        REQUIRE(c.getID() == 0);
        REQUIRE(c.getSize() == 0);
    }

    /**
     * Tests constructor with ID.
     */
    SECTION("Custom Constructor 1"){
        ConnectedComponent c(1);
        REQUIRE(c.getID() == 1);
        REQUIRE(c.getSize() == 0);
    }

    /**
     * Tests copy constructor.
     */
    SECTION("Copy Constructor"){
        ConnectedComponent c1(1);
        ConnectedComponent c2(c1);
        const std::pair<int, int> * vec1Ptr = c1.getPixels().data();
        REQUIRE(c1.getID() == c2.getID());
        REQUIRE(c1.getSize() == c2.getSize());
    }

    /**
     * Tests move constructor.
     */
    SECTION("Move Constructor"){
        ConnectedComponent c1(1);
        ConnectedComponent c2(std::move(c1));
        const std::pair<int, int> * vec1Ptr = c1.getPixels().data();

        REQUIRE(c2.getID() == 1);
        REQUIRE(c2.getSize() == 0);
        REQUIRE(c1.getID() == 0);
        REQUIRE(c1.getSize() == 0);

        REQUIRE(vec1Ptr == nullptr);
    }

    /**
     * Tests copy assignment operator.
     */
    SECTION("Copy Assignment"){
        ConnectedComponent c1(1);
        ConnectedComponent c2(2);
        c2 = c1;
        const std::pair<int, int> * vec1Ptr = c1.getPixels().data();
        REQUIRE(c1.getID() == c2.getID());
        REQUIRE(c1.getSize() == c2.getSize());
    }

    /**
     * Tests move assignment operator.
     */
    SECTION("Move Assignment"){
        ConnectedComponent c1(1);

        ConnectedComponent c2(2);
        c2 = std::move(c1);
        
        const std::pair<int, int> * vec1Ptr = c1.getPixels().data();
        REQUIRE(c2.getID() == 1);
        REQUIRE(c2.getSize() == 0);
        REQUIRE(c1.getID() == 0);
        REQUIRE(c1.getSize() == 0);
        REQUIRE(vec1Ptr == nullptr);
    }
}

/**
 * Unit tests for PGMimageProcessor class.
 */
TEST_CASE("PGMimageProcessor class TEST"){
    
    /**
     * Tests default constructor.
     */
    SECTION("Default Constructor"){
        std::cout << "Testing the PGMimageProcessor class: Default Constructor" << std::endl;
        PGMimageProcessor p;
        REQUIRE(p.getWidth() == 0);
        REQUIRE(p.getHeight() == 0);
        REQUIRE(p.getComponentCount() == 0);
        REQUIRE(p.getLargestSize() == 0);
        REQUIRE(p.getSmallestSize() == 0);
    }

    /**
     * Tests custom constructor with file input.
     */
    SECTION("Custom Constructor"){
        std::cout << "Testing the PGMimageProcessor class: Custom Constructor" << std::endl;
        PGMimageProcessor p("input/Shapes.pgm");
        REQUIRE(p.getWidth() > 0);
        REQUIRE(p.getHeight() > 0);
        REQUIRE(p.getComponentCount() == 0);
    }

    PGMimageProcessor p1("input/Birds_Colours.pgm");

    /**
     * Tests copy constructor.
     */
    SECTION("Copy Constructor"){
        std::cout << "Testing the PGMimageProcessor class: Copy Constructor" << std::endl;
        p1.extractComponents(171, 2);  // Extract components to validate copying

        PGMimageProcessor p2(p1);
        REQUIRE(p1.getWidth() == p2.getWidth());
        REQUIRE(p1.getHeight() == p2.getHeight());
        REQUIRE(p1.getComponentCount() == p2.getComponentCount());
    }

    /**
     * Tests move constructor.
     */
    SECTION("Move Constructor"){
        std::cout << "Testing the PGMimageProcessor class: Move Constructor" << std::endl;
        p1.extractComponents(171, 2);

        PGMimageProcessor p2(std::move(p1));

        REQUIRE(p2.getWidth() > 0);
        REQUIRE(p2.getHeight() > 0);
        REQUIRE(p2.getComponentCount() > 0);

        //p1 should now be in an empty/moved state
        REQUIRE(p1.getWidth() == 0);
        REQUIRE(p1.getHeight() == 0);
        REQUIRE(p1.getComponentCount() == 0);
    }

    /**
     * Tests copy assignment operator.
     */
    SECTION("Copy Assignment"){
        std::cout << "Testing the PGMimageProcessor class: Copy Assignment" << std::endl;
        PGMimageProcessor p2("input/Shapes_Colours.pgm");
        
        p1.extractComponents(128, 1); 
        p2 = p1; //copy assignment

        REQUIRE(p1.getWidth() == p2.getWidth());
        REQUIRE(p1.getHeight() == p2.getHeight());
        REQUIRE(p1.getComponentCount() == p2.getComponentCount());
    }

    /**
     * Tests move assignment operator.
     */
    SECTION("Move Assignment"){
        std::cout << "Testing the PGMimageProcessor class: Move Assignment" << std::endl;
        PGMimageProcessor p2("input/Shapes_Colours.pgm");
        
        p1.extractComponents(128, 1);
        p2 = std::move(p1); // Move assignment

        REQUIRE(p2.getWidth() > 0);
        REQUIRE(p2.getHeight() > 0);
        REQUIRE(p2.getComponentCount() > 0);

        //p1 should now be in an empty/moved state
        REQUIRE(p1.getWidth() == 0);
        REQUIRE(p1.getHeight() == 0);
        REQUIRE(p1.getComponentCount() == 0);
    }

    PGMimageProcessor imageProcessor;
    bool readPGMFile = imageProcessor.readPGM<false>("input/Birds-1.pgm");
    PGMimageProcessor ppmImageProcessor;
    bool readPPMFile = ppmImageProcessor.readPGM<true>("input/Chess_Colours.ppm");
    
    /**
     * Tests reading a PGM file.
     */
    SECTION("PGMimageProcessor Read", "[readPGM]"){
        std::cout << "Testing the PGMimageProcessor class: Reading in the pgm file - readPMG" << std::endl;
        REQUIRE(readPGMFile == true);
        REQUIRE(imageProcessor.getWidth() > 0);
        REQUIRE(imageProcessor.getHeight() > 0);

        std::cout << "Testing the PGMimageProcessor class: Reading in the ppm file - readPMG" << std::endl;
        REQUIRE(readPPMFile == true);
    }
    

    /**
     * Tests component extraction based on threshold and connectivity.
     */
    SECTION("Extract Components", "[extractComponents]"){
        std::cout << "Testing the PGMimageProcessor class: Extracting the components - extractComponents" << std::endl;
        int numComponents = imageProcessor.extractComponents(35, 2);
        REQUIRE(numComponents > 0);
        REQUIRE(numComponents == imageProcessor.getComponentCount());
    }

    /**
     * Tests filtering components by minimum size.
     */
    SECTION("Filter Components", "[filterComponentsBySize]"){
        std::cout << "Testing the PGMimageProcessor class: Filter the components by size - filterComponentsBySize" << std::endl;
        int numComponents = imageProcessor.extractComponents(35, 2);
        int filteredComponents = imageProcessor.filterComponentsBySize(1000, std::numeric_limits<int>::max());
        REQUIRE(filteredComponents <= numComponents);
    }

    /**
     * Tests writing filtered components to a PGM file.
     */
    SECTION("Write Components - PGM", "[writeComponents]"){
        std::cout << "Testing the PGMimageProcessor class: Writing to a pgm file - writeComponents" << std::endl;
        imageProcessor.extractComponents(35, 2);
        bool writeSuccess = imageProcessor.writeComponents<int>("output/test_pgm_output");
        
        REQUIRE(writeSuccess == true);
    }
    
    /**
     * Tests writing components to a PPM file without border.
     */
    SECTION("Write Components - PPM without border", "[writeComponents]"){
        std::cout << "Testing the PGMimageProcessor class: Writing to a ppm file with no boarder- writeComponents" << std::endl;
        imageProcessor.extractComponents(35, 2);

        bool writeSuccess = ppmImageProcessor.writeComponents<int, false>("output/test_ppm_output_no_boarder");
        
        REQUIRE(writeSuccess == true);
    }

    /**
     * Tests writing components to a PPM file with border.
     */
    SECTION("Write Components - PPM with border", "[writeComponents]"){
        std::cout << "Testing the PGMimageProcessor class: Writing to a ppm file with boarder - writeComponents" << std::endl;
        imageProcessor.extractComponents(35, 2);

        bool writeSuccess = ppmImageProcessor.writeComponents<int, true>("output/test_ppm_output_boarder");
        
        REQUIRE(writeSuccess == true);
    }

    SECTION("Component Size Test"){
        imageProcessor.extractComponents(35, 2);
        std::cout << "Testing the PGMimageProcessor class: Getting the largest Component size - getLargestSize" <<std::endl;
        REQUIRE(imageProcessor.getLargestSize() == 7671);

        std::cout << "Testing the PGMimageProcessor class: Getting the smallest Component size - getSmallestSize" <<std::endl;
        REQUIRE(imageProcessor.getSmallestSize() == 4007);

        std::cout << "Testing the PGMimageProcessor class: Getting the number of components extracted" <<std::endl;
        REQUIRE(imageProcessor.getComponentCount() == 8);
    }

}