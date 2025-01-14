
// GlimMfcDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "GlimMfc.h"
#include "GlimMfcDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"
#include <iostream>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
// 콘솔 출력 확인
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGlimMfcDlg 대화 상자



CGlimMfcDlg::CGlimMfcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIMMFC_DIALOG, pParent)
	, m_nRadius(10) // 반지름 초기값
	, m_nThichness(1)// 두께초기값
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGlimMfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_nRadius);
	DDX_Text(pDX, IDC_EDIT_THICKNESS, m_nThichness);
}

BEGIN_MESSAGE_MAP(CGlimMfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_IMAGE, &CGlimMfcDlg::OnBnClickedBtnImage)
	ON_BN_CLICKED(IDC_BTN_RESET, &CGlimMfcDlg::OnBnClickedBtnReset)
//	ON_WM_MBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_BN_CLICKED(IDC_BTN_RANDOM, &CGlimMfcDlg::OnBnClickedBtnRandom)
END_MESSAGE_MAP()


// CGlimMfcDlg 메시지 처리기

BOOL CGlimMfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_count = 0;
	m_check = false;
	m_drag = 0;
	c1x, c1y, c2x, c2y, c3x, c3y = 0;
	m_imageStartX = 150;
	m_imageStartY = 50;
	m_pThread = NULL;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CGlimMfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CGlimMfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CGlimMfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//이미지 생성 버튼
void CGlimMfcDlg::OnBnClickedBtnImage()
{
	CreateImage();
}

//이미지 좌클릭
void CGlimMfcDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	std::cout << m_count << "카운트" << std::endl;
	if (m_check == false)
	{
		MessageBox(_T("이미지를 생성해주세요"), _T("안내"), MB_OK);
		return;
	}

	UpdateData(true);
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nRadius = m_nRadius;

	int nPitch = m_image.GetPitch(); 
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	if (nRadius != 0 && m_count < 3 && m_nThichness != 0)
	{
		// 클릭 좌표가 이미지 영역 내에 있는지 확인
		if (point.x >= m_imageStartX && point.x < m_imageStartX + nWidth && point.y >= m_imageStartY && point.y < m_imageStartY + nHeight)
		{
			std::cout << "count:" << m_count << std::endl;
			// 이미지 내부 좌표 계산
			int nimageX = point.x - m_imageStartX;
			int nimageY = point.y - m_imageStartY;

			std::cout << "x:" << nimageX << std::endl;
			std::cout << "y:" << nimageY << std::endl;

			drawCircle(fm, nimageX, nimageY, nRadius);

			nCiecleX[m_count] = nimageX;
			nCiecleY[m_count] = nimageY;

			CString str;

			str.Format(_T("x좌표: %d , y좌표: %d"), nCiecleX[m_count], nCiecleY[m_count]);

			if (m_count == 0)
				SetDlgItemText(IDC_EDIT_C1, str);
			else if (m_count == 1)
				SetDlgItemText(IDC_EDIT_C2, str);
			else if (m_count == 2)
				SetDlgItemText(IDC_EDIT_C3, str);

			m_count++;

			for (int i = 0; i < m_count; i++)
			{
				std::cout << "x좌표저장" << nCiecleX[i] << std::endl;
				std::cout << "y좌표저장" << nCiecleY[i] << std::endl;
			}
		}
	}
	else if(nRadius == 0)
		MessageBox(_T("반지름 크기를 입력해주세요"), _T("안내"), MB_OK);
	else if(m_nThichness == 0)
		MessageBox(_T("두께를 1이상으로 설정해주세요"), _T("안내"), MB_OK);

	if (m_count == 3)
	{
		c1x = nCiecleX[0];
		c1y = nCiecleY[0];
		c2x = nCiecleX[1];
		c2y = nCiecleY[1];
		c3x = nCiecleX[2];
		c3y = nCiecleY[2];
		drawBigCircle(fm, c1x, c1y, c2x, c2y, c3x, c3y);
		m_count++;
	}

	if (m_ThreadRun == false)
	{
		if (ClickCheck(c1x, c1y, point.x - m_imageStartX, point.y - m_imageStartY, nRadius) && m_count == 4)
			m_drag = 1;
		else if (ClickCheck(c2x, c2y, point.x - m_imageStartX, point.y - m_imageStartY, nRadius))
			m_drag = 2;
		else if (ClickCheck(c3x, c3y, point.x - m_imageStartX, point.y - m_imageStartY, nRadius))
			m_drag = 3;
	}

	CDialogEx::OnLButtonDown(nFlags, point);

	UpdateDisplay();
}

