/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 */

#include "stdafx.h"
#include <MBCTYPE.H>
#ifdef _MERGE_PROXYSTUB
#include "RScript.h"
#else
#include "GRScript.h"
#endif
#include "RScriptCore.h"

#include "win32ole.h"
#include "eventsink.h"
#include "ItemDisp.h"
#include "Initializer.h"
#include "ScrError.h"
#if defined(__IRubyEngine_INTERFACE_DEFINED__)
#include "RubyWrapper.h"
#endif

extern "C"
{
void rb_define_class_variable(VALUE klass, const char* name, VALUE val);
}

/////////////////////////////////////////////////////////////////////////////
// CRScriptCore

EngineList CRScriptCore::s_listEngine;
CComAutoCriticalSection CRScriptCore::s_crit;
long CRScriptCore::s_lCookie(0);

#ifndef __IRubyEngine_INTERFACE_DEFINED__
CRScriptCore* CRScriptCore::s_pRScriptCore = NULL;
LPBYTE CRScriptCore::s_pStackTop = NULL;

CRScriptCore* CRScriptCore::GetCurrentEngine()
{
	return s_pRScriptCore;
}
#else
ID CRScriptCore::s_idInstanceEval(Qnil);
ID CRScriptCore::s_idModuleEval(Qnil);
#endif

CRScriptCore::CRScriptCore()
		: m_pSite(NULL),
		  m_state(SCRIPTSTATE_UNINITIALIZED),
		  m_dwThreadID(GetCurrentThreadId()),
		  m_threadState(SCRIPTTHREADSTATE_NOTINSCRIPT),
		  m_pScM(NULL),
		  m_pProv(NULL),
		  m_dwSafety(0), m_dwBaseSecurityLevel(0), m_nStartLinePersistent(0)
{
	ATLTRACE(_("ScriptCore Creation in Thread:%08X\n"), GetCurrentThreadId());
	m_pUnkMarshaler = NULL;
        CInitializer::GetInstance().InitNew();

}

static char* asr_argv[] = {"ActiveScriptRuby", "-e", ";", NULL};

void CRScriptCore::InitializeEnvironment()
{
	ATLTRACE(_("Initialize Environment in Thread:%08X\n"), GetCurrentThreadId());
#ifndef __IRubyEngine_INTERFACE_DEFINED__
	int stacktop;
	s_pStackTop = reinterpret_cast<LPBYTE>(&stacktop);
#endif
        int dummyargc(1);
        char* dummyargv[] = {"dummy", NULL };
        char** pargv;
	ruby_sysinit(&dummyargc, &pargv);
	RUBY_INIT_STACK;
	ruby_init();
	ruby_options(3, asr_argv);
	try
	{
		rb_require("win32ole");
		VALUE v = rb_eval_string("WIN32OLE");
		// override original constructor
		rb_define_singleton_method(v, "new", reinterpret_cast<VALUE(*)(...)>(fole_s_new), -1);
		rb_define_singleton_method(v, "connect", reinterpret_cast<VALUE(*)(...)>(fole_s_connect), 1);
		rb_define_singleton_method(v, "attach", reinterpret_cast<VALUE(*)(...)>(foleex_attach), 2);
		rb_define_method(v, "__release", reinterpret_cast<VALUE(*)(...)>(foleex_release), 0);
		rb_define_method(v, "method_missing", reinterpret_cast<VALUE(*)(...)>(foleex_missing), -1);
		rb_define_method(v, "each", reinterpret_cast<VALUE(*)(...)>(foleex_each), 0);
		if (rb_const_defined_at(v, rb_intern("ARGV")) == Qfalse)
		{
			rb_define_const(v, "ARGV", rb_ary_new());
		}
		s_valueWin32Ole = v;
		s_valueWIN32OLERuntimeError = rb_eval_string("WIN32OLERuntimeError");
		v = rb_define_class("WIN32OLEEX", v);
		s_valueWin32OleEx = v;
		rb_define_singleton_method(v, "attach", reinterpret_cast<VALUE(*)(...)>(foleex_attach), 2);
		rb_define_method(v, "method_missing", reinterpret_cast<VALUE(*)(...)>(foleex_missing), -1);
		//
		s_valueActiveRubyScript = rb_define_class("ActiveScriptRuby", rb_cObject);
		rb_define_singleton_method(s_valueActiveRubyScript, "trace", (VALUE(*)(...))trace_hook, 6);
		rb_define_singleton_method(s_valueActiveRubyScript, "settrace", (VALUE(*)(...))trace_set, 1);
		rb_define_singleton_method(s_valueActiveRubyScript, "rubyize", (VALUE(*)(...))rubyize, 1);
	}
	catch (...)
	{
		ATLTRACE(_T("Exception for class\n"));
	}
}

int CRScriptCore::s_lastline(0);
bool CRScriptCore::s_fRaiseException(false);

VALUE __cdecl CRScriptCore::trace_set(VALUE self, VALUE v)
{
	switch (v)
	{
	case Qtrue:
		if (s_fTrace == false)
		{
			s_fTrace = true;
			TraceOn();
		}
		break;
	case Qfalse:
		if (s_fTrace)
		{
			s_fTrace = false;
			TraceOff();
		}
		break;
	default:
        rb_raise(rb_eTypeError, "not valid value");
		break;
	}
	return s_fTrace;
}

void CRScriptCore::TraceOff()
{
#if 0
	int state;
	rb_protect((VALUE(*)())rb_eval_string, (VALUE)"set_trace_func nil", &state);
#endif
}

