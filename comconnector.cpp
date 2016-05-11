#include "comconnector.h"

namespace nsComConnector
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    CComConnector::CComConnector()
    {

    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CComConnector::~CComConnector()
    {
        connection->Release();
        instanceConnection->Release();

        vector<IDispatch*>::iterator it = m_parents.begin();
        for(;it != m_parents.end(); it++)
        {
            (*it)->Release();
            cout << "Released" << endl;
        }
        CoUninitialize();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    HRESULT CComConnector::makeConnect(string object, wstring conString)
    {
        // "V83.COMConnector.1"
        initCom(object, &instanceConnection);

        if (instanceConnection == NULL)
        {
            return -1;
        }

        HRESULT hr;
        hr = execInvoke(L"Connect", conString, DISPATCH_METHOD, 1, instanceConnection, &m_resInvoke);
        connection = m_resInvoke.pdispVal;

        return hr;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CPerson*> CComConnector::getEmployeis(const QString& tabNumberPrefix)
    {
        QString l_mes = "START getEmployeis: " + QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
        CLogger::getInstance()->log(l_mes.toStdString().c_str());

        vector<CPerson*> employeis;

        IDispatch* nextObject = NULL;
        m_dogVar = m_resInvoke;

        execInvoke(L"Справочники", L"", DISPATCH_PROPERTYGET, 0, connection, &m_resInvoke);
        nextObject = m_resInvoke.pdispVal;
        m_parents.push_back(nextObject);

        execInvoke(L"СотрудникиОрганизаций", L"", DISPATCH_PROPERTYGET, 0,nextObject, &m_resInvoke);
        nextObject = m_resInvoke.pdispVal;
        m_parents.push_back(nextObject);

        execInvoke(L"Выбрать", L"", DISPATCH_METHOD, 0, nextObject, &m_resInvoke);
        nextObject = m_resInvoke.pdispVal;
        m_parents.push_back(nextObject);

        execInvoke(L"Next", L"", DISPATCH_METHOD, 0, nextObject, &m_resInvoke);

        int count = 1;
        bool nextRes = m_resInvoke.boolVal;
        IDispatch* l_dolg = NULL;
        while (nextRes != false)
        {
            cout << "Loading employees: " << count << endl;
            CPerson* newEmployee = new CPerson();
            // execInvoke(L"Наименование", L"", DISPATCH_PROPERTYGET, 0, nextObject, &m_resInvoke);

            IDispatch* l_fizLico = NULL;
            execInvoke(L"Физлицо", L"", DISPATCH_PROPERTYGET, 0, nextObject, &m_resInvoke);
            l_fizLico = m_resInvoke.pdispVal;
            if (l_fizLico != NULL)
            {
                execInvoke(L"Наименование", L"", DISPATCH_PROPERTYGET, 0, l_fizLico, &m_resInvoke);
                // I used this line to convert from unicode to CP1251 and see russian letters in console
                // QByteArray name = codec->fromUnicode(l_str);
                QString lv_name       = QString::fromWCharArray(m_resInvoke.bstrVal);
                QStringList l_fio     = lv_name.split(" ");

                QString l_fName;
                QString l_mName;
                QString l_lName;

                if (l_fio.size() == 1)
                {
                    l_fName = l_fio.at(0);
                }
                else if (l_fio.size() == 2)
                {
                    l_fName = l_fio.at(0);
                    l_mName = l_fio.at(1);
                }
                else if (l_fio.size() == 3)
                {
                    l_fName = l_fio.at(0);
                    l_mName = l_fio.at(1);
                    l_lName = l_fio.at(2);
                }

                l_mes = l_fName + " " + l_mName + " " + l_lName;
                l_mes = l_mes.trimmed();
                if (l_mes.trimmed().isEmpty() == true)
                {
                    l_fizLico->Release();
                    CLogger::getInstance()->log("EMPTY NAME.");
                    execInvoke(L"Next", L"", DISPATCH_METHOD, 0, nextObject, &m_resInvoke);
                    nextRes = m_resInvoke.boolVal;
                    count++;
                    delete newEmployee;
                    continue;
                }
                CLogger::getInstance()->log(toWinStr(l_mes).c_str());

                newEmployee->setFirstName(l_fName);
                newEmployee->setMidleName(l_mName);
                newEmployee->setLastName(l_lName);

                //СтраховойНомерПФР 14символов
                execInvoke(L"СтраховойНомерПФР", L"", DISPATCH_PROPERTYGET, 0, l_fizLico, &m_resInvoke);
                QString l_snils = toWinStr(m_resInvoke.bstrVal).c_str();
                l_snils = l_snils.trimmed();
                if (l_snils.isEmpty())
                {
                    l_snils = "temp";
                }
                newEmployee->setInn(l_snils.toStdString().c_str());
                newEmployee->setSnils(l_snils.toStdString().c_str());

                //ДатаРождения
                execInvoke(L"ДатаРождения", L"", DISPATCH_PROPERTYGET, 0, l_fizLico, &m_resInvoke);
                newEmployee->setBirthDay(oleDateToQDate(m_resInvoke.date));
                l_fizLico->Release();
            }

            execInvoke(L"Актуальность", L"", DISPATCH_PROPERTYGET, 0, nextObject, &m_resInvoke);
            bool activity = m_resInvoke.boolVal;
            if (activity == false)
            {
                CLogger::getInstance()->log("\tNot Active.");
                execInvoke(L"Next", L"", DISPATCH_METHOD, 0, nextObject, &m_resInvoke);
                nextRes = m_resInvoke.boolVal;
                count++;
                continue;
            }

            newEmployee->setActivity(activity);

            execInvoke(L"Код", L"", DISPATCH_PROPERTYGET, 0, nextObject, &m_resInvoke);
            newEmployee->setTableNumber(convertTableNumber(tabNumberPrefix, toWinStr(m_resInvoke.bstrVal)));

            execInvoke(L"ДатаПриемаНаРаботу", L"", DISPATCH_PROPERTYGET, 0, nextObject, &m_resInvoke);
            newEmployee->setHireDate(oleDateToQDate(m_resInvoke.date));

            execInvoke(L"ДатаУвольнения", L"", DISPATCH_PROPERTYGET, 0, nextObject, &m_resInvoke);
            newEmployee->setDismissionDate(oleDateToQDate(m_resInvoke.date));

            execInvoke(L"Должность", L"", DISPATCH_PROPERTYGET, 0, nextObject, &m_resInvoke);
            l_dolg = m_resInvoke.pdispVal;
            if (l_dolg != NULL)
            {
                execInvoke(L"Наименование", L"", DISPATCH_PROPERTYGET, 0, l_dolg, &m_resInvoke);
                newEmployee->setPosition(toWinStr(m_resInvoke.bstrVal).c_str());

                l_dolg->Release();
            }

            IDispatch* l_org = NULL;
            execInvoke(L"Организация", L"", DISPATCH_PROPERTYGET, 0, nextObject, &m_resInvoke);
            l_org = m_resInvoke.pdispVal;
            if (l_org != NULL)
            {
                execInvoke(L"Наименование", L"", DISPATCH_PROPERTYGET, 0, l_org, &m_resInvoke);
                newEmployee->setOrganisation(toWinStr(m_resInvoke.bstrVal));
                l_org->Release();
            }

            execInvoke(L"ВидЗанятости", L"", DISPATCH_PROPERTYGET, 0, nextObject, &m_resInvoke);
            IDispatch* l_curVid = m_resInvoke.pdispVal;

            execInvokeRefParam(L"XMLString", l_curVid, DISPATCH_METHOD, 1, connection, &m_resInvoke);
            newEmployee->setVidZanyatosti(toWinStr(m_resInvoke.bstrVal).c_str());
            CLogger::getInstance()->log(toWinStr(m_resInvoke.bstrVal).c_str());
            if (l_curVid != NULL)
            {
                l_curVid->Release();
            }


            execInvoke(L"ВидДоговора", L"", DISPATCH_PROPERTYGET, 0, nextObject, &m_resInvoke);
            l_curVid = m_resInvoke.pdispVal;

            execInvokeRefParam(L"XMLString", l_curVid, DISPATCH_METHOD, 1, connection, &m_resInvoke);
            newEmployee->setVidDogovora(toWinStr(m_resInvoke.bstrVal).c_str());
            CLogger::getInstance()->log(toWinStr(m_resInvoke.bstrVal).c_str());
            if (l_curVid != NULL)
            {
                l_curVid->Release();
            }

            execInvoke(L"Next", L"", DISPATCH_METHOD, 0, nextObject, &m_resInvoke);

            count++;
            employeis.push_back(newEmployee);
            nextRes = m_resInvoke.boolVal;
        }

        l_mes = "END getEmployeis: " + QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
        CLogger::getInstance()->log(l_mes.toStdString().c_str());
        // cout << "END: getEmploies" << endl;
        return employeis;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CComConnector::HrMessage(const HRESULT hr)
    {
        switch(hr)
        {
            case S_OK:
                cout << "Success." << endl;
                break;
            case DISP_E_BADPARAMCOUNT:
                cout << "The number of elements provided to DISPPARAMS is different from the number of parameters accepted." << endl;
                break;
            case DISP_E_BADVARTYPE:
                cout << "DISP_E_BADVARTYPE" << endl;
                break;
            case DISP_E_EXCEPTION:
                cout << "DISP_E_EXCEPTION" << endl;
                break;
            case DISP_E_MEMBERNOTFOUND:
                cout << "DISP_E_MEMBERNOTFOUND" << endl;
                break;
            case DISP_E_NONAMEDARGS:
                cout << "DISP_E_NONAMEDARGS" << endl;
                break;
            case DISP_E_OVERFLOW:
                cout << "DISP_E_OVERFLOW" << endl;
                break;
            case DISP_E_PARAMNOTFOUND:
                cout << "DISP_E_PARAMNOTFOUND" << endl;
                break;
            case DISP_E_TYPEMISMATCH:
                cout << "DISP_E_TYPEMISMATCH" << endl;
                break;
            case DISP_E_UNKNOWNINTERFACE:
                cout << "DISP_E_UNKNOWNINTERFACE" << endl;
                break;
            case DISP_E_UNKNOWNLCID:
                cout << "DISP_E_UNKNOWNLCID" << endl;
                break;
            case DISP_E_PARAMNOTOPTIONAL:
                cout << "DISP_E_PARAMNOTOPTIONAL" << endl;
                break;
            default:
                break;
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    HRESULT CComConnector::initCom(string comServerName, IDispatch** outInstanceDispatch)
    {
        if (FAILED(CoInitialize(NULL)))
        {
            cout << "ComOle initialization FAILED" << endl;
        }

        CLSID v83Sid;

        WCHAR szWideProgId[128];
        long lLen = MultiByteToWideChar(CP_ACP, 0, comServerName.c_str(), strlen(comServerName.c_str()), szWideProgId, sizeof(szWideProgId));
        szWideProgId[lLen] = '\0';

        HRESULT hr = CLSIDFromProgID(szWideProgId, &v83Sid);

        if (FAILED(hr))
        {
            cout.setf(ios::hex, ios::basefield);
            cout << "Failed get ID by name: " << comServerName << endl;
            cout.unsetf(ios::hex);
        }
        else
        {
            cout << "CLSID SUCCESSFULLY FOUND" << endl;
        }

        IClassFactory* pCF;
        hr = CoGetClassObject(v83Sid, CLSCTX_INPROC, NULL, IID_IClassFactory, (void**) &pCF);

        if (FAILED(hr))
        {
            cout << "FAILED: CoGetClassObject" << endl;
        }

        hr = pCF->CreateInstance(NULL, IID_IDispatch, (void**) outInstanceDispatch);

        if (FAILED(hr))
        {
            cout << "FAILED: CreateInstance" << endl;
        }

        pCF->Release();

        pCF = NULL;
        return hr;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    HRESULT CComConnector::execInvoke(wstring methodName, wstring param, WORD objType, int paramNum, IDispatch* parent, VARIANT* inOutRes)
    {
        if (parent == NULL)
        {
            return E_POINTER;
        }

        DISPID dispIdInit;
        OLECHAR* fName = new OLECHAR[methodName.size() + 1];
        wcsncpy(fName, methodName.c_str(), methodName.size() + 1);


        HRESULT hr;
        hr = parent->GetIDsOfNames(IID_NULL, &fName,1, LOCALE_SYSTEM_DEFAULT, &dispIdInit);
        // cout << "DISP ID: " << dispIdInit << endl;
        delete[] fName;

        if (FAILED(hr))
        {
            cout << "GetIDsOfNames: " << methodName.c_str() << " FAILED." << endl;
        }

        EXCEPINFO except;
        DISPPARAMS dispParam = {NULL, NULL, 0, 0};

        if (paramNum > 0)
        {
            dispParam.rgvarg = new VARIANT[paramNum];
            dispParam.cArgs = paramNum;
        }


        OLECHAR* lParam;

        lParam = new OLECHAR[param.size() + 1];
        wcsncpy(lParam, param.c_str(), param.size() + 1);

        if (paramNum > 0)
        {
            VariantInit(dispParam.rgvarg);
            dispParam.rgvarg[0].vt = VT_BSTR;
            dispParam.rgvarg[0].bstrVal = SysAllocString(lParam);
        }

        inOutRes->pdispVal = NULL;

        // cout << "1 " << inOutRes->pdispVal << endl;
        hr = parent->Invoke(dispIdInit, IID_NULL, LOCALE_SYSTEM_DEFAULT, objType, &dispParam, inOutRes, &except, NULL);
        // cout << "2 " << inOutRes->pdispVal << endl;
        delete[] lParam;

        if (paramNum > 0)
        {
            delete[] dispParam.rgvarg;
        }

        if (inOutRes->pdispVal == NULL)
        {
            // cout << "Next object is NULL " << endl;
        }


        if (FAILED(hr))
        {
            HrMessage(hr);
            cout << "Connect failed." << endl;
            cout << "Ex error code: " << except.wCode << endl;
        }

        return hr;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    HRESULT CComConnector::execInvokeRefParam(wstring methodName, IDispatch* param, WORD objType, int paramNum, IDispatch* parent, VARIANT* inOutRes)
    {
        if (parent == NULL)
        {
            return E_POINTER;
        }

        DISPID dispIdInit;
        OLECHAR* fName = new OLECHAR[methodName.size() + 1];
        wcsncpy(fName, methodName.c_str(), methodName.size() + 1);


        HRESULT hr;
        hr = parent->GetIDsOfNames(IID_NULL, &fName,1, LOCALE_SYSTEM_DEFAULT, &dispIdInit);
        // cout << "DISP ID: " << dispIdInit << endl;
        delete[] fName;

        if (FAILED(hr))
        {
            cout << "GetIDsOfNames RefParam: " << methodName.c_str() << " FAILED." << endl;
        }

        EXCEPINFO except;
        DISPPARAMS dispParam = {NULL, NULL, 0, 0};

        if (paramNum > 0)
        {
            dispParam.rgvarg = new VARIANT[paramNum];
            dispParam.cArgs = paramNum;
        }


//        OLECHAR* lParam;

//        lParam = new OLECHAR[param.size() + 1];
//        wcsncpy(lParam, param.c_str(), param.size() + 1);

        if (paramNum > 0)
        {
            VariantInit(dispParam.rgvarg);
            dispParam.rgvarg[0].vt = VT_DISPATCH;
            dispParam.rgvarg[0].pdispVal = param;
        }

        inOutRes->pdispVal = NULL;

        // cout << "1 " << inOutRes->pdispVal << endl;
        hr = parent->Invoke(dispIdInit, IID_NULL, LOCALE_SYSTEM_DEFAULT, objType, &dispParam, inOutRes, &except, NULL);
        // cout << "2 " << inOutRes->pdispVal << endl;
        //delete[] lParam;

        if (paramNum > 0)
        {
            delete[] dispParam.rgvarg;
        }

        if (inOutRes->pdispVal == NULL)
        {
            // cout << "Next object is NULL " << endl;
        }


        if (FAILED(hr))
        {
            HrMessage(hr);
            cout << "Connect failed." << endl;
            cout << "Ex error code: " << except.wCode << endl;
        }

        return hr;
    }


    // ---------- ---------- ---------- ---------- ---------- ----------
    QDate CComConnector::oleDateToQDate(DATE date)
    {
        QDate l_zeroPoint(1899, 12, 30);
        if (static_cast<double>(date) > 0)
        {
           l_zeroPoint = l_zeroPoint.addDays(static_cast<double>(date));
        }

        l_zeroPoint = l_zeroPoint.addDays(2);
        return l_zeroPoint;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CComConnector::toWinStr(BSTR str)
    {
        QString l_str = QString::fromWCharArray(str);

        return toWinStr(l_str);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CComConnector::toWinStr(QString str)
    {
        QTextCodec* codec = QTextCodec::codecForName("CP1251");
        QByteArray kod = codec->fromUnicode(str);
        return kod.data();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CComConnector::convertTableNumber(const QString prefix, const string number)
    {
        QString res;
        QString l_number(number.c_str());
        l_number = l_number.trimmed();

        int zeroCount = 0;
        int l_pos     = 0;
        while ((l_number.at(l_pos) == '0') && (l_pos < (int)l_number.size()))
        {
            l_pos++;
            zeroCount++;
        }
        l_number = l_number.remove(0, zeroCount);

        zeroCount = 10 - prefix.size() - l_number.size();
        QString l_z = "0";
        l_z = l_z.repeated(zeroCount);

        res = prefix + l_z + l_number;

        return res;
    }

//    // ---------- ---------- ---------- ---------- ---------- ----------
//    QDate CComConnector::oleDateToQDate(DATE date)
//    {
//        for (int i = 0; i <= 6; i++)
//        {
//            cout <<"" << i << " Date double: " << setprecision(i) << (double) date << endl;
//        }
//        USHORT maskYear  = 0xFE00; // shiftR 9
//        USHORT maskMonth = 0x01E0; // shiftR 5
//        USHORT maskDay   = 0x001F; // zero shift;

//        USHORT dosDate;
//        USHORT dosTime;
//        int res = VariantTimeToDosDateTime(date, &dosDate, &dosTime);

//        if (res == 1)
//        {
//            cout << "DATE IS OK" << endl;
//        }
//        else
//        {
//            cout << "DATE IS WRONG" << endl;
//        }

//        USHORT day   = (dosDate & maskDay);
//        USHORT month = (dosDate & maskMonth) >> 5;
//        USHORT year  = 1980 + ((dosDate & maskYear) >> 9);

//        // cout.setf(ios::hex, ios::basefield);
//        // cout << "DOSDATE_FULL: " << date << endl;
//        // cout << "DOSDATE: " << dosDate << endl;

//        // cout.unsetf(ios::hex);
//        // cout << "DAY: " << day << endl;
//        // cout << "MONTH: " << month << endl;
//        // cout << "YEAR: " << year << endl;

//        return QDate(year, month, day);
//    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    map<IDispatch*, QString> CComConnector::getVidZanyatosti()
    {
        map<IDispatch*, QString> l_vid;

        IDispatch* nextObject = NULL;
        IDispatch* listening  = NULL;
        IDispatch* vidZan     = NULL;
        execInvoke(L"Перечисления", L"", DISPATCH_PROPERTYGET, 0, connection, &m_resInvoke);
        listening = m_resInvoke.pdispVal;

        execInvoke(L"ВидыЗанятостиВОрганизации", L"", DISPATCH_PROPERTYGET, 0, listening, &m_resInvoke);
        vidZan = m_resInvoke.pdispVal;

        execInvoke(L"ОсновноеМестоРаботы", L"", DISPATCH_PROPERTYGET, 0, vidZan, &m_resInvoke);
        nextObject = m_resInvoke.pdispVal;
        l_vid.insert(pair<IDispatch*,QString>(nextObject, "ОсновноеМестоРаботы"));
        cout.setf(ios_base::hex);
        cout << "Z1 " << nextObject << endl;

        execInvoke(L"Совместительство", L"", DISPATCH_PROPERTYGET, 0, vidZan, &m_resInvoke);
        nextObject = m_resInvoke.pdispVal;
        l_vid.insert(pair<IDispatch*,QString>(nextObject, "Совместительство"));
        cout << "Z2 " << nextObject << endl;

        execInvoke(L"ВнутреннееСовместительство", L"", DISPATCH_PROPERTYGET, 0, vidZan, &m_resInvoke);
        nextObject = m_resInvoke.pdispVal;
        l_vid.insert(pair<IDispatch*,QString>(nextObject, "ВнутреннееСовместительство"));
        cout << "Z3 " << nextObject << endl;

        // nextObject->Release();
        vidZan->Release();
        listening->Release();

//        auto it = begin(l_vid);
//        for(;it != end(l_vid); it++)
//        {
//            (*it).first->Release();
//        }
        // m_parents.push_back(nextObject);
        return l_vid;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    map<IDispatch*, QString> CComConnector::getVidDogovorov()
    {
        map<IDispatch*, QString> l_vid;

        IDispatch* nextObject = NULL;
        IDispatch* listening  = NULL;
        IDispatch* vidZan     = NULL;
        execInvoke(L"Перечисления", L"", DISPATCH_PROPERTYGET, 0, connection, &m_resInvoke);
        listening = m_resInvoke.pdispVal;

        execInvoke(L"ВидыДоговоровСФизЛицами", L"", DISPATCH_PROPERTYGET, 0, listening, &m_resInvoke);
        vidZan = m_resInvoke.pdispVal;

        execInvoke(L"ТрудовойДоговор", L"", DISPATCH_PROPERTYGET, 0, vidZan, &m_resInvoke);
        nextObject = m_resInvoke.pdispVal;
        l_vid.insert(pair<IDispatch*,QString>(nextObject, "ТрудовойДоговор"));

        execInvoke(L"Подряда", L"", DISPATCH_PROPERTYGET, 0, vidZan, &m_resInvoke);
        nextObject = m_resInvoke.pdispVal;
        l_vid.insert(pair<IDispatch*,QString>(nextObject, "Подряда"));

        execInvoke(L"Авторский", L"", DISPATCH_PROPERTYGET, 0, vidZan, &m_resInvoke);
        nextObject = m_resInvoke.pdispVal;
        l_vid.insert(pair<IDispatch*,QString>(nextObject, "Авторский"));

        execInvoke(L"ДоговорУправленческий", L"", DISPATCH_PROPERTYGET, 0, vidZan, &m_resInvoke);
        nextObject = m_resInvoke.pdispVal;
        l_vid.insert(pair<IDispatch*,QString>(nextObject, "ДоговорУправленческий"));

        // nextObject->Release();
        vidZan->Release();
        listening->Release();

//        auto it = begin(l_vid);
//        for(;it != end(l_vid); it++)
//        {
//            (*it).first->Release();
//        }
        // m_parents.push_back(nextObject);
        return l_vid;
    }
}
