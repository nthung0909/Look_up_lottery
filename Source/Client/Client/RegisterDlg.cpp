// RegisterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"


// RegisterDlg dialog

IMPLEMENT_DYNAMIC(RegisterDlg, CDialogEx)

RegisterDlg::RegisterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGISTER, pParent)
	,username(_T(""))
	,password(_T(""))
{

}

RegisterDlg::~RegisterDlg()
{
}

void RegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERNAME, username);
	DDX_Text(pDX, IDC_PASSWORD, password);
}

BEGIN_MESSAGE_MAP(RegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_REGISTER, &RegisterDlg::OnBnClickedRegister)
	ON_BN_CLICKED(IDC_CANCEL, &RegisterDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// RegisterDlg message handlers

bool RegisterDlg::validateEmail(CString email) {
	return true;
}


void RegisterDlg::OnBnClickedRegister()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_USERNAME, username);
	GetDlgItemText(IDC_PASSWORD, password);
	if (password.GetLength() < 6) {
		MessageBox(_T("Password must be as least 6 characters"), _T("ERROR"));
	}else if(!username.CompareNoCase(_T("")))
		MessageBox(_T("Username mustn't null"), _T("ERROR"));
	else {
		CDialogEx::OnOK();
	}
}


void RegisterDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	username = _T("");
	password = _T("");
	CDialogEx::OnCancel();
}
