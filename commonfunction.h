#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H

#include <map>
#include <QString>

#include "comconnector.h"
#include "constansts.h"
#include "dbconnector.h"
#include "person.h"

using namespace db_connector;
using namespace nsComConnector;

extern map<int, QString>    firedDeps;
extern map<QString,wstring> connections;


struct pv
{
    const CPerson* toFind;

    bool operator()(const CPerson* other) const
    {
        if ((toFind->getTableNumber() == other->getTableNumber()) &&
            (toFind->getFullName() == other->getFullName()))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

void updateActivePerson(CPerson* person, CDbConnector* con, QString organizationId);
void updatePersonProcedure(CPerson* person, CDbConnector* con, vector<CPerson*> pList, QString organizationId);
void syncMain(CPerson* person, CDbConnector* con, CPerson* dbPerson, QString organizationId);
void syncDogovora(CPerson* person, CDbConnector* con, CPerson* dbPerson, QString organizationId);

// ---------- ---------- ---------- ---------- ---------- ----------
void addPerson(CPerson* person, QString organizationId);

// ---------- ---------- ---------- ---------- ---------- ----------
void updateActivePerson(CPerson* person, CDbConnector* con, QString organizationId);

// ---------- ---------- ---------- ---------- ---------- ----------
QString prepareMoveFileName(const QString& fileName);

// ---------- ---------- ---------- ---------- ---------- ----------
void updatePersonProcedure(CPerson* person, CDbConnector* con, vector<CPerson*> pList, QString organizationId);

// ---------- ---------- ---------- ---------- ---------- ----------
void syncMain(CPerson* person, CDbConnector* con, CPerson* dbPerson, QString organizationId);

// ---------- ---------- ---------- ---------- ---------- ----------
void syncDogovora(CPerson* person, CDbConnector* con, CPerson* dbPerson, QString organizationId);

// ---------- ---------- ---------- ---------- ---------- ----------
void updatePassivePerson(CPerson* person, CDbConnector* con);

// ---------- ---------- ---------- ---------- ---------- ----------
void loadAccessMatrix();

// ---------- ---------- ---------- ---------- ---------- ----------
void updatePersonProfiles();

// ---------- ---------- ---------- ---------- ---------- ----------
void updatePersonProfilesWithNull();

// ---------- ---------- ---------- ---------- ---------- ----------
void updateFiredPerson(CPerson* person, CDbConnector* con, QString organizationId);

// ---------- ---------- ---------- ---------- ---------- ----------
QString getFireDepartment(const QString& dep);

// ---------- ---------- ---------- ---------- ---------- ----------
void clearPersonVector(vector<CPerson*>& vec);

void syncPersonsListWithFired(CDbConnector& connection, vector<CPerson*>& persons, QString organizationId);

void syncPersons(int type = 0);
void syncCardNumLegosVSAnviz();
void syncPersonsList(CDbConnector& connection, vector<CPerson*>& persons, QString organizationId);

QString getPrefixById(QString id);

HRESULT initCom(string comServerName, IDispatch** outInstanceDispatch);
HRESULT execInvoke(wstring methodName, wstring param, WORD objType, int paramNum, IDispatch* parent, VARIANT* inOutRes);
void HrMessage(const HRESULT hr);
string bstrToStr(BSTR input);

string toWinStr(QString str);
QString fromWinStr(QString str);

void f1();
void f2();
void f3();
#endif // COMMONFUNCTION_H

