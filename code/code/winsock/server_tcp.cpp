// server_tcp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//#include <iostream>
#include <stdio.h>
#include <winsock2.h>

int main()
{
	//std::cout << "Hello World!\n";

	WSADATA              wsaData;
	SOCKET               ListeningSocket;
	SOCKET               NewConnection;
	SOCKADDR_IN          ServerAddr;
	SOCKADDR_IN          ClientAddr;
	int                  Port = 5150;
	int			   		 ClientAddrLen;
	char				 msg[] = "Hello World!";

	// 初始化Windows Socket 2.2

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// 创建一个新的Socket来响应客户端的连接请求

	ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 填写服务器地址信息
	// 端口为5150
	// IP地址为INADDR_ANY，注意使用htonl将IP地址转换为网络格式

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(Port);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 绑定监听端口

	bind(ListeningSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));

	// 开始监听，指定最大同时连接数为5

	listen(ListeningSocket, 5);

	// 接受新的连接

	for (;;)
	{
		ClientAddrLen = sizeof(ClientAddr);
		NewConnection = accept(ListeningSocket, (SOCKADDR*)&ClientAddr, &ClientAddrLen);

		// 新的连接建立后，就可以互相通信了
		//  
		printf("Got a connection\n");
		
		if (send(NewConnection, msg, sizeof(msg), 0) > 0)
			printf("Send out message: %s\n", msg);

		closesocket(NewConnection);
	}

	closesocket(ListeningSocket);

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
