#include "stdafx.h"

#include "Common.h"
#include <mswsock.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "mswsock.lib")

// 安全关闭一个TCP连接
bool ShutdownConnection(SOCKET sd)
{
    // 首先发送一个TCP FIN分段，向对方表明已经完成数据发送。
    if (shutdown(sd, SD_SEND) == SOCKET_ERROR)
	{
        cout << "shutdown error " << WSAGetLastError() <<endl;
        return false;
    }
    char buff[nBuffSize];
    int nRecv;
    // 继续接收对方的数据，直到recv返回0为止
    do
	{
        nRecv = recv(sd, buff, nBuffSize, 0);
        if (nRecv == SOCKET_ERROR)
		{
            cout <<"recv error " << WSAGetLastError() <<endl;
            return false;
        }
        else if(nRecv > 0){
            cout<<nRecv<<" unexpected bytes received."<<endl;
        }
    }
	while(nRecv != 0);
    // 关闭套接字
    if (closesocket(sd) == SOCKET_ERROR)
	{
        cout << "closesocket error " << WSAGetLastError() <<endl;
        return false;
    }
    return true;
}

// 返回一个监听套接字并进入监听状态
SOCKET BindListen()
{
    // 创建一个监听套接字
    SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == INVALID_SOCKET){
        cout << "socket error " << WSAGetLastError() <<endl;
        return INVALID_SOCKET;
    }
    // 填充本地套接字地址
    // 这里使用了通配地址INADDR_ANY。当然我们也可以指明一个具体的本地IP地址，但
    // 是使用通配地址的话，我们可以接受来自所有网络接口的连接请求。这对于带有多
    // 个网卡，即multihomed的服务器来说，可以简化服务器的编程。
    sockaddr_in saListen;
    saListen.sin_family = AF_INET;
    saListen.sin_addr.s_addr = htonl(INADDR_ANY);
    saListen.sin_port = htons(nDefaultServerPort);
    // 调用bind把本地套接字地址绑定到监听套接字
    if (bind(sd, (sockaddr*)&saListen, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
        cout << "bind error " << WSAGetLastError() <<endl;
        closesocket(sd);
        return INVALID_SOCKET;
    }
    // 开始监听
    if (listen(sd, 5) == SOCKET_ERROR)
	{
        cout << "listen error " << WSAGetLastError() <<endl;
        closesocket(sd);
        return INVALID_SOCKET;
    }
    return sd;
}

// 接受一个客户端连接并返回对应于该连接的套接字句柄
SOCKET AcceptConnection(SOCKET sdListen)
{
    sockaddr_in saRemote;
    int nSize = sizeof(sockaddr_in);
    SOCKET sd = accept(sdListen, (sockaddr*)&saRemote, &nSize);
    if (sd == INVALID_SOCKET)
	{
        cout << "accept error " << WSAGetLastError() <<endl;
    }
    return sd;
}

// 创建一个WSA_FLAG_OVERLAPPED套接字 
SOCKET CreateOverlappedSocket() 
{
    SOCKET hSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0,WSA_FLAG_OVERLAPPED);
    if (hSocket == INVALID_SOCKET) 
	{
        cout <<"WSASocket error " << WSAGetLastError() << endl;
    }
    return hSocket;
}

// 返回一个用于异步I/O的监听套接字并进入监听状态
SOCKET BindListenOverlapped() 
{
    // 创建一个用于异步I/O的监听套接字
    SOCKET sd = CreateOverlappedSocket();
    if (sd == INVALID_SOCKET) 
	{
        return INVALID_SOCKET;
    }
    // 填充本地套接字地址
    sockaddr_in saListen;
    saListen.sin_family = AF_INET;
    saListen.sin_addr.s_addr = htonl(INADDR_ANY);
    saListen.sin_port = htons(nDefaultServerPort);
    // 调用bind把本地套接字地址绑定到监听套接字
    if(bind(sd, (sockaddr*)&saListen, sizeof(sockaddr_in)) == 
        SOCKET_ERROR){
        cout << "bind error " << WSAGetLastError() <<endl;
        closesocket(sd);
        return INVALID_SOCKET;
    }
    // 开始监听
    if(listen(sd, 5) == SOCKET_ERROR){
        cout << "listen error " << WSAGetLastError() <<endl;
        closesocket(sd);
        return INVALID_SOCKET;
    }
    return sd;
}
// 调用AcceptEx时需要用到的缓冲区，这个缓冲区用来保存本地和远程地址
char bAcceptBuffer[2 * (sizeof(SOCKADDR_IN) + 16)];
DWORD dwAcceptBytes = 0;

// 发出异步AcceptEx请求
SOCKET StartAccept(SOCKET hListenSocket, HANDLE hListenEvent, LPOVERLAPPED lpOverlapListen) 
{
    // 创建一个异步套接字hAcceptSocket，并传给AcceptEx。当异步的AcceptEx完成
    // 时，即当WSAWaitForMultipleEvents成功返回且其返回值表示出现信号的事件是
    // 监听套接字的事件时, 在此处创建的hAcceptSocket就代表成功接受的客户连接
    SOCKET hAcceptSocket = CreateOverlappedSocket();
    if (hAcceptSocket == INVALID_SOCKET)
        return INVALID_SOCKET;
    // 初始化监听套接字的WSAOVERLAPPED结构
    ZeroMemory(lpOverlapListen, sizeof(WSAOVERLAPPED));
    lpOverlapListen->hEvent = hListenEvent;

    // 发出异步AcceptEx请求
    if (!AcceptEx(hListenSocket, hAcceptSocket, bAcceptBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwAcceptBytes, lpOverlapListen)) 
	{
        // 如果AcceptEx失败并且错误代码不是ERROR_IO_PENDING
        int lastErr = WSAGetLastError();
        if (lastErr != ERROR_IO_PENDING)
		{
            cout <<"AcceptEx error " << lastErr << endl;
            closesocket(hAcceptSocket);
            return INVALID_SOCKET;
        }
    }
    return hAcceptSocket;
}