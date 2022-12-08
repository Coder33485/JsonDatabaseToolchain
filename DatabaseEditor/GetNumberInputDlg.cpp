// GetNumberInputDlg.cpp: 实现文件
//

#include "pch.h"
#include "DatabaseEditor.h"
#include "afxdialogex.h"
#include "GetNumberInputDlg.h"

#include <sstream>


// CGetNumberInputDlg 对话框

IMPLEMENT_DYNAMIC(CGetNumberInputDlg, CDialogEx)

CGetNumberInputDlg::CGetNumberInputDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GETINPUT_DIALOG_NUMBER, pParent)
{
	m_Number = nullptr;
	m_Tip = _T("");
}

CGetNumberInputDlg::~CGetNumberInputDlg()
{
}

void CGetNumberInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GETINPUT_NUMBER_EDIT, m_Edit);
	DDX_Control(pDX, IDC_STATIC_GET_INPUT, m_Static);
}


BEGIN_MESSAGE_MAP(CGetNumberInputDlg, CDialogEx)
END_MESSAGE_MAP()


// CGetNumberInputDlg 消息处理程序


BOOL CGetNumberInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDialogEx::OnInitDialog();

	m_Font.CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");

	m_Edit.SetFont(&m_Font);

	m_Static.SetWindowText(m_Tip);

	// TODO:  在此添加额外的初始化
	if (!m_Number)
		OnCancel();

	*m_Number = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CGetNumberInputDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString LocalBuffer;
	m_Edit.GetWindowText(LocalBuffer);
	std::wstringstream wss;
	wss.clear();
	wss << LocalBuffer.GetString();
	wss >> *m_Number;
	CDialogEx::OnOK();
}


INT_PTR CGetNumberInputDlg::DoModal()
{
	return IDCANCEL;
}


INT_PTR CGetNumberInputDlg::StartDialog(size_t* Number, CString Tip)
{
	m_Number = Number;
	m_Tip = Tip;
	return CDialogEx::DoModal();
}
