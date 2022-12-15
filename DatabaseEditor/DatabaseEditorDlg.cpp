
// DatabaseEditorDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DatabaseEditor.h"
#include "DatabaseEditorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int GetColumnCount(CListCtrl* m_ListCtrl)
{
	CHeaderCtrl* pHeaderCtrl = m_ListCtrl->GetHeaderCtrl();
	return(pHeaderCtrl->GetItemCount());
}

void AdjustColumnWidth(CListCtrl* m_ListCtrl)
{
	m_ListCtrl->SetRedraw(FALSE);
	int nColumnCount = GetColumnCount(m_ListCtrl);
	for (int i = 0; i < nColumnCount; i++)
	{
		m_ListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE);
		int nColumnWidth = m_ListCtrl->GetColumnWidth(i);
		m_ListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
		int nHeaderWidth = m_ListCtrl->GetColumnWidth(i);

		m_ListCtrl->SetColumnWidth(i, max(nColumnWidth, nHeaderWidth));
	}
	m_ListCtrl->SetRedraw(TRUE);
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDatabaseEditorDlg 对话框



CDatabaseEditorDlg::CDatabaseEditorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DATABASEEDITOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ExFunction = false;
}

void CDatabaseEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_TBL_COMBO, m_ComboTables);
	DDX_Control(pDX, IDC_EXFUNC_WAIT_STATIC, m_ExFuncStatic);
}

BEGIN_MESSAGE_MAP(CDatabaseEditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_ABOUT, &CDatabaseEditorDlg::OnAbout)
	ON_COMMAND(ID_EXIT, &CDatabaseEditorDlg::OnExit)
	ON_COMMAND(ID_ADD_FD, &CDatabaseEditorDlg::OnAddFd)
	ON_COMMAND(ID_ADD_TBL, &CDatabaseEditorDlg::OnAddTbl)
	ON_COMMAND(ID_CLOSE_DB, &CDatabaseEditorDlg::OnCloseDb)
	ON_COMMAND(ID_DB_DESCRIPTION, &CDatabaseEditorDlg::OnDbDescription)
	ON_COMMAND(ID_DEL_FD, &CDatabaseEditorDlg::OnDelFd)
	ON_COMMAND(ID_DEL_TBL, &CDatabaseEditorDlg::OnDelTbl)
	ON_COMMAND(ID_NEW_DB, &CDatabaseEditorDlg::OnNewDb)
	ON_COMMAND(ID_OPEN_DB, &CDatabaseEditorDlg::OnOpenDb)
	ON_COMMAND(ID_RENAME_TBL, &CDatabaseEditorDlg::OnRenameTbl)
	ON_COMMAND(ID_RENEME_FD, &CDatabaseEditorDlg::OnRenemeFd)
	ON_COMMAND(ID_SAVE_DB, &CDatabaseEditorDlg::OnSaveDb)
	ON_COMMAND(ID_SAVE_AS, &CDatabaseEditorDlg::OnSaveAs)
	ON_BN_CLICKED(IDC_ADD_FD_BUTTON, &CDatabaseEditorDlg::OnBnClickedAddFdButton)
	ON_BN_CLICKED(IDC_DEL_FD_BUTTON, &CDatabaseEditorDlg::OnBnClickedDelFdButton)
	ON_BN_CLICKED(IDC_ADD_TBL_BUTTON, &CDatabaseEditorDlg::OnBnClickedAddTblButton)
	ON_BN_CLICKED(IDC_DEL_TBL_BUTTON, &CDatabaseEditorDlg::OnBnClickedDelTblButton)
	ON_CBN_SELCHANGE(IDC_TBL_COMBO, &CDatabaseEditorDlg::OnCbnSelchangeTblCombo)
	ON_BN_CLICKED(IDC_DEL_SEL_TBL_BUTTON, &CDatabaseEditorDlg::OnBnClickedDelSelTblButton)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_TBL_DESCRIPTION, &CDatabaseEditorDlg::OnTblDescription)
	ON_COMMAND(ID_DEL_ROW, &CDatabaseEditorDlg::OnDelRow)
	ON_COMMAND(ID_DEL_MULTI_ROW, &CDatabaseEditorDlg::OnDelMultiRow)
	ON_BN_CLICKED(IDC_DEL_ROW_BUTTON, &CDatabaseEditorDlg::OnBnClickedDelRowButton)
	ON_BN_CLICKED(IDC_DEL_MULTI_ROW_BUTTON, &CDatabaseEditorDlg::OnBnClickedDelMultiRowButton)
	ON_COMMAND(ID_ADD_ROW, &CDatabaseEditorDlg::OnAddRow)
	ON_COMMAND(ID_ADD_MULTI_ROW, &CDatabaseEditorDlg::OnAddMultiRow)
	ON_BN_CLICKED(IDC_ADD_ROW_BUTTON, &CDatabaseEditorDlg::OnBnClickedAddRowButton)
	ON_BN_CLICKED(IDC_ADD_MULTI_ROW_BUTTON, &CDatabaseEditorDlg::OnBnClickedAddMultiRowButton)
	ON_COMMAND(ID_RENAME_THIS_TBL, &CDatabaseEditorDlg::OnRenameThisTbl)
