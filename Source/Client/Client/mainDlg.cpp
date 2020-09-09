// mainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "mainDlg.h"


// mainDlg dialog

IMPLEMENT_DYNAMIC(mainDlg, CDialogEx)

mainDlg::mainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN, pParent)
	,day(_T(""))
	,query(_T(""))
	,command(_T(""))
{

}

mainDlg::~mainDlg()
{
}

void mainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DAY, day);
	DDX_Text(pDX, IDC_QUERY, query);
	DDX_Control(pDX, IDC_DAY, date);

	CComboBox * date = (CComboBox*)GetDlgItem(IDC_DAY);
	date->AddString(_T("20/06"));
	date->AddString(_T("21/06"));
	date->AddString(_T("22/06"));
	date->AddString(_T("23/06"));
	date->AddString(_T("24/06"));
	date->AddString(_T("25/06"));
	date->AddString(_T("26/06"));
}


BEGIN_MESSAGE_MAP(mainDlg, CDialogEx)
	ON_BN_CLICKED(IDC_OK, &mainDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, &mainDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// mainDlg message handlers


void mainDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_DAY, day);
	GetDlgItemText(IDC_QUERY, query);
	OnCancel();
}


void mainDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	this->day = _T("");
	this->query = _T("");
	OnCancel();
}