//원그리기
void CGlimMfcDlg::drawCircle(unsigned char* fm, int x, int y, int nRadius)
{
	int nCenterX = x;
	int nCenterY = y;
	int nPitch = m_image.GetPitch();

	for (int j = y - nRadius; j < y + nRadius; j++)
	{
		for (int i = x - nRadius; i < x + nRadius; i++)
		{
			if (j < 0 || j >= m_image.GetHeight() || i < 0 || i >= m_image.GetWidth())
				continue;
			if (isInCircle(i, j, nCenterX, nCenterY, nRadius))
				fm[j * nPitch + i] = 80;
		}
	}
}

//작은원 좌표검사
bool CGlimMfcDlg::isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
	bool bRet = false;

	double dx = i - nCenterX;
	double dy = j - nCenterY;
	double dDist = dx * dx + dy * dy;

	if (dDist < nRadius * nRadius)
		bRet = true;

	return bRet;
}

//클릭체크
bool CGlimMfcDlg::ClickCheck(int x, int y, int tx, int ty, int nRadius)
{
	bool bRet = false;

	if (pow(nRadius,2) >= pow(x - tx,2) + pow(y - ty,2))
		bRet = true;

	return bRet;
}

//큰원그리기
void CGlimMfcDlg::drawBigCircle(unsigned char* fm, int c1x, int c1y, int c2x, int c2y, int c3x, int c3y)
{
	double D = c1x * (c2y - c3y) + c2x * (c3y - c1y) + c3x * (c1y - c2y);

	double Cx = ((pow(c1x, 2) + pow(c1y, 2)) * (c2y - c3y)) +
		((pow(c2x, 2) + pow(c2y, 2)) * (c3y - c1y)) +
		((pow(c3x, 2) + pow(c3y, 2)) * (c1y - c2y));
	double Cy = ((pow(c1x, 2) + pow(c1y, 2)) * (c3x - c2x)) +
		((pow(c2x, 2) + pow(c2y, 2)) * (c1x - c3x)) +
		((pow(c3x, 2) + pow(c3y, 2)) * (c2x - c1x));
	double C = ((pow(c1x, 2) + pow(c1y, 2)) * (c2x * c3y - c3x * c2y)) +
		((pow(c2x, 2) + pow(c2y, 2)) * (c3x * c1y - c1x * c3y)) +
		((pow(c3x, 2) + pow(c3y, 2)) * (c1x * c2y - c2x * c1y));

	double h = -Cx / (2 * D);
	double k = -Cy / (2 * D);
	double r = sqrt(pow((c1x + h), 2) + pow((c1y + k), 2));

	int nCenterX = -h;
	int nCenterY = -k;
	int nPitch = m_image.GetPitch();

	for (int j = nCenterY - r; j < nCenterY + r; j++)
	{
		for (int i = nCenterX - r; i < nCenterX + r; i++)
		{
			if (j < 0 || j >= m_image.GetHeight() || i < 0 || i >= m_image.GetWidth())
				continue;
			if (isInBigCircle(i, j, nCenterX, nCenterY, r))
				fm[j * nPitch + i] = 80;
		}
	}
}

//큰원 좌표검사
bool CGlimMfcDlg::isInBigCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
	bool bRet = false;

	double dx = i - nCenterX;
	double dy = j - nCenterY;
	double dDist = dx * dx + dy * dy;
	int thickness = m_nThichness;
	if (dDist <= nRadius * nRadius && dDist >= (nRadius - thickness) * (nRadius - thickness))
		bRet = true;

	return bRet;
}

//이미지 갱신
void CGlimMfcDlg::UpdateDisplay()
{
	CClientDC dc(this);
	m_image.Draw(dc, 150, 50);
}

//초기화 버튼
void CGlimMfcDlg::OnBnClickedBtnReset()
{
	m_image.Destroy();
	CreateImage();
	m_count = 0;
	SetDlgItemText(IDC_EDIT_C1, NULL);
	SetDlgItemText(IDC_EDIT_C2, NULL);
	SetDlgItemText(IDC_EDIT_C3, NULL);
	m_ThreadRun = false;
}

//이미지 생성
void CGlimMfcDlg::CreateImage()
{
	m_check = true;
	int nWidth = 700;
	int nHeight = 450;
	int nBpp = 8;

	if (m_image == NULL)
	{
		//이미지만들기
		m_image.Create(nWidth, nHeight, nBpp);
		if (nBpp == 8)
		{
			static RGBQUAD rgb[256];
			for (int i = 0; i < 256; i++)
				rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
			m_image.SetColorTable(0, 256, rgb);

			int nPitch = m_image.GetPitch();
			unsigned char* fm = (unsigned char*)m_image.GetBits();

			for (int j = 0; j < nHeight; j++)
			{
				for (int i = 0; i < nWidth; i++)
				{
					fm[j * nPitch + i] = 255;
				}
			}
			CClientDC dc(this);
			m_image.Draw(dc, 150, 50);
		}
	}
}

void CGlimMfcDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_drag = 0;
	if (m_drag == 0 && m_count == 3)
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CGlimMfcDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	int nRadius = m_nRadius;
	int nWidth = 700;
	int nHeight = 450;
	int m_DragX = point.x - m_imageStartX;
	int m_DragY = point.y - m_imageStartY;

	if (m_drag == 1 && m_check && m_count == 4)
	{
		c1x = m_DragX;
		c1y = m_DragY;
		m_image.Destroy();
		CreateImage();
		unsigned char* fm = (unsigned char*)m_image.GetBits();
		drawCircle(fm, c1x, c1y, nRadius);
		drawCircle(fm, c2x, c2y, nRadius);
		drawCircle(fm, c3x, c3y, nRadius);
		drawBigCircle(fm, c1x, c1y, c2x, c2y, c3x, c3y);
		UpdateDisplay();
	}
	else if (m_drag == 2 && m_check && m_count == 4)
	{
		c2x = m_DragX;
		c2y = m_DragY;
		m_image.Destroy();
		CreateImage();
		unsigned char* fm = (unsigned char*)m_image.GetBits();
		drawCircle(fm, c1x, c1y, nRadius);
		drawCircle(fm, c2x, c2y, nRadius);
		drawCircle(fm, c3x, c3y, nRadius);
		drawBigCircle(fm, c1x, c1y, c2x, c2y, c3x, c3y);
		UpdateDisplay();
	}
	else if (m_drag == 3 && m_check && m_count == 4)
	{
		c3x = m_DragX;
		c3y = m_DragY;
		m_image.Destroy();
		CreateImage();
		unsigned char* fm = (unsigned char*)m_image.GetBits();
		drawCircle(fm, c1x, c1y, nRadius);
		drawCircle(fm, c2x, c2y, nRadius);
		drawCircle(fm, c3x, c3y, nRadius);
		drawBigCircle(fm, c1x, c1y, c2x, c2y, c3x, c3y);
		UpdateDisplay();
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

//랜덤이동버튼
void CGlimMfcDlg::OnBnClickedBtnRandom()
{
	RandomMove();
}

//랜덤이동
void CGlimMfcDlg::RandomMove()
{
	m_ThreadRun = true;
	m_pThread = AfxBeginThread(ThreadFunction, this);
}

//쓰레드 기능
UINT CGlimMfcDlg::ThreadFunction(LPVOID pParam)
{
	int nWidth = 700;
	int nHeight = 450;
	CGlimMfcDlg* pDlg = (CGlimMfcDlg*)pParam; // 다이얼로그 클래스 포인터
	int nRadius = pDlg -> m_nRadius;
	CString str1, str2, str3;

	if (pDlg->m_check && pDlg->m_ThreadRun)
	{
		for (int i = 0; i < 10; ++i)
		{
			pDlg->c1x = rand() % nWidth;
			pDlg->c2x = rand() % nWidth;
			pDlg->c3x = rand() % nWidth;
			pDlg->c1y = rand() % nHeight;
			pDlg->c2y = rand() % nHeight;
			pDlg->c3y = rand() % nHeight;

			if (!pDlg->m_ThreadRun)
				break;
			pDlg->m_image.Destroy();
			pDlg->CreateImage();
			unsigned char* fm = (unsigned char*)pDlg ->m_image.GetBits();
			pDlg -> drawCircle(fm,  pDlg -> c1x,  pDlg -> c1y, nRadius);
			pDlg -> drawCircle(fm,  pDlg -> c2x,  pDlg -> c2y, nRadius);
			pDlg -> drawCircle(fm,  pDlg -> c3x,  pDlg -> c3y, nRadius);
			pDlg -> drawBigCircle(fm, pDlg->c1x, pDlg->c1y, pDlg->c2x, pDlg->c2y, pDlg->c3x, pDlg->c3y);

			str1.Format(_T("x좌표: %d , y좌표: %d"), pDlg->c1x, pDlg->c1y);
			pDlg->SetDlgItemText(IDC_EDIT_C1, str1);
			str2.Format(_T("x좌표: %d , y좌표: %d"), pDlg->c2x, pDlg->c2y);
			pDlg->SetDlgItemText(IDC_EDIT_C2, str2);
			str3.Format(_T("x좌표: %d , y좌표: %d"), pDlg->c3x, pDlg->c3y);
			pDlg->SetDlgItemText(IDC_EDIT_C3, str3);

			pDlg -> UpdateDisplay();
			Sleep(2000);
		}
	}
	else if(pDlg->m_check == false)
		pDlg -> MessageBox(_T("이미지를 생성해주세요"), _T("안내"), MB_OK);

	pDlg -> m_ThreadRun = false;

	return 0; // 쓰레드 종료
}



