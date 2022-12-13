#pragma once
#include "afxdialogex.h"


// CGetNumberInputDlg 对话框

class CGetNumberInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGetNumberInputDlg)

public:
	CGetNumberInputDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGetNumberInputDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GETINPUT_DIALOG_NUMBER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual INT_PTR DoModal();
	INT_PTR StartDialog(size_t* Number, CString Tip = L"");
	INT_PTR StartDialog(CString* Buffer, CString Tip = L"");
private:
	CEdit m_Edit;
	CStatic m_Static;
	CFont m_Font;
	bool ReturnNumber;
	size_t* m_Number;
	CString* m_Buffer;
public:
	CString m_Tip;
};
