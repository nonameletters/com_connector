#include "syncronizer.h"

/** Name of this application */
#define APPNAME "1CSyncronizerS"

/** Publisher of this application */
#define ORGANISATION "nonameletters"

/** Short description of the Windows service */
#define DESCRIPTION "Синхронизация баз 1С с TimeControl"

namespace db_connector
{

    // ---------- ---------- ---------- ---------- ---------- ----------
    CSyncronizer::CSyncronizer(int argc, char *argv[])
        : QtService<QCoreApplication>(argc, argv, APPNAME)
    {
        setServiceDescription(DESCRIPTION);
        setStartupType(QtServiceController::AutoStartup);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CSyncronizer::start()
    {
        QCoreApplication* app = application();
        app->setApplicationName(APPNAME);
        app->setOrganizationName(ORGANISATION);
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CSyncronizer::stop()
    {

    }
}