END_MESSAGE_MAP()


// CDatabaseEditorDlg 消息处理程序

BOOL CDatabaseEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetAppIcon();

	AdjustColumnWidth(&m_ListCtrl);

	// TODO: 在此添加额外的初始化代码
	InitDatabaseHandle(m_Database);

	m_ExFuncStatic.ShowWindow(SW_HIDE);

	ModuleProcessor();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDatabaseEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
		CDialogEx::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDatabaseEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDatabaseEditorDlg::OnAbout()
{
	CAboutDlg Dlg;
	Dlg.DoModal();
}


void CDatabaseEditorDlg::OnExit()
{
	CDialogEx::OnOK();
}


void CDatabaseEditorDlg::SetAppIcon()
{
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
}


void CDatabaseEditorDlg::OnAddFd()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (!m_Database.Selected)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	std::string fd_name;
	CString FdName, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_NEW_FD_NAME);
	if (Dlg.StartDialog(&FdName, Tip) == IDCANCEL)
		return;
	fd_name = CW2A(FdName, CP_UTF8);
	CreateField(m_Database, fd_name);
	FlushFields();
}


void CDatabaseEditorDlg::OnAddTbl()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	CGetInputDlg Dlg;
	CString TblName, Tip;
	std::string tbl_name;
	Tip.LoadString(IDS_TBL_NAME);
	if (Dlg.StartDialog(&TblName, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	tbl_name = CW2A(TblName, CP_UTF8);
	CreateTable(m_Database, tbl_name);
	FlushTables();
}


void CDatabaseEditorDlg::OnCloseDb()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	InitDatabaseHandle(m_Database);
	FlushTables();
	FlushFields();
}


void CDatabaseEditorDlg::OnDbDescription()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	CDatabaseDescriptionEditor Dlg;
	Dlg.StartDialog(&m_Database);
}


