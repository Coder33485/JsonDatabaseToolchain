#pragma once
#include "afxdialogex.h"

#include <DatabaseController.h>
#include <StringProcessor.h>


// CTableDescriptionEditor 对话框

class CTableDescriptionEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CTableDescriptionEditor)

public:
	CTableDescriptionEditor(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTableDescriptionEditor();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TBL_DESCRIPTION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditBlock;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual INT_PTR DoModal();
	INT_PTR StartDialog(DatabaseHandle* Handle);
private:
	DatabaseHandle* DB_Handle;
};
