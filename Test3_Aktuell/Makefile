# Compiler und Flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -pthread  # -pthread für Multithreading-Unterstützung
LDFLAGS = -lws2_32                   # Winsock-Bibliothek für Windows (falls erforderlich)

# Quellcode-Dateien
MAIN_SRC = main.cpp
SERVER_SRC = TCPEchoServer.cpp
CLIENT_SRC = TCPEchoClient.cpp
SEND_CLIENT_SRC = TCPEchoClientSend.cpp
LASER_SRC = laser_data_analasys.cpp
ODOM_SRC = odom_data_analasys.cpp

# Objektdateien
MAIN_OBJ = $(MAIN_SRC:.cpp=.o)
SERVER_OBJ = $(SERVER_SRC:.cpp=.o)
CLIENT_OBJ = $(CLIENT_SRC:.cpp=.o)
SEND_CLIENT_OBJ = $(SEND_CLIENT_SRC:.cpp=.o)
LASER_OBJ = $(LASER_SRC:.cpp=.o)
ODOM_OBJ = $(ODOM_SRC:.cpp=.o)

# Ziel-Dateien
MAIN_TARGET = myprogram.exe
SERVER_TARGET = server.exe
CLIENT_TARGET = client.exe
SEND_CLIENT_TARGET = send.exe
LASER_TARGET = laser_analysis.exe
ODOM_TARGET = odom_analysis.exe

# Standardziel: Alle ausführbaren Dateien erstellen
all: $(MAIN_TARGET) $(SERVER_TARGET) $(CLIENT_TARGET) $(SEND_CLIENT_TARGET) $(LASER_TARGET) $(ODOM_TARGET)

# Erstellen der Hauptprogramm-Ausführbaren Datei
$(MAIN_TARGET): $(MAIN_OBJ)
	$(CXX) $(MAIN_OBJ) $(LDFLAGS) -o $@

# Erstellen der Server-Ausführbaren Datei
$(SERVER_TARGET): $(SERVER_OBJ)
	$(CXX) $(SERVER_OBJ) $(LDFLAGS) -o $@

# Erstellen der Client-Ausführbaren Datei
$(CLIENT_TARGET): $(CLIENT_OBJ)
	$(CXX) $(CLIENT_OBJ) $(LDFLAGS) -o $@

# Erstellen der zusätzlichen Client-Ausführbaren Datei (send.exe)
$(SEND_CLIENT_TARGET): $(SEND_CLIENT_OBJ)
	$(CXX) $(SEND_CLIENT_OBJ) $(LDFLAGS) -o $@

# Erstellen der Laser-Analyse-Ausführbaren Datei
$(LASER_TARGET): $(LASER_OBJ)
	$(CXX) $(LASER_OBJ) $(LDFLAGS) -o $@

# Erstellen der Odometry-Analyse-Ausführbaren Datei
$(ODOM_TARGET): $(ODOM_OBJ)
	$(CXX) $(ODOM_OBJ) $(LDFLAGS) -o $@

# Kompilieren der Quellcode-Dateien
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Bereinigung (für Windows)
clean:
	del /f /q *.o $(MAIN_TARGET) $(SERVER_TARGET) $(CLIENT_TARGET) $(SEND_CLIENT_TARGET) $(LASER_TARGET) $(ODOM_TARGET)

# Standardziel für Make
.PHONY: all clean
