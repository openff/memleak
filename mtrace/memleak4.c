
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>

#include <mcheck.h>
/**
 * 原理：通过函数修改malloc调用情况，当mem_trace启动后 malloc会调用myself版本，当mem_untrace调用后malloc调用系统版本。
*/

//定义2个系统提供的辅助函数指针
typedef void *(*malloc_hook_t)(size_t size, const void *caller);
malloc_hook_t malloc_f;
typedef void (*free_hook_t)(void *p, const void *caller);
free_hook_t free_f;

int replaced = 0;
//此函数调用后 更改malloc底层调用函数，变更为myself函数
void mem_trace(void);
//反之 myself -> systemctl
void mem_untrace(void);

void *malloc_hook_f(size_t size, const void *caller) {
    //防止下列函数调用malloc进入此区间 死递归
	mem_untrace(); //让下列api走系统的
	void *ptr = malloc(size);
	char buff[128] = {0};
	sprintf(buff, "./mem/%p.mem", ptr);

	FILE *fp = fopen(buff, "w");
	fprintf(fp, "[+%p] --> addr:%p, size:%ld\n", caller, ptr, size);
	fflush(fp);

	fclose(fp); //free
	
	mem_trace();
		
	return ptr;
	
}

void free_hook_f(void *p, const void *caller) {

	mem_untrace();
	//printf("-%p: addr[%p]\n", caller, p);

	char buff[128] = {0};
	sprintf(buff, "./mem/%p.mem", p);

	if (unlink(buff) < 0) { // no exist
		printf("double free: %p\n", p);
		return NULL;
	}
	
	free(p);
	mem_trace();
	
}

void mem_trace(void) { //mtrace

	replaced = 1;
    //备份系统指向函数
	malloc_f = __malloc_hook; 
	free_f = __free_hook;
    //将系统函数更替
	__malloc_hook = malloc_hook_f;
	__free_hook = free_hook_f;
}

void mem_untrace(void) {
	//回溯
	__malloc_hook = malloc_f;
	__free_hook = free_f;
	replaced = 0;
}

int main() {
    mem_trace();  //开启
    //内存检测区间
    void *p1 = malloc(10);
    void *p2 = malloc(20); //calloc, realloc

    free(p1);

    void *p3 = malloc(20);
    void *p4 = malloc(20);

    free(p2);
    free(p4);

    mem_untrace();  //关闭

    return 0;
}