VALUE __cdecl CRScriptCore::trace_hook(VALUE self, VALUE event, VALUE file, VALUE line, VALUE id, VALUE binding, VALUE klass)
{
	ATLTRACE(_T("trace:event=%s, file=%s, line=%d, binding=%08X Thread=%08X\n"), RSTRING_PTR(event), RSTRING_PTR(file), NUM2INT(line), binding, GetCurrentThreadId());
	if (!s_fRaiseException)
	{
		if (strcmp(RSTRING_PTR(event), "raise") == 0)
		{
			s_lastline = NUM2INT(line);
			s_fRaiseException = true;
		}
	}
	return Qnil;
}

void CRScriptCore::TraceOn()
{
#if 0
	if (s_fTrace == false) return;

	s_fRaiseException = false;
	s_lastline = 0;
	int state;
	rb_protect((VALUE(*)())rb_eval_string, (VALUE)"set_trace_func proc{|event, file, line, id, binding, klass|\r\n"
					   "  ActiveScriptRuby.trace event, file, line, id, binding, klass\r\n"
					   "}\r\n", &state);
#endif
}

HRESULT CRScriptCore::FinalConstruct()
{
	s_crit.Lock();
	m_lCookie = ++s_lCookie;	// for WIN95 - InterlockedIncrement(&s_lCookie);
	if (m_lCookie >= FIXNUM_MAX)
	{
		m_lCookie = s_lCookie = 0;
	}
	s_listEngine.push_front(this);
	s_crit.Unlock();
	return CoCreateFreeThreadedMarshaler(
		GetControllingUnknown(), &m_pUnkMarshaler.p);
}

void CRScriptCore::FinalRelease()
{
	s_crit.Lock();
	for (EngineListIter it = s_listEngine.begin(); it != s_listEngine.end(); it++)
	{
		if (*it == this)
		{
			s_listEngine.erase(it);
			break;
		}
	}
	s_crit.Unlock();
	m_pUnkMarshaler.Release();
}

#if defined(__IRubyEngine_INTERFACE_DEFINED__)
	static const char prefix = '@';
#else
	static const char prefix = '$';
#endif

// invoked in the main thread
void CRScriptCore::DefineGlobalProperties(LPCSTR pObjName)
{
	static char merge[] = "begin\r\n"
		                     "%c%s.ole_put_methods.map{|x| x.name} | %c%s.ole_get_methods.map{|x| x.name}\r\n"
					      "rescue RuntimeError\r\n"
						     "[]\r\n"
						  "end\r\n";
	size_t alloclen = sizeof(merge) + strlen(pObjName) * 2 + 16;
	char* p = reinterpret_cast<LPSTR>(alloca(alloclen + 1));
	int cb = sprintf(p, merge, prefix, pObjName, prefix, pObjName);
#if defined(__IRubyEngine_INTERFACE_DEFINED__)
        VALUE args[] = { rb_str_new(p, cb), rb_str_new("ActiveScriptRuby", 16), INT2NUM(1) };
        VALUE a = rb_obj_instance_eval(3, args, GetModuleValue());
#else
	VALUE a = rb_eval_string(p);
#endif
	for (int i = 0; i < RARRAY_LEN(a); i++)
	{
		VALUE name = rb_ary_entry(a, i);
		char* prop = StringValueCStr(name);
		if (strlen(prop) > alloclen)
		{
			continue;	// too long name
		}
		sprintf(p, "$%s", prop);
		rb_define_virtual_variable(p, (VALUE(*)(ANYARGS))GlobalGetter, (void(*)(ANYARGS))GlobalSetter);
	}
}

VALUE __cdecl CRScriptCore::GlobalGetter(ID id)
{
	static char getter[] = "%c_asr_default_object.%s";
	const char* name = rb_id2name(id);
	char* p = reinterpret_cast<char*>(alloca(strlen(name) + sizeof(getter) + 8));
	int cb = sprintf(p, getter, prefix, name + 1);
#if defined(__IRubyEngine_INTERFACE_DEFINED__)
	return eval_string(p, cb);
#else
	return rb_eval_string(p);
#endif
}

void __cdecl CRScriptCore::GlobalSetter(VALUE val, ID id, VALUE* var)
{
	static char object[] = "%c_asr_default_object";
	const char* name = rb_id2name(id);
	char* p = reinterpret_cast<char*>(alloca(sizeof(object) + 4));
	int cb = sprintf(p, object, prefix);
#if defined(__IRubyEngine_INTERFACE_DEFINED__)
	VALUE o = eval_string(p, cb);
#else
	VALUE o = rb_eval_string(p);
#endif
	sprintf(p, "%s=", name + 1);
	rb_funcall(o, rb_intern(p), 1, val);
}

// invoked in the main thread
void CRScriptCore::DefineGlobalMethods(LPCSTR pObjName)
{
	static char methoddef[] = "def %s(*a)\r\n"
		                      "  %c%s.%s(*a)\r\n"
						 	  "end\r\n";
	static const int MAX_PROPNAME = 64;
	char* p = reinterpret_cast<LPSTR>(alloca(sizeof(methoddef) + strlen(pObjName) + MAX_PROPNAME * 2 + 8));
	int state;
    VALUE param[] = {
		GetOleObject(s_valueWin32OleEx, m_strGlobalObjectName.c_str()), 
	    rb_intern("ole_func_methods"), 
		0,
	};
    VALUE a = rb_protect((VALUE(*)(VALUE))funcall, (VALUE)param, &state);
	if (state)
	{	// maybe no typelib
		return;
	}
	ID name = rb_intern("name");
	for (int i = 0; i < RARRAY_LEN(a); i++)
	{
		VALUE o = rb_ary_entry(a, i);
		VALUE propname = rb_funcall(o, name, 0);
		char* prop = StringValueCStr(propname);
		if (strlen(prop) > MAX_PROPNAME) 
		{
			continue; // skip, the name is too long
		}
#define LOWER_PROP_NAME
#ifdef LOWER_PROP_NAME
		char method[65];
		strcpy(method, prop);
		method[0] = tolower(method[0]);
		int cb = sprintf(p, methoddef, method, prefix, pObjName, prop);
#else
		int cb = sprintf(p, methoddef, prop, prefix, pObjName, prop);
#endif
#if defined(__IRubyEngine_INTERFACE_DEFINED__)
                VALUE args[] = { rb_str_new(p, cb), rb_str_new("ActiveScriptRuby", 16), INT2NUM(1) };
                rb_obj_instance_eval(3, args, GetModuleValue());
#else
		rb_eval_string(p);
#endif
	}
}

