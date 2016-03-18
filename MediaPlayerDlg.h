// MediaPlayerDlg.h : header file
//
//{{AFX_INCLUDES()
#include "wmpplayer4.h"
#include "wmpcontrols.h"
#include "wmpsettings.h"
#include "wmpplaylist.h"
#include "wmpmedia.h"
#include "afxcmn.h"
#include "afxwin.h"
//}}AFX_INCLUDES

#if !defined(AFX_MEDIAPLAYERDLG_H__4153FF65_B2BF_4F5A_93E7_A476913CF86C__INCLUDED_)
#define AFX_MEDIAPLAYERDLG_H__4153FF65_B2BF_4F5A_93E7_A476913CF86C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMediaPlayerDlg dialog

class CMediaPlayerDlg : public CDialog
{
// Construction
public:
	CMediaPlayerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMediaPlayerDlg)
	enum { IDD = IDD_MEDIAPLAYER_DIALOG };
	CSliderCtrl	m_SliderVol;
	CListCtrl	m_ListP;
	CWMPPlayer4	m_ActiveMovie;
	CWMPControls m_MediaControl;
	CWMPSettings m_MediaSetting;
	CString	m_position;
	CWMPPlaylist m_MediaList;
	CWMPMedia m_Media;
	int		m_order;
	CSliderCtrl *pSlidCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMediaPlayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMediaPlayerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenFile();
	afx_msg void OnPlay();
	afx_msg void OnPause();
	afx_msg void OnPlayList();
	afx_msg void OnStop();
	afx_msg void OnFullscreen();
	afx_msg void OnFall();
	afx_msg void OnSpeed();
	afx_msg void OnLastSong();
	afx_msg void OnNextSong();
	afx_msg void OnAddvol();
	afx_msg void OnSubvol();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckmute();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void Onsequenceplay();
	afx_msg void OnRandomPlay();
	afx_msg void OnLoopPlay();
	afx_msg void OnAbout();
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_SliderProcess;
	CStatic m_CurrentPos;
	CStatic m_EndPos;
	CSliderCtrl *pSliderProcess;
	afx_msg void OnNcPaint();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcRButtonDblClk(UINT nHitTest, CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDIAPLAYERDLG_H__4153FF65_B2BF_4F5A_93E7_A476913CF86C__INCLUDED_)
