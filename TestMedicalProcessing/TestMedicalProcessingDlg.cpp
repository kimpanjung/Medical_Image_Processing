
// TestMedicalProcessingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestMedicalProcessing.h"
#include "TestMedicalProcessingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CTestMedicalProcessingDlg dialog




CTestMedicalProcessingDlg::CTestMedicalProcessingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestMedicalProcessingDlg::IDD, pParent)
	, m_PixelMax(0)
	, m_PixelMin(0)
	, m_WidthVal(0)
	, m_LevelVal(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestMedicalProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestMedicalProcessingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CTestMedicalProcessingDlg message handlers

BOOL CTestMedicalProcessingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	//m_FileName.Empty();
	m_FileName = CString("Lena_2byte.raw");
	//m_FileName = CString("Lena.raw");

	m_ImageSizeX = 512;
	m_ImageSizeY = 512;

	m_RawImage = NULL;
	m_ViewImage = NULL;

	m_BitmapImage = NULL;
	m_Palette = NULL;

	// Double Buffering
		//Image
	m_pMemBitmap = NULL;
	m_pMemGraphics = NULL;
	m_pCachedBitmap = NULL;


	OpenFile(m_FileName, m_ImageSizeX, m_ImageSizeY);

	MoveWindow(200, 100, 512, 512);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestMedicalProcessingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestMedicalProcessingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		CPaintDC dc(this);
		Graphics graphics(dc);

		if (m_pMemBitmap == NULL)
			UpdateScreen(&graphics);

		// Image draw
		graphics.DrawCachedBitmap(m_pCachedBitmap, 0, 0);
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestMedicalProcessingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestMedicalProcessingDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here


	CloseGraphicsMem();
	CloseImageMemory();

	if (m_BitmapImage != NULL) {
		delete m_BitmapImage;
		m_BitmapImage = NULL;
	}
	if (m_Palette != NULL) {
		delete[] m_Palette;
		m_Palette = NULL;
	}

}

void CTestMedicalProcessingDlg::UpdateScreen(Graphics* pGraphics)
{

	int YGabTop = 5;
	int XGabLeft = 5;

	// creat image map
	m_pMemBitmap = ::new Bitmap(m_ImageSizeX, m_ImageSizeY);
	m_pMemGraphics = Graphics::FromImage(m_pMemBitmap);
	
	CSize SizeImage;
	SizeImage.cx = (int)(m_ImageSizeX);
	SizeImage.cy = (int)(m_ImageSizeY);

	// 순서대로 그려짐 배경 -> 이미지 -> 텍스트
	// create background color graphics

	// 1) background color
	SolidBrush Background_Black(Color(255, 255, 255));
	m_pMemGraphics->FillRectangle(&Background_Black, 0, 0, m_ImageSizeX, m_ImageSizeY);		
	
	// 2) image
	m_pMemGraphics->DrawImage(m_BitmapImage, Rect((int)0, (int)0, (int)(SizeImage.cx), (int)(SizeImage.cy)), 0, 0, (int)(SizeImage.cx), (int)(SizeImage.cy), UnitPixel);
	
	
	// Text out Font 
	Gdiplus::Font Text_Output_Font(L"Arial", 15, FontStyleRegular, UnitPixel);
	SolidBrush Text_Output_Brush(Color(255, 255, 0));	
	CString Text_Out_String;
	PointF Text_Out_Point;
	BSTR Text_Out_String_BSTR;

	// text output
	Text_Out_Point = PointF((float)(XGabLeft), (float)(YGabTop));
	Text_Out_String.Format(_T("File name : %s\nLevel : %d , Width : %d"), m_FileName, m_LevelVal, m_WidthVal);
	Text_Out_String_BSTR = Text_Out_String.AllocSysString();
	m_pMemGraphics->DrawString(Text_Out_String_BSTR, -1, &Text_Output_Font, Text_Out_Point, &Text_Output_Brush);


	//Gdiplus::Font Text_Output_Font2(L"Arial", 15, FontStyleRegular, UnitPixel);
	//SolidBrush Text_Output_Brush2(Color(255, 255, 100));
	//CString Text_Out_String2;
	//PointF Text_Out_Point2;
	//BSTR Text_Out_String_BSTR2;

	//// text output
	//Text_Out_Point2 = PointF((float)(XGabLeft), (float)(YGabTop)+15);
	//Text_Out_String2.Format(_T("%.2f, %.2f"), m_LevelVal, m_WidthVal);
	//Text_Out_String_BSTR2 = Text_Out_String2.AllocSysString();
	//m_pMemGraphics->DrawString(Text_Out_String_BSTR2, -1, &Text_Output_Font2, Text_Out_Point2, &Text_Output_Brush2);


	// now create the cached bitmap
	m_pCachedBitmap = ::new CachedBitmap(m_pMemBitmap, pGraphics);

}

