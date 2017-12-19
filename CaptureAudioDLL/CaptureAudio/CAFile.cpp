#include "stdafx.h"
#include "CAFile.h"
#include "CAOutputFormat.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <filesystem>
#include <io.h>

#define REFTIMES_PER_SEC  10000000 
#define REFTIMES_PER_MILLISEC  10000 

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

CAObject::CAObject(const string path) :filepath(path), pFormat(NULL), pPCMBuffer(NULL), pReadBufferPoint(NULL), pfilestream(NULL) {
	GetNameFromPath();
}

string CAObject::GetName() const {
	return filename;
}

string CAObject::GetPath() const {
	return filepath;
}

const WAVEFORMATEX* CAObject::GetWAVEFORMATEX() const {
	return pFormat;
}

int CAObject::GetAudioLength() const
{
	return PCMLength / pFormat->nAvgBytesPerSec;
}

int CAObject::GetFileSize() const
{
	int size = 0;
	FILE* pfile = fopen(filepath.c_str(), "rb");
	if (pfile) {
		size = filelength(fileno(pfile));
		fclose(pfile);
	}
	return size;
}

string CAObject::GetFileCreateTime() const
{
	struct _stat buf;
	char timebuf[26];
	if (_stat(filepath.c_str(), &buf) != 0) {
		return string();
	}
	if (ctime_s(timebuf, 26, &buf.st_mtime)) {
		return string();
	}
	return string(timebuf);
}

int CAObject::GetChannels() const
{
	return pFormat->nChannels;
}

int CAObject::GetSamplePerSec() const
{
	return pFormat->nSamplesPerSec;
}

const void* CAObject::GetPCMBuffer() const {
	return pPCMBuffer;
}

UINT32 CAObject::GetPCMLength() const {
	return PCMLength;
}

HRESULT CAObject::ReadFileStream()
{
	if (pfilestream != NULL) {
		return E_NOTIMPL;
	}
	pfilestream = fopen(filepath.c_str(), "rb");
	if (pfilestream == NULL) {
		return E_NOTIMPL;
	}
	fseek(pfilestream, 0 - WAVEFORMATEXTENSIBLELENGTH, SEEK_END);//指向文件末尾前40字节的位置
	void* pformatbuffer = malloc(WAVEFORMATEXTENSIBLELENGTH);//开辟临时缓存，存放格式数据
	fread(pformatbuffer, WAVEFORMATEXTENSIBLELENGTH, 1, pfilestream);//从文件流中读取数据到缓存中

	//根据格式是否为WAVE_FORMAT_EXTENSIBLE创建pFormat
	if (*(WORD*)pformatbuffer == WAVE_FORMAT_EXTENSIBLE) {
		WAVEFORMATEXTENSIBLE* pFormatExtensible = new WAVEFORMATEXTENSIBLE();
		pFormatExtensible->Samples.wValidBitsPerSample = *(WORD*)((char*)pformatbuffer + 18);
		pFormatExtensible->dwChannelMask = *(DWORD*)((char*)pformatbuffer + 20);
		pFormatExtensible->SubFormat = *(GUID*)((char*)pformatbuffer + 24);
		pFormat = reinterpret_cast<WAVEFORMATEX*>(pFormatExtensible);
	}
	else {
		pFormat = new WAVEFORMATEX();
	}
	pFormat->wFormatTag = *(WORD*)pformatbuffer;
	pFormat->nChannels = *(WORD*)((char*)pformatbuffer + 2);
	pFormat->nSamplesPerSec = *(DWORD*)((char*)pformatbuffer + 4);
	pFormat->nAvgBytesPerSec = *(DWORD*)((char*)pformatbuffer + 8);
	pFormat->nBlockAlign = *(WORD*)((char*)pformatbuffer + 12);
	pFormat->wBitsPerSample = *(WORD*)((char*)pformatbuffer + 14);
	pFormat->cbSize = *(WORD*)((char*)pformatbuffer + 16);
	
	fseek(pfilestream, 0 - WAVEFORMATEXTENSIBLELENGTH, SEEK_END);//返回末尾前40字节位置
	PCMLength = ftell(pfilestream);//获取pcm数据长度
	fseek(pfilestream, 0, SEEK_SET);//文件指针返回起始点
	pPCMBuffer = malloc(PCMLength);//分配内存
	PCMLength = fread(pPCMBuffer, 1, PCMLength, pfilestream);//从文件流中读取数据到pcm缓存中
	pReadBufferPoint = pPCMBuffer;//初始化读指针
	fclose(pfilestream);
	pfilestream = NULL;
	if (IsCAObjectValid()) {
		return S_OK;
	}
	return E_NOTIMPL;
}

