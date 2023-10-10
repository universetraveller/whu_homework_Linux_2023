# Document
## Capture network packet and export ip address to a file
Use `tcpdump -c 10 dst host 192.168` to capture 10 packets sent to this computer, then run some `curl` commands in parallel to simulate internet communication.  

When packets are captured, use command `awk` to parse ip addresses which send packets to this computer, then use command `uniq` to remove duplicate lines and write the result to file `ip_captured`.  

## 'Hello, world' C program
Write the source code file of 'Hello, world' program and the makefile to build it then run command `make` to get executable file.  
