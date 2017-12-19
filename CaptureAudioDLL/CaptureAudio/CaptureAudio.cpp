// CaptureAudio.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CAMainProcess.h"
#include "CAFile.h"
#include "CAOutputFormat.h"
#include <string>
#include <thread>
#include <iostream>
#include "Reflection.h"

/*����dll����ò�Ҫ�����࣬������ʹ���ַ�����Ϊdll��gui֮��ͨѶ���������������еĲ���path����ʵ���ļ��ľ���·��*/

//���ܽӿ�

/*����Ĭ���ļ��С�ע��Ĭ���ļ��е����ý������½����������ڲ��ҵ�ʱ�򲻻��漰*/
extern "C" __declspec(dllexport) void SetDefaultFolder(const char* path) {
	CAMainProcess::GetInstance()->SetFolder(path);
}

/*����Ĭ��·�����ظ������ֵľ���·��*/
extern "C" __declspec(dllexport) const char* GetPathByName(const char* name) {
	string* ppath = new string(CAMainProcess::GetInstance()->GetPath(name));
	return ppath->c_str();
}

/*�ж�·���ļ��Ƿ����*/
extern "C" __declspec(dllexport) bool IsCAFileExist(const char* path) {
	return CAMainProcess::GetInstance()->IsCAFileExist(path);
}

/*�ж��б����Ƿ������ͬ��CAʵ��*/
extern "C" __declspec(dllexport) bool IsCAObjectInList(const char* path) {
	return CAMainProcess::GetInstance()->IsCAObjectInMap(path);
}

/*�ж�ʵ���Ƿ���ã�����ڶ�ȡ���ݺ�ʹ�ã�������Ҫ�����ж��б����Ƿ����ʵ��*/
extern "C" __declspec(dllexport) bool IsCAObjectValid(const char* path) {
	return CAMainProcess::GetInstance()->IsCAObjectValid(path);
}

/*�����б�,�Ѿ���ɴ���ʵ���Ͷ�ȡ����*/
extern "C" __declspec(dllexport) void UpdateMap() {
	CAMainProcess::GetInstance()->UpdateCAFilesMap();
}

/*����ʵ���ӿڣ����ṩ�жϹ��ܣ�����ļ��Ƿ���ں�ʵ���Ƿ����ʹ��*/
extern "C" __declspec(dllexport) void NewCAObject(const char* path) {
	CAMainProcess::GetInstance()->NewCAObject(path);
}

/*���ļ��ж�ȡ���ݣ�����ڴ���ʵ����ʹ��*/
extern "C" __declspec(dllexport) void ReadDataFromCAFile(const char* path) {
	CAMainProcess::GetInstance()->ReadCAFile(path);
}

/*ɾ���ļ�������ļ��Ƿ����ʹ��*/
extern "C" __declspec(dllexport) void DeleteCAFile(const char* path) {
	CAMainProcess::GetInstance()->DeleteCAFile(path);
}

/*�Ƴ�ʵ�������ʵ���Ƿ����ʹ��*/
extern "C" __declspec(dllexport) void RemoveCAObject(const char* path) {
	CAMainProcess::GetInstance()->RemoveCAObject(path);
}

/*���õ���ʱ�����룩*/
extern "C" __declspec(dllexport) void SetCountdownToStart(long sec, bool* presult) {
	CAMainProcess::GetInstance()->SetCountdownToStart(sec, presult);
}

/*���ó���ʱ��*/
extern "C" __declspec(dllexport) void SetCountdownToStop(long sec, bool* presult) {
	CAMainProcess::GetInstance()->SetCountdownToStop(sec, presult);
}

/*�����Զ����ʽ*/
extern "C" __declspec(dllexport) void SetCustomFormat(const unsigned short wFormatTag, const unsigned short nChannels, const unsigned long nSamplesPerSec, const unsigned long nAvgBytePerSec, const unsigned short nBlockAlign, const unsigned short wBitsPerSample, const unsigned short cbsize, const unsigned short Samples, const unsigned long dwChannelMask, const char* SubFormatstring) {
	GUID SubFormat = { 0 };
	CLSIDFromString((LPCOLESTR)SubFormatstring, (LPCLSID)&SubFormat);
	CAMainProcess::GetInstance()->SetCustomWAVEFORMATEX(wFormatTag, nChannels, nSamplesPerSec, nAvgBytePerSec, nBlockAlign, wBitsPerSample, cbsize, Samples, dwChannelMask, SubFormat);
}

/*ȡ���Զ���*/
extern "C" __declspec(dllexport) void CancelCustomFormat() {
	CAMainProcess::GetInstance()->DeleteCustomFormat();
}

/*ȡ������ʱ*/
extern "C" __declspec(dllexport) void CancelCountdown() {
	CAMainProcess::GetInstance()->DeleteCountdownToStart();
}

