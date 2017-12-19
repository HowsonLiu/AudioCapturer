#include "stdafx.h"
#include "CAOutputFormat.h"
#include "CAFile.h"
#include <Audioclient.h>
#include <filesystem>

HRESULT PCMOutputWAV::Save(const CAObject* pcafile, const string path)
{
	UINT32 pcmlen = pcafile->GetPCMLength();
	const WAVEFORMATEX* pformat = pcafile->GetWAVEFORMATEX();
	string wavfile = path + "\\" + pcafile->GetName() + ".wav";

	FILE* pf = fopen(wavfile.c_str(), "wb");
	if (pf == NULL) {
		return E_NOTIMPL;
	}

	void* wav = malloc(pcmlen + 44);
	memcpy(wav, "RIFF", 4);
	*(int *)((char*)wav + 4) = pcmlen + 36;
	memcpy(((char*)wav + 8), "WAVEfmt ", 8);
	*(int *)((char*)wav + 16) = 16;
	*(WORD*)((char*)wav + 20) = WAVE_FORMAT_PCM;
	*(WORD*)((char*)wav + 22) = pformat->nChannels;
	*(DWORD*)((char*)wav + 24) = pformat->nSamplesPerSec;
	*(DWORD*)((char*)wav + 28) = pformat->nAvgBytesPerSec;
	*(WORD*)((char*)wav + 32) = pformat->nBlockAlign;
	*(WORD*)((char*)wav + 34) = pformat->wBitsPerSample;
	strcpy((char*)((char*)wav + 36), "data");
	*(int *)((char*)wav + 40) = pcmlen;
	memcpy((char*)wav + 44, pcafile->GetPCMBuffer(), pcmlen);

	fwrite(wav, pcmlen + 44, 1, pf);
	fclose(pf);
	free(wav);

	return S_OK;
}

HRESULT NonPCMOutputWAV::Save(const CAObject* pcafile, const string path)
{
	UINT32 pcmlen = pcafile->GetPCMLength();
	const WAVEFORMATEX* pformat = pcafile->GetWAVEFORMATEX();
	string wavfile = path + "\\" + pcafile->GetName() + ".wav";

	FILE* pf = fopen(wavfile.c_str(), "wb");
	if (pf == NULL) {
		return E_NOTIMPL;
	}

	void* wav = malloc(pcmlen + 58);
	memcpy(wav, "RIFF", 4);
	*(int *)((char*)wav + 4) = pcmlen + 50;
	memcpy(((char*)wav + 8), "WAVEfmt ", 8);
	*(int *)((char*)wav + 16) = 18;
	*(WORD*)((char*)wav + 20) = pformat->wFormatTag;
	*(WORD*)((char*)wav + 22) = pformat->nChannels;
	*(DWORD*)((char*)wav + 24) = pformat->nSamplesPerSec;
	*(DWORD*)((char*)wav + 28) = pformat->nAvgBytesPerSec;
	*(WORD*)((char*)wav + 32) = pformat->nBlockAlign;
	*(WORD*)((char*)wav + 34) = pformat->wBitsPerSample;
	*(WORD*)((char*)wav + 36) = pformat->cbSize;
	memcpy(((char*)wav + 38), "fact", 4);
	*(DWORD*)((char*)wav + 42) = 4;
	*(DWORD*)((char*)wav + 46) = pcmlen / 2;
	strcpy((char*)((char*)wav + 50), "data");
	*(int *)((char*)wav + 54) = pcmlen;
	memcpy((char*)wav + 58, pcafile->GetPCMBuffer(), pcmlen);

	fwrite(wav, pcmlen + 58, 1, pf);
	fclose(pf);
	free(wav);

	return S_OK;
}

HRESULT ExtensibleOutputWAV::Save(const CAObject* pcafile, const string path)
{
	UINT32 pcmlen = pcafile->GetPCMLength();
	const WAVEFORMATEX* pformat = pcafile->GetWAVEFORMATEX();
	WAVEFORMATEX* pformatnoncast = const_cast<WAVEFORMATEX*>(pformat);
	WAVEFORMATEXTENSIBLE* pformatex = reinterpret_cast<WAVEFORMATEXTENSIBLE*>(pformatnoncast);
	string wavfile = path + "\\" + pcafile->GetName() + ".wav";

	FILE* pf = fopen(wavfile.c_str(), "wb");
	if (pf == NULL) {
		return E_NOTIMPL;
	}

	void* wav = malloc(pcmlen + 80);
	memcpy(wav, "RIFF", 4);
	*(int *)((char*)wav + 4) = pcmlen + 72;
	memcpy(((char*)wav + 8), "WAVEfmt ", 8);
	*(int *)((char*)wav + 16) = 40;
	*(WORD*)((char*)wav + 20) = WAVE_FORMAT_EXTENSIBLE;
	*(WORD*)((char*)wav + 22) = pformat->nChannels;
	*(DWORD*)((char*)wav + 24) = pformat->nSamplesPerSec;
	*(DWORD*)((char*)wav + 28) = pformat->nAvgBytesPerSec;
	*(WORD*)((char*)wav + 32) = pformat->nBlockAlign;
	*(WORD*)((char*)wav + 34) = pformat->wBitsPerSample;
	*(WORD*)((char*)wav + 36) = pformat->cbSize;
	*(WORD*)((char*)wav + 38) = pformatex->Samples.wValidBitsPerSample;
	*(DWORD*)((char*)wav + 40) = pformatex->dwChannelMask;
	*(GUID*)((char*)wav + 44) = pformatex->SubFormat;
	memcpy(((char*)wav + 60), "fact", 4);
	*(DWORD*)((char*)wav + 64) = 4;
	*(DWORD*)((char*)wav + 68) = pcmlen / 2;
	strcpy((char*)((char*)wav + 72), "data");
	*(int *)((char*)wav + 76) = pcmlen;
	memcpy((char*)wav + 80, pcafile->GetPCMBuffer(), pcmlen);

	fwrite(wav, pcmlen + 80, 1, pf);
	fclose(pf);
	free(wav);

	return S_OK;
}

PCMBase * WAVFactory::SavePCMFormat()
{
	return new PCMOutputWAV;
}

NonPCMBase * WAVFactory::SaveNonPCMFormat()
{
	return new NonPCMOutputWAV;
}

ExtensibleBase * WAVFactory::SaveExtensibleFormat()
{
	return new ExtensibleOutputWAV;
}

void CACaptureFormat_Microphone::GetCACaptureFormat_Parameter(__MIDL___MIDL_itf_mmdeviceapi_0000_0000_0001 & firstparm, DWORD & secondparm)
{
	firstparm = eCapture;
	secondparm = 0;
}

void CACaptureFormat_Loopback::GetCACaptureFormat_Parameter(__MIDL___MIDL_itf_mmdeviceapi_0000_0000_0001 & firstparm, DWORD & secondparm)
{
	firstparm = eRender;
	secondparm = AUDCLNT_STREAMFLAGS_LOOPBACK;
}
