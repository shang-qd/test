#pragma once

#ifdef WIN32
#ifdef VCYBERVOICESDK_EXPORTS
#define VCYBERVOICESDK_API __declspec(dllexport)
#else
#define VCYBERVOICESDK_API __declspec(dllimport)
#endif // VCYBERINPUTSDK_EXPORTS
#else
#define VCYBERVOICESDK_API __attribute__ ((visibility ("default")))
#endif // WIN32


#ifdef __cplusplus
extern "C"
{
#endif

#define SESSION_HANDLE void*
//#define INPUTLOGTAG "Input::::::"
//#define VDLOGTAG "CloudVD::::::"


	typedef struct _InputResultData
	{
		char* 	result;					//识别结果字符串。各个语法结果以“[语法名]=识别结果1;”的形式出现。
		unsigned int recogStatus;					//当前识别状态
	} InputResultData;

	typedef enum _RecogStatus
	{
		VINPUT_ASR_STATUS_IN_PROCESS = 0,    //识别正在进行中
		VINPUT_ASR_STATUS_RETRIEVED,   //还有后续音频
		VINPUT_ASR_STATUS_NO_MATCH,    //最后一块音频
		VINPUT_ASR_STATUS_NO_SPEECH,    //没有发现音频
		VINPUT_ASR_STATUS_COMPLETE,    //识别结束
		VINPUT_ASR_STATUS_DISCERN,    //语音开始识别正在进行中
	} eRecogStatus;


	typedef enum _eInputReturnCode
	{
		VVSDK_SUCCESS = 							0,		// 成功
		VVSDK_WARN_FAILED =							1,		// 部分失败
		VVSDK_ERR_FAILED =							2,		// 失败
		VVSDK_ERR_BUFFER_NOT_ENOUGH =				3,		// 服务器错误（失败-错误缓冲区长度不够）
		VVSDK_ERR_SYSTEM_FAILED =					10001,	// 服务器错误（系统错误）
		VVSDK_ERR_INVALID_PARAM =					10002,	// 无效的参数
		VVSDK_ERR_NO_SUCH_GRAMMAR_SERVER = 			10003,	// 无可用的语法服务器
		VVSDK_ERR_REQUEST_ALREADY_EXISTED = 		10004,	// 识别请求已存在
		VVSDK_ERR_REQUEST_NO_EXISTED = 				10005,	// 识别请求不存在
		VVSDK_ERR_REQUEST_TIMEOUT = 				10006,	// 识别请求超时
		VVSDK_ERR_REQUEST_INVALID_STATUS = 			10007,	// 识别请求状态无效
		VVSDK_ERR_ALREADY_LAST_SPEECH = 			10008,	// 已经存在最后音频
		VVSDK_ERR_NET_ACCESS_FAILED = 				10009,	// 服务器错误（网络访问失败）
		VVSDK_ERR_WRONG_GRAMMAR = 					10010,	// 错误的语法名
		VVSDK_WARN_SPEECH_NO_COMPLETE = 			11001,	// 结果未完成
		VVSDK_WARN_PART_GRAMMAR_SERVER_UNAVAILABLE = 11002,	// 服务器错误（有部分语法服务器不可用）
		VVSDK_WARN_BUFFER_NOT_ENOUGH = 				11003,	// 服务器错误（缓冲器长度不够）
		VVSDK_WARN_BUFFER_NOT_ENOUGH2 =				12000,  // 服务器错误（缓冲区长度不够）
		VVSDK_ERR_WRONG_STATE =						13000,	// 错误的状态
		VVSDK_ERR_AUTH_FAILED =						13001,	// 鉴权失败
		VVSDK_ERR_WRONG_VERSION =					13002,	// 版本错误
		VVSDK_ERR_DECODE_FAILED =					13003,	// 数据不能解压
		VVSDK_ERR_SEND_INVALID =					13004,	// 发送的数据无效
		VVSDK_ERR_OUT_AUDIO =						13005,	// 音频超长

		VVSDK_ERR_LIBRARY_UNINIT =					90001,	// 语音识别库尚未初始化
		VVSDK_ERR_LIBRARY_REINIT =					90002,	// 语音识别库重复初始化
		VVSDK_ERR_VOICE_DATA_NULL = 				90003,	// 发送的语音数据为空
		VVSDK_ERR_SDK_NOMEMORY =					90004,	// 本地没有足够的内存
		VVSDK_ERR_INTERFACE_INVALID_PARAM = 		90005,	// 本地接口参数输入无效
		VVSDK_ERR_ENCRYPT_FAILED =					90006,	// 加密失败
		VVSDK_ERR_INVALID_RESP_XML =				90007,	// 无效的服务器返回XML
		VVSDK_ERR_HTTP_NOT_STATUS_OK =				90008,	// 返回错误的http状态
		VVSDK_ERR_LIBRARY_UNSTARTSESSION =			90009,	// 语音SESSION尚未初始化
		VVSDK_ERR_LIBRARY_RESTARTSESSION =			90010,	// 语音SESSION重复初始化
	
		VVSDK_ERR_CANCEL = 							90011,	// 用户取消操作
	    VVSDK_ERR_RECEIVEAUDIO = 					90012,	// 没有返回音频
	    VVSDK_ERR_RECEIVEDATA = 					90013,	// 没有返回数据
	    VVSDK_ERR_SILK_ENCODE_ERROR = 				90014,	// 音频压缩出错
	    VVSDK_ERR_SILK_DECODE_ERROR = 				90015,	// 音频解压缩出错
	    VVSDK_ERR_ERROR_SERVEURL = 					90016,	// 服务器地址为空
	    VVSDK_ERR_APPKEY_ISNULL = 					90017,	// 语音识别引擎appkey为空
	    VVSDK_ERR_APPKEY_INVALID = 					90018,	// 语音识别引擎appkey无效
	    VVSDK_ERR_BEGINRECORD_ERROR = 				90019,	// 录音启动失败
	
	    VVSDK_ERR_INVALID_URL =		 				90020,	// 无效的URL

		VVSDK_ERR_UNKNOWN = 						90100,	// SDK未知错误

		VVSDK_ERR_HTTP_UNKNOWN_ERROR =				91001,	// HTTP错误（未知网络错误--Unknown error）
		VVSDK_ERR_HTTP_INVALID_HANDLE =				91002,	// HTTP错误（无效的HTTP会话对象--an Invalid handle or possible bad pointer was passed to a function）
		VVSDK_ERR_HTTP_NO_MEMORY =					91003,	// HTTP错误（内存不足，分配内存失败--Buffer too small or a failure while in memory allocation）
		VVSDK_ERR_HTTP_SOCKET_INVALID =				91004,	// HTTP错误（无效的Socket对象--an attempt to use an invalid socket handle was made）
		VVSDK_ERR_HTTP_SOCKET_CANT_SET =			91005,	// HTTP错误（设置Socket属性失败--Can't send socket parameters）
		VVSDK_ERR_HTTP_SOCKET_RESOLVE =				91006,	// HTTP错误（解析域名失败--Error while resolving host name）
		VVSDK_ERR_HTTP_SOCKET_CONNECT =				91007,	// HTTP错误（连接服务器失败--Error while connecting to the remote server）
		VVSDK_ERR_HTTP_SOCKET_TIME_OUT =			91008,	// HTTP错误（网络超时--socket time out error）
		VVSDK_ERR_HTTP_SOCKET_RECV =				91009,	// HTTP错误（接收网络数据失败--Error while receiving data）
		VVSDK_ERR_HTTP_SOCKET_SEND =				91010,	// HTTP错误（发送网络数据失败--Error while sending data）
		VVSDK_ERR_HTTP_HEADER_RECV =				91011,	// HTTP错误（接收HTTP header失败--Error while receiving the remote HTTP headers）
		VVSDK_ERR_HTTP_HEADER_NOT_FOUND =			91012,	// HTTP错误（HTTP header内容异常--Could not find element within header）
		VVSDK_ERR_HTTP_HEADER_BIG_CLUE =			91013,	// HTTP错误（Http header key过长--The headers search clue was too large for the internal API buffer）
		VVSDK_ERR_HTTP_HEADER_NO_LENGTH =			91014,	// HTTP错误（未指定content length和chunk--No content length was specified for the outgoing data. the caller should specify chunking mode in the session creation）
		VVSDK_ERR_HTTP_CHUNK_TOO_BIG =				91015,	// HTTP错误（chunk过大--The HTTP chunk token that was received from the server was too big and possibly wrong）
		VVSDK_ERR_HTTP_AUTH_HOST =					91016,	// HTTP错误（身份认证失败--Could not authenticate with the remote host）
		VVSDK_ERR_HTTP_AUTH_PROXY =					91017,	// HTTP错误（代理认证失败--Could not authenticate with the remote proxy）
		VVSDK_ERR_HTTP_BAD_VERB =					91018,	// HTTP错误（无效的HTTP 方法--Bad or not supported HTTP verb was passed to a function）
		VVSDK_ERR_HTTP_LONG_INPUT =					91019,	// HTTP错误（无效的输入参数长度--a function received a parameter that was too large）
		VVSDK_ERR_HTTP_BAD_STATE =					91020,	// HTTP错误（HTTP会话内部状态错误--The session state prevents the current function from proceeding）
		VVSDK_ERR_HTTP_CHUNK =						91021,	// HTTP错误（无法获取chunk长度--Could not parse the chunk length while in chunked transfer）
		VVSDK_ERR_HTTP_BAD_URL =					91022,	// HTTP错误（无效的URL--Could not parse curtail elements from the URL (such as the host name, HTTP prefix act')）
		VVSDK_ERR_HTTP_BAD_HEADER =					91023,	// HTTP错误（无效的返回HTTP header--Could not detect key elements in the received headers）
		VVSDK_ERR_HTTP_BUFFER_RSIZE =				91024,	// HTTP错误（分配内存失败--Error while attempting to resize a buffer）
		VVSDK_ERR_HTTP_BAD_AUTH =					91025,	// HTTP错误（SDK不支持的身份验证模式--Authentication schema is not supported）
		VVSDK_ERR_HTTP_AUTH_MISMATCH =				91026,	// HTTP错误（服务端不支持的身份验证模式--The selected authentication schema does not match the server response）
		VVSDK_ERR_HTTP_NO_DIGEST_TOKEN =			91027,	// HTTP错误（摘要认证失败--an element was missing while parsing the digest authentication challenge）
		VVSDK_ERR_HTTP_NO_DIGEST_ALG =				91028,	// HTTP错误（摘要认证失败--Digest algorithem could be MD5 or MD5-sess other types are not supported）
		VVSDK_ERR_HTTP_SOCKET_BIND =				91029,	// HTTP错误（绑定Socket失败--Binding error）
		VVSDK_ERR_HTTP_TLS_NEGO =					91030,	// HTTP错误（SSL错误--Tls negotiation error）
		VVSDK_ERR_HTTP_NOT_IMPLEMENTED =			91064,	// HTTP错误（不支持的HTTP特性--Feature is not (yet) implemented）
		VVSDK_ERR_HTTP_PREBUFFER_SMALL =			91065,	// HTTP错误（缓冲区长度不够--Predefine buffer too small）
		VVSDK_ERR_HTTP_INBUFFER_SMALL =				91066,	// HTTP错误（输入的内存参数长度不够--Input param buffer too small）
	} eInputReturnCode;

	VCYBERVOICESDK_API eInputReturnCode  VInputInit(const char* configs);
	VCYBERVOICESDK_API eInputReturnCode  VInputSessionBegin(const char* params, SESSION_HANDLE *handle);
	VCYBERVOICESDK_API eInputReturnCode  VInputPostData(SESSION_HANDLE handle, const void* waveData, unsigned int dataLen, int dataStatus, int* recogStatus);
	VCYBERVOICESDK_API eInputReturnCode  VInputGetResult(SESSION_HANDLE handle, InputResultData **resultData);
	VCYBERVOICESDK_API eInputReturnCode  VInputGetDetailError(SESSION_HANDLE handle, char **DetailError);
	VCYBERVOICESDK_API eInputReturnCode  VInputEndSession(SESSION_HANDLE handle);
	VCYBERVOICESDK_API eInputReturnCode  VInputFini();


	/**
	 * CloudVDSDK params
	 */


	typedef enum _eReturnCode
	{
		CLOUDVD_SUCCESS = 0,							// 成功
		CLOUDVD_ERR_SYSTEM_FAILED = 10001,				// 系统错误
		CLOUDVD_ERR_INVALID_PARAM = 10002,				// 无效的参数
		CLOUDVD_ERR_NO_SUCH_GRAMMAR_SERVER = 10003,		// 无可用的语法服务器
		CLOUDVD_ERR_SESSION_ALREADY_EXISTED = 10004,	// 识别请求已存在
		CLOUDVD_ERR_SESSION_NO_EXISTED = 10005,			// 识别请求不存在
		CLOUDVD_ERR_SESSION_TIMEOUT = 10006,			// 识别请求超时
		CLOUDVD_ERR_SESSION_INVALID_STATUS = 10007,		// 识别请求状态无效
		CLOUDVD_ERR_ALREADY_LAST_SPEECH = 10008,		// 已经存在最后音频
		CLOUDVD_ERR_NET_ACCESS_FAILED = 10009,			// 网络访问失败
		CLOUDVD_ERR_REQUEST_ALREADY_TERMINATED = 10010,	// 识别已被强制关闭

		CLOUDVD_ERR_LIBRARY_UNINIT = 10011,				// 语音识别库尚未初始化

		CLOUDVD_WARN_SPEECH_NO_COMPLETE = 11001,		// 结果未完成
		CLOUDVD_WARN_PART_GRAMMAR_SERVER_UNAVAILABLE = 11002,// 有部分语法服务器不可用
		CLOUDVD_ERR_WRONG_STATE = 20000,				// 错误的状态
		CLOUDVD_ERR_AUTH_FAILED = 20001,				// 鉴权失败
		CLOUDVD_ERR_WRONG_VERSION = 20002,				// 版本错误
		CLOUDVD_ERR_COMPRESS_FAILED = 20003,			// 数据不能压缩
		CLOUDVD_ERR_DECOMPRESS_FAILED = 20004,			// 数据不能解压
		CLOUDVD_ERR_SEND_INVALID = 20005,				// 发送的数据无效
		CLOUDVD_ERR_OUT_AUDIO = 20006,					// 发送的数据无效
		CLOUDVD_ERR_NULL_POINTER = 21000,				// 空指针
		CLOUDVD_ERR_NOMEMORY = 21001,					// 内存不足
		CLOUDVD_ERR_INVALID_XML = 21002,				// 无效的XML
		CLOUDVD_ERR_FAIL_ENCODE = 21003,				// 编码失败
		CLOUDVD_ERR_INVALID_URL = 21004,				// 无效的URL
		CLOUDVD_ERR_OPEN_FAIL = 21005,					// 打开失败
		CLOUDVD_ERR_CONN_TIMEOUT = 21006,				// 连接超时
		CLOUDVD_ERR_SEND_FAIL = 21007,					// 发送失败
		CLOUDVD_ERR_SET_FAIL = 21008,					// 写入失败
		CLOUDVD_ERR_SOCKET_FAIL = 21009,				// 网络异常
	} eReturnCode;



	typedef enum _eAudioStatus
	{
		AUDIO_STATUS_FIRST = 1,		// first audio data section
		AUDIO_STATUS_CONTINUE,		// midile audio data section
		AUDIO_STATUS_LAST,			// last audio data section
	} eAudioStatus;


	typedef struct _ResultData
	{
		char* 	result;					//识别结果json字符串
		char*	audio;					//返回的音频
		int		audioLenth;				//音频长度
		char*   uniqueSessionID;		//本次识别唯一标识id（vin+时间戳构成，保证唯一且能正常提取详细时间信息）
	} ResultData;
	typedef struct _SearchResult
	{
		int 	type;					//查询返回的json数据类型
		int		resultLength;				//音频长度
		char*   result;		//查询返回的json字符串
	} SearchResult;

	/**
	* 描述：
	*		初始化云连接库。
	*		使用该库之前（例如程序启动）首先调用该接口。
	*		如参数发生变化，需要先调用CloudVDFini之后，再调用该接口。
	*
	* 参数：
	*		【in】const char* configs：连接参数列表，为Json格式的字符串。
	*
	* 返回：函数执行结果。详见eReturnCode定义。
	*
	**/
	VCYBERVOICESDK_API eReturnCode CloudVDInit(const char * configs);
	typedef eReturnCode(*p_CloudVDInit) (const char * configs);


	/**
	* 描述：
	*		开启一次语音识别。
	*		每次开始语音识别时进行调用。设置此次语音识别所用参数。
	*
	* 参数：
	*		【in】 const char* params：参数列表，为Json格式的字符串。
	*		【out】 SESSION_HANDLE & handle：会话启动成功时，返回会话句柄。
	*
	* 返回：函数执行结果。详见eReturnCode定义。
	*
	**/
	VCYBERVOICESDK_API eReturnCode CloudVDStartSession(const char * params, SESSION_HANDLE * handle);
	typedef eReturnCode(*p_CloudVDStartSession) (const char * params, SESSION_HANDLE * handle);


	/**
	* 描述：
	*		推送VR语音数据到云端。
	*
	* 参数：
	*		【in】 SESSION_HANDLE handle：CloudVDStartSession返回的会话句柄
	*		【in】 const void * waveData：执行语音数据的指针
	*		【in】 unsigned int dataLen：语音数据的长度
	*		【in】 int dataStatus：音频数据块状态
	*
	* 返回：函数执行结果。详见eReturnCode定义。
	*
	**/
	VCYBERVOICESDK_API eReturnCode CloudVDPostData(SESSION_HANDLE handle, const void * waveData, unsigned int dataLen, eAudioStatus dataStatus);
	typedef eReturnCode(*p_CloudVDPostData) (SESSION_HANDLE handle, const void * waveData, unsigned int dataLen, eAudioStatus dataStatus);


	/**
	* 描述：
	*		获取语音识别的结果。
	*
	* 参数：
	*		【in】 SESSION_HANDLE handle：CloudVDStartSession返回的会话句柄
	*		【out】ResultData **resultData：获得识别结果结构体指针。
	*				接口返回后，调用者应该在再次调用SDK任何接口之前，
	*				立刻缓存返回结构体数据，不可缓存结构体指针。
	*				结构体所有权归SDK所有。调用者只能读取其中数据，
	*				不能修改数据、释放内存。
	*
	* 返回：函数执行结果。详见eReturnCode定义。
	*
	**/
	VCYBERVOICESDK_API eReturnCode CloudVDGetResult(SESSION_HANDLE handle, ResultData ** resultData);
	typedef eReturnCode(*p_CloudVDGetResult) (SESSION_HANDLE handle, ResultData ** resultData);

	/**
	* 描述：
	*		获取搜索信息的结果。
	*
	* 参数：
	*		【in】 const char * params：json 格式的参数
	*		【out】SearchResult **SearchResult：获得查询信息结构体指针。
	*				接口返回后，调用者应该在再次调用SDK任何接口之前，
	*				立刻缓存返回结构体数据，不可缓存结构体指针。
	*				结构体所有权归SDK所有。调用者只能读取其中数据，
	*				不能修改数据、释放内存。
	*
	* 返回：函数执行结果。详见eReturnCode定义。
	*
	**/
	VCYBERVOICESDK_API eReturnCode CloudVDSearchInfo(const char * params, SearchResult ** SearchResult);
	typedef eReturnCode(*p_CloudVDSearchInfo) (const char * params, SearchResult ** SearchResult);
	/**
	* 描述：
	*		结束一次语音识别，并将本次识别所用资源释放。
	*
	* 参数：
	*		【in】 SESSION_HANDLE handle：CloudVDStartSession返回的会话句柄
	*		【in】 int handle：本次语音识别的句柄
	*
	* 返回：函数执行结果。详见eReturnCode定义。
	*
	**/
	VCYBERVOICESDK_API eReturnCode CloudVDEndSession(SESSION_HANDLE handle);
	typedef eReturnCode(*p_CloudVDEndSession) (SESSION_HANDLE handle);




	/**
	* 描述：
	*		释放云连接库。
	*		不再使用该库时（例如程序退出），调用该接口。
	*
	* 参数：
	*
	* 返回：函数执行结果。详见eReturnCode定义。
	*
	**/
	VCYBERVOICESDK_API eReturnCode CloudVDFini();
	typedef eReturnCode(*p_CloudVDFini) ();


#ifdef __cplusplus
}
#endif
