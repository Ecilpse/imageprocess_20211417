
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
	gResultImg = NULL;

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


void CImageProDoc::Erosion()
{
	int x, y, min;
	for (y = 1; y < imageHeight - 1; y++)
		for (x = 1; x < imageWidth - 1; x++) {
			min = 255;
			if (inputImg[y - 1][x - 1] < min) min = inputImg[y - 1][x - 1];
			if (inputImg[y - 1][x] < min) min = inputImg[y - 1][x];
			if (inputImg[y - 1][x + 1] < min) min = inputImg[y - 1][x + 1];
			if (inputImg[y][x - 1] < min) min = inputImg[y][x - 1];
			if (inputImg[y][x] < min) min = inputImg[y][x];
			if (inputImg[y][x + 1] < min) min = inputImg[y][x + 1];
			if (inputImg[y + 1][x - 1] < min) min = inputImg[y + 1][x - 1];
			if (inputImg[y + 1][x] < min) min = inputImg[y + 1][x];
			if (inputImg[y + 1][x + 1] < min) min = inputImg[y + 1][x + 1];

			resultImg[y][x] = min;
		}
}

void CImageProDoc::Dilation()
{
	int x, y, max;

	for (y=1;y < imageHeight -1; y++)
		for (x = 1; x < imageWidth - 1; x++) {
			max = 0;
			if (inputImg[y - 1][x - 1] < max) max = inputImg[y - 1][x - 1];
			if (inputImg[y - 1][x] < max) max = inputImg[y - 1][x];
			if (inputImg[y - 1][x + 1] < max) max = inputImg[y - 1][x + 1];
			if (inputImg[y][x - 1] < max) max = inputImg[y][x - 1];
			if (inputImg[y][x] < max) max = inputImg[y][x];
			if (inputImg[y][x + 1] < max) max = inputImg[y][x + 1];
			if (inputImg[y + 1][x - 1] < max) max = inputImg[y + 1][x - 1];
			if (inputImg[y + 1][x] < max) max = inputImg[y + 1][x];
			if (inputImg[y + 1][x + 1] < max) max = inputImg[y + 1][x + 1];

			resultImg[y][x] = max;

		}
}


void CImageProDoc::Opening()
{
	Erosion();

	CopyResultToInput();
	Erosion();

	CopyResultToInput();
	Erosion();

	CopyResultToInput();
	Dilation();

	CopyResultToInput();
	Dilation();

	CopyResultToInput();
	Dilation();
}


void CImageProDoc::CopyResultToInput()
{
	int x, y;

	for (y = 0; y < imageHeight; y++)
		for (x = 0; x < imageWidth; x++)
			inputImg[y][x] = resultImg[y][x];
}


void CImageProDoc::Closing()
{
	Dilation();

	CopyResultToInput();
	Dilation();

	CopyResultToInput();
	Dilation();

	CopyResultToInput();
	Erosion();

	CopyResultToInput();
	Erosion();

	CopyResultToInput();
	Erosion();
}


void CImageProDoc::GeometryZoominPixelCopy()
{
	int i, y, x;

	gImageWidth = imageWidth * 3;
	gImageHeight = imageHeight * 3;

	gResultImg = (unsigned char**)malloc(gImageHeight * sizeof(unsigned char*));

	for (i = 0; i < gImageHeight; i++) {
		gResultImg[i] = (unsigned char*)malloc(gImageWidth * depth);
	}

	for (y = 0; y < gImageHeight; y++)
		for (x = 0; x < gImageWidth; x++)
			gResultImg[y][x] = inputImg[y / 3][x / 3];
}


void CImageProDoc::GeometryZoominInterpolation()
{
	int i, y, x;
	float src_x, src_y;
	float alpha, beta;
	int scale_x, scale_y;
	int E, F;
	int Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;

	scale_x = 3;
	scale_y = 3;

	gImageWidth = imageWidth * scale_x;
	gImageHeight = imageHeight * scale_y;
	
	gResultImg = (unsigned char**)malloc(gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < gImageHeight; i++) {
		gResultImg[i] = (unsigned char*)malloc(gImageWidth * depth);

	}

	for (y=0; y< gImageHeight; y++)
		for (x = 0; x < gImageWidth; x++) {
			src_x = x / (float)scale_x;
			src_y = y / (float)scale_y;
			alpha = src_x - x / scale_x;
			beta = src_y - y / scale_y;

			Ax = x / scale_x;
			Ay = y / scale_y;
			Bx = Ax + 1;
			By = Ay;
			Cx = Ax;
			Cy = Ay + 1;
			Dx = Ax + 1;
			Dy = Ay + 1;

			if (Bx > imageWidth - 1) Bx = imageWidth - 1;
			if (Dx > imageWidth - 1) Bx = imageWidth - 1;
			if (Cy > imageHeight - 1) Cy = imageHeight - 1;
			if (Dy > imageHeight - 1) Dy = imageHeight - 1;

			E = (int)(inputImg[Ay][Ax] * (1 - alpha) + inputImg[By][Bx] * alpha);
			F = (int)(inputImg[Cy][Cx] * (1 - alpha) + inputImg[Dy][Dx] * alpha);

			gResultImg[y][x] = (unsigned char)(E * (1 - beta) + F * beta);
		}
} 


