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

/*由于dll中最好不要导出类，所以我使用字符串作为dll与gui之间通讯的桥梁，以下所有的参数path都是实际文件的绝对路径*/

//功能接口

/*设置默认文件夹。注意默认文件夹的作用仅限与新建和搜索，在查找的时候不会涉及*/
extern "C" __declspec(dllexport) void SetDefaultFolder(const char* path) {
	CAMainProcess::GetInstance()->SetFolder(path);
}

/*根据默认路径返回给定名字的绝对路径*/
extern "C" __declspec(dllexport) const char* GetPathByName(const char* name) {
	string* ppath = new string(CAMainProcess::GetInstance()->GetPath(name));
	return ppath->c_str();
}

/*判断路径文件是否存在*/
extern "C" __declspec(dllexport) bool IsCAFileExist(const char* path) {
	return CAMainProcess::GetInstance()->IsCAFileExist(path);
}

/*判断列表中是否存在相同的CA实例*/
extern "C" __declspec(dllexport) bool IsCAObjectInList(const char* path) {
	return CAMainProcess::GetInstance()->IsCAObjectInMap(path);
}

/*判断实例是否可用，最好在读取数据后使用，而且需要自行判定列表中是否存在实例*/
extern "C" __declspec(dllexport) bool IsCAObjectValid(const char* path) {
	return CAMainProcess::GetInstance()->IsCAObjectValid(path);
}

/*更新列表,已经完成创建实例和读取数据*/
extern "C" __declspec(dllexport) void UpdateMap() {
	CAMainProcess::GetInstance()->UpdateCAFilesMap();
}

/*创建实例接口，不提供判断功能，配合文件是否存在和实例是否存在使用*/
extern "C" __declspec(dllexport) void NewCAObject(const char* path) {
	CAMainProcess::GetInstance()->NewCAObject(path);
}

/*从文件中读取数据，最好在创建实例后使用*/
extern "C" __declspec(dllexport) void ReadDataFromCAFile(const char* path) {
	CAMainProcess::GetInstance()->ReadCAFile(path);
}

/*删除文件，配合文件是否存在使用*/
extern "C" __declspec(dllexport) void DeleteCAFile(const char* path) {
	CAMainProcess::GetInstance()->DeleteCAFile(path);
}

/*移除实例，配合实例是否存在使用*/
extern "C" __declspec(dllexport) void RemoveCAObject(const char* path) {
	CAMainProcess::GetInstance()->RemoveCAObject(path);
}

/*设置倒计时（毫秒）*/
extern "C" __declspec(dllexport) void SetCountdownToStart(long sec, bool* presult) {
	CAMainProcess::GetInstance()->SetCountdownToStart(sec, presult);
}

/*设置持续时间*/
extern "C" __declspec(dllexport) void SetCountdownToStop(long sec, bool* presult) {
	CAMainProcess::GetInstance()->SetCountdownToStop(sec, presult);
}

/*设置自定义格式*/
extern "C" __declspec(dllexport) void SetCustomFormat(const unsigned short wFormatTag, const unsigned short nChannels, const unsigned long nSamplesPerSec, const unsigned long nAvgBytePerSec, const unsigned short nBlockAlign, const unsigned short wBitsPerSample, const unsigned short cbsize, const unsigned short Samples, const unsigned long dwChannelMask, const char* SubFormatstring) {
	GUID SubFormat = { 0 };
	CLSIDFromString((LPCOLESTR)SubFormatstring, (LPCLSID)&SubFormat);
	CAMainProcess::GetInstance()->SetCustomWAVEFORMATEX(wFormatTag, nChannels, nSamplesPerSec, nAvgBytePerSec, nBlockAlign, wBitsPerSample, cbsize, Samples, dwChannelMask, SubFormat);
}

/*取消自定义*/
extern "C" __declspec(dllexport) void CancelCustomFormat() {
	CAMainProcess::GetInstance()->DeleteCustomFormat();
}

/*取消倒计时*/
extern "C" __declspec(dllexport) void CancelCountdown() {
	CAMainProcess::GetInstance()->DeleteCountdownToStart();
}

/*取消持续时间*/
extern "C" __declspec(dllexport) void CancelDuration() {
	CAMainProcess::GetInstance()->DeleteCoundownToStop();
}

/*开始录制，最好在创建实例后使用*/
extern "C" __declspec(dllexport) bool CaptureAudio(const char* path, const char* captureformat, bool* stopcapture) {
	return SUCCEEDED(CAMainProcess::GetInstance()->StartCapture(path, captureformat, stopcapture));
}

/*开始试听,最好确认实例是否正确可用*/
extern "C" __declspec(dllexport) bool RenderAudio(const char* path, bool* stoprender) {
	return SUCCEEDED(CAMainProcess::GetInstance()->StartRender(path, stoprender));
}

/*转化为可播放格式，需要确认实例是否可用*/
extern "C" __declspec(dllexport) bool Transform(const char* path, const char* format, const char* outputpath) {
	return SUCCEEDED(CAMainProcess::GetInstance()->Output(path, format, outputpath));
}

//显示接口

/*查看默认保存文件夹*/
extern "C" __declspec(dllexport) const char* GetDefaultFolder() {
	string* pfolder = new string(CAMainProcess::GetInstance()->GetFolder());
	return pfolder->c_str();
}

/*获取列表中所有实例的路径*/
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

/*获取所有录制格式字符串*/
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

/*获取所有保存格式字符串*/
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

/*查看实例名字*/
extern "C" __declspec(dllexport) const char* GetCAObjectName(const char* path) {
	string* pname = new string(CAMainProcess::GetInstance()->GetCAObjectName(path));
	return pname->c_str();
}

/*查看实例大小*/
extern "C" __declspec(dllexport) const int GetCAObjectSize(const char* path) {
	return CAMainProcess::GetInstance()->GetCAObjectSize(path);
}

/*查看实例播放时间*/
extern "C" __declspec(dllexport) const int GetCAObjectDuration(const char* path) {
	return CAMainProcess::GetInstance()->GetCAObjectDuration(path);
}

/*查看实例创建时间*/
extern "C" __declspec(dllexport) const char* GetCAObjectCreateTime(const char* path) {
	string* ptime = new string(CAMainProcess::GetInstance()->GetCAObjectCreateTime(path));
	return ptime->c_str();
}

/*查看实例声道数*/
extern "C" __declspec(dllexport) int GetCAObjectChannels(const char* path) {
	return CAMainProcess::GetInstance()->GetCAObjectChannels(path);
}

/*查看实例采样率*/
extern "C" __declspec(dllexport) int GetCAObjectSamplePerSec(const char* path) {
	return CAMainProcess::GetInstance()->GetCAObjectSamplePerSec(path);
}

/*查看倒计时开始时间*/
extern "C" __declspec(dllexport) long* GetCountdownToStart() {
	return CAMainProcess::GetInstance()->GetCountdownToStart();
}

/*查看倒计时结束时间*/
extern "C" __declspec(dllexport) long* GetCountdownToStop() {
	return CAMainProcess::GetInstance()->GetCountdownToStop();
}

/*查看已运行时间*/
extern "C" __declspec(dllexport) long* GetRunTime() {
	return CAMainProcess::GetInstance()->GetRunTime();
}