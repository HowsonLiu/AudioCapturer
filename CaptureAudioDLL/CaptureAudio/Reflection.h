#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <map>
#include <vector>
#include "CAOutputFormat.h"
using namespace std;

//���׷���

#define CREATCALLBACKFUNC(classname)		classname* creatclass##classname() {return new classname;}		//�꣬����ָ������ָ��ĺ���

#define REGISTTOCAP(name,classname)		RegistCapture registcap##classname(#name,(callbackfunc)creatclass##classname)      //ע�ᵽcaptureformat��
		
#define REGISTTOOUT(name,classname)		RegistOutput registout##classname(#name,(callbackfunc)creatclass##classname)		//ע�ᵽOutputformat��

typedef void* (*callbackfunc)(void);

class Reflection_Factory {
private:
	Reflection_Factory() {};
	Reflection_Factory(const Reflection_Factory&) {};
	Reflection_Factory& operator= (const Reflection_Factory&) {};
	static Reflection_Factory* instance;

	map<string, callbackfunc> CACaptureformat_map;
	map<string, callbackfunc> CAOutputformat_map;

	vector<string> CACaptureformat_vec;
	vector<string> CAOutputformat_vec;

public:
	static Reflection_Factory* GetInstance();

	void CaptureRegist(string, callbackfunc);
	void OutputRegist(string, callbackfunc);

	void* GetCaptureClass(string);
	void* GetOutputClass(string);

	vector<string> GetAllCaptureClassNames();
	vector<string> GetAllOutputClassNames();
};

class RegistCapture {
public:
	RegistCapture(string, callbackfunc);
};

class RegistOutput {
public:
	RegistOutput(string, callbackfunc);
};