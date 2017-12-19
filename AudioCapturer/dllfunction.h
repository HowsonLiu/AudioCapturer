#ifndef DLLFUNCTION_H
#define DLLFUNCTION_H

#endif // DLLFUNCTION_H

#include <vector>
#include <string>
#include <QString>

//功能接口

/*设置默认文件夹*/
extern "C" __declspec(dllimport) void SetDefaultFolder(const char* path);

/*根据默认路径返回给定名字的绝对路径*/
extern "C" __declspec(dllimport) const char* GetPathByName(const char* name);

/*判断路径文件是否存在*/
extern "C" __declspec(dllimport) bool IsCAFileExist(const char* path);

/*判断列表中是否存在相同的CA实例*/
extern "C" __declspec(dllimport) bool IsCAObjectInList(const char* path);

/*判断实例是否可用，最好在读取数据后使用，而且需要自行判定列表中是否存在实例*/
extern "C" __declspec(dllimport) bool IsCAObjectValid(const char* path);

/*更新列表,已经完成创建实例和读取数据*/
extern "C" __declspec(dllimport) void UpdateMap();

/*创建实例接口，不提供判断功能，配合文件是否存在和实例是否存在使用*/
extern "C" __declspec(dllimport) void NewCAObject(const char* path);

/*从文件中读取数据，最好在创建实例后使用*/
extern "C" __declspec(dllimport) void ReadDataFromCAFile(const char* path);

/*删除文件，配合文件是否存在使用*/
extern "C" __declspec(dllimport) void DeleteCAFile(const char* path);

/*移除实例，配合实例是否存在使用*/
extern "C" __declspec(dllimport) void RemoveCAObject(const char* path);

/*设置倒计时（毫秒）*/
extern "C" __declspec(dllimport) void SetCountdownToStart(long sec, bool* presult);

/*设置持续时间*/
extern "C" __declspec(dllimport) void SetCountdownToStop(long sec, bool* presult);

/*设置自定义格式*/
extern "C" __declspec(dllimport) void SetCustomFormat(const unsigned short wFormatTag, const unsigned short nChannels, const unsigned long nSamplesPerSec, const unsigned long nAvgBytePerSec, const unsigned short nBlockAlign, const unsigned short wBitsPerSample, const unsigned short cbsize, const unsigned short Samples, const unsigned long dwChannelMask, const char* SubFormatstring);

/*取消自定义*/
extern "C" __declspec(dllimport) void CancelCustomFormat();

/*取消倒计时*/
extern "C" __declspec(dllimport) void CancelCountdown();

/*取消持续时间*/
extern "C" __declspec(dllimport) void CancelDuration();

/*开始录制，最好在创建实例后使用*/
extern "C" __declspec(dllimport) bool CaptureAudio(const char* path, const char* captureformat, bool* stopcapture);

/*开始试听,最好确认实例是否正确可用*/
extern "C" __declspec(dllimport) bool RenderAudio(const char* path, bool* stoprender);

/*转化为可播放格式，需要确认实例是否可用*/
extern "C" __declspec(dllimport) bool Transform(const char* path, const char* format, const char* outputpath);

//显示接口

/*查看默认保存文件夹*/
extern "C" __declspec(dllimport) const char* GetDefaultFolder();

/*获取列表中所有实例的路径*/
extern "C" __declspec(dllimport) char** GetListNames();

/*获取所有录制格式字符串*/
extern "C" __declspec(dllimport) char** GetCaptureFormatNames();

/*获取所有保存格式字符串*/
extern "C" __declspec(dllimport) char** GetTransformFormatNames();

/*查看实例名字*/
extern "C" __declspec(dllimport) const char* GetCAObjectName(const char* path);

/*查看实例大小*/
extern "C" __declspec(dllimport) const int GetCAObjectSize(const char* path);

/*查看实例播放时间*/
extern "C" __declspec(dllimport) const int GetCAObjectDuration(const char* path);

/*查看实例创建时间*/
extern "C" __declspec(dllimport) const char* GetCAObjectCreateTime(const char* path);

/*查看实例声道数*/
extern "C" __declspec(dllimport) int GetCAObjectChannels(const char* path);

/*查看实例采样率*/
extern "C" __declspec(dllimport) int GetCAObjectSamplePerSec(const char* path);

/*查看倒计时开始时间*/
extern "C" __declspec(dllimport) long* GetCountdownToStart();

/*查看倒计时结束时间*/
extern "C" __declspec(dllimport) long* GetCountdownToStop();

/*查看已运行时间*/
extern "C" __declspec(dllimport) long* GetRunTime();

std::vector<std::string> ChangeCharppToVector(char** cpp);

QString ChangeCharpToQString(const char* pchar);
