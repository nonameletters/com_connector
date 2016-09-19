#include "mainwindow.h"
#include <QApplication>
#include <QAxObject>
#include <QTextCodec>
#include <QString>
#include <QDateTime>
#include <QTimer>

#include <windows.h>
#include <initguid.h>
#include <iostream>
#include <unknwn.h>
#include <comutil.h>
#include <new>
#include <set>
#include <map>
#include <fstream>

#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <QThread>


#include "constansts.h"

#include "logger.h"

#include "syncronizer.h"
#include "starter.h"

#include "runparams.h"

using namespace ns_logger;

using namespace std;
using namespace nsComConnector;
using namespace db_connector;

map<int, QString>    firedDeps;
map<QString,wstring> connections;


typedef ns_logger::CLogger llog;


//             Работающие    Уволенные
// МК          266           278
// ДЕНИР       267           279
// ТД          268           280
// ЭКСЦЕЛЬСИОР 269           281
// ФАБРИКА     270           282

// ---------- ---------- ---------- ---------- ---------- ----------
int main(int argc, char *argv[])
{
    // QTextCodec* codec = QTextCodec::codecForName("CP1251");

    firedDeps[266] = "278";
    firedDeps[267] = "279";
    firedDeps[268] = "280";
    firedDeps[269] = "281";
    firedDeps[270] = "282";


    connections.insert(make_pair("266", L"Srvr=srv-app;Ref=zup_prod;Usr=Администратор;Pwd=4lTa3vBH;"));
    connections.insert(make_pair("267", L"Srvr=srv-app;Ref=zup_denir_prod;Usr=Администратор;Pwd=4lTa3vBH;"));
    connections.insert(make_pair("268", L"Srvr=srv-app;Ref=zup_td;Usr=Admin1C;Pwd=admin1c;"));
    connections.insert(make_pair("269", L"Srvr=srv-app;Ref=zup_ekscelsior;Usr=Администратор;Pwd=4lTa3vBH;"));
    connections.insert(make_pair("270", L"Srvr=srv-app;Ref=zup_fabrika;Usr=Администратор;Pwd=4lTa3vBH;"));

//    CStarter l_starter(argc, argv);
//    l_starter.exec();

    //CRunParams p1(1, "First", f1);
    //CRunParams p2(2, "Second", f2);
    //CRunParams p3(3, "Third", f3);


    vector<CRunParams> l_params;
    l_params.push_back(CRunParams(1, "First  - desck", f1, argc, argv));
    l_params.push_back(CRunParams(2, "Second - desck", f2, argc, argv));
    l_params.push_back(CRunParams(3, "Third  - desck", f3, argc, argv));


    for_each(l_params.begin(), l_params.end(), CRunParams::execIfParam);

//    QThread* l_secThread = new QThread();

//    QTimer *l_timer = new QTimer();
//    QObject::connect(l_timer, &QTimer::timeout, updatePersonProfilesWithNull);
//    l_timer->start(30000);

//    l_timer->moveToThread(l_secThread);
//    l_secThread->start();





//    CSyncronizer sync(argc, argv);
//    sync.exec();



// ---------- ---------- ---------- ---------- ---------- ----------
// Next comment block is working block uncomment for productive
//    if (argc > 1)
//    {
//        // cout << " Argc is: " << argc << endl;
//        int l_firstParam = QString(argv[1]).toInt();
//        if (l_firstParam == 1)
//        {
//            cout << "Starting syncronization." << endl;
//            syncPersons();
//        }
//        else if(l_firstParam == 2)
//        {
//            cout << "Add person." << endl;
//            if (argc == 10)
//            {
//                // Нет СНИЛС
//                CPerson* l_newPerson = new CPerson();
//                l_newPerson->setGroupSid(argv[2]);
//                l_newPerson->setFirstName(codec->toUnicode(argv[3]));
//                l_newPerson->setMidleName(codec->toUnicode(argv[4]));
//                l_newPerson->setLastName(codec->toUnicode(argv[5]));
//                l_newPerson->setVidDogovora(codec->toUnicode(argv[6]));
//                l_newPerson->setVidZanyatosti(codec->toUnicode(argv[7]));
//                l_newPerson->setBirthDay(QDate::fromString(argv[8], "dd-MM-yyyy"));
//                l_newPerson->setTableNumber(codec->toUnicode(argv[9]));
//                l_newPerson->setSnils("temp"); // In DB SNILS not NULL;
//                addPerson(l_newPerson, argv[2]);

//            }
//            else if (argc == 11)
//            {
//                // Есть все параметры СНИЛС состоит из одной (цельной) последовательности
//                // символов
//                CPerson* l_newPerson = new CPerson();
//                l_newPerson->setGroupSid(argv[2]); // DB Id
//                l_newPerson->setFirstName(codec->toUnicode(argv[3])); // Familiya
//                l_newPerson->setMidleName(codec->toUnicode(argv[4])); // Imya
//                l_newPerson->setLastName(codec->toUnicode(argv[5])); // Otchestvo
//                l_newPerson->setVidDogovora(codec->toUnicode(argv[6]));
//                l_newPerson->setVidZanyatosti(codec->toUnicode(argv[7]));
//                l_newPerson->setBirthDay(QDate::fromString(argv[8], "dd-MM-yyyy"));
//                l_newPerson->setTableNumber(codec->toUnicode(argv[9])); // Table Number
//                l_newPerson->setSnils(argv[10]); // SNILS
//                addPerson(l_newPerson, argv[2]);
//            }
//            else if (argc == 12)
//            {
//                // Есть все параметры, СНИЛС состоит из двух частей 8 и 9 параметры
//                // символов
//                CPerson* l_newPerson = new CPerson();
//                l_newPerson->setGroupSid(argv[2]); // DB Id
//                l_newPerson->setFirstName(codec->toUnicode(argv[3])); // Familiya
//                l_newPerson->setMidleName(codec->toUnicode(argv[4])); // Imya
//                l_newPerson->setLastName(codec->toUnicode(argv[5])); // Otchestvo
//                l_newPerson->setVidDogovora(codec->toUnicode(argv[6]));
//                l_newPerson->setVidZanyatosti(codec->toUnicode(argv[7]));
//                l_newPerson->setBirthDay(QDate::fromString(argv[8], "dd-MM-yyyy"));
//                l_newPerson->setTableNumber(codec->toUnicode(argv[9])); // Table Number
//                l_newPerson->setSnils(QString(argv[10]) + " " + QString(argv[11])); // SNILS
//                addPerson(l_newPerson, argv[2]);

//            }
//            else
//            {
//                cout << "Wrong number of parameters" << endl;
//                cout << "Prog_name operation_type 1 2 3 4 5 6 7" << endl;
//                cout << "operation_type (1, 2, 3)" << endl;
//                cout << "1 - dataBase Id" << endl;
//                cout << "2 - Familiya" << endl;
//                cout << "3 - Imya" << endl;
//                cout << "4 - Otchestvo" << endl;
//                cout << "5 - Vid dogovora" << endl;
//                cout << "6 - Vid zanyatosty" << endl;
//                cout << "7 - BirthDay" << endl;
//                cout << "8 - TableNumber" << endl;
//                cout << "9 - SNILS" << endl;
//            }
//        }
//        else if (l_firstParam == 3)
//        {
//            cout << "Remove person." << endl;
//        }
//        else if (l_firstParam == 5)
//        {
//            cout << "Sync card numbers Legos VS Anviz" << endl;
//            syncCardNumLegosVSAnviz();
//        }
//        else if (l_firstParam == 4)
//        {
//            QTextCodec* l_c = QTextCodec::codecForName("CP1251");
//            OLL << "ADD PERSON BY READING FILES";

//            CConFile l_source;
//            vector<CPerson*> l_pList = l_source.getPersonList();
//            auto it = begin(l_pList);
//            for(; it != end(l_pList); it++)
//            {
//                OLL << "USER FILE NAME";
//                LL << l_c->fromUnicode((*it)->getFullFileName());
//                addPerson((*it), (*it)->getGroupSid());
//                QFile l_file((*it)->getFullFileName());
//                l_file.copy(prepareMoveFileName((*it)->getFullFileName()));
//                l_file.remove();
//            }

//            it = begin(l_pList);
//            for(; it != end(l_pList); it++)
//            {
//                if ((*it) != nullptr)
//                {
//                    delete (*it);
//                }
//            }
//        }
//        else if (l_firstParam == 9)
//        {
//            cout << "Starting syncronization with fired." << endl;
//            syncPersons(1);
//        }
//        else if (l_firstParam == 10)
//        {
//            cout << "Load ACCESS MATRIX from matrix.csv" << endl;
//            loadAccessMatrix();

//        }
//        else if (l_firstParam == 11)
//        {
//            CDbConnector con(TC);
//            con.updatePersonAccessDoors();
//        }
//        else if (l_firstParam == 12)
//        {
//            updatePersonProfiles();
//        }
//        else if (l_firstParam == 14)
//        {
//            // updatePersonProfilesWithNull();
//        }
//        else if (l_firstParam == 15)
//        {
//            int i;
//            cin >> i;
////            while (true)
////            {

////            }
//        }
//        else
//        {
//            cout << "Wrong param." << endl;
//            cout << "1 - start full syncronization." << endl;
//            cout << "2 - add person." << endl;
//            cout << "3 - remove person." << endl;
//            cout << "4 - add person from XML file from ./users dir." << endl;
//        }
//    }
//    else
//    {
//        cout << "Print help" << endl;
//        cout << "First param is a type of operation." << endl;
//        cout << "1 - start full syncronization." << endl;
//        cout << "2 - add person." << endl;
//        cout << "3 - remove person." << endl;
//        cout << "4 - add person from XML file from ./users dir." << endl;
//        cout << "// ---------- ---------- ---------- ---------- ---------- ----------" << endl;
//        cout << "// command line" << endl;
//        cout << "Prog_name operation_type 1 2 3 4 5 6 7" << endl;
//        cout << "operation_type (1, 2, 3)" << endl;
//        cout << "1 - dataBase Id" << endl;
//        cout << "2 - Familiya" << endl;
//        cout << "3 - Imya" << endl;
//        cout << "4 - Otchestvo" << endl;
//        cout << "5 - BirthDay" << endl;
//        cout << "6 - TableNumber" << endl;
//        cout << "7 - SNILS" << endl;
//    }

    cout << "Press Enter to continue..." << endl;
    llog::getInstance()->close();
    return 0;
}