void CImageProDoc::GeometryZoomoutSubsampling()
{
	int i, y, x;
	int src_x, src_y;
	int scale_x = 3, scale_y = 3;

	gImageWidth = imageWidth / scale_x;
	gImageHeight = imageHeight / scale_y;

	gResultImg = (unsigned char**)malloc(gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < gImageHeight; i++) {
		gResultImg[i] = (unsigned char*)malloc(gImageWidth * depth);
	}

	for (y = 0; y < gImageHeight; y++)
		for (x = 0; x < gImageWidth; x++) {
			src_y = y * scale_y;
			src_x = x * scale_x;
			if (src_x > imageWidth - 1) src_x = imageWidth - 1;
			if (src_y > imageHeight - 1) src_y = imageHeight - 1;
			gResultImg[y][x] = inputImg[src_y][src_x];
		}
}


void CImageProDoc::GeometryZoomoutAvg()
{
	int i, j, y, x;
	int sum;
	int src_x, src_y;
	int scale_x = 3, scale_y = 3;

	gImageWidth = imageWidth / scale_x + 1;
	gImageHeight = imageHeight / scale_y + 1;

	gResultImg = (unsigned char**)malloc(gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < gImageHeight; i++) {
		gResultImg[i] = (unsigned char*)malloc(gImageWidth * depth);
	}
	for (y=0; y < imageHeight; y= y+scale_y)
		for (x = 0; x < imageWidth; x = x + scale_x) {
			sum = 0;
			for (i=0; i<scale_y; i++)
				for (j = 0; j < scale_x; j++) {
					src_x = x + j;
					src_y = y + i;

					if (src_x > imageWidth - 1) src_x = imageWidth - 1;
					if (src_y > imageHeight - 1) src_y = imageHeight - 1;
					sum += inputImg[src_y][src_x];
				}

			sum = sum / (scale_x * scale_y);
			if (sum > 255) sum = 255;
			if (sum < 0) sum = 0;

			gResultImg[y / scale_y][x / scale_x] = (unsigned char)sum;
		}
}


void CImageProDoc::GeometryMirror()
{
	int y, x;

	for(y=0; y< imageHeight; y++)
		for (x = 0; x < imageWidth; x++) {
			resultImg[y][x] = inputImg[y][imageWidth - 1 - x];
		}
}


void CImageProDoc::GeometryFlip()
{
	int y, x;

	for (y = 0; y < imageHeight; y++)
		for (x = 0; x < imageWidth; x++) {
			resultImg[imageHeight -1 - y][x] = inputImg[y][x];
		}

}

#define PI 3.14159
void CImageProDoc::GeometryRotate()
{
	int y, x, x_source, y_source, Cx, Cy;
	float angle;
	int Oy;
	int i, xdiff, ydiff;

	Oy = imageHeight - 1;

	angle = PI / 180.0 * 30.0;
	Cx = imageWidth / 2;
	Cy = imageHeight / 2;

	gImageWidth = (int)(imageHeight * cos(PI / 2.0 - angle) +
		imageWidth * cos(angle));
	gImageHeight = (int)(imageHeight * cos(angle) +
		imageWidth * cos(PI / 2.0 - angle));
	gResultImg = (unsigned char**)malloc(gImageHeight * sizeof(unsigned char *));

	for (i = 0; i < gImageHeight; i++) {
		gResultImg[i] = (unsigned char*)malloc(gImageWidth * depth);
	}

	xdiff = (gImageWidth - imageWidth) / 2;
	ydiff = (gImageHeight - imageHeight) / 2;

	for (y = -ydiff; y < gImageHeight - ydiff; y++)
		for (x = -xdiff; x < gImageWidth - xdiff; x++) {
			x_source = (int)(((Oy - y) - Cy) * sin(angle) + (x - Cx) * cos(angle) + Cx);
			y_source = (int)(((Oy - y) - Cy) * cos(angle) - (x - Cx) * sin(angle) + Cy);

			y_source = Oy - y_source;

			if (x_source < 0 || x_source > imageWidth - 1 ||
				y_source < 0 || y_source > imageHeight - 1)
				gResultImg[y + ydiff][x + xdiff] = 255;
			else
				gResultImg[y + ydiff][x + xdiff] = inputImg[y_source][x_source];
;		}
}

