
// ImageProView.cpp: CImageProView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImagePro.h"
#endif

#include "ImageProDoc.h"
#include "ImageProView.h"

#include <vfw.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define TWO_IMAGES   1
#define THREE_IMAGES    2
#define TWO_IMAGES_SCALED   4
#define MORPHING    8
#define AVI_FILE    16

// CImageProView

IMPLEMENT_DYNCREATE(CImageProView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_32771, &CImageProView::OnPixelHistoEq)
	ON_COMMAND(ID_32772, &CImageProView::OnPixelAdd)
	ON_COMMAND(ID_32773, &CImageProView::OnPixelTwoImageAdd)
	ON_COMMAND(ID_REGION_SHARPENING, &CImageProView::OnRegionSharpening)
	ON_COMMAND(ID_REGION_SMOOTHING, &CImageProView::OnRegionSmoothing)
	ON_COMMAND(ID_REGION_EMBOSSING, &CImageProView::OnRegionEmbossing)
	ON_COMMAND(ID_EROSION, &CImageProView::OnErosion)
	ON_COMMAND(ID_DILATION, &CImageProView::OnDilation)
	ON_COMMAND(ID_OPENING, &CImageProView::OnOpening)
	ON_COMMAND(ID_CLOSING, &CImageProView::OnClosing)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_PIXEL_COPY, &CImageProView::OnGeometryZoominPixelCopy)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_INTERPOLATION, &CImageProView::OnGeometryZoominInterpolation)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_SUBSAMPLING, &CImageProView::OnGeometryZoomoutSubsampling)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_AVG, &CImageProView::OnGeometryZoomoutAvg)
	ON_COMMAND(ID_GEOMETRY_MIRROR, &CImageProView::OnGeometryMirror)
	ON_COMMAND(ID_GEOMETRY_FLIP, &CImageProView::OnGeometryFlip)
	ON_COMMAND(ID_GEOMETRY_ROTATE, &CImageProView::OnGeometryRotate)
	ON_COMMAND(ID_GEOMETRY_WARPING, &CImageProView::OnGeometryWarping)
END_MESSAGE_MAP()

// CImageProView 생성/소멸

CImageProView::CImageProView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CImageProView::~CImageProView()
{
}

BOOL CImageProView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CImageProView 그리기

