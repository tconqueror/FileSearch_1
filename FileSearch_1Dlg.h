
// FileSearch_1Dlg.h : header file
//
#include <stack>
#pragma once
typedef struct ff
{
	int a;
	CString b, c, d, e, f;
};

// CFileSearch1Dlg dialog
class CFileSearch1Dlg : public CDialogEx
{
// Construction
public:
	CFileSearch1Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILESEARCH_1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
private:
	CListCtrl result;
public:
	afx_msg void OnBnClickedsearchbtn();
	afx_msg void OnBnClickedOk();
private:
	CEdit dir;
	CEdit file;
	CString Cdir;
	CString Cfile;
	CString intToCString(DWORD64 n);
	CString fileTimeToCString(FILETIME &a);
	void Find(CString dir, CString file);
	void Push(ff a);
	void syncWithStack(std::stack<ff> a);
	static UINT __cdecl StaticThreadFunc(LPVOID);
	UINT ThreadFunc();
	int count = 0;
	CWinThread* cw;
	bool status = false;
};
