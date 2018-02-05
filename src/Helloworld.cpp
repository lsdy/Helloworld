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

#define socket_t int

static void socket_call(const char *label, int result) {
	if (result < 0) {
		fprintf(stderr, "%s : %s", label, strerror(errno));
		abort();
	}
}

void set_nonblock(socket_t fd) {
//	int nb;
//	nb = 1;
//	ioctl(fd, FIONBIO, &nb);
	int fl = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}

socket_t startup(char* _ip, int _port)  //创建一个套接字，绑定，检测服务器
		{
	//sock
	//1.创建套接字
	socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
	socket_call("create socket", sock);

	int opt = 1;
	socket_call("set socket options",
			setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)));

	//2.填充本地 sockaddr_in 结构体（设置本地的IP地址和端口）
	struct sockaddr_in local;
	local.sin_port = htons(_port);
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr(_ip);

	//3.bind（）绑定
	socket_call("bind socket",
			bind(sock, (struct sockaddr*) &local, sizeof(local)));
	//4.listen（）监听 检测服务器
	socket_call("listen socket", listen(sock, 256));
	return sock;    //这样的套接字返回
}

int main(int argc, char **argv) {

	socket_t listen_sock = startup("127.0.0.1", 8081);

	int epfd = epoll_create(256);
	struct epoll_event _ev;
	_ev.events = EPOLLIN;
	_ev.data.fd = listen_sock;

	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &_ev);

	struct epoll_event revs[128];

	int timeout = -1;
	int num = 0;
	int done = 0;
	static int Con_Num = 0;
	static int Client_Write = 0;
	while (!done) {
		switch ((num = epoll_wait(epfd, revs, 128, timeout))) {
		case 0:
			printf("timeout\n");
			break;
		case -1:
			socket_call("epoll_wait", num);
			break;
		default:

			for (int i = 0; i < num; i++) {
				int rsock = revs[i].data.fd;
//				printf("even type : %d\n", revs[i].events);
				if (rsock == listen_sock && (revs[i].events & EPOLLIN)) {
					printf("Client Connect Times : %d\n", ++Con_Num);
					struct sockaddr_in peer;
					socklen_t len = sizeof(peer);
					int new_fd = accept(listen_sock, (struct sockaddr*) &peer,
							&len);
					if (new_fd > 0) {
						printf("get a new client from %s:%d ,give fd : %d\n",
								inet_ntoa(peer.sin_addr), ntohs(peer.sin_port),
								new_fd);
						/*
						 * 设为非阻塞的意义是什么？
						 */
						set_nonblock(new_fd);
						_ev.events = EPOLLIN | EPOLLET;
						_ev.data.fd = new_fd;
						epoll_ctl(epfd, EPOLL_CTL_ADD, new_fd, &_ev);
					}

				} else {
					if (revs[i].events & EPOLLIN) {
						char buf[5];
						ssize_t _s = read(rsock, buf, sizeof(buf) - 1);
						if (_s > 0) {
							printf("Client Write socket : %d times : %d\n",
									rsock, ++Client_Write);
							buf[_s] = '\0';
							printf("Client Send buf :\n%s", buf);
							while (true) {
								ssize_t _s = read(rsock, buf, sizeof(buf) - 1);
								if (_s < 0) {
									printf("%s\n",strerror(errno));
//									printf("errno: %d\n", errno);
									if (errno == EAGAIN) {
										_ev.data.fd = rsock;
										_ev.events = EPOLLOUT | EPOLLET;
										epoll_ctl(epfd, EPOLL_CTL_MOD, rsock,
												&_ev);
										break;
									} else {
										socket_call("read finish", _s);
									}
								} else if (_s == 0) {
									printf("Client :%d Close After Read\n",
											revs[i].data.fd);
									epoll_ctl(epfd, EPOLL_CTL_DEL, rsock, NULL);
									close(rsock);
									break;
								}
								buf[_s] = '\0';
								printf("%s", buf);
							}
//							printf("\n");
						} else if (_s == 0) {
							printf("Client :%d Close\n", revs[i].data.fd);
							epoll_ctl(epfd, EPOLL_CTL_DEL, rsock, NULL);
							close(rsock);
						}

					} else if (revs[i].events & EPOLLOUT) {
						const char* msg="HTTP/1.0.200 OK";
						write(rsock,msg,strlen(msg));
						epoll_ctl(epfd,EPOLL_CTL_DEL,rsock,NULL);
						close(rsock);
					}
				}
			}

		}
	}

}

