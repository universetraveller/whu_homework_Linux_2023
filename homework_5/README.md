# Document
## Basic Principle
I have created a file system when have done kernel experiments, and I use docker container now. So I would use this file system to test my code. However, the file system (60 MB) is too large to github, so I would not upload it. If there is requirement to reproduce this experiment, please point the file system path from `./rootfs.ext4` to `/dev/sda1` if you have Linux installed on your machine.  
The super block save date via bytes offset, so we just need to read the file system with structure contains different offset size variables and the data will be automatically filled. We can both install file system supports and define the structure in the code (implement later way there).  

## Data Structure
See also [Linux kernel ext4 file system](https://docs.kernel.org/filesystems/ext4/globals.html)  
Full definition [ext4.h](https://elixir.bootlin.com/linux/latest/source/fs/ext4/ext4.h#L1289)  
The super block has too many parameters to parse so I choose to analyze the first 15 parameters. As the website, each parameter keeps size __le32 that equals to uint32_t, so I choose uint32_t as the offset.  
Group descriptor, inodes bitmap and inodes table entry are processed using the same approach.  
I choose the first 3 parameters to analyze for group descriptor becasue they are the lower 32-bits of bitmap that can almost represent the situation of bitmap.  

## Algorithms
No algorithm is required. Just define a structure, read data to it, and print the data.  