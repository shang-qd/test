/********************************************************************

重复乃万恶之源

created:	2016/04/10
filename: 	OIS_IPC.h
author:		Michael Lee
purpose:

modified:
消息队列接口，支持“客户端-服务器”模式和“发布-订阅”模式

客户端-服务器模式：一对一，客户端发送，服务端接收并回复
发布-订阅模式：一对多，发布端发送，所有订阅端接收（不回复）

具体使用方法参加Test和TestClient
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
		MAX_MSG_BYTES = 1024,		// 单个消息最大字节数
		MAX_MSG_COUNT = 10000,		// 队列中缓存的最大消息数量
		MAX_DATA_BYTES = 32 * 1024,	// 单个数据包最大字节数
		SEND_TIMEOUT = 100,			// 发送响应消息超时，单位：毫秒
		RECV_TIMEOUT = 100,			// 接收响应消息超时，单位：毫秒
		RETRY_TIMES = 3,			// 接收响应消息重试次数
	};

	enum IPC_TYPE
	{
		IPC_REQREP = 0,			// 请求-应答，通讯流程是一发一收
		IPC_PUBSUB,				// 发布-订阅
		IPC_PUSHPULL, 			// 推-拉
		IPC_UDP,				// UDP
		IPC_SHAREDMEM			// 共享内存
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