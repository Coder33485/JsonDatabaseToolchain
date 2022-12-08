#pragma once
#include "afxdialogex.h"
#include "MyConvert.h"

#include <DatabaseControler.h>


// CDatabaseDescriptionEditor 对话框

class CDatabaseDescriptionEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CDatabaseDescriptionEditor)

public:
	CDatabaseDescriptionEditor(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDatabaseDescriptionEditor();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DB_DESCRIPTION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditAuthor;
	CEdit m_EditBlock;
	CEdit m_EditName;
	CEdit m_EditToolchain;
	CEdit m_EditVersion;
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal();
	INT_PTR StartDialog(DatabaseHandle* Handle);
private:
	DatabaseHandle* DB_Handle;
};
