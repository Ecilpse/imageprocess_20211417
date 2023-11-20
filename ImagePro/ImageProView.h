
// ImageProView.h: CImageProView 클래스의 인터페이스
//

#pragma once


class CImageProView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CImageProView() noexcept;
	DECLARE_DYNCREATE(CImageProView)

// 특성입니다.
public:
	CImageProDoc* GetDocument() const;
	int viewMode;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CImageProView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPixelHistoEq();
	afx_msg void OnPixelAdd();
	afx_msg void OnPixelTwoImageAdd();
	afx_msg void OnRegionSharpening();
	void convolve(unsigned char** inimg, unsigned char** outimg, int cols, int rows, float mask[][3], int bias, int depth);
	afx_msg void OnRegionSmoothing();
	afx_msg void OnRegionEmbossing();
};

#ifndef _DEBUG  // ImageProView.cpp의 디버그 버전
inline CImageProDoc* CImageProView::GetDocument() const
   { return reinterpret_cast<CImageProDoc*>(m_pDocument); }
#endif

