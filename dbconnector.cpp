#include "dbconnector.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "ccon_legos.h"
#include "ccon_iiko.h"
#include "ccon_tc.h"

namespace db_connector
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDbConnector::throwIfNull(CDbConnector* member, const char* mesg) const
    {
        if (member == nullptr)
        {
            throw CDbExcep(mesg);
        }

        return;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CDbConnector::getConnectionResult() const
    {
        throwIfNull(pm_connector, "In \"getConnectionResult()\" null member");
        return pm_connector->getConnectionResult();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CPerson*> CDbConnector::getPersonList()
    {
        throwIfNull(pm_connector, "In \"getPersonList()\" null member");
        return pm_connector->getPersonList();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CPerson*> CDbConnector::getPersonListByPerson(const CPerson* empl)
    {
        throwIfNull(pm_connector, "In \"getPersonListByPerson()\" null member");
        return pm_connector->getPersonListByPerson(empl);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CPerson*> CDbConnector::getPersonListByTabNumber(const QString& tabNum)
    {
        throwIfNull(pm_connector, "In \"getPersonListByTabNumber()\" null member");
        return pm_connector->getPersonListByTabNumber(tabNum);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CDbConnector::getCreateStatus() const
    {
        throwIfNull(pm_connector, "In \"getCreateStatus()\" null member");
        return QString("CreateStatus: NOT NULL");
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDbConnector::addPerson(const CPerson* person)
    {
        throwIfNull(pm_connector, "In \"addPerson()\" null member");
        pm_connector->addPerson(person);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDbConnector::addPerson(const CPerson* person, const QString& deptNum)
    {
        // cout << "BASE: addPerson" << endl;
        throwIfNull(pm_connector, "In \"addPerson()\" null member");
        pm_connector->addPerson(person, deptNum);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDbConnector::addPersonList(const vector<CPerson*> list)
    {
        throwIfNull(pm_connector, "In \"addPersonList()\" null member");
        pm_connector->addPersonList(list);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDbConnector::updateBirthDay(const QString& id, const QDate& date)
    {
        throwIfNull(pm_connector, "In \"updateBirthDay()\" null member");
        pm_connector->updateBirthDay(id, date);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDbConnector::updateSnils(const QString& id, const QString& snils)
    {
        throwIfNull(pm_connector, "In \"updateSnils()\" null member");
        pm_connector->updateSnils(id, snils);
    }
    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDbConnector::updateTableNumber(const QString& id, const QString& tabNum)
    {
        throwIfNull(pm_connector, "In \"updateTableNumber()\" null member");
        pm_connector->updateTableNumber(id, tabNum);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDbConnector::updateDepartmentNumber(const QString& id, const QString& departId)
    {
        throwIfNull(pm_connector, "In \"updateDepartmentNumber()\" null member");
        pm_connector->updateDepartmentNumber(id, departId);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDbConnector::updateKeyAndCommentByTableNumber(const QString& tabNum, const QString& key, const QString& comment)
    {
        throwIfNull(pm_connector, "In \"updateKeyAndCommentByTableNumber()\" null member");
        pm_connector->updateKeyAndCommentByTableNumber(tabNum, key, comment);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDbConnector::updateNameParams(const QString& id, const CPerson* person)
    {
        throwIfNull(pm_connector, "In \"updateNameParams()\" null member");
        pm_connector->updateNameParams(id, person);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDbConnector::updatePersonPosition(const QString& id, const CPerson* person)
    {
        throwIfNull(pm_connector, "In \"updatePersonPosition()\" null member");
        pm_connector->updatePersonPosition(id, person);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CDolgnost CDbConnector::getDolgnostById(const QString& id)
    {
        throwIfNull(pm_connector, "In \"getDolgnostById()\" null member");
        return pm_connector->getDolgnostById(id);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CDolgnost CDbConnector::getDolgnostByCode(const QString& code)
    {
        throwIfNull(pm_connector, "In \"getDolgnostByCode()\" null member");
        return pm_connector->getDolgnostByCode(code);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CDolgnost CDbConnector::createNewPosition(const CDolgnost& value)
    {
        throwIfNull(pm_connector, "In \"createNewPosition()\" null member");
        return pm_connector->createNewPosition(value);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CProfile CDbConnector::getProfileByCode(const QString& code)
    {
        throwIfNull(pm_connector, "In \"getProfileByCode()\" null member");
        return pm_connector->getProfileByCode(code);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CDoor> CDbConnector::getDoorsForProfile(const QString& id)
    {
        throwIfNull(pm_connector, "In \"getDoorsForProfile()\" null member");
        return pm_connector->getDoorsForProfile(id);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CProfile CDbConnector::getProfileById(const QString& id)
    {
        throwIfNull(pm_connector, "In \"getProfileById()\" null member");
        return pm_connector->getProfileById(id);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CDoor> CDbConnector::getAllDoors()
    {
        throwIfNull(pm_connector, "In \"getAllDoors()\" null member");
        return pm_connector->getAllDoors();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CProfile CDbConnector::createNewProfileByPerson(const CPerson* person)
    {
        throwIfNull(pm_connector, "In \"createNewProfileByPerson()\" null member");
        return pm_connector->createNewProfileByPerson(person);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CDoor> CDbConnector::createNewProfileDoors(const QString& pfId)
    {
        throwIfNull(pm_connector, "In \"createNewProfileDoors()\" null member");
        return pm_connector->createNewProfileDoors(pfId);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDbConnector::updatePersonProfile(const CPerson* person)
    {
        throwIfNull(pm_connector, "In \"updatePersonProfile()\" null member");
        return pm_connector->updatePersonProfile(person);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CDbConnector::CDbConnector(DBSOURCES type)
    {
        switch(type)
        {
            case LEGOS:
                pm_connector = new CConLegos();
                break;
            case IIKO:
                pm_connector = new CConIiko();
                break;
            case TC:
                pm_connector = new CConTC();
                break;
            default:
                throw CDbExcep("Bad type of source.");
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CDbConnector::CDbConnector()
    {
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CDbConnector::~CDbConnector()
    {
        if (pm_connector != nullptr)
        {
            // cout << "PM_CONNECTOR DELETED." << endl;
            delete pm_connector;
        }

    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    // Exception class
    // ---------- ---------- ---------- ---------- ---------- ----------
    CDbExcep::CDbExcep(const char* mes)
    {
        m_mesg = mes;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    const char* CDbExcep::what() const _GLIBCXX_USE_NOEXCEPT
    {
        return m_mesg.toStdString().c_str();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CDbExcep::~CDbExcep()
    {

    }
}
