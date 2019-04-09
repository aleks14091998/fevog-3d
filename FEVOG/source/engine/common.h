#ifndef _COMMON_H
#define _COMMON_H

#include"FEV_Eng.h"
#include"FuncDelegate.h"

#include"platform/PlatformAPI.h"
#include "Utils.h"

#include <string>
#include <fstream>


using namespace valk;

#ifdef VALK_USE_COM

#define IJTS_BASE_IMPLEMENTATION(interface_name) \
	HRESULT CALLBACK GetGUID(GUID &guid)\
	{\
		guid = IID_##interface_name;\
		return S_OK;\
	}\
	HRESULT CALLBACK QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)\
	{\
		*ppvObject = NULL;\
		if(::memcmp(&riid,&__uuidof(IUnknown),sizeof(GUID)) == 0) \
		*ppvObject = static_cast<IUnknown *>(this);\
		else if(::memcmp(&riid,&IID_##interface_name,sizeof(GUID)) == 0) \
		*ppvObject = static_cast<interface_name *>(this);\
		else return E_NOINTERFACE;\
		return S_OK;\
	}\
	ULONG CALLBACK AddRef(){return 1;}\
	ULONG CALLBACK Release(){return 1;}

#define IJTS_BASE_IMPLEMENTATION1(interface1_name, interface2_name) \
	HRESULT CALLBACK GetGUID(GUID &guid)\
	{\
		guid = IID_##interface1_name;\
		return S_OK;\
	}\
	HRESULT CALLBACK QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)\
	{\
		*ppvObject = NULL;\
		if(::memcmp(&riid,&__uuidof(IUnknown),sizeof(GUID)) == 0) \
		*ppvObject = static_cast<IUnknown *>(this);\
		else if(::memcmp(&riid,&IID_##interface1_name,sizeof(GUID)) == 0) \
		*ppvObject = static_cast<interface1_name *>(this);\
		else if(::memcmp(&riid,&IID_##interface2_name,sizeof(GUID)) == 0) \
		*ppvObject = static_cast<interface2_name *>(this);\
		else return E_NOINTERFACE;\
		return S_OK;\
	}\
	ULONG CALLBACK AddRef(){return 1;}\
	ULONG CALLBACK Release(){return 1;}

#define IUNKNOWN_IMPLEMENTATION(interface_name) \
	HRESULT CALLBACK QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)\
	{\
		*ppvObject = NULL;\
		if(::memcmp(&riid,&__uuidof(IUnknown),sizeof(GUID)) == 0)\
		*ppvObject = static_cast<IUnknown *>(this);\
		else if(::memcmp(&riid,&__uuidof(interface_name),sizeof(GUID)) == 0)\
		{*ppvObject = static_cast<interface_name *>(this); AddRef();}\
		else return E_NOINTERFACE;\
		return S_OK;\
	}\
	ULONG CALLBACK AddRef() { return 1; }\
	ULONG CALLBACK Release() { return 1; }

#else
#define IJTS_BASE_IMPLEMENTATION(interface_name) \
	HRESULT CALLBACK GetGUID(GUID &guid)\
	{\
		guid = IID_##interface_name;\
		return S_OK;\
	}

#define IJTS_BASE_IMPLEMENTATION1(interface1_name, interface2_name) \
	HRESULT CALLBACK GetGUID(GUID &guid)\
	{\
		guid = IID_##interface1_name;\
		return S_OK;\
	}

#define IUNKNOWN_IMPLEMENTATION(interface_name) 
#endif
#endif // !_CORE_H