HRESULT CAObject::WriteFileStream() {
	pfilestream = fopen(filepath.c_str(), "ab+");
	if (pfilestream == NULL) {
		return E_NOTIMPL;
	}
	void* pformatbuffer = SetFormatBuffer();
	if (pformatbuffer == NULL) {
		return E_NOTIMPL;
	}
	fwrite(pformatbuffer, WAVEFORMATEXTENSIBLELENGTH, 1, pfilestream);
	free(pformatbuffer);
	fclose(pfilestream);
	pfilestream = NULL;
	return S_OK;
}

HRESULT CAObject::SaveAsOtherFormat(OutputFormatFactory* outputformat,const string savepath) {
	if (pFormat->wFormatTag == WAVE_FORMAT_PCM) {
		PCMBase* pPCM = outputformat->SavePCMFormat();
		if (pPCM == NULL) {
			return E_NOTIMPL;
		}
		if (SUCCEEDED(pPCM->Save(this, savepath))) {
			delete pPCM;
			return S_OK;
		}
		else {
			return E_NOTIMPL;
		}
	}
	else if (pFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
		ExtensibleBase* pExtensible = outputformat->SaveExtensibleFormat();
		if (pExtensible == NULL) {
			return E_NOTIMPL;
		}
		if (SUCCEEDED(pExtensible->Save(this, savepath))) {
			delete pExtensible;
			return S_OK;
		}
		else {
			return E_NOTIMPL;
		}
	}
	else {
		NonPCMBase* pNonPCM = outputformat->SaveNonPCMFormat();
		if (pNonPCM == NULL) {
			return E_NOTIMPL;
		}
		if (SUCCEEDED(pNonPCM->Save(this, savepath))) {
			delete pNonPCM;
			return S_OK;
		}
		else {
			return E_NOTIMPL;
		}
	}
}

void* CAObject::SetFormatBuffer() {
	void* pformatbuffer = malloc(WAVEFORMATEXTENSIBLELENGTH);
	if (pformatbuffer == NULL) {
		return NULL;
	}
	*(WORD*)pformatbuffer = pFormat->wFormatTag;
	*(WORD*)((char*)pformatbuffer + 2) = pFormat->nChannels;
	*(DWORD*)((char*)pformatbuffer + 4) = pFormat->nSamplesPerSec;
	*(DWORD*)((char*)pformatbuffer + 8) = pFormat->nAvgBytesPerSec;
	*(WORD*)((char*)pformatbuffer + 12) = pFormat->nBlockAlign;
	*(WORD*)((char*)pformatbuffer + 14) = pFormat->wBitsPerSample;
	*(WORD*)((char*)pformatbuffer + 16) = pFormat->cbSize;
	if (pFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
		PWAVEFORMATEXTENSIBLE pFormatExtensible = reinterpret_cast<PWAVEFORMATEXTENSIBLE>(pFormat);
		*(WORD*)((char*)pformatbuffer + 18) = pFormatExtensible->Samples.wValidBitsPerSample;
		*(DWORD*)((char*)pformatbuffer + 20) = pFormatExtensible->dwChannelMask;
		*(GUID*)((char*)pformatbuffer + 24) = pFormatExtensible->SubFormat;
	}
	return pformatbuffer;
}

bool CAObject::DeleteCAFile() {
	return remove(filepath.c_str()) == 0;
}

bool CAObject::IsCAFileExist() {
	FILE* pfile = fopen(filepath.c_str(), "r");
	if (pfile == NULL) {
		return false;
	}
	fclose(pfile);
	return true;
}

bool CAObject::IsCAObjectValid() {
	return (pFormat != NULL && pPCMBuffer != NULL && PCMLength != 0) ? true : false;
}

HRESULT CAObject::OpenFileStream() {
	if (pfilestream == NULL) {
		pfilestream = fopen(filepath.c_str(), "wb");
		return S_OK;
	}
	return E_NOTIMPL;
}


