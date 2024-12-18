# Compiler und Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -pthread
LDFLAGS = -lws2_32 -lpthread  # Für Windows: Verlinke mit ws2_32.lib

# Zielprogramm
EXEC = robot_control

# Quell- und Objektdateien
SRC = Main.cpp LaserScanClient.cpp OdometryClient.cpp SharedMemory.cpp
OBJ = $(SRC:.cpp=.o)

# Standardziel
all: $(EXEC)

# Erstellen des Zielprogramms
$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Regel zur Kompilierung der .cpp-Dateien zu .o-Dateien
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Aufräumen
clean:
	rm -f $(OBJ) $(EXEC)
