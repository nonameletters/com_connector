#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QDate>
#include <iostream>
#include <QByteArray>

using namespace std;

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
            void setPosition(string position);
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

            QString getSid()             const;
            QString getGroupSid()        const;
            QString getFirstName()       const;
            QString getMidleName()       const;
            QString getLastName()        const;
            QString getFullName()        const;
            QString getKeyNumber()       const;
            QString getTableNumber()     const;
            QString getSnils()           const;
            QDate   getBirthDay()        const;
            QDate   getHireDate()        const;
            QDate   getDismissionDate () const;
            QString getVidDogovora()     const;            
            QString getVidZanyatosti()   const;
            QString getComment()         const;
            QByteArray   getPhoto()      const;

            string getSidStd()           const;
            string getGroupSidStd()      const;
            string getFirstNameStd()     const;
            string getMidleNameStd()     const;
            string getLastNameStd()      const;
            string getKeyNumberStd()     const;
            string getTableNumberStd()   const;
            string getSnilsStd()         const;
            string getPositionStd()      const;
            string getInnStd()           const;
            bool   getFailedStd()        const;
            bool   getActivity()         const;
            string getOrganisationStd()  const;
            string getLaborContractStd() const;
            QString getRecordIdStd()     const;
            string getVidDogovoraStd()   const;
            string getVidZanyatostiStd() const;            

            friend ostream & operator << (ostream & out, CPerson& p);
//            {
//                out << " // ---------- ---------- ---------- ---------- ---------- ---------- " << endl;
//                out << " SID      : " << p.getSidStd() << endl;
//                out << " NAME     : " << p.getFullName().toStdString() << endl;

//                auto it = begin(l_list);
//                for(; it != end(l_list); it++)
//                {
//                    out << (*(*it));
//                }

                // out << p.getSidStd() << " " << p.getGroupSidStd() << " " << p.getFirstNameStd() << " " << p.getMidleNameStd() << " " << p.getLastNameStd() << " " << p.getKeyNumberStd() << " " << p.getTableNumberStd() << " " << p.getSnilsStd();
//                return out;
//            }

        private:
            QString m_sid;
            QString m_groupSid;
            QString m_firstName;
            QString m_midleName;
            QString m_lastName;
            QString m_keyNumber;
            QString m_tableNumber;
            QString m_snils;
            string  m_position;
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
    };
}
#endif // PERSON_H