void CImageProView::OnDraw(CDC * pDC)
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (viewMode == AVI_FILE) {
		loadAVIFile(pDC);
		return;
	}
	if (pDoc -> inputImg == NULL) return;

	if (pDoc->depth == 1) {
		for (int y = 0; y < pDoc->imageHeight; y++)
			for (int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],
					          pDoc->inputImg[y][x],
					          pDoc->inputImg[y][x]));

		if (viewMode == THREE_IMAGES) {
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->inputImg2[y][x],
							pDoc->inputImg2[y][x],
							pDoc->inputImg2[y][x]));
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth*2 + 60, y,
						RGB(pDoc->resultImg[y][x],
							pDoc->resultImg[y][x],
							pDoc->resultImg[y][x]));
		}
		else if (viewMode == TWO_IMAGES_SCALED) {
			for (int y = 0; y < pDoc->gImageHeight; y++)
				for (int x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->gResultImg[y][x],
							pDoc->gResultImg[y][x],
							pDoc->gResultImg[y][x]));
		}
		//else if (viewMode == MORPHING) {
		//	for (int y = 0; y < pDoc->imageHeight; y++)
		//		for (int x = 0; x < pDoc->imageWidth; x++)
		//			pDC->SetPixel(x + pDoc->imageWidth + 30, y,
		//				RGB(pDoc->inputImg2[y][x],
		//					pDoc->inputImg2[y][x],
		//					pDoc->inputImg2[y][x]));
		//		for (int i = 0; i < 10; i++)
		//			for (int y = 0; y < pDoc->imageHeight; y++)
		//				for (int x = 0; x < pDoc->imageWidth; x++)
		//					pDC->SetPixel(x + pDoc->imageWidth * 2 + 60, y,
		//						RGB(pDoc->morpedImg[i][y][x],
		//							pDoc->morpedImg[i][y][x],
		//							pDoc->morpedImg[i][y][x]));
		//}
		else {
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->resultImg[y][x],
							pDoc->resultImg[y][x],
							pDoc->resultImg[y][x]));
		}
	}
	else if (pDoc->depth == 3) {
		for (int y = 0; y < pDoc->imageHeight; y++)
			for (int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][3 * x],
					     pDoc->inputImg[y][3 * x + 1],
					     pDoc->inputImg[y][3 * x + 2]));

		if (viewMode == THREE_IMAGES) {
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->inputImg2[y][3 * x],
							pDoc->inputImg2[y][3 * x + 1],
							pDoc->inputImg2[y][3 * x + 2]));
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth * 2 + 60, y,
						RGB(pDoc->resultImg[y][3 * x],
							pDoc->resultImg[y][3 * x + 1],
							pDoc->resultImg[y][3 * x + 2]));
		}
		else if (viewMode == TWO_IMAGES_SCALED) {
			for (int y = 0; y < pDoc->gImageHeight; y++)
				for (int x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->gResultImg[y][3 * x],
							pDoc->gResultImg[y][3 * x + 1],
							pDoc->gResultImg[y][3 * x + 2]));
		}
		else {
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth+ 30, y,
						RGB(pDoc->resultImg[y][3 * x],
							pDoc->resultImg[y][3 * x + 1],
							pDoc->resultImg[y][3 * x + 2]));
		}
	}


	//for (int y = 0; y < 256; y++)
	//	for (int x = 0; x < 256; x++)
	//		pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],
	//			pDoc->inputImg[y][x],
	//			pDoc->inputImg[y][x]));
	//
	//if (viewMode == THREE_IMAGES) {
	//	for (int y = 0; y < 256; y++)
	//		for (int x = 0; x < 256; x++)
	//			pDC->SetPixel(x + 300, y, RGB(pDoc->inputImg2[y][x],
	//				pDoc->inputImg2[y][x],
	//				pDoc->inputImg2[y][x]));
	//	for (int y = 0; y < 256; y++)
	//		for (int x = 0; x < 256; x++)
	//			pDC->SetPixel(x + 600, y, RGB(pDoc->resultImg[y][x],
	//				pDoc->resultImg[y][x],
	//				pDoc->resultImg[y][x]));
	//}
	//else {
	//	for (int y = 0; y < 256; y++)
	//		for (int x = 0; x < 256; x++)
	//			pDC->SetPixel(x + 300, y, RGB(pDoc->resultImg[y][x],
	//				pDoc->resultImg[y][x],
	//				pDoc->resultImg[y][x]));
	//}
}

void CImageProView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = 2048;
	sizeTotal.cy = 1024;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CImageProView 인쇄


void CImageProView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageProView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CImageProView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CImageProView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CImageProView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageProView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageProView 진단

