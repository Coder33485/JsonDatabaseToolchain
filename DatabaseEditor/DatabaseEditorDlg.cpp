
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
}

void CDatabaseEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_TBL_COMBO, m_ComboTables);
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

	m_CtrlDown = false;

	// TODO: 在此添加额外的初始化代码
	InitDatabaseHandle(Database);

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
	{
		CDialogEx::OnPaint();
	}
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
	if (!Database.Loaded)
		return;
	if (!Database.Selected)
		return;
	std::string fd_name;
	CString FdName, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_NEW_FD_NAME);
	Dlg.StartDialog(&FdName, Tip);
	if (FdName == L"")
		return;
	fd_name = CW2A(FdName, CP_UTF8);
	CreateField(Database, fd_name);
	FlushFields();
}


void CDatabaseEditorDlg::OnAddTbl()
{
	if (!Database.Loaded)
		return;
	CGetInputDlg Dlg;
	CString TblName, Tip;
	std::string tbl_name;
	Tip.LoadString(IDS_TBL_NAME);
	Dlg.StartDialog(&TblName, Tip);
	if (TblName == L"")
		return;
	tbl_name = CW2A(TblName, CP_UTF8);
	CreateTable(Database, tbl_name);
	FlushTables();
}


void CDatabaseEditorDlg::OnCloseDb()
{
	InitDatabaseHandle(Database);
	FlushTables();
	FlushFields();
}


void CDatabaseEditorDlg::OnDbDescription()
{
	if (!Database.Loaded)
		return;
	CDatabaseDescriptionEditor Dlg;
	Dlg.StartDialog(&Database);
}


void CDatabaseEditorDlg::OnDelFd()
{
	if (!Database.Loaded)
		return;
	if (!Database.Selected)
		return;
	std::string fd_name;
	CString FdName, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_FD_NAME);
	Dlg.StartDialog(&FdName, Tip);
	if (FdName == L"")
		return;
	fd_name = CW2A(FdName, CP_UTF8);
	DeleteField(Database, fd_name);
	FlushFields();
}


void CDatabaseEditorDlg::OnDelTbl()
{
	if (!Database.Loaded)
		return;
	CGetInputDlg Dlg;
	CString TblName, Tip;
	std::string tbl_name;
	Tip.LoadString(IDS_TBL_NAME);
	Dlg.StartDialog(&TblName, Tip);
	if (TblName == L"")
		return;
	tbl_name = CW2A(TblName, CP_UTF8);
	DeleteTable(Database, tbl_name);
	FlushTables();
	FlushFields();
}


void CDatabaseEditorDlg::OnNewDb()
{
	if (Database.Loaded)
		return;
	std::string path, name, author;
	CString Path, Name, Author, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_DB_PATH);
	Dlg.StartDialog(&Path, Tip);
	path = CW2A(Path, CP_UTF8);
	Tip.LoadString(IDS_DB_NAME);
	Dlg.StartDialog(&Name, Tip);
	name = CW2A(Name, CP_UTF8);
	Tip.LoadString(IDS_DB_AUTHOR);
	Dlg.StartDialog(&Author, Tip);
	author = CW2A(Author, CP_UTF8);
	CreateDatabase(Database, path, name, author);
	// TODO: 在此添加命令处理程序代码
}


void CDatabaseEditorDlg::OnOpenDb()
{
	if (Database.Loaded)
		return;
	std::string path;
	CString Path, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_DB_PATH);
	Dlg.StartDialog(&Path, Tip);
	path = CW2A(Path, CP_UTF8);
	LoadDatabase(Database, path);
	FlushTables();
}


void CDatabaseEditorDlg::OnRenameTbl()
{
	if (!Database.Loaded)
		return;
	std::string old_table_name, new_table_name;
	CString OldTblName, NewTblName, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_TBL_NAME);
	Dlg.StartDialog(&OldTblName, Tip);
	if (OldTblName == L"")
		return;
	old_table_name = CW2A(OldTblName, CP_UTF8);
	Tip.LoadString(IDS_NEW_TBL_NAME);
	Dlg.StartDialog(&NewTblName, Tip);
	if (NewTblName == L"")
		return;
	new_table_name = CW2A(NewTblName, CP_UTF8);
	RenameTable(Database, old_table_name, new_table_name);
	FlushTables();
}


void CDatabaseEditorDlg::OnRenemeFd()
{
	if (!Database.Loaded)
		return;
	if (!Database.Selected)
		return;
	std::string old_field_name, new_field_name;
	CString OldFdName, NewFdName, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_FD_NAME);
	Dlg.StartDialog(&OldFdName, Tip);
	if (OldFdName == L"")
		return;
	old_field_name = CW2A(OldFdName, CP_UTF8);
	Tip.LoadString(IDS_NEW_FD_NAME);
	Dlg.StartDialog(&NewFdName, Tip);
	if (NewFdName == L"")
		return;
	new_field_name = CW2A(NewFdName, CP_UTF8);
	RenameField(Database, old_field_name, new_field_name);
	FlushFields();
}


void CDatabaseEditorDlg::OnSaveDb()
{
	if (!Database.Loaded)
		return;
	SaveDatabase(Database);
}


void CDatabaseEditorDlg::OnSaveAs()
{
	if (!Database.Loaded)
		return;
	std::string path;
	CString Path, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_DB_PATH);
	Dlg.StartDialog(&Path, Tip);
	path = CW2A(Path, CP_UTF8);
	SaveDatabaseTo(Database, path);
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
	SelectTable(Database, table_name);
	FlushFields();
}


