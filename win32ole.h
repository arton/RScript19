/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 * This header is only for wrapping win32ole.c
 *
 *  $Date: 2006-12-02 15:21:54 +0900 (Sat, 02 12 2006) $
 */
#ifndef RSCRIPT_WIN32_OLEH
#define RSCRIPT_WIN32_OLEH

#ifndef __IRubyEngine_INTERFACE_DEFINED__
class CRScriptCore;
#define IRubyEngine CRScriptCore
#endif

struct oledata {
	union {
		IDispatch *pDispatch;
		IDispatchEx *pDispatchEx;
	};
};

struct oledataex : public oledata {
	IRubyEngine* pEngine;
	IServiceProvider* pServiceProvider;	// != NULL, if IDispatchEx was supplied.
};

struct oleparam {
    DISPPARAMS dp;
    OLECHAR** pNamedArgs;
    struct oledataex *pole;
	IServiceProvider* pProv;
};

struct SInvokeParam {
	IRubyEngine* pengine;
	VALUE module;
	ID id;
	int cArgs;
	VALUE val[1];	// maybe expand
};

struct STypeConvParam {
	IRubyEngine* pengine;
	VARIANT* pvar;
	VALUE val;
};

extern st_table *rb_class_tbl;

#endif