HRESULT CAObject::CopyData(BYTE* pdata, UINT32 numFramesAvailable,BOOL* emergencystop) {
	if (pdata == NULL) {
		*emergencystop = TRUE;
		return S_OK;
	}
	auto writenum = fwrite(pdata, pFormat->nBlockAlign, numFramesAvailable, pfilestream);//写入了nBlockAlign * numFramesAvailable个字节
	PCMLength += writenum * pFormat->nBlockAlign;
	return S_OK;
}

HRESULT CAObject::LoadData(BYTE* pData, UINT32 size, DWORD* flag) {
	if (pData == NULL) {
		return E_INVALIDARG;
	}
	if ((PCMLength - ReadIndex) < size) {
		size = PCMLength - ReadIndex;
		*flag = AUDCLNT_BUFFERFLAGS_SILENT;
	}
	memcpy(pData, pReadBufferPoint, size);//复制size个字节，size也是等于nBlockAlign * numFramesAvailable
	pReadBufferPoint = (BYTE*)pReadBufferPoint + size;
	ReadIndex += size;
}

HRESULT CAObject::CloseFileStream() {
	if (pfilestream != NULL) {
		fclose(pfilestream);
		pfilestream = NULL;
		return S_OK;
	}
	return E_NOTIMPL;
}

HRESULT CAObject::SetParmAfterCapture() {
	if (pfilestream != NULL) {
		return E_NOTIMPL;
	}
	pfilestream = fopen(filepath.c_str(), "rb");
	if (pfilestream == NULL) {
		return E_NOTIMPL;
	}
	fseek(pfilestream, 0, SEEK_END);
	PCMLength = ftell(pfilestream);
	fseek(pfilestream, 0, SEEK_SET);
	pPCMBuffer = malloc(PCMLength);
	PCMLength = fread(pPCMBuffer, 1, PCMLength, pfilestream);
	pReadBufferPoint = pPCMBuffer;
	fclose(pfilestream);
	pfilestream = NULL;
	if (IsCAObjectValid()) {
		return S_OK;
	}
	return E_NOTIMPL;
}


void CAObject::GetNameFromPath()
{
	const char* path = filepath.c_str();
	char* pbegin = const_cast<char*>(path);
	char* pend = const_cast<char*>(path) + strlen(path);
	if (strstr(path, "\\") != NULL) {
		while (pend >= path) {
			if (*pend == '\\') {
				pbegin = pend + 1;
				pend = const_cast<char*>(path) + strlen(path);
				break;
			}
			pend--;
		}
	}
	else if (strstr(path, "/") != NULL) {
		while (pend >= path) {
			if (*pend == '/') {
				pbegin = pend + 1;
				pend = const_cast<char*>(path) + strlen(path);
				break;
			}
			pend--;
		}
	}
	if (strstr(path, ".") != NULL) {
		while (pend >= path) {
			if (*pend == '.') {
				break;
			}
			pend--;
		}
	}
	filename = string(pbegin, pend);
}

HRESULT CAObject::GetWAVEFORMATEX(WAVEFORMATEX* format) {
	if (format != NULL) {
		if (format->wFormatTag != WAVE_FORMAT_EXTENSIBLE) {
			pFormat = new WAVEFORMATEX(*format);
		}
		else {
			WAVEFORMATEXTENSIBLE* pEx = new WAVEFORMATEXTENSIBLE();
			WAVEFORMATEXTENSIBLE* formatEx = reinterpret_cast<WAVEFORMATEXTENSIBLE*>(format);
			*pEx = *formatEx;
			pFormat = reinterpret_cast<WAVEFORMATEX*>(pEx);
		}
		if (pFormat != NULL) {
			return S_OK;
		}
	}
	return E_INVALIDARG;
}

HRESULT CAObject::ChangeToCostomWAVEFORMATEX(WAVEFORMATEX* defaultformat, WAVEFORMATEX* customformat) {
	if (customformat != NULL) {
		*defaultformat = *customformat;
	}
	return S_OK;
}

