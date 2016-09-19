#include <QSqlQuery>
#include <QSqlRecord>

#include "ccon_tc.h"

namespace db_connector
{
    struct dc
    {
        CDoor toFind;

        bool operator()(const CDoor other) const
        {
            if (toFind.getId() == other.getId())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    };

    // ---------- ---------- ---------- ---------- ---------- ----------
    CConTC::CConTC()
    {
        setDriverName(QObject::tr("QODBC"));
        setDatabaseName(QObject::tr("DRIVER=Firebird/InterBase(r) driver;DBNAME=10.10.11.155/3053:E:\\BASE\\OKO.FDB"));
        setUsername(QObject::tr("SYSDBA"));
        setPassword(QObject::tr("masterkey"));
        connect();
//        cout << "TC created" << endl;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CConTC::~CConTC()
    {
//        cout << "TC DESTRUCTOR" << endl;
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
        vector<CPerson*> l_list;

        QSqlQuery l_query(getDatabase());
        l_query.prepare("select UID, USERGROUP, FIRSTNAME, MIDNAME, SNAME, FULLNAME, SHEDCARDKEY, TABNUM, BITHDAY, AUTOPASS, DOLJ, USER_COMMENT, BT_ENTER_PROFILE FROM users ORDER BY FULLNAME");

        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
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
        int l_snils          = l_rec.indexOf("AUTOPASS");
        int l_dolj           = l_rec.indexOf("DOLJ");
        int l_commentInd     = l_rec.indexOf("USER_COMMENT");
        int l_profId         = l_rec.indexOf("BT_ENTER_PROFILE");

        int count = 0;

        while (l_query.next())
        {
            count++;

//            if (count >= 20)
//            {
//                return l_list;
//            }
            // cout << "Resiving persons from DB: " << count << endl;
            QString l_sidVal         = l_query.value(l_sidInd).toString();
            QString l_groupSidVal    = l_query.value(l_groupSidInd).toString();
            QString l_firstNameVal   = l_query.value(l_firstNameInd).toString();
            QString l_midleNameVal   = l_query.value(l_midleNameInd).toString();
            QString l_lastNameVal    = l_query.value(l_lastNameInd).toString();
            // QString l_fullNameVal    = l_query.value(l_fullNameInd).toString();
            QString l_keyNumberVal   = l_query.value(l_keyNumberInd).toString();
            QString l_tableNumberVal = l_query.value(l_tableNumberInd).toString();
            QDate   l_birthDate      = l_query.value(l_birthDayInd).toDate();
            QString l_snilsVal       = l_query.value(l_snils).toString();
            QString l_doljVal        = l_query.value(l_dolj).toString();
            QString l_commentVal     = l_query.value(l_commentInd).toString();
            QString l_profIdVal      = l_query.value(l_profId).toString();

            CDolgnost l_doljTmp = getDolgnostById(l_doljVal);

            CPerson* l_per = new CPerson(l_sidVal, l_groupSidVal, l_firstNameVal, l_lastNameVal, l_midleNameVal, l_keyNumberVal, l_tableNumberVal);
            l_per->setBirthDay(l_birthDate);
            l_per->setSnils(l_snilsVal);
            l_per->setInn(l_snilsVal.toStdString());
            l_per->setComment(l_commentVal);
            l_per->setPosition(l_doljTmp);
            l_per->setProfile(getProfileById(l_profIdVal));
            l_list.push_back(l_per);
        }
        l_query.clear();
        l_query.finish();
        // close();
        return l_list;

        // return getPersonListByTabNumber("%%");
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CPerson*> CConTC::getPersonListWithNullProfile()
    {
        // connect();
        vector<CPerson*> l_list;

        // QString l_curQuery;
        // l_curQuery = "select UID, USERGROUP, FIRSTNAME, MIDNAME, SNAME, FULLNAME, SHEDCARDKEY, TABNUM, BITHDAY, USER_COMMENT FROM users WHERE TABNUM LIKE '%" + tabNum + "%' ORDER BY FULLNAME";


        QSqlQuery l_query(getDatabase());
        l_query.prepare("select UID, USERGROUP, FIRSTNAME, MIDNAME, SNAME, FULLNAME, SHEDCARDKEY, TABNUM, BITHDAY, AUTOPASS, DOLJ, USER_COMMENT, BT_ENTER_PROFILE FROM users WHERE BT_ENTER_PROFILE IS null ORDER BY FULLNAME");

        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
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
        int l_snils          = l_rec.indexOf("AUTOPASS");
        int l_dolj           = l_rec.indexOf("DOLJ");
        int l_commentInd     = l_rec.indexOf("USER_COMMENT");
        int l_profId         = l_rec.indexOf("BT_ENTER_PROFILE");

        int count = 0;

        while (l_query.next())
        {
            count++;

//            if (count >= 20)
//            {
//                return l_list;
//            }
            // cout << "Resiving persons from DB: " << count << endl;
            QString l_sidVal         = l_query.value(l_sidInd).toString();
            QString l_groupSidVal    = l_query.value(l_groupSidInd).toString();
            QString l_firstNameVal   = l_query.value(l_firstNameInd).toString();
            QString l_midleNameVal   = l_query.value(l_midleNameInd).toString();
            QString l_lastNameVal    = l_query.value(l_lastNameInd).toString();
            // QString l_fullNameVal    = l_query.value(l_fullNameInd).toString();
            QString l_keyNumberVal   = l_query.value(l_keyNumberInd).toString();
            QString l_tableNumberVal = l_query.value(l_tableNumberInd).toString();
            QDate   l_birthDate      = l_query.value(l_birthDayInd).toDate();
            QString l_snilsVal       = l_query.value(l_snils).toString();
            QString l_doljVal        = l_query.value(l_dolj).toString();
            QString l_commentVal     = l_query.value(l_commentInd).toString();
            QString l_profIdVal      = l_query.value(l_profId).toString();

            CDolgnost l_doljTmp = getDolgnostById(l_doljVal);

            CPerson* l_per = new CPerson(l_sidVal, l_groupSidVal, l_firstNameVal, l_lastNameVal, l_midleNameVal, l_keyNumberVal, l_tableNumberVal);
            l_per->setBirthDay(l_birthDate);
            l_per->setSnils(l_snilsVal);
            l_per->setInn(l_snilsVal.toStdString());
            l_per->setComment(l_commentVal);
            l_per->setPosition(l_doljTmp);
            l_per->setProfile(getProfileById(l_profIdVal));
            l_list.push_back(l_per);
        }
        l_query.clear();
        l_query.finish();
        // close();
        return l_list;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CPerson*> CConTC::getPersonListByTabNumber(const QString& tabNum)
    {
        // connect();
        vector<CPerson*> l_list;

        // QString l_curQuery;
        // l_curQuery = "select UID, USERGROUP, FIRSTNAME, MIDNAME, SNAME, FULLNAME, SHEDCARDKEY, TABNUM, BITHDAY, USER_COMMENT FROM users WHERE TABNUM LIKE '%" + tabNum + "%' ORDER BY FULLNAME";


        QSqlQuery l_query(getDatabase());
        l_query.prepare("select UID, USERGROUP, FIRSTNAME, MIDNAME, SNAME, FULLNAME, SHEDCARDKEY, TABNUM, BITHDAY, AUTOPASS, DOLJ, USER_COMMENT, BT_ENTER_PROFILE FROM users WHERE TABNUM LIKE :tabNum ORDER BY FULLNAME");
        l_query.bindValue(":tabNum", tabNum);

        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
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
        int l_snils          = l_rec.indexOf("AUTOPASS");
        int l_dolj           = l_rec.indexOf("DOLJ");
        int l_commentInd     = l_rec.indexOf("USER_COMMENT");
        int l_profId         = l_rec.indexOf("BT_ENTER_PROFILE");

        int count = 0;

        while (l_query.next())
        {
            count++;

//            if (count >= 20)
//            {
//                return l_list;
//            }
            // cout << "Resiving persons from DB: " << count << endl;
            QString l_sidVal         = l_query.value(l_sidInd).toString();
            QString l_groupSidVal    = l_query.value(l_groupSidInd).toString();
            QString l_firstNameVal   = l_query.value(l_firstNameInd).toString();
            QString l_midleNameVal   = l_query.value(l_midleNameInd).toString();
            QString l_lastNameVal    = l_query.value(l_lastNameInd).toString();
            // QString l_fullNameVal    = l_query.value(l_fullNameInd).toString();
            QString l_keyNumberVal   = l_query.value(l_keyNumberInd).toString();
            QString l_tableNumberVal = l_query.value(l_tableNumberInd).toString();
            QDate   l_birthDate      = l_query.value(l_birthDayInd).toDate();
            QString l_snilsVal       = l_query.value(l_snils).toString();
            QString l_doljVal        = l_query.value(l_dolj).toString();
            QString l_commentVal     = l_query.value(l_commentInd).toString();
            QString l_profIdVal      = l_query.value(l_profId).toString();

            CDolgnost l_doljTmp = getDolgnostById(l_doljVal);

            CPerson* l_per = new CPerson(l_sidVal, l_groupSidVal, l_firstNameVal, l_lastNameVal, l_midleNameVal, l_keyNumberVal, l_tableNumberVal);
            l_per->setBirthDay(l_birthDate);
            l_per->setSnils(l_snilsVal);
            l_per->setInn(l_snilsVal.toStdString());
            l_per->setComment(l_commentVal);
            l_per->setPosition(l_doljTmp);
            l_per->setProfile(getProfileById(l_profIdVal));
            l_list.push_back(l_per);
        }
        l_query.clear();
        l_query.finish();
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
        if (person == nullptr)
        {
            return;
        }

        QString l_uId = getNextId("USERS", "UID");

        CDolgnost l_d = getDolgnostByCode(person->getPosNumber());
        if ((l_d.getRecId() == "") || (l_d.getRecId() == "0"))
        {
            l_d = createNewPosition(person->getPosition());
        }

        CProfile l_p = getProfileByCode(person->getTableNumber());
        if ((l_p.isEmpty()) || (l_p.getCode() != person->getTableNumber()))
        {
            l_p = createNewProfileByPerson(person);
        }

        QSqlQuery l_query(getDatabase());
        l_query.prepare("INSERT INTO USERS (uid, DEPART, firstname, sname, midname, fullname, tabnum, shedcardkey, bithday, startdate, add_profile_start, user_comment, device_uid, autopass, dolj, bt_enter_profile) "
                        "VALUES (:uid, :depart, :fname, :sname, :mname, :fioName, :tnum, :key, :bday, :sdate, :adate, :ucomment, :devUid, :apass, :dolj, :profile)");
        l_query.bindValue(":uid",      l_uId);
        l_query.bindValue(":depart",   deptNum.isEmpty() == true ? QVariant() : deptNum);
        l_query.bindValue(":fname",    person->getFirstName().isEmpty() == true ? QVariant() : person->getFirstName());
        l_query.bindValue(":sname",    person->getMidleName().isEmpty() == true ? QVariant() : person->getMidleName());
        l_query.bindValue(":mname",    person->getLastName().isEmpty() == true ? QVariant() : person->getLastName());
        l_query.bindValue(":fioName",  person->getFullName().isEmpty() == true ? QVariant() : person->getFullName());
        l_query.bindValue(":tnum",     person->getTableNumber().isEmpty() == true ? QVariant() : person->getTableNumber());
        l_query.bindValue(":key",      person->getKeyNumber().isEmpty() == true ? QVariant() : person->getKeyNumber());
        l_query.bindValue(":bday",     person->getBirthDay());
        l_query.bindValue(":sdate",    QDate::currentDate());
        l_query.bindValue(":adate",    QDate::currentDate());
        l_query.bindValue(":ucomment", person->getComment().isEmpty() == true ? QVariant() : person->getComment().isEmpty());
        l_query.bindValue(":devUid",   l_uId.isEmpty() == true ? QVariant() : l_uId);
        l_query.bindValue(":apass",    person->getSnils().isEmpty() == true ? QVariant() : person->getSnils());
        l_query.bindValue(":dolj",     l_d.getRecId().isEmpty() == true ? QVariant() : l_d.getRecId());
        l_query.bindValue(":profile",  l_p.getId().isEmpty() == true ? QVariant() : l_p.getId());


        QTextCodec* l_c = QTextCodec::codecForName("CP866");
        cout << ":uid " << l_uId.toStdString() << endl;
        cout << ":depart " << deptNum.toStdString()  << endl;
        cout << ":fname " << l_c->fromUnicode(person->getFirstName()).data() << endl;
        cout << ":sname " << l_c->fromUnicode(person->getMidleName()).data()  << endl;
        cout << ":mname " << l_c->fromUnicode(person->getLastName()).data()  << endl;
        cout << ":fioName " << l_c->fromUnicode(person->getFullName()).data()  << endl;
        cout << ":tnum " << person->getTableNumber().toStdString() << endl;
        cout << ":key " << person->getKeyNumber().toStdString() << endl;
        cout << ":bday " << l_c->fromUnicode(person->getBirthDay().toString()).data() << endl;
        cout << ":sdate " << l_c->fromUnicode(QDate::currentDate().toString()).data() << endl;
        cout << ":adate " << l_c->fromUnicode(QDate::currentDate().toString()).data() << endl;
        cout << ":ucomment " << l_c->fromUnicode(person->getComment()).data() << endl;
        cout << ":devUid " << l_uId.toStdString() << endl;
        cout << ":apass " << person->getSnils().toStdString() << endl;
        cout << ":dolj " << l_d.getRecId().toStdString() << endl;
        cout << ":profile " << l_p.getId().toStdString() << endl;

        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
            return;
        }
        l_query.clear();
        l_query.finish();

        moveGenId("USERS_DEVICE_UID_GEN", l_uId);
        moveGenId("USERS_GEN", l_uId);


        QByteArray l_photo = person->getPhoto();
        if (l_photo.size() <= 10)
        {
            return;
        }

        QString l_pId = getNextId("USERS_PHOTO", "PHOTOID");
        moveGenId("USERS_PHOTO_GEN", l_pId);
        l_query.prepare("INSERT INTO USERS_PHOTO (PHOTOID, USERID, PHOTO) "
                        "VALUES (:pId, :uId, :photo)");
        l_query.bindValue(":pId", l_pId);
        l_query.bindValue(":uId", l_uId);
        l_query.bindValue(":photo", l_photo);

        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
            return;
        }
        l_query.clear();
        l_query.finish();

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
            l_query.clear();
            l_query.finish();
            return;
        }
        l_query.clear();
        l_query.finish();
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
        l_query.prepare("SELECT UID, DEPART, FIRSTNAME, MIDNAME, SNAME, FULLNAME, BITHDAY, TABNUM, SHEDCARDKEY, AUTOPASS, DOLJ, USER_COMMENT, BT_ENTER_PROFILE "
                        "FROM USERS WHERE FULLNAME LIKE :fullName ORDER BY FULLNAME");
        l_query.bindValue(":fullName", l_valFullName);

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
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
        int l_dolj           = l_rec.indexOf("DOLJ");
        int l_commentInd     = l_rec.indexOf("USER_COMMENT");
        int l_profId         = l_rec.indexOf("BT_ENTER_PROFILE");

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
            QString l_doljVal        = l_query.value(l_dolj).toString();
            QString l_commentVal     = l_query.value(l_commentInd).toString();
            QString l_profIdVal      = l_query.value(l_profId).toString();

            CDolgnost l_doljTmp = getDolgnostById(l_doljVal);
            CPerson* l_per = new CPerson(l_sidVal, l_groupSidVal, l_firstNameVal, l_lastNameVal, l_midleNameVal, l_keyNumberVal, l_tableNumberVal);
            l_per->setBirthDay(l_birthDate);
            l_per->setSnils(l_valSnils);
            l_per->setPosition(l_doljTmp);
            l_per->setComment(l_commentVal);
            l_per->setProfile(getProfileById(l_profIdVal));
            l_list.push_back(l_per);
        }
        l_query.clear();
        l_query.finish();

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
        l_query.clear();
        l_query.finish();
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
        l_query.clear();
        l_query.finish();
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
        l_query.clear();
        l_query.finish();
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
        l_query.clear();
        l_query.finish();
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

