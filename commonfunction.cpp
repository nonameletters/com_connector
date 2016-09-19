#include "commonfunction.h"

// ---------- ---------- ---------- ---------- ---------- ----------
void addPerson(CPerson* person, QString organizationId)
{
    if (person == nullptr)
    {
        return;
    }

    // Надо сделать проверку на существование пользователя.
    CDbConnector anvizCon(TC);
    updateActivePerson(person, &anvizCon, organizationId);
    // anvizCon.addPerson(person, person->getGroupSid());
}

// ---------- ---------- ---------- ---------- ---------- ----------
void updateActivePerson(CPerson* person, CDbConnector* con, QString organizationId)
{
    if ((person == nullptr) || (con == nullptr))
    {
        OLL << "updateActivePerson: nullptr";
        return;
    }

    // Получаем список сотрудников с одинаковыми ФИО
    OLL << "\tSTART SYNC PERSON";
    LL << *person;
    // llog::getInstance()->log(toWinStr(person->getFullName()).c_str());

    if (person->getFullName().isEmpty())
    {
        OLL << "\tPerson have an empty name. Stop sync such person.";
        return;
    }

    vector<CPerson*> l_acsList = con->getPersonListByPerson(person);
    vector<CPerson*> l_pListByTn = con->getPersonListByTabNumber(person->getTableNumber());
    // vector<CPerson*> l_updatable;

    // QString l_tMesg = "\tVariants by FIO: " + l_acsList.size();
    LL << QString("\tVariants by FIO: ").append(QString::number(l_acsList.size()));
    // l_tMesg = "\tVariants by TAB NUM: "  + l_pListByTn.size();
//    LL << (QString("\tVariants by TAB NUM: ") + QString::number(l_pListByTn.size()));
    LL << QString("\tVariants by TAB NUM: ").append(QString::number(l_pListByTn.size()));


    // person - is a user that was readed from 1C base
    // we search the same user in DB and update
    if ((l_acsList.empty() == true) && (l_pListByTn.empty() == true))
    {
        // В базе СКУД не найдено пользователя с заданными ФИО
        // создаем нового пользователя.
        try
        {
            OLL << "\tAdding new person.";
            con->addPerson(person, organizationId);
            // CLogger::getInstance()->log("ADD new person");
        }
        catch (CDbExcep& ex)
        {
            cout << ex.what() << endl;
        }
    }
    else if (l_acsList.empty() == true)
    {
        updatePersonProcedure(person, con, l_pListByTn, organizationId);
    }
    else if (l_pListByTn.empty() == true)
    {
        updatePersonProcedure(person, con, l_acsList, organizationId);
    }
    else
    {
        updatePersonProcedure(person, con, l_pListByTn, organizationId);
    }

    OLL << "\tEND SYNC CURRENT PERSON.";
}

// ---------- ---------- ---------- ---------- ---------- ----------
QString prepareMoveFileName(const QString& fileName)
{
    int l_pos = fileName.length() - 1;
    while ((l_pos >= 0) && (fileName.at(l_pos) != '/'))
    {
        l_pos--;
    }

    if ((l_pos == 0) || (l_pos == fileName.length()))
    {
        return fileName;
    }

    QString l_resStr = fileName;
    l_resStr.insert(l_pos + 1, "updated/");
    return l_resStr;
}

// ---------- ---------- ---------- ---------- ---------- ----------
void updatePersonProcedure(CPerson* person, CDbConnector* con, vector<CPerson*> pList, QString organizationId)
{
    QTextCodec* c = QTextCodec::codecForName("CP1251");
    OLL << "\t\tUpdating EXIST person.";

    CDolgnost l_d = con->getDolgnostByCode(person->getPosNumber());

    if ((l_d.getRecId() == "") || (l_d.getRecId() == "0"))
    {
        l_d = con->createNewPosition(person->getPosition());
    }

    person->setPosition(l_d);

    // Пробегаем список пользователей, полученных из СКУД, совпавших по ФИО
    auto it = begin(pList);
    for (;it != end(pList); it++)
    {
        // Сотрудники с ОСНОВНЫМ МЕСТОМ РАБОТЫ
        // QString l_mainWinStr = toWinStr(g_zanMain).c_str();
        // QString l_mainWinStr = g_zanMain;
        // CLogger::getInstance()->log("Vid zanyatosti.");
        // CLogger::getInstance()->log(c->fromUnicode(person->getVidZanyatosti()));


        OLL << "\t\tPERSON VZ";
        LL << c->fromUnicode(QString("\t\t").append(person->getVidZanyatosti()));
        OLL << "\t\tCOMPARABLE VZ";
        LL << c->fromUnicode(QString("\t\t").append(g_zanMain));

        if (person->getVidZanyatosti().compare(g_zanMain) == 0)
        {
            OLL << "\t\tSYNC MAIN WORKPLACE.";
            syncMain(person, con, (*it), organizationId);
        }
        // Сотруник либо на ВНУТРЕННЕМ СОВМЕСТИТЕЛЬСТВЕ, либо на ВНЕШНЕМ СОВМЕСТИТЕЛЬСТВЕ, либо на ДОГОВОРЕ ПОДРЯДА.
        // Если на ВНУТРЕННЕМ СОВМЕСТИТЕЛЬСТВЕ ничего не делаем, т.к. точно есть запись по
        // основному месту работы. Проверяем работает ли он по ДОГОВОРУ ПОДРЯДА, либо на ВНЕШЕМ СОВМЕСТИТЕЛЬСТВЕ
        else
        {
            OLL << "\t\tSYNC DOGOVOR WORKPLACE.";
            syncDogovora(person, con, (*it), organizationId);
        }

    }
}

