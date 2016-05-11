#include <QSqlQuery>
#include <QSqlRecord>

#include "ccon_tc.h"

namespace db_connector
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    CConTC::CConTC()
    {
        setDriverName(QObject::tr("QODBC"));
        setDatabaseName(QObject::tr("DRIVER=Firebird/InterBase(r) driver;DBNAME=10.10.11.155/3053:E:\\BASE\\OKO.FDB"));
        setUsername(QObject::tr("SYSDBA"));
        setPassword(QObject::tr("masterkey"));
        connect();
        cout << "TC created" << endl;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CConTC::~CConTC()
    {

    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CConTC::getConnectionName()
    {
        return QString("TC");
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CConTC::getConnectionResult() const
    {
        return getConResult();
    }
    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CPerson*> CConTC::getPersonList()
    {
        return getPersonListByTabNumber("");
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CPerson*> CConTC::getPersonListByTabNumber(const QString& tabNum)
    {
        // connect();
        vector<CPerson*> l_list;

        // QString l_curQuery;
        // l_curQuery = "select UID, USERGROUP, FIRSTNAME, MIDNAME, SNAME, FULLNAME, SHEDCARDKEY, TABNUM, BITHDAY, USER_COMMENT FROM users WHERE TABNUM LIKE '%" + tabNum + "%' ORDER BY FULLNAME";


        QSqlQuery l_query(getDatabase());
        l_query.prepare("select UID, USERGROUP, FIRSTNAME, MIDNAME, SNAME, FULLNAME, SHEDCARDKEY, TABNUM, BITHDAY, USER_COMMENT FROM users WHERE TABNUM LIKE :tabNum ORDER BY FULLNAME");
        l_query.bindValue(":tabNum", tabNum);

        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            return l_list;
        }

        QSqlRecord l_rec = l_query.record();

        int l_sidInd         = l_rec.indexOf("UID");
        int l_groupSidInd    = l_rec.indexOf("USERGROUP");
        int l_firstNameInd   = l_rec.indexOf("FIRSTNAME");
        int l_midleNameInd   = l_rec.indexOf("MIDNAME");
        int l_lastNameInd    = l_rec.indexOf("SNAME");
        // int l_fullNameInd    = l_rec.indexOf("FULLNAME");
        int l_keyNumberInd   = l_rec.indexOf("SHEDCARDKEY");
        int l_tableNumberInd = l_rec.indexOf("TABNUM");
        int l_birthDayInd    = l_rec.indexOf("BITHDAY");
        int l_commentInd     = l_rec.indexOf("USER_COMMENT");

        while (l_query.next())
        {
            QString l_sidVal         = l_query.value(l_sidInd).toString();
            QString l_groupSidVal    = l_query.value(l_groupSidInd).toString();
            QString l_firstNameVal   = l_query.value(l_firstNameInd).toString();
            QString l_midleNameVal   = l_query.value(l_midleNameInd).toString();
            QString l_lastNameVal    = l_query.value(l_lastNameInd).toString();
            // QString l_fullNameVal    = l_query.value(l_fullNameInd).toString();
            QString l_keyNumberVal   = l_query.value(l_keyNumberInd).toString();
            QString l_tableNumberVal = l_query.value(l_tableNumberInd).toString();
            QDate   l_birthDate      = l_query.value(l_birthDayInd).toDate();
            QString l_commentVal     = l_query.value(l_commentInd).toString();

            CPerson* l_per = new CPerson(l_sidVal, l_groupSidVal, l_firstNameVal, l_midleNameVal, l_lastNameVal, l_keyNumberVal, l_tableNumberVal);
            l_per->setBirthDay(l_birthDate);
            l_per->setComment(l_commentVal);
            l_list.push_back(l_per);
        }

        // close();
        return l_list;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::addPerson(const CPerson* person)
    {
        addPerson(person, "");
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::addPerson(const CPerson* person, const QString& deptNum)
    {
        //cout << "CConTC: addPerson" << endl;
        QString l_uId = getNextId("USERS", "UID");

        QSqlQuery l_query(getDatabase());
        l_query.prepare("INSERT INTO USERS (uid, DEPART, firstname, sname, midname, fullname, tabnum, shedcardkey, bithday, startdate, add_profile_start, user_comment, device_uid, autopass) "
                        "VALUES (:uid, :depart, :fname, :sname, :mname, :fioName, :tnum, :key, :bday, :sdate, :adate, :ucomment, :devUid, :apass)");
        l_query.bindValue(":uid",      l_uId);
        l_query.bindValue(":depart",   deptNum);
        l_query.bindValue(":fname",    person->getFirstName());
        l_query.bindValue(":sname",    person->getMidleName());
        l_query.bindValue(":mname",    person->getLastName());
        l_query.bindValue(":fioName",  person->getFullName());
        l_query.bindValue(":tnum",     person->getTableNumber());
        l_query.bindValue(":key",      person->getKeyNumber());
        l_query.bindValue(":bday",     person->getBirthDay());
        l_query.bindValue(":sdate",    QDate::currentDate());
        l_query.bindValue(":adate",    QDate::currentDate());
        l_query.bindValue(":ucomment", person->getComment());
        l_query.bindValue(":devUid",   l_uId);
        l_query.bindValue(":apass",    person->getSnils());


        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            return;
        }

        moveGenId("USERS_DEVICE_UID_GEN", l_uId);
        moveGenId("USERS_GEN", l_uId);
        moveGenId("USERS_PHOTO_GEN", l_uId);

        QByteArray l_photo = person->getPhoto();
        if (l_photo.size() <= 10)
        {
            return;
        }

        QString l_pId = getNextId("USERS_PHOTO", "PHOTOID");
        l_query.prepare("INSERT INTO USERS_PHOTO (PHOTOID, USERID, PHOTO) "
                        "VALUES (:pId, :uId, :photo)");
        l_query.bindValue(":pId", l_pId);
        l_query.bindValue(":uId", l_uId);
        l_query.bindValue(":photo", l_photo);

        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            return;
        }

        return;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::addPersonList(const vector<CPerson*> list)
    {
        auto it = begin(list);
        for(; it != end(list); it++)
        {
            addPerson(*it);
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::updatePerson(const CPerson* person)
    {
        if (person == nullptr)
        {
            return;
        }

        QSqlQuery l_query(getDatabase());

        l_query.prepare("UPDATE users SET FIRSTNAME = :fname, SNAME = :sname, MIDNAME = :mname, FULLNAME = :flName, SHEDCARDKEY = :key, BITHDAY = :bday, DEPART = :dpart, USER_COMMENT = :ucomment WHERE TABNUM = :tnum");
        l_query.bindValue(":fname",    person->getFirstName());
        l_query.bindValue(":sname",    person->getMidleName());
        l_query.bindValue(":mname",    person->getLastName());
        l_query.bindValue(":flName",   person->getFullName());
        l_query.bindValue(":key",      person->getKeyNumber());
        l_query.bindValue(":bday",     person->getBirthDay());
        l_query.bindValue(":dpart",    person->getGroupSid().toInt());
        l_query.bindValue(":ucomment", person->getComment());
        l_query.bindValue(":tnum",     person->getTableNumber());

        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            return;
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CPerson*> CConTC::getPersonListByPerson(const CPerson* person)
    {
        vector<CPerson*> l_list;
        if (person == nullptr)
        {
            return l_list;
        }

        QSqlQuery l_query(getDatabase());

        QString l_valFullName = "%" + person->getFullName() + "%";
        l_query.prepare("SELECT UID, DEPART, FIRSTNAME, MIDNAME, SNAME, FULLNAME, BITHDAY, TABNUM, SHEDCARDKEY, AUTOPASS, USER_COMMENT "
                        "FROM USERS WHERE FULLNAME LIKE :fullName ORDER BY FULLNAME");
        l_query.bindValue(":fullName", l_valFullName);

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            return l_list;
        }

        QSqlRecord l_rec = l_query.record();

        int l_sidInd         = l_rec.indexOf("UID");
        int l_groupSidInd    = l_rec.indexOf("DEPART");
        int l_firstNameInd   = l_rec.indexOf("FIRSTNAME");
        int l_midleNameInd   = l_rec.indexOf("MIDNAME");
        int l_lastNameInd    = l_rec.indexOf("SNAME");
        // int l_fullNameInd    = l_rec.indexOf("FULLNAME");
        int l_keyNumberInd   = l_rec.indexOf("SHEDCARDKEY");
        int l_tableNumberInd = l_rec.indexOf("TABNUM");
        int l_birthDayInd    = l_rec.indexOf("BITHDAY");
        int l_snils          = l_rec.indexOf("AUTOPASS");
        int l_commentInd     = l_rec.indexOf("USER_COMMENT");

        while (l_query.next())
        {
            QString l_sidVal         = l_query.value(l_sidInd).toString();
            QString l_groupSidVal    = l_query.value(l_groupSidInd).toString();
            QString l_firstNameVal   = l_query.value(l_firstNameInd).toString();
            QString l_midleNameVal   = l_query.value(l_midleNameInd).toString();
            QString l_lastNameVal    = l_query.value(l_lastNameInd).toString();
            // QString l_fullNameVal    = l_query.value(l_fullNameInd).toString();
            QString l_keyNumberVal   = l_query.value(l_keyNumberInd).toString();
            QString l_tableNumberVal = l_query.value(l_tableNumberInd).toString();
            QDate   l_birthDate      = l_query.value(l_birthDayInd).toDate();
            QString l_valSnils       = l_query.value(l_snils).toString();
            QString l_commentVal     = l_query.value(l_commentInd).toString();

            CPerson* l_per = new CPerson(l_sidVal, l_groupSidVal, l_firstNameVal, l_midleNameVal, l_lastNameVal, l_keyNumberVal, l_tableNumberVal);
            l_per->setBirthDay(l_birthDate);
            l_per->setSnils(l_valSnils);
            l_per->setComment(l_commentVal);
            l_list.push_back(l_per);
        }

        return l_list;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::updateBirthDay(const QString& id, const QDate& date)
    {
        if (id.isEmpty() || (!date.isValid()))
        {
            return;
        }

        QSqlQuery l_query(getDatabase());

        l_query.prepare("UPDATE USERS SET BITHDAY = :bday "
                        "WHERE UID = :id");
        l_query.bindValue(":bday", date);
        l_query.bindValue(":id", id);

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }

        return;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::updateSnils(const QString& id, const QString& snils)
    {
        // Не проверям СНИЛС для возможности обнуления поля.
        if (id.isEmpty())
        {
            return;
        }

        QSqlQuery l_query(getDatabase());

        l_query.prepare("UPDATE USERS SET AUTOPASS = :snils "
                        "WHERE UID = :id");
        l_query.bindValue(":snils", snils);
        l_query.bindValue(":id", id);

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }

        return;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::updateTableNumber(const QString& id, const QString& tabNum)
    {
        // Не проверям ТАБЕЛЬНЫЙ НОМЕР для возможности обнуления поля.
        if (id.isEmpty())
        {
            return;
        }

        QSqlQuery l_query(getDatabase());

        l_query.prepare("UPDATE USERS SET TABNUM = :tabNum "
                        "WHERE UID = :id");
        l_query.bindValue(":tabNum", tabNum);
        l_query.bindValue(":id", id);

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }

        return;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::updateDepartmentNumber(const QString& id, const QString& departId)
    {
        // Не проверям ТАБЕЛЬНЫЙ НОМЕР для возможности обнуления поля.
        if (id.isEmpty())
        {
            return;
        }

        QSqlQuery l_query(getDatabase());

        l_query.prepare("UPDATE USERS SET DEPART = :depart "
                        "WHERE UID = :id");
        l_query.bindValue(":depart", departId);
        l_query.bindValue(":id", id);

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }

        return;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::updateKeyAndCommentByTableNumber(const QString& tabNum, const QString& key, const QString& comment)
    {
        // Не проверям КЛЮЧ И КОММЕНТЫ   для возможности обнуления поля.
        if (tabNum.isEmpty())
        {
            return;
        }

        QSqlQuery l_query(getDatabase());

        l_query.prepare("UPDATE USERS SET  SHEDCARDKEY = :key, USER_COMMENT= :comment "
                        "WHERE TABNUM = :tabNum");
        l_query.bindValue(":key", key);
        l_query.bindValue(":comment", comment);
        l_query.bindValue(":tabNum", tabNum);

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }

        return;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CConTC::getNextId(const QString& table, const QString& column)
    {
        QString l_curQuery;
        l_curQuery = "SELECT MAX(" +column + ") + 1 AS ID FROM " + table;

        QSqlQuery l_query(getDatabase());

        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            return QString();
        }

        QSqlRecord l_rec = l_query.record();

        int l_id = l_rec.indexOf("ID");

        QString l_result;
        while (l_query.next())
        {
            l_result = l_query.value(l_id).toString();

        }

        return l_result;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::moveGenId(const QString& genName, const QString& val)
    {
        QString l_curQuery;
        l_curQuery = "SET GENERATOR " + genName + " TO " + val + ";";

        QSqlQuery l_query(getDatabase());

        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }
    }
}
