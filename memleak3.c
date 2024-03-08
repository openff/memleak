#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>

#include <mcheck.h>


void *malloc_hook(size_t size, const char *file, int line) {

	void *p = malloc(size);

	char buff[128] = {0};
	sprintf(buff, "./mem/%p.mem",p);

	FILE *fp = fopen(buff, "w");
	fprintf(fp, "c/cpp:%s,line:%d --> addr:%p, size:%ld\n", file, line, p, size);
	
    fflush(fp);	

	fclose(fp);

	return p;
}

void free_hook(void *p, const char *file, int line) {

	char buff[128] = {0};
	sprintf(buff, "./mem/%p.mem", p);

	if (unlink(buff) < 0) { // 多次关闭
		printf("double free: %p\n", p);
		return ;
	}
	
	free(p);

}

#define malloc(size)	malloc_hook(size, __FILE__, __LINE__)
#define free(p)			free_hook(p, __FILE__, __LINE__)

int main(){
    void *p1 = malloc(10);
	void *p2 = malloc(20); //calloc, realloc

	free(p1);

	void *p3 = malloc(20);
	void *p4 = malloc(20);

	free(p2);
	free(p4);

	printf("a");
    return 0;
}