// ---------- ---------- ---------- ---------- ---------- ----------
void syncMain(CPerson* person, CDbConnector* con, CPerson* dbPerson, QString organizationId)
{
    CPerson* l_curPerson = dbPerson;

    con->updatePersonProfile(l_curPerson);

    // OLL << "1C PERSON";
    // LL << *person;
    OLL << "\t\t\tDB PERSON";
    LL << *l_curPerson;
    // Проверяем заполнены ли дата рождения и СНИЛС у пользователя
    // если одно из полей не заполнено, то производим остальные проверки
    if ((l_curPerson->getBirthDay().isValid() == false) || \
        (l_curPerson->getSnils().isEmpty() == true))
    {
        if (l_curPerson->getTableNumber().compare(person->getTableNumber()) == 0)
        {
            // Табельные номера совпадают, значит это один и тот же человек
            // Обновляем записи по ДР и СНИЛС
            if (con->isFired(l_curPerson))
            {
                con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
            }
            con->updateBirthDay(l_curPerson->getSid(), person->getBirthDay());
            con->updateSnils(l_curPerson->getSid(), person->getSnils());

            if (l_curPerson->getPosNumber() != person->getPosNumber() )
            {
                con->updatePersonPosition(l_curPerson->getSid(), person);
            }

            CLogger::getInstance()->log("UPDATE BD&S");
        }
        else
        {
            // Табельные номера отличаются, т.к. не можем точно идентифицировать пользователя
            // Проверяем совпадение СНИЛС и дня рождения. Если один из параметров совпадает, то скорее
            // всего это тот человек который нам нужен, обновляем основные параметры
            if ((person->getSnils().compare(l_curPerson->getSnils()) == 0) || \
                (person->getBirthDay() == l_curPerson->getBirthDay()))
            {
                if (con->isFired(l_curPerson))
                {
                    con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                }
                con->updateTableNumber(l_curPerson->getSid(), person->getTableNumber());
                con->updateBirthDay(l_curPerson->getSid(), person->getBirthDay());
                con->updateSnils(l_curPerson->getSid(), person->getSnils());
                if (l_curPerson->getPosNumber() != person->getPosNumber() )
                {
                    con->updatePersonPosition(l_curPerson->getSid(), person);
                }
            }
            else
            {
                // У записи совпадает только ФИО, скорее всего это либо битая запись
                // либо есть другой человек с таким табельным номером. Выдаем ошибку.
                CLogger::getInstance()->log("SYNC ERROR MAIN: Table numbers are diffirent. Snils and birth day is not valid.");
            }
        }
    }
    else
    {
        // У пользователя заполнены и СНИЛС и Дата Рождения идентифицируем
        // его по этим данным.
        if ((l_curPerson->getBirthDay() == person->getBirthDay()) && \
            (l_curPerson->getSnils().compare(person->getSnils()) == 0))
        {
            // Совпадает СНИЛС и Дата рожедения, человек на основном месте работы.
            // Если табельные номера отличаются, то обновим их, т.к. человек на основном
            // месте. Если табелные номера совпадают, то записи полностью идентичные и ничего
            // не делаем.

            if (person->getTableNumber().compare(l_curPerson->getTableNumber()) != 0)
            {
                // Обновляем табельный номер.
                con->updateTableNumber(l_curPerson->getSid(), person->getTableNumber());
                con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                CLogger::getInstance()->log("UPDATE Table Number");
            }
            else
            {
                if (con->isFired(l_curPerson))
                {
                    con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                }
            }

//            OLL << "1";
//            LL << l_curPerson->getPosNumber();
//            OLL << "2";
//            LL << person->getPosNumber();
            if (l_curPerson->getPosNumber() != person->getPosNumber() )
            {
                con->updatePersonPosition(l_curPerson->getSid(), person);
//              OLL << "Try to update";
            }
//            else
//            {
//                OLL << "POSITON ARE EAQUAL";
//            }
            OLL << "END SYNC PERSON MAIN: SNILS and BIRTH DAY are EQUAL";
            return;
        }

        // Дата рождения совпадает, но СНИЛС разный.
        // Вероятность ошибки при вводе СНИЛС более высока нежли чем даты рожедния
        // обновляем СНИЛС у пользователя
        if ((l_curPerson->getBirthDay() == person->getBirthDay()) && \
            (l_curPerson->getSnils().compare(person->getSnils()) != 0))
        {
            // Если табельные номера различны, то скорее всего человер раньше был либо совместитель
            // либо по договору. Т.к. ФИО и Дата рождения равны, то скорее всего, что это один и тот же
            // человек. Обновим табельный номер и СНИЛС т.к. человек на основном месте работы.
            if (person->getTableNumber().compare(l_curPerson->getTableNumber()) != 0)
            {
                // Обновляем табельный номер.
                con->updateTableNumber(l_curPerson->getSid(), person->getTableNumber());

                // Обновляем СНИЛС
                con->updateSnils(l_curPerson->getSid(), person->getSnils());
                con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                CLogger::getInstance()->log("UPDATE TN&S");
            }
            else
            {
                // Табельные номера совпали, есть ошибка только в СНИЛС обновим его.
                // Обновляем только СНИЛС
                if (con->isFired(l_curPerson))
                {
                    con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                }
                con->updateSnils(l_curPerson->getSid(), person->getSnils());
                CLogger::getInstance()->log("S");
            }

            if (l_curPerson->getPosNumber() != person->getPosNumber() )
            {
                con->updatePersonPosition(l_curPerson->getSid(), person);
            }
            return;
        }

        // Дата рождения различается, но СНИЛС одинаковый
        // Возможнжо была ошибка при передаче дня рожедения
        // или ошибка во время предыдущей синхронизации
        // (некорректно выгружился ДР ранее 1980)
        if ((l_curPerson->getBirthDay() != person->getBirthDay()) && \
            (l_curPerson->getSnils().compare(person->getSnils()) == 0))
        {
            // Если табельные номера различны, то скорее всего человек раньше был либо совместитель
            // либо по договору. Т.к. ФИО и Дата рождения равны, то скорее всего, что это один и тот же
            // человек. Обновим табельный номер и СНИЛС т.к. человек на основном месте работы.
            if (person->getTableNumber().compare(l_curPerson->getTableNumber()) != 0)
            {
                // Обновляем табельный номер.
                con->updateTableNumber(l_curPerson->getSid(), person->getTableNumber());
                // Обновляем Дату рожедения
                con->updateBirthDay(l_curPerson->getSid(), person->getBirthDay());
                con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                CLogger::getInstance()->log("UPDATE TN&S");
            }
            else
            {
                // Табельные номера совпали, есть ошибка только в дате рождения обновим ее.
                // Обновляем только дату рождения.
                con->updateBirthDay(l_curPerson->getSid(), person->getBirthDay());
                if (con->isFired(l_curPerson))
                {
                    con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                }
                CLogger::getInstance()->log("S");
            }

            if (l_curPerson->getPosNumber() != person->getPosNumber() )
            {
                con->updatePersonPosition(l_curPerson->getSid(), person);
            }
            return;
        }
    }
}

