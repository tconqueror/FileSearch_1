
// FileSearch_1Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "FileSearch_1.h"
#include "FileSearch_1Dlg.h"
#include "afxdialogex.h"
#include <string>
#include <filesystem>
//#include <stack>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
private:
	
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


// CFileSearch1Dlg dialog



CFileSearch1Dlg::CFileSearch1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILESEARCH_1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileSearch1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, result);
	DDX_Control(pDX, IDC_EDIT1, dir);
	DDX_Control(pDX, IDC_EDIT2, file);
}

BEGIN_MESSAGE_MAP(CFileSearch1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CFileSearch1Dlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_searchBtn, &CFileSearch1Dlg::OnBnClickedsearchbtn)
END_MESSAGE_MAP()


// CFileSearch1Dlg message handlers

BOOL CFileSearch1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	result.InsertColumn(0, L"TimeCreate", LVCFMT_LEFT, 80);
	result.InsertColumn(0, L"LastTimeChange", LVCFMT_LEFT, 100);
	result.InsertColumn(0, L"Location", LVCFMT_LEFT, 170);
	result.InsertColumn(0, L"Size", LVCFMT_LEFT, 80);
	result.InsertColumn(0, L"Name", LVCFMT_LEFT, 80);
	result.InsertColumn(0, L"No", LVCFMT_LEFT, 30);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileSearch1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFileSearch1Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFileSearch1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileSearch1Dlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CFileSearch1Dlg::OnBnClickedsearchbtn()
{
	if (dir.GetWindowTextLengthW() == 0)
	{	
		AfxMessageBox(L"Enter somthing please");
		return;
	}
	CString Cdir;
	dir.GetWindowText(Cdir);
	if (!PathFileExists(Cdir))
	{
		AfxMessageBox(L"The directory does not exist!");
		return;
	}
	CString Cfile;
	file.GetWindowText(Cfile);
	std::stack<ff> st;
	Cfile.MakeLower();
	CFileSearch1Dlg::Find(Cdir, st, Cfile);
	syncWithStack(st);
}

CString CFileSearch1Dlg::intToCString(DWORD64 n)
{
	CString temp;
	temp.Format(_T("%lld"), n);
	return temp;
}

CString CFileSearch1Dlg::fileTimeToCString(FILETIME &a)
{
	SYSTEMTIME st;
	FileTimeToSystemTime(&a, &st);
	CString res = L"";
	res = res + intToCString(st.wHour) + L":";
	res = res + intToCString(st.wMinute) + L":";
	res = res + intToCString(st.wSecond) + L" ";
	res = res + intToCString(st.wDay) + L"/";
	res = res + intToCString(st.wMonth) + L"/";
	res = res + intToCString(st.wYear);
	return res;
}

void CFileSearch1Dlg::Find(CString dir, std::stack<ff> &st, CString file)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(dir + L"\\*", &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox(L"Wrong path!");
		//AfxMessageBox(intToCString(st.size()));
		//AfxMessageBox(dir + L"\\*");
		return;
	}
	do
	{
		if (ffd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
			CString tmp = ffd.cFileName;
			tmp.MakeLower();
			if (tmp.Find(file) == -1)
				continue;
			ff temp;
			temp.a = st.size() + 1;
			temp.b = ffd.cFileName;
			DWORD64 x,y;
			x = ffd.nFileSizeLow;
			y = ffd.nFileSizeHigh;
			y = (y << 32);
			x = x + y;
			temp.c = intToCString(x / 1024) + L" KB";
			temp.d = dir + L"\\" + ffd.cFileName;
			temp.e = fileTimeToCString(ffd.ftLastWriteTime);
			temp.f = fileTimeToCString(ffd.ftCreationTime);
			st.push(temp);
		}
		else
		{
			if ((ffd.cFileName[0] == 46) &  ((CString(ffd.cFileName).GetLength())==1))
				continue;
			if ((ffd.cFileName[0] == 46) & (ffd.cFileName[1] == 46) & ((CString(ffd.cFileName).GetLength()) == 2))
				continue;
			CString s = dir + L"\\";
			//s.Delete(s.GetLength() - 1);
			s = s + ffd.cFileName;
			CFileSearch1Dlg::Find(s, st, file);
		}
	} while ((FindNextFile(hFind, &ffd)) != 0);
	FindClose(hFind);
}

void CFileSearch1Dlg::Push(ff a)
{
	int n = result.InsertItem(0, intToCString(a.a));
	result.SetItemText(n, 1, a.b);
	result.SetItemText(n, 2, a.c);
	result.SetItemText(n, 3, a.d);
	result.SetItemText(n, 4, a.e);
	result.SetItemText(n, 5, a.f);
}

void CFileSearch1Dlg::syncWithStack(std::stack<ff> a)
{
	result.DeleteAllItems();
	while (a.size())
	{
		ff temp = a.top();
		a.pop();
		Push(temp);
	}
}
