#include "stdafx.h"
#include "CAMainProcess.h"
#include <io.h>
#include <thread>
#include "Reflection.h"
#include <filesystem>

CAMainProcess* CAMainProcess::instance = new CAMainProcess;
CAMainProcess* CAMainProcess::GetInstance() {
	return instance;
}

string CAMainProcess::GetFolder() const
{
	return DefaultCAFilesFolderPath;
}

void CAMainProcess::SetFolder(const string folder)
{
	DefaultCAFilesFolderPath = folder;
}

void CAMainProcess::UpdateCAFilesMap()
{
	_finddata_t fileinfo;
	intptr_t hfile = 0;
	string filepath;
	if ((hfile = _findfirst(filepath.assign(DefaultCAFilesFolderPath).append("\\*" + suffix).c_str(), &fileinfo)) != -1) {
		do {
			if ((fileinfo.attrib&_A_SUBDIR)) {

			}
			else {
				string validpath = filepath.assign(DefaultCAFilesFolderPath).append("\\").append(fileinfo.name);
				if (!IsCAObjectInMap(validpath)) {
					NewCAObject(validpath);
					ReadCAFile(validpath);
				}
			}
		} while (_findnext(hfile, &fileinfo) == 0);
			_findclose(hfile);
	}
}

string CAMainProcess::GetPath(const string name) const
{
	return DefaultCAFilesFolderPath + "\\" + name + suffix;
}

bool CAMainProcess::IsCAFileExist(const string path)
{
	FILE* pcafile = fopen(path.c_str(), "r");
	if (pcafile == NULL) {
		return false;
	}
	fclose(pcafile);
	return true;
}

bool CAMainProcess::IsCAObjectInMap(const string path)
{
	map<string, CAObject*>::iterator iter = LocalCAFiles.find(path);
	if (iter != LocalCAFiles.end()) {
		return true;
	}
	return false;
}

bool CAMainProcess::IsCAObjectValid(const string path)
{
	return SelectCAObject(path)->IsCAObjectValid();
}

void CAMainProcess::NewCAObject(const string path)
{
	CAObject* pcaobject = new CAObject(path);
	LocalCAFiles.insert(pair<string, CAObject*>(path, pcaobject));
}

void CAMainProcess::ReadCAFile(const string path)
{
	SelectCAObject(path)->ReadFileStream();
}

CAObject * CAMainProcess::SelectCAObject(const string path)
{
	map<string,CAObject*>::iterator iter = LocalCAFiles.find(path);
	if (iter != LocalCAFiles.end()) {
		return iter->second;
	}
	return nullptr;
}

void CAMainProcess::RemoveCAObject(const string path)
{
	delete LocalCAFiles.at(path);
	LocalCAFiles.erase(path);
}

bool CAMainProcess::DeleteCAFile(const string path)
{
	return remove(path.c_str()) == 0;
}

void CAMainProcess::SetCustomWAVEFORMATEX(const WORD wFormatTag, const WORD nChannels, const DWORD nSamplesPerSec, const DWORD nAvgBytePerSec, const WORD nBlockAlign, const WORD wBitsPerSample, const WORD cbsize, const WORD Samples, const DWORD dwChannelMask, const GUID SubFormat)
{
	if (pCustomFormat == nullptr) {
		if (wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
			WAVEFORMATEXTENSIBLE* pFormatExtensible = new WAVEFORMATEXTENSIBLE;
			pFormatExtensible->Samples.wValidBitsPerSample = Samples;
			pFormatExtensible->dwChannelMask = dwChannelMask;
			pFormatExtensible->SubFormat = SubFormat;
			pCustomFormat = reinterpret_cast<WAVEFORMATEX*>(pFormatExtensible);
		}
		else {
			pCustomFormat = new WAVEFORMATEX;
		}
	}
	else {
		delete pCustomFormat;
		pCustomFormat = nullptr;
		SetCustomWAVEFORMATEX(wFormatTag, nChannels, nSamplesPerSec, nAvgBytePerSec, nBlockAlign, wBitsPerSample, cbsize, Samples, dwChannelMask, SubFormat);
	}
	pCustomFormat->wFormatTag = wFormatTag;
	pCustomFormat->nChannels = nChannels;
	pCustomFormat->nSamplesPerSec = nSamplesPerSec;
	pCustomFormat->nAvgBytesPerSec = nAvgBytePerSec;
	pCustomFormat->nBlockAlign = nBlockAlign;
	pCustomFormat->wBitsPerSample = wBitsPerSample;
	pCustomFormat->cbSize = cbsize;
}

void CAMainProcess::SetCountdownToStart(long sec, bool* presult)
{
	if (pcountdowntostart == nullptr) {
		pcountdowntostart = new CATime(sec, presult);
	}
	else {
		pcountdowntostart->SetTime(sec);
		pcountdowntostart->SetBool(presult);
	}
}

void CAMainProcess::SetCountdownToStop(long sec, bool* presult)
{
	if (pcountdowntostop == nullptr) {
		pcountdowntostop = new CATime(sec, presult);
	}
	else {
		pcountdowntostop->SetTime(sec);
		pcountdowntostop->SetBool(presult);
	}
}

