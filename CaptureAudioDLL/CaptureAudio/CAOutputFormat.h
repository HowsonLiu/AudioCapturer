#pragma once
#include "stdafx.h"
#include <mmdeviceapi.h>
#include <vector>

using namespace std;

class CAObject;

/*由于api中不同的参数可以实现不同的录制模式，因此我把他抽象成一个抽象类，子类通过继承后重载实现不同的录制模式*/

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

/*使用抽象工厂设计模式*/


//产品族PCM
class PCMBase {
public:
	virtual ~PCMBase() {};

	virtual HRESULT Save(const CAObject*, const string) = 0;
};

//产品
class PCMOutputWAV :public PCMBase {
public:
	virtual ~PCMOutputWAV() {};

	virtual HRESULT Save(const CAObject*, const string) override;
};

//产品族NonPCM
class NonPCMBase {
public:
	virtual ~NonPCMBase() {};

	virtual HRESULT Save(const CAObject*, const string) = 0;
};

//产品
class NonPCMOutputWAV :public NonPCMBase {
public:
	virtual ~NonPCMOutputWAV() {};

	virtual HRESULT Save(const CAObject*, const string) override;
};

//产品族Extensible
class ExtensibleBase {
public:
	virtual ~ExtensibleBase() {};

	virtual HRESULT Save(const CAObject*, const string) = 0;
};

//产品
class ExtensibleOutputWAV :public ExtensibleBase {
public:
	virtual ~ExtensibleOutputWAV() {};

	virtual HRESULT Save(const CAObject*, const string) override;
};

//抽象工厂
class OutputFormatFactory {
public:
	virtual ~OutputFormatFactory() {};

	virtual PCMBase* SavePCMFormat() = 0;
	virtual NonPCMBase* SaveNonPCMFormat() = 0;
	virtual ExtensibleBase* SaveExtensibleFormat() = 0;
};

//WAV工厂
class WAVFactory :public OutputFormatFactory {
public:
	virtual ~WAVFactory() {};

	virtual PCMBase* SavePCMFormat() override;
	virtual NonPCMBase* SaveNonPCMFormat() override;
	virtual ExtensibleBase* SaveExtensibleFormat() override;
};