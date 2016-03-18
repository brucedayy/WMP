// MediaPlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MediaPlayer.h"
#include "MediaPlayerDlg.h"
#include <cstdlib>
#include <afx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaPlayerDlg dialog

bool flag = false;
int count = 0;
int indexList;
int spaceflag = 1;

CMediaPlayerDlg::CMediaPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMediaPlayerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMediaPlayerDlg)
	m_order = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMediaPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMediaPlayerDlg)
	DDX_Control(pDX, IDC_SLIDERVOL, m_SliderVol);
	DDX_Control(pDX, IDC_LISTP, m_ListP);
	DDX_Control(pDX, IDC_MEDIAPLAYER, m_ActiveMovie);
	DDX_Radio(pDX, IDC_RADIOSEQUENCEPLAY, m_order);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SLIDERPOCESS, m_SliderProcess);
	DDX_Control(pDX, IDC_CURRENTPOS, m_CurrentPos);
	DDX_Control(pDX, IDC_ENDPOS, m_EndPos);
}

BEGIN_MESSAGE_MAP(CMediaPlayerDlg, CDialog)
	//{{AFX_MSG_MAP(CMediaPlayerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPENFILE, OnOpenFile)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_BN_CLICKED(IDC_PLAYLIST, OnPlayList)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_FULLSCREEN, OnFullscreen)
	ON_BN_CLICKED(IDC_FALL, OnFall)
	ON_BN_CLICKED(IDC_SPEED, OnSpeed)
	ON_BN_CLICKED(IDC_LASTSONG, OnLastSong)
	ON_BN_CLICKED(IDC_NEXTSONG, OnNextSong)
	ON_BN_CLICKED(IDC_ADDVOL, OnAddvol)
	ON_BN_CLICKED(IDC_SUBVOL, OnSubvol)
	ON_NOTIFY(NM_CLICK, IDC_LISTP, OnClickList)
	ON_BN_CLICKED(IDC_CHECKMUTE, OnCheckmute)	
	ON_BN_CLICKED(IDC_RADIOSEQUENCEPLAY, Onsequenceplay)
	ON_BN_CLICKED(IDC_RADIORANDOMPLAY, OnRandomPlay)
	ON_BN_CLICKED(IDC_RADIOLOOPPLAY, OnLoopPlay)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_QUIT, OnClose)
	ON_WM_HSCROLL(IDC_SLIDERVOL,OnHScroll)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_NCRBUTTONDBLCLK()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMediaPlayerDlg message handlers

