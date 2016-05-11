#ifndef COMCONNECTOR_H
#define COMCONNECTOR_H

#include <memory>
#include <map>

#include <windows.h>
#include <initguid.h>
#include <unknwn.h>
#include <comutil.h>
#include <string>
#include <iostream>
#include <vector>
#include <QStringList>
#include <person.h>
#include <QTextCodec>
#include <oleauto.h>
#include <QDateTime>
#include <QString>
#include <iomanip>

#include "logger.h"

using namespace ns_logger;
using namespace std;
using namespace db_connector;

namespace nsComConnector
{
    class CComConnector
    {
        public:
            CComConnector();
            virtual ~CComConnector();
            HRESULT makeConnect(string object, wstring conString);
            vector<CPerson*> getEmployeis(const QString& tabNumberPrefix);

            void HrMessage(const HRESULT hr);
            map<IDispatch*, QString> getVidZanyatosti();
            map<IDispatch*, QString> getVidDogovorov();

        private:
            QDate   oleDateToQDate(DATE date);
            string  toWinStr(BSTR str);
            string  toWinStr(QString str);
            QString convertTableNumber(const QString prefix, const string number);

        private:
            HRESULT initCom(string comServerName, IDispatch** outInstanceDispatch);
            HRESULT execInvoke(wstring methodName, wstring param, WORD objType, int paramNum, IDispatch* parent, VARIANT* inOutRes);
            HRESULT execInvokeRefParam(wstring methodName, IDispatch* param, WORD objType, int paramNum, IDispatch* parent, VARIANT* inOutRes);

            VARIANT m_resInvoke;
            VARIANT m_dogVar;

            IDispatch* instanceConnection = NULL;
            IDispatch* connection         = NULL;

            vector<IDispatch*> m_parents;
    };
}
#endif // COMCONNECTOR_H
