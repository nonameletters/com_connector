#ifndef CCON_LEGOS_H
#define CCON_LEGOS_H

#include <QObject>
#include <QTextCodec>
#include <QTextStream>
#include <QString>
#include <QSqlDatabase>

#include "dbconnector.h"
#include "base_con.h"

namespace db_connector
{
    class CConLegos : public CDbConnector, CBaseCon
    {
        public:
            CConLegos();
            virtual ~CConLegos();

            virtual QString getConnectionName();
            virtual QString getConnectionResult() const;
            virtual vector<CPerson*> getPersonList();
            // virtual void addPerson(const CPerson* person);
            // virtual void addPersonList(const vector<CPerson*> list);
    };

}

#endif // CCON_LEGOS_H