void CAMainProcess::DeleteCustomFormat()
{
	if (pCustomFormat != nullptr) {
		delete pCustomFormat;
		pCustomFormat = nullptr;
	}
}

void CAMainProcess::DeleteCountdownToStart()
{
	if (pcountdowntostart != nullptr) {
		delete pcountdowntostart;
		pcountdowntostart = nullptr;
	}
}

void CAMainProcess::DeleteCoundownToStop()
{
	if (pcountdowntostop != nullptr) {
		delete pcountdowntostop;
		pcountdowntostop = nullptr;
	}
}

HRESULT CAMainProcess::StartCapture(const string path, const string captureformat, bool* capturestop)
{
	HRESULT hr;
	CACaptureFormatBase* pcapformat = (CACaptureFormatBase*)Reflection_Factory::GetInstance()->GetCaptureClass(captureformat);
	CAObject* pcaobject = SelectCAObject(path);
	if (pcountdowntostart != nullptr && pcountdowntostop != nullptr) {
		pcountdowntostart->Start();
		pcountdowntostart->threadjoin();
		pcountdowntostop->Start();
		pcountdowntostop->threaddetach();
	}
	pruntime->Reset();
	pruntime->SetTime(MAXCAPTURETIME);
	pruntime->SetBool(capturestop);
	pruntime->Start();
	pruntime->threaddetach();
	hr = ::StartCapture(pcaobject, pcapformat, pCustomFormat, capturestop);
	return hr;
}

HRESULT CAMainProcess::StartRender(const string path, bool* stop)
{
	HRESULT hr;
	CAObject* pcaobject = SelectCAObject(path);
	pruntime->Reset();
	pruntime->SetTime(MAXCAPTURETIME);
	pruntime->SetBool(stop);
	pruntime->Start();
	pruntime->threaddetach();
	hr = ::StartRender(pcaobject, stop);
	return hr;
}

HRESULT CAMainProcess::Output(const string capath, const string outputformat, const string outputpath)
{
	HRESULT hr;
	CAObject* pcaobject = SelectCAObject(capath);
	OutputFormatFactory* poutputformat = (OutputFormatFactory*)Reflection_Factory::GetInstance()->GetOutputClass(outputformat);
	hr = pcaobject->SaveAsOtherFormat(poutputformat, outputpath);
	return hr;
}

string CAMainProcess::GetCAObjectName(const string path)
{
	return SelectCAObject(path)->GetName();
}

int CAMainProcess::GetCAObjectSize(const string path)
{
	return SelectCAObject(path)->GetFileSize();
}

int CAMainProcess::GetCAObjectDuration(const string path)
{
	return SelectCAObject(path)->GetAudioLength();
}

int CAMainProcess::GetCAObjectChannels(const string path)
{
	return SelectCAObject(path)->GetChannels();
}

int CAMainProcess::GetCAObjectSamplePerSec(const string path)
{
	return SelectCAObject(path)->GetSamplePerSec();
}

string CAMainProcess::GetCAObjectCreateTime(const string path)
{
	return SelectCAObject(path)->GetFileCreateTime();
}

vector<string> CAMainProcess::GetMapObjectPaths()
{
	vector<string> allnames;
	map<string, CAObject*>::iterator iter = LocalCAFiles.begin();
	while (iter != LocalCAFiles.end()) {
		allnames.push_back(iter->first);
		iter++;
	}
	return allnames;
}

long * CAMainProcess::GetCountdownToStart()
{
	if (pcountdowntostart == nullptr) {
		return nullptr;
	}
	return pcountdowntostart->GetLeaveTime();
}

long * CAMainProcess::GetCountdownToStop()
{
	if (pcountdowntostop == nullptr) {
		return nullptr;
	}
	return pcountdowntostop->GetLeaveTime();
}

long * CAMainProcess::GetRunTime()
{
	return pruntime->GetPassTime();
}








CATime::CATime(long l, bool* pb = nullptr) :requiretime(l), currenttime(0), starttime(0), passtime(0), leavetime(0), pthread(nullptr), presult(pb)
{
}

void CATime::CountDown()
{
	starttime = clock();
	while (*presult == false) {
		currenttime = clock();
		passtime = currenttime - starttime;
		leavetime = requiretime - passtime;
		if (leavetime < 0) {
			break;
		}
	}
	*presult = true;
}

void CATime::Start()
{
	pthread = new thread(&CATime::CountDown, this);
}

void CATime::Stop()
{
	*presult = true;
}

void CATime::threadjoin()
{
	pthread->join();
}

void CATime::threaddetach()
{
	pthread->detach();
}

bool CATime::IsStarted()
{
	if (pthread == nullptr) {
		return false;
	}
	return true;
}

void CATime::SetTime(long time)
{
	requiretime = time;
}

void CATime::SetBool(bool * pb)
{
	presult = pb;
}

void CATime::Reset()
{
	requiretime = 0;
	pthread = nullptr;
	presult = nullptr;
}

clock_t* CATime::GetPassTime()
{
	return &passtime;
}

clock_t* CATime::GetLeaveTime()
{
	return &leavetime;
}
