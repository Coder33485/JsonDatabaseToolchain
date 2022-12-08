#pragma once
#include "afxdialogex.h"


// CGetInputDlg 对话框

class CGetInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGetInputDlg)

public:
	CGetInputDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGetInputDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GETINPUT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal();
	INT_PTR StartDialog(CString* Buffer, CString Tip = L"");
	virtual void OnOK();
private:
	CEdit m_Edit;
	CString m_Tip;
	CString* m_Buffer = nullptr;
	CFont m_Font;
	CStatic m_Static;
};
