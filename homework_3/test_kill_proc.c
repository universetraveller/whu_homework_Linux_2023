#include "proc_util.h"
#include <assert.h>
#include <unistd.h>
int make_sleep(){
	pid_t p = fork();
	if(p == 0)
		sleep(5);
	return p;
}
void test_search_pid(){
	int p = 1;
	search_pid(&p, "");
	assert(p == 1);
	p = 65535;
	search_pid(&p, "make");
	assert(p != 65535);
}

void test_kill_proc(){
	assert(!kill_proc(make_sleep(), ""));
}

int main(){
	test_search_pid();
	test_kill_proc();
}
