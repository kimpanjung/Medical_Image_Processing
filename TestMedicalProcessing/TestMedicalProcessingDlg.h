
// TestMedicalProcessingDlg.h : header file
//

#pragma once
#include "atltypes.h"

#define IMAGETYPE unsigned short


// CTestMedicalProcessingDlg dialog
class CTestMedicalProcessingDlg : public CDialogEx
{
// Construction
public:
	CTestMedicalProcessingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTMEDICALPROCESSING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


public:

	void CloseGraphicsMem(void);
	void RedrawWindowArea(void);
	void OpenFile(CString FileName, int SizeX, int SizeY);
	void BmpHeaderPalette(void);
	bool File2Memory(CString OpenFileName, IMAGETYPE *Memory, int imagesize_x, int imagesize_y);
	void MakeBitmap(void);
	void ReMakeBitmap(void);
	void UpdateScreen(Graphics* pGraphics);
	void SetImageMemory(void);
	void CloseImageMemory(void);
	int RoundFunc(int r);
	void MakePalette(void);

public:
	// Graphics
	Graphics*		m_pMemGraphics;  // memory graphics
	Bitmap*			m_pMemBitmap;
	CachedBitmap*	m_pCachedBitmap;

	CString m_FileName;

	int m_ImageSizeX;
	int m_ImageSizeY;

	CSize m_WindowSize;

	IMAGETYPE *m_RawImage;
	unsigned char *m_ViewImage;

	Bitmap *m_BitmapImage;
	BITMAPINFO m_BitmapInfo;
	ColorPalette *m_Palette;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	void PixelMaxMindata();
	void WidthLevelData();
	unsigned short m_PixelMax;
	unsigned short m_PixelMin;
	int m_WidthVal;
	int m_LevelVal;
	void Leveling();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
