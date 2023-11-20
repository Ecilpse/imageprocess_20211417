
// imageprocess_20211417View.cpp: Cimageprocess20211417View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "imageprocess_20211417.h"
#endif

#include "imageprocess_20211417Doc.h"
#include "imageprocess_20211417View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cimageprocess20211417View

IMPLEMENT_DYNCREATE(Cimageprocess20211417View, CView)

BEGIN_MESSAGE_MAP(Cimageprocess20211417View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cimageprocess20211417View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_32771, &Cimageprocess20211417View::On32771)
END_MESSAGE_MAP()

// Cimageprocess20211417View 생성/소멸

Cimageprocess20211417View::Cimageprocess20211417View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

Cimageprocess20211417View::~Cimageprocess20211417View()
{
}

BOOL Cimageprocess20211417View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// Cimageprocess20211417View 그리기

void Cimageprocess20211417View::OnDraw(CDC* pDC)
{
	Cimageprocess20211417Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDC->TextOut(100, 100, "첫 번째 영상처리 프로그램 예제입니다");
}


// Cimageprocess20211417View 인쇄


void Cimageprocess20211417View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cimageprocess20211417View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void Cimageprocess20211417View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void Cimageprocess20211417View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void Cimageprocess20211417View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cimageprocess20211417View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cimageprocess20211417View 진단

#ifdef _DEBUG
void Cimageprocess20211417View::AssertValid() const
{
	CView::AssertValid();
}

void Cimageprocess20211417View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cimageprocess20211417Doc* Cimageprocess20211417View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cimageprocess20211417Doc)));
	return (Cimageprocess20211417Doc*)m_pDocument;
}
#endif //_DEBUG


// Cimageprocess20211417View 메시지 처리기


void Cimageprocess20211417View::On32771()
{
	AfxMessageBox("안녕하세요 여러분!");
}
