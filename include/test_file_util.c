#include "file_util.h"
int main(){
	char *fileContents = read_file("file_util.h");
	printf("File content:\n %s", fileContents);
	write_to_file("example.txt", "File content");
}
