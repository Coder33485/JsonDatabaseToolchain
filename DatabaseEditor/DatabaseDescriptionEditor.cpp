// DatabaseDescriptionEditor.cpp: 实现文件
//

#include "pch.h"
#include "DatabaseEditor.h"
#include "afxdialogex.h"
#include "DatabaseDescriptionEditor.h"


// CDatabaseDescriptionEditor 对话框

IMPLEMENT_DYNAMIC(CDatabaseDescriptionEditor, CDialogEx)

CDatabaseDescriptionEditor::CDatabaseDescriptionEditor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DB_DESCRIPTION_DIALOG, pParent)
{
	DB_Handle = nullptr;
}

CDatabaseDescriptionEditor::~CDatabaseDescriptionEditor()
{
}

void CDatabaseDescriptionEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DSP_DB_AUTHOR_EDIT, m_EditAuthor);
	DDX_Control(pDX, IDC_DSP_DB_BLOCK_EDIT, m_EditBlock);
	DDX_Control(pDX, IDC_DSP_DB_NAME_EDIT, m_EditName);
	DDX_Control(pDX, IDC_DSP_DB_TOOLCHAIN_EDIT, m_EditToolchain);
	DDX_Control(pDX, IDC_DSP_DB_VERSION_EDIT, m_EditVersion);
}


BEGIN_MESSAGE_MAP(CDatabaseDescriptionEditor, CDialogEx)
END_MESSAGE_MAP()


// CDatabaseDescriptionEditor 消息处理程序


void CDatabaseDescriptionEditor::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString Temp;
	m_EditName.GetWindowText(Temp);
	DB_Handle->DB_Description.Name = CW2A(Temp, CP_UTF8);
	m_EditAuthor.GetWindowText(Temp);
	DB_Handle->DB_Description.Author = CW2A(Temp, CP_UTF8);
	m_EditBlock.GetWindowText(Temp);
	DB_Handle->DB_Description.Message = CW2A(Temp, CP_UTF8);
	SaveDatabaseDescription(*DB_Handle);

	CDialogEx::OnOK();
}


BOOL CDatabaseDescriptionEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!DB_Handle)
		OnCancel();

	// TODO:  在此添加额外的初始化
	std::wstring w_str;
	StringToWstring(DB_Handle->DB_Description.Name, w_str);
	m_EditName.SetWindowText(w_str.c_str());
	StringToWstring(DB_Handle->DB_Description.Author, w_str);
	m_EditAuthor.SetWindowText(w_str.c_str());
	StringToWstring(DB_Handle->DB_Description.Version, w_str);
	m_EditVersion.SetWindowText(w_str.c_str());
	StringToWstring(DB_Handle->DB_Description.Toolchain, w_str);
	m_EditToolchain.SetWindowText(w_str.c_str());
	StringToWstring(DB_Handle->DB_Description.Message, w_str);
	m_EditBlock.SetWindowText(w_str.c_str());


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


INT_PTR CDatabaseDescriptionEditor::DoModal()
{
	return IDCANCEL;
}


INT_PTR CDatabaseDescriptionEditor::StartDialog(DatabaseHandle* Handle)
{
	// TODO: 在此处添加实现代码.
	DB_Handle = Handle;
	return CDialogEx::DoModal();
}