HRESULT CAObject::SetWAVEFORMATEX(WAVEFORMATEX* pformat) {
	if (pFormat == NULL) {
		return E_NOTIMPL;
	}
	if (pFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
		WAVEFORMATEXTENSIBLE* pFEx = reinterpret_cast<WAVEFORMATEXTENSIBLE*>(pFormat);
		pformat = reinterpret_cast<WAVEFORMATEX*>(pFEx);
		if (pformat != NULL) {
			return S_OK;
		}
	}
	else {
		pformat = pFormat;
		if (pformat != NULL) {
			return S_OK;
		}
	}
	return E_NOTIMPL;
}

HRESULT StartCapture(CAObject* pcafile, CACaptureFormatBase* pcapturefornat, WAVEFORMATEX* pwaveformat, bool* requirestop) {
	HRESULT hr;
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient *pAudioClient = NULL;
	IAudioCaptureClient *pCaptureClient = NULL;
	WAVEFORMATEX *pwfx = NULL;
	UINT32 packetLength = 0;
	BYTE *pData;
	DWORD flags;
	CoInitialize(NULL);

	__MIDL___MIDL_itf_mmdeviceapi_0000_0000_0001 paraone;
	DWORD paratwo;
	BOOL emergencystop = FALSE;

	if (pcapturefornat == nullptr) {
		hr = E_POINTER;
		EXIT_ON_ERROR(hr)
	}

	pcapturefornat->GetCACaptureFormat_Parameter(paraone, paratwo);

	hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator), NULL,
		CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
		(void**)&pEnumerator);
	EXIT_ON_ERROR(hr)

		hr = pEnumerator->GetDefaultAudioEndpoint(
			paraone, eConsole, &pDevice);
	EXIT_ON_ERROR(hr)

		hr = pDevice->Activate(
			__uuidof(IAudioClient), CLSCTX_ALL,
			NULL, (void**)&pAudioClient);
	EXIT_ON_ERROR(hr)

		hr = pAudioClient->GetMixFormat(&pwfx);
	EXIT_ON_ERROR(hr)

		hr = pcafile->ChangeToCostomWAVEFORMATEX(pwfx, pwaveformat);

		hr = pAudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			paratwo,
			hnsRequestedDuration,
			0,
			pwfx,
			NULL);
	EXIT_ON_ERROR(hr)

		// Get the size of the allocated buffer.
		hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	EXIT_ON_ERROR(hr)

		hr = pAudioClient->GetService(
			__uuidof(IAudioCaptureClient),
			(void**)&pCaptureClient);
	EXIT_ON_ERROR(hr)

		// Notify the audio sink which format to use.
		hr = pcafile->GetWAVEFORMATEX(pwfx);
	EXIT_ON_ERROR(hr)

		// Calculate the actual duration of the allocated buffer.
		hnsActualDuration = (double)REFTIMES_PER_SEC *
		bufferFrameCount / pwfx->nSamplesPerSec;

	hr = pcafile->OpenFileStream();
	EXIT_ON_ERROR(hr)

	hr = pAudioClient->Start();  // Start recording.
	EXIT_ON_ERROR(hr)

		// Each loop fills about half of the shared buffer.
		while (*requirestop == false && emergencystop == FALSE)
		{
			// Sleep for half the buffer duration.
			Sleep(hnsActualDuration / REFTIMES_PER_MILLISEC / 2);

			hr = pCaptureClient->GetNextPacketSize(&packetLength);
			EXIT_ON_ERROR(hr)

				while (packetLength != 0)
				{
					// Get the available data in the shared buffer.
					hr = pCaptureClient->GetBuffer(
						&pData,
						&numFramesAvailable,
						&flags, NULL, NULL);
					EXIT_ON_ERROR(hr)

						if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
						{
							pData = NULL;  // Tell CopyData to write silence.
						}

					// Copy the available capture data to the audio sink.
					hr = pcafile->CopyData(pData, numFramesAvailable, &emergencystop);
					EXIT_ON_ERROR(hr)

						hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
					EXIT_ON_ERROR(hr)

						hr = pCaptureClient->GetNextPacketSize(&packetLength);
					EXIT_ON_ERROR(hr)
				}
		}

	hr = pAudioClient->Stop();  // Stop recording.
	EXIT_ON_ERROR(hr)

	hr = pcafile->CloseFileStream();
	EXIT_ON_ERROR(hr)

	hr = pcafile->SetParmAfterCapture();
	EXIT_ON_ERROR(hr);

	hr = pcafile->WriteFileStream();
	EXIT_ON_ERROR(hr);

		Exit:
	CoTaskMemFree(pwfx);
	SAFE_RELEASE(pEnumerator)
		SAFE_RELEASE(pDevice)
		SAFE_RELEASE(pAudioClient)
		SAFE_RELEASE(pCaptureClient)

		return hr;
}

