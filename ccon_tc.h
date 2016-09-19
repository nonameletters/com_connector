#ifndef CCON_TC_H
#define CCON_TC_H

#include <QObject>
#include <QTextCodec>
#include <QTextStream>
#include <QString>
#include <QSqlDatabase>
#include <QVariant>
#include <memory>

#include "dbconnector.h"
#include "base_con.h"

namespace db_connector
{
    class CConTC : public CDbConnector, CBaseCon
    {
        public:
            CConTC();
            virtual ~CConTC();

            virtual QString getConnectionName();
            virtual QString getConnectionResult() const;
            virtual vector<CPerson*> getPersonList();
            virtual vector<CPerson*> getPersonListWithNullProfile();
            virtual vector<CPerson*> getPersonListByTabNumber(const QString& tabNum);
            virtual void addPerson(const CPerson* person);
            virtual void addPerson(const CPerson* person, const QString& deptNum);
            virtual void addPersonList(const vector<CPerson*> list);
            virtual void updatePerson(const CPerson* person);
            virtual vector<CPerson*> getPersonListByPerson(const CPerson* person);
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
            virtual void addLineToAccessMatrix(const vector<string>& row);
            virtual vector<CDoor> getDoorsByDolgnost(const QString& dolj);
            virtual bool isFired(const CPerson* person);
            virtual void updatePersonAccessDoors();
            virtual void closeDoorsForProfile(const CProfile& profile);

            QString getNextId(const QString& table, const QString& column);
            virtual void clearAccessMatrix();

        private:
            void moveGenId(const QString& genName, const QString& val);
            void updateCurDoorEnterMode(const QString& profileId, const QString& doorId, const QString& enterMode);
            void addNewDoorToProfileWithEnterMode(const QString& profileId, const QString& doorId, const QString& enterMode);
    };

}
#endif // CCON_TC_H