// ---------- ---------- ---------- ---------- ---------- ----------
void syncDogovora(CPerson* person, CDbConnector* con, CPerson* dbPerson, QString organizationId)
{
    // (*LL) << "DOGOVOR PODRYADA";
//                QString l_podryadWinStr = toWinStr(g_dogPodryad).c_str();
//                QString l_sovmWinStr = toWinStr(g_zanSovm).c_str();
    // QString l_podryadWinStr = g_dogPodryad;
    // QString l_sovmWinStr = g_zanSovm;

// I don't remember why i check g_dogPodryad, but now it seem to me that it is not necessary
//    if ((person->getVidDogovora().compare(g_dogPodryad) == 0) ||
//        (person->getVidZanyatosti().compare(g_zanSovm) == 0))
//    {
//        OLL << "\t\t\t Vid raboty ne sootvetstvuet. Exit sync dogovora.";
//        return;
//    }

    if (person->getVidZanyatosti().compare(g_zanSovm) == 0)
    {
        OLL << "\t\t\t Vid raboty ne sootvetstvuet. Exit sync dogovora.";
        return;
    }

    CPerson* l_curPerson = dbPerson;

    con->updatePersonProfile(l_curPerson);
//    QTextCodec* codec = QTextCodec::codecForName("CP1251");

    OLL << "\t\t\tDB PERSON";
    LL << *l_curPerson;

//    (*LL) << "1 - person; 2 - dbPerson";
//    LL << codec->fromUnicode(person->getFullName());
//    LL << codec->fromUnicode(l_curPerson->getFullName());
    // Проверяем заполнены ли дата рождения и СНИЛС у пользователя
    // если одно из полей не заполнено, то производим остальные проверки
    if ((l_curPerson->getBirthDay().isValid() == false) || \
        (l_curPerson->getSnils().isEmpty() == true))
    {
        if (l_curPerson->getTableNumber().compare(person->getTableNumber()) == 0)
        {
            // Табельные номера совпадают, значит это один и тот же человек
            // Обновляем записи по ДР и СНИЛС
            con->updateBirthDay(l_curPerson->getSid(), person->getBirthDay());
            con->updateSnils(l_curPerson->getSid(), person->getSnils());

            if (l_curPerson->getFullName() != person->getFullName())
            {
                con->updateNameParams(l_curPerson->getSid(), person);
                con->updatePersonPosition(l_curPerson->getSid(), person);
            }

            if (con->isFired(l_curPerson))
            {
                con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
            }

            if (l_curPerson->getPosNumber() != person->getPosNumber() )
            {
                con->updatePersonPosition(l_curPerson->getSid(), person);
            }
            CLogger::getInstance()->log("UPDATE DOGOVOR BD&S");
        }
        else
        {
            // Табельные номера отличаются, т.к. не можем точно идентифицировать пользователя
            // Проверяем совпадение СНИЛС и дня рождения. Если один из параметров совпадает, то скорее
            // всего это тот человек который нам нужен, обновляем основные параметры
            // Т.К. человек работает по ДОГОВОРУ ПОДРЯДА, то скорее всего табельные номера различаются
            // в связи с тем что это запись с табельным номером по основному месту работы
            // пропустим обновление табельного номера
            if ((person->getSnils().compare(l_curPerson->getSnils()) == 0) || \
                (person->getBirthDay() == l_curPerson->getBirthDay()))
            {
                // con->updateTableNumber(l_curPerson->getSid(), person->getTableNumber());
                con->updateBirthDay(l_curPerson->getSid(), person->getBirthDay());
                con->updateSnils(l_curPerson->getSid(), person->getSnils());
                con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                if (l_curPerson->getFullName() != person->getFullName())
                {
                    con->updateNameParams(l_curPerson->getSid(), person);
                }

                if (l_curPerson->getPosNumber() != person->getPosNumber() )
                {
                    con->updatePersonPosition(l_curPerson->getSid(), person);
                }
            }
            else
            {
                // У записи совпадает только ФИО, скорее всего это либо битая запись
                // либо есть другой человек с таким табельным номером. Выдаем ошибку.
                CLogger::getInstance()->log("SYNC ERROR SOVM: Table numbers are diffirent. Snils and birth day is not valid.");
                return;
            }
        }
    }
    else
    {
        // У пользователя заполнены и СНИЛС и Дата Рождения идентифицируем
        // его по этим данным.
        if ((l_curPerson->getBirthDay() == person->getBirthDay()) && \
            (l_curPerson->getSnils().compare(person->getSnils()) == 0))
        {
            // Совпадает СНИЛС и Дата рожедения, человек работает по ДОГОВОРУ ПОДРЯДА.
            // Т.к. СНИЛС и Дата рождения совпали, и челове работает по ДОГОВОРУ ПОДРЯДА,
            // обновлять табельный номер не надо. Проверяем только заполнен он или нет
            // если табельный номер не заполнен, то заполняем

            if (l_curPerson->getTableNumber().isEmpty() == true)
            {
                // Обновляем табельный номер.
                con->updateTableNumber(l_curPerson->getSid(), person->getTableNumber());
                con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                if (l_curPerson->getFullName() != person->getFullName())
                {
                    con->updateNameParams(l_curPerson->getSid(), person);
                }
                CLogger::getInstance()->log("UPDATE Table Number");
            }
            else
            {
                if (l_curPerson->getFullName() != person->getFullName())
                {
                    con->updateNameParams(l_curPerson->getSid(), person);
                    con->updatePersonPosition(l_curPerson->getSid(), person);
                }

                if (con->isFired(l_curPerson))
                {
                    con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                }
            }

            if (l_curPerson->getPosNumber() != person->getPosNumber() )
            {
                con->updatePersonPosition(l_curPerson->getSid(), person);
            }

            OLL << "END SYNC PERSON DOG: SNILS and BIRTH DAY are EQUAL";
            return;
        }

        // Дата рождения совпадает, но СНИЛС разный.
        // Вероятность ошибки при вводе СНИЛС более высока нежли чем даты рожедния
        // обновляем СНИЛС у пользователя
        if ((l_curPerson->getBirthDay() == person->getBirthDay()) && \
            (l_curPerson->getSnils().compare(person->getSnils()) != 0))
        {
            // Приходим к тому что есть различев в СНИЛС, но более вероятно, что это один
            // и тот же человек т.к. ФИО равны и Даты рождения равны.
            // В связи с тем что человек на ДОГОВОРЕ ПОДРЯДА обновим табельный номер
            // только в том случае если он не заполнен
            if (l_curPerson->getTableNumber().isEmpty() == true)
            {
                // Обновляем табельный номер.
                con->updateTableNumber(l_curPerson->getSid(), person->getTableNumber());
                // Обновляем СНИЛС
                con->updateSnils(l_curPerson->getSid(), person->getSnils());
                con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                con->updatePersonPosition(l_curPerson->getSid(), person);
                if (l_curPerson->getFullName() != person->getFullName())
                {
                    con->updateNameParams(l_curPerson->getSid(), person);
                }

                CLogger::getInstance()->log("UPDATE TN&S");
            }
            else
            {
                if (l_curPerson->getFullName() != person->getFullName())
                {
                    con->updateNameParams(l_curPerson->getSid(), person);
                }

                if (con->isFired(l_curPerson))
                {
                    con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                }
                con->updatePersonPosition(l_curPerson->getSid(), person);
            }

            if (l_curPerson->getPosNumber() != person->getPosNumber() )
            {
                con->updatePersonPosition(l_curPerson->getSid(), person);
            }
        }

        // Возможно были ошибки при передаче ДР.
        // СНИЛС однинаковый, скорее всего это нужный нам человек
        if ((l_curPerson->getBirthDay() != person->getBirthDay()) && \
            (l_curPerson->getSnils().compare(person->getSnils()) == 0))
        {
            // Т.к. человек на ДОГОВОРЕ ПОДРЯДА обновим табельный номер,
            // только если он не заполнен
            if (person->getTableNumber().isEmpty() == true)
            {
                // Обновляем табельный номер.
                con->updateTableNumber(l_curPerson->getSid(), person->getTableNumber());
                // Обновляем Дату рожедения
                con->updateBirthDay(l_curPerson->getSid(), person->getBirthDay());
                con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                if (l_curPerson->getFullName() != person->getFullName())
                {
                    con->updateNameParams(l_curPerson->getSid(), person);
                }

                CLogger::getInstance()->log("PODR UPDATE TN&S");
            }
            else
            {
                // Табельные номера совпали, есть ошибка только в дате рождения обновим ее.
                // Обновляем только дату рождения.
                con->updateBirthDay(l_curPerson->getSid(), person->getBirthDay());
                con->updatePersonPosition(l_curPerson->getSid(), person);
                if (con->isFired(l_curPerson))
                {
                    con->updateDepartmentNumber(l_curPerson->getSid(), organizationId);
                }

                if (l_curPerson->getFullName() != person->getFullName())
                {
                    con->updateNameParams(l_curPerson->getSid(), person);
                }
                CLogger::getInstance()->log("PODR S");
            }

            if (l_curPerson->getPosNumber() != person->getPosNumber() )
            {
                con->updatePersonPosition(l_curPerson->getSid(), person);
            }
            return;
        }
    }
    //}
}

