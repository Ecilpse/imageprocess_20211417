
// ImageProDoc.cpp: CImageProDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImagePro.h"
#endif

#include "ImageProDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageProDoc

IMPLEMENT_DYNCREATE(CImageProDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProDoc, CDocument)
END_MESSAGE_MAP()


// CImageProDoc 생성/소멸

CImageProDoc::CImageProDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	inputImg = NULL;
	inputImg2 = NULL;
	resultImg = NULL;

}

CImageProDoc::~CImageProDoc()
{
	int i;

	if (inputImg != NULL)
	{
		for (i = 0; i < imageHeight; i++)
			free(inputImg[i]);
		free(inputImg);
	}

	if (inputImg2 != NULL)
	{
		for (i = 0; i < imageHeight; i++)
			free(inputImg2[i]);
		free(inputImg2);
	}

	if (resultImg != NULL)
	{
		for (i = 0; i < imageHeight; i++)
			free(resultImg[i]);
		free(resultImg);
	}
}

BOOL CImageProDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CImageProDoc serialization

void CImageProDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
		LoadImageFile(ar);
	}
	//if (ar.IsStoring())
	//{
	//	ar.Write(resultImg, 256 * 256);
	//}
	//else
	//{
	//	CFile* fp = ar.GetFile();
	//	if (fp->GetLength() == 256 * 256) ar.Read(inputImg, 256 * 256);
	//	else AfxMessageBox("256x256 크기의 파일만 사용가능합니다.");
	//}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageProDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CImageProDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageProDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageProDoc 진단

#ifdef _DEBUG
void CImageProDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageProDoc 명령



void CImageProDoc::PixelHistoEq()
{
	int x, y, i, k;
	int acc_hist = 0;
	float N = 256 * 256;
	int hist[256], sum[256];

	for (k = 0; k < 256; k++) hist[k] = 0;

	//명암값의 빈도수 조사
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
			k = inputImg[y][x];
			hist[k] = hist[k] + 1;
		}
	for (i = 0; i < 256; i++) {
		acc_hist = acc_hist + hist[i];
		sum[i] = acc_hist;
	}

	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
			k = inputImg[y][x];
			resultImg[y][x] = (unsigned char)(sum[k] / N * 255);
		}
}


void CImageProDoc::PixelAdd()
{
	int value;

	for(int y=0; y<256; y++)
		for (int x = 0; x < 256; x++) {
			value = inputImg[y][x] + 100;
			if (value > 255) resultImg[y][x] = 255;
			else resultImg[y][x] = value;
		}
}


void CImageProDoc::PixelTwoImageAdd()
{
	int value;

	LoadTwoImages();

	//for (int y = 0; y < 256; y++)
	//	for (int x = 0; x < 256; x++) {
	//		value = inputImg[y][x] + inputImg2[y][x];
	//		if (value > 255) resultImg[y][x] = 255;
	//		else resultImg[y][x] = value;
	//	}
	for (int y = 0; y < 256; y++)
		for (int x = 0; x < 256; x++) {
			value = inputImg[y][x] - inputImg2[y][x];
			if (value > 255) resultImg[y][x] = 255;
			else if (value < 0) value = 0;
			if (value > 64) value = 255;
			else            value = 0;
			resultImg[y][x] = value;
		}

}


void CImageProDoc::LoadTwoImages()
{
	CFile file;
	CFileDialog dlg(TRUE);

	AfxMessageBox("Select the First Image");

	if (dlg.DoModal() == IDOK) {
		file.Open(dlg.GetPathName(), CFile::modeRead);
		file.Read(inputImg, 256 * 256);
		file.Close();
	}

	AfxMessageBox("Select the Second Image");

	if (dlg.DoModal() == IDOK) {
		file.Open(dlg.GetPathName(), CFile::modeRead);
		file.Read(inputImg2, 256 * 256);
		file.Close();
	}
}


void CImageProDoc::LoadImageFile(CArchive& ar)
{
	int i, maxValue;
	char type[16], buf[256];
	CFile* fp = ar.GetFile();
	CString fname = fp->GetFilePath();

	if (strcmp(strrchr(fname, '.'), ".ppm") == 0 ||
		strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strrchr(fname, '.'), ".PGM") == 0 ||
		strcmp(strrchr(fname, '.'), ".pgm") == 0)
	{
		ar.ReadString(type, 15);

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d %d", &imageWidth, &imageHeight);

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0) depth = 1;
		else depth = 3;
		}
	else if (strcmp(strchr(fname, '.'), ".raw") == 0 ||
		strcmp(strchr(fname, '.'), ".RAW") == 0)
	{
		if (fp->GetLength() != 256 * 256) {
			AfxMessageBox("256x256 크기의 파일만 사용가능합니다.");
			return;
		}

		imageWidth = 256;
		imageHeight = 256;
		depth = 1;
	}

	//기억 장소 할당
	inputImg = (unsigned char**) malloc(imageHeight * sizeof(unsigned char*));
	resultImg = (unsigned char**) malloc(imageHeight * sizeof(unsigned char*));

	for (i = 0; i < imageHeight; i++) {
		inputImg[i] = (unsigned char*) malloc(imageWidth * depth);
		resultImg[i] = (unsigned char*) malloc(imageWidth * depth);
	}

	//영상 데이터 읽기
	for (i = 0; i < imageHeight; i++)
		ar.Read(inputImg[i], imageWidth * depth);
}