BOOL CMediaPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	//关联控制
	m_MediaControl=static_cast<CWMPControls>(m_ActiveMovie.GetControls());
	m_MediaSetting=m_ActiveMovie.GetSettings();
	m_MediaList=static_cast<CWMPPlaylist>(m_ActiveMovie.GetCurrentPlaylist());
	m_Media=static_cast<CWMPMedia>(m_ActiveMovie.GetCurrentMedia());


	CRect rect;
	m_ListP.GetClientRect(&rect);
	m_ListP.SetExtendedStyle(m_ListP.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListP.InsertColumn(0, _T("名称"), LVCFMT_CENTER, 2 * rect.Width() / 4, 0);
	m_ListP.InsertColumn(1, _T("格式"), LVCFMT_CENTER, rect.Width() / 4, 1);
	m_ListP.InsertColumn(2, _T("时长"), LVCFMT_CENTER, rect.Width() / 4, 2);
	m_ListP.InsertColumn(3, NULL, LVCFMT_CENTER, 0, 3);
	m_ListP.InsertColumn(4, NULL, LVCFMT_CENTER, 0, 4);

	//设置声音滑动条的样式
	m_SliderVol.SetRange(0,100,false);
	m_SliderVol.SetTicFreq(10);
	m_SliderVol.SetLineSize(1);

	//设置声音滑动条的初始位置
	pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERVOL);
	pSlidCtrl->SetPos(m_MediaSetting.GetVolume());

	//设置进度条样式
	m_SliderProcess.SetRange(0, 200, false);
	m_SliderProcess.SetTicFreq(1);
	m_SliderProcess.SetLineSize(1);
	m_SliderProcess.SetPos(0);	

	m_MediaSetting.SetAutoStart(false);  //禁止其自动开始
	SetDlgItemText(IDC_CURRENTPOS,NULL);
	SetDlgItemText(IDC_ENDPOS,NULL);
	SetDlgItemText(IDC_CURRENTPOS,_T("00:00"));
	SetDlgItemText(IDC_ENDPOS,_T("00:00"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMediaPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMediaPlayerDlg::OnPaint() 
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(rect);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpBackground;
	bmpBackground.LoadBitmapW(IDB_BITMAP3);
	BITMAP bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap *pbmpOld = dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY); 
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMediaPlayerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMediaPlayerDlg::OnOpenFile()
{
	// TODO: Add your control notification handler code here
	m_ListP.SetFocus(); 	
	CString str;
	POSITION pos;
	pos = m_ListP.GetFirstSelectedItemPosition();
	indexList = m_ListP.GetNextSelectedItem(pos);
	m_ListP.SetItemState(indexList, ~LVIS_SELECTED, LVIS_SELECTED);
	TCHAR szFilter[] = 
		_T("Mp3 File(*.mp3)|*.mp3|Wma File(*.wma)|\
		*.wma|Video File(*.dat)|*.dat|Wave File(*.wav)|\
		*.wav|AVI File(*.avi)|*.avi|Movie File(*.mov)|\
		*.mov|Media File(*.mmm)|*.mmm|Mid File(*.mid;*,rmi)|\
		*.mid;*.rmi|MPEG File(*.mpeg)|*.mpeg|All File(*.*)|*.*||");
	CFileDialog fileDlg(TRUE,NULL, NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilePath;
	if (IDOK == fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();
		strFilePath.MakeUpper();
		m_ActiveMovie.SetUrl(strFilePath);
		m_Media=static_cast<CWMPMedia>(m_ActiveMovie.newMedia(strFilePath));
		m_MediaList.appendItem(m_Media);	
		m_ListP.InsertItem(0, m_Media.GetName());
		m_ListP.SetItemText(0, 1, CString(m_Media.getItemInfo(_T("FileType"))));
		m_ListP.SetItemText(0, 2, CString(m_Media.GetDurationString()));
		m_ListP.SetItemText(0, 3, CString(strFilePath));
		m_ListP.SetItemText(0, 4, CString(m_Media.getItemInfo(_T("Duration"))));
		m_ActiveMovie.SetUrl(m_ListP.GetItemText(0, 3));
		m_ListP.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);		
	}
}

void CMediaPlayerDlg::OnPlay() 
{
	// TODO: Add your control notification handler code here
	CFileFind finder;
	if(m_MediaList.GetCount()<=0)
	{
		MessageBox(_T("列表无文件！"),_T("Warning"),MB_OK);
		spaceflag=1;
	}
	else
	{
		if(!finder.FindFile(m_ActiveMovie.GetUrl()))
		{
			MessageBox(_T("文件不已存在，该列项将被删除！"),_T("Warning"),MB_OK);
		}
		else
		{
			KillTimer(0);
			m_MediaControl.play();
			if(m_MediaControl.GetCurrentPosition()==0)
				m_MediaControl.SetCurrentPosition(0.01);
			SetTimer(0,20,NULL);
			spaceflag=0;
		}
	}

}

void CMediaPlayerDlg::OnPause() 
{
	// TODO: Add your control notification handler code here
	m_MediaControl.pause();
	spaceflag=1;
}

void CMediaPlayerDlg::OnPlayList()
{
	// TODO: Add your control notification handler code here
	if(flag=!flag)
	{
		SetWindowPos(NULL,0,0,480,410,SWP_NOMOVE);
	}
	else
	{
		SetWindowPos(NULL,0,0,760,410,SWP_NOMOVE);
		
	}	
}


void CMediaPlayerDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	KillTimer(0);
	m_MediaControl.stop();
	SetDlgItemText(IDC_CURRENTPOS, _T("00:00"));  //设置静态文本的值
	SetDlgItemText(IDC_ENDPOS, _T("00:00"));
	m_SliderProcess.SetPos(0);
}


void CMediaPlayerDlg::OnFullscreen() 
{
	// TODO: Add your control notification handler code here
	if(m_MediaControl.GetCurrentPosition()!=0&&!m_ActiveMovie.GetFullScreen())
	{		
		m_ActiveMovie.SetFullScreen(true);
	}	
}

void CMediaPlayerDlg::OnFall()
{
	// TODO: Add your control notification handler code here
	double t = m_MediaControl.GetCurrentPosition();
	m_MediaControl.SetCurrentPosition(t=t-10);	
}


void CMediaPlayerDlg::OnSpeed()
{
	// TODO: Add your control notification handler code here
	double t = m_MediaControl.GetCurrentPosition();
	m_MediaControl.SetCurrentPosition(t=t+10);	
}

