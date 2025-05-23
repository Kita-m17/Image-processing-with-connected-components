Image Format Conversion Guide
- To convert a colour PNG to greyscale PGM image - First convert from PNG to PPM to PMG:
pngtopnm input.png | ppmtopgm > output.pgm

- To convert from PNG file to PPM file:
pngtopnm input.png > output.ppm

- If your PNG image is already grayscale and you want to convert it directly to PGM:
pngtopnm input.png > output.pgm

- To convert a PPM file back to PNG:
pnmtopng input.ppm > output.png

- To convert from PGM to PNG:
pnmtopng input.pgm > output.png

Running the Program
- Before compiling the code, make sure that you have converted the png files to the appropriate .pgm or .png pile using the Image Format Conversion Guide above.
- Once you've compiled the project, run the program from the command line using:
./findcomp [options] <inputPGMfile>

Options:
-t <int>: Sets the threshold for component detecteion (default = 128)

-m <int>: Sets the minimum size for valid components (default = 1)

-f <min> <max>: Filters components between a minimum and maximum size

-p: Prints details about each component

-w <filename>: Write retained components to a new PGM file (only the file name)

-b <ppm_filename>: Write a PPM file with bounding boxes drawn around each retained component (only the file name)

Example:
./findcomp -t 100 -m 50 -p -w outputFileName input.pgm

- After executing the program, and creating the output .pgm or .ppm files, you can convert the output file to .png using the Image Format Conversion Guide.

Running the Unit Tests (UnitTests.cpp):
- Before compiling UnitTests.cpp, make sure there's an input folder and a output folder in the current directory containing the following files:
- Shapes_Colours.pgm
- Chess_Colours.pgm
- Chess_Colours.ppm
- These were used as test files during unit testing.

- To execute the Unit tests after compiling, run ./tester.

!Disclaimer: This may take a little bit of time to run due to catch.hpp being used for test management.
