// SCommTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SCommTest.h"
#include "SCommTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCommTestDlg dialog

CSCommTestDlg::CSCommTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCommTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSCommTestDlg)
	m_strRXData = _T("");
	m_strTXData = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bAutoSend = FALSE; //m_bAutoSendΪBOOL�ͱ�������CLASSVIEW��ΪCSCommTestDlg����룬���ڹ��캯���г�ʼ��

}

void CSCommTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCommTestDlg)
	DDX_Control(pDX, IDC_CHECK_HEXDISPLAY, m_ctrlHexDisplay);
	DDX_Control(pDX, IDC_CHECK_HEXSEND, m_ctrlHexSend);
	DDX_Control(pDX, IDC_MSCOMM1, m_ctrlComm);
	DDX_Text(pDX, IDC_EDIT_RXDATA, m_strRXData);
	DDX_Text(pDX, IDC_EDIT_TXDATA, m_strTXData);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSCommTestDlg, CDialog)
	//{{AFX_MSG_MAP(CSCommTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MANUALSEND, OnButtonManualsend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_AUTOSEND, OnCheckAutosend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCommTestDlg message handlers

BOOL CSCommTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	if(m_ctrlComm.GetPortOpen())
		m_ctrlComm.SetPortOpen(FALSE);
	
	m_ctrlComm.SetCommPort(12); //ѡ��com12
	if( !m_ctrlComm.GetPortOpen())
		m_ctrlComm.SetPortOpen(TRUE);//�򿪴���
	else
		AfxMessageBox("cannot open serial port");
	
	m_ctrlComm.SetSettings("9600,n,8,1"); //������9600����У�飬8������λ��1��ֹͣλ 
	m_ctrlComm.SetInputMode(1); //1����ʾ�Զ����Ʒ�ʽ��ȡ����
	m_ctrlComm.SetRThreshold(1); 
	//����1��ʾÿ�����ڽ��ջ��������ж��ڻ����1���ַ�ʱ������һ���������ݵ�OnComm�¼�
	m_ctrlComm.SetInputLen(0); //���õ�ǰ���������ݳ���Ϊ0
	m_ctrlComm.GetInput();//��Ԥ���������������������

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSCommTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSCommTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSCommTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BEGIN_EVENTSINK_MAP(CSCommTestDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSCommTestDlg)
	ON_EVENT(CSCommTestDlg, IDC_MSCOMM1, 1 /* OnComm */, OnComm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSCommTestDlg::OnComm() 
{
	// TODO: Add your control notification handler code here
// 	VARIANT variant_inp;
//     COleSafeArray safearray_inp;
//     LONG len,k;
//     BYTE rxdata[2048]; //����BYTE���� An 8-bit integer that is not signed.
//     CString strtemp;
//     if(m_ctrlComm.GetCommEvent()==2) //�¼�ֵΪ2��ʾ���ջ����������ַ�
//     {             
// 		////////��������Ը����Լ���ͨ��Э����봦�����
//         variant_inp=m_ctrlComm.GetInput(); //��������
//         safearray_inp=variant_inp; //VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
//         len=safearray_inp.GetOneDimSize(); //�õ���Ч���ݳ���
//         for(k=0;k<len;k++)
//             safearray_inp.GetElement(&k,rxdata+k);//ת��ΪBYTE������
//         for(k=0;k<len;k++) //������ת��ΪCstring�ͱ���
//         {
//             BYTE bt=*(char*)(rxdata+k); //�ַ���
//             strtemp.Format("%c",bt); //���ַ�������ʱ����strtemp���
//             m_strRXData+=strtemp; //������ձ༭���Ӧ�ַ��� 
//         }
//     }
//     UpdateData(FALSE); //���±༭������


	
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len,k;
	BYTE rxdata[2048]; //����BYTE���� An 8-bit integer that is not signed.
	CString strtemp;
	if(m_ctrlComm.GetCommEvent()==2) //�¼�ֵΪ2��ʾ���ջ����������ַ�
	{
		////////��������Ը����Լ���ͨ��Э����봦�����
		variant_inp=m_ctrlComm.GetInput(); //��������
		safearray_inp=variant_inp; //VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
		len=safearray_inp.GetOneDimSize(); //�õ���Ч���ݳ���
		for(k=0;k<len;k++)
			safearray_inp.GetElement(&k,rxdata+k);//ת��ΪBYTE������
		for(k=0;k<len;k++) //������ת��ΪCstring�ͱ���
		{
			BYTE bt=*(char*)(rxdata+k); //�ַ���
			if(m_ctrlHexDisplay.GetCheck())
				strtemp.Format("%02X ",bt); //���ַ���ʮ�����Ʒ�ʽ������ʱ����strtemp��ţ�ע���������һ���ո�
			else 
				strtemp.Format("%c",bt); //���ַ�������ʱ����strtemp���
			
			m_strRXData+=strtemp; //������ձ༭���Ӧ�ַ��� 
		}
	}
	UpdateData(FALSE); //���±༭������
}

void CSCommTestDlg::OnButtonManualsend() 
{
	// TODO: Add your control notification handler code here
// 	UpdateData(TRUE); //��ȡ�༭������
// 	m_ctrlComm.SetOutput(COleVariant(m_strTXData));//����ASCII����

	UpdateData(TRUE); //��ȡ�༭������
	if(m_ctrlHexSend.GetCheck())
	{
		CByteArray hexdata;
		int len=String2Hex(m_strTXData,hexdata); //�˴����ص�len�������ڼ��㷢���˶��ٸ�ʮ��������
		m_ctrlComm.SetOutput(COleVariant(hexdata)); //����ʮ����������
	}
	else 
		m_ctrlComm.SetOutput(COleVariant(m_strTXData));//����ASCII�ַ�����
}

int CSCommTestDlg::String2Hex(CString str, CByteArray &senddata)
{
	//�������ת�������ĸ�ʽ���ƣ��ڷ��Ϳ��е�ʮ�����ַ�Ӧ��ÿ�����ַ�֮�����һ���ո�
    //�磺A1 23 45 0B 00 29
	int hexdata,lowhexdata;
	int hexdatalen=0;
	int len=str.GetLength();
	senddata.SetSize(len/2);
	for(int i=0;i<len;)
	{
		char lstr,hstr=str[i];
		if(hstr==' ')
		{
			i++;
			continue;
		}
		i++;
		if(i>=len)
			break;
		lstr=str[i];
		hexdata=ConvertHexChar(hstr);
		lowhexdata=ConvertHexChar(lstr);
		if((hexdata==16)||(lowhexdata==16))
			break;
		else 
			hexdata=hexdata*16+lowhexdata;
		i++;
		senddata[hexdatalen]=(char)hexdata;
		hexdatalen++;
	}
	senddata.SetSize(hexdatalen);
	return hexdatalen;
}


char CSCommTestDlg::ConvertHexChar(char ch)
{
	//����һ�����ַ�ת��Ϊ��Ӧ��ʮ������ֵ�ĺ���
    //���ܣ�������0-F֮����ַ�����ת��Ϊ��Ӧ��ʮ�������ַ������򷵻�-1
	if((ch>='0')&&(ch<='9'))
		return ch-0x30;
	else if((ch>='A')&&(ch<='F'))
		return ch-'A'+10;
	else if((ch>='a')&&(ch<='f'))
		return ch-'a'+10;
	else return (-1);
}

void CSCommTestDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	OnButtonManualsend();
	CDialog::OnTimer(nIDEvent);
}

void CSCommTestDlg::OnCheckAutosend() 
{
	// TODO: Add your control notification handler code here
	m_bAutoSend=!m_bAutoSend;
	if(m_bAutoSend)
	{
		SetTimer(1,1000,NULL);//ʱ��Ϊ1000����
	}
	else
	{
		KillTimer(1);  //ȡ����ʱ
	}

}
