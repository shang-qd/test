#include "stdafx.h"

#include "Common.h"
#include <mswsock.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "mswsock.lib")

// ��ȫ�ر�һ��TCP����
bool ShutdownConnection(SOCKET sd)
{
    // ���ȷ���һ��TCP FIN�ֶΣ���Է������Ѿ�������ݷ��͡�
    if (shutdown(sd, SD_SEND) == SOCKET_ERROR)
	{
        cout << "shutdown error " << WSAGetLastError() <<endl;
        return false;
    }
    char buff[nBuffSize];
    int nRecv;
    // �������նԷ������ݣ�ֱ��recv����0Ϊֹ
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
    // �ر��׽���
    if (closesocket(sd) == SOCKET_ERROR)
	{
        cout << "closesocket error " << WSAGetLastError() <<endl;
        return false;
    }
    return true;
}

// ����һ�������׽��ֲ��������״̬
SOCKET BindListen()
{
    // ����һ�������׽���
    SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == INVALID_SOCKET){
        cout << "socket error " << WSAGetLastError() <<endl;
        return INVALID_SOCKET;
    }
    // ��䱾���׽��ֵ�ַ
    // ����ʹ����ͨ���ַINADDR_ANY����Ȼ����Ҳ����ָ��һ������ı���IP��ַ����
    // ��ʹ��ͨ���ַ�Ļ������ǿ��Խ���������������ӿڵ�������������ڴ��ж�
    // ����������multihomed�ķ�������˵�����Լ򻯷������ı�̡�
    sockaddr_in saListen;
    saListen.sin_family = AF_INET;
    saListen.sin_addr.s_addr = htonl(INADDR_ANY);
    saListen.sin_port = htons(nDefaultServerPort);
    // ����bind�ѱ����׽��ֵ�ַ�󶨵������׽���
    if (bind(sd, (sockaddr*)&saListen, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
        cout << "bind error " << WSAGetLastError() <<endl;
        closesocket(sd);
        return INVALID_SOCKET;
    }
    // ��ʼ����
    if (listen(sd, 5) == SOCKET_ERROR)
	{
        cout << "listen error " << WSAGetLastError() <<endl;
        closesocket(sd);
        return INVALID_SOCKET;
    }
    return sd;
}

// ����һ���ͻ������Ӳ����ض�Ӧ�ڸ����ӵ��׽��־��
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

// ����һ��WSA_FLAG_OVERLAPPED�׽��� 
SOCKET CreateOverlappedSocket() 
{
    SOCKET hSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0,WSA_FLAG_OVERLAPPED);
    if (hSocket == INVALID_SOCKET) 
	{
        cout <<"WSASocket error " << WSAGetLastError() << endl;
    }
    return hSocket;
}

// ����һ�������첽I/O�ļ����׽��ֲ��������״̬
SOCKET BindListenOverlapped() 
{
    // ����һ�������첽I/O�ļ����׽���
    SOCKET sd = CreateOverlappedSocket();
    if (sd == INVALID_SOCKET) 
	{
        return INVALID_SOCKET;
    }
    // ��䱾���׽��ֵ�ַ
    sockaddr_in saListen;
    saListen.sin_family = AF_INET;
    saListen.sin_addr.s_addr = htonl(INADDR_ANY);
    saListen.sin_port = htons(nDefaultServerPort);
    // ����bind�ѱ����׽��ֵ�ַ�󶨵������׽���
    if(bind(sd, (sockaddr*)&saListen, sizeof(sockaddr_in)) == 
        SOCKET_ERROR){
        cout << "bind error " << WSAGetLastError() <<endl;
        closesocket(sd);
        return INVALID_SOCKET;
    }
    // ��ʼ����
    if(listen(sd, 5) == SOCKET_ERROR){
        cout << "listen error " << WSAGetLastError() <<endl;
        closesocket(sd);
        return INVALID_SOCKET;
    }
    return sd;
}
// ����AcceptExʱ��Ҫ�õ��Ļ�����������������������汾�غ�Զ�̵�ַ
char bAcceptBuffer[2 * (sizeof(SOCKADDR_IN) + 16)];
DWORD dwAcceptBytes = 0;

// �����첽AcceptEx����
SOCKET StartAccept(SOCKET hListenSocket, HANDLE hListenEvent, LPOVERLAPPED lpOverlapListen) 
{
    // ����һ���첽�׽���hAcceptSocket��������AcceptEx�����첽��AcceptEx���
    // ʱ������WSAWaitForMultipleEvents�ɹ��������䷵��ֵ��ʾ�����źŵ��¼���
    // �����׽��ֵ��¼�ʱ, �ڴ˴�������hAcceptSocket�ʹ���ɹ����ܵĿͻ�����
    SOCKET hAcceptSocket = CreateOverlappedSocket();
    if (hAcceptSocket == INVALID_SOCKET)
        return INVALID_SOCKET;
    // ��ʼ�������׽��ֵ�WSAOVERLAPPED�ṹ
    ZeroMemory(lpOverlapListen, sizeof(WSAOVERLAPPED));
    lpOverlapListen->hEvent = hListenEvent;

    // �����첽AcceptEx����
    if (!AcceptEx(hListenSocket, hAcceptSocket, bAcceptBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwAcceptBytes, lpOverlapListen)) 
	{
        // ���AcceptExʧ�ܲ��Ҵ�����벻��ERROR_IO_PENDING
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