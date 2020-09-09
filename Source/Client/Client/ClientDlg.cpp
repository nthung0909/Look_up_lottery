// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "RegisterDlg.h"
#include "mainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDlg dialog




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
: CDialog(CClientDlg::IDD, pParent)
/*, m_msgString(_T(""))*/
, IP(_T(""))
, m_userName(_T(""))
, m_password(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	/*DDX_Text(pDX, IDC_MSGBOX, m_msgString);*/
	DDX_Text(pDX, IDC_IP, IP);
	DDX_Text(pDX, IDC_USER, m_userName);
	DDX_Text(pDX, IDC_PASSWORD, m_password);

}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE (WM_SOCKET, SockMsg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LOGIN, &CClientDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_LOGOUT, &CClientDlg::OnBnClickedLogout)
	ON_BN_CLICKED(IDC_REGISTER, &CClientDlg::OnBnClickedRegister)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	IP="127.0.0.1";
	GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientDlg::Split(CString src,CString des[3])
{
	int p1,p2,p3;

	p1=src.Find(_T("\r\n"),0);
	des[0]=src.Mid(0,p1);

	p2=src.Find(_T("\r\n"),p1+1);
	des[1]=src.Mid(p1+2,p2-(p1+2));
	p3 = src.Find(_T("\r\n"), p2 + 1);
	des[2] = src.Mid(p2 + 2, p3 - (p2 + 2));
}

char* CClientDlg::ConvertToChar(const CString &s)
{
	int nSize = s.GetLength();
	char *pAnsiString = new char[nSize+1];
	memset(pAnsiString,0,nSize+1);
	wcstombs(pAnsiString, s, nSize+1);
	return pAnsiString;
}

void CClientDlg::mSend(CString Command)
{
	int Len=Command.GetLength();
	Len+=Len;
	PBYTE sendBuff = new BYTE[1000];
	memset(sendBuff,0,1000);
	memcpy(sendBuff,(PBYTE)(LPCTSTR)Command, Len);
	send(sClient,(char*)&Len,sizeof(Len),0);
	send(sClient,(char*)sendBuff,Len,0);
	delete sendBuff;
}

int CClientDlg::mRecv(CString &Command)
{
	PBYTE buffer = new BYTE[1000];
	memset(buffer,0, 1000);
	recv(sClient ,(char*)&buffLength,sizeof(int),0);
	recv(sClient,(char*)buffer,buffLength,0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;

	if(Command.GetLength()==0)
		return -1 ;
	return 0;
}
LRESULT CClientDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{

	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
		{
			CString temp;
			if(mRecv(temp)<0)
				break;

			Split(temp,strResult);
			int flag1=_ttoi(strResult[0]);
			int flag2=_ttoi(strResult[1]);
			
			switch(flag1)
			{
			case 1://login
				{
				if (flag2 == 1)
				{
					isLogin = true;
					GetDlgItem(IDC_LOGOUT)->EnableWindow(TRUE);
					GetDlgItem(IDC_LOGIN)->EnableWindow(FALSE);
					SetDlgItemText(IDC_REGISTER, _T("Query"));
					this->MessageBox(_T("Login successful!"), _T("Login"));
				}
				else
					this->MessageBox(_T("Login fail!!!"), _T("ERROR"));
		
					UpdateData(FALSE);
					break;
				}

			case 2:
				{
					if (flag2 == 1) {
						this->MessageBox(_T("Invalid Query Syntax!!!"));
					}
					else if (flag2 == 2||flag2==6) {
						this->MessageBox(_T("Invalid City!!!"));
					}
					else if (flag2 == 3) {
						this->MessageBox(_T("Invalid Day!!!"));
					}
					else if(flag2==4) {
						this->MessageBox(_T("Good luck next time!!!"));
					}
					else if(flag2 == 5) {
						int flag3 = _ttoi(strResult[2]);
						int money=2000000000;
						CString prize;
						prize.Format(_T("%d"), flag3-1);
						prize + _T("th");
						switch (flag3)
						{
						case 1:prize = _T("jackport");
							money = 2000000000;
							break;
						case 2:money = 30000000;
							break;
						case 3:money = 15000000;
							break;
						case 4:money = 10000000;
							break;
						case 5:money = 3000000;
							break;
						case 6:money = 1000000;
							break;
						case 7:money = 400000;
							break;
						case 8:money = 200000;
							break;
						default:
							break;
						}
						CString _money;
						_money.Format(_T("%d"), money);
						this->MessageBox(_T("Congratulation!!!\n\nYour prize :")+prize+_T("\n\nYour money:")+_money+_T("VND\n"));
					}
					else {
						this->MessageBox(strResult[2],_T("Prize in day"));
					}
					UpdateData(FALSE);
					break;
				}
			case 3:
				{
				this->MessageBox(_T("1. <Tên tỉnh thành>\n2. <Tên tỉnh thành> <số vé>\n"), _T("GUIDE"));
				break;
				}
			case 4: {
				if (flag2 == 1) {
					this->MessageBox(_T("Register Succesful!!!"), _T("REGISTER"));
				}
				else {
					this->MessageBox(_T("Invalid type of username \nor username is exists"), _T("FAIL"));
					RegisterDlg reg;
					reg.DoModal();
				}
				break;
			}
			}
			break;
		}
	case FD_CLOSE:
		{
			closesocket(sClient);
			isLogin = false;
			//m_msgString+=_T("Server is OFF\r\n");
			GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);
			GetDlgItem(IDC_LOGIN)->EnableWindow(TRUE);
			SetDlgItemText(IDC_REGISTER, _T("Register"));
			MessageBox(_T("Server is OFF"));
			UpdateData(FALSE);
			break;
		}

	}
	return 0;
}
void CClientDlg::OnBnClickedLogin()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	sClient = socket(AF_INET, SOCK_STREAM, 0);
	hostent* host=NULL;
	if (sClient == INVALID_SOCKET)
	{
		MessageBox(_T("socket() failed"),_T("ERROR"), 0);
		return;
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_port = htons(PORT);

	char* cIP=ConvertToChar(IP);

	servAdd.sin_addr.s_addr = inet_addr(cIP);

	CStringA cpy_IP(IP); 

	if (servAdd.sin_addr.s_addr==INADDR_NONE)
	{
		host = (gethostbyname(cpy_IP));
		if (host == NULL)
		{
			MessageBox(_T("Cannot connect to server!"),_T("ERROR"), 0);
		}
		CopyMemory(&servAdd.sin_addr, host->h_addr_list[0],
			host->h_length);
		return;
	}

	int err=connect(sClient,(struct sockaddr*)&servAdd,sizeof(servAdd));
	if (err==SOCKET_ERROR){
		MessageBox(_T("Connect failed!!"),_T("ERROR"), 0);
		return;
	}

	Command=_T("1\r\n");
	Command+= m_userName+_T("\r\n")+m_password+_T("\r\n");

	mSend(Command);

	WSAAsyncSelect(sClient,m_hWnd,WM_SOCKET,FD_READ|FD_CLOSE);
	UpdateData(FALSE);
}