#if defined(__IRubyEngine_INTERFACE_DEFINED__)
// caller must be in the main thread
VALUE CRScriptCore::eval_string(char* p, int cb)
{
	VALUE module = Qnil;
	CRubyWrapper::GetCWrapper()->GetCurrentEngine()->GetModule((DWORD*)&module);
        VALUE args[] = { rb_str_new(p, cb), rb_str_new("ActiveScriptRuby", 16), INT2NUM(1) };
        return rb_obj_instance_eval(3, args, module);
}
#endif
VALUE __cdecl CRScriptCore::funcall(VALUE v[])
{
	return rb_funcall(v[0], v[1], (int)v[2]);
}

VALUE CRScriptCore::GetOleObject(VALUE self, char* pName)
{
	USES_CONVERSION;
	return GetOleObject(self, A2W(pName));
}

VALUE CRScriptCore::GetOleObject(VALUE self, LPCOLESTR pstrName)
{
	ItemMapIter it = m_mapItem.find(pstrName);
	if (it == m_mapItem.end())
	{
		it = (m_mapItem.insert(ItemMap::value_type(pstrName, new CItemHolder))).first;
	}

	VALUE obj = Qnil;
	IDispatch* pDisp = NULL;
	IActiveScriptSite* pSite = GetSite();
	if (pSite)
	{
		pDisp = (*it).second->GetDispatch(pSite, const_cast<LPOLESTR>(pstrName), (m_dwThreadID == GetCurrentThreadId()));
		pSite->Release();
	}
	if (pDisp)
	{
		oledataex *pole;
		// setting free function is not required. because ScriptEngine hold it.
		obj = Data_Make_Struct(self, oledataex, 0, 0, pole);
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
		IDispatchEx* pEx;
		if (QueryInterface(IID_IRubyEngine, (void**)&pole->pEngine) == S_OK)
		{
			// adjust ref count (because never release the ptr in WIN32OLEEX)
			pole->pEngine->Release();
		}
		if (pDisp->QueryInterface(IID_IDispatchEx, (void**)&pEx) == S_OK)
		{
			pole->pDispatch = pEx;
			pole->pServiceProvider = this;
			pDisp->Release();
		}
		else
#endif
		{
			pole->pDispatch = pDisp;
		}
		ATLTRACE(_T("add OLE Object into list:%08X\n"), pole);
		m_listOleObj.push_back(reinterpret_cast<oledata*>(pole));
	}
	return obj;
}

IActiveScriptSite* CRScriptCore::GetSite()
{
	m_pSite->AddRef();
	return m_pSite;
}

void CRScriptCore::PushServiceProvider(IServiceProvider* psp)
{
	psp->AddRef();
	s_crit.Lock();
	m_listServiceProvider.push_front(psp);
	s_crit.Unlock();
}

void CRScriptCore::PopServiceProvider()
{
	IServiceProvider* p = NULL;
	s_crit.Lock();
	if (m_listServiceProvider.size() > 0)
	{
		p = m_listServiceProvider.front();
		m_listServiceProvider.pop_front();
	}
	s_crit.Unlock();
	if (p)
		p->Release();
}

CRScriptCore* CRScriptCore::GetEngine(long lCookie)
{
	CRScriptCore* pResult = NULL;
	s_crit.Lock();
	for (EngineListIter it = s_listEngine.begin(); it != s_listEngine.end(); it++)
	{
		if ((*it)->GetCookie() == lCookie)
		{
			pResult = *it;
			break;
		}
	}
	s_crit.Unlock();
	return pResult;
}

VALUE CRScriptCore::GetModuleValue()
{
	return rb_mKernel;
}

