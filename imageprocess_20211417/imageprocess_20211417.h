
// imageprocess_20211417.h: imageprocess_20211417 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


// Cimageprocess20211417App:
// 이 클래스의 구현에 대해서는 imageprocess_20211417.cpp을(를) 참조하세요.
//

class Cimageprocess20211417App : public CWinAppEx
{
public:
	Cimageprocess20211417App() noexcept;


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cimageprocess20211417App theApp;
