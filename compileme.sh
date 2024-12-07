gcc TCPEchoClient.c DieWithError.c -o TCPEchoClient

gcc TCPEchoServer.c DieWithError.c HandleTCPClient.c -o TCPEchoServer

gcc TCPEchoServer-Fork.c DieWithError.c HandleTCPClient.c AcceptTCPConnection.c CreateTCPServerSocket.c -o TCPEchoServer-Fork

gcc TCPEchoServer-Fork-SIGCHLD.c DieWithError.c HandleTCPClient.c AcceptTCPConnection.c CreateTCPServerSocket.c -o TCPEchoServer-Fork-SIGCHLD

gcc TCPEchoServer-ForkN.c DieWithError.c HandleTCPClient.c AcceptTCPConnection.c CreateTCPServerSocket.c -o TCPEchoServer-ForkN

gcc TCPEchoServer-Select.c DieWithError.c HandleTCPClient.c AcceptTCPConnection.c CreateTCPServerSocket.c -o TCPEchoServer-Select

gcc TCPEchoServer-Thread.c DieWithError.c HandleTCPClient.c AcceptTCPConnection.c CreateTCPServerSocket.c -lpthread -o TCPEchoServer-Thread

gcc BroadcastReceiver.c DieWithError.c -o BroadcastReceiver

gcc BroadcastSender.c DieWithError.c -o BroadcastSender

gcc UDPEchoClient.c DieWithError.c -o UDPEchoClient

gcc UDPEchoServer.c DieWithError.c -o UDPEchoServer