typedef struct
{
	int Px;
	int Py;
	int Qx;
	int Qy;
} control_line;

void CImageProDoc::GeometryWarping()
{
	control_line source_lines[23] =
	{ {167.2075}, {34, 189, 90, 21}, {55, 249, 30.128}, {118, 320.65,261},
	 {123, 321, 171, 321}, {179, 319, 240, 264}, {247, 251, 282, 135}, {281, 114, 228, 8},
	 {78, 106, 123, 109}, {187, 115, 235, 114}, {72, 142, 99, 128}, {74, 150, 122, 154},
	 {108, 127, 123, 146}, {182, 152, 213, 132}, {183, 159, 229, 157 }, { 219,131,248,154 },
	 {80, 246, 117, 212 }, {127, 222, 146, 223 }, {154, 227, 174, 221 }, { 228, 252, 183, 213 },
	 {114, 255, 186, 257}, {109, 258, 143, 277}, {152,278,190,262 } };
	control_line dest_lines[23] =
	{ {120, 8, 200, 6}, {12, 93, 96, 16},{74, 271, 16, 110}, {126,336,96,290},
	{142,337,181,335 }, { 192,335,232,280 }, { 244, 259, 288, 188 }, { 285, 92, 212, 13 },
	{96,135,136,118},{194,119,223,125 }, { 105,145,124,134 }, { 118,146,138,151},
	{131,133,139,146},{188,146,198,134 }, { 189,153,218,146},{204,133,221,140},
	{91,268,122,202},{149,206,159,209 }, { 170,209,181,204 }, { 235,265,288,199},
	{121,280,205,284},{112,286,160,301}, {166,301,214,287} };

	double u;
	double h;
	double d;
	double tx, ty;
	double xp, yp;

	double weight;
	double totalWeight;
	double a = 0.001;
	double b = 2.0;
	double p = 0.75;

	int x1, x2, y1, y2;
	int src_x1, src_y1, src_x2, src_y2;
	double src_line_length, dest_line_length;

	int num_lines = 23;
	int line;
	int x, y;

	int source_x, source_y;
	int last_row, last_col;
	last_row = imageHeight - 1;
	last_col = imageWidth - 1;

	for (y = 0; y < imageHeight; y++)
	{
		for (x = 0; x < imageWidth; x++)
		{
			totalWeight = 0.0;
			tx = 0.0;
			ty = 0.0;

			for (line = 0; line < num_lines; line++)
			{
				x1 = dest_lines[line].Px;
				y1 = dest_lines[line].Py;
				x2 = dest_lines[line].Qx;
				y2 = dest_lines[line].Qy;

				dest_line_length = sqrt((double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));

				u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
					(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				h = (double)((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

				if (u < 0) d = sqrt((double)((x - x1) * (x - x1) + (y - y1) * (y - y1)));
				else if (u > 1) d = sqrt((double)((x - x2) * (x - x2) + (y - y2) * (y - y2)));
				else d = fabs(h);

				src_x1 = source_lines[line].Px;
				src_y1 = source_lines[line].Py;
				src_x2 = source_lines[line].Qx;
				src_y2 = source_lines[line].Qy;
				src_line_length = sqrt((double)((src_x2 - src_x1) * (src_x2 - src_x1) +
					(src_y2 - src_y1) * (src_y2 - src_y1)));

				xp = src_x1 + u * (src_x2 - src_x1) -
					h * (src_y2 - src_y1) / src_line_length;
				yp = src_y1 + u * (src_y2 - src_y1) +
					h * (src_x2 - src_x1) / src_line_length;

				weight = pow((pow((double)(dest_line_length), p) / (a + d)), b);
				tx += (xp - x) * weight;
				ty += (yp - y) * weight;
				totalWeight += weight;
			}
			source_x = x + (int)(tx / totalWeight + 0.5);
			source_y = y + (int)(ty / totalWeight + 0.5);

			if (source_x < 0) source_x = 0;
			if (source_x > last_col) source_x = last_col;
			if (source_y < 8) source_y = 0;
			if (source_y > last_row) source_y = last_row;
			resultImg[y][x] = inputImg[source_y][source_x];
		}
	}
}