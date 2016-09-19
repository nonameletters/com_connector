#include "starter.h"


void updatePersonProfilesWithNull()
{
    OLL << "STARTED UPDATE PERSON PROFILE WITH NULL";
    CDbConnector l_timeControl(TC);

    vector<CPerson*> l_pl = l_timeControl.getPersonListWithNullProfile();
    size_t count = 0;
    auto it = begin(l_pl);
    for (; it != end(l_pl); it++)
    {
        count++;
        cout << "Updating person profile: " << count << endl;
        LL << *(*it);
        l_timeControl.updatePersonProfile(*it);
    }
}

void addUpdateUseresFromFolder()
{
    QTextCodec* l_c = QTextCodec::codecForName("CP1251");
    OLL << "ADD PERSON BY READING FILES";

    CConFile l_source;
    vector<CPerson*> l_pList = l_source.getPersonList();
    auto it = begin(l_pList);
    for(; it != end(l_pList); it++)
    {
        OLL << "USER FILE NAME";
        LL << l_c->fromUnicode((*it)->getFullFileName());
        addPerson((*it), (*it)->getGroupSid());
        QFile l_file((*it)->getFullFileName());
        l_file.copy(prepareMoveFileName((*it)->getFullFileName()));
        l_file.remove();
    }

    it = begin(l_pList);
    for(; it != end(l_pList); it++)
    {
        if ((*it) != nullptr)
        {
            delete (*it);
        }
    }
}

CStarter::CStarter(int argc, char *argv[]) : QtService<QCoreApplication>(argc, argv, APPNAME)
{
    l_profileCreation = nullptr;
    l_userSync        = nullptr;

    setServiceDescription(DESCRIPTION);
    setStartupType(QtServiceController::AutoStartup);
}

// ---------- ---------- ---------- ---------- ---------- ----------
void CStarter::start()
{
    QCoreApplication *app = application();
    app->setApplicationName(APPNAME);
    app->setOrganizationName(ORGANIZATION);
    app->addLibraryPath("C:\\Windows\\System32\\");
    app->addLibraryPath("plugins\\");

    QStringList l_path = app->libraryPaths();
    auto it = begin(l_path);
    for (;it != end(l_path); it++)
    {
        cout << (*it).toStdString() << endl;
    }

    if (l_profileCreation == nullptr)
    {
        l_profileCreation = new QTimer();
        QObject::connect(l_profileCreation, &QTimer::timeout, updatePersonProfilesWithNull);
        l_profileCreation->start(30000); // Every 30 seconds
    }

    if (l_userSync == nullptr)
    {
        l_userSync = new QTimer();
        QObject::connect(l_userSync, &QTimer::timeout, addUpdateUseresFromFolder);
        l_userSync->start(300000); // Every 5 minutes
    }
}

// ---------- ---------- ---------- ---------- ---------- ----------
void CStarter::stop()
{
    if (l_profileCreation != nullptr)
    {
        delete l_profileCreation;
    }

    if (l_userSync != nullptr)
    {
        delete l_userSync;
    }
}