void CTestMedicalProcessingDlg::CloseGraphicsMem(void)
{
	if (m_pMemBitmap)
	{
		::delete m_pMemBitmap;
		m_pMemBitmap = NULL;
	}

	if (m_pMemGraphics)
	{
		delete m_pMemGraphics;
		m_pMemGraphics = NULL;
	}

	if (m_pCachedBitmap)
	{
		::delete m_pCachedBitmap;
		m_pCachedBitmap = NULL;
	}
}

void CTestMedicalProcessingDlg::RedrawWindowArea(void)
{

	CloseGraphicsMem();

	//Invalidate(true);
	CRect Refresh_Area_of_Image;
	Refresh_Area_of_Image.top = 0;
	Refresh_Area_of_Image.left = 0;
	Refresh_Area_of_Image.right = m_WindowSize.cx;
	Refresh_Area_of_Image.bottom = m_WindowSize.cy;

	InvalidateRect(&Refresh_Area_of_Image);

}

void CTestMedicalProcessingDlg::SetImageMemory(void)
{

	if (m_RawImage == NULL) {
		m_RawImage = new IMAGETYPE[m_ImageSizeX*m_ImageSizeY];
		memset(m_RawImage, 0, sizeof(IMAGETYPE)*m_ImageSizeY*m_ImageSizeX);
	}
	if (m_ViewImage == NULL) {
		m_ViewImage = new unsigned char[m_ImageSizeX*m_ImageSizeY];
		memset(m_ViewImage, 0, sizeof(unsigned char)*m_ImageSizeY*m_ImageSizeX);
	}
}

void CTestMedicalProcessingDlg::CloseImageMemory(void)
{
	if (m_RawImage != NULL) {
		delete[]m_RawImage;
		m_RawImage = NULL;
	}

	if (m_ViewImage != NULL) {
		delete[]m_ViewImage;
		m_ViewImage = NULL;
	}
}


// File Open 해서 Memory 에 저장..
bool CTestMedicalProcessingDlg::File2Memory(CString OpenFileName, IMAGETYPE *Memory, int imagesize_x, int imagesize_y)
{
	CFile OpenFile;

	if (!OpenFile.Open(OpenFileName, CFile::modeRead)) {
		//		AfxMessageBox("Error : Data "+OpenFileName+" File is missing! ");
		return false;
	}

	//	if(OpenFile.GetLength() != unsigned long (sizeof(IMAGETYPE)*imagesize_x*imagesize_y)) 	{
	//		int filesize_temp = (int) (OpenFile.GetLength() - unsigned long (sizeof(IMAGETYPE)*imagesize_x*imagesize_y));
	//		if(filesize_temp < 0) {
	//			AfxMessageBox(_T("영상 사이즈가 작습니다.!"));
	//			OpenFile.Close();
	//			return false;
	//		}
	//		else 
	//		OpenFile.Seek(filesize_temp, CFile::begin);
	//	}
	OpenFile.Read(Memory, sizeof(IMAGETYPE)*imagesize_x*imagesize_y);
	OpenFile.Close();

	return true;
}

void CTestMedicalProcessingDlg::OpenFile(CString FileName, int SizeX, int SizeY)
{

	m_ImageSizeX = SizeX;
	m_ImageSizeY = SizeY;

	CloseImageMemory();
	SetImageMemory();


	ZeroMemory(&m_BitmapInfo, sizeof(m_BitmapInfo));
	BmpHeaderPalette();

	// file name setting 
	m_FileName = FileName;

	if (FileName.IsEmpty() || !File2Memory(FileName, m_RawImage, SizeX, SizeY)) {
		m_FileName = "Memory";
	}
	else {
		m_FileName = FileName;
	}

	// Raw -> View image 로 변환
	//memcpy(m_ViewImage, m_RawImage, sizeof(IMAGETYPE)*m_ImageSizeX*m_ImageSizeY);
	//======== Leveling ========//
	// 2byte -> leveling -> 1byte //	
	PixelMaxMindata();
	WidthLevelData();
	//=========================//

	MakeBitmap();
	RedrawWindowArea();
}




void CTestMedicalProcessingDlg::BmpHeaderPalette(void)
{
	m_BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_BitmapInfo.bmiHeader.biPlanes = 1;
	m_BitmapInfo.bmiHeader.biWidth = m_ImageSizeX;
	m_BitmapInfo.bmiHeader.biHeight = -m_ImageSizeY;
	m_BitmapInfo.bmiHeader.biBitCount = 8;  // 8 bit 
	m_BitmapInfo.bmiHeader.biCompression = BI_RGB;
	m_BitmapInfo.bmiHeader.biClrUsed = 256;     // 여기엔 사용한 팔레트의 수를 넣습니다. (8비트 => 256)
	m_BitmapInfo.bmiHeader.biSizeImage = m_ImageSizeX*m_ImageSizeY;

	MakePalette();
}


