# Document
## Basic Principle
The idea is from file `cs_model/chat.c`, which I have written for Client-Server style p2p chatting. And then I implemented `p2p_chat.c` as side-to-side chatting via two processes. One process is to send messages and the another is to receive messages and check if the user has authentication.  

The implementation is based on socket. All call to the command should have a password to specify a unique communicating session. The message is received only when passwords are matched.    

Yet, I know there are some better ways to check if the user is legal (use public key and private key like wiregurad or use session token like Web Applications), but I have no time to implement them.  

## Data Structure
`struct sockaddr_in` which is used to specify the ip address and the port for the socket is the only used data structure.  

## Algorithms
### Before chatting
Bind the ip address (broadcast address) with the specific port to the socket, and set the ip address of peer to another sockaddr_in for sending messages.  

### Send messages
Read a message from buffer. If it is '/exit', the whole process will exit (the child process will be killed at the end of the parent process). Add authentication info to the head of the message, then send it to the destination address. Then, print the formatted infomation of the message (with boxing, time, user name, message and overriding the input characters) to the local terminal.  

### Receive messages
Receive a message from broadcast address, and check the password. If there is no authentication infomation or the password is unmatched, skip this message.  

If the user has authentication, print the formatted infomation of the message like what is done in the  send messages process.  
