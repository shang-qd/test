#pragma once
#include <winsock2.h> 
#include <mswsock.h>
#include <iostream>
using namespace std;

const int nDefaultServerPort = 10000;
const int nBuffSize = 1024;

// 对于同一个客户连接套接字，任一时刻只能有一个未完的异步I/O操作，要么是
// WSASend（IoWrite），要么是WSARecv（IoRead）
typedef enum _IO_OPERATION
{
    IoRead,
    IoWrite
} IO_OPERATION;

// 每一个客户连接都有一个IOContext对象，IOContext继承于WSAOVERLAPPED，在调用异
// 步I/O操作WSASend或者WSARecv时，会把IOContext对象的地址作为它们的
// WSAOVERLAPPED参数，这样以后在异步I/O操作完成后，系统将会提供调用时传入的
// WSAOVERLAPPED指针，此时再把WSAOVERLAPPED指针强制转换成IOContext指针，就可
// 以通过IOContext的hSocket成员来确定当前的完成通知是针对哪一个客户连接的。
struct IOContext : WSAOVERLAPPED 
{
    SOCKET hSocket;
    char Buffer[nBuffSize];
    int nBytesInBuf;
    WSABUF wsaBuffer;
    IO_OPERATION op;
    IOContext(SOCKET sock) : hSocket(sock), nBytesInBuf(0)
	{
        wsaBuffer.buf = Buffer;
        wsaBuffer.len = nBuffSize;
        op = IoRead;
        ZeroMemory(this, sizeof(WSAOVERLAPPED));
    }
};

bool ShutdownConnection(SOCKET sd);

SOCKET BindListen();

SOCKET AcceptConnection(SOCKET sdListen);

SOCKET CreateOverlappedSocket();

SOCKET BindListenOverlapped();

SOCKET StartAccept(SOCKET hListenSocket, HANDLE hListenEvent, LPOVERLAPPED lpOverlapListen);