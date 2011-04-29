/*
 *  Copyright(c) 2000 arton
 *
 *  You may distribute under the terms of either the GNU General Public
 *  License
 *
 */

#include "stdafx.h"
#include "GRScript.h"
#include "Initializer.h"
#include "Win32Ole.h"
#include "RScriptCore.h"

CInitializer CInitializer::GlobalInitializer;

CInitializer::CInitializer()
	: m_fInit(false)
{
}

void CInitializer::InitNew()
{
	if (m_fInit) return;
	m_crit.Lock();
	if (m_fInit)
	{
		m_crit.Unlock();
		return;
	}
	m_fInit = true;
	m_crit.Unlock();

	CRScriptCore::InitializeEnvironment();
}

CInitializer::~CInitializer()
{
	ATLTRACE(_("RScript Initializer was Destroyed\n"));
}

