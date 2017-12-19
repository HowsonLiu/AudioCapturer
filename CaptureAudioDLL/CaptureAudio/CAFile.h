#pragma once
#include "stdafx.h"
#include "CAOutputFormat.h"
#include "CAMainProcess.h"
#include <string>
#include <Audioclient.h>

/*
WORD 占 2字节
DWORD 占 4 字节
WAVEFORMATEX 有5个WORD和2个DWORD 占 18字节
union WORD 占 2 字节
GUID SubFormat 16 Byte
所以 WAVEFORMATEXTENSIBLE 有1个WAVEFORMATEX和1个WORD和1个DWORD和1个GUID 占 40字节
*/
#define WAVEFORMATEXTENSIBLELENGTH 40

using namespace std;

class CAObject {

	//操作接口
	friend HRESULT StartCapture(CAObject*, CACaptureFormatBase*, WAVEFORMATEX*, bool*);//开始录音
	friend HRESULT StartRender(CAObject*, bool*);//开始试听

public:
	CAObject(const string);
	~CAObject() = default;

	//用于显示
	string GetName() const;//获取名字
	int GetAudioLength() const;//获取播放时长
	int GetFileSize() const;//获取文件大小
	string GetFileCreateTime() const;//获取文件创建时间
	int GetChannels() const;//获取声道数
	int GetSamplePerSec() const;//获取采样率

	//转换格式专用
	const WAVEFORMATEX* GetWAVEFORMATEX() const;//获取音频格式
	const void* GetPCMBuffer() const;//获取pcm数据
	UINT32 GetPCMLength() const;//获取pcm数据长度

	//用于操作

	string GetPath() const;//获取路径
	bool IsCAFileExist();//检查路径文件是否存在
	bool IsCAObjectValid();//检查类属性是否正确可用
	HRESULT ReadFileStream();//将文件内容写入类中
	HRESULT SaveAsOtherFormat(OutputFormatFactory*,const string);//保存为其他可播放文件
	bool DeleteCAFile();//删除文件

private:
	string filename;
	string filepath;

	WAVEFORMATEX* pFormat;
	void* pPCMBuffer;
	void* pReadBufferPoint;
	UINT32 PCMLength;
	UINT32 ReadIndex;
	FILE* pfilestream;

	void GetNameFromPath();//推算处名字

	HRESULT GetWAVEFORMATEX(WAVEFORMATEX*);//接受pcm数据之前获取默认格式
	HRESULT ChangeToCostomWAVEFORMATEX(WAVEFORMATEX*, WAVEFORMATEX*);//接受自定义音频格式
	HRESULT OpenFileStream();//开启文件流
	HRESULT CopyData(BYTE*, UINT32, BOOL*);//向文件流写入pcm数据
	HRESULT CloseFileStream();//关闭文件流
	HRESULT SetParmAfterCapture();//完成类属性填写
	void* SetFormatBuffer();//开辟内存存放音频格式
	HRESULT WriteFileStream();//将类写入文件中

	HRESULT SetWAVEFORMATEX(WAVEFORMATEX*);//告诉音频端点格式
	HRESULT LoadData(BYTE*, UINT32, DWORD*);//向音频端点缓存写入数据
};