//
// IObjectSafety
//
HRESULT STDMETHODCALLTYPE CRScriptCore::SetInterfaceSafetyOptions( REFIID riid, DWORD dwOptionsSetMask, DWORD dwEnabledOptions )
{
	ATLTRACE(_T("SetSafety Mask = %08X, Option = %08X\n"), dwOptionsSetMask, dwEnabledOptions);
	if (dwOptionsSetMask & INTERFACESAFE_FOR_UNTRUSTED_CALLER)
	{
		// script from untrusted caller (This for ActiveXs never called)
		if (dwEnabledOptions & INTERFACESAFE_FOR_UNTRUSTED_CALLER)
		{
			m_dwSafety |= INTERFACESAFE_FOR_UNTRUSTED_CALLER;
		}
		else
		{
			m_dwSafety &= ~INTERFACESAFE_FOR_UNTRUSTED_CALLER;
		}
	}
	if (dwOptionsSetMask & INTERFACESAFE_FOR_UNTRUSTED_DATA)
	{
		// internet, localhost, trusted site...
		if (dwEnabledOptions & INTERFACESAFE_FOR_UNTRUSTED_DATA)
		{
			m_dwSafety |= INTERFACESAFE_FOR_UNTRUSTED_DATA;
		}
		else
		{
			m_dwSafety &= ~INTERFACESAFE_FOR_UNTRUSTED_DATA;
		}
	}
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	if (dwOptionsSetMask & INTERFACE_USES_SECURITY_MANAGER)
	{
		if (dwEnabledOptions & INTERFACE_USES_SECURITY_MANAGER)
		{
			m_dwSafety |= INTERFACE_USES_SECURITY_MANAGER;
		}
		else
		{
			m_dwSafety &= ~INTERFACE_USES_SECURITY_MANAGER;
		}
	}
	if (dwOptionsSetMask & INTERFACE_USES_DISPEX)
	{
		if (dwEnabledOptions & INTERFACE_USES_DISPEX)
		{
			m_dwSafety |= INTERFACE_USES_DISPEX;
		}
		else
		{
			m_dwSafety &= ~INTERFACE_USES_DISPEX;
		}
	}
#endif
	if (m_dwSafety & INTERFACESAFE_FOR_UNTRUSTED_CALLER)
		m_dwBaseSecurityLevel = 4;	
	else if (m_dwSafety & INTERFACESAFE_FOR_UNTRUSTED_DATA)
		m_dwBaseSecurityLevel = 3;
	else
		m_dwBaseSecurityLevel = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRScriptCore::GetInterfaceSafetyOptions( REFIID riid, DWORD* pdwSupportedOptions, DWORD* pdwEnabledOptions )
{
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
 #ifdef SUPPORT_IE_EMBED
	*pdwEnabledOptions = m_dwSafety;
	*pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA
		| INTERFACE_USES_DISPEX | INTERFACE_USES_SECURITY_MANAGER;
 #else
	*pdwEnabledOptions = m_dwSafety = 0;
 #endif
#else
	*pdwEnabledOptions = m_dwSafety = 0;
#endif
	return S_OK;
}

//
// IServiceProvider
HRESULT STDMETHODCALLTYPE CRScriptCore::QueryService(
	REFGUID guidService,
	REFIID riid,
	void **ppv)
{
	if (!ppv) return E_POINTER;
	HRESULT hr = E_NOINTERFACE;
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	if (m_listServiceProvider.size() > 0)
	{
		IServiceProvider* p = m_listServiceProvider.front();
		hr = p->QueryService(guidService, riid, ppv);
	}
	else
	{
		*ppv = NULL;
		if (InlineIsEqualGUID(guidService, SID_GetCaller)
			|| InlineIsEqualGUID(guidService, IID_IActiveScriptSite))
		{
			IActiveScriptSite* pSite = GetSite();
			if (pSite)
			{
				hr = pSite->QueryInterface(riid, ppv);
			}
			pSite->Release();
		}
		else if (m_pProv)
		{
			hr = m_pProv->QueryService(guidService, riid, ppv);
		}
	}
#endif
	return hr;
}

//
// IActiveScript
//
HRESULT STDMETHODCALLTYPE CRScriptCore::SetScriptSite( 
            /* [in] */ IActiveScriptSite __RPC_FAR *pass)
{
	if (!pass) return E_POINTER;
	if (m_pSite) return E_UNEXPECTED;

	MakeScope();

	pass->AddRef();
	m_pSite = pass;

#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	if (m_dwSafety & INTERFACE_USES_SECURITY_MANAGER)
	{
		IServiceProvider* pProv = NULL;
		HRESULT hr = m_pSite->QueryInterface(IID_IServiceProvider, (void**)&pProv);
		if (hr == S_OK)
		{
			IInternetHostSecurityManager* pScm = NULL;
			hr = pProv->QueryService(SID_SInternetHostSecurityManager, IID_IInternetHostSecurityManager, (void**)&pScm);
			if (hr == S_OK)
			{
				BYTE b[_MAX_PATH];
				DWORD dw(sizeof(b));
				hr = pScm->GetSecurityId(b, &dw, 0);
				m_pScM = pScm;
			}
			m_pProv = pProv;
		}
	}
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRScriptCore::GetScriptSite( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
{
	if (!ppvObject) return E_POINTER;
	*ppvObject = NULL;
	if (!m_pSite) return S_FALSE;
	return m_pSite->QueryInterface(riid, ppvObject);
}
        
HRESULT STDMETHODCALLTYPE CRScriptCore::SetScriptState( 
            /* [in] */ SCRIPTSTATE ss)
{
	ATLTRACE(_T("SetScriptState %08X\n"), ss);
	if (!m_pSite) return E_UNEXPECTED;
	if (m_state != ss)
	{
		switch (ss)
		{
		case SCRIPTSTATE_STARTED:
			BindNamedItem();
			break;
		case SCRIPTSTATE_CONNECTED:
			Connect();
			BindNamedItem();
			ConnectEvents();
			break;
		case SCRIPTSTATE_INITIALIZED:
			Disconnect(true);	// Disconnect all Sink
			UnbindNamedItem();
			break;
		case SCRIPTSTATE_DISCONNECTED:
			Disconnect(true);	// Disconnect all Sink
			break;
		case SCRIPTSTATE_UNINITIALIZED:
			return E_FAIL;

//			Uninitialize();
			break;
		default:
			break;
		}

		m_state = ss;
		if (ss != SCRIPTSTATE_UNINITIALIZED)
		{
			m_pSite->OnStateChange(ss);
		}
		return S_OK;
	}
	return S_FALSE;
}
        
HRESULT STDMETHODCALLTYPE CRScriptCore::GetScriptState( 
            /* [out] */ SCRIPTSTATE __RPC_FAR *pssState)
{
	if (!pssState) return E_POINTER;
	*pssState = m_state;
	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CRScriptCore::Close( void)
{
	if (!m_pSite) return S_FALSE;

	m_state = SCRIPTSTATE_CLOSED;
	Disconnect();
#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	if (m_pScM)
	{
		m_pScM->Release();
		m_pScM = NULL;
	}
	if (m_pProv)
	{
		m_pProv->Release();
		m_pProv = NULL;
	}
#endif
	m_pSite->Release();
	m_pSite = NULL;
	return S_OK;
}

void CRScriptCore::Uninitialize()
{
	if (!m_pSite) return;
	UnbindNamedItem();
	m_state = SCRIPTSTATE_UNINITIALIZED;
	m_pSite->OnStateChange(SCRIPTSTATE_UNINITIALIZED);

#ifdef __IRubyWrapper_INTERFACE_DEFINED__
	if (m_pScM)
	{
		m_pScM->Release();
		m_pScM = NULL;
	}
	if (m_pProv)
	{
		m_pProv->Release();
		m_pProv = NULL;
	}
#endif
	m_pSite->Release();
	m_pSite = NULL;
}

HRESULT STDMETHODCALLTYPE CRScriptCore::AddNamedItem( 
            /* [in] */ LPCOLESTR pstrName,
            /* [in] */ DWORD dwFlags)
{
	if (!pstrName) return E_POINTER;

	ATLTRACE(_T("AddNameItem %ls flag=%08X\n"), pstrName, dwFlags);

	if (!(dwFlags & SCRIPTITEM_ISVISIBLE)) return S_OK;

	if (dwFlags & SCRIPTITEM_GLOBALMEMBERS)
		m_strGlobalObjectName = pstrName;

	LPOLESTR p = wcscpy(new OLECHAR[wcslen(pstrName) + 1], pstrName);
	ItemMapIter it = m_mapItem.find(p);
	if (it != m_mapItem.end())
	{
		if ((*it).second)
			delete (*it).second;
	}
	m_mapItem.insert(ItemMap::value_type(p, new CItemHolder(dwFlags)));
	if (m_state == SCRIPTSTATE_STARTED || m_state == SCRIPTSTATE_CONNECTED)
	{
		AddNamedItem(pstrName);
	}
	return S_OK;
}


HRESULT CRScriptCore::LoadTypeLib(
            /* [in] */ REFGUID rguidTypeLib,
            /* [in] */ DWORD dwMajor,
            /* [in] */ DWORD dwMinor,
			/* [out]*/ ITypeLib** ppResult)
{
	OLECHAR strGuid[64];
	StringFromGUID2(rguidTypeLib, strGuid, 64);
	char key[MAX_PATH];
	sprintf(key, "TypeLib\\%ls\\%d.%d", strGuid, dwMajor, dwMinor);
	HKEY hTypeLib;
	if (RegOpenKeyExA(HKEY_CLASSES_ROOT, key, 0, KEY_READ, &hTypeLib) != ERROR_SUCCESS)
	{
		return E_INVALIDARG;
	}
	bool found(false);
	FILETIME ft;
	char buff[MAX_PATH];
	for (int i = 0; !found; i++)
	{
		DWORD szbuff = sizeof(buff);
		if (RegEnumKeyExA(hTypeLib, i, buff, &szbuff, NULL, NULL, NULL, &ft) != ERROR_SUCCESS)
		{
			break;
		}
		strcat(buff, "\\win32");
		HKEY hWin32;
		if (RegOpenKeyExA(hTypeLib, buff, 0, KEY_READ, &hWin32) != ERROR_SUCCESS)
		{
			break;
		}
		szbuff = sizeof(buff);
		if (RegQueryValueExA(hWin32, NULL, NULL, NULL, reinterpret_cast<LPBYTE>(&buff), &szbuff) == ERROR_SUCCESS)
		{
			found = true;
		}
		RegCloseKey(hWin32);
	}
	RegCloseKey(hTypeLib);
	if (!found)
	{
		return E_INVALIDARG;
	}
	USES_CONVERSION;
	if (::LoadTypeLib(A2W(buff), ppResult) != S_OK)
	{
		return TYPE_E_CANTLOADLIBRARY;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRScriptCore::AddTypeLib( 
            /* [in] */ REFGUID rguidTypeLib,
            /* [in] */ DWORD dwMajor,
            /* [in] */ DWORD dwMinor,
            /* [in] */ DWORD dwFlags)
{
	ATLTRACE(_T("AddTypeLib\n"));
	ITypeLib* pLib;
	HRESULT hr = LoadTypeLib(rguidTypeLib, dwMajor, dwMinor, &pLib);
	if (hr != S_OK)
	{
		return hr;
	}

	int count = pLib->GetTypeInfoCount();
	for (int i = 0; i < count; i++)
	{
		ITypeInfo* pInfo;
		if (pLib->GetTypeInfo(i, &pInfo) != S_OK)
		{
			continue;
		}
		TYPEATTR* pattr;
		if (pInfo->GetTypeAttr(&pattr) != S_OK)
		{
			pInfo->Release();
			continue;
		}
		VARDESC* pvardesc;
		BSTR strName;
		UINT pc;
		for (int iv = 0; iv < pattr->cVars; iv++)
		{
			if (pInfo->GetVarDesc(iv, &pvardesc) != S_OK)
			{
				continue;
			}
			strName = NULL;
            if(pvardesc->varkind == VAR_CONST &&
               !(pvardesc->wVarFlags & (VARFLAG_FHIDDEN | VARFLAG_FRESTRICTED | VARFLAG_FNONBROWSABLE))) {
				if (pInfo->GetNames(pvardesc->memid, &strName, 1, &pc) != S_OK || pc == 0 || !strName)
				{
					continue;
				}
				DefineConstant(strName, pvardesc->lpvarValue);
				SysFreeString(strName);
			}
			pInfo->ReleaseVarDesc(pvardesc);
		}
		pInfo->ReleaseTypeAttr(pattr);
		pInfo->Release();
	}
        return S_OK;
//	m_listTypeLib.push_back(pLib);
}

HRESULT STDMETHODCALLTYPE CRScriptCore::GetScriptDispatch( 
            /* [in] */ LPCOLESTR pstrItemName,
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdisp)
{
	if (!ppdisp) return E_POINTER;
	*ppdisp = NULL;
	ATLTRACE(_T("GetScriptDispatch for %ls\n"), (pstrItemName) ? pstrItemName : L"GLOBALNAMESPACE");
	if (!pstrItemName || m_strGlobalObjectName == pstrItemName)
        {
		IDispatch* pDisp = NULL;
                ItemMapIter it = (pstrItemName) ? m_mapItem.find(pstrItemName) : m_mapItem.end();
		if (it != m_mapItem.end())
		{
			pDisp = (*it).second->GetDispatch(m_pSite, const_cast<LPOLESTR>(pstrItemName), (m_dwThreadID == GetCurrentThreadId()));
                        pDisp->AddRef();
                }
		if ((*ppdisp = GetOuterDispatch(pDisp)) == NULL)
                {
                    return S_FALSE;
                }
                (*ppdisp)->AddRef();
	}
	else
	{
		ATLTRACE(_T("GetScriptDispatch for %ls\n"), pstrItemName);

		ItemMapIter it = m_mapItem.find(pstrItemName);
		if (it != m_mapItem.end())
		{
			IDispatch* pDisp = (*it).second->GetDispatch(m_pSite, const_cast<LPOLESTR>(pstrItemName), (m_dwThreadID == GetCurrentThreadId()));
			*ppdisp = pDisp;
		}
		else
		{
			return InternalGetScriptDispatch(pstrItemName, ppdisp);
		}
	}
	return S_OK;
}

HRESULT CRScriptCore::InternalGetScriptDispatch(LPCOLESTR pstrItemName, IDispatch** ppdisp)
{
	*ppdisp = NULL;
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CRScriptCore::GetCurrentScriptThreadID( 
            /* [out] */ SCRIPTTHREADID __RPC_FAR *pstidThread)
{
	if (!pstidThread) return E_POINTER;
	*pstidThread = m_dwThreadID;
	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CRScriptCore::GetScriptThreadID( 
            /* [in] */ DWORD dwWin32ThreadId,
            /* [out] */ SCRIPTTHREADID __RPC_FAR *pstidThread)
{
	if (pstidThread) return E_POINTER;
	HRESULT hr = E_UNEXPECTED;
	s_crit.Lock();
	for (EngineListIter it = s_listEngine.begin(); it != s_listEngine.end(); it++)
	{
		if ((*it)->GetThreadID() == dwWin32ThreadId)
		{
			*pstidThread = (*it)->GetThreadID();
			hr = S_OK;
			break;
		}
	}
	s_crit.Unlock();
	return hr;
}
        
HRESULT STDMETHODCALLTYPE CRScriptCore::GetScriptThreadState( 
            /* [in] */ SCRIPTTHREADID stidThread,
            /* [out] */ SCRIPTTHREADSTATE __RPC_FAR *pstsState)
{
	ATLTRACE(_T("GetScriptThreadState %08X\n"), stidThread);
	if (pstsState) return E_POINTER;
	HRESULT hr = S_OK;
	switch (stidThread)
	{
	case SCRIPTTHREADID_CURRENT:
	case SCRIPTTHREADID_BASE:
		*pstsState = SCRIPTTHREADSTATE_NOTINSCRIPT;
		break;
	default: 
		{
			
			hr = E_UNEXPECTED;
			s_crit.Lock();
			for (EngineListIter it = s_listEngine.begin(); it != s_listEngine.end(); it++)
			{
				if ((*it)->GetThreadID() == stidThread)
				{
					*pstsState = (*it)->GetThreadState();
					hr = S_OK;
					break;
				}
			}
			s_crit.Unlock();
		}
		break;
	}
	return hr;
}

static void interruptThread()
{
	ATLTRACE(_T("interruptThread()\n"));
	rb_raise(rb_eInterrupt, "interrput");
}

void CRScriptCore::InterruptThread(HANDLE h)
{
	ATLTRACE(_T("InterruptThread(%08X)\n"), h);
	CONTEXT con;
	ZeroMemory(&con, sizeof(CONTEXT));
	con.ContextFlags = CONTEXT_CONTROL;
	GetThreadContext(h, &con);
	FARPROC proc = (FARPROC)&interruptThread;
	con.ContextFlags = CONTEXT_CONTROL;
#if !defined(_M_X64)
	con.Eip = (long)proc;
#else
	con.Rip = (DWORD64)proc;
#endif
	SetThreadContext(h, &con);
}

HRESULT STDMETHODCALLTYPE CRScriptCore::Clone( 
            /* [out] */ IActiveScript __RPC_FAR *__RPC_FAR *ppscript)
{
	ATLTRACE(_T("Clone\n"));
	ATLTRACENOTIMPL(_("Clone"));
}

//
// IActiveScriptParse
//
HRESULT STDMETHODCALLTYPE CRScriptCore::InitNew( void)
{
	ATLTRACE(_T("InitNew\n"));
	Disconnect();
	if (m_pSite)
	{
		m_pSite->OnStateChange(m_state = SCRIPTSTATE_INITIALIZED);
	}
	return S_OK;
}

CScriptlet::CScriptlet(LPCOLESTR code, LPCOLESTR item, LPCOLESTR subitem, LPCOLESTR event, ULONG startline) :
	m_startline(startline)
{
	if (code) m_code = code;
	if (item) m_item = item;
	if (subitem) m_subitem = subitem;
	if (event) m_event = event;
}

HRESULT CScriptlet::Add(CRScriptCore* pCore)
{
	BSTR bstr = NULL;
	EXCEPINFO excep;
	return pCore->AddScriptlet(L"", m_code.c_str(), m_item.c_str(), m_subitem.c_str(), m_event.c_str(),
		L";", SCRIPTTEXT_ISVISIBLE , m_startline, 0, &bstr, &excep);
}
        
HRESULT STDMETHODCALLTYPE CRScriptCore::AddScriptlet( 
            /* [in] */ LPCOLESTR pstrDefaultName,
            /* [in] */ LPCOLESTR pstrCode,
            /* [in] */ LPCOLESTR pstrItemName,
            /* [in] */ LPCOLESTR pstrSubItemName,
            /* [in] */ LPCOLESTR pstrEventName,
            /* [in] */ LPCOLESTR pstrDelimiter,
#if defined(_M_X64)
            /* [in] */ DWORDLONG dwSourceContextCookie,
#else
            /* [in] */ DWORD dwSourceContextCookie,
#endif
            /* [in] */ ULONG ulStartingLineNumber,
            /* [in] */ DWORD dwFlags,
            /* [out] */ BSTR __RPC_FAR *pbstrName,
            /* [out] */ EXCEPINFO __RPC_FAR *pexcepinfo)
{
	ATLTRACE(_("AddScriptlet for %ls, cookie=%d, Flags=%d\n"), pstrEventName, dwSourceContextCookie, dwFlags);

	if (!(dwFlags | SCRIPTTEXT_ISVISIBLE)) return S_OK;

	if (m_state != SCRIPTSTATE_CONNECTED)
	{
		m_listScriptlets.push_back(CScriptlet(pstrCode, pstrItemName, pstrSubItemName, pstrEventName, ulStartingLineNumber));
		return S_OK;
	}

	if (pstrItemName && pstrEventName && pstrCode)
	{
		HRESULT hr = E_UNEXPECTED;
		ItemMapIter it = m_mapItem.find(pstrItemName);
		if (it != m_mapItem.end())
		{
			EventMapIter itev = m_mapEvent.end();
			IDispatch* pDisp = (*it).second->GetDispatch(m_pSite, const_cast<LPOLESTR>(pstrItemName), (m_dwThreadID == GetCurrentThreadId()));
			if (pstrSubItemName)
			{
				itev = m_mapEvent.find(pstrSubItemName);
				if (itev == m_mapEvent.end())
				{
					itev = (m_mapEvent.insert(EventMap::value_type(pstrSubItemName, new CEventSink(this)))).first;
				}
				(*itev).second->AddRef();
				hr = (*itev).second->Advise(pDisp, const_cast<LPOLESTR>(pstrSubItemName));
			}
			else
			{
				itev = m_mapEvent.find(pstrItemName);
			}
			if (pDisp)
				pDisp->Release();

			if (itev != m_mapEvent.end())
			{
				hr = (*itev).second->ResolveEvent(pstrEventName, ulStartingLineNumber, pstrCode);
			}
			if (hr == S_OK)
			{
				*pbstrName = SysAllocString(pstrDefaultName);
			}
			else
			{
				USES_CONVERSION;

				CComBSTR bstr(pstrSubItemName);
				bstr += L"_";
				bstr += pstrEventName;
				*pbstrName = bstr.Copy();
				VARIANT v;
				VariantInit(&v);
				size_t len = wcslen(pstrCode);
				LPSTR pScript = new char[len * 2 + bstr.Length() * 2 + 32];
				int n = sprintf(pScript, "def %s()\r\n  ", W2A(bstr.m_str));
				size_t m = WideCharToMultiByte(GetACP(), 0, pstrCode, (int)len, pScript + n, (int)len * 2 + 1, NULL, NULL);
				strcpy(pScript + n + m, "\r\nend\r\n");
				hr = ParseText(ulStartingLineNumber, pScript, pstrItemName, pexcepinfo, &v, dwFlags);
				delete[] pScript;
				VariantClear(&v);
			}
		}
		return hr;
	}
	return E_INVALIDARG;
}
        
HRESULT STDMETHODCALLTYPE CRScriptCore::ParseScriptText( 
            /* [in] */ LPCOLESTR pstrCode,
            /* [in] */ LPCOLESTR pstrItemName,
            /* [in] */ IUnknown __RPC_FAR *punkContext,
            /* [in] */ LPCOLESTR pstrDelimiter,
#if defined(_M_X64)
            /* [in] */ DWORDLONG dwSourceContextCookie,
#else
            /* [in] */ DWORD dwSourceContextCookie,
#endif
			/* [in] */ ULONG ulStartingLineNumber,
            /* [in] */ DWORD dwFlags,
            /* [out] */ VARIANT __RPC_FAR *pvarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo)
{
	if (!m_pSite) return E_UNEXPECTED;
#if 0
	ATLTRACE(_T("ParseScriptText: %*ls\n"), 128, pstrCode);
#endif
	size_t len = wcslen(pstrCode);
	LPSTR psz = new char[len * 2 + 1];
	size_t cb = WideCharToMultiByte(GetACP(), 0, pstrCode, (int)len, psz, (int)len * 2 + 1, NULL, NULL);
	*(psz + cb) = '\0';

	if ((dwFlags & (SCRIPTTEXT_ISPERSISTENT | SCRIPTTEXT_ISVISIBLE)) == (SCRIPTTEXT_ISPERSISTENT | SCRIPTTEXT_ISVISIBLE)
		&& m_state == SCRIPTSTATE_UNINITIALIZED)
	{
		m_nStartLinePersistent = ulStartingLineNumber;
		m_strScriptPersistent = psz;
		delete[] psz;
		return S_OK;
	}

	HRESULT hr = E_UNEXPECTED;
	try 
	{
		if (pvarResult)
		{
			VariantInit(pvarResult);
		}
		Connect();
		hr = ParseText(ulStartingLineNumber, psz, pstrItemName, pExcepInfo, pvarResult, dwFlags);
	}
	catch (...)
	{
	}
	delete[] psz;

	return hr;
}

// IActiveScriptParseProcedure
HRESULT STDMETHODCALLTYPE CRScriptCore::ParseProcedureText( 
            /* [in] */ LPCOLESTR pstrCode,
            /* [in] */ LPCOLESTR pstrFormalParams,
            /* [in] */ LPCOLESTR pstrProcedureName,
            /* [in] */ LPCOLESTR pstrItemName,
            /* [in] */ IUnknown __RPC_FAR *punkContext,
            /* [in] */ LPCOLESTR pstrDelimiter,
#if defined(_M_X64)
            /* [in] */ DWORDLONG dwSourceContextCookie,
#else
            /* [in] */ DWORD dwSourceContextCookie,
#endif
            /* [in] */ ULONG ulStartingLineNumber,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdisp)
{
	static int seqcnt(0);
	if (!(dwFlags | SCRIPTTEXT_ISVISIBLE)) return S_OK;
	if (!pstrCode || !ppdisp) return E_POINTER;
	*ppdisp = NULL;
	VARIANT v;
	VariantInit(&v);
	EXCEPINFO excep;
	memset(&excep, 0, sizeof(EXCEPINFO));

	USES_CONVERSION;
	LPCSTR ProcName = (pstrProcedureName) ? W2A(pstrProcedureName) : "undef";
	LPCSTR ItemName = (pstrItemName) ? W2A(pstrItemName) : "unkole";

	size_t len = wcslen(pstrCode);
	LPSTR psz = new char[len * 2 + strlen(ProcName) + strlen(ItemName) + 32];
	int n = sprintf(psz, "@%s_%s%d = Proc.new {\r\n ", ItemName, ProcName, seqcnt++);
	size_t m = WideCharToMultiByte(GetACP(), 0, pstrCode, (int)len, psz + n, (int)len * 2 + 1, NULL, NULL);
	strcpy(psz + n + m, "\r\n}");
	HRESULT hr = ParseText(ulStartingLineNumber, psz, pstrItemName, &excep, &v, dwFlags);
	if (v.vt == VT_DISPATCH)
	{
		*ppdisp = v.pdispVal;
	}
	else
	{
		VariantClear(&v);
	}
	delete[] psz;
	return S_OK;
}

// Called by EventSink
HRESULT CRScriptCore::Invoke(VALUE, int, int, LPCSTR pstr, VARIANT* pvResult, EXCEPINFO FAR* pExcepInfo, DWORD)
{
	return S_OK;
}

HRESULT CRScriptCore::OnScriptError(IActiveScriptError* p) 
{ 
	return m_pSite->OnScriptError(p); 
}
//
// Change ScriptEngine State to Connected by myself
//
HRESULT CRScriptCore::Connect()
{
	if (m_state == SCRIPTSTATE_CONNECTED) return S_OK;

	if (m_mapItem.size() <= 0) return S_OK;

	bool fSameApt = (m_dwThreadID == GetCurrentThreadId());
	for (ItemMapIter it = m_mapItem.begin(); it != m_mapItem.end(); it++)
	{
		if ((*it).second->IsOK() == false)
			AddNamedItem((*it).first.c_str());

		if ((*it).second->IsSource())
		{
			IDispatch* pDisp = (*it).second->GetDispatch(m_pSite, const_cast<LPOLESTR>((*it).first.c_str()), fSameApt);
			if (pDisp)
			{
				EventMapIter itev = m_mapEvent.find((*it).first.c_str());
				if (itev == m_mapEvent.end())
				{
					itev = (m_mapEvent.insert(EventMap::value_type((*it).first, new CEventSink(this)))).first;
				}
				(*itev).second->AddRef();
				(*itev).second->Advise(pDisp);
				pDisp->Release();
			}
		}
	}
	return m_pSite->OnStateChange(SCRIPTSTATE_CONNECTED);
}

void CRScriptCore::ConnectEvents()
{
	for (ScriptletListIter it = m_listScriptlets.begin(); it != m_listScriptlets.end(); it++)
	{
		it->Add(this);
	}
	m_listScriptlets.clear();
}
//
// Change ScriptEngine State to Disconnected by Host
//
void CRScriptCore::Disconnect(bool fSinkOnly)
{
#if 0	// I made a mistake. These objects contained Ruby's Thread-oriented localized pointer.
	    // So, can never release them here. (RIP !)
	for (OleObjListIter itole = m_listOleObj.begin(); itole != m_listOleObj.end(); itole++)
	{
		(*itole)->pDispatch->Release();
		(*itole)->pDispatch = NULL;
	}
	m_listOleObj.clear();
#endif

	for (EventMapIter itev = m_mapEvent.begin(); itev != m_mapEvent.end(); itev++)
	{
		if ((*itev).second)
		{
			(*itev).second->Release();
		}
	}
	m_mapEvent.clear();

	if (fSinkOnly) return;	// return If triggered by SetScriptState -> DISCONNECTED

	// Close or InitNew

	for (ItemMapIter it = m_mapItem.begin(); it != m_mapItem.end(); it++)
	{
		if ((*it).second)
		{
			delete (*it).second;
		}
	}

  m_mapItem.clear();
}

// Resolve Dispatch Pointer (Triggered by SetScriptState -> START)
void CRScriptCore::BindNamedItem()
{
	for (ItemMapIter it = m_mapItem.begin(); it != m_mapItem.end(); it++)
	{
		if ((*it).second->IsOK() == false)
			AddNamedItem((*it).first.c_str());
	}
}

void CRScriptCore::UnbindNamedItem()
{
	for (ItemMapIter it = m_mapItem.begin(); it != m_mapItem.end(); it++)
	{
		if ((*it).second)
			(*it).second->Empty();
	}
}

void CRScriptCore::CopyNamedItem(ItemMap& map)
{
	for (ItemMapIter it = map.begin(); it != map.end(); it++)
	{
		m_mapItem.insert(ItemMap::value_type((*it).first, new CItemHolder));
	}
}

void CRScriptCore::ForceChangeThreadContext()
{
#ifndef UNDER_NT4_ONLY
	MSG msg;
	while (::PeekMessage(&msg, NULL, WM_USER, 0xffffffff, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
#endif
	Sleep(0);
}


