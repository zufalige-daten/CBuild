#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

int subprocess_start(int (*subprocess_function)(void)){
	int ret = fork();
	if(ret == -1){
		return -1;
	}
	if(ret == 0){
		int exit_stat = subprocess_function();
		exit(exit_stat);
	}
	return ret;
}

void *subprocess_create_share(size_t data_len){
	void * ret = mmap(NULL, data_len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	return ret;
}

void subprocess_delete_share(void *share, size_t data_len){
	munmap(share, data_len);
}

