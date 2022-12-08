
// DatabaseEditorDlg.h: 头文件
//

#pragma once

#include "GetInputDlg.h"
#include "GetNumberInputDlg.h"
#include "DatabaseDescriptionEditor.h"
#include "TableDescriptionEditor.h"
#include "MyConvert.h"

#include <DatabaseControler.h>
#include <iostream>
#include <sstream>


// CDatabaseEditorDlg 对话框
class CDatabaseEditorDlg : public CDialogEx
{
	// 构造
public:
	CDatabaseEditorDlg(CWnd* pParent = nullptr);	// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATABASEEDITOR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAbout();
	afx_msg void OnExit();
	DatabaseHandle Database;
	void SetAppIcon();
	CListCtrl m_ListCtrl;
	afx_msg void OnAddFd();
	afx_msg void OnAddTbl();
	afx_msg void OnCloseDb();
	afx_msg void OnDbDescription();
	afx_msg void OnDelFd();
	afx_msg void OnDelTbl();
	afx_msg void OnNewDb();
	afx_msg void OnOpenDb();
	afx_msg void OnRenameTbl();
	afx_msg void OnRenemeFd();
	afx_msg void OnSaveDb();
	afx_msg void OnSaveAs();
	afx_msg void OnBnClickedAddFdButton();
	afx_msg void OnBnClickedDelFdButton();
	afx_msg void OnBnClickedAddTblButton();
	afx_msg void OnBnClickedDelTblButton();
	afx_msg void OnCbnSelchangeTblCombo();
	CComboBox m_ComboTables;
	void FlushFields();
	void FlushTables();
	afx_msg void OnBnClickedDelSelTblButton();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	bool m_CtrlDown;
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnTblDescription();
	afx_msg void OnDelRow();
	afx_msg void OnDelMultiRow();
	afx_msg void OnBnClickedDelRowButton();
	afx_msg void OnBnClickedDelMultiRowButton();
	afx_msg void OnAddRow();
	afx_msg void OnAddMultiRow();
	afx_msg void OnBnClickedAddRowButton();
	afx_msg void OnBnClickedAddMultiRowButton();
	afx_msg void OnRenameThisTbl();
};
