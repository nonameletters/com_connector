#include "mainwindow.h"
#include <QApplication>
#include <QAxObject>
#include <QTextCodec>

#include <windows.h>
#include <initguid.h>
#include <iostream>
#include <unknwn.h>

using namespace std;

int main(int argc, char *argv[])
{
    QTextCodec* local = QTextCodec::codecForLocale();
    cout << QString(local->name()).toStdString() << endl;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("windows-1251"));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QAxObject ax("V83.COMConnector.1");
    //QAxObject* con = ax.querySubObject("GetIDsOfNames");
    //QAxObject* con = ax.querySubObject("Connect(\"Srvr=\"srv-app\";Ref=\"zup_test_2\";Usr=\"Администратор\";Pwd=\"qwerty\";\")");
    //ax.dynamicCall("Connect(\"Srvr=srv-app;Ref=zup_test_2;Usr=Admin;Pwd=12345678;\")");
    //QAxObject* con = ax.querySubObject("Connect(\"Srvr=srv-app;Ref=zup_test_2;Usr=Администратор;Pwd=qwerty;\")");

    //QString l_conStr = "Connect(\"Srvr=srv-app;Ref=zup_test_2;Usr=Admin;Pwd=12345678;\")";
    QString l_conStr = "Connect(\"Srvr=srv-app;Ref=zup_test_2;Usr=Администратор;Pwd=qwerty;\")";
    cout << "STR1: " << l_conStr.toStdString().c_str() << endl;
    QTextCodec* l_codec = QTextCodec::codecForName("windows-1251");
    QByteArray resStr = l_codec->fromUnicode(l_conStr);
    cout << "STR2: " << resStr.constData() << endl;

    //QAxObject* con = ax.querySubObject(l_conStr.toStdString().c_str());
    QAxObject* con = ax.querySubObject(l_codec->toUnicode(resStr.constData()).toStdString().c_str());

    // QAxObject* con = ax.querySubObject("GetUsers()");
    if (con)
    {
        cout << "Connection get" << endl;
    }

    // QString l_res = "someshit";

    //local = QTextCodec::codecForLocale();
    //    cout << QString(local->name()).toStdString() << endl;
    //con->querySubObject("NewObject", l_res);

/*
    if (FAILED(CoInitialize(NULL)))
    {
        cout << "Connection FAILED" << endl;
    }

    CLSID v83Sid;

    char* szV83ProgId = "V83.COMConnector.1";

    WCHAR szWideProgId[128];
    long lLen = MultiByteToWideChar(CP_ACP, 0, szV83ProgId, strlen(szV83ProgId), szWideProgId, sizeof(szWideProgId));
    szWideProgId[lLen] = '\0';

    HRESULT hr = CLSIDFromProgID(szWideProgId, &v83Sid);

    if (FAILED(hr))
    {
        cout.setf(ios::hex, ios::basefield);
        cout << "ID: " <<  hr << endl;
    }
    else
    {
        cout << "CLSID SUCCESS" << endl;
    }

    IClassFactory* pCF;
    hr = CoGetClassObject(v83Sid, CLSCTX_INPROC, NULL, IID_IClassFactory, (void**) &pCF);

    IDispatch* pUnk;
    hr = pCF->CreateInstance(NULL, IID_IDispatch, (void**) &pUnk);

    DISPID dispIdInit;
    OLECHAR * fName = L"Connect";

    hr = pUnk->GetIDsOfNames(IID_NULL, &fName,1, LOCALE_SYSTEM_DEFAULT, &dispIdInit);

    pCF->Release();

    DISPPARAMS dispParam = {NULL, NULL, 0, 0};
    dispParam.rgvarg = new VARIANT[1];
    dispParam.cArgs = 1;

    VariantInit(dispParam.rgvarg);
    dispParam.rgvarg[0].vt = VT_BSTR;
    dispParam.rgvarg[0].bstrVal = SysAllocString(OLESTR("Srvr=srv-app;Ref=zup_test_2;Usr=Администратор;Pwd=qwerty;"));

    IDispatch* conDisp;
    VARIANT conResult;

    hr = pUnk->Invoke(dispIdInit,IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispParam, &conResult, NULL, NULL);

    if (FAILED(hr))
    {
        cout << "Connect failed." << endl;
    }
    else
    {
        cout << "Connection SUCCESS to zup_test" << endl;
    }
    //pUnk->Release();

    fName = L"ConnectWorkingProcess";
    hr = pUnk->GetIDsOfNames(IID_NULL, &fName,1, LOCALE_SYSTEM_DEFAULT, &dispIdInit);

    if (FAILED(hr))
    {
        cout << "Create Object fault" << endl;
    }
*/

    return a.exec();
}