void CTestMedicalProcessingDlg::MakePalette(void)
{
	// 팔레트 만드는 곳..
	if (m_Palette != NULL)
		delete[]m_Palette;

	m_Palette = new ColorPalette[256];
	m_Palette->Flags = PaletteFlagsGrayScale;
	m_Palette->Count = 256;

	int i;
	for (i = 0; i < 256; i++) {
		m_Palette->Entries[i] = Color::MakeARGB(255, (BYTE)i, (BYTE)i, (BYTE)i);
	}
}


void CTestMedicalProcessingDlg::MakeBitmap(void)
{
	if (m_BitmapImage != NULL)
		delete m_BitmapImage;

	m_BitmapImage = Bitmap::FromBITMAPINFO(&m_BitmapInfo, m_ViewImage);
	m_BitmapImage->SetPalette(m_Palette);

}

void CTestMedicalProcessingDlg::PixelMaxMindata()
{
	unsigned short tmpMax, tmpMin;

	for (int x = 0; x < m_ImageSizeX; x++) {
		for (int y = 0; y < m_ImageSizeY; y++) {

			// 최대 최소값을 루프내에 1번으로 고정 //
			if (x == 0 && y == 0) {
				tmpMax = m_RawImage[0];
				tmpMin = m_RawImage[0];

				//tmpMax = 0;
				//tmpMin = 0;
			}

			if (tmpMax < m_RawImage[m_ImageSizeX*y + x]) {
				tmpMax = m_RawImage[m_ImageSizeX*y + x];
			}

			if (tmpMin > m_RawImage[m_ImageSizeX*y + x]) {
				tmpMin = m_RawImage[m_ImageSizeX*y + x];
			}
		}
	}
	m_PixelMax = tmpMax;
	m_PixelMin = tmpMin;
}

void CTestMedicalProcessingDlg::Leveling()
{
}



void CTestMedicalProcessingDlg::WidthLevelData()
{
	//==== width & level ====//
	//1. width 색조폭 = 최대값 - 최소값
	//2. level 색조치 = 최소값 + (색조폭*0.5)

	// MP = 2^8-1 = 255
	// LUT[] = int(MP+1)
	// a = level - window/2 //Minimum
	// b = level + window/2 //maximum

	m_WidthVal = m_PixelMax - m_PixelMin;
	m_LevelVal = m_PixelMin + (m_WidthVal / 2);

	int Minimum = m_LevelVal - (m_WidthVal / 2);
	int Maximum = m_LevelVal + (m_WidthVal / 2);


	IMAGETYPE tmpRawImage;

	// 1st portion = all zero (0 ~ minimum-1)
	// 2nd portion = linear scaling func (minimum ~ maximum)
	// 3rd portion = max pixel intensity (maximum+1 ~ MP(255))

	for (int x = 0; x < m_ImageSizeX; x++) {
		for (int y = 0; y < m_ImageSizeY; y++) {

			tmpRawImage = m_RawImage[(y*m_ImageSizeX) + x];

			if (tmpRawImage < Minimum) {
				m_ViewImage[y*m_ImageSizeX + x] = 0;
			}

			else if (tmpRawImage >= Minimum && tmpRawImage <= Maximum) {
				// 8bit casting//
				// int -> (x)
				// 칸을 나누기 때문에 float type!! 
				m_ViewImage[y*m_ImageSizeX + x] =
					(unsigned char)((((float)(tmpRawImage - Minimum) / (float)m_WidthVal)) * 255);
			}

			else {
				m_ViewImage[y*m_ImageSizeX + x] = 255;
			}
		}
	}

	//////////////////////////////////////////////////
	//for (int y = 0; y < m_ImageSizeY; y++) {		
	//	for (int x = 0; x < m_ImageSizeX; x++) {		

	//		tmpRawImage = (*(m_RawImage + (m_ImageSizeX*y + x)));

	//		if (tmpRawImage < Minimum) {

	//			m_ViewImage[(y*m_ImageSizeX) + x] = 0;

	//		}
	//		else if (tmpRawImage >= Minimum && tmpRawImage <= Maximum) {

	//			m_ViewImage[(y*m_ImageSizeX) + x] =(unsigned char) ((float)((float)tmpRawImage - Minimum) / (float)m_WidthVal) * 255;

	//		}
	//		else {

	//			m_ViewImage[(y*m_ImageSizeX) + x] = 255;

	//		}
	//	}
	//}
	// Ary[x][y] = Ary[(x*widthX)+y];

}

int CTestMedicalProcessingDlg::RoundFunc(int r) {
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

void CTestMedicalProcessingDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}


void CTestMedicalProcessingDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnRButtonDown(nFlags, point);
}


BOOL CTestMedicalProcessingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_UP)
	{
		m_WidthVal += 20;
		MakeBitmap();
		RedrawWindowArea();

		/*

		*/
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTestMedicalProcessingDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