// ---------- ---------- ---------- ---------- ---------- ----------
void loadAccessMatrix()
{
    fstream l_file;
    l_file.open("matrix.csv", ios_base::in);
    if (!l_file.is_open())
    {
        cout << "File \"matrix.csv\" is not opened." << endl;
        return;
    }

    vector<vector<string> > l_accessMatrix;
    string l_curLine;
    int count = 0;
    while (getline(l_file, l_curLine))
    {
        string l_curVal;
        vector<string> l_mRow;
        for(size_t i = 0; i < l_curLine.size(); ++i)
        {
            if (l_curLine[i] == ';')
            {
                if (l_curVal.empty())
                {
                    l_mRow.push_back("0");
                }
                else
                {
                    l_mRow.push_back(l_curVal);
                }
                l_curVal.clear();
                continue;
            }
            l_curVal = l_curVal + l_curLine[i];
        }


        if (!l_mRow.empty())
        {
            l_accessMatrix.push_back(l_mRow);
        }
        ++count;
        cout << count << endl;
    }

    count = 0;
    for(size_t i = 0; i < l_accessMatrix.size(); ++i)
    {
        ++count;
        for (size_t j = 0; j < l_accessMatrix[i].size(); ++j)
        {
            if ((j == 0) || (j == 1))
                cout << "" << count << "\t " <<"column ";
            else
                cout << l_accessMatrix[i][j] << " ";
        }
        cout << endl;
    }

    CDbConnector l_timeControl(TC);
    l_timeControl.clearAccessMatrix();
    for (size_t i = 0; i < l_accessMatrix.size(); ++i)
    {
        l_timeControl.addLineToAccessMatrix(l_accessMatrix[i]);
    }
}

