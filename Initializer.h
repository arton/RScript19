/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 *  $Date: 2006-11-03 04:23:11 +0900 (Fri, 03 11 2006) $
 */

#ifndef INITALIZER_HEADER
#define INITALIZER_HEADER

extern "C" void NtInitialize(int *, char ***);

class CInitializer
{
public:
	CInitializer();
	~CInitializer();
	void InitNew();
	static inline CInitializer& GetInstance() { return GlobalInitializer; }
	inline void Lock() { m_crit.Lock(); }
	inline void Unlock() { m_crit.Unlock(); }
	inline bool IsInit() const { return m_fInit; }
	inline void SetInit(bool f) { m_fInit = f; }
	static inline bool InRuby() { return g_fInRuby; }
	static inline void SetInRuby(bool f) { g_fInRuby = f; }
	void Destroy();
private:
	static CInitializer GlobalInitializer;
	static bool g_fInRuby;
	CComAutoCriticalSection m_crit;
	DWORD m_dwRubyThread;
	bool m_fInit;
};

#endif

