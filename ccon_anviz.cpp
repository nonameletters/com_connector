#include "ccon_anviz.h"
#include <QSqlQuery>
#include <QSqlRecord>

namespace db_connector
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    CConAnviz::CConAnviz()
    {
        setDriverName(QObject::tr("QODBC"));
        setDatabaseName(QObject::tr("Driver={SQL Server};Server=srv-acs\\sqlexpress;Database=anviz;"));
        setUsername(QObject::tr("anviz"));
        setPassword(QObject::tr("anviz"));
        connect();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CConAnviz::~CConAnviz()
    {

    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CConAnviz::getConnectionName()
    {
        return QString("ANVIZ");
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CConAnviz::getConnectionResult() const
    {
        return getConResult();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CPerson*> CConAnviz::getPersonList()
    {
        // connect();
        vector<CPerson*> l_list;
        CPerson* l_temp = new CPerson();
        l_list = getPersonListByPerson(l_temp);
        if (l_temp == nullptr)
        {
            delete l_temp;
        }
        return l_list;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CPerson*> CConAnviz::getPersonListByPerson(const CPerson* empl)
    {
        vector<CPerson*> l_list;
        if (empl == nullptr)
        {
            return l_list;
        }

        QString l_curQuery;
        l_curQuery = "SELECT Ui.USERID AS UID, Ui.BrithDay AS BDAY, Ui.Name AS NAME, DEPTID, DUTY, CARDNUM, CODE1C, St.snils AS SNILS FROM \
                      dbo.userinfo AS Ui LEFT OUTER JOIN dbo.snilsTable AS St ON Ui.USERID = St.userId WHERE \
                      Ui.Name LIKE '%" + empl->getFullName().trimmed() +"%' ORDER BY Ui.Name;";

        QSqlQuery l_query(getDatabase());

        //QTextStream l_out(stdout);

        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            return l_list;
        }

        QSqlRecord l_rec = l_query.record();

        int l_id            = l_rec.indexOf("UID");
        int l_birthDay      = l_rec.indexOf("BDAY");
        int l_name          = l_rec.indexOf("NAME");
        int l_cardnum       = l_rec.indexOf("CARDNUM");
        int l_code1c        = l_rec.indexOf("CODE1C");
        int l_snils         = l_rec.indexOf("SNILS");
        int l_deptId        = l_rec.indexOf("DEPTID");


        while (l_query.next())
        {
            QString lv_id         = l_query.value(l_id).toString();
            QDate   lv_birthDay   = l_query.value(l_birthDay).toDate();
            QString lv_name       = l_query.value(l_name).toString();
            QStringList l_fio     = lv_name.split(" ");

            QString l_fName;
            QString l_mName;
            QString l_lName;

            if (l_fio.size() == 1)
            {
                l_fName = l_fio.at(0);
            }
            else if (l_fio.size() == 2)
            {
                l_fName = l_fio.at(0);
                l_mName = l_fio.at(1);
            }
            else if (l_fio.size() == 3)
            {
                l_fName = l_fio.at(0);
                l_mName = l_fio.at(1);
                l_lName = l_fio.at(2);
            }

            QString lv_cardNum    = l_query.value(l_cardnum).toString();
            QString lv_code1c     = l_query.value(l_code1c).toString();
            QString lv_snils      = l_query.value(l_snils).toString();
            QString lv_deptId     = l_query.value(l_deptId).toString();

            CPerson* l_tPer = new CPerson(lv_id, lv_deptId, l_fName, l_mName, l_lName, lv_cardNum, lv_code1c, lv_snils);
            l_tPer->setBirthDay(lv_birthDay);
            l_tPer->setRecordId(lv_id);
            l_list.push_back(l_tPer);
        }

        // close();
        return l_list;

    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConAnviz::addPerson(const CPerson* person, QString deptNum)
    {
        // Если таблица пустая, то надо вставить пустую запись с ID = 1
        // что бы потом получать следующие ID
        QString l_fullName = person->getFirstName().trimmed() + " " + \
                             person->getMidleName().trimmed() + " " + \
                             person->getLastName().trimmed();

        l_fullName.trimmed();
        QString l_birthDay = person->getBirthDay().toString("yyyy-dd-MM 00:00:00");
        QString l_mes = QString("TEST DATE: ") + l_birthDay;
        CLogger::getInstance()->log(l_mes.toStdString().c_str());
        // cout << "TEST DATE: "   << l_birthDay.toStdString().c_str() << endl;
        QString l_curQuery;
        l_curQuery = "DECLARE @nId varchar(20) = (SELECT MAX(Cast(UserId AS int)) + 1 FROM UserInfo);";
        l_curQuery = l_curQuery + "INSERT INTO dbo.UserInfo (UserId, Name, DeptId, BrithDay, IDCard, isAtt, isOverTime, isRest, Code1C) " + \
                                  "VALUES " + \
                                  "(@nId, '" + l_fullName + "', " + deptNum + ", '" + l_birthDay + "', '" + person->getTableNumber() + "', 1, 1, 1, '" + person->getTableNumber() + "');";
        l_curQuery = l_curQuery + "INSERT INTO dbo.SnilsTable (UserId, Snils) VALUES (@nId, '" + person->getSnils() + "');";

        // cout << l_curQuery.toStdString().c_str() << endl;
        QSqlQuery l_query(getDatabase());


        if (l_query.exec(l_curQuery) == false)
        {
            CLogger::getInstance()->log(l_curQuery.toStdString().c_str());
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }
        // close();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConAnviz::addPersonList(const vector<CPerson*> list)
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

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConAnviz::updateBirthDay(QString recordId, QDate birthDay)
    {
        QString l_curQuery;
        QString l_bDay = birthDay.toString("yyyy-dd-MM");
        l_curQuery = "UPDATE UserInfo SET BrithDay = '" + l_bDay + " 00:00:00' WHERE UserId = '" + recordId + "'";

        QSqlQuery l_query(getDatabase());


        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConAnviz::updateSnils(QString recordId, QString snils)
    {

        QString l_curQuery;
        l_curQuery = "SELECT userId, Snils FROM SnilsTable WHERE userId = '" + recordId + "'";

        QSqlQuery l_query(getDatabase());

        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            return;
        }

        QString l_recordId;

        if (l_query.size() > 0)
        {
            QSqlRecord l_rec = l_query.record();

            int l_id         = l_rec.indexOf("userId");

            while (l_query.next())
            {
                l_recordId    = l_query.value(l_id).toString();
            }
            // Запись с таким ИД существует, обновляем СНИЛС
            l_curQuery = "UPDATE SnilsTable SET snils = '" + snils +"' WHERE userId = '" + recordId + "'";
            l_query.exec(l_curQuery);
        }
        else
        {
            l_recordId = recordId;
            // Записи не существует. Добавляем новую запись о СНИЛС
            l_curQuery = "INSERT INTO SnilsTable(userId, Snils) VALUES ('" + l_recordId + "', '" + snils + "')";
            l_query.exec(l_curQuery);
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConAnviz::updateTableNumber(QString recordId, QString number)
    {
        QString l_curQuery;
        l_curQuery = "UPDATE UserInfo SET Code1C = '" + number + "', IDCard = '" + number + "' WHERE UserId = '" + recordId + "'";

        QSqlQuery l_query(getDatabase());


        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConAnviz::updateDeptId(QString recordId, QString depId)
    {
        QString l_curQuery;
        l_curQuery = "UPDATE UserInfo SET DeptId = '" + depId + "' WHERE UserId = '" + recordId + "'";

        QSqlQuery l_query(getDatabase());


        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConAnviz::updateKeyByTableNumber(QString tableNumber, QString key)
    {
        if (tableNumber.isEmpty())
        {
            return;
        }

        // Some numbers could starts with
        // ДЭН - сотрудники Денир
        // ТД  - сотрудники Торговый Дом
        // ФБ  - сотрудники Фабрика
        // Э   - сотрудники Эксцельсиор

        if (tableNumber.startsWith("ДЭН") || tableNumber.startsWith("ТД") || \
            tableNumber.startsWith("ФБ")  || tableNumber.startsWith("Э"))
        {
            // do nothing table number is OK;
        }
        else
        {
            int l_len = tableNumber.length();
            int l_dif = 10 - l_len;
            QString l_zeroStr;
            for (int i = 0; i < l_dif; i++)
            {
                l_zeroStr = l_zeroStr + "0";
            }
            tableNumber = l_zeroStr + tableNumber;
        }

        QString l_curQuery;
        l_curQuery = "UPDATE UserInfo SET CardNum = '" + key + "' WHERE Code1C = '" + tableNumber + "'";

        QSqlQuery l_query(getDatabase());


        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConAnviz::updateKeyAndCommentByTableNumber(QString tableNumber, QString key, QString comment)
    {
        if (tableNumber.isEmpty())
        {
            return;
        }

        // Some numbers could starts with
        // ДЭН - сотрудники Денир
        // ТД  - сотрудники Торговый Дом
        // ФБ  - сотрудники Фабрика
        // Э   - сотрудники Эксцельсиор

        if (tableNumber.startsWith("ДЭН") || tableNumber.startsWith("ТД") || \
            tableNumber.startsWith("ФБ")  || tableNumber.startsWith("Э"))
        {
            // do nothing table number is OK;
        }
        else
        {
            int l_len = tableNumber.length();
            int l_dif = 10 - l_len;
            QString l_zeroStr;
            for (int i = 0; i < l_dif; i++)
            {
                l_zeroStr = l_zeroStr + "0";
            }
            tableNumber = l_zeroStr + tableNumber;
        }

        comment = comment.trimmed();
        QString l_curQuery;
        l_curQuery = "UPDATE UserInfo SET CardNum = '" + key + "', NativePlace = '" + comment + "' WHERE Code1C = '" + tableNumber + "'";

        QSqlQuery l_query(getDatabase());


        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConAnviz::closeConnection()
    {
        close();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
//    void CConAnviz::updateBirthDaySnils(QString recordId, QDate birthDay, QString snils)
//    {
//        throwIfNull(pm_connector, "In \"updateBirthDaySnils()\" null member");
//        pm_connector->updateBirthDaySnils(recordId, birthDay, snils);
//    }

    // ---------- ---------- ---------- ---------- ---------- ----------
//    void CConAnviz::updateTableNumberSnils(QString recordId, QString number, QString snils)
//    {
//        throwIfNull(pm_connector, "In \"updateTableNumberSnils()\" null member");
//        pm_connector->updateTableNumberSnils(recordId, number, snils);
//    }
}