// ---------- ---------- ---------- ---------- ---------- ----------
void updatePersonProfiles()
{
    CDbConnector l_timeControl(TC);

    vector<CPerson*> l_pl = l_timeControl.getPersonList();
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

// ---------- ---------- ---------- ---------- ---------- ----------
void updateFiredPerson(CPerson* person, CDbConnector* con, QString organizationId)
{
    OLL << "Start update fired person";
    if ((person == nullptr) || (con == nullptr))
    {
        OLL << "UpdatePassivePerson: nullptr";
        return;
    }

    vector<CPerson*> l_pList = con->getPersonListByTabNumber(person->getTableNumber());
    auto it = begin(l_pList);
    for(;it != end(l_pList); ++it)
    {
        con->updatePersonProfile(*it);
        con->updateDepartmentNumber((*it)->getSid(), getFireDepartment(organizationId));
    }
}

// ---------- ---------- ---------- ---------- ---------- ----------
QString getFireDepartment(const QString& dep)
{
    //             Работающие    Уволенные
    // МК          266           278
    // ДЕНИР       267           279
    // ТД          268           280
    // ЭКСЦЕЛЬСИОР 269           281
    // ФАБРИКА     270           282


    auto it = firedDeps.find(dep.toInt());

    if (it == firedDeps.end())
    {
        return dep;
    }
    else
    {
        return (*it).second;
    }
}

// ---------- ---------- ---------- ---------- ---------- ----------
void clearPersonVector(vector<CPerson*>& vec)
{
    auto it = begin(vec);
    for (; it != end(vec); it++)
    {
        delete (*it);
    }

    vec.clear();
}

// ---------- ---------- ---------- ---------- ---------- ----------
void syncPersonsListWithFired(CDbConnector& connection, vector<CPerson*>& persons, QString organizationId)
{
    vector<CPerson*> activePersons;
    vector<CPerson*> passivePersons;


    QString l_mes = "START update users WHIT FIRED: " + QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
    LL << l_mes;

    int count = 1;
    vector<CPerson*>::iterator it = persons.begin();
    for (; it != persons.end(); it++)
    {
        QString l_mes = QString("UPDATING: ") + QString::number(count) + " of " + QString::number(persons.size());
        LL << l_mes;
        CPerson* l_person = (*it);
        // Делаем два списка пользователей: работающие и уволенные
        // В СКУД синхронизруем работающих пользователай затем
        // по списку уволенных проверяем есть ли они в списке работающих
        // если пользователя нет в списке работающих, то удаляем его из СКУД
        if (l_person->getActivity() == true)
        {
            activePersons.push_back(l_person);
            updateActivePerson(l_person, &connection, organizationId);
        }
        else
        {
            passivePersons.push_back(l_person);
            // updateFiredPerson(l_person, &connection, organizationId);
        }

        count++;
    }

    // Работающие пользователи синхронизованы.
    // Перемещаем уволенных в нужную папку.
    cout << "Passive persons count: " << passivePersons.size() << endl;
    it = passivePersons.begin();
    count = 0;
    for(; it != passivePersons.end(); ++it)
    {
        count++;
        pv comparator;
        comparator.toFind = (*it);
        auto curIt = find_if(activePersons.begin(), activePersons.end(), comparator);

        if (curIt != activePersons.end())
        {
            cout << "Person not found: " << count << endl;
            continue;
        }

        updateFiredPerson(*it, &connection, organizationId);
    }

    l_mes = "END update users WITH FIRED: " + QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
    CLogger::getInstance()->log(l_mes.toStdString().c_str());
}

// ---------- ---------- ---------- ---------- ---------- ----------";
void syncPersons(int type)
{
    QString l_mes("WHOLE SYNC START TIME: " + QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
    LL << l_mes;

    QString l_id     = "";
    wstring l_conStr = L"";

    CDbConnector anvizCon(TC);

    map<QString, wstring>::iterator it = begin(connections);
    for(;it != end(connections); it++)
    {
        l_id     = (*it).first;
        l_conStr = (*it).second;

        OLL << "// ---------- ---------- ---------- ---------- ---------- ----------";
        CComConnector mCon;
        mCon.makeConnect("V83.COMConnector.1", l_conStr);

        vector<CPerson*> empl = mCon.getEmployeis(getPrefixById(l_id));
        OLL << "1C DB READING FINISHED";
        if (type == 0)
        {
            syncPersonsList(anvizCon, empl, l_id);
        }
        else
        {
            syncPersonsListWithFired(anvizCon, empl, l_id);
        }

        clearPersonVector(empl);

    }
    //    Connect ot 1C and out persons


    l_mes = "WHOLE SYNC END TIME: " + QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
    LL << l_mes;

    cout << "Sincronization is finished." << endl;
}

// ---------- ---------- ---------- ---------- ---------- ----------
void syncPersonsList(CDbConnector& connection, vector<CPerson*>& persons, QString organizationId)
{
//    vector<CPerson*> activePersons;
//    vector<CPerson*> passivePersons;


    QString l_mes = "START update users: " + QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
    LL << l_mes;

    int count = 1;
    vector<CPerson*>::iterator it = persons.begin();
    for (; it != persons.end(); it++)
    {
        QString l_mes = QString("UPDATING: ") + QString::number(count) + " of " + QString::number(persons.size());
        LL << l_mes;
        CPerson* l_person = (*it);
        // Делаем два списка пользователей: работающие и уволенные
        // В СКУД синхронизруем работающих пользователай затем
        // по списку уволенных проверяем есть ли они в списке работающих
        // если пользователя нет в списке работающих, то удаляем его из СКУД
        if (l_person->getActivity() == true)
        {
            // activePersons.push_back(l_person);
            updateActivePerson(l_person, &connection, organizationId);
        }
//        else
//        {
//            passivePersons.push_back(l_person);
//            // updateFiredPerson(l_person, &connection, organizationId);
//        }

        count++;
    }

    l_mes = "END update users: " + QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
    CLogger::getInstance()->log(l_mes.toStdString().c_str());
}

// ---------- ---------- ---------- ---------- ---------- ----------
QString getPrefixById(QString id)
{
    QString l_prefix;

    if (id.compare("266") == 0)
    {
        l_prefix = "";
    }
    else if (id.compare("267") == 0)
    {
        l_prefix = "ДЭН";
    }
    else if (id.compare("268") == 0)
    {
        l_prefix = "ТД";
    }
    else if (id.compare("269") == 0)
    {
        l_prefix = "Э";
    }
    else if (id.compare("270") == 0)
    {
        l_prefix = "ФБ";
    }
    else
    {
        l_prefix = "";
    }

    return l_prefix;
}

// ---------- ---------- ---------- ---------- ---------- ----------
void syncCardNumLegosVSAnviz()
{
    CDbConnector legosCon(LEGOS);
//    CDbConnector anvizCon(ANVIZ);
    CDbConnector anvizCon(TC);

    cout << "Start GetPersons" << endl;
    vector<CPerson*> l_legPersons = legosCon.getPersonList();
    cout << "Persons list size: " << l_legPersons.size() << endl;

    int count = 0;
    auto it = begin(l_legPersons);
    for (;it != end(l_legPersons); it++)
    {
        cout << "Updating " << count << " of " << l_legPersons.size() << endl;
        // anvizCon.updateKeyByTableNumber((*it)->getTableNumber(), (*it)->getKeyNumber());
        anvizCon.updateKeyAndCommentByTableNumber((*it)->getTableNumber(), (*it)->getKeyNumber(), (*it)->getComment());
        count++;
    }
}


// ---------- ---------- ---------- ---------- ---------- ----------
HRESULT initCom(string comServerName, IDispatch** outInstanceDispatch)
{
    if (FAILED(CoInitialize(NULL)))
    {
        cout << "ComOle initialization FAILED" << endl;
    }

    CLSID v83Sid;

    WCHAR szWideProgId[128];
    long lLen = MultiByteToWideChar(CP_ACP, 0, comServerName.c_str(), strlen(comServerName.c_str()), szWideProgId, sizeof(szWideProgId));
    szWideProgId[lLen] = '\0';

    HRESULT hr = CLSIDFromProgID(szWideProgId, &v83Sid);

    if (FAILED(hr))
    {
        cout.setf(ios::hex, ios::basefield);
        cout << "Failed get ID by name: " << comServerName << endl;
        cout.unsetf(ios::showbase);
    }
    else
    {
        cout << "CLSID SUCCESSFULLY FOUND" << endl;
    }

    IClassFactory* pCF;
    hr = CoGetClassObject(v83Sid, CLSCTX_INPROC, NULL, IID_IClassFactory, (void**) &pCF);

    if (FAILED(hr))
    {
        cout << "FAILED: CoGetClassObject" << endl;
    }

    hr = pCF->CreateInstance(NULL, IID_IDispatch, (void**) outInstanceDispatch);

    if (FAILED(hr))
    {
        cout << "FAILED: CreateInstance" << endl;
    }

    pCF->Release();

    pCF = NULL;
    return hr;
}

// ---------- ---------- ---------- ---------- ---------- ----------
HRESULT execInvoke(wstring methodName, wstring param, WORD objType, int paramNum, IDispatch* parent, VARIANT* inOutRes)
{
    if (parent == NULL)
    {
        return E_POINTER;
    }

    DISPID dispIdInit;
    OLECHAR* fName = new OLECHAR[methodName.size() + 1];
    wcsncpy(fName, methodName.c_str(), methodName.size() + 1);


    HRESULT hr;
    hr = parent->GetIDsOfNames(IID_NULL, &fName,1, LOCALE_SYSTEM_DEFAULT, &dispIdInit);
    delete[] fName;

    if (FAILED(hr))
    {
        cout << "GetIDsOfNames: " << methodName.c_str() << " FAILED." << endl;
    }

    EXCEPINFO except;
    DISPPARAMS dispParam = {NULL, NULL, 0, 0};

    if (paramNum > 0)
    {
        dispParam.rgvarg = new VARIANT[paramNum];
        dispParam.cArgs = paramNum;
    }


    OLECHAR* lParam;

    lParam = new OLECHAR[param.size() + 1];
    wcsncpy(lParam, param.c_str(), param.size() + 1);

    if (paramNum > 0)
    {
        VariantInit(dispParam.rgvarg);
        dispParam.rgvarg[0].vt = VT_BSTR;
        dispParam.rgvarg[0].bstrVal = SysAllocString(lParam);
    }

    inOutRes->pdispVal = NULL;

    // cout << "1 " << inOutRes->pdispVal << endl;
    hr = parent->Invoke(dispIdInit, IID_NULL, LOCALE_SYSTEM_DEFAULT, objType, &dispParam, inOutRes, &except, NULL);
    // cout << "2 " << inOutRes->pdispVal << endl;
    delete[] lParam;

    if (paramNum > 0)
    {
        delete[] dispParam.rgvarg;
    }

    if (inOutRes->pdispVal == NULL)
    {
        cout << "Next object is NULL " << endl;
    }


    if (FAILED(hr))
    {
        HrMessage(hr);
        cout << "Connect failed." << endl;
        cout << "Ex error code: " << except.wCode << endl;
    }

    return hr;
}


// ---------- ---------- ---------- ---------- ---------- ----------
string bstrToStr(BSTR input)
{
    size_t len = (wcslen(input) + 1) * 2;
    char* newstr = new char[len];
    //strcpy_s(newstr, len, (char*)temp);
    wcstombs(newstr,(OLECHAR*)input, len);
    string resStr(newstr);
    return resStr;
}

// ---------- ---------- ---------- ---------- ---------- ----------
HRESULT GetNextObject(wstring objectName, IDispatch** outParent)
{
    DISPID dispIdent;
    OLECHAR* fName = new OLECHAR[objectName.size() + 1];
    wcsncpy(fName, objectName.c_str(), objectName.size() + 1);

    HRESULT hr;
    hr = (*outParent)->GetIDsOfNames(IID_NULL, &fName,1, LOCALE_SYSTEM_DEFAULT, &dispIdent);

    if (FAILED(hr))
    {
        cout << "GetIDsOfNames: " << objectName.c_str() << " FAILED." << endl;
    }

    return hr;
}

// ---------- ---------- ---------- ---------- ---------- ----------
void HrMessage(HRESULT hr)
{
    switch(hr)
    {
        case S_OK:
            cout << "Success." << endl;
            break;
        case DISP_E_BADPARAMCOUNT:
            cout << "The number of elements provided to DISPPARAMS is different from the number of parameters accepted." << endl;
            break;
        case DISP_E_BADVARTYPE:
            cout << "DISP_E_BADVARTYPE" << endl;
            break;
        case DISP_E_EXCEPTION:
            cout << "DISP_E_EXCEPTION" << endl;
            break;
        case DISP_E_MEMBERNOTFOUND:
            cout << "DISP_E_MEMBERNOTFOUND" << endl;
            break;
        case DISP_E_NONAMEDARGS:
            cout << "DISP_E_NONAMEDARGS" << endl;
            break;
        case DISP_E_OVERFLOW:
            cout << "DISP_E_OVERFLOW" << endl;
            break;
        case DISP_E_PARAMNOTFOUND:
            cout << "DISP_E_PARAMNOTFOUND" << endl;
            break;
        case DISP_E_TYPEMISMATCH:
            cout << "DISP_E_TYPEMISMATCH" << endl;
            break;
        case DISP_E_UNKNOWNINTERFACE:
            cout << "DISP_E_UNKNOWNINTERFACE" << endl;
            break;
        case DISP_E_UNKNOWNLCID:
            cout << "DISP_E_UNKNOWNLCID" << endl;
            break;
        case DISP_E_PARAMNOTOPTIONAL:
            cout << "DISP_E_PARAMNOTOPTIONAL" << endl;
            break;
        default:
            break;
    }
}

// ---------- ---------- ---------- ---------- ---------- ----------
string toWinStr(QString str)
{
    QTextCodec* codec = QTextCodec::codecForName("CP1251");
    QByteArray kod = codec->fromUnicode(str);
    return kod.data();
}


// ---------- ---------- ---------- ---------- ---------- ----------
QString fromWinStr(QString str)
{
    QTextCodec* codec = QTextCodec::codecForName("CP1251");
    return codec->toUnicode(str.toStdString().c_str());
}

void f1()
{
    cout << "f1" << endl;
}

void f2()
{
    cout << "f2" << endl;
}

void f3()
{
    cout << "f3" << endl;
}
