CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -o

TARGET   = flight_system
SOURCES  = main.cpp utils.cpp flight.cpp passenger.cpp booking.cpp fare.cpp notifications.cpp

all:
	$(CXX) $(CXXFLAGS) $(TARGET) $(SOURCES)
	@echo "Build successful! Run with: ./$(TARGET)"

clean:
	rm -f $(TARGET)
