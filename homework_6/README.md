# Document
## Thought
Parse the command line arguments, and add link, username and password to the end of `/etc/davfs2/secrets`. Update configuration file each time the command executed.  

I only finish the tasks and I do not implement local webdav cloud storage because I have no time.  

## davfs2
davfs2 is a Linux file system driver that allows you to mount a WebDAV
resource into your Unix file system tree. So - and that is what makes
davfs2 different - applications can use it without knowing about WebDAV.
You may edit WebDAV resources using standard applications that interact
with the file system as usual.  

davfs2 supports SSL and proxy, HTTP authentication (basic and digest)
and client certificates.  

## Data Structure
No need

## Algorithms
Update configuration file each time the command is executed to prevent errors at running davfs2.    

Update secret when the command is called to keep mounting up to date.  