void CDatabaseEditorDlg::FlushFields()
{
	int  int_itemcount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();
	for (int nIndex = 0; nIndex < int_itemcount; ++nIndex)
		m_ListCtrl.DeleteColumn(0);
	m_ListCtrl.DeleteAllItems();
	if (!Database.Loaded)
		return;
	if (!Database.Selected)
		return;
	std::wstringstream wss;
	stringlist list, dat;
	GetFieldList(Database, list);
	std::wstring wstr;
	CString ID;
	ID.LoadString(IDS_LINEID);
	m_ListCtrl.InsertColumn(0, ID);
	for (size_t i = 0; i < list.size(); i++)
	{
		UTF8ToGBK(list[i], wstr);
		m_ListCtrl.InsertColumn(i + 1, wstr.c_str());
		//#error "字符串是UTF8没有问题，但是汉字无法转为GBK显示，使用CA2W结果还是UTF8编码"
		GetFieldData(Database, list[i], dat);
		if (i == 0)
			for (size_t j = 0; j < dat.size(); j++)
			{
				wss.clear();
				wss << j;
				wss >> wstr;
				m_ListCtrl.InsertItem(j, wstr.c_str());
				UTF8ToGBK(dat[j], wstr);
				m_ListCtrl.SetItemText(j, i + 1, wstr.c_str());
			}
		else
			for (size_t j = 0; j < dat.size(); j++)
			{
				UTF8ToGBK(dat[j], wstr);
				m_ListCtrl.SetItemText(j, i + 1, wstr.c_str());
			}
	}
	AdjustColumnWidth(&m_ListCtrl);
}


void CDatabaseEditorDlg::FlushTables()
{
	m_ComboTables.ResetContent();
	if (!Database.Loaded)
		return;
	stringlist table_list;
	std::wstring w_str;
	if (!GetTableList(Database, table_list))
		return;
	for (size_t i = 0; i < table_list.size(); i++)
	{
		UTF8ToGBK(table_list[i], w_str);
		m_ComboTables.AddString(w_str.c_str());
	}
	m_ComboTables.SetCurSel(0);
	OnCbnSelchangeTblCombo();
}


void CDatabaseEditorDlg::OnBnClickedDelSelTblButton()
{
	if (!Database.Loaded)
		return;
	if (!Database.Selected)
		return;
	DeleteTable(Database, Database.Table);
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

		switch (pMsg->wParam)
		{
		case 'S'://Ctrl + S
			if (m_CtrlDown)
			{
				OnSaveDb();
				return TRUE;
			}
			break;
		case VK_CONTROL:
			m_CtrlDown = true;
			return TRUE;
		default:
			return TRUE;
		}
	}
	if (pMsg->message == WM_KEYUP)
	{
		switch (pMsg->wParam)
		{
		case VK_CONTROL:
			m_CtrlDown = false;
			return TRUE;
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
	if (Database.Loaded)
		return;
	LoadDatabase(Database, path);
	FlushTables();
	CDialogEx::OnDropFiles(hDropInfo);
}


void CDatabaseEditorDlg::OnTblDescription()
{
	if (!Database.Loaded)
		return;
	if (!Database.Selected)
		return;
	CTableDescriptionEditor Dlg;
	Dlg.StartDialog(&Database);
}


void CDatabaseEditorDlg::OnDelRow()
{
	if (!Database.Loaded)
		return;
	if (!Database.Selected)
		return;
	size_t id = 0;
	CGetNumberInputDlg Dlg;
	Dlg.StartDialog(&id, L"行号");
	DeleteLine(Database, id);
	FlushFields();
}


void CDatabaseEditorDlg::OnDelMultiRow()
{
	if (!Database.Loaded)
		return;
	if (!Database.Selected)
		return;
	size_t id = 0, num = 0;
	CGetNumberInputDlg Dlg;
	Dlg.StartDialog(&id, L"起始行号");
	Dlg.StartDialog(&num, L"行数");
	DeleteLine(Database, id, num);
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
	if (!Database.Loaded)
		return;
	if (!Database.Selected)
		return;
	stringlist list;
	GetFieldList(Database, list);
	CGetInputDlg Dlg;
	CString Unit;
	std::wstring w_str;
	stringlist unit_list;
	for (size_t i = 0; i < list.size(); i++)
	{
		UTF8ToGBK(list[i], w_str);
		Dlg.StartDialog(&Unit, w_str.c_str());
		unit_list.push_back(std::string(CW2A(Unit, CP_UTF8)));
	}
	AppendLine(Database, unit_list);
	FlushFields();
}


void CDatabaseEditorDlg::OnAddMultiRow()
{
	if (!Database.Loaded)
		return;
	if (!Database.Selected)
		return;
	stringlist list;
	GetFieldList(Database, list);
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
			UTF8ToGBK(list[i], w_str);
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
			AppendLine(Database, unit_list);
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
	if (!Database.Loaded)
		return;
	std::string new_table_name;
	CString NewTblName, Tip;
	CGetInputDlg Dlg;
	Tip.LoadString(IDS_NEW_TBL_NAME);
	Dlg.StartDialog(&NewTblName, Tip);
	if (NewTblName == L"")
		return;
	new_table_name = CW2A(NewTblName, CP_UTF8);
	RenameTable(Database, Database.Table, new_table_name);
	FlushTables();
}
