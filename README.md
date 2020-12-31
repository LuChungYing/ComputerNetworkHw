# ComputerNetworkHw
電腦網路程式作業(利用UDP實做TCP)

We learn from the course that “encapsulation” is used frequently in networking. In this project, we will emulate TCP sender and receiver in application layer and call UDP protocol to transmit TCP packets. That is, TCP packets will be encapsulated and sent in UDP layer by the source and decapsulated in the receiver. In this project, both directions need to be implemented. 
The TAs will tell you how to test your program. That is, TAs will write clients and/or servers to test your program. Therefore, it becomes very important to follow TAs’ formats so your program can be test. The TCP segment structure is as enclosed and you must follow the segment format and implement at least the following fields: source port, destination port, sequence number, acknowledgment number, and checksum for primitive TCP, TCP Tahoe, TCP Reno, and TCP SACK. The state diagram is as shown in the last page of the project. The server provides four video files for clients to retrieve. A client can request multiple video files within each request. The round trip delay is first set to be 150 ms for each connection. The variable threshold is initially set to 64K bytes. The MSS is 1K bytes. The TCP buffer size is assumed to have a buffer size of 32K bytes. The maximum transmission rate for each TCP connection from the server side to the client side is assumed to be 200 kbps, and the maximum transmission rate for each TCP connection from the client side to the client side is assumed to be 100 kbps. The TAs will provide you with these four video clips which basically are small video files. 


Step 1: First, you would write one server and four clients, and the server and four clients can both transmit packets and ACKs. The server must be able to handle multiple file requests from multiple clients.

Step 2: Channels may have losses. 

Step 3: Delay ACKs are added to senders and receivers.

Step 4: The TCP congestion control consists of the slow start and the congestion avoidance mechanisms. In step 4, you implement the slow start mechanism and the congestion avoidance mechanism.

Step 5: The fast retransmit mechanism is added (Tahoe). 
For the moment, there are still only two states, slow start state, and congestion avoidance state. However, the fast retransmit mechanism is already included in TCP Tahoe. For TCP Tahoe, when a loss 
happens, fast retransmit is triggered and the slow start state is entered.

Step 6: The fast recovery mechanism is added (TCP Reno).

Step 7: The SACK option is incorporated into the standard.

Step 8: Now increase the number of clients to 200. The server side has to implement some prevention mechanisms to handle simultaneous requests from clients. One possible solution is suggested as follows. Since it is probably unrealistic to folk 200 child processes, the parent process has to have a method to queue input requests. Therefore, the parent process can fork some child processes responsible for queuing input requests, which in turn dispatch requests to other child processes to complete the job. In other words, the requests from the clients may need to wait for services in the server but the server will respond to clients’ requests eventually.


The program consists of nine subprojects.