void CClientDlg::OnBnClickedLogout()
{
	// TODO: Add your control notification handler code here
	INT_PTR i = MessageBox(_T("Are you sure?"),_T("Confirm"), MB_OKCANCEL);
	if (i == IDCANCEL)
		return;
	Command=_T("3\r\n");
	mSend(Command);
}

void CClientDlg::OnBnClickedRegister()
{
	// TODO: Add your control notification handler code here
	if (!isLogin) {
		UpdateData(TRUE);
		sClient = socket(AF_INET, SOCK_STREAM, 0);
		hostent* host = NULL;
		if (sClient == INVALID_SOCKET)
		{
			MessageBox(_T("socket() failed"), _T("ERROR"), 0);
			return;
		}

		servAdd.sin_family = AF_INET;
		servAdd.sin_port = htons(PORT);

		char* cIP = ConvertToChar(IP);

		servAdd.sin_addr.s_addr = inet_addr(cIP);

		CStringA cpy_IP(IP);

		if (servAdd.sin_addr.s_addr == INADDR_NONE)
		{
			host = (gethostbyname(cpy_IP));
			if (host == NULL)
			{
				MessageBox(_T("Cannot connect to server!"), _T("ERROR"), 0);
			}
			CopyMemory(&servAdd.sin_addr, host->h_addr_list[0],
				host->h_length);
			return;
		}

		int err = connect(sClient, (struct sockaddr*)&servAdd, sizeof(servAdd));
		if (err == SOCKET_ERROR) {
			MessageBox(_T("Connect failed!!"), _T("ERROR"), 0);
			return;
		}
		RegisterDlg reg_dlg;
		reg_dlg.DoModal();
		if (reg_dlg.username.CompareNoCase(_T("")) && reg_dlg.password.CompareNoCase(_T("")))
		{
			Command = _T("4\r\n");
			Command += reg_dlg.username + _T("\r\n") + reg_dlg.password + _T("\r\n");
			mSend(Command);
		}
	}
	else {
		mainDlg main;
		main.DoModal();
		CString tmp;
		CString _day = main.day, _query = main.query;
		tmp = _T("2\r\n") + _day + _T("\r\n") + _query + _T("\r\n");
		if (_query.CompareNoCase(_T(""))) {
			mSend(tmp);
		}
		if (!_query.CompareNoCase(_T("")))
			this->MessageBox(_T("Query mustn't null"), _T("ERROR"));
	}
}



