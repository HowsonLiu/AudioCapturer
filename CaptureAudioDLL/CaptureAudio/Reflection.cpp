#include "stdafx.h"
#include "Reflection.h"

Reflection_Factory* Reflection_Factory::instance = new Reflection_Factory;

Reflection_Factory* Reflection_Factory::GetInstance() {
	return instance;
}

void Reflection_Factory::CaptureRegist(string name, callbackfunc mothod)
{
	CACaptureformat_map.insert(pair<string, callbackfunc>(name, mothod));
	CACaptureformat_vec.push_back(name);
}

void Reflection_Factory::OutputRegist(string name, callbackfunc mothod)
{
	CAOutputformat_map.insert(pair<string, callbackfunc>(name, mothod));
	CAOutputformat_vec.push_back(name);
}

void * Reflection_Factory::GetCaptureClass(string name)
{
	map<string, callbackfunc>::const_iterator iter;
	iter = CACaptureformat_map.find(name);
	if (iter == CACaptureformat_map.end()) {
		return nullptr;
	}
	else {
		return iter->second();
	}
}

void * Reflection_Factory::GetOutputClass(string name)
{
	map<string, callbackfunc>::const_iterator iter;
	iter = CAOutputformat_map.find(name);
	if (iter == CAOutputformat_map.end()) {
		return nullptr;
	}
	else {
		return iter->second();
	}
}

vector<string> Reflection_Factory::GetAllCaptureClassNames()
{
	return CACaptureformat_vec;
}

vector<string> Reflection_Factory::GetAllOutputClassNames()
{
	return CAOutputformat_vec;
}

RegistCapture::RegistCapture(string name, callbackfunc mothed)
{
	Reflection_Factory::GetInstance()->CaptureRegist(name, mothed);
}

RegistOutput::RegistOutput(string name, callbackfunc mothed)
{
	Reflection_Factory::GetInstance()->OutputRegist(name, mothed);
}


CREATCALLBACKFUNC(CACaptureFormat_Microphone);
REGISTTOCAP(Microphone, CACaptureFormat_Microphone);

CREATCALLBACKFUNC(CACaptureFormat_Loopback);
REGISTTOCAP(Loopback, CACaptureFormat_Loopback);

CREATCALLBACKFUNC(WAVFactory);
REGISTTOOUT(WAV, WAVFactory);