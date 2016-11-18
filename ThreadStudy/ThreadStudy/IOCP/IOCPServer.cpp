#include "stdafx.h"

#include "Common.h"
#include <vector>
#include <process.h>

typedef vector<IOContext*> ConnectionList;
ConnectionList g_conns;
// 临界区g_csConns用于同步线程池的线程对g_conns的访问
CRITICAL_SECTION g_csConns;

DWORD WINAPI WorkerThread (LPVOID para);
typedef unsigned (__stdcall *PTHREAD_START) (void *);
#define MAX_THREAD_NUM 32
// EXIT_THREAD用来通知线程池的线程退出
#define EXIT_THREAD 2012

SOCKET g_hListenSocket = INVALID_SOCKET;

// 添加一个IOContext对象到g_conns，注意需要使用临界区进行线程同步
void AddToList(IOContext* pIOContext)
{
    EnterCriticalSection(&g_csConns);
    g_conns.push_back(pIOContext);
    LeaveCriticalSection(&g_csConns);
}

// 从g_conns中移除一个IOContext对象，注意需要使用临界区进行线程同步
void RemoveFromList(IOContext* pIOContext)
{
    EnterCriticalSection(&g_csConns);
    ConnectionList::iterator it = g_conns.begin();
    while (it != g_conns.end())
	{
        if ((*it) == pIOContext)
		{
            g_conns.erase(it);
            break;
        }
        ++it;
    }
    LeaveCriticalSection(&g_csConns);
}

// 输入Ctrl+c来结束服务程序，收到Ctrl+c消息后，强制调用closesocket来关闭监
// 听套接字，这样主线程在调用accept时就会返回错误，从而退出主循环
BOOL WINAPI CtrlHandler (DWORD dwEvent) 
{
	SOCKET sockTemp = INVALID_SOCKET;
	switch (dwEvent) 
	{
	case CTRL_C_EVENT:
        if (g_hListenSocket == INVALID_SOCKET)
		{
            return TRUE;
        }
		sockTemp = g_hListenSocket;
		g_hListenSocket = INVALID_SOCKET;
		closesocket(sockTemp);
    	return TRUE;
	}
	return FALSE;
}

// 释放一个IOContext对象
void CloseIO(IOContext* pIOContext)
{
    closesocket(pIOContext->hSocket);
    RemoveFromList(pIOContext);
    delete pIOContext;
}

// IOCPServer的主体函数
void DoWork() 
{
	// 接收键盘的Ctrl+c消息
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
	// 定义线程句柄数组，用于保存线程池中线程的句柄
    HANDLE hThreads[MAX_THREAD_NUM];
	for (int i = 0; i < MAX_THREAD_NUM; ++i)
	{
		hThreads[i] = INVALID_HANDLE_VALUE;
	}
    DWORD dwThreadCount = 0;
	// 初始化用于线程同步的临界区g_csConns
    InitializeCriticalSection(&g_csConns);
    // 为了简化程序设计，监听套接字使用同步方式
	g_hListenSocket = BindListen();
    if (g_hListenSocket == INVALID_SOCKET)
        goto cleanup;
	// 创建一个IOCP内核对象，最后一个参数NumberOfConcurrentThreads为0表示
	// 使用当前系统的CPU数目来作为运行线程队列的最大值    
    HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
    if (hIOCP == NULL)
	{
        cout << "CreateIoCompletionPort error " << GetLastError() << endl;
        goto cleanup;
    }
    // 获取系统CPU的数目
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    DWORD dwCPU = systemInfo.dwNumberOfProcessors;
    // 创建线程池，线程池的线程数目是CPU数目的两倍
    for (dwThreadCount = 0; dwThreadCount < dwCPU * 2; ++dwThreadCount) 
	{
		// 创建线程并把hIOCP作为线程函数的参数
        unsigned int dwThreadId = 0;
        HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, (PTHREAD_START)WorkerThread, hIOCP, 0, &dwThreadId);
        if (hThread == NULL)
		{
	        cout << "_beginthreadex error " << GetLastError() << endl;
			goto cleanup;
        }
        hThreads[dwThreadCount] = hThread;
    }
	// 主循环
    while (true) 
	{
		// 采用同步方式接受一个客户连接
        SOCKET hAcceptSocket = AcceptConnection(g_hListenSocket);
		// accept返回INVALID_SOCKET，有可能是因为真的发生了I/O错误，也有可
		// 能是因为在程序接收到Ctrl+c消息时主动关闭了监听套接字，从而导致
		// 主线程退出主循环
        if (hAcceptSocket == INVALID_SOCKET)
		{
            goto cleanup;
        }
        // 让完成端口于IO关联起来，就是操作完成端口的IO队列
        hIOCP = CreateIoCompletionPort((HANDLE)hAcceptSocket, hIOCP, 0, 0);
        if (hIOCP == NULL)
		{
	        cout<<"CreateIoCompletionPort error "<<GetLastError() << endl;
            goto cleanup;
        }

        // 通过WSARecv发出异步请求。
        IOContext* pIOContext = new IOContext(hAcceptSocket);
        AddToList(pIOContext);
        DWORD dwFlags = 0;
		// 发出第一个异步I/O请求
        int nRet = WSARecv(hAcceptSocket, &(pIOContext->wsaBuffer), 1, NULL, &dwFlags, (LPWSAOVERLAPPED)pIOContext, NULL);
		int lastErr = WSAGetLastError();
        if (nRet == SOCKET_ERROR) 
		{
            if (ERROR_IO_PENDING != lastErr)
			{
		        cout << "WSARecv error "<< lastErr << endl;
                CloseIO(pIOContext);
            }
		}
    }
cleanup:
    if (hIOCP != NULL)
	{
        // 发出模拟的完成通知，CompletionKey的值EXIT_THREAD，以便线程能够退出
		for (DWORD i = 0; i < dwThreadCount; ++i)
		{
			PostQueuedCompletionStatus(hIOCP, 0, EXIT_THREAD, NULL);
		}
    }

	// 等待线程池的所有线程退出
    if (dwThreadCount > 0)
	{
        WaitForMultipleObjects(dwThreadCount, hThreads, TRUE, INFINITE);
    }
	for (DWORD i = 0; i < dwThreadCount; ++i)
	{
		CloseHandle(hThreads[i]);
	}
	// 释放资源
    EnterCriticalSection(&g_csConns);
    ConnectionList::iterator it = g_conns.begin();
    while (it != g_conns.end())
	{
        closesocket((*it)->hSocket);
        delete (*it);
        ++it;
    }
    g_conns.clear();
    LeaveCriticalSection(&g_csConns);
    if (hIOCP)
	{
        CloseHandle(hIOCP);
        hIOCP = NULL;
    }
    if (g_hListenSocket != INVALID_SOCKET)
	{
        closesocket(g_hListenSocket);
        g_hListenSocket = INVALID_SOCKET;
    }
	DeleteCriticalSection(&g_csConns);
}

// 线程池的线程函数
DWORD WINAPI WorkerThread(LPVOID para)
{
	HANDLE hIOCP = (HANDLE)para;
	IOContext* pIOContext = NULL; 
    DWORD dwBytes = 0;
    ULONG ulKey = 0;
    DWORD dwFlags = 0;
    int nRet = 0;
    while (true)
	{
		// 调用GetQueuedCompletionStatus，线程进入hIOCP的等待队列
        BOOL bOK = GetQueuedCompletionStatus(hIOCP,&dwBytes, &ulKey,(LPOVERLAPPED*)&pIOContext,INFINITE);
        // 收到线程退出通知，退出循环
        if (bOK && ulKey == EXIT_THREAD)
		{
            break;
        }
		// 发生了错误
        if (!bOK) 
		{
	        cout << "GetQueuedCompletionStatus error " << GetLastError() << endl;
            if (pIOContext != NULL && pIOContext->Internal != 0)
			{
                cout << "I/O error "<< pIOContext->Internal << endl;
                // STATUS_REMOTE_DISCONNECT or STATUS_CONNECTION_RESET
				// 连接被对方意外关闭
                if (pIOContext->Internal == 0xC000013C || pIOContext->Internal == 0xc000020d)
				{
		            cout << "Connection was closed unexpectedly" << endl;
                }
            }
            CloseIO(pIOContext);
            continue;
        }
		// 对方正常关闭了连接
		if (dwBytes == 0)
		{
		    cout << "Connection closed by peer." << endl;
            CloseIO(pIOContext);
		    continue;
		}
	    // 如果当前异步IO读完成，就再次发送异步IO写请求
        if (pIOContext->op == IoRead)
		{
	        // 更新可用数据的大小
            pIOContext->nBytesInBuf += dwBytes;
	        // 为即将调用的WSASend准备好缓冲区参数
            pIOContext->wsaBuffer.buf = pIOContext->Buffer;
            pIOContext->wsaBuffer.len = pIOContext->nBytesInBuf;
            ZeroMemory(pIOContext, sizeof(WSAOVERLAPPED));
            pIOContext->op = IoWrite;
            dwFlags = 0;
            nRet = WSASend(pIOContext->hSocket, &(pIOContext->wsaBuffer),1, NULL, dwFlags, pIOContext, NULL);
	        int lastErr = WSAGetLastError();
	        if (nRet == SOCKET_ERROR && WSA_IO_PENDING != lastErr) 
			{
	            cout<<"WSASend error "<< lastErr << endl;
                CloseIO(pIOContext);
            }
        }
	    // 如果当前的异步IO写完成 再次发送异步IO读请求
        else if (pIOContext->op == IoWrite)
		{
            pIOContext->nBytesInBuf -= dwBytes;
            if (pIOContext->nBytesInBuf > 0) 
			{
                memmove(pIOContext->Buffer, pIOContext->Buffer + dwBytes,pIOContext->nBytesInBuf);
            }
	        // 计算缓冲区空闲空间的偏移
            pIOContext->wsaBuffer.buf = pIOContext->Buffer + pIOContext->nBytesInBuf;
	        // 计算缓冲区空闲空间的大小
            pIOContext->wsaBuffer.len = nBuffSize - pIOContext->nBytesInBuf;
            ZeroMemory(pIOContext, sizeof(WSAOVERLAPPED));
            pIOContext->op = IoRead;
            dwFlags = 0;
	        // 发出异步WSARecv请求
            nRet = WSARecv(pIOContext->hSocket, &(pIOContext->wsaBuffer),1, NULL, &dwFlags, pIOContext, NULL);
	        int lastErr = WSAGetLastError();
	        if (nRet == SOCKET_ERROR && WSA_IO_PENDING != lastErr) 
			{
	            cout<<"WSARecv error "<< lastErr << endl;
                CloseIO(pIOContext);
            }
        }
    }
    return 0;
}

int iocp_main(int argc, char* argv[])
{
    WSAData wsaData;
	int nCode;
    if ((nCode = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) 
	{
		cout << "WSAStartup error " << nCode << endl;
        return -1;
    }
    DoWork();
    WSACleanup();
    return 0;
}