#ifndef CCON_IIKO_H
#define CCON_IIKO_H

#include <QObject>
#include <QSqlDatabase>

#include "dbconnector.h"
#include "base_con.h"

namespace db_connector
{
    class CConIiko : public CDbConnector, CBaseCon
    {
        public:
            CConIiko();
            virtual ~CConIiko();

            virtual QString getConnectionName();
            virtual QString getConnectionResult() const;
            virtual vector<CPerson*> getPersonList();
            virtual void addPerson(const CPerson* person);
            virtual void addPersonList(const vector<CPerson*> list);
    };

}
#endif // CCON_IIKO_H
