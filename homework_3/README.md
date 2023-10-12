# Document
## Implementation of Linux kill function
Linux implements kill function as a system call (`#define __NR_kill 129`).  

Source path: signal.h kill -> signal.c kill -> signal.c sys_kill -> signal.c my_syscall2(__NR_kill, pid, signal) -> assemble language  

## Implementation of processes in Linux
Data structure: task_struct  

Schedule: sched  

## /proc directory  
A virtual file system to access internal data structure of kernel.  

Each pid will present as a sub-directory in this folder.  

## Implemetation of the homework
1. Print all processes (pid and name)  
Get all entries in `/proc`, and then detect if a entry is a number (pid). If we find a number, print it.  

The name of a process can be the command of it, and this command is saved in the file `/proc/{pid}/cmdline`. We just need to print this cmdline file.  

2. Kill a process based on function's parameter(s)
We can define a function with 2 parameters that one is pid and another is command.  

Find the pid of the process by pid or command.  

Use function `kill(pid, signal)` in `signal.h` to kill the process has been found based on its pid, as kill function is defined as a system call in kernel.  

## Data structure
* dirent: struct to access a directory entry  

* FILE: struct to process a file  

## Algorithm
No algorithm is used.  
