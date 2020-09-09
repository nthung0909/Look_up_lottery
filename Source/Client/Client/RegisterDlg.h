#pragma once

#include<fstream>
#include<string>
#include <regex>
using namespace std;
// RegisterDlg dialog

class RegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RegisterDlg)

public:
	RegisterDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RegisterDlg();
	
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	bool validateEmail(CString email);
	CString username, password;
	afx_msg void OnBnClickedRegister();
	afx_msg void OnBnClickedCancel();
};
