#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QString>

#include <QDateTime>
#include <QObject>
#include <QTextCodec>
#include <QTextStream>

#include <memory>
#include <iostream>
#include <string>
#include <list>
#include <vector>

#include <exception>

#include "dolgnost.h"
#include "profile.h"
#include "door.h"
#include "person.h"
// #include "event.h"

using namespace std;

namespace db_connector
{
    enum DBSOURCES
    {
        LEGOS,
        IIKO,
        ANVIZ,
        TC
    };

    class CDbExcep : public exception
    {
        public:
            CDbExcep(const char* mes);
            virtual const char* what() const _GLIBCXX_USE_NOEXCEPT;
            ~CDbExcep();

        private:
            CDbExcep();
            QString m_mesg;
    };

    class CDbConnector
    {
        public:
            CDbConnector(DBSOURCES type);

            virtual ~CDbConnector();

            virtual QString getConnectionResult() const;
            virtual vector<CPerson*> getPersonList();
            virtual QString getCreateStatus() const;
            virtual void addPerson(const CPerson* person);
            virtual void addPerson(const CPerson* person, const QString& deptNum);
            virtual void addPersonList(const vector<CPerson*> list);
            virtual vector<CPerson*> getPersonListByPerson(const CPerson* person);
            virtual vector<CPerson*> getPersonListByTabNumber(const QString& tabNum);
            virtual void updateBirthDay(const QString& id, const QDate& date);
            virtual void updateSnils(const QString& id, const QString& snils);
            virtual void updateTableNumber(const QString& id, const QString& tabNum);
            virtual void updateDepartmentNumber(const QString& id, const QString& departId);
            virtual void updateKeyAndCommentByTableNumber(const QString& tabNum, const QString& key, const QString& comment);
            virtual void updateNameParams(const QString& id, const CPerson* person);
            virtual void updatePersonPosition(const QString& id, const CPerson* person);
            virtual CDolgnost getDolgnostById(const QString& id);
            virtual CDolgnost getDolgnostByCode(const QString& code);
            virtual CDolgnost createNewPosition(const CDolgnost& value);
            virtual CProfile getProfileByCode(const QString& code);
            virtual vector<CDoor> getDoorsForProfile(const QString& id);
            virtual CProfile getProfileById(const QString& id);
            virtual vector<CDoor> getAllDoors();
            virtual CProfile createNewProfileByPerson(const CPerson* person);
            virtual vector<CDoor> createNewProfileDoors(const QString& pfId);
            virtual void updatePersonProfile(const CPerson* person);
        protected:
            CDbConnector();

        private:
            void throwIfNull(CDbConnector* member, const char* mesg) const;

        private:
            CDbConnector* pm_connector = nullptr;


    };

}
#endif // DBCONNECTOR_H
