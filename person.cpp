#include "person.h"

namespace db_connector
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    ostream & operator << (ostream & out, CPerson& p)
    {
        out << " // ---------- ---------- ---------- ---------- ---------- ---------- " << endl;
        out << " SID      : " << p.getSidStd() << endl;
        out << " NAME     : " << p.getFullName().toStdString() << endl;
        return out;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CPerson::CPerson(QString sid,       QString groupSid, QString firstName, \
                     QString midleName, QString lastName, QString keyNumber, \
                     QString tableNumber, QString snils) :
                     m_sid(sid), m_groupSid(groupSid), m_firstName(firstName), \
                     m_midleName(midleName), m_lastName(lastName), m_keyNumber(keyNumber), \
                     m_tableNumber(tableNumber), m_snils(snils)
    {
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CPerson::CPerson(const CPerson& person)
    {
        m_sid            = person.getSid();
        m_groupSid       = person.getGroupSid();
        m_firstName      = person.getFirstName();
        m_midleName      = person.getMidleName();
        m_lastName       = person.getLastName();
        m_keyNumber      = person.getKeyNumber();
        m_tableNumber    = person.getTableNumber();
        m_snils          = person.getSnils();
        m_position       = person.getPositionStd();
        m_inn            = person.getInnStd();
        m_failed         = person.getFailedStd();
        m_activity       = person.getActivity();
        m_organisation   = person.getOrganisationStd();
        m_laborContract  = person.getLaborContractStd();
        m_recordId       = person.getRecordIdStd();
        m_birthDay       = person.getBirthDay();
        m_hireDate       = person.getHireDate();
        m_dismissionDate = person.getDismissionDate();
        m_dogovor        = person.getVidDogovora();
        m_zanyatost      = person.getVidZanyatosti();
        m_comment        = person.getComment();
        m_photo          = person.getPhoto();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CPerson::~CPerson()
    {

    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setSid(QString value)
    {
        m_sid = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setGroupSid(QString value)
    {
        m_groupSid = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setFirstName(QString value)
    {
        m_firstName = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setMidleName(QString value)
    {
        m_midleName = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setLastName(QString value)
    {
        m_lastName = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setKeyNumber(QString value)
    {
        m_keyNumber = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setTableNumber(QString value)
    {
        m_tableNumber = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setSnils(QString value)
    {
        m_snils = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setPosition(string position)
    {
        m_position = position;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setInn(string inn)
    {
        m_inn = inn;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setFailed(bool failed)
    {
        m_failed = failed;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setActivity(bool activity)
    {
        m_activity = activity;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setOrganisation(string org)
    {
        m_organisation = org;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setBirthDay(QDate date)
    {
        m_birthDay = date;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setHireDate(QDate date)
    {
        m_hireDate = date;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setDismissionDate(QDate date)
    {
        m_dismissionDate = date;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setLaborContract(string contract)
    {
        m_laborContract = contract;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setRecordId(QString recId)
    {
        m_recordId = recId;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setVidDogovora(QString dogovor)
    {
        m_dogovor = dogovor;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setVidZanyatosti(QString zanyatost)
    {
        m_zanyatost = zanyatost;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setComment(const QString& comment)
    {
        m_comment = comment;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CPerson::setPhoto(const QByteArray& photo)
    {
        m_photo = photo;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CPerson::getSid() const
    {
        return m_sid;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CPerson::getGroupSid() const
    {
        return m_groupSid;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CPerson::getFirstName() const
    {
        return  m_firstName;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CPerson::getMidleName() const
    {
        return m_midleName;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CPerson::getLastName() const
    {
        return m_lastName;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CPerson::getFullName() const
    {
        QString l_fullName(getFirstName().trimmed() + " " + \
                           getMidleName().trimmed() + " " + \
                           getLastName().trimmed());
        return l_fullName.trimmed();
    }
    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CPerson::getKeyNumber() const
    {
        return m_keyNumber;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CPerson::getTableNumber() const
    {
        return m_tableNumber;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CPerson::getSnils() const
    {
        return m_snils;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QDate CPerson::getBirthDay()        const
    {
        return m_birthDay;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QDate CPerson::getHireDate() const
    {
        return m_hireDate;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QDate CPerson::getDismissionDate () const
    {
        return m_dismissionDate;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CPerson::getVidDogovora()     const
    {
        return m_dogovor;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CPerson::getVidZanyatosti()   const
    {
        return m_zanyatost;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CPerson::getComment()         const
    {
        return m_comment;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QByteArray CPerson::getPhoto()      const
    {
        return m_photo;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getSidStd()         const
    {
        return getSid().toStdString();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getGroupSidStd()    const
    {
        return getGroupSid().toStdString();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getFirstNameStd()   const
    {
        return getFirstName().toStdString();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getMidleNameStd()   const
    {
        return getMidleName().toStdString();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getLastNameStd()    const
    {
        return getLastName().toStdString();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getKeyNumberStd()   const
    {
        return getKeyNumber().toStdString();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getTableNumberStd() const
    {
        return getTableNumber().toStdString();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getSnilsStd()       const
    {
        return getSnils().toStdString();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getPositionStd() const
    {
        return m_position;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getInnStd() const
    {
        return m_inn;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    bool CPerson::getFailedStd() const
    {
        return m_failed;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    bool CPerson::getActivity() const
    {
        return m_activity;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getOrganisationStd() const
    {
        return m_organisation;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getLaborContractStd() const
    {
        return m_laborContract;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CPerson::getRecordIdStd()      const
    {
        return m_recordId;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getVidDogovoraStd()   const
    {
        return m_dogovor.toStdString();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CPerson::getVidZanyatostiStd() const
    {
        return m_zanyatost.toStdString();
    }
}
