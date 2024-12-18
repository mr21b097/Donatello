# Compiler und Flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -pthread # -pthread für Multithreading-Unterstützung
LDFLAGS = -lws2_32 # Hinzufügen der Winsock-Bibliothek für Windows (falls erforderlich)

# Quellcode-Dateien
SRC = main.cpp LaserScanClient.cpp OdometryClient.cpp GeometryMsgsClient.cpp Server.cpp
OBJ = $(SRC:.cpp=.o)

# Ziel-Datei
TARGET = my_program

# Standardeingabe: Ziel für das Erstellen der ausführbaren Datei
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o $(TARGET)

# Kompilieren der Quellcode-Dateien
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Bereinigung (für Windows)
clean:
	del /f /q $(OBJ) $(TARGET)

# Standardeingabe für Make
.PHONY: clean
