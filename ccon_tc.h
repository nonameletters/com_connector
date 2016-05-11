#ifndef CCON_TC_H
#define CCON_TC_H

#include <QObject>
#include <QTextCodec>
#include <QTextStream>
#include <QString>
#include <QSqlDatabase>
#include <QVariant>

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

            QString getNextId(const QString& table, const QString& column);

        private:
            void moveGenId(const QString& genName, const QString& val);
    };

}
#endif // CCON_TC_H
