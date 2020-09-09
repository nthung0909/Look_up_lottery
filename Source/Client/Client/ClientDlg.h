// ClientDlg.h : header file

#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#define PORT 25000
#define WM_SOCKET WM_USER+2

// CClientDlg dialog
class CClientDlg : public CDialog
{
	// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	enum { IDD = IDD_CLIENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	LRESULT SockMsg(WPARAM wParam,LPARAM lParam);
	char* ConvertToChar(const CString &s);
	void Split(CString src,CString des[3]);
	void mSend(CString Command);
	int mRecv(CString &Command);

	SOCKET sClient;
	sockaddr_in servAdd;
	CString Command;
	int	buffLength;
	CString strResult[3];
	CString userOnline;
	int level;
	int isLogin=false;

	CString m_msgString;
	afx_msg void OnBnClickedLogin();
	CString IP, m_userName, m_password;
	afx_msg void OnBnClickedLogout();
	afx_msg void OnBnClickedRegister();
};
