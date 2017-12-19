#pragma once
#include "stdafx.h"
#include "CAOutputFormat.h"
#include "CAMainProcess.h"
#include <string>
#include <Audioclient.h>

/*
WORD ռ 2�ֽ�
DWORD ռ 4 �ֽ�
WAVEFORMATEX ��5��WORD��2��DWORD ռ 18�ֽ�
union WORD ռ 2 �ֽ�
GUID SubFormat 16 Byte
���� WAVEFORMATEXTENSIBLE ��1��WAVEFORMATEX��1��WORD��1��DWORD��1��GUID ռ 40�ֽ�
*/
#define WAVEFORMATEXTENSIBLELENGTH 40

using namespace std;

class CAObject {

	//�����ӿ�
	friend HRESULT StartCapture(CAObject*, CACaptureFormatBase*, WAVEFORMATEX*, bool*);//��ʼ¼��
	friend HRESULT StartRender(CAObject*, bool*);//��ʼ����

public:
	CAObject(const string);
	~CAObject() = default;

	//������ʾ
	string GetName() const;//��ȡ����
	int GetAudioLength() const;//��ȡ����ʱ��
	int GetFileSize() const;//��ȡ�ļ���С
	string GetFileCreateTime() const;//��ȡ�ļ�����ʱ��
	int GetChannels() const;//��ȡ������
	int GetSamplePerSec() const;//��ȡ������

	//ת����ʽר��
	const WAVEFORMATEX* GetWAVEFORMATEX() const;//��ȡ��Ƶ��ʽ
	const void* GetPCMBuffer() const;//��ȡpcm����
	UINT32 GetPCMLength() const;//��ȡpcm���ݳ���

	//���ڲ���

	string GetPath() const;//��ȡ·��
	bool IsCAFileExist();//���·���ļ��Ƿ����
	bool IsCAObjectValid();//����������Ƿ���ȷ����
	HRESULT ReadFileStream();//���ļ�����д������
	HRESULT SaveAsOtherFormat(OutputFormatFactory*,const string);//����Ϊ�����ɲ����ļ�
	bool DeleteCAFile();//ɾ���ļ�

private:
	string filename;
	string filepath;

	WAVEFORMATEX* pFormat;
	void* pPCMBuffer;
	void* pReadBufferPoint;
	UINT32 PCMLength;
	UINT32 ReadIndex;
	FILE* pfilestream;

	void GetNameFromPath();//���㴦����

	HRESULT GetWAVEFORMATEX(WAVEFORMATEX*);//����pcm����֮ǰ��ȡĬ�ϸ�ʽ
	HRESULT ChangeToCostomWAVEFORMATEX(WAVEFORMATEX*, WAVEFORMATEX*);//�����Զ�����Ƶ��ʽ
	HRESULT OpenFileStream();//�����ļ���
	HRESULT CopyData(BYTE*, UINT32, BOOL*);//���ļ���д��pcm����
	HRESULT CloseFileStream();//�ر��ļ���
	HRESULT SetParmAfterCapture();//�����������д
	void* SetFormatBuffer();//�����ڴ�����Ƶ��ʽ
	HRESULT WriteFileStream();//����д���ļ���

	HRESULT SetWAVEFORMATEX(WAVEFORMATEX*);//������Ƶ�˵��ʽ
	HRESULT LoadData(BYTE*, UINT32, DWORD*);//����Ƶ�˵㻺��д������
};