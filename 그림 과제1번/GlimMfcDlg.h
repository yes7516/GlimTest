
// GlimMfcDlg.h: 헤더 파일
//

#pragma once


// CGlimMfcDlg 대화 상자
class CGlimMfcDlg : public CDialogEx
{
// 생성입니다.
private:
	CImage m_image;
	INT m_count;
	bool m_check;
	int m_drag;
	int nCiecleX[3];
	int nCiecleY[3];
	int m_imageStartX;
	int m_imageStartY;
	bool m_ThreadRun;
	INT m_nRadius;
	INT m_nThichness;
	int m_DragX;
	int m_DragY;
	int c1x, c1y, c2x, c2y, c3x, c3y;
	CWinThread* m_pThread;
public:
	CGlimMfcDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIMMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void CGlimMfcDlg::drawCircle(unsigned char* fm, int x, int y, int nRadius);
	void CGlimMfcDlg::UpdateDisplay();
	bool CGlimMfcDlg::isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	void CGlimMfcDlg::drawBigCircle(unsigned char* fm, int c1x, int c1y, int c2x, int c2y, int c3x, int c3y);
	bool CGlimMfcDlg::isInBigCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	bool CGlimMfcDlg::ClickCheck(int i, int j, int nCenterX, int nCenterY, int nRadius);
	void CGlimMfcDlg::CreateImage();
	void CGlimMfcDlg::RandomMove();
	static UINT CGlimMfcDlg::ThreadFunction(LPVOID pParam);
	afx_msg void OnBnClickedBtnImage();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnRandom();
};
