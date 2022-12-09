// CGetInputDlg.cpp: 实现文件
//

#include "pch.h"
#include "DatabaseEditor.h"
#include "afxdialogex.h"
#include "GetInputDlg.h"


// CGetInputDlg 对话框

IMPLEMENT_DYNAMIC(CGetInputDlg, CDialogEx)

CGetInputDlg::CGetInputDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GETINPUT_DIALOG, pParent)
{
	m_Buffer = nullptr;
	m_Tip = _T("");
}

CGetInputDlg::~CGetInputDlg()
{
}

void CGetInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GETINPUT_AUTO_EDIT, m_Edit);
	DDX_Control(pDX, IDC_STATIC_GET_INPUT, m_Static);
}


BEGIN_MESSAGE_MAP(CGetInputDlg, CDialogEx)
END_MESSAGE_MAP()


// CGetInputDlg 消息处理程序


BOOL CGetInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Font.CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");

	m_Edit.SetFont(&m_Font);

	m_Static.SetWindowText(m_Tip);

	// TODO:  在此添加额外的初始化
	if (!m_Buffer)
		OnCancel();

	m_Buffer->Empty();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


INT_PTR CGetInputDlg::DoModal()
{
	return IDCANCEL;
}


INT_PTR CGetInputDlg::StartDialog(CString* Buffer, CString Tip)
{
	m_Buffer = Buffer;
	m_Tip = Tip;
	return CDialogEx::DoModal();
}


void CGetInputDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString LocalBuffer;
	m_Edit.GetWindowText(LocalBuffer);
	if (LocalBuffer != L"")
	{
		*m_Buffer = LocalBuffer;
		CDialogEx::OnOK();
		return;
	}
	CDialogEx::OnCancel();
}