void CDatabaseEditorDlg::OnDelFd()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (!m_Database.Selected)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	std::string fd_name;
	CString FdName, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_FD_NAME);
	if (Dlg.StartDialog(&FdName, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	fd_name = CW2A(FdName, CP_UTF8);
	DeleteField(m_Database, fd_name);
	FlushFields();
}


void CDatabaseEditorDlg::OnDelTbl()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	CGetInputDlg Dlg;
	CString TblName, Tip;
	std::string tbl_name;
	Tip.LoadString(IDS_TBL_NAME);
	if (Dlg.StartDialog(&TblName, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	tbl_name = CW2A(TblName, CP_UTF8);
	if (!DeleteTable(m_Database, tbl_name))
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	FlushTables();
	FlushFields();
}


void CDatabaseEditorDlg::OnNewDb()
{
	if (m_Database.Loaded)
		return;
	std::string path, name, author;
	CString Path, Name, Author, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_DB_PATH);
	if (Dlg.StartDialog(&Path, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	path = CW2A(Path, CP_UTF8);
	Tip.LoadString(IDS_DB_NAME);
	if (Dlg.StartDialog(&Name, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	name = CW2A(Name, CP_UTF8);
	Tip.LoadString(IDS_DB_AUTHOR);
	if (Dlg.StartDialog(&Author, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	author = CW2A(Author, CP_UTF8);
	CreateDatabase(m_Database, path, name, author);
	// TODO: 在此添加命令处理程序代码
}


void CDatabaseEditorDlg::OnOpenDb()
{
	if (m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	std::string path;
	CString Path, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_DB_PATH);
	if (Dlg.StartDialog(&Path, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	path = CW2A(Path, CP_UTF8);
	if (!LoadDatabase(m_Database, path))
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	FlushTables();
}


void CDatabaseEditorDlg::OnRenameTbl()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	std::string old_table_name, new_table_name;
	CString OldTblName, NewTblName, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_TBL_NAME);
	if (Dlg.StartDialog(&OldTblName, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	old_table_name = CW2A(OldTblName, CP_UTF8);
	Tip.LoadString(IDS_NEW_TBL_NAME);
	if (Dlg.StartDialog(&NewTblName, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	new_table_name = CW2A(NewTblName, CP_UTF8);
	if (!RenameTable(m_Database, old_table_name, new_table_name))
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	FlushTables();
}


void CDatabaseEditorDlg::OnRenemeFd()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (!m_Database.Selected)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	std::string old_field_name, new_field_name;
	CString OldFdName, NewFdName, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_FD_NAME);
	if (Dlg.StartDialog(&OldFdName, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	old_field_name = CW2A(OldFdName, CP_UTF8);
	Tip.LoadString(IDS_NEW_FD_NAME);
	if (Dlg.StartDialog(&NewFdName, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	new_field_name = CW2A(NewFdName, CP_UTF8);
	if (!RenameField(m_Database, old_field_name, new_field_name))
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	FlushFields();
}


void CDatabaseEditorDlg::OnSaveDb()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (!SaveDatabase(m_Database))
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
}


void CDatabaseEditorDlg::OnSaveAs()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	std::string path;
	CString Path, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_DB_PATH);
	if (Dlg.StartDialog(&Path, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	path = CW2A(Path, CP_UTF8);
	if (!SaveDatabaseTo(m_Database, path))
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
}


void CDatabaseEditorDlg::OnBnClickedAddFdButton()
{
	OnAddFd();
}


void CDatabaseEditorDlg::OnBnClickedDelFdButton()
{
	OnDelFd();
}


void CDatabaseEditorDlg::OnBnClickedAddTblButton()
{
	OnAddTbl();
}


void CDatabaseEditorDlg::OnBnClickedDelTblButton()
{
	OnDelTbl();
}


void CDatabaseEditorDlg::OnCbnSelchangeTblCombo()
{
	std::string table_name;
	CString TableName;
	m_ComboTables.GetWindowText(TableName);
	if (TableName == L"")
		return;
	table_name = CW2A(TableName, CP_UTF8);
	if (!SelectTable(m_Database, table_name))
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	FlushFields();
}


void CDatabaseEditorDlg::FlushFields()
{
	int  int_itemcount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();
	for (int nIndex = 0; nIndex < int_itemcount; ++nIndex)
		m_ListCtrl.DeleteColumn(0);
	m_ListCtrl.DeleteAllItems();
	if (!m_Database.Loaded)
		return;
	if (!m_Database.Selected)
		return;
	std::wstringstream wss;
	stringlist list, dat;
	GetFieldList(m_Database, list);
	std::wstring wstr;
	CString ID;
	ID.LoadString(IDS_ROW_NUMBER);
	m_ListCtrl.InsertColumn(0, ID);
	for (size_t i = 0; i < list.size(); i++)
	{
		StringToWstring(list[i], wstr);
		m_ListCtrl.InsertColumn(i + 1, wstr.c_str());
		GetFieldData(m_Database, list[i], dat);
		if (i == 0)
			for (size_t j = 0; j < dat.size(); j++)
			{
				wss.clear();
				wss << j;
				wss >> wstr;
				m_ListCtrl.InsertItem(j, wstr.c_str());
				StringToWstring(dat[j], wstr);
				m_ListCtrl.SetItemText(j, i + 1, wstr.c_str());
			}
		else
			for (size_t j = 0; j < dat.size(); j++)
			{
				StringToWstring(dat[j], wstr);
				m_ListCtrl.SetItemText(j, i + 1, wstr.c_str());
			}
	}
	AdjustColumnWidth(&m_ListCtrl);
}


void CDatabaseEditorDlg::FlushTables()
{
	m_ComboTables.ResetContent();
	if (!m_Database.Loaded)
		return;
	stringlist table_list;
	std::wstring w_str;
	if (!GetTableList(m_Database, table_list))
		return;
	for (size_t i = 0; i < table_list.size(); i++)
	{
		StringToWstring(table_list[i], w_str);
		m_ComboTables.AddString(w_str.c_str());
	}
	m_ComboTables.SetCurSel(0);
	OnCbnSelchangeTblCombo();
}


void CDatabaseEditorDlg::OnBnClickedDelSelTblButton()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (!m_Database.Selected)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (!DeleteTable(m_Database, m_Database.SelectedTable))
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	FlushTables();
	FlushFields();
}


void CDatabaseEditorDlg::OnOK()
{
	// Do nothing.
}


BOOL CDatabaseEditorDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		BOOL bCtrl = ::GetKeyState(VK_CONTROL) & 0x8000;
		if (m_ExFunction)
		{
			if (pMsg->wParam < 'A' || pMsg->wParam > 'Z')
				return CDialogEx::PreTranslateMessage(pMsg);
			m_ExFunction = false;
			m_ExFuncStatic.ShowWindow(SW_HIDE);
			switch (pMsg->wParam)
			{
			case 'S'://S
				if (bCtrl)
				{
					OnSaveAs();
					return TRUE;
				}
				break;
			}
			MessageBeep(MB_ICONHAND);
			return TRUE;
		}
		else
			switch (pMsg->wParam)
			{
			case 'S'://Ctrl + S
				if (bCtrl)
				{
					OnSaveDb();
					return TRUE;
				}
				break;
			case 'N'://Ctrl + N
				if (bCtrl)
				{
					OnNewDb();
					return TRUE;
				}
				break;
			case 'O'://Ctrl + O
				if (bCtrl)
				{
					OnOpenDb();
					return TRUE;
				}
				break;
			case 'D'://Ctrl + D
				if (bCtrl)
				{
					OnCloseDb();
					return TRUE;
				}
				break;
			case 'K':
				if (bCtrl)
				{
					m_ExFunction = true;
					m_ExFuncStatic.ShowWindow(SW_SHOWNORMAL);
					return TRUE;
				}
				break;
			}
	}
	else if (pMsg->message == WM_COMMAND)
	{
		if (pMsg->wParam < IDM_EXTERN_COMMAND)
			return CDialogEx::PreTranslateMessage(pMsg);
		stringlist ParamList;
		for (size_t i = 0; i < m_RecordList.size(); i++)
		{
			if (pMsg->wParam == m_RecordList[i].MenuCommand)
			{
				if (m_RecordList[i].Loading && GetParamList(m_RecordList[i].ParamTypeList, m_RecordList[i].TipList, ParamList))
					m_RecordList[i].MainFunc(m_Database, ParamList);
				else
					MessageBeep(MB_ICONHAND);
			}
			else if (pMsg->wParam == m_RecordList[i].MenuCommand + 1)
			{
				if (m_RecordList[i].Loading)
				{
					m_RecordList[i].FreeFunc();
					FreeLibrary(m_RecordList[i].hModule);
					m_RecordList[i].Loading = false;
				}
				else
					MessageBeep(MB_ICONHAND);
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDatabaseEditorDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString m_FilePath;
	::DragQueryFile(hDropInfo, 0, m_FilePath.GetBuffer(512), 512);
	//::DragFinish(hDropInfo);
	std::string path = CW2A(m_FilePath, CP_UTF8);
	if (m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	LoadDatabase(m_Database, path);
	FlushTables();
	CDialogEx::OnDropFiles(hDropInfo);
}


void CDatabaseEditorDlg::OnTblDescription()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (!m_Database.Selected)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	CTableDescriptionEditor Dlg;
	Dlg.StartDialog(&m_Database);
}


void CDatabaseEditorDlg::OnDelRow()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (!m_Database.Selected)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	size_t id = 0;
	CGetNumberInputDlg Dlg;
	CString Tip;
	Tip.LoadString(IDS_ROW_NUMBER);
	if (Dlg.StartDialog(&id, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (!DeleteLine(m_Database, id))
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	FlushFields();
}


void CDatabaseEditorDlg::OnDelMultiRow()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (!m_Database.Selected)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	size_t id = 0, num = 0;
	CGetNumberInputDlg Dlg;
	CString Tip;
	Tip.LoadString(IDS_BEGIN_ROW_NUMBER);
	if (Dlg.StartDialog(&id, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	Tip.LoadString(IDS_NUMBER_OF_ROW);
	if (Dlg.StartDialog(&num, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (!DeleteLine(m_Database, id, num))
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	FlushFields();
}


void CDatabaseEditorDlg::OnBnClickedDelRowButton()
{
	OnDelRow();
}


void CDatabaseEditorDlg::OnBnClickedDelMultiRowButton()
{
	OnDelMultiRow();
}


void CDatabaseEditorDlg::OnAddRow()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (!m_Database.Selected)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	stringlist list;
	GetFieldList(m_Database, list);
	CGetInputDlg Dlg;
	CString Unit, Tip;
	Tip.LoadString(IDS_EXIT_TIP);
	std::wstring w_str;
	stringlist unit_list;
	for (size_t i = 0; i < list.size(); i++)
	{
		StringToWstring(list[i], w_str);
		if (i == 0)
			Dlg.StartDialog(&Unit, (w_str + Tip.GetString()).c_str());
		else
			Dlg.StartDialog(&Unit, w_str.c_str());
		if (i == 0 && (Unit == L"/exit" || Unit == L"$exit" || Unit == L"\\exit"))
			break;
		unit_list.push_back(std::string(CW2A(Unit, CP_UTF8)));
	}
	if (!AppendLine(m_Database, unit_list))
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	FlushFields();
}


void CDatabaseEditorDlg::OnAddMultiRow()
{
	if (!m_Database.Loaded)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (!m_Database.Selected)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	stringlist list;
	GetFieldList(m_Database, list);
	CGetInputDlg Dlg;
	CString Unit, Tip;
	Tip.LoadString(IDS_EXIT_TIP);
	std::wstring w_str;
	stringlist unit_list;
	bool loop = true;
	while (loop)
	{
		unit_list.clear();
		for (size_t i = 0; i < list.size(); i++)
		{
			StringToWstring(list[i], w_str);
			if (i == 0)
				Dlg.StartDialog(&Unit, (w_str + Tip.GetString()).c_str());
			else
				Dlg.StartDialog(&Unit, w_str.c_str());
			if (i == 0 && (Unit == L"/exit" || Unit == L"$exit" || Unit == L"\\exit"))
			{
				loop = false;
				break;
			}
			unit_list.push_back(std::string(CW2A(Unit, CP_UTF8)));
		}
		if (loop)
			if (!AppendLine(m_Database, unit_list))
				MessageBeep(MB_ICONHAND);
	}
	FlushFields();
}


void CDatabaseEditorDlg::OnBnClickedAddRowButton()
{
	OnAddRow();
}


void CDatabaseEditorDlg::OnBnClickedAddMultiRowButton()
{
	OnAddMultiRow();
}


void CDatabaseEditorDlg::OnRenameThisTbl()
{
	if (!m_Database.Loaded || !m_Database.Selected)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	std::string new_table_name;
	CString NewTblName, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_NEW_TBL_NAME);
	if (Dlg.StartDialog(&NewTblName, Tip) == IDCANCEL)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	new_table_name = CW2A(NewTblName, CP_UTF8);
	if (!RenameTable(m_Database, m_Database.SelectedTable, new_table_name))
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	FlushTables();
}


void CDatabaseEditorDlg::ModuleProcessor()
{
	char* buffer = new char[256];
	memset(buffer, 0, 256);
	size_t len = GetModuleFileNameA(nullptr, buffer, 256);
	if (len > 256)
	{
		delete[] buffer;
		buffer = new char[len];
		memset(buffer, 0, len * sizeof(char));
		len = GetModuleFileNameA(nullptr, buffer, len);
	}
	for (size_t i = len; i != -1; i--)
	{
		if (buffer[i] == '\\')
		{
			buffer[i] = 0;
			break;
		}
	}
	std::string path = buffer;
	std::vector<std::string> Modules;
	delete[] buffer;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATAA pNextInfo;
	hFile = FindFirstFileA((path + "\\Module*.dll").c_str(), &pNextInfo);
	if (INVALID_HANDLE_VALUE == hFile)
		return;
	char infPath[MAX_PATH] = { 0 };
	if (pNextInfo.cFileName[0] != '.')
		Modules.push_back(pNextInfo.cFileName);
	while (FindNextFileA(hFile, &pNextInfo))
	{
		if (pNextInfo.cFileName[0] == '.')
			continue;
		Modules.push_back(pNextInfo.cFileName);
	}
	HMODULE hLibrary = nullptr;
	ModuleInitFunc InitFunc = nullptr;
	ModuleMain MainFunc = nullptr;
	ModuleFree FreeFunc = nullptr;
	std::string MenuString;
	std::wstring w_MenuString;
	stringlist ParamTypeList, TipList;
	m_RecordList.clear();
	ModuleRecord RecTemp;
	unsigned int LoadedModule = 0;
	CMenu* pMenu = nullptr, * pSubMenu = nullptr;
	pMenu = this->GetMenu();
	pSubMenu = pMenu->GetSubMenu(2);
	for (size_t i = 0; i < Modules.size(); i++)
	{
		hLibrary = LoadLibraryA(Modules[i].c_str());
		if (hLibrary)
		{
			InitFunc = (ModuleInitFunc)GetProcAddress(hLibrary, "ModuleInit");
			MainFunc = (ModuleMain)GetProcAddress(hLibrary, "ModuleMain");
			FreeFunc = (ModuleFree)GetProcAddress(hLibrary, "ModuleFree");
			if (InitFunc && MainFunc && FreeFunc)
			{
				ParamTypeList.clear();
				TipList.clear();
				if (InitFunc(MenuString, ParamTypeList, TipList))
				{
					StringToWstring(MenuString, w_MenuString);
					RecTemp.Loading = true;
					RecTemp.hModule = hLibrary;
					RecTemp.MenuCommand = IDM_EXTERN_COMMAND + LoadedModule;
					RecTemp.MainFunc = MainFunc;
					RecTemp.FreeFunc = FreeFunc;
					RecTemp.ParamTypeList = ParamTypeList;
					RecTemp.TipList = TipList;
					m_RecordList.push_back(RecTemp);
					pSubMenu->AppendMenu(MF_SEPARATOR);
					pSubMenu->AppendMenu(MF_STRING, RecTemp.MenuCommand, w_MenuString.c_str());
					pSubMenu->AppendMenu(MF_STRING, RecTemp.MenuCommand + 1, L"卸载该模块");
					LoadedModule += 2;
				}
				else
					FreeLibrary(hLibrary);
			}
			else
				FreeLibrary(hLibrary);
		}
	}
}


bool CDatabaseEditorDlg::GetParamList(const stringlist ParamTypeList, const stringlist TipList, stringlist& ParamList)
{
	if (!m_Database.Loaded)
		return false;
	if (!m_Database.Selected)
		return false;
	CGetInputDlg StringDlg;
	CGetNumberInputDlg NumberDlg;
	CString Buffer;
	std::wstring Tip;
	std::string temp;
	ParamList.clear();
	for (size_t i = 0; i < ParamTypeList.size(); i++)
	{
		StringToWstring(TipList[i], Tip);
		if (ParamTypeList[i] == u8"String")
			if (StringDlg.StartDialog(&Buffer, Tip.c_str()) == IDCANCEL)
				return false;
			else
			{
				temp.clear();
				WstringToString(Buffer.GetString(), temp);
				ParamList.push_back(temp);
			}
		else if (ParamTypeList[i] == u8"Number")
			if (NumberDlg.StartDialog(&Buffer, Tip.c_str()) == IDCANCEL)
				return false;
			else
			{
				temp.clear();
				WstringToString(Buffer.GetString(), temp);
				ParamList.push_back(temp);
			}
	}
	return true;
}