void CMediaPlayerDlg::OnLastSong() 
{
	// TODO: Add your control notification handler code here
	m_ListP.SetFocus();
	POSITION pos;CString strName;
	pos = m_ListP.GetFirstSelectedItemPosition();
	indexList = m_ListP.GetNextSelectedItem(pos);
	if(indexList == -1)
	{
		AfxMessageBox(_T("列表内没有被选中项！"));
		return;
	}
	if(indexList == 0)
	{
		AfxMessageBox(_T("已经到了列表头部！"));
		m_ListP.SetItemState(indexList, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		return;
	}
	m_ListP.SetItemState(indexList, ~LVIS_SELECTED, LVIS_SELECTED);
	m_ListP.SetItemState(indexList - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	strName = m_ListP.GetItemText(indexList - 1, 3);
	CMediaPlayerDlg::OnStop();
	m_ActiveMovie.SetUrl(strName);
	CMediaPlayerDlg::OnPlay();
}


void CMediaPlayerDlg::OnNextSong() 
{
	// TODO: Add your control notification handler code here
	m_ListP.SetFocus();
	POSITION pos;CString strName;
	UINT flag1 = LVIS_SELECTED|LVIS_FOCUSED;
	pos = m_ListP.GetFirstSelectedItemPosition();
	indexList = m_ListP.GetNextSelectedItem(pos);
	if(indexList == -1)
	{
		AfxMessageBox(_T("列表内没有被选中项！"));
		return;
	}
	if (indexList == m_ListP.GetItemCount() - 1)
	{
		AfxMessageBox(_T("已经到了列表末尾！"));
		m_ListP.SetItemState(indexList, flag1, flag1);
		return;
	}
	m_ListP.SetItemState(indexList, ~LVIS_SELECTED, LVIS_SELECTED);
	m_ListP.SetItemState(indexList + 1, flag1, flag1);
	strName = m_ListP.GetItemText(indexList + 1, 3);
	CMediaPlayerDlg::OnStop();
	m_ActiveMovie.SetUrl(strName);
	CMediaPlayerDlg::OnPlay();
}

void CMediaPlayerDlg::OnAddvol() 
{
	// TODO: Add your control notification handler code here
	m_MediaSetting.SetVolume(m_MediaSetting.GetVolume()+10);
	m_SliderVol.SetPos(m_MediaSetting.GetVolume());
}


void CMediaPlayerDlg::OnSubvol() 
{
	// TODO: Add your control notification handler code here
	int Volume=m_MediaSetting.GetVolume();
	m_MediaSetting.SetVolume(Volume=Volume-10);
	m_SliderVol.SetPos(m_MediaSetting.GetVolume());
}

void CMediaPlayerDlg::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);   

	// TODO: Add your control notification handler code here
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	*pResult = 0;
	CString strName;
	if(-1 !=pNMListView->iItem)
	{
		strName = m_ListP.GetItemText(pNMListView->iItem, 3);
		CMediaPlayerDlg::OnStop();
		m_ActiveMovie.SetUrl(strName);
		CMediaPlayerDlg::OnPlay();
	}


}

void CMediaPlayerDlg::OnCheckmute()
{
	// TODO: Add your control notification handler code here
		if(!m_MediaSetting.GetMute())
		m_MediaSetting.SetMute(true);
		else
		m_MediaSetting.SetMute(false);
}

void CMediaPlayerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	switch(nIDEvent)
	{
	case 0:   //计时器选择(此处只有ID为0的计时器，可省)
	{
		    if(m_MediaControl.GetCurrentPosition()==0)
			{	
				m_SliderProcess.SetPos(0);
				SetDlgItemText(IDC_CURRENTPOS, _T("00:00"));
				SetDlgItemText(IDC_ENDPOS, _T("00:00"));
			switch(m_order)
			{
				case 0:
					{
						KillTimer(0);
						POSITION pos;
						pos = m_ListP.GetFirstSelectedItemPosition();
						indexList = m_ListP.GetNextSelectedItem(pos);
						m_ListP.SetFocus();
						if (indexList== m_ListP.GetItemCount() - 1)
						{
							m_ListP.SetItemState(indexList, LVIS_SELECTED, LVIS_SELECTED);
							m_MediaControl.stop();
							return;
						}
						m_ListP.SetItemState(indexList, ~LVIS_SELECTED, LVIS_SELECTED);
						m_ListP.SetItemState(indexList+ 1, LVIS_SELECTED, LVIS_SELECTED);
						CString strName = m_ListP.GetItemText(indexList + 1, 3);
						m_ActiveMovie.SetUrl(strName);
						CMediaPlayerDlg::OnPlay();
						break;
					}
				case 1:
					{
						KillTimer(0);
						m_ListP.SetFocus();
						POSITION pos;
						pos = m_ListP.GetFirstSelectedItemPosition();
						indexList = m_ListP.GetNextSelectedItem(pos);
						m_ListP.SetItemState(indexList, ~LVIS_SELECTED, LVIS_SELECTED);
						int index;
						srand((unsigned)time( NULL ));
						index = rand() % m_ListP.GetItemCount();
						m_ListP.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
						CString strName = m_ListP.GetItemText(index, 3);
						m_ActiveMovie.SetUrl(strName);
						CMediaPlayerDlg::OnPlay();
						break;
					}				
				case 2:
					{
						KillTimer(0);
						CMediaPlayerDlg::OnPlay();
						break;
					}					
				default:
					{					
						KillTimer(0);
						break;
					}
			}
					break;
			}			
			POSITION pos;
			double td;
			pos = m_ListP.GetFirstSelectedItemPosition();
			indexList = m_ListP.GetNextSelectedItem(pos);
			td = _ttol(m_ListP.GetItemText(indexList, 4));
			m_SliderProcess.SetPos((int)((m_MediaControl.GetCurrentPosition() / td) * 200));
			SetDlgItemText(IDC_ENDPOS, m_ListP.GetItemText(indexList, 2));
			SetDlgItemText(IDC_CURRENTPOS,m_MediaControl.GetCurrentPositionString());
			break;
	}
	}		
	CDialog::OnTimer(nIDEvent);
}

void CMediaPlayerDlg::Onsequenceplay()
{
	// TODO: Add your control notification handler code here
	m_order=0;
}

void CMediaPlayerDlg::OnRandomPlay()
{
	// TODO: Add your control notification handler code here
	m_order=1;
}

void CMediaPlayerDlg::OnLoopPlay()
{
	// TODO: Add your control notification handler code here
	m_order=2;
}

void CMediaPlayerDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CMediaPlayerDlg::OnClose()
{
	// TODO: Add your control notification handler code here
	CMediaPlayerDlg::OnStop();
	SendMessage(WM_CLOSE);
}

void CMediaPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your control notification handler code here
	int m_int;
	m_int = pSlidCtrl->GetPos();
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	m_MediaSetting.SetVolume(m_int);
}



void CMediaPlayerDlg::OnNcPaint()
{
	// TODO: Add your control notification handler code here
	CWindowDC dc(this);
	CRect rect;
	GetWindowRect(rect);
	rect.top = 0;
	rect.bottom = 32;
	rect.left = 0;
	rect.right = 767;
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpBackground;
	bmpBackground.LoadBitmapW(IDB_BITMAP2);
	BITMAP bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap *pbmpOld = dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	CRect rcleftedge;
	GetWindowRect(rcleftedge);
	rcleftedge.top = 32;
	rcleftedge.bottom = 414;
	rcleftedge.left = 0;
	rcleftedge.right = 10;
	dc.FillSolidRect(rcleftedge, RGB(0, 0, 0));
	CRect rcrightedge;
	GetWindowRect(rcrightedge);
	rcrightedge.top = 32;
	rcrightedge.bottom = 414;
	rcrightedge.left = 747;
	rcrightedge.right = 767;
	dc.FillSolidRect(rcrightedge, RGB(0, 0, 0));
	CRect rcbottomedge;
	GetWindowRect(rcbottomedge);
	rcbottomedge.top = 401;
	rcbottomedge.bottom = 414;
	rcbottomedge.left = 10;
	rcbottomedge.right = 767;
	dc.FillSolidRect(rcbottomedge, RGB(0, 0, 0));
}


BOOL CMediaPlayerDlg::OnNcActivate(BOOL bActive)
{
	// TODO: Add your control notification handler code here
	CMediaPlayerDlg::OnNcPaint();
	return TRUE;
}

void CMediaPlayerDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your control notification handler code here
	CMediaPlayerDlg::OnNcPaint();
	CDialog::OnNcLButtonDown(nHitTest, point);	
}


void CMediaPlayerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your control notification handler code here
	CMediaPlayerDlg::OnNcPaint();
	CDialog::OnLButtonUp(nFlags, point);
	//处理单击About	
}


void CMediaPlayerDlg::OnNcRButtonDblClk(UINT nHitTest, CPoint point)
{
	// TODO: Add your control notification handler code here
	CAboutDlg cad;
	cad.DoModal();

	CDialog::OnNcRButtonDblClk(nHitTest, point);
}
