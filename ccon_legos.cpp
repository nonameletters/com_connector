#include <QSqlQuery>
#include <QSqlRecord>

#include "ccon_legos.h"
namespace db_connector
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    CConLegos::CConLegos()
    {
        setDriverName(QObject::tr("QODBC"));
        setDatabaseName(QObject::tr("Driver={SQL Server};Server=192.168.11.109\\Legos;Database=FLEX_DB;"));
        setUsername(QObject::tr("sa"));
        setPassword(QObject::tr("hizhechu"));
        connect();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CConLegos::~CConLegos()
    {

    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CConLegos::getConnectionName()
    {
        return QString("LEGOS");
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CConLegos::getConnectionResult() const
    {
        return getConResult();
    }
    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CPerson*> CConLegos::getPersonList()
    {
        // connect();
        vector<CPerson*> l_list;

        QString l_curQuery;
        l_curQuery = "select SID, GROUP_SID, FIRST_NAME, MIDDLE_NAME, LAST_NAME, KEY_NUMBER, TABLE_NUMBER, SHORT_COMMENT FROM dbo.users ORDER BY LAST_NAME";


        QSqlQuery l_query(getDatabase());

        //QTextStream l_out(stdout);

        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            return l_list;
        }

        QSqlRecord l_rec = l_query.record();

        int l_sidInd         = l_rec.indexOf("SID");
        int l_groupSidInd    = l_rec.indexOf("GROUP_SID");
        int l_firstNameInd   = l_rec.indexOf("FIRST_NAME");
        int l_midleNameInd   = l_rec.indexOf("MIDDLE_NAME");
        int l_lastNameInd    = l_rec.indexOf("LAST_NAME");
        int l_keyNumberInd   = l_rec.indexOf("KEY_NUMBER");
        int l_tableNumberInd = l_rec.indexOf("TABLE_NUMBER");
        int l_commentInd     = l_rec.indexOf("SHORT_COMMENT");

        while (l_query.next())
        {
            QString l_sidVal         = l_query.value(l_sidInd).toString();
            QString l_groupSidVal    = l_query.value(l_groupSidInd).toString();
            QString l_firstNameVal   = l_query.value(l_firstNameInd).toString();
            QString l_midleNameVal   = l_query.value(l_midleNameInd).toString();
            QString l_lastNameVal    = l_query.value(l_lastNameInd).toString();
            QString l_keyNumberVal   = l_query.value(l_keyNumberInd).toString();
            QString l_tableNumberVal = l_query.value(l_tableNumberInd).toString();
            QString l_comment        = l_query.value(l_commentInd).toString();

            CPerson* l_person = new CPerson(l_sidVal, l_groupSidVal, l_firstNameVal, l_midleNameVal, l_lastNameVal, l_keyNumberVal, l_tableNumberVal);
            l_person->setComment(l_comment);
            l_list.push_back(l_person);
        }

        // close();
        return l_list;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    //void CConLegos::addPerson(const CPerson* person)
    //{
        // connect();

        // close();
    //}

    // ---------- ---------- ---------- ---------- ---------- ----------
    //void CConLegos::addPersonList(const vector<CPerson*> list)
    //{

    //}
}
