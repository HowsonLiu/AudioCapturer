#pragma once
#include "stdafx.h"
#include "CAFile.h"
#include <vector>
#include <Audioclient.h>
#include <time.h>
#include <map>
#include <thread>

//��󲶻�ʱ��15����
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
	void Start();//������ʱ�߳�
	void Stop();//�޸�presult��ֹͣ�߳�
	void threadjoin();//�߳�join
	void threaddetach();//�߳�detach
	bool IsStarted();//�жϽ����Ƿ���
	void SetTime(long);//����ʱ��
	void SetBool(bool*);//���ý��ָ��
	void Reset();//�ָ�Ĭ������
	clock_t* GetPassTime();//��ȡ�ѹ�ʱ�䣨���룩
	clock_t* GetLeaveTime();//��ȡʣ��ʱ�䣨���룩
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

	//function interface��ͨ������·����ʵ�����в���
	void SetFolder(const string);//���ñ���·��
	void UpdateCAFilesMap();//�����б�
	string GetPath(const string) const;//ͨ�����ֺ�Ĭ�ϱ���·�����������·��
	bool IsCAFileExist(const string);//�ж��ļ��Ƿ����
	bool IsCAObjectInMap(const string);//�ж�ʵ���Ƿ����
	bool IsCAObjectValid(const string);//�ж�ʵ���Ƿ���ȷ
	void NewCAObject(const string);//����ʵ��
	void ReadCAFile(const string);//ָ��ʵ�����ļ��ж�ȡ����
	void RemoveCAObject(const string);//�Ƴ�ʵ��
	bool DeleteCAFile(const string);//ɾ���ļ�
	void SetCustomWAVEFORMATEX(const WORD, const WORD, const DWORD, const DWORD, const WORD, const WORD, const WORD, const WORD, const DWORD, const GUID);//�Զ�����Ƶ��ʽ
	void SetCountdownToStart(long, bool*);//���õ���ʱ��ʼ��ʱ��ͽ��ָ��
	void SetCountdownToStop(long, bool*);//���õ���ʱ������ʱ��ͽ��ָ��
	void DeleteCustomFormat();//ȡ���Զ����ʽ
	void DeleteCountdownToStart();//ȡ������ʱ��ʼ
	void DeleteCoundownToStop();//ȡ������ʱ����

	HRESULT StartCapture(const string, const string, bool*);//�����ļ�����ʼ¼��
	HRESULT StartRender(const string, bool*);//ֱ�Ӵ��ļ�������
	HRESULT Output(const string, const string, const string);//ת��Ϊ�ɲ��Ÿ�ʽ

	//information interface��Ҳ��ͨ������·�����в���
	string GetFolder() const;//��ȡĬ�ϱ���·��
	string GetCAObjectName(const string);//��ȡʵ������
	int GetCAObjectSize(const string);//��ȡʵ����С
	int GetCAObjectDuration(const string);//��ȡʵ������ʱ��
	int GetCAObjectChannels(const string);//��ȡʵ��������
	int GetCAObjectSamplePerSec(const string);//��ȡʵ��������
	string GetCAObjectCreateTime(const string);//��ȡʵ������ʱ��
	vector<string> GetMapObjectPaths();//��ȡ�б�������ʵ��·��
	long* GetCountdownToStart();//��ȡ��ʼ����ʱ
	long* GetCountdownToStop();//��ȡ��������ʱ
	long* GetRunTime();//��ȡ��¼��ʱ��
};