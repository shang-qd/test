#include "stdafx.h"

#include "Common.h"
#include <vector>
#include <process.h>

typedef vector<IOContext*> ConnectionList;
ConnectionList g_conns;
// �ٽ���g_csConns����ͬ���̳߳ص��̶߳�g_conns�ķ���
CRITICAL_SECTION g_csConns;

DWORD WINAPI WorkerThread (LPVOID para);
typedef unsigned (__stdcall *PTHREAD_START) (void *);
#define MAX_THREAD_NUM 32
// EXIT_THREAD����֪ͨ�̳߳ص��߳��˳�
#define EXIT_THREAD 2012

SOCKET g_hListenSocket = INVALID_SOCKET;

// ���һ��IOContext����g_conns��ע����Ҫʹ���ٽ��������߳�ͬ��
void AddToList(IOContext* pIOContext)
{
    EnterCriticalSection(&g_csConns);
    g_conns.push_back(pIOContext);
    LeaveCriticalSection(&g_csConns);
}

// ��g_conns���Ƴ�һ��IOContext����ע����Ҫʹ���ٽ��������߳�ͬ��
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

// ����Ctrl+c��������������յ�Ctrl+c��Ϣ��ǿ�Ƶ���closesocket���رռ�
// ���׽��֣��������߳��ڵ���acceptʱ�ͻ᷵�ش��󣬴Ӷ��˳���ѭ��
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

// �ͷ�һ��IOContext����
void CloseIO(IOContext* pIOContext)
{
    closesocket(pIOContext->hSocket);
    RemoveFromList(pIOContext);
    delete pIOContext;
}

// IOCPServer�����庯��
void DoWork() 
{
	// ���ռ��̵�Ctrl+c��Ϣ
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
	// �����߳̾�����飬���ڱ����̳߳����̵߳ľ��
    HANDLE hThreads[MAX_THREAD_NUM];
	for (int i = 0; i < MAX_THREAD_NUM; ++i)
	{
		hThreads[i] = INVALID_HANDLE_VALUE;
	}
    DWORD dwThreadCount = 0;
	// ��ʼ�������߳�ͬ�����ٽ���g_csConns
    InitializeCriticalSection(&g_csConns);
    // Ϊ�˼򻯳�����ƣ������׽���ʹ��ͬ����ʽ
	g_hListenSocket = BindListen();
    if (g_hListenSocket == INVALID_SOCKET)
        goto cleanup;
	// ����һ��IOCP�ں˶������һ������NumberOfConcurrentThreadsΪ0��ʾ
	// ʹ�õ�ǰϵͳ��CPU��Ŀ����Ϊ�����̶߳��е����ֵ    
    HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
    if (hIOCP == NULL)
	{
        cout << "CreateIoCompletionPort error " << GetLastError() << endl;
        goto cleanup;
    }
    // ��ȡϵͳCPU����Ŀ
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    DWORD dwCPU = systemInfo.dwNumberOfProcessors;
    // �����̳߳أ��̳߳ص��߳���Ŀ��CPU��Ŀ������
    for (dwThreadCount = 0; dwThreadCount < dwCPU * 2; ++dwThreadCount) 
	{
		// �����̲߳���hIOCP��Ϊ�̺߳����Ĳ���
        unsigned int dwThreadId = 0;
        HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, (PTHREAD_START)WorkerThread, hIOCP, 0, &dwThreadId);
        if (hThread == NULL)
		{
	        cout << "_beginthreadex error " << GetLastError() << endl;
			goto cleanup;
        }
        hThreads[dwThreadCount] = hThread;
    }
	// ��ѭ��
    while (true) 
	{
		// ����ͬ����ʽ����һ���ͻ�����
        SOCKET hAcceptSocket = AcceptConnection(g_hListenSocket);
		// accept����INVALID_SOCKET���п�������Ϊ��ķ�����I/O����Ҳ�п�
		// ������Ϊ�ڳ�����յ�Ctrl+c��Ϣʱ�����ر��˼����׽��֣��Ӷ�����
		// ���߳��˳���ѭ��
        if (hAcceptSocket == INVALID_SOCKET)
		{
            goto cleanup;
        }
        // ����ɶ˿���IO�������������ǲ�����ɶ˿ڵ�IO����
        hIOCP = CreateIoCompletionPort((HANDLE)hAcceptSocket, hIOCP, 0, 0);
        if (hIOCP == NULL)
		{
	        cout<<"CreateIoCompletionPort error "<<GetLastError() << endl;
            goto cleanup;
        }

        // ͨ��WSARecv�����첽����
        IOContext* pIOContext = new IOContext(hAcceptSocket);
        AddToList(pIOContext);
        DWORD dwFlags = 0;
		// ������һ���첽I/O����
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
        // ����ģ������֪ͨ��CompletionKey��ֵEXIT_THREAD���Ա��߳��ܹ��˳�
		for (DWORD i = 0; i < dwThreadCount; ++i)
		{
			PostQueuedCompletionStatus(hIOCP, 0, EXIT_THREAD, NULL);
		}
    }

	// �ȴ��̳߳ص������߳��˳�
    if (dwThreadCount > 0)
	{
        WaitForMultipleObjects(dwThreadCount, hThreads, TRUE, INFINITE);
    }
	for (DWORD i = 0; i < dwThreadCount; ++i)
	{
		CloseHandle(hThreads[i]);
	}
	// �ͷ���Դ
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

