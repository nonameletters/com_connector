#include <QSqlQuery>
#include <QSqlRecord>

#include "ccon_iiko.h"

namespace db_connector
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    CConIiko::CConIiko()
    {
        setDriverName(QObject::tr("QODBC"));
        setDatabaseName(QObject::tr("Driver={SQL Server};Server=SRV-IIKO\\RESTO;Database=iikoCard;"));
        setUsername(QObject::tr("db_admin"));
        setPassword(QObject::tr("41TlkM37"));
        connect();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CConIiko::~CConIiko()
    {

    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CConIiko::getConnectionName()
    {
        return QString("IIKO");
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CConIiko::getConnectionResult() const
    {
        return getConResult();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CPerson*> CConIiko::getPersonList()
    {
        // connect();
        vector<CPerson*> l_list;

        QString l_curQuery;
        l_curQuery = "SELECT ID, REVISION, ISDELETED, WHENDELETED, LEGALPOSITION, NAME, EMAIL, CODE, ORGANIZATIONID FROM dbo.Client";

        QSqlQuery l_query(getDatabase());

        //QTextStream l_out(stdout);

        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            return l_list;
        }

        QSqlRecord l_rec = l_query.record();

        int l_id            = l_rec.indexOf("ID");
        int l_revision      = l_rec.indexOf("REVISION");
        // int l_isDeleted     = l_rec.indexOf("ISDELETED");
        // int l_whenDeleted   = l_rec.indexOf("WHENDELETED");
        // int l_legalPosition = l_rec.indexOf("LEGALPOSITION");
        int l_name          = l_rec.indexOf("NAME");
        // int l_email         = l_rec.indexOf("EMAIL");
        int l_code          = l_rec.indexOf("CODE");
        // int l_orgId         = l_rec.indexOf("ORGANISATIONID");

        while (l_query.next())
        {
            QString lv_id         = l_query.value(l_id).toString();
            QString lv_revision    = l_query.value(l_revision).toString();
            // QString lv_isDeleted   = l_query.value(l_isDeleted).toString();
            // QString lv_whenDeleted   = l_query.value(l_whenDeleted).toString();
            // QString lv_legalPosition    = l_query.value(l_legalPosition).toString();
            QString lv_name   = l_query.value(l_name).toString();
            // QString lv_email = l_query.value(l_email).toString();
            QString lv_code = l_query.value(l_code).toString();
            // QString lv_orgId = l_query.value(l_orgId).toString();

            l_list.push_back(new CPerson(lv_id, lv_revision, lv_name, "", "", "", lv_code));
        }

        // close();
        return l_list;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConIiko::addPerson(const CPerson* person)
    {
        // connect();
        QString l_fullName = person->getLastName() + " " + \
                             person->getFirstName() + " " + \
                             person->getMidleName();

        QString l_curQuery;
        l_curQuery = "INSERT INTO dbo.Client(ID, ISDELETED, LEGALPOSITION, NAME, CODE) VALUES (NEWID(), 0, 0, '" + l_fullName +"', '" + person->getTableNumber()+ "')";

        QSqlQuery l_query(getDatabase());


        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }
        // close();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConIiko::addPersonList(const vector<CPerson*> list)
    {
        // connect();
        vector<CPerson*>::const_iterator it = list.begin();
        for (;it != list.end(); it++)
        {
            QString l_fullName = (*it)->getLastName() + " " + \
                                 (*it)->getFirstName() + " " + \
                                 (*it)->getMidleName();

            QString l_curQuery;
            l_curQuery = "INSERT INTO dbo.Client(ID, ISDELETED, LEGALPOSITION, NAME, CODE) VALUES (NEWID(), 0, 0, '" + l_fullName +"', '" + (*it)->getTableNumber()+ "')";

            QSqlQuery l_query(getDatabase());


            if (l_query.exec(l_curQuery) == false)
            {
                cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
                cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            }
        }
        // close();
    }
}