        QString tableNumber = tabNum.trimmed();
        if (tableNumber.startsWith("ДЭН") || \
            tableNumber.startsWith("ФБ"))
        {
            // do nothing table number is OK;
        }
        else if (tableNumber.startsWith("ТД"))
        {
            int l_len = tableNumber.length();
            int l_dif = 10 - l_len;
            QString l_zeroStr;
            for (int i = 0; i < l_dif; i++)
            {
                l_zeroStr = l_zeroStr + "0";
            }
            tableNumber = "ТД" + l_zeroStr + tableNumber.right(3);
        }
        else if (tableNumber.startsWith("Э"))
        {
            int l_len = tableNumber.length();
            int l_dif = 10 - l_len;
            QString l_zeroStr;
            for (int i = 0; i < l_dif; i++)
            {
                l_zeroStr = l_zeroStr + "0";
            }
            tableNumber = "Э" + l_zeroStr + tableNumber.right(4);
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

        QSqlQuery l_query(getDatabase());
        QTextCodec* l_c = QTextCodec::codecForName("CP866");

        cout << "Updating with tabNumber: " << l_c->fromUnicode(tableNumber).data() << endl;
        cout << "Key: " << key.toStdString() << endl;
        cout << "Comment: " << comment.toStdString() << endl;
        l_query.prepare("UPDATE USERS SET  SHEDCARDKEY = :key, USER_COMMENT= :comment "
                        "WHERE TABNUM = :tabNum");
        l_query.bindValue(":key", key);
        l_query.bindValue(":comment", comment);
        l_query.bindValue(":tabNum", tableNumber);

        if (tableNumber == "ТД00000473")
        {
            return;
        }
        cout << "After bind" << endl;
        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }
        l_query.clear();
        l_query.finish();
        cout << "Before exit" << endl;
        return;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::updateNameParams(const QString& id, const CPerson* person)
    {
        // Не проверям КЛЮЧ И КОММЕНТЫ   для возможности обнуления поля.
        if ((id.isEmpty()) || (person == nullptr))
        {
            return;
        }

        QSqlQuery l_query(getDatabase());

        //QTextCodec* codec = QTextCodec::codecForName("CP1251");
        l_query.prepare("UPDATE USERS SET  FIRSTNAME = :fname, SNAME = :sname, MIDNAME = :mname, FULLNAME = :flName "
                        "WHERE uid = :uid");

        l_query.bindValue(":fname", person->getFirstName());
        l_query.bindValue(":sname", person->getLastName());
        l_query.bindValue(":mname", person->getMidleName());
        l_query.bindValue(":flName", person->getFullName());
        l_query.bindValue(":uid", id);

//        l_query.bindValue(":fname", codec->toUnicode(person->getFirstNameStd().c_str()));
//        l_query.bindValue(":sname", codec->toUnicode(person->getLastNameStd().c_str()));
//        l_query.bindValue(":mname", codec->toUnicode(person->getMidleNameStd().c_str()));
//        l_query.bindValue(":flName", codec->toUnicode(person->getFullName().toStdString().c_str()));
//        l_query.bindValue(":uid", codec->toUnicode(id.toStdString().c_str()));

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }
        l_query.clear();
        l_query.finish();
        return;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::updatePersonPosition(const QString& id, const CPerson* person)
    {
        OLL << "ENTER TO UPDATE POSITION";
        LL << id;
        // Не проверям КЛЮЧ И КОММЕНТЫ   для возможности обнуления поля.
        if ((id.isEmpty()) || (person == nullptr))
        {
            return;
        }

        QSqlQuery l_query(getDatabase());

        //QTextCodec* codec = QTextCodec::codecForName("CP1251");

        if (person->getPosRecId().isEmpty())
        {
            OLL << "PERSON HAVE EMPTY ID VALUE.";
            l_query.prepare("UPDATE USERS SET  DOLJ = NULL "
                            "WHERE uid = :uid");
            // l_query.bindValue(":dolj", "NULL");
        }
        else
        {
            LL << QString("SETUP DOLJ VALUE: ").append(person->getPosRecId());
            l_query.prepare("UPDATE USERS SET  DOLJ = :dolj "
                            "WHERE uid = :uid");
            l_query.bindValue(":dolj", person->getPosRecId());
        }
        l_query.bindValue(":uid", id);


        if(!l_query.exec())
        {
            OLL << "ID_ERROR";
            LL << person->getPosRecId();
            LL << id;
            LL << *person;
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }
        // OLL << "UPDATING USER POSITION";
        l_query.clear();
        l_query.finish();
        return;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CDolgnost CConTC::getDolgnostById(const QString& id)
    {
        QString l_kod = id.trimmed();

        if (l_kod.isEmpty() || l_kod == "0")
        {
            OLL << "EMPTY DOLJNOST ID";
            return CDolgnost();
        }

        QSqlQuery l_query(getDatabase());

        QTextCodec* c = QTextCodec::codecForName("CP1251");
        OLL << "\t\tID IS";
        LL << QString("\t\t").append(c->fromUnicode(l_kod));
        l_kod = "%" + l_kod + "%";
        l_query.prepare("SELECT ID, DICTID, DICTVALUE, CODE "
                        "FROM DICTLIST_DET WHERE ID LIKE :id ORDER BY DICTVALUE");
        l_query.bindValue(":id", l_kod);

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
            return CDolgnost();
        }

        QSqlRecord l_rec = l_query.record();

        int l_recId = l_rec.indexOf("ID");
        int l_name  = l_rec.indexOf("DICTVALUE");
        int l_code  = l_rec.indexOf("CODE");


        while (l_query.next())
        {
            QString l_recIdVal = l_query.value(l_recId).toString();
            QString l_nameVal  = l_query.value(l_name).toString();
            QString l_codeVal  = l_query.value(l_code).toString();

            l_query.clear();
            l_query.finish();
            return CDolgnost(l_recIdVal, l_codeVal, l_nameVal);
        }
        l_query.clear();
        l_query.finish();

        return CDolgnost();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CDolgnost CConTC::getDolgnostByCode(const QString& code)
    {
        QString l_kod = code.trimmed();

        if (l_kod.isEmpty() || l_kod == "0")
        {
            OLL << "EMPTY DOLJNOST KOD";
            return CDolgnost();
        }

        QSqlQuery l_query(getDatabase());

        QTextCodec* c = QTextCodec::codecForName("CP1251");
        OLL << "\t\tKOD IS";
        LL << QString("\t\t").append(c->fromUnicode(l_kod));
        l_kod = "%" + l_kod + "%";
        l_query.prepare("SELECT ID, DICTID, DICTVALUE, CODE "
                        "FROM DICTLIST_DET WHERE DICTID = 1 AND CODE LIKE :kod ORDER BY DICTVALUE");
        l_query.bindValue(":kod", l_kod);

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
            return CDolgnost();
        }

        QSqlRecord l_rec = l_query.record();

        int l_recId = l_rec.indexOf("ID");
        int l_name  = l_rec.indexOf("DICTVALUE");
        int l_code  = l_rec.indexOf("CODE");


        while (l_query.next())
        {
            QString l_recIdVal = l_query.value(l_recId).toString();
            QString l_nameVal  = l_query.value(l_name).toString();
            QString l_codeVal  = l_query.value(l_code).toString();

            l_query.clear();
            l_query.finish();
            return CDolgnost(l_recIdVal, l_codeVal, l_nameVal);
        }
        l_query.clear();
        l_query.finish();

        return CDolgnost();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CDolgnost CConTC::createNewPosition(const CDolgnost& value)
    {
        QTextCodec* c = QTextCodec::codecForName("CP1251");
        QString l_kod = value.getId();

        if (l_kod.isEmpty() || l_kod == "0")
        {
            OLL << "Can't create a new doljnost.";
            return CDolgnost();
        }
        else
        {
            OLL << "DOLJNOST KOD IS";
            LL << c->fromUnicode(l_kod);
        }

        QString l_nId = getNextId("DICTLIST_DET", "ID");

        OLL << "Next ID is";
        LL << l_nId;
        QSqlQuery l_query(getDatabase());

        l_query.prepare("INSERT INTO DICTLIST_DET (ID, DICTID, DICTVALUE, CODE) VALUES (:id, 1, :name, :kod)");
        l_query.bindValue(":id", l_nId);
        l_query.bindValue(":name", value.getName());
        l_query.bindValue(":kod", value.getId());

        OLL << "BEFORE EXEC";
        LL << c->fromUnicode(value.getName());
        LL << c->fromUnicode(value.getId());
        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
            OLL << "EXEC ERROR";
            return CDolgnost();
        }

        moveGenId("DICTLIST_DET_GEN", l_nId);

        OLL << "Doljnost is created";
        LL << l_nId;
        return CDolgnost(l_nId, value.getName(), value.getId());
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CProfile CConTC::getProfileByCode(const QString& code)
    {
        QSqlQuery l_query(getDatabase());
        l_query.prepare("SELECT DPID, DPNAME, ENTERCLOSE, IDENT_TYPE, CODE, ISDEFAULT FROM "
                        "PROFILES_ACCESS WHERE CODE LIKE :lcode");
        l_query.bindValue(":lcode", code);

        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
            return CProfile();
        }

        QSqlRecord l_rec = l_query.record();

        int l_id   = l_rec.indexOf("DPID");
        int l_name = l_rec.indexOf("DPNAME");
        int l_ec   = l_rec.indexOf("ENTERCLOSE");
        int l_it   = l_rec.indexOf("IDENT_TYPE");
        int l_code = l_rec.indexOf("CODE");

        while (l_query.next())
        {
            CProfile l_prof;
            l_prof.setId(l_query.value(l_id).toString());
            l_prof.setName(l_query.value(l_name).toString());
            l_prof.setEnterClose(l_query.value(l_ec).toString());
            l_prof.setIdentType(l_query.value(l_it).toString());
            l_prof.setCode(l_query.value(l_code).toString());
            l_prof.setDoors(getDoorsForProfile(l_query.value(l_id).toString()));

            return l_prof;
        }

        return CProfile();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CDoor> CConTC::getDoorsForProfile(const QString& id)
    {
        vector<CDoor> doors;
        if (id.isEmpty())
        {
            return doors;
        }

        QSqlQuery l_query(getDatabase());
        l_query.prepare("SELECT DPID, DOORID, ENTERMODE, USETIMEINTERVAL, IDENT_TYPE FROM "
                        "PROFILES_ACCESS_DET WHERE DPID LIKE :pid");
        l_query.bindValue(":pid", id);

        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            return doors;
        }

        QSqlRecord l_rec = l_query.record();
        int l_id         = l_rec.indexOf("DOORID");
        int l_eMode      = l_rec.indexOf("ENTERMODE");
        int l_utInterval = l_rec.indexOf("USETIMEINTERVAL");
        int l_iType      = l_rec.indexOf("IDENT_TYPE");

        while (l_query.next())
        {
            CDoor l_door;
            l_door.setId(l_query.value(l_id).toString());
            l_door.setEnterMode(l_query.value(l_eMode).toString());
            l_door.setUseTimeInterval(l_query.value(l_utInterval).toString());
            l_door.setIdentType(l_query.value(l_iType).toString());
            doors.push_back(l_door);
        }

        return doors;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CProfile CConTC::getProfileById(const QString& id)
    {
//        cout << "Before getDatabase" << endl;
        QSqlQuery l_query(getDatabase());
//        cout << "After getDatabase" << endl;
        l_query.prepare("SELECT DPID, DPNAME, ENTERCLOSE, IDENT_TYPE, CODE, ISDEFAULT FROM "
                        "PROFILES_ACCESS WHERE DPID LIKE :lid");
        l_query.bindValue(":lid", id);

        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
            return CProfile();
        }

        QSqlRecord l_rec = l_query.record();

        int l_id   = l_rec.indexOf("DPID");
        int l_name = l_rec.indexOf("DPNAME");
        int l_ec   = l_rec.indexOf("ENTERCLOSE");
        int l_it   = l_rec.indexOf("IDENT_TYPE");
        int l_code = l_rec.indexOf("CODE");

        while (l_query.next())
        {
            CProfile l_prof;
            l_prof.setId(l_query.value(l_id).toString());
            l_prof.setName(l_query.value(l_name).toString());
            l_prof.setEnterClose(l_query.value(l_ec).toString());
            l_prof.setIdentType(l_query.value(l_it).toString());
            l_prof.setCode(l_query.value(l_code).toString());
            vector<CDoor> l_doors = getDoorsForProfile(l_query.value(l_id).toString());
            l_prof.setDoors(l_doors);

            return l_prof;
        }

        return CProfile();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CDoor> CConTC::getAllDoors()
    {
        QSqlQuery l_query(getDatabase());
        l_query.prepare("SELECT DID, DUSEFORFACT, DEFAULTCLOSE, DOORNAME, ISDEFAULT FROM DOORS");

        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
            return vector<CDoor>();
        }

        QSqlRecord l_rec = l_query.record();

        int l_id   = l_rec.indexOf("DID");
        int l_uff  = l_rec.indexOf("DUSEFORFACT");
        int l_dc   = l_rec.indexOf("DEFAULTCLOSE");
        int l_name = l_rec.indexOf("DOORNAME");
        int l_idf  = l_rec.indexOf("ISDEFAULT");

        vector<CDoor> doors;
        while (l_query.next())
        {
            CDoor l_d;
            l_d.setId(l_query.value(l_id).toString());
            l_d.setUseForFact(l_query.value(l_uff).toString());
            l_d.setDefaultClose(l_query.value(l_dc).toString());
            l_d.setDoorName(l_query.value(l_name).toString());
            l_d.setIsDefault(l_query.value(l_idf).toString());

            doors.push_back(l_d);
        }

        return doors;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CProfile CConTC::createNewProfileByPerson(const CPerson* person)
    {
        if (person == nullptr)
        {
            return CProfile();
        }

        QTextCodec* c = QTextCodec::codecForName("CP1251");
        QString l_kod = person->getTableNumber();

        if (l_kod.isEmpty() || l_kod == "0")
        {
            OLL << "Empty kod for profile. Table number is empty.";
            // OLL << "Can't create a new profile.";
            // return CProfile();
        }
        else
        {
            OLL << "PROFILE ID IS";
            LL << c->fromUnicode(l_kod);
        }

        QString l_nId = getNextId("PROFILES_ACCESS", "DPID");

        OLL << "Next profile ID is";
        LL << l_nId;
        QSqlQuery l_query(getDatabase());        

        QString l_name = person->getFirstName() + "_" + person->getMidleName() + "_" + person->getLastName() + "_" + person->getTableNumber();
        l_query.prepare("INSERT INTO PROFILES_ACCESS (DPID, DPNAME, ENTERCLOSE, IDENT_TYPE, CODE) VALUES (:id, :name, 1, 5, :kod)");
        l_query.bindValue(":id", l_nId);
        l_query.bindValue(":name", l_name);
        l_query.bindValue(":kod", !person->getTableNumber().isEmpty() ? person->getTableNumber() : "visitor");

        OLL << "BEFORE EXEC";
        LL << c->fromUnicode(l_name);

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
            OLL << "EXEC ERROR";
            return CProfile();
        }

        moveGenId("PROFILES_ACCESS_GEN", l_nId);

        OLL << "Profile is created";
        LL << l_nId;
        CProfile l_p;
        l_p.setId(l_nId);
        l_p.setName(l_name);
        l_p.setCode(person->getTableNumber());
        l_p.setEnterClose("1");
        l_p.setIdentType("5");
        l_p.setDoors(createNewProfileDoors(l_nId));
        return l_p;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CDoor> CConTC::createNewProfileDoors(const QString& pfId)
    {
        QTextCodec* c = QTextCodec::codecForName("CP1251");
        QString l_kod = pfId.trimmed();

        if (l_kod.isEmpty() || l_kod == "0")
        {
            OLL << "Can't create a new dors for profile.";
            return vector<CDoor>();
        }
        else
        {
            OLL << "Creating dors for profile with id";
            LL << c->fromUnicode(l_kod);
        }

        vector<CDoor> l_ad = getAllDoors();

        vector<CDoor> l_resDoors;

        auto it = begin(l_ad);
        for(;it != end(l_ad);++it)
        {
            QString l_nId = getNextId("PROFILES_ACCESS_DET", "PDETID");

            OLL << "Next door ID for profile ";
            LL << l_nId;
            QSqlQuery l_query(getDatabase());

            l_query.prepare("INSERT INTO PROFILES_ACCESS_DET (PDETID, DPID, DOORID, ENTERMODE, USETIMEINTERVAL, IDENT_TYPE) VALUES"
                            "(:id, :pfId, :drId, 1, 0, 5)");
            l_query.bindValue(":id", l_nId);
            l_query.bindValue(":pfId", pfId);
            l_query.bindValue(":drId", (*it).getId());


            if(!l_query.exec())
            {
                cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
                cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
                l_query.clear();
                l_query.finish();
                OLL << "Create doors EXEC ERROR";
            }

            moveGenId("PROFILES_ACCESS_DET_GEN", l_nId);

            CDoor l_door;
            l_door.setId((*it).getId());
            l_door.setDoorName((*it).getDoorName());
            l_door.setEnterMode("0");
            l_door.setIdentType("5");
            l_door.setUseTimeInterval("0");
            l_door.setIsDefault("0");
            l_door.setDefaultClose("0");
            l_resDoors.push_back(l_door);
        }

        OLL << "Doors are created";

        return l_resDoors;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::updatePersonProfile(const CPerson* person)
    {
        if (person == nullptr)
            return;

        QString l_profId = person->getProfile().getId();
        OLL << "Profile id from updatePersonProfile";
        LL << l_profId;
        CProfile l_p = getProfileById(l_profId);

        if ((l_p.getCode() == person->getTableNumber()) && (!person->getTableNumber().isEmpty()))
            return;

        QString noTableNumberProfileName = person->getFirstName() + "_" + person->getMidleName() + "_" + person->getLastName() + "_";
        if ((person->getTableNumber().isEmpty()) && (l_p.getName() == noTableNumberProfileName))
            return;

        l_p = createNewProfileByPerson(person);

        QSqlQuery l_query(getDatabase());

        l_query.prepare("UPDATE USERS SET  BT_ENTER_PROFILE = :id "
                        "WHERE uid = :uid");
        l_query.bindValue(":id", l_p.getId());
        l_query.bindValue(":uid", person->getSid());

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }
        l_query.clear();
        l_query.finish();
        return;

    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::addLineToAccessMatrix(const vector<string>& row)
    {
        if (row.size() < 18)
            return;

        QTextCodec* l_c = QTextCodec::codecForName("CP1251");
        QSqlQuery l_query(getDatabase());
        QString l_nId = getNextId("ACCESS_MATRIX", "ID");
        cout << "Row size: " << row.size() << endl;
        cout << "Next id: " << l_nId.toStdString() << endl;
        l_query.prepare("INSERT INTO ACCESS_MATRIX (ID, DOLJ_CODE, DEPART_NAME, DOLJ_NAME, E_ADMIN, E_SK, E_UGP, E_STREET_UBOY, "
                        "EX_DVOR, EX_AUTOSTOY, E_SMOKE, E_LIFT_SK, E_SKLAD1, E_SKLAD2, E_TEX_SKLAD, E_TUR_PROIZ, E_TUR_PROXOD, E_TUR_SGP, E_TUR_UBOY, E_AUTOSTOY) "
                        "VALUES (:id, :doljCode, :depName, :doljName, :pr1, :pr2, :pr3, :pr4, :pr5, :pr6, :pr7, :pr8, :pr9, :pr10, :pr11, :pr12, :pr13, :pr14, :pr15, :pr16)");
        l_query.bindValue(":id", l_nId);
        QString l_str;
        l_str = l_c->toUnicode(row[0].c_str());
        l_query.bindValue(":doljCode", l_str.trimmed());
        l_str = l_c->toUnicode(row[2].c_str());
        l_query.bindValue(":depName", l_str);
        l_str = l_c->toUnicode(row[1].c_str());
        l_query.bindValue(":doljName", l_str);
        l_query.bindValue(":pr1", row[3] == "1" ? "1" : "0");
        l_query.bindValue(":pr2", row[4] == "1" ? "1" : "0");
        l_query.bindValue(":pr3", row[5] == "1" ? "1" : "0");
        l_query.bindValue(":pr4", row[6] == "1" ? "1" : "0");
        l_query.bindValue(":pr5", row[7] == "1" ? "1" : "0");
        l_query.bindValue(":pr6", row[8] == "1" ? "1" : "0");
        l_query.bindValue(":pr7", row[9] == "1" ? "1" : "0");
        l_query.bindValue(":pr8", row[10] == "1" ? "1" : "0");
        l_query.bindValue(":pr9", row[11] == "1" ? "1" : "0");
        l_query.bindValue(":pr10", row[12] == "1" ? "1" : "0");
        l_query.bindValue(":pr11", row[13] == "1" ? "1" : "0");
        l_query.bindValue(":pr12", row[14] == "1" ? "1" : "0");
        l_query.bindValue(":pr13", row[15] == "1" ? "1" : "0");
        l_query.bindValue(":pr14", row[16] == "1" ? "1" : "0");
        l_query.bindValue(":pr15", row[17] == "1" ? "1" : "0");
        l_query.bindValue(":pr16", "1");

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            abort();
        }

        l_query.clear();
        l_query.finish();
        return;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CDoor> CConTC::getDoorsByDolgnost(const QString& dolj)
    {
        if (dolj.isEmpty())
            return vector<CDoor>();

        QSqlQuery l_query(getDatabase());
        l_query.prepare("SELECT E_ADMIN, E_SK, E_UGP, E_STREET_UBOY, EX_DVOR, EX_AUTOSTOY, E_SMOKE, E_LIFT_SK, E_SKLAD1, "
                        "E_SKLAD2, E_TEX_SKLAD, E_TUR_PROIZ, E_TUR_PROXOD, E_TUR_SGP, E_TUR_UBOY FROM ACCESS_MATRIX "
                        "WHERE DOLJ_CODE LIKE :doljName");
        l_query.bindValue(":doljName", dolj);

        if (l_query.exec() == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
            return vector<CDoor>();
        }

        vector<CDoor> l_doors;

        QSqlRecord l_rec = l_query.record();

        int l_eAdmin      = l_rec.indexOf("E_ADMIN");
        int l_eSk         = l_rec.indexOf("E_SK");
        int l_eUgp        = l_rec.indexOf("E_UGP");
        int l_eStreetUboy = l_rec.indexOf("E_STREET_UBOY");
        int l_exDvor      = l_rec.indexOf("EX_DVOR");
        int l_exAutostoy  = l_rec.indexOf("EX_AUTOSTOY");
        int l_eSmoke      = l_rec.indexOf("E_SMOKE");
        int l_eLiftSk     = l_rec.indexOf("E_LIFT_SK");
        int l_sklad1      = l_rec.indexOf("E_SKLAD1");
        int l_sklad2      = l_rec.indexOf("E_SKLAD2");
        int l_eTexSklad   = l_rec.indexOf("E_TEX_SKLAD");
        int l_eTurProiz   = l_rec.indexOf("E_TUR_PROIZ");
        int l_eTurProxod  = l_rec.indexOf("E_TUR_PROXOD");
        int l_eTurSgp     = l_rec.indexOf("E_TUR_SGP");
        int l_eTurUboy    = l_rec.indexOf("E_TUR_UBOY");

        while (l_query.next())
        {
            CDoor l_d;
            l_d.setId("8");
            l_d.setDoorName("Вход в Администрацию");
            l_d.setEnterMode(l_query.value(l_eAdmin).toString());
            l_doors.push_back(l_d);

            l_d.setId("14");
            l_d.setDoorName("Вход на СК");
            l_d.setEnterMode(l_query.value(l_eSk).toString());
            l_doors.push_back(l_d);

            l_d.setId("7");
            l_d.setDoorName("Вход в УГП");
            l_d.setEnterMode(l_query.value(l_eUgp).toString());
            l_doors.push_back(l_d);

            l_d.setId("6");
            l_d.setDoorName("Вход с улицы в Убой");
            l_d.setEnterMode(l_query.value(l_eStreetUboy).toString());
            l_doors.push_back(l_d);

            l_d.setId("9");
            l_d.setDoorName("Вход во двор");
            l_d.setEnterMode(l_query.value(l_exDvor).toString());
            l_doors.push_back(l_d);

            l_d.setId("10");
            l_d.setDoorName("Вход на автостоянку");
            l_d.setEnterMode(l_query.value(l_exAutostoy).toString());
            l_doors.push_back(l_d);

            l_d.setId("17");
            l_d.setDoorName("Курилка");
            l_d.setEnterMode(l_query.value(l_eSmoke).toString());
            l_doors.push_back(l_d);

            l_d.setId("13");
            l_d.setDoorName("Лифт на сырокопоть");
            l_d.setEnterMode(l_query.value(l_eLiftSk).toString());
            l_doors.push_back(l_d);

            l_d.setId("11");
            l_d.setDoorName("Склад №1");
            l_d.setEnterMode(l_query.value(l_sklad1).toString());
            l_doors.push_back(l_d);

            l_d.setId("12");
            l_d.setDoorName("Склад №2");
            l_d.setEnterMode(l_query.value(l_sklad2).toString());
            l_doors.push_back(l_d);

            l_d.setId("15");
            l_d.setDoorName("Технический склад");
            l_d.setEnterMode(l_query.value(l_eTexSklad).toString());
            l_doors.push_back(l_d);

            l_d.setId("2");
            l_d.setDoorName("Турникет производство");
            l_d.setEnterMode(l_query.value(l_eTurProiz).toString());
            l_doors.push_back(l_d);

            l_d.setId("1");
            l_d.setDoorName("Турникет проходная");
            l_d.setEnterMode(l_query.value(l_eTurProxod).toString());
            l_doors.push_back(l_d);

            l_d.setId("4");
            l_d.setDoorName("Турникет СГП");
            l_d.setEnterMode(l_query.value(l_eTurSgp).toString());
            l_doors.push_back(l_d);

            l_d.setId("5");
            l_d.setDoorName("Турникет Убой");
            l_d.setEnterMode(l_query.value(l_eTurUboy).toString());
            l_doors.push_back(l_d);

            return l_doors;
        }

        return l_doors;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    bool CConTC::isFired(const CPerson* person)
    {
        // Уволенные (МК)          - 278
        // Уволенные (Денир)       - 279
        // Уволенные (ТД)          - 280
        // Уволенные (Эксцельсиор) - 281
        // Уволенные (Фабрика)     - 282
        if ((person->getGroupSid() == "278") || (person->getGroupSid() == "279") ||
            (person->getGroupSid() == "280") || (person->getGroupSid() == "281") ||
            (person->getGroupSid() == "278"))
        {
            return true;
        }
        return false;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::updatePersonAccessDoors()
    {
//        vector<CPerson*> l_main = getPersonList();
//        vector<CPerson*> l_pList;
//        l_pList.assign(l_main.begin(), l_main.begin() + 100);

        QTextCodec* l_c = QTextCodec::codecForName("CP866");
        vector<CPerson*> l_pList = getPersonList();

        cout << " " << l_pList.size() << endl;
        int count = 0;
        for (auto el : l_pList)
        {
            count++;
            cout << "Updating door for person: " << count << " " << l_c->fromUnicode(el->getFullName()).data() << endl;
            // Двери из матрицы доступа
            vector<CDoor> l_d = getDoorsByDolgnost(el->getPosition().getId());
            cout << "Dolj name: " << l_c->fromUnicode(el->getPosition().getName()).data() << endl;
            cout << "Matrix doors num: " << l_d.size() << endl;

            // Двери привязанные к человеку
            vector<CDoor> l_pD = getDoorsForProfile(el->getProfile().getId());
            cout << "Person doors num: " << l_pD.size() << endl;

            if (isFired(el))
            {
                closeDoorsForProfile(el->getProfile());
            }
            else
            {
                // Пробегаем двери разрешенные по матрице доступа;
                for (auto cd : l_d)
                {
                    dc lc;
                    lc.toFind = cd;

                    vector<CDoor>::iterator it = find_if(l_pD.begin(), l_pD.end(), lc);
                    if (it != l_pD.end())
                    {
                        // Update doors;
                        //cout << "EQUAL DOORS" << endl;
                        updateCurDoorEnterMode(el->getProfile().getId(), cd.getId(), cd.getEnterMode());
                    }
                    else
                    {
                        // Add new door to profile;
                        // cout << "DIFFIRENT DOORS" << endl;
                        addNewDoorToProfileWithEnterMode(el->getProfile().getId(), cd.getId(), cd.getEnterMode());
                    }
                }
            }
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::updateCurDoorEnterMode(const QString& profileId, const QString& doorId, const QString& enterMode)
    {
        QSqlQuery l_query(getDatabase());
        // l_query.prepare("UPDATE PROFILES_ACCESS_DET SET enterMode = :em WHERE dpid = :pi AND doorid = :di");
        QString l_rEm;
        if (enterMode.trimmed() == "1")
        {
            l_rEm = "0";
        }
        else
        {
            l_rEm = "1";
        }
        // l_query.bindValue(":em", enterMode == "1" ? "0" : "1");
        l_query.bindValue(":em", l_rEm.toInt());
        l_query.bindValue(":pi", profileId.toInt());
        l_query.bindValue(":id", doorId.toInt());

//        cout << "Profile id: " << profileId.toStdString() << endl;
//        cout << "DoorId  id: " << doorId.toStdString() << endl;
//        cout << "Em        : " << l_rEm.toStdString() << endl;

        QString l_curQuery = "UPDATE PROFILES_ACCESS_DET SET enterMode = '" + l_rEm + "' WHERE dpid = '" + profileId + "' AND doorid = '" + doorId + "';";
        // cout << l_curQuery.toStdString() << endl;
        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
        }
        else
        {
            // cout << "Door updates successfully." << endl;
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::addNewDoorToProfileWithEnterMode(const QString& profileId, const QString& doorId, const QString& enterMode)
    {
        QString l_nId = getNextId("PROFILES_ACCESS_DET", "PDETID");

        OLL << "Next door ID for profile ";
        LL << l_nId;
        QSqlQuery l_query(getDatabase());

        l_query.prepare("INSERT INTO PROFILES_ACCESS_DET (PDETID, DPID, DOORID, ENTERMODE, USETIMEINTERVAL, IDENT_TYPE) VALUES"
                        "(:id, :pfId, :drId, :em, 0, 5)");
        l_query.bindValue(":id", l_nId);
        l_query.bindValue(":pfId", profileId);
        l_query.bindValue(":drId", doorId);
        l_query.bindValue(":em", enterMode.isEmpty() ? "1" : enterMode);


        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            cout << "l_nId " << l_nId.toStdString() << endl;
            cout << "pfId " << profileId.toStdString() << endl;
            cout << "drId " << doorId.toStdString() << endl;
            cout << "em   " << enterMode.toStdString() << endl;
            l_query.clear();
            l_query.finish();
            OLL << "Create doors EXEC ERROR";
        }

        moveGenId("PROFILES_ACCESS_DET_GEN", l_nId);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::clearAccessMatrix()
    {
        QSqlQuery l_query(getDatabase());
        l_query.prepare("DELETE FROM ACCESS_MATRIX");

        if(!l_query.exec())
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CConTC::closeDoorsForProfile(const CProfile& profile)
    {
        QSqlQuery l_query(getDatabase());
        l_query.prepare("UPDATE PROFILES_ACCESS_DET SET ENTERMODE = 1 WHERE DPID = :pId");
        l_query.bindValue(":pId", profile.getId());

        if (l_query.exec() == false)
        {
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
            return;
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CConTC::getNextId(const QString& table, const QString& column)
    {
        QString l_curQuery;
        l_curQuery = "SELECT MAX(" +column + ") + 1 AS ID FROM " + table;

        LL << l_curQuery;
        QSqlQuery l_query(getDatabase());
        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_query.lastQuery().toStdString().c_str() << endl;
            cout << "Falt message: " << l_query.lastError().text().toStdString() << endl;
            l_query.clear();
            l_query.finish();
            return QString();
        }

        QSqlRecord l_rec = l_query.record();

        int l_id = l_rec.indexOf("ID");

        QString l_result;
        while (l_query.next())
        {
            l_result = l_query.value(l_id).toString();
            OLL << "1---1";
            LL << l_result;
        }

        OLL << "Result is";
        LL << l_result;
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
        l_query.clear();
        l_query.finish();
    }
}
