#include "stdafx.h"
#include "CalculatorDlg.h"
#include "Resource.h"
#include <cmath>

IMPLEMENT_DYNAMIC(CalculatorDlg, CDialogEx)

BEGIN_MESSAGE_MAP(CalculatorDlg, CDialogEx)
ON_WM_CREATE()
ON_BN_CLICKED(IDC_ADD, OnAdd)
ON_BN_CLICKED(IDC_SUB, OnSub)
ON_BN_CLICKED(IDC_MUL, OnMul)
ON_BN_CLICKED(IDC_DIV, OnDiv)
ON_BN_CLICKED(IDC_MOD, OnMod)
ON_BN_CLICKED(IDC_EXP, OnExp)
ON_EN_CHANGE(IDC_NUM1, OnNum)
ON_EN_CHANGE(IDC_NUM2, OnNum)
END_MESSAGE_MAP()

CalculatorDlg::CalculatorDlg() noexcept //standalone
	: CDialogEx(IDD_CALCULATOR)
	, num1(0)
	, num2(0)
	, result("0")
	, operation(' ')
	, history{}
{}

void CalculatorDlg::Init()
{
	DoModal();
}

void CalculatorDlg::Update(const std::string& strRes)
{
	CString c{strRes.c_str()};
	result.SetString(c);
	CString txt;
	txt.Format(L"%g %c %g = %s", num1, operation, num2, result);
	int count = history.AddString(txt); 
	history.SetCurSel(count);
	UpdateData(FALSE);
}

std::pair<double, double> CalculatorDlg::GetValues()
{
	UpdateData(TRUE);
	return std::make_pair(num1, num2);
}

void CalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	::DDX_Text(pDX, IDC_NUM1, num1);
	::DDX_Text(pDX, IDC_NUM2, num2);
	::DDX_Text(pDX, IDC_RESULT, result);
	::DDX_Control(pDX, IDC_HIST, history);
}

BOOL CalculatorDlg::OnInitDialog()
{
	BOOL res = CDialogEx::OnInitDialog();
	if(res != FALSE)
		history.SetWindowText(L"History");
	return res;
}

void CalculatorDlg::OnAdd()
{
	//UpdateData();
	//result.Format(L"%g", num1 + num2);
	//operation = '+';
	//CString txt;
	//txt.Format(L"%g + %g = %s", num1, num2, result);
	//int count = history.AddString(txt);
	//history.SetCurSel(count);
	//UpdateData(FALSE);

	//new
	operation = '+';
	onAdd();
}

void CalculatorDlg::OnSub()
{
	//UpdateData();
	//result.Format(L"%g", num1 - num2);
	//operation = '-';
	//CString txt;
	//txt.Format(L"%g - %g = %s", num1, num2, result);
	//int count = history.AddString(txt);
	//history.SetCurSel(count);
	//UpdateData(FALSE);

	//new
	operation = '-';
	onSub();
}

void CalculatorDlg::OnMul()
{
	//UpdateData();
	//result.Format(L"%g", num1 * num2);
	//operation = '*';
	//CString txt;
	//txt.Format(L"%g * %g = %s", num1, num2, result);
	//int count = history.AddString(txt);
	//history.SetCurSel(count);
	//UpdateData(FALSE);

	//new
	operation = '*';
	onMul();
}

void CalculatorDlg::OnDiv()
{
	//UpdateData();
	//result.Format(L"%g", num1 / num2);
	//operation = '/';
	//CString txt;
	//txt.Format(L"%g / %g = %s", num1, num2, result);
	//int count = history.AddString(txt);
	//history.SetCurSel(count);
	//UpdateData(FALSE);

	//new
	operation = '/';
	onDiv();
}

void CalculatorDlg::OnMod()
{
	//UpdateData();
	//auto divis = static_cast<int>(std::abs(num2));
	//if(divis == 0)
	//	result.Format(L"%g", 0.0);
	//else {
	//	auto divid = static_cast<int>(std::abs(num1));
	//	if((num1 < 0 && num2 > 0) || (num1 > 0 && num2 < 0))
	//		result.Format(L"%i", -divid % divis);
	//	else
	//		result.Format(L"%i", divid % divis);
	//}
	//operation = '%';
	//CString txt;
	//txt.Format(L"%g mod %g = %s", num1, num2, result);
	//int count = history.AddString(txt);
	//history.SetCurSel(count);
	//UpdateData(FALSE);

	//new
	operation = '%';
	onMod();
}

void CalculatorDlg::OnExp()
{
	//UpdateData();
	//result.Format(L"%g", pow(num1, num2));
	//operation = '^';
	//CString txt;
	//txt.Format(L"%g ^ %g = %s", num1, num2, result);
	//int count = history.AddString(txt);
	//history.SetCurSel(count);
	//UpdateData(FALSE);

	//new
	operation = '^';
	onExp();
}

void CalculatorDlg::OnNum()
{
	//switch(operation)
	//{
	//case '+':
	//	OnAdd();
	//	break;
	//case '-':
	//	OnSub();
	//	break;
	//case '*':
	//	OnMul();
	//	break;
	//case '/':
	//	OnDiv();
	//	break;
	//case '%':
	//	OnMod();
	//	break;
	//case '^':
	//	OnExp();
	//	break;
	//}
}

void CalculatorDlg::OnOK()
{
	//Keep empty to prevent close dialog if press key 'ENTER'
}
