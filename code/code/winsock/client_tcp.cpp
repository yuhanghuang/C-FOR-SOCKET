// client_tcp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    //std::cout << "Hello World!\n";
	WSADATA              wsaData;
	SOCKET               s;
	SOCKADDR_IN          ServerAddr;
	int                  Port = 5150;
	char				 msg[256];

	//初始化Windows Socket 2.2

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// 创建一个新的Socket来连接服务器

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 填写服务器端地址信息
	// 端口为5150
	// 服务器IP地址为"136.149.3.29"，注意使用inet_addr将IP地址转换为网络格式

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(Port);
	//ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", &ServerAddr.sin_addr.s_addr);

	// 向服务器发出连接请求

	if (connect(s, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		printf("connect error\n");
		closesocket(s);
		exit(1);
	}

	// 新的连接建立后，就可以互相通信了
	
	memset(msg, 0, sizeof(msg));
	if (recv(s, msg, sizeof(msg)-1, 0) > 0)
		printf("Received message from server: %s\n", msg);
	
	system("PAUSE");

	closesocket(s);

	// 释放Windows Socket DLL的相关资源

	WSACleanup();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
