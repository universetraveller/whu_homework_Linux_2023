#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct Ext4Superblock {
    uint32_t inodes_count;       
    uint32_t blocks_count;       
    uint32_t r_blocks_count;     
    uint32_t free_blocks_count;  
    uint32_t free_inodes_count;  
    uint32_t first_data_block;   
    uint32_t log_block_size;     
    uint32_t log_frag_size;      
    uint32_t blocks_per_group;   
    uint32_t frags_per_group;    
    uint32_t inodes_per_group;   
    uint32_t mtime;              
    uint32_t wtime;              
    uint32_t mnt_count;
    uint32_t max_mnt_count;
};

struct Ext4GroupDescriptor {
    uint32_t block_bitmap;
    uint32_t inode_bitmap;
    uint32_t inode_table;
};

void print_line(const char* str, int num){
    for(int i = 0; i < num; ++i)
        printf("%s", str);
    printf("\n");
}

void print_to_align(const char* str, uint32_t to_p, int num){
    printf("%s", str);
    if(strlen(str) < num){
        for(int i = 0; i < num - strlen(str); ++i)
            printf("%s", " ");
    }
    printf(" %u\n", to_p);
}

void fill_bitmap(int* binary, uint32_t n){
    for(int i = 0; i < 32; ++i)
        *(binary+i) = 0;
    if (n == 0) 
        return;

    int index = 0;

    while (n > 0) {
        binary[index++] = n % 2;
        n /= 2;
    }
}

void print_bitmap(const char* str, uint32_t n, int num){
    int binary[32];
    fill_bitmap(binary, n);
    printf("%s", str);
    if(strlen(str) < num){
        for(int i = 0; i < num - strlen(str); ++i)
            printf("%s", " ");
    }
    printf(" ");
    for(int i = 0; i < 32; ++i)
        printf("%d", binary[i]);
    printf(" (%u)\n", n);
}
void inspect_file_system(const char* device_path) {
    FILE* file = fopen(device_path, "rb");
    if (!file) {
        perror("Error opening device");
        exit(EXIT_FAILURE);
    }

    fseek(file, 1024, SEEK_SET);

    struct Ext4Superblock superblock;
    fread(&superblock, sizeof(struct Ext4Superblock), 1, file);

    // super block analysis
    print_line("-", 80);
    printf("EXT4 Superblock Information\n");
    print_line("-", 80);
    print_to_align("Total inode count", superblock.inodes_count, 65);
    print_to_align("Total block count", superblock.blocks_count, 65);
    print_to_align("This number of blocks can only be allocated by the super-user", superblock.r_blocks_count, 65);
    print_to_align("Free block count", superblock.free_blocks_count, 65);
    print_to_align("Free inode count", superblock.free_inodes_count, 65);
    print_to_align("First data block", superblock.first_data_block, 65);
    print_to_align("Block size (bytes)", 1024 << superblock.log_block_size, 65);
    print_to_align("Cluster size (bytes)", 1024 << superblock.log_frag_size, 65);
    print_to_align("Blocks per group", superblock.blocks_per_group, 65);
    print_to_align("Clusters per group", superblock.frags_per_group, 65);
    print_to_align("Inodes per group", superblock.inodes_per_group, 65);
    print_to_align("Mount time", superblock.mtime, 65);
    print_to_align("Write time", superblock.wtime, 65);
    print_to_align("Number of mounts since the last fsck", superblock.mnt_count, 65);
    print_to_align("Number of mounts beyond which a fsck is needed", superblock.max_mnt_count, 65);
    print_line("-", 80);

    // group descriptor analysis
    uint32_t group_desc_size = sizeof(struct Ext4GroupDescriptor);
    uint32_t group_desc_table_size = superblock.blocks_per_group * group_desc_size;
    uint32_t group_desc_table_offset = 1024 + group_desc_table_size;

    fseek(file, group_desc_table_offset, SEEK_SET);

    struct Ext4GroupDescriptor* group_desc_table = (struct Ext4GroupDescriptor*)malloc(group_desc_table_size);

    fread(group_desc_table, group_desc_size, superblock.blocks_per_group, file);

    fclose(file);

    printf("\n\n\n");

    print_line("=", 80);
    printf("EXT4 Group Descriptor Table Information:\n");
    printf("Notice: Only print groups whose bitmaps are not all zero\n");
    print_line("=", 80);
    int _first_round = 1;
    for (uint32_t i = 0; i < superblock.blocks_per_group; i++) {
        uint32_t _block_bitmap = group_desc_table[i].block_bitmap;
        uint32_t _inode_bitmap = group_desc_table[i].inode_bitmap;
        uint32_t _inode_table = group_desc_table[i].inode_table;
        if(_block_bitmap || _inode_bitmap || _inode_table){
            if(!_first_round)
                print_line("-", 80);
            else
                _first_round = 0;
            print_to_align("Group", i, 20);
            if(_block_bitmap)
                print_bitmap("Block Bitmap", _block_bitmap, 20);
            if(_inode_bitmap)
                print_bitmap("Inode Bitmap", _inode_bitmap, 20);
            if(_inode_table)
                print_bitmap("Inode Table", _inode_table, 20);
        }
    }
    print_line("=", 80);

    free(group_desc_table);
}

int main() {
    const char* file_system = "./rootfs.ext4";
    
    inspect_file_system(file_system);

    return 0;
}