HRESULT StartRender(CAObject* pcafile, bool* requirestop) {
	HRESULT hr;
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient *pAudioClient = NULL;
	IAudioRenderClient *pRenderClient = NULL;
	WAVEFORMATEX *pwfx = NULL;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	UINT32 numFramesPadding;
	BYTE *pData;
	DWORD flags = 0;
	CoInitialize(NULL);

	hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator), NULL,
		CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
		(void**)&pEnumerator);
	EXIT_ON_ERROR(hr)

		hr = pEnumerator->GetDefaultAudioEndpoint(
			eRender, eConsole, &pDevice);
	EXIT_ON_ERROR(hr)

		hr = pDevice->Activate(
			__uuidof(IAudioClient), CLSCTX_ALL,
			NULL, (void**)&pAudioClient);
	EXIT_ON_ERROR(hr)

		hr = pAudioClient->GetMixFormat(&pwfx);
	EXIT_ON_ERROR(hr)

		hr = pAudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			0,
			hnsRequestedDuration,
			0,
			pwfx,
			NULL);
	EXIT_ON_ERROR(hr)

		// Tell the audio source which format to use.
		hr = pcafile->SetWAVEFORMATEX(pwfx);
	EXIT_ON_ERROR(hr)

		// Get the actual size of the allocated buffer.
		hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	EXIT_ON_ERROR(hr)

		hr = pAudioClient->GetService(
			__uuidof(IAudioRenderClient),
			(void**)&pRenderClient);
	EXIT_ON_ERROR(hr)

		//重置读指针
		pcafile->pReadBufferPoint = pcafile->pPCMBuffer;
		pcafile->ReadIndex = 0;

		// Grab the entire buffer for the initial fill operation.
		hr = pRenderClient->GetBuffer(bufferFrameCount, &pData);
		EXIT_ON_ERROR(hr)

			// Load the initial data into the shared buffer.
			hr = pcafile->LoadData(pData, bufferFrameCount * pwfx->nBlockAlign, &flags);
	EXIT_ON_ERROR(hr)

		hr = pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
	EXIT_ON_ERROR(hr)

		// Calculate the actual duration of the allocated buffer.
		hnsActualDuration = (double)REFTIMES_PER_SEC *
		bufferFrameCount / pwfx->nSamplesPerSec;

	hr = pAudioClient->Start();  // Start playing.
	EXIT_ON_ERROR(hr)

		// Each loop fills about half of the shared buffer.
		while (flags != AUDCLNT_BUFFERFLAGS_SILENT && *requirestop == false)
		{
			// Sleep for half the buffer duration.
			Sleep((DWORD)(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

			// See how much buffer space is available.
			hr = pAudioClient->GetCurrentPadding(&numFramesPadding);
			EXIT_ON_ERROR(hr)

				numFramesAvailable = bufferFrameCount - numFramesPadding;

			// Grab all the available space in the shared buffer.
			hr = pRenderClient->GetBuffer(numFramesAvailable, &pData);
			EXIT_ON_ERROR(hr)

				// Get next 1/2-second of data from the audio source.
				hr = pcafile->LoadData(pData, numFramesAvailable * pwfx->nBlockAlign, &flags);
			EXIT_ON_ERROR(hr)

				hr = pRenderClient->ReleaseBuffer(numFramesAvailable, flags);
			EXIT_ON_ERROR(hr)
		}

	// Wait for last data in buffer to play before stopping.
	Sleep((DWORD)(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

	hr = pAudioClient->Stop();  // Stop playing.
	EXIT_ON_ERROR(hr)

		//重置读指针
		pcafile->pReadBufferPoint = pcafile->pPCMBuffer;
		pcafile->ReadIndex = 0;

		Exit:
	CoTaskMemFree(pwfx);
	SAFE_RELEASE(pEnumerator)
		SAFE_RELEASE(pDevice)
		SAFE_RELEASE(pAudioClient)
		SAFE_RELEASE(pRenderClient)

		return hr;
}
