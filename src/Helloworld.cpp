//============================================================================
// Name        : Helloworld.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <leveldb/db.h>
#include <boost/random.hpp>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>

typedef void (*function_test)(int a);

function_test g_ft;

void set_test_func(function_test ft){
	g_ft=ft;
}

//试验发现修改函数返回值和修改参数类型个数都不行，编译不通过
void real_func(int a){
	printf("%d\n",a);
}

int main(int argc, char **argv) {
	set_test_func(real_func);
	if(g_ft){
		g_ft(1);
	}
}