/*ȡ������ʱ��*/
extern "C" __declspec(dllexport) void CancelDuration() {
	CAMainProcess::GetInstance()->DeleteCoundownToStop();
}

/*��ʼ¼�ƣ�����ڴ���ʵ����ʹ��*/
extern "C" __declspec(dllexport) bool CaptureAudio(const char* path, const char* captureformat, bool* stopcapture) {
	return SUCCEEDED(CAMainProcess::GetInstance()->StartCapture(path, captureformat, stopcapture));
}

/*��ʼ����,���ȷ��ʵ���Ƿ���ȷ����*/
extern "C" __declspec(dllexport) bool RenderAudio(const char* path, bool* stoprender) {
	return SUCCEEDED(CAMainProcess::GetInstance()->StartRender(path, stoprender));
}

/*ת��Ϊ�ɲ��Ÿ�ʽ����Ҫȷ��ʵ���Ƿ����*/
extern "C" __declspec(dllexport) bool Transform(const char* path, const char* format, const char* outputpath) {
	return SUCCEEDED(CAMainProcess::GetInstance()->Output(path, format, outputpath));
}

//��ʾ�ӿ�

/*�鿴Ĭ�ϱ����ļ���*/
extern "C" __declspec(dllexport) const char* GetDefaultFolder() {
	string* pfolder = new string(CAMainProcess::GetInstance()->GetFolder());
	return pfolder->c_str();
}

/*��ȡ�б�������ʵ����·��*/
extern "C" __declspec(dllexport) char** GetListNames() {
	char** nameschar = new char*[100];
	vector<string> namevec = CAMainProcess::GetInstance()->GetMapObjectPaths();
	int i;
	for (i = 0; i < namevec.size(); i++) {
		string* name = new string(namevec[i]);
		nameschar[i] = const_cast<char*>(name->c_str());
	}
	nameschar[i] = nullptr;
	return nameschar;
}

/*��ȡ����¼�Ƹ�ʽ�ַ���*/
extern "C" __declspec(dllexport) char** GetCaptureFormatNames() {
	char** nameschar = new char*[10];
	vector<string> namevec = Reflection_Factory::GetInstance()->GetAllCaptureClassNames();
	int i;
	for (i = 0; i < namevec.size(); i++) {
		string* name = new string(namevec[i]);
		nameschar[i] = const_cast<char*>(name->c_str());
	}
	nameschar[i] = nullptr;
	return nameschar;
}

/*��ȡ���б����ʽ�ַ���*/
extern "C" __declspec(dllexport) char** GetTransformFormatNames() {
	char** nameschar = new char*[20];
	vector<string> namevec = Reflection_Factory::GetInstance()->GetAllOutputClassNames();
	int i;
	for (i = 0; i < namevec.size(); i++) {
		string* name = new string(namevec[i]);
		nameschar[i] = const_cast<char*>(name->c_str());
	}
	nameschar[i] = nullptr;
	return nameschar;
}

/*�鿴ʵ������*/
extern "C" __declspec(dllexport) const char* GetCAObjectName(const char* path) {
	string* pname = new string(CAMainProcess::GetInstance()->GetCAObjectName(path));
	return pname->c_str();
}

/*�鿴ʵ����С*/
extern "C" __declspec(dllexport) const int GetCAObjectSize(const char* path) {
	return CAMainProcess::GetInstance()->GetCAObjectSize(path);
}

/*�鿴ʵ������ʱ��*/
extern "C" __declspec(dllexport) const int GetCAObjectDuration(const char* path) {
	return CAMainProcess::GetInstance()->GetCAObjectDuration(path);
}

/*�鿴ʵ������ʱ��*/
extern "C" __declspec(dllexport) const char* GetCAObjectCreateTime(const char* path) {
	string* ptime = new string(CAMainProcess::GetInstance()->GetCAObjectCreateTime(path));
	return ptime->c_str();
}

/*�鿴ʵ��������*/
extern "C" __declspec(dllexport) int GetCAObjectChannels(const char* path) {
	return CAMainProcess::GetInstance()->GetCAObjectChannels(path);
}

/*�鿴ʵ��������*/
extern "C" __declspec(dllexport) int GetCAObjectSamplePerSec(const char* path) {
	return CAMainProcess::GetInstance()->GetCAObjectSamplePerSec(path);
}

/*�鿴����ʱ��ʼʱ��*/
extern "C" __declspec(dllexport) long* GetCountdownToStart() {
	return CAMainProcess::GetInstance()->GetCountdownToStart();
}

/*�鿴����ʱ����ʱ��*/
extern "C" __declspec(dllexport) long* GetCountdownToStop() {
	return CAMainProcess::GetInstance()->GetCountdownToStop();
}

/*�鿴������ʱ��*/
extern "C" __declspec(dllexport) long* GetRunTime() {
	return CAMainProcess::GetInstance()->GetRunTime();
}