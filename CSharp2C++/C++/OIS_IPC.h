/********************************************************************

�ظ������֮Դ

created:	2016/04/10
filename: 	OIS_IPC.h
author:		Michael Lee
purpose:

modified:
��Ϣ���нӿڣ�֧�֡��ͻ���-��������ģʽ�͡�����-���ġ�ģʽ

�ͻ���-������ģʽ��һ��һ���ͻ��˷��ͣ�����˽��ղ��ظ�
����-����ģʽ��һ�Զ࣬�����˷��ͣ����ж��Ķ˽��գ����ظ���

����ʹ�÷����μ�Test��TestClient
*********************************************************************/

#pragma once

#ifndef OIS_IPC_H
#define OIS_IPC_H

#ifdef OIS_IPC_EXPORTS
#define OIS_IPC_API __declspec(dllexport)
#else
#define OIS_IPC_API __declspec(dllimport)
#endif

namespace OIS4
{
	typedef unsigned int uint;

	enum
	{
		MAX_MSG_BYTES = 1024,		// ������Ϣ����ֽ���
		MAX_MSG_COUNT = 10000,		// �����л���������Ϣ����
		MAX_DATA_BYTES = 32 * 1024,	// �������ݰ�����ֽ���
		SEND_TIMEOUT = 100,			// ������Ӧ��Ϣ��ʱ����λ������
		RECV_TIMEOUT = 100,			// ������Ӧ��Ϣ��ʱ����λ������
		RETRY_TIMES = 3,			// ������Ӧ��Ϣ���Դ���
	};

	enum IPC_TYPE
	{
		IPC_REQREP = 0,			// ����-Ӧ��ͨѶ������һ��һ��
		IPC_PUBSUB,				// ����-����
		IPC_PUSHPULL, 			// ��-��
		IPC_UDP,				// UDP
		IPC_SHAREDMEM			// �����ڴ�
	};

	class IIPCCallback
	{
	public:
		virtual int OnRecv(void* pData, uint& nDataSize) = 0;
		virtual int OnSent(void* pData, uint& nDataSize) = 0;
		virtual int OnError(uint nErrNo, const char* pszErrInfo) = 0;
	};

	class ISender
	{
	public:

		virtual bool Create(const char *pszConnectString, IIPCCallback *pCB) = 0;
		virtual bool Send(void *pData, uint nSize) = 0;
		virtual void Destroy() = 0;
	};

	class IRecver
	{
	public:

		virtual bool Create(const char *pszConnectString, IIPCCallback *pCB) = 0;
		virtual void Destroy() = 0;
	};
}

OIS_IPC_API OIS4::ISender* CreateSender(OIS4::IPC_TYPE type);
OIS_IPC_API OIS4::IRecver* CreateRecver(OIS4::IPC_TYPE type);

OIS_IPC_API void DestroySender(OIS4::ISender *pSender);
OIS_IPC_API void DestroyRecver(OIS4::IRecver *pRecver);

#endif