//////////////////
// von Christoph//
//////////////////


#include <iostream>
#include <thread>
#include <string>
#include "LaserScanClient.h"
#include "OdometryClient.h"
#include "GeometryMsgsClient.h"
#include "Server.h"

// Funktion, um den Server in einem Thread zu starten
void runServer(unsigned short port) 
{
        Server server(port); //Server objekt erstellen
        server.start();     //Server starten
}

void runLaserScanClient(const std::string& serverIP, unsigned short Port) 
{   
    //std::string LaserDaten;
    std::string laserData;
    
        LaserScanClient laserClient(serverIP, Port);

        if (laserClient.connectToServer()) {
            laserData = laserClient.readLaserData();
            std::cout << "Erhaltene Laserdaten: " << laserData << std::endl;
        }
        std::cout << "Erhaltene Laserdaten: " << laserData << std::endl;
    
    //return;
}

void runOdometryClient(const std::string& serverIP, unsigned short Port) 
{
    std::string OdomDaten;
    OdometryClient odomclient(serverIP, Port); //erstellen eines OdometryClient objelcts
    if (odomclient.connectToServer()) //verbinden zum server + abfrage ob dies erfolgreich war
    {
       OdomDaten = odomclient.readOdometryData(); //Lesen der Odometry daten + rückgabe in die variable OdomDaten als std::string
       
    }
    std::cout << "Odometry Daten: " << OdomDaten << std::endl; //Ausgabe der Odometry daten in das Ternimal (Testzwecke)
    std::cout << "Testodom" << std::endl;
    return;
}

void runGeometryClient(const std::string& serverIP, unsigned short Port) 
{
    GeometryMsgsClient geometryclient(serverIP, Port); //erstellen eines GeometryMsgsClient objelcts
    if (geometryclient.connectToServer()) //verbinden zum server + abfrage ob dies erfolgreich war
    {
        geometryclient.sendGeometryMsg(0.1,0.5); //Senden der Geometry message mit den angegebenen werten
    }

    //Sleep(5000);

    //geometryclient.sendGeometryMsg(0.0,0.0); //Reset der gesendteten daten
    std::cout << "TestGeom" << std::endl;
    return;
}

/////////////////////////////////////////
// HIER BEGINNT DIE MAIN  von Christoph//
/////////////////////////////////////////

int main() {
    std::string serverIP = "192.168.100.54";  // Server-IP

    unsigned short port1 = 9997;  // Port für LaserScan
    //unsigned short port2 = 9998;  // Port für Odometry

    // Server in einem separaten Thread starten
    std::thread serverThread1(runServer, port1);  // Server auf Port 9997 starten
    //std::thread serverThread2(runServer, port2);  // Server auf Port 9998 starten
 

    // Warte, damit der Server starten kann
    std::this_thread::sleep_for(std::chrono::seconds(3));  // Warte 1 Sekunde

    // Clients in separaten Threads starten
    std::thread laserScanClientThread(runLaserScanClient, serverIP, port1);
    //std::thread odometryClientThread(runOdometryClient, serverIP, port2);
    //std::thread geometryMsgsClientThread(runGeometryClient, serverIP, 9999);  // Ein fester Port für Geometry-Client

    // Warten, dass alle Client-Threads beendet sind
    laserScanClientThread.join();
    //odometryClientThread.join();
    //geometryMsgsClientThread.join();

    // Warten, dass der Server-Thread beendet ist (in diesem Fall wird der Server unendlich laufen)
    serverThread1.join();
    //serverThread2.join();
 

    return 0;
}


/* Test Code Zeilen

std::string LaserDaten;
    std::string OdomDaten;

    LaserScanClient Laserclient("192.168.100.54", 9997); //erstellen eines LaserScanClient objelcts
    if (Laserclient.connectToServer()) //verbinden zum server + abfrage ob dies erfolgreich war
    {
        LaserDaten = Laserclient.readLaserData(); //Lesen der LaserScan daten + rückgabe in die variable LaserDaten als std::string
    }
    
    OdometryClient odomclient("192.168.100.54", 9998); //erstellen eines OdometryClient objelcts
    if (odomclient.connectToServer()) //verbinden zum server + abfrage ob dies erfolgreich war
    {
        OdomDaten = odomclient.readOdometryData(); //Lesen der Odometry daten + rückgabe in die variable OdomDaten als std::string
    }

    std::cout << "Laser Daten: " << LaserDaten << std::endl; //Ausgabe der Laser daten in das Ternimal (Testzwecke)
    std::cout << "Odometry Daten: " << OdomDaten << std::endl; //Ausgabe der Odometry daten in das Ternimal (Testzwecke)

    GeometryMsgsClient geometryclient("192.168.100.54", 9999); //erstellen eines GeometryMsgsClient objelcts
    if (geometryclient.connectToServer()) //verbinden zum server + abfrage ob dies erfolgreich war
    {
        geometryclient.sendGeometryMsg(0.0,0.5); //Senden der Geometry message mit den angegebenen werten
    }

    */