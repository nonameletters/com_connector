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
        l_query.bindValue(":dolj",     l_d.getRecId());
        l_query.bindValue(":profile",  l_p.getId());

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
        l_query.clear();
        l_query.finish();
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
            OLL << "Can't create a new profile.";
            return CProfile();
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
        l_query.bindValue(":kod", person->getTableNumber());

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
                            "(:id, :pfId, :drId, 0, 0, 5)");
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

        if (l_p.getCode() == person->getTableNumber())
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