// �̳߳ص��̺߳���
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
		// ����GetQueuedCompletionStatus���߳̽���hIOCP�ĵȴ�����
        BOOL bOK = GetQueuedCompletionStatus(hIOCP,&dwBytes, &ulKey,(LPOVERLAPPED*)&pIOContext,INFINITE);
        // �յ��߳��˳�֪ͨ���˳�ѭ��
        if (bOK && ulKey == EXIT_THREAD)
		{
            break;
        }
		// �����˴���
        if (!bOK) 
		{
	        cout << "GetQueuedCompletionStatus error " << GetLastError() << endl;
            if (pIOContext != NULL && pIOContext->Internal != 0)
			{
                cout << "I/O error "<< pIOContext->Internal << endl;
                // STATUS_REMOTE_DISCONNECT or STATUS_CONNECTION_RESET
				// ���ӱ��Է�����ر�
                if (pIOContext->Internal == 0xC000013C || pIOContext->Internal == 0xc000020d)
				{
		            cout << "Connection was closed unexpectedly" << endl;
                }
            }
            CloseIO(pIOContext);
            continue;
        }
		// �Է������ر�������
		if (dwBytes == 0)
		{
		    cout << "Connection closed by peer." << endl;
            CloseIO(pIOContext);
		    continue;
		}
	    // �����ǰ�첽IO����ɣ����ٴη����첽IOд����
        if (pIOContext->op == IoRead)
		{
	        // ���¿������ݵĴ�С
            pIOContext->nBytesInBuf += dwBytes;
	        // Ϊ�������õ�WSASend׼���û���������
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
	    // �����ǰ���첽IOд��� �ٴη����첽IO������
        else if (pIOContext->op == IoWrite)
		{
            pIOContext->nBytesInBuf -= dwBytes;
            if (pIOContext->nBytesInBuf > 0) 
			{
                memmove(pIOContext->Buffer, pIOContext->Buffer + dwBytes,pIOContext->nBytesInBuf);
            }
	        // ���㻺�������пռ��ƫ��
            pIOContext->wsaBuffer.buf = pIOContext->Buffer + pIOContext->nBytesInBuf;
	        // ���㻺�������пռ�Ĵ�С
            pIOContext->wsaBuffer.len = nBuffSize - pIOContext->nBytesInBuf;
            ZeroMemory(pIOContext, sizeof(WSAOVERLAPPED));
            pIOContext->op = IoRead;
            dwFlags = 0;
	        // �����첽WSARecv����
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