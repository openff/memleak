
#include <stdlib.h>
#include <stdio.h>
#include <mcheck.h>

int main() {
    mtrace();
    // 在这里执行内存分配和释放操作
    void *p1 = malloc(10);
    void *p2 = malloc(20);
    free(p1);

    void *p3 = malloc(20);
    void *p4 = malloc(20);
    
    free(p2);
    free(p4);
    void *p5 = malloc(20);
    void *p6 = malloc(20);
    muntrace();

    return 0;
}