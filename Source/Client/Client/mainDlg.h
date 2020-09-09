#pragma once

// mainDlg dialog

class mainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(mainDlg)

public:
	mainDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~mainDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBoxEx date;
	CString command, day, query;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
