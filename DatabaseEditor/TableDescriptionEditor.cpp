// TableDescriptionEditor.cpp: 实现文件
//

#include "pch.h"
#include "DatabaseEditor.h"
#include "afxdialogex.h"
#include "TableDescriptionEditor.h"


// CTableDescriptionEditor 对话框

IMPLEMENT_DYNAMIC(CTableDescriptionEditor, CDialogEx)

CTableDescriptionEditor::CTableDescriptionEditor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TBL_DESCRIPTION_DIALOG, pParent)
{
	DB_Handle = nullptr;
}

CTableDescriptionEditor::~CTableDescriptionEditor()
{
}

void CTableDescriptionEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DSP_TBL_BLOCK_EDIT, m_EditBlock);
}


BEGIN_MESSAGE_MAP(CTableDescriptionEditor, CDialogEx)
END_MESSAGE_MAP()


// CTableDescriptionEditor 消息处理程序


BOOL CTableDescriptionEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (!DB_Handle || !DB_Handle->Selected)
		OnCancel();

	std::wstring w_str;
	UTF8ToGBK(DB_Handle->TBL_Description.Message, w_str);
	m_EditBlock.SetWindowText(w_str.c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CTableDescriptionEditor::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString Temp;
	m_EditBlock.GetWindowText(Temp);
	DB_Handle->TBL_Description.Message = CW2A(Temp, CP_UTF8);
	SaveTableDescription(*DB_Handle);
	CDialogEx::OnOK();
}


INT_PTR CTableDescriptionEditor::DoModal()
{
	return IDCANCEL;
}


INT_PTR CTableDescriptionEditor::StartDialog(DatabaseHandle* Handle)
{
	DB_Handle = Handle;
	return CDialogEx::DoModal();
}
