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

robot_control: Main.o LaserScanClient.o OdometryClient.o VelocityClient.o SharedMemory.o
	g++ -std=c++17 -Wall -g -pthread Main.o LaserScanClient.o OdometryClient.o VelocityClient.o SharedMemory.o -o robot_control



# Standardeingabe: Ziel für das Erstellen der ausführbaren Datei
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o $(TARGET)


# Regel zur Kompilierung der .cpp-Dateien zu .o-Dateien
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Bereinigung (für Windows)
clean:
	rm -f *.o robot_control

