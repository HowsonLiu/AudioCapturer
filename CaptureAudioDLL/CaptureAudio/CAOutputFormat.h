#pragma once
#include "stdafx.h"
#include <mmdeviceapi.h>
#include <vector>

using namespace std;

class CAObject;

/*����api�в�ͬ�Ĳ�������ʵ�ֲ�ͬ��¼��ģʽ������Ұ��������һ�������࣬����ͨ���̳к�����ʵ�ֲ�ͬ��¼��ģʽ*/

class CACaptureFormatBase {
public:
	virtual void GetCACaptureFormat_Parameter(__MIDL___MIDL_itf_mmdeviceapi_0000_0000_0001&, DWORD&) = 0;
};

class CACaptureFormat_Microphone : public CACaptureFormatBase {
public:
	virtual void GetCACaptureFormat_Parameter(__MIDL___MIDL_itf_mmdeviceapi_0000_0000_0001&, DWORD&) override;
};

class CACaptureFormat_Loopback : public CACaptureFormatBase {
public:
	virtual void GetCACaptureFormat_Parameter(__MIDL___MIDL_itf_mmdeviceapi_0000_0000_0001&, DWORD&) override;
};

/*ʹ�ó��󹤳����ģʽ*/


//��Ʒ��PCM
class PCMBase {
public:
	virtual ~PCMBase() {};

	virtual HRESULT Save(const CAObject*, const string) = 0;
};

//��Ʒ
class PCMOutputWAV :public PCMBase {
public:
	virtual ~PCMOutputWAV() {};

	virtual HRESULT Save(const CAObject*, const string) override;
};

//��Ʒ��NonPCM
class NonPCMBase {
public:
	virtual ~NonPCMBase() {};

	virtual HRESULT Save(const CAObject*, const string) = 0;
};

//��Ʒ
class NonPCMOutputWAV :public NonPCMBase {
public:
	virtual ~NonPCMOutputWAV() {};

	virtual HRESULT Save(const CAObject*, const string) override;
};

//��Ʒ��Extensible
class ExtensibleBase {
public:
	virtual ~ExtensibleBase() {};

	virtual HRESULT Save(const CAObject*, const string) = 0;
};

//��Ʒ
class ExtensibleOutputWAV :public ExtensibleBase {
public:
	virtual ~ExtensibleOutputWAV() {};

	virtual HRESULT Save(const CAObject*, const string) override;
};

//���󹤳�
class OutputFormatFactory {
public:
	virtual ~OutputFormatFactory() {};

	virtual PCMBase* SavePCMFormat() = 0;
	virtual NonPCMBase* SaveNonPCMFormat() = 0;
	virtual ExtensibleBase* SaveExtensibleFormat() = 0;
};

//WAV����
class WAVFactory :public OutputFormatFactory {
public:
	virtual ~WAVFactory() {};

	virtual PCMBase* SavePCMFormat() override;
	virtual NonPCMBase* SaveNonPCMFormat() override;
	virtual ExtensibleBase* SaveExtensibleFormat() override;
};