/* Copyright 2015-2016 Pascal COMBES <pascom@orange.fr>
 * 
 * This file is part of QTestLibPlugin.
 * 
 * QTestLibPlugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * QTestLibPlugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with QTestLibPlugin. If not, see <http://www.gnu.org/licenses/>
 */

#include "testsuitemodeltest.h"
#include "testhelper.h"

#include <testsuitemodel.h>

#include <projectexplorer/session.h>
#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <projectexplorer/runnables.h>
#include <projectexplorer/runconfiguration.h>
#include <projectexplorer/localapplicationruncontrol.h>

#include <extensionsystem/pluginmanager.h>

#include <QtTest>

/*
 * NOTE This is extracted from QtCreator sources <qmakeprojectmanager/desktopqmakerunconfiguration.cpp>
 * I hoope this wont change in next releases otherwise these tests will be broken.
 */
#define QMAKE_RUNCONFIG_PREFIX "Qt4ProjectManager.Qt4RunConfiguration"
const QString CommandLineArgumentsKey = QLatin1String(QMAKE_RUNCONFIG_PREFIX ".CommandLineArguments");
const QString WorkingDirectoryKey = QLatin1String(QMAKE_RUNCONFIG_PREFIX ".UserWorkingDirectory");

namespace QTestLibPlugin {
namespace Test {

TestSuiteModelTest::TestSuiteModelTest(void)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    mTests.clear();
    mTests << "OneClassTest";
    mTests << "AllMessagesTest";
    mTests << "MultipleClassesTest";
    mTests << "SignalsTest";
    mTests << "LimitsTest";

    mParserFormats.clear();
    mParserFormats << "txt";
    mParserFormats << "xml";
    mParserFormats << "lightxml";
    mParserFormats << "xunitxml";
}

void TestSuiteModelTest::init(void)
{
    mOpenProjects.clear();
}

void TestSuiteModelTest::cleanup(void)
{
    foreach (ProjectExplorer::Project* project, mOpenProjects) {
        closeProject(project);
    }
}

void TestSuiteModelTest::testZeroRemoveBad(void)
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;

    // Try to remove non existing test
    SUB_TEST_FUNCTION(removeTestAt(&model, 0));
    //testRuns.removeAt(0);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::testZeroClear(void)
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;

    // Clear test suite
    SUB_TEST_FUNCTION(clearTests(&model));
    testRuns.clear();
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::testAppendOne(void)
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;

    // Append one test
    TestRunData testData(mTests.at(qrand() % mTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData.testName, testData.parserFormat, testData.testVerbosity));
    testRuns.append(&testData);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::testAppendOneRemoveOne()
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;

    // Append one test
    TestRunData testData(mTests.at(qrand() % mTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData.testName, testData.parserFormat, testData.testVerbosity));
    testRuns.append(&testData);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Remove the appended test
    SUB_TEST_FUNCTION(removeTestAt(&model, 0));
    testRuns.removeAt(0);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::testRemoveBadAppendOne(void)
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;

    // Try to remove non existing test
    SUB_TEST_FUNCTION(removeTestAt(&model, 0));
    //testRuns.removeAt(0);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Append one test
    TestRunData testData(mTests.at(qrand() % mTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData.testName, testData.parserFormat, testData.testVerbosity));
    testRuns.append(&testData);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::testAppendOneRemoveBad()
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;

    // Append one test
    TestRunData testData(mTests.at(qrand() % mTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData.testName, testData.parserFormat, testData.testVerbosity));
    testRuns.append(&testData);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Try to remove non existing test
    SUB_TEST_FUNCTION(removeTestAt(&model, 1));
    //testRuns.removeAt(1);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::testAppendOneClear()
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;

    // Append one test
    TestRunData testData(mTests.at(qrand() % mTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData.testName, testData.parserFormat, testData.testVerbosity));
    testRuns.append(&testData);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Clear test suite
    SUB_TEST_FUNCTION(clearTests(&model));
    testRuns.clear();
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::testAppendTwo(void)
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;
    QStringList localTests = mTests;

    // Append one test
    TestRunData testData1(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData1.testName, testData1.parserFormat, testData1.testVerbosity));
    testRuns.append(&testData1);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Append one test
    TestRunData testData2(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData2.testName, testData2.parserFormat, testData2.testVerbosity));
    testRuns.append(&testData2);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::testAppendTwoRemoveFirst(void)
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;
    QStringList localTests = mTests;

    // Append one test
    TestRunData testData1(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData1.testName, testData1.parserFormat, testData1.testVerbosity));
    testRuns.append(&testData1);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Append one test
    TestRunData testData2(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData2.testName, testData2.parserFormat, testData2.testVerbosity));
    testRuns.append(&testData2);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Remove first test
    SUB_TEST_FUNCTION(removeTestAt(&model, 0));
    testRuns.removeAt(0);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::testAppendTwoRemoveSecond(void)
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;
    QStringList localTests = mTests;

    // Append one test
    TestRunData testData1(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData1.testName, testData1.parserFormat, testData1.testVerbosity));
    testRuns.append(&testData1);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Append one test
    TestRunData testData2(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData2.testName, testData2.parserFormat, testData2.testVerbosity));
    testRuns.append(&testData2);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Remove second test
    SUB_TEST_FUNCTION(removeTestAt(&model, 1));
    testRuns.removeAt(1);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::testAppendTwoRemoveBad(void)
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData*> testRuns;
    QStringList localTests = mTests;

    // Append one test
    TestRunData testData1(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData1.testName, testData1.parserFormat, testData1.testVerbosity));
    testRuns.append(&testData1);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Append one test
    TestRunData testData2(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData2.testName, testData2.parserFormat, testData2.testVerbosity));
    testRuns.append(&testData2);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Try to remove non existing test
    SUB_TEST_FUNCTION(removeTestAt(&model, 2));
    //testRuns.removeAt(2);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::testAppendOneRemoveAppendOne(void)
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;
    QStringList localTests = mTests;

    // Append one test
    TestRunData testData1(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData1.testName, testData1.parserFormat, testData1.testVerbosity));
    testRuns.append(&testData1);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Remove first test
    SUB_TEST_FUNCTION(removeTestAt(&model, 0));
    testRuns.removeAt(0);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Append one test
    TestRunData testData2(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData2.testName, testData2.parserFormat, testData2.testVerbosity));
    testRuns.append(&testData2);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

}

void TestSuiteModelTest::testAppendOneRemoveBadAppendOne(void)
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;
    QStringList localTests = mTests;

    // Append one test
    TestRunData testData1(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData1.testName, testData1.parserFormat, testData1.testVerbosity));
    testRuns.append(&testData1);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Try to remove non existing test
    SUB_TEST_FUNCTION(removeTestAt(&model, 1));
    //testRuns.removeAt(1);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Append one test
    TestRunData testData2(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData2.testName, testData2.parserFormat, testData2.testVerbosity));
    testRuns.append(&testData2);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::testAppendTwoClear(void)
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;
    QStringList localTests = mTests;

    // Append one test
    TestRunData testData1(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData1.testName, testData1.parserFormat, testData1.testVerbosity));
    testRuns.append(&testData1);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Append one test
    TestRunData testData2(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData2.testName, testData2.parserFormat, testData2.testVerbosity));
    testRuns.append(&testData2);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Clear test suite
    SUB_TEST_FUNCTION(clearTests(&model));
    testRuns.clear();
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::testAppendOneClearAppendOne(void)
{
    QTestLibPlugin::Internal::TestSuiteModel model(this);
    QList< TestRunData* > testRuns;
    QStringList localTests = mTests;

    // Append one test
    TestRunData testData1(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData1.testName, testData1.parserFormat, testData1.testVerbosity));
    testRuns.append(&testData1);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Clear test suite
    SUB_TEST_FUNCTION(clearTests(&model));
    testRuns.clear();
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));

    // Append one test
    TestRunData testData2(localTests.takeAt(qrand() % localTests.size()), mParserFormats.at(qrand() % mParserFormats.size()), (QTestLibModelTester::Verbosity) ((qrand() % ((int) QTestLibModelTester::VerbosityCountMinusOne + 1)) - 1));
    SUB_TEST_FUNCTION(appendTest(&model, testData2.testName, testData2.parserFormat, testData2.testVerbosity));
    testRuns.append(&testData2);
    SUB_TEST_FUNCTION(parseSuiteRoot(&model, testRuns));
}

void TestSuiteModelTest::parseSuiteRoot(const QAbstractItemModel* model, const QList< TestRunData* > &tests)
{
    BEGIN_SUB_TEST_FUNCTION

    QVERIFY(model->rowCount(QModelIndex()) == tests.size());
    QVERIFY(model->columnCount(QModelIndex()) == 3);

    int i = 0;
    QList< TestRunData* >::const_iterator it = tests.constBegin();
    QTestLibModelTester tester(model);

    for (; it!= tests.constEnd(); it++, i++) {
        QVERIFY2(model->index(i, 0, QModelIndex()).isValid(), qPrintable(QString("Children %1 of root element is not valid").arg(i)));

        tester.setFormat((*it)->parserFormat);
        if (QString::compare((*it)->parserFormat, "txt", Qt::CaseSensitive) == 0)
            tester.setVerbosity((*it)->testVerbosity);
        else if (QString::compare((*it)->parserFormat, "xml", Qt::CaseSensitive) == 0)
            tester.setVerbosity(qMax(QTestLibModelTester::Normal, (*it)->testVerbosity));
        else if (QString::compare((*it)->parserFormat, "lightxml", Qt::CaseSensitive) == 0)
            tester.setVerbosity(qMax(QTestLibModelTester::Normal, (*it)->testVerbosity));
        else if (QString::compare((*it)->parserFormat, "xunitxml", Qt::CaseSensitive) == 0)
            tester.setVerbosity(qMax(QTestLibModelTester::Normal, (*it)->testVerbosity));
        else
            QVERIFY2(false, qPrintable(QString("Unknown parser format %1").arg((*it)->parserFormat)));

        tester.setResultsFile(TESTS_DIR "/" + (*it)->testName + "/" + (*it)->testName.toLower() + ".xml");
        QVERIFY2(tester.checkIndex(model->index(i, 0, QModelIndex())), qPrintable(tester.error()));
    }

    END_SUB_TEST_FUNCTION
}

QStringList TestSuiteModelTest::commandLineArguments(const QString& format, QTestLibModelTester::Verbosity verbosity)
{
    QStringList cmdArgs;

    int argsVersion = qrand() % 6;
    if (format == "txt") {
        switch (argsVersion % 3) {
        case 0:
            break;
        case 1:
            cmdArgs.append(QString("-%1").arg(format));
            break;
        case 2:
            cmdArgs.append(QString("-o -,%1").arg(format));
            break;
        default:
            Q_ASSERT(false); // IMPOSSIBLE
            break;
        }
    } else {
        if (argsVersion % 2 == 1)
            cmdArgs.append(QString("-%1").arg(format));
        else
            cmdArgs.append(QString("-o -,%1").arg(format));
    }

    switch(verbosity) {
    case QTestLibModelTester::Silent:
        cmdArgs.append("-silent");
        break;
    case QTestLibModelTester::Normal:
        break;
    case QTestLibModelTester::Verbose1:
        cmdArgs.append("-v1");
        break;
    case QTestLibModelTester::Verbose2:
        cmdArgs.append("-v2");
        break;
    case QTestLibModelTester::VerboseSignal:
        cmdArgs.append("-vs");
        break;
    default:
        qWarning() << "Sentinel value VerbosityCountMinusOne must not be used as verbosity.";
        break;
    }

    qDebug() << "Command line arguments:" << cmdArgs;
    return cmdArgs;
}

void TestSuiteModelTest::appendTest(QTestLibPlugin::Internal::TestSuiteModel *model, const QString& test, const QString& format, QTestLibModelTester::Verbosity verbosity)
{
    BEGIN_SUB_TEST_FUNCTION

    ProjectExplorer::Project* project;
    QVERIFY(openQMakeProject(TESTS_DIR "/" + test + "/" + test + ".pro", &project));
    mOpenProjects << project;

    // Retrieve RunConfiguration:
    ProjectExplorer::RunConfiguration* testRunConfig = NULL;
    foreach (ProjectExplorer::RunConfiguration* runConfig, project->activeTarget()->runConfigurations()) {
        if (!runConfig->runnable().is<ProjectExplorer::StandardRunnable>())
            continue;

        ProjectExplorer::StandardRunnable localRunnable = runConfig->runnable().as<ProjectExplorer::StandardRunnable>();
        QFileInfo exeFileInfo(localRunnable.executable);
        qDebug() << exeFileInfo.absoluteFilePath();
        QVERIFY(exeFileInfo.exists());
        if (QString::compare(exeFileInfo.baseName(), test, Qt::CaseSensitive) == 0)
            testRunConfig = runConfig;
        break;
    }
    QVERIFY(testRunConfig != NULL);

    // Change the run configuration map:
    QStringList cmdArgs = commandLineArguments(format, verbosity);
    QVariantMap map = testRunConfig->toMap();
    map.remove(CommandLineArgumentsKey);
    map.insert(CommandLineArgumentsKey, cmdArgs.join(QLatin1Char(' ')));
    map.remove(WorkingDirectoryKey);
    map.insert(WorkingDirectoryKey, TESTS_DIR "/" + test + "/");

    // Restore a modified run configuration from the modified map:
    ProjectExplorer::IRunConfigurationFactory* runConfigFactory = ProjectExplorer::IRunConfigurationFactory::find(project->activeTarget(), map);
    QVERIFY(runConfigFactory != NULL);
    QVERIFY(runConfigFactory->canRestore(project->activeTarget(), map));
    ProjectExplorer::RunConfiguration* modifiedRunConfig = runConfigFactory->restore(project->activeTarget(), map);
    QVERIFY(modifiedRunConfig != NULL);
    QVERIFY(modifiedRunConfig->runnable().is<ProjectExplorer::StandardRunnable>());
    ProjectExplorer::StandardRunnable modifiedRunnable = modifiedRunConfig->runnable().as<ProjectExplorer::StandardRunnable>();
    QCOMPARE(modifiedRunnable.commandLineArguments, cmdArgs.join(QLatin1Char(' ')));
    QCOMPARE(modifiedRunnable.workingDirectory, TESTS_DIR "/" + test);

    // Find a run control factory
    ProjectExplorer::IRunControlFactory* runControlFactory = NULL;
    foreach (runControlFactory, ExtensionSystem::PluginManager::getObjects<ProjectExplorer::IRunControlFactory>()) {
        if (runControlFactory->canRun(modifiedRunConfig, ProjectExplorer::Constants::NORMAL_RUN_MODE))
            break;
    }
    QVERIFY(runControlFactory != NULL);
    QVERIFY(runControlFactory->canRun(modifiedRunConfig, ProjectExplorer::Constants::NORMAL_RUN_MODE));

    // Create a run control
    QString errMsg;
    ProjectExplorer::RunControl* runControl = runControlFactory->create(modifiedRunConfig, ProjectExplorer::Constants::NORMAL_RUN_MODE, &errMsg);
    QCOMPARE(errMsg, QString());
    QVERIFY(runControl != NULL);
    QCOMPARE(runControl->runConfiguration(), modifiedRunConfig);

    // Run run control
    model->appendTestRun(runControl);
    QSignalSpy runControlFinishedSpy(runControl, SIGNAL(finished()));
    runControl->start();
    if (!runControlFinishedSpy.wait()) {
        runControl->stop();
        QVERIFY((runControlFinishedSpy.count() == 1) || runControlFinishedSpy.wait());
        QSKIP("Computer is too slow for this test.");
    }

    END_SUB_TEST_FUNCTION
}

void TestSuiteModelTest::removeTestAt(QTestLibPlugin::Internal::TestSuiteModel *model, int i)
{
    BEGIN_SUB_TEST_FUNCTION

    QSignalSpy rowsAboutToBeRemovedSpy(model, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)));
    QSignalSpy rowsRemovedSpy(model, SIGNAL(rowsRemoved(QModelIndex, int, int)));

    int emissions = (int) (i >= 0) && (i < model->rowCount());
    model->removeTestRun(i);

    QVERIFY2(rowsAboutToBeRemovedSpy.count() == emissions, "RowsAboutToBeRemoved was not emitted");
    QVERIFY2(rowsRemovedSpy.count() == emissions, "RowsRemoved signal was not emitted");

    if (emissions == 1) {
        SUB_TEST_FUNCTION(checkSignalArguments("rowsAboutToBeRemoved()", rowsAboutToBeRemovedSpy.takeFirst(), i, i));
        SUB_TEST_FUNCTION(checkSignalArguments("rowsRemoved()", rowsRemovedSpy.takeFirst(), i, i));
    }

    END_SUB_TEST_FUNCTION
}

void TestSuiteModelTest::clearTests(QTestLibPlugin::Internal::TestSuiteModel *model)
{
    BEGIN_SUB_TEST_FUNCTION

    QSignalSpy modelAboutToBeResetSpy(model, SIGNAL(modelAboutToBeReset()));
    QSignalSpy modelResetSpy(model, SIGNAL(modelReset()));

    model->clear();

    QVERIFY2(modelAboutToBeResetSpy.count() == 1, "RowsAboutToBeRemoved was not emitted");
    QVERIFY2(modelResetSpy.count() == 1, "RowsRemoved signal was not emitted");

    END_SUB_TEST_FUNCTION
}

void TestSuiteModelTest::checkSignalArguments(const QString& signal, const QList<QVariant>& args, int first, int last)
{
    BEGIN_SUB_TEST_FUNCTION

    QVERIFY2(args.at(0).type() == QVariant::ModelIndex,
             qPrintable(QString("The first argument of signal %1 should be a QModelIndex").arg(signal)));
    QVERIFY2(!args.at(0).toModelIndex().isValid(),
             qPrintable(QString("The first argument of signal %1 should be an invalid QModelIndex").arg(signal)));
    QVERIFY2(args.at(1).type() == QVariant::Int,
             qPrintable(QString("The second argument of signal %1 should be an integer").arg(signal)));
    QVERIFY2(args.at(1).toInt() == first,
             qPrintable(QString("The second argument of signal %1 should be %2").arg(signal).arg(first)));
    QVERIFY2(args.at(2).type() == QVariant::Int,
             qPrintable(QString("The third argument of signal %1 should be an integer").arg(signal)));
    QVERIFY2(args.at(2).toInt() == last,
             qPrintable(QString("The third argument of signal %1 should be %2").arg(signal).arg(last)));

    END_SUB_TEST_FUNCTION
}

} // Test
} // QTestLibPlugin
