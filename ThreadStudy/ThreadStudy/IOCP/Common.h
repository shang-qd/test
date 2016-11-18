#pragma once
#include <winsock2.h> 
#include <mswsock.h>
#include <iostream>
using namespace std;

const int nDefaultServerPort = 10000;
const int nBuffSize = 1024;

// ����ͬһ���ͻ������׽��֣���һʱ��ֻ����һ��δ����첽I/O������Ҫô��
// WSASend��IoWrite����Ҫô��WSARecv��IoRead��
typedef enum _IO_OPERATION
{
    IoRead,
    IoWrite
} IO_OPERATION;

// ÿһ���ͻ����Ӷ���һ��IOContext����IOContext�̳���WSAOVERLAPPED���ڵ�����
// ��I/O����WSASend����WSARecvʱ�����IOContext����ĵ�ַ��Ϊ���ǵ�
// WSAOVERLAPPED�����������Ժ����첽I/O������ɺ�ϵͳ�����ṩ����ʱ�����
// WSAOVERLAPPEDָ�룬��ʱ�ٰ�WSAOVERLAPPEDָ��ǿ��ת����IOContextָ�룬�Ϳ�
// ��ͨ��IOContext��hSocket��Ա��ȷ����ǰ�����֪ͨ�������һ���ͻ����ӵġ�
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