#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <thread>
#include <iostream>
#define PORT 7000
#define QUEUE 20
int conn;
void thread_task()
{
}

int main()
{
    //printf("%d\n",AF_INET);     //IPv4 协议
    printf("%d\n", SOCK_STREAM);              //字节流套接字
    int ss = socket(AF_INET, SOCK_STREAM, 0); //若成功则返回一个 sockefd (套接字描述符)

    struct sockaddr_in server_sockaddr; //一般是存储地址和端口的。用于信息的显示及存储使用

    //设置 sockaddr_in 结构体中相关参数
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT); //将一个无符号短整型数值转换为网络字节序，即大端序

    //INADDR_ANY 表示地址 0.0.0.0，此地址表示所有的地址
    //一般来说，在各个系统中均定义为 0 值
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(ss, (struct sockaddr *)(&server_sockaddr), sizeof(server_sockaddr)) == 1)
    {
        perror("bind");
        exit(1);
    }

    if (listen(ss, QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }

    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    //成功返回非负面熟字，出错返回 -1
    conn = accept(ss, (struct sockaddr *)&client_addr, &length);
    // 如果 accept 成功，那么其返回值是由内核自动生成的全新描述符，代表与所返回的客户的 tcp 链接
    // accept 之后就会用新的套接字 conn

    if (conn < 0)
    {
        perror("connect");
        exit(1);
    }

    char buffer[1024];
    //创建另外一个线程
    //std::thread t(thread_task);
    //t.join();
    //char buf[1024];
    //主线程

    while (1)
    {
        //这里把send注释掉了，所以这个程序中server只能是接收client端的数据并能给client发送数据，即使不注释掉也没用，因为没有对是否有数据传入和传入
        //进行判断所以按照下面的代码这样写，每次都要先让server输入后才能输出client传过来的数据，若是server不输入则程序无法向下走就没有client发送过来的输出，
        //而且每次显示也只能是一行，这样显示就全是错的了，所以就需要select和FD_ISSET的判断了
        // memset(buf, 0 ,sizeof(buf));
        // if(fgets(buf, sizeof(buf),stdin) != NULL) {
        //     send(conn, buf, sizeof(buf), 0);
        // }

        memset(buffer, 0, sizeof(buffer));
        int len = recv(conn, buffer, sizeof(buffer), 0); //从 TCP 链接的另一端接收数据
        /*该函数的第一个参数指定接收端套接字描述符；
        第二个参数指明一个缓冲区，用于存放recv函数接收到的数据；
        第三个参数指明buf的长度；
        第四个参数一般值为 0 ？？？？ */
        if (strcmp(buffer, "exit\n") == 0)
        {
            break;
        }
        printf("%s", buffer); //如果有接收到数据，则打印
        //必须由返回数据，这样才算是一个完整的请求
        send(conn, buffer, len, 0); //向 TCP 连接的另一端发送数据
    }
    close(conn); // 因为 accept 函数连接成功后，还会生成一个新的套接字描述符，结束后也要关闭
    close(ss);

    return 0;
}
