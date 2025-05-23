driver: driver.o ConnectedComponent.o PGMimageProcessor.o
	g++ driver.o ConnectedComponent.o PGMimageProcessor.o -o findcomp -std=c++20

tester: UnitTests.o PGMimageProcessor.o ConnectedComponent.o
	g++ UnitTests.o PGMimageProcessor.o ConnectedComponent.o -o tester -std=c++20

UnitTests.o: UnitTests.cpp
	g++ -c UnitTests.cpp -o UnitTests.o -std=c++20

driver.o: driver.cpp
	g++ -c driver.cpp -o driver.o -std=c++20

ConnectedComponent.o: ConnectedComponent.cpp
	g++ -c ConnectedComponent.cpp -o ConnectedComponent.o -std=c++20

PGMimageProcessor.o: PGMimageProcessor.cpp
	g++ -c PGMimageProcessor.cpp -o PGMimageProcessor.o -std=c++20

run: findcomp
	./findcomp

clean:
	rm *.o findcomp
