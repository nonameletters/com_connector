#ifndef CCON_ANVIZ_H
#define CCON_ANVIZ_H

#include <QObject>
#include <QSqlDatabase>

#include "dbconnector.h"
#include "base_con.h"
#include "logger.h"

using namespace ns_logger;

namespace db_connector
{
    class CConAnviz : public CDbConnector, CBaseCon
    {
        public:
            CConAnviz();
            virtual ~CConAnviz();

            virtual QString getConnectionName();
            virtual QString getConnectionResult() const;
            virtual vector<CPerson*> getPersonList();
            virtual vector<CPerson*> getPersonListByPerson(const CPerson* empl); // Ищем сотруника по ФИО
            virtual void addPerson(const CPerson* person, QString deptNum);
            virtual void addPersonList(const vector<CPerson*> list);
            virtual void updateBirthDay(QString recordId, QDate birthDay);
            virtual void updateSnils(QString recordId, QString snils);
            virtual void updateTableNumber(QString recordId, QString number);
            virtual void updateDeptId(QString recordId, QString depId);
            virtual void updateKeyByTableNumber(QString tableNumber, QString key);
            virtual void updateKeyAndCommentByTableNumber(QString tableNumber, QString key, QString comment);
            virtual void closeConnection();

            // Два метода для ускорения работы
            // при последовательном вызове работа замедляется
//            virtual void updateBirthDaySnils(QString recordId, QDate birthDay, QString snils);
//            virtual void updateTableNumberSnils(QString recordId, QString number, QString snils);
    };

}

#endif // CCON_ANVIZ_H
