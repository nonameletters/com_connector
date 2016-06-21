#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QDate>
#include <iostream>
#include <QByteArray>

#include <QTextCodec>

#include "dolgnost.h"
#include "profile.h"
#include "logger.h"

using namespace std;
using namespace ns_logger;

namespace db_connector
{
    class CPerson
    {
        public:
            CPerson(QString sid         = "", QString groupSid = "", QString firstName = "", \
                    QString midleName   = "", QString lastName = "", QString keyNumber = "", \
                    QString tableNumber = "", QString snils = "");
            CPerson(const CPerson& person);
            virtual ~CPerson();

            void setSid(QString value);
            void setGroupSid(QString value);
            void setFirstName(QString value);
            void setMidleName(QString value);
            void setLastName(QString value);
            void setKeyNumber(QString value);
            void setTableNumber(QString value);
            void setSnils(QString value);
            void setPosRecId(const QString& value);
            void setPosNumber(const QString& value);
            void setPositionName(const QString& value);
            void setPosition(const CDolgnost& value);
            void setInn(string inn);
            void setFailed(bool failed);
            void setActivity(bool activity);
            void setOrganisation(string org);
            void setBirthDay(QDate date);
            void setHireDate(QDate date);
            void setDismissionDate(QDate date);
            void setLaborContract(string contract);
            void setRecordId(QString recId);
            void setVidDogovora(QString dogovor);
            void setVidZanyatosti(QString zanyatost);
            void setComment(const QString& comment);
            void setPhoto(const QByteArray& photo);
            void setFullFileName(const QString& fileName);
            void setProfile(const CProfile& value);

            QString getSid()             const;
            QString getGroupSid()        const;
            QString getFirstName()       const;
            QString getMidleName()       const;
            QString getLastName()        const;
            QString getFullName()        const;
            QString getKeyNumber()       const;
            QString getTableNumber()     const;            
            QString getSnils()           const;
            QString   getPosRecId()      const;
            QString   getPosNumber()     const;
            QString   getPosName()       const;
            CDolgnost getPosition()      const;
            QDate   getBirthDay()        const;
            QDate   getHireDate()        const;
            QDate   getDismissionDate () const;
            QString getVidDogovora()     const;            
            QString getVidZanyatosti()   const;
            QString getComment()         const;
            QByteArray   getPhoto()      const;
            QString getFullFileName()    const;
            CProfile getProfile()        const;

            string getSidStd()           const;
            string getGroupSidStd()      const;
            string getFirstNameStd()     const;
            string getMidleNameStd()     const;
            string getLastNameStd()      const;
            string getKeyNumberStd()     const;
            string getTableNumberStd()   const;
            string getSnilsStd()         const;
            string getPosNumberStd()     const;
            string getPositionStd()      const;
            string getInnStd()           const;
            bool   getFailedStd()        const;
            bool   getActivity()         const;
            string getOrganisationStd()  const;
            string getLaborContractStd() const;
            QString getRecordIdStd()     const;
            string getVidDogovoraStd()   const;
            string getVidZanyatostiStd() const;            

            friend ostream & operator<<(ostream & out, CPerson& p);
            friend CLogger*  operator<<(CLogger* l, const CPerson& p);

        private:
            QString m_sid;
            QString m_groupSid;
            QString m_firstName;
            QString m_midleName;
            QString m_lastName;
            QString m_keyNumber;
            QString m_tableNumber;
            QString m_snils;
//            QString m_posRecId;
//            QString m_posNumber;
//            string  m_position;
            string  m_inn;
            bool    m_failed;
            bool    m_activity;
            string  m_organisation;
            string  m_laborContract;
            QString m_recordId;
            QDate   m_birthDay;
            QDate   m_hireDate;
            QDate   m_dismissionDate;
            QString m_dogovor;
            QString m_zanyatost;
            QString m_comment;
            QByteArray m_photo;
            QString m_fullFileName;
            CDolgnost m_dolgnost;
            CProfile m_profile;
    };
}
#endif // PERSON_H