#ifdef _DEBUG
void CImageProView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProDoc* CImageProView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProDoc)));
	return (CImageProDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProView 메시지 처리기


void CImageProView::OnPixelHistoEq()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->PixelHistoEq();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnPixelAdd()
{
	CImageProDoc* pDoc;

	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->PixelAdd();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnPixelTwoImageAdd()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->PixelTwoImageAdd();
	viewMode = THREE_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnRegionSharpening()
{
	CImageProDoc* pDoc = GetDocument();

	float kernel[3][3] = { {0,-1,0},{-1,5,-1},{0,-1,0} };

	convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

	Invalidate();

}

void CImageProView::OnRegionSmoothing()
{
	CImageProDoc* pDoc = GetDocument();

	float kernel[3][3] = { {1 / 9.0f,1 / 9.0f,1 / 9.0f},{1 / 9.0f,1 / 9.0f,1 / 9.0f},{1 / 9.0f,1 / 9.0f,1 / 9.0f} };

	convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

	Invalidate();

}



void CImageProView::OnRegionEmbossing()
{
	CImageProDoc* pDoc = GetDocument();

	float kernel[3][3] = { {-1,0,0},{0,0,0},{0,0,1} };

	convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

	Invalidate();

}

void CImageProView::convolve(unsigned char** inimg, unsigned char** outimg, int cols, int rows, float mask[][3], int bias, int depth)
{
	int x, y;
	int i, j;
	int sum, rsum, bsum, gsum;

	for (y=1; y< rows-1; y++)
		for (x=1; x < cols-1; x++)
		{
			if (depth == 1)
			{
				sum = 0;
				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++)
					{
						sum += (inimg[y + j - 1][x + i - 1] * mask[j][i]);
					}
				sum += bias;
				if (sum > 255) sum = 255;
				else if (sum < 0) sum = 0;

				outimg[y][x] = sum;
			}
			else
			{
				rsum = 0; gsum = 0; bsum = 0;
				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++)
					{
						rsum += (inimg[y + j - 1][3 * (x + i - 1) + 0] * mask[j][i]);
						gsum += (inimg[y + j - 1][3 * (x + i - 1) + 1] * mask[j][i]);
						bsum += (inimg[y + j - 1][3 * (x + i - 1) + 2] * mask[j][i]);
					}
				rsum += bias;
				gsum += bias;
				bsum += bias;


				if (rsum > 255) rsum = 255;
				else if (rsum < 0) rsum = 0;
				if (gsum > 255) gsum = 255;
				else if (gsum < 0) gsum = 0;
				if (bsum > 255) bsum = 255;
				else if (bsum < 0) bsum = 0;

				outimg[y][3 * x + 0] = rsum;
				outimg[y][3 * x + 1] = gsum;
				outimg[y][3 * x + 2] = bsum;
			}
		}

}
void CImageProView::OnErosion()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->Erosion();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnDilation()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->Dilation();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnOpening()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->Opening();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnClosing()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->Closing();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnGeometryZoominPixelCopy()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryZoominPixelCopy();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CImageProView::OnGeometryZoominInterpolation()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryZoominInterpolation();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CImageProView::OnGeometryZoomoutSubsampling()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryZoomoutSubsampling();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}

void CImageProView::OnGeometryZoomoutAvg()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryZoomoutAvg();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CImageProView::OnGeometryMirror()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryMirror();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnGeometryFlip()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryFlip();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnGeometryRotate()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryRotate();
	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CImageProView::OnGeometryWarping()
{
	CImageProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryWarping();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProView::OnAviView()
{
	CFile file;
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK) {
		strcpy_s(AVIFileName, dlg.GetPathName());
		viewMode = AVI_FILE;
	}
	Invalidate(FALSE);
}


void CImageProView::loadAVIFile(CDC * pDC)
{
	//LPBITMAPINFOHEADER pbmih;
	//PAVIFILE pavi;
    //PAVISTREAM pstm;
	//PGETFRAME pfrm;
	//AVIFILEINFO fi;
	//AVISTREAMINFO si;
	//int stm;
	//int frame;
	//int x, y;
	//unsigned char* image;
	//
	//AVIFileInit();
	//AVIFileOpen(&pavi, AVIFileName, OF_READ | OF_SHARE_DENY_NONE, NULL);
	//AVIFileInfo(pavi, &fi, sizeof(AVIFILEINFO));
	//
	//for (stm = 0; stm < fi.dwStreams; stm++) {
	//	AVIFileGetStream(pavi, &pstm, 0, stm);
	//	AVIStreamInfo(pstm, &si, sizeof(si));
	//
	//	if (si.fccType == streamtypeVIDEO) {
	//		pfrm = AVIStreamGetFrameOpen(pstm, NULL);
	//
	//		for (frame = 0; frame < si.dwLength; frame++) {
	//			pbmih = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pfrm, frame);
	//			if (!pbmih) continue;
	//			image = (unsigned char*)((LPSTR)pbmih + (WORD)pbmih->biSize);
	//			for (y = 0; y < fi.dwHeight; y++)
	//				for (x = 0; x < fi.dwWidth; x++)
	//					pDC->SetPixel(x, fi.dwHeight - y - 1,
	//						RGB(image[(y * fi.dwWidth + x) * 3 + 2],
	//							image[(y * fi.dwWidth + x) * 3 + 1],
	//							image[(y * fi.dwWidth + x) * 3]));
	//		}
	//	}
	//}
	//AVIStreamGetFrameClose(pfrm);
	//AVIStreamRelease(pstm);
	//AVIFileRelease(pavi);
	//AVIFileExit();
	//
}
