#pragma once
#include "stdafx.h"
#include "CAFile.h"
#include <vector>
#include <Audioclient.h>
#include <time.h>
#include <map>
#include <thread>

//最大捕获时间15分钟
#define MAXCAPTURETIME 9000000

using namespace std;

class CATime {
public:
	CATime(long, bool*);
private:
	clock_t requiretime;
	clock_t currenttime;
	clock_t starttime;
	clock_t passtime;
	clock_t leavetime;
	thread* pthread;
	bool* presult;

	void CountDown();

public:
	void Start();//开启计时线程
	void Stop();//修改presult以停止线程
	void threadjoin();//线程join
	void threaddetach();//线程detach
	bool IsStarted();//判断进程是否开启
	void SetTime(long);//设置时间
	void SetBool(bool*);//设置结果指针
	void Reset();//恢复默认设置
	clock_t* GetPassTime();//获取已过时间（毫秒）
	clock_t* GetLeaveTime();//获取剩余时间（毫秒）
};

class CAMainProcess {
	friend class CAObject;
private:
	CAMainProcess() :pCustomFormat(nullptr), pcountdowntostart(nullptr), pcountdowntostop(nullptr), pruntime(new CATime(MAXCAPTURETIME, nullptr)), DefaultCAFilesFolderPath("D:\\CAFile") {};
	CAMainProcess(const CAMainProcess&) {};
	CAMainProcess& operator=(const CAMainProcess&) {};
	static CAMainProcess* instance;

	map<string, CAObject*> LocalCAFiles;
	string DefaultCAFilesFolderPath;
	const string suffix = ".ca";
	WAVEFORMATEX* pCustomFormat;
	CATime* pcountdowntostart;
	CATime* pcountdowntostop;
	CATime* pruntime;
	
	CAObject* SelectCAObject(const string);

public:
	//singleton
	static CAMainProcess* GetInstance();

	//function interface，通过绝对路径对实例进行操作
	void SetFolder(const string);//设置保存路径
	void UpdateCAFilesMap();//更新列表
	string GetPath(const string) const;//通过名字和默认保存路径推算出绝对路径
	bool IsCAFileExist(const string);//判断文件是否存在
	bool IsCAObjectInMap(const string);//判断实例是否存在
	bool IsCAObjectValid(const string);//判断实例是否正确
	void NewCAObject(const string);//创建实例
	void ReadCAFile(const string);//指定实例从文件中读取数据
	void RemoveCAObject(const string);//移除实例
	bool DeleteCAFile(const string);//删除文件
	void SetCustomWAVEFORMATEX(const WORD, const WORD, const DWORD, const DWORD, const WORD, const WORD, const WORD, const WORD, const DWORD, const GUID);//自定义音频格式
	void SetCountdownToStart(long, bool*);//设置倒计时开始的时间和结果指针
	void SetCountdownToStop(long, bool*);//设置倒计时结束的时间和结果指针
	void DeleteCustomFormat();//取消自定义格式
	void DeleteCountdownToStart();//取消倒计时开始
	void DeleteCoundownToStop();//取消倒计时结束

	HRESULT StartCapture(const string, const string, bool*);//创建文件，开始录音
	HRESULT StartRender(const string, bool*);//直接从文件中试听
	HRESULT Output(const string, const string, const string);//转换为可播放格式

	//information interface，也是通过绝对路径进行操作
	string GetFolder() const;//获取默认保存路径
	string GetCAObjectName(const string);//获取实例名字
	int GetCAObjectSize(const string);//获取实例大小
	int GetCAObjectDuration(const string);//获取实例播放时长
	int GetCAObjectChannels(const string);//获取实例声道数
	int GetCAObjectSamplePerSec(const string);//获取实例采样率
	string GetCAObjectCreateTime(const string);//获取实例创建时间
	vector<string> GetMapObjectPaths();//获取列表中所有实例路径
	long* GetCountdownToStart();//获取开始倒计时
	long* GetCountdownToStop();//获取结束倒计时
	long* GetRunTime();//获取已录制时间
};