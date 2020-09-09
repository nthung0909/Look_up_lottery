// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include <cctype>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerDlg dialog




CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
: CDialog(CServerDlg::IDD, pParent)
, m_msgString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BOXCHAT, m_msgString);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET,SockMsg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LISTEN, &CServerDlg::OnBnClickedListen)
	ON_BN_CLICKED(IDC_CANCEL, &CServerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerDlg::Split(CString src,CString des[3],CString regex)
{
	int p1,p2,p3;
	int space = regex.GetLength();
	p1=src.Find(regex,0);
	des[0]=src.Mid(0,p1);

	p2=src.Find(regex,p1+1);
	des[1]=src.Mid(p1+space,p2-(p1+space));

	p3 = src.Find(regex, p2 + 1);
	if(p3>-1)
		des[2] = src.Mid(p2 + space, p3 - (p2 + space));
}
char* CServerDlg::ConvertToChar(const CString &s)
{
	int nSize = s.GetLength();
	char *pAnsiString = new char[nSize+1];
	memset(pAnsiString,0,nSize+1);
	wcstombs(pAnsiString, s, nSize+1);
	return pAnsiString;
}

void CServerDlg::mSend(SOCKET sk, CString Command)
{
	int Len=Command.GetLength();
	Len+=Len;
	PBYTE sendBuff = new BYTE[1000];
	memset(sendBuff,0,1000);
	memcpy(sendBuff,(PBYTE)(LPCTSTR)Command, Len);
	send(sk,(char*)&Len,sizeof(Len),0);
	send(sk,(char*)sendBuff,Len,0);
	delete sendBuff;
}

int CServerDlg::mRecv(SOCKET sk, CString &Command)
{
	PBYTE buffer = new BYTE[1000];
	memset(buffer,0, 1000);
	recv(sk ,(char*)&buffLength,sizeof(int),0);
	recv(sk,(char*)buffer,buffLength,0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;

	if(Command.GetLength()==0)
		return -1;
	return 0;

}
bool CServerDlg::checkUsername(CString usn) {
	int len = usn.GetLength();
	if (len < 10)
		return false;
	if (usn.Find(_T("@gmail.com"), len - 10)<1)
		return false;
	ifstream f("../Source/Server/data/account.txt");
	//ifstream f("../data/account.txt");
	if (f.fail()) {
		return false;
	}
	else {
		string line;
		CString _line;
		while (f >> line)
		{
			_line = line.c_str();
			if (_line.CompareNoCase(usn)) {
				f >> line;
			}
			else {
				f.close();
				return false;
			}
		}
	}
	f.close();
	return true;
}

bool CServerDlg::checkLogin(CString usn, CString psw) {
	if (!usn.CompareNoCase(_T("")) || !psw.CompareNoCase(_T("")))
		return false;
	ifstream f;
	f.open("../Source/Server/data/account.txt", ios::app);
	//f.open("../data/account.txt");
	if (f.fail()) {
		MessageBox(_T("Can not open file"));
		return false;
	}
	else {
		string line;
		CString _line;
		while (f >> line)
		{
			_line = line.c_str();
			if (_line.CompareNoCase(usn)) {
				f >> line;
				continue;
			}
			else {
				f >> line;
				_line = line.c_str();
				if (!_line.CompareNoCase(psw)) {
					f.close();
					return true;
				}
			}
		}
	}
	f.close();
	return false;
}
int CServerDlg::checkQuery(CString command) {
	//format command
	/*for (int i = 0; i < command.GetLength(); i++) {
		
	}*/
	CString strResult[3];
	Split(command, strResult,_T(" "));
	if (strResult[1].CompareNoCase(_T(""))) {
		if (strResult[1].GetLength() != 6)
			return -1;
		for (int i = 0; i < strResult[1].GetLength(); i++) {
			if (!std::isdigit(strResult[1][i]))
				return -1;	//not number
		}
	}
	else {
		return -2;
	}
	if (strResult[2].CompareNoCase(_T("")))
		return -1; //thua parameter
	return 1;
}
int CServerDlg::searchQuery(CString day, CString query, CString &prize) {
	CString strResult[3];
	Split(query, strResult,_T(" "));
	ifstream f;
	f.open("../Source/Server/data/data.txt",ios::app);
	//f.open("../data/data.txt");
	if (f.fail()) {
		MessageBox(_T("Can not open file"));
		return -1;
	}
	else {
		string line;
		CString _line;
		while (f >> line)
		{
			_line = line.c_str();
			if (_line.CompareNoCase(day)) {
				f >> line;
			}
			else {
				bool stop = false;
				while (f >> line) {
					if (stop)break;
					_line = line.c_str();
					if (_line.MakeUpper().CompareNoCase(strResult[0])) {
						continue;
					}
					else {
						stop = true;
						string tmp;
						while (f >> tmp) {
							_line = tmp.c_str();
							if (!_line.CompareNoCase(_T("+")))
								break;
							int prInt = _ttoi(_line); //giải
							int len = 5;//số ký tự của giải
							switch (prInt)
							{
							case 1: len = 6;
								break;
							case 6:
								len = 4;
								break;
							case 7:len = 3;
								break;
							case 8:
								len = 2;
								break;
							default: len = 5;
								break;
							}
							while (f >> line) {
								_line = line.c_str();
								if (!_line.CompareNoCase(_T("-")))
									break;
								if (strResult[1].Find(_line, 6 - len) > -1) {
									prize = tmp.c_str();
									return 1;
								}
							}
						}
						return 0;
					}
				}
				return -2;
			}
		}
	}
	return -1;
}
int CServerDlg::searchSingleDay(CString query,CString &result) {
	ifstream f;
	f.open("../Source/Server/data/data.txt",ios::app);
	//f.open("../data/data.txt");
	if (f.fail()) {
		MessageBox(_T("Can not open file"));
		return 0;
	}
	string line;
	CString _line;
	bool stop = false;
	while (f >> line) {
		if (stop)break;
		_line = line.c_str();
		if (!_line.CompareNoCase(_T("26/06"))) {
			stop = true;
			bool stopCity = false;
			while (f >> line) {
				if (stopCity)break;
				_line = line.c_str();
				if (!_line.MakeUpper().CompareNoCase(query)) {
					stopCity = true;
					while (f >> line) {
						_line = line.c_str();
						if (!_line.CompareNoCase(_T("+")))
							break;
						if (!_line.CompareNoCase(_T("-")))
							continue;
						result += _T("\n") + _line;
					}
					if (result.CompareNoCase(_T("")))
						return 1;
				}
			}
			return 0;
		}
	}
	return 0;
}
void CServerDlg::OnBnClickedListen()
{
	// TODO: Add your control notification handler code here

	UpdateData();
	sockServer =socket(AF_INET,SOCK_STREAM,0);
	serverAdd.sin_family=AF_INET;
	serverAdd.sin_port=htons(PORT);
	serverAdd.sin_addr.s_addr =htonl(INADDR_ANY);
	bind(sockServer,(SOCKADDR*)&serverAdd,sizeof (serverAdd));
	listen(sockServer,5);
	int err =WSAAsyncSelect(sockServer,m_hWnd,WM_SOCKET,FD_READ|FD_ACCEPT|FD_CLOSE);
	if (err)
		MessageBox((LPCTSTR)"Cant call WSAAsyncSelect");	
	GetDlgItem(IDC_LISTEN)->EnableWindow(FALSE);
	number_Socket=0;
	pSock=new SockName[200];

	srand((unsigned)time(NULL));
	R = rand(); 
}

void CServerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

LRESULT CServerDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
	{
		pSock[number_Socket].sockClient = accept(wParam, NULL, NULL);
		GetDlgItem(IDC_LISTEN)->EnableWindow(FALSE);
		break;
	}
	case FD_READ:
	{

		int post = -1, dpos = -1;

		for (int i = 0; i < number_Socket; i++)
		{
			if (pSock[i].sockClient == wParam)
			{
				if (i < number_Socket)
					post = i;
			}
		}

		CString temp;
		if (mRecv(wParam, temp) < 0)
			break;
		Split(temp, strResult, _T("\r\n"));
		int flag = _ttoi(strResult[0]);
		char* tem = ConvertToChar(strResult[1]);
		switch (flag)
		{
		case 1://Login
		{
			int t = 0;
			if (number_Socket > 0)
			{
				for (int i = 0; i < number_Socket; i++)
				{
					if (strcmp(tem, pSock[i].Name) == 0)//Trung ten user
					{
						t = 1;
						break;
					}
				}
			}
			if (!checkLogin(strResult[1], strResult[2]))
				t = 1;
			if (t == 0)
			{
				strcpy(pSock[number_Socket].Name, tem);
				Command = _T("1\r\n1\r\n");
				m_msgString += strResult[1] + _T(" login\r\n");
				UpdateData(FALSE);
				number_Socket++;
			}
			else
				Command = _T("1\r\n0\r\n");
			mSend(wParam, Command);
			UpdateData(FALSE);
			break;
		}

		case 2://response query
		{
			int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
				}
			}
			if (!strResult[2].MakeUpper().CompareNoCase(_T("H")))
				Command = _T("3\r\n");
			else {
				int check = checkQuery(strResult[2] + _T(" "));
				if (check == -1)
					Command = _T("2\r\n5\r\n");
				else if (check == -2) {
					CString result=_T("");
					int searchSingle= searchSingleDay(strResult[2], result);
					if(searchSingle==0)
						Command= _T("2\r\n6\r\n");
					else {
						Command = _T("2\r\n7\r\n")+result+_T("\r\n");
					}
				}
				else {
					CString prize = _T("");
					int result = searchQuery(strResult[1], strResult[2] + _T(" "), prize);
					if (result == -1)
						Command = _T("2\r\n3\r\n");
					else if (result == -2)
						Command = _T("2\r\n2\r\n");
					else if (result == 0)
						Command = _T("2\r\n4\r\n");
					else
						Command = _T("2\r\n5\r\n") + prize + _T("\r\n");
				}
			}
			mSend(wParam, Command);
			UpdateData(FALSE);

			break;
		}
		case 3:
		{
			int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
				}
			}
			m_msgString += pSock[post].Name;
			m_msgString += " logout\r\n";
			closesocket(wParam);
			for (int j = post; j < number_Socket; j++)
			{
				pSock[post].sockClient = pSock[post + 1].sockClient;
				strcpy(pSock[post].Name, pSock[post + 1].Name);
			}
			number_Socket--;
			UpdateData(FALSE);
			break;
		}
		case 4: {
			int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
				}
			}
			strcpy(pSock[number_Socket].Name, tem);
			number_Socket++;
			Command = _T("4\r\n");
			if (checkUsername(strResult[1])) {
				ofstream outfile;
				outfile.open("../Source/Server/data/account.txt", ios::app);
				//outfile.open("../data/account.txt", ios::app);
				if (outfile.fail()) {
					MessageBox(_T("Can not open file"));
					UpdateData(FALSE);
					break;
				}
				string usn, ps;
				usn = CW2A(strResult[1].GetString());
				ps = CW2A(strResult[2].GetString());
				outfile << usn << "\n" << ps << "\n";
				outfile.close();
				Command += _T("1\r\n");
			}
			else
				Command += _T("2\r\n");
			mSend(wParam, Command);
			UpdateData(FALSE);
			break;
		}
		}
		break;
	}
	case FD_CLOSE:
	{
		UpdateData();
		int post = -1;
		for (int i = 0; i < number_Socket; i++)
		{
			if (pSock[i].sockClient == wParam)
			{
				if (i < number_Socket)
					post = i;
			}
		}
		m_msgString += pSock[post].Name;
		m_msgString += " logout\r\n";
		closesocket(wParam);
		for (int j = post; j < number_Socket; j++)
		{
			pSock[post].sockClient = pSock[post + 1].sockClient;
			strcpy(pSock[post].Name, pSock[post + 1].Name);
		}
		number_Socket--;
		UpdateData(FALSE);
		break;
	}
	return 0;
	}
}