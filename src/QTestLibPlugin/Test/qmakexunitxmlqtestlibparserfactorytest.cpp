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

#include "qmakexunitxmlqtestlibparserfactorytest.h"
#include "testhelper.h"

#include <xunitxmlqtestlibparserfactory.h>
#include <baseqmakeqtestlibparserfactory.h>
#include <qtestlibpluginconstants.h>
#include <testrunconfiguration.h>

#include <projectexplorer/session.h>
#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <projectexplorer/runconfiguration.h>

#include <qmakeprojectmanager/desktopqmakerunconfiguration.h>

#include <extensionsystem/pluginmanager.h>

#include <utils/hostosinfo.h>

#include <QtTest>

namespace QTestLibPlugin {
namespace Test {

/*
 * NOTE This is extracted from QtCreator sources <qmakeprojectmanager/desktopqmakerunconfiguration.cpp>
 * I hoope this wont change in next releases otherwise these tests will be broken.
 */
#define QMAKE_RUNCONFIG_PREFIX "Qt4ProjectManager.Qt4RunConfiguration"
const QString CommandLineArgumentsKey = QLatin1String(QMAKE_RUNCONFIG_PREFIX ".CommandLineArguments");

void QMakeXUnitXMLQTestLibParserFactoryTest::initTestCase(void)
{
    QStringList projectPathes;

    // NOTE _data() function is not available for initTestCase()
    projectPathes << QLatin1String(TESTS_DIR "/OneClassTest");
    projectPathes << QLatin1String(TESTS_DIR "/AllMessagesTest");
    projectPathes << QLatin1String(TESTS_DIR "/MultipleClassesTest");
    projectPathes << QLatin1String(TESTS_DIR "/SignalsTest");
    projectPathes << QLatin1String(TESTS_DIR "/LimitsTest");
    projectPathes << QLatin1String(TESTS_DIR "/OneSubTest");
    projectPathes << QLatin1String(TESTS_DIR "/TwoSubTests");
    projectPathes << QLatin1String(TESTS_DIR "/NoSubTestOne");
    projectPathes << QLatin1String(TESTS_DIR "/NoSubTestTwo");

    foreach (QString projectPath, projectPathes)
        QVERIFY(removeProjectUserFiles(projectPath));
}

void QMakeXUnitXMLQTestLibParserFactoryTest::init(void)
{
    mProject = NULL;
}

void QMakeXUnitXMLQTestLibParserFactoryTest::cleanup(void)
{
    closeProject(mProject);
}


void QMakeXUnitXMLQTestLibParserFactoryTest::dataTest(void)
{
    QList< QPair<QString, bool> > verbosities;
    verbosities << qMakePair(QString::null, true);
    verbosities << qMakePair(QLatin1String("-silent"), true);
    verbosities << qMakePair(QLatin1String("-v1"), true);
    verbosities << qMakePair(QLatin1String("-v2"), true);
    verbosities << qMakePair(QLatin1String("-vs"), true);

    QList< QPair<QString, bool> > formats;
    formats << qMakePair(QString::null, false);
    formats << qMakePair(QLatin1String("txt"), false);
    formats << qMakePair(QLatin1String("csv"), false);
    formats << qMakePair(QLatin1String("xml"), false);
    formats << qMakePair(QLatin1String("lightxml"), false);
    formats << qMakePair(QLatin1String("xunitxml"), true);

    QTest::addColumn<QStringList>("cmdArgs");
    QTest::addColumn<bool>("result");

    typedef QPair<QString, bool> QPairStringbool;
    foreach (QPairStringbool formatPair, formats) {
        foreach (QPairStringbool verbosityPair, verbosities) {
            QStringList cmdArgs;

            if (!formatPair.first.isNull()) {
                cmdArgs << QLatin1Char('-') + formatPair.first;
                cmdArgs << QLatin1String("-o") << QLatin1String("-,") + formatPair.first;
            }
            if (!verbosityPair.first.isNull())
                cmdArgs << verbosityPair.first;

            QTest::newRow(qPrintable(cmdArgs.join(' '))) << cmdArgs << (formatPair.second && verbosityPair.second);
        }
    }
}

void QMakeXUnitXMLQTestLibParserFactoryTest::dataMakeCheck(void)
{
    QList< QPair<Internal::QTestLibArgsParser::TestVerbosity, bool> > verbosities;
    verbosities << qMakePair(Internal::QTestLibArgsParser::NormalVerbosity, true);
    verbosities << qMakePair(Internal::QTestLibArgsParser::Silent, true);
    verbosities << qMakePair(Internal::QTestLibArgsParser::Verbose1, true);
    verbosities << qMakePair(Internal::QTestLibArgsParser::Verbose2, true);
    verbosities << qMakePair(Internal::QTestLibArgsParser::VerboseSignal, true);

    QList< QPair<Internal::QTestLibArgsParser::TestOutputFormat, bool> > formats;
    formats << qMakePair(Internal::QTestLibArgsParser::TxtFormat, false);
    formats << qMakePair(Internal::QTestLibArgsParser::CsvFormat, false);
    formats << qMakePair(Internal::QTestLibArgsParser::XmlFormat, false);
    formats << qMakePair(Internal::QTestLibArgsParser::LightXmlFormat, false);
    formats << qMakePair(Internal::QTestLibArgsParser::XUnitXmlFormat, true);

    QTest::addColumn<Internal::QTestLibArgsParser::TestOutputFormat>("format");
    QTest::addColumn<Internal::QTestLibArgsParser::TestVerbosity>("verbosity");
    QTest::addColumn<bool>("result");

    typedef QPair<Internal::QTestLibArgsParser::TestVerbosity, bool> QPairVerbositybool;
    typedef QPair<Internal::QTestLibArgsParser::TestOutputFormat, bool> QPairFormatbool;
    foreach (QPairFormatbool formatPair, formats) {
        foreach (QPairVerbositybool verbosityPair, verbosities) {
            Internal::QTestLibArgsParser args;
            args.setVerbosity(verbosityPair.first);
            args.setOutputFormat(formatPair.first);
            QTest::newRow(qPrintable(args.toString())) << formatPair.first << verbosityPair.first << (formatPair.second && verbosityPair.second);
        }
    }
}

void QMakeXUnitXMLQTestLibParserFactoryTest::testOneClass(void)
{
    QFETCH(QStringList, cmdArgs);
    QFETCH(bool, result);

    runTest("OneClassTest", cmdArgs, result);
}

void QMakeXUnitXMLQTestLibParserFactoryTest::testAllMessages(void)
{
    QFETCH(QStringList, cmdArgs);
    QFETCH(bool, result);

    runTest("AllMessagesTest", cmdArgs, result);
}

void QMakeXUnitXMLQTestLibParserFactoryTest::testMultipleClasses(void)
{
    QFETCH(QStringList, cmdArgs);
    QFETCH(bool, result);

    runTest("MultipleClassesTest", cmdArgs, result);
}

void QMakeXUnitXMLQTestLibParserFactoryTest::testSignalsTest(void)
{
    QFETCH(QStringList, cmdArgs);
    QFETCH(bool, result);

    runTest("SignalsTest", cmdArgs, result);
}

void QMakeXUnitXMLQTestLibParserFactoryTest::testLimits(void)
{
    QFETCH(QStringList, cmdArgs);
    QFETCH(bool, result);

    runTest("LimitsTest", cmdArgs, result);
}

void QMakeXUnitXMLQTestLibParserFactoryTest::testOneSubTest(void)
{
    QFETCH(Internal::QTestLibArgsParser::TestOutputFormat, format);
    QFETCH(Internal::QTestLibArgsParser::TestVerbosity, verbosity);
    QFETCH(bool, result);

    runMakeCheck("OneSubTest", format, verbosity, result);
}

void QMakeXUnitXMLQTestLibParserFactoryTest::testTwoSubTests(void)
{
    QFETCH(Internal::QTestLibArgsParser::TestOutputFormat, format);
    QFETCH(Internal::QTestLibArgsParser::TestVerbosity, verbosity);
    QFETCH(bool, result);

    runMakeCheck("TwoSubTests", format, verbosity, result);
}

void QMakeXUnitXMLQTestLibParserFactoryTest::runTest(const QString& testName, const QStringList& cmdArgs, bool result)
{
    QVERIFY(openQMakeProject(TESTS_DIR "/" + testName + "/" + testName + ".pro", &mProject));

    // Retrieve RunConfiguration:
    ProjectExplorer::RunConfiguration* testRunConfig = NULL;
    foreach (ProjectExplorer::RunConfiguration* runConfig, mProject->activeTarget()->runConfigurations()) {
        if (!runConfig->runnable().is<ProjectExplorer::StandardRunnable>())
            continue;

        ProjectExplorer::StandardRunnable runnable = runConfig->runnable().as<ProjectExplorer::StandardRunnable>();
        QFileInfo exeFileInfo(runnable.executable);
        qDebug() << exeFileInfo.absoluteFilePath();
        QVERIFY(exeFileInfo.exists());
        if (QString::compare(exeFileInfo.baseName(), testName, Qt::CaseSensitive) == 0)
            testRunConfig = runConfig;
        break;
    }
    QVERIFY(testRunConfig != NULL);

    // Change the run configuration map:
    QVariantMap map = testRunConfig->toMap();
    map.remove(CommandLineArgumentsKey);
    map.insert(CommandLineArgumentsKey, cmdArgs.join(QLatin1Char(' ')));

    // Restore a modified run configuration from the modified map:
    ProjectExplorer::IRunConfigurationFactory* runConfigFactory = ProjectExplorer::IRunConfigurationFactory::find(mProject->activeTarget(), map);
    QVERIFY(runConfigFactory != NULL);
    QVERIFY(runConfigFactory->canRestore(mProject->activeTarget(), map));
    ProjectExplorer::RunConfiguration *modifiedRunConfig = runConfigFactory->restore(mProject->activeTarget(), map);
    QVERIFY(modifiedRunConfig != NULL);
    ProjectExplorer::StandardRunnable modifiedRunnable = modifiedRunConfig->runnable().as<ProjectExplorer::StandardRunnable>();
    QCOMPARE(modifiedRunnable.commandLineArguments, cmdArgs.join(QLatin1Char(' ')));

    testFactory(modifiedRunConfig, result);
}

void QMakeXUnitXMLQTestLibParserFactoryTest::runMakeCheck(const QString& testName, Internal::QTestLibArgsParser::TestOutputFormat format, Internal::QTestLibArgsParser::TestVerbosity verbosity, bool result)
{
    QVERIFY(openQMakeProject(TESTS_DIR "/" + testName + "/" + testName + ".pro", &mProject));

    // Retrieve RunConfiguration:
    ProjectExplorer::RunConfiguration* testRunConfig = NULL;
    foreach (ProjectExplorer::RunConfiguration* runConfig, mProject->activeTarget()->runConfigurations()) {
        if (runConfig->id() != Core::Id(Constants::TestRunConfigurationId))
            continue;
        testRunConfig = runConfig;
        break;
    }
    QVERIFY(testRunConfig != NULL);

    // Change the run configuration map:
    QVariantMap map = testRunConfig->toMap();
    map.remove(Constants::FormatKey);
    map.remove(Constants::VerbosityKey);
    map.insert(Constants::FormatKey, (int) format);
    map.insert(Constants::VerbosityKey, (int) verbosity);

    // Restore a modified run configuration from the modified map:
    ProjectExplorer::IRunConfigurationFactory* runConfigFactory = ProjectExplorer::IRunConfigurationFactory::find(mProject->activeTarget(), map);
    QVERIFY(runConfigFactory != NULL);
    QVERIFY(runConfigFactory->canRestore(mProject->activeTarget(), map));
    ProjectExplorer::RunConfiguration *modifiedRunConfig = runConfigFactory->restore(mProject->activeTarget(), map);
    QVERIFY(modifiedRunConfig != NULL);
    QVERIFY(modifiedRunConfig->runnable().is<ProjectExplorer::StandardRunnable>());
    ProjectExplorer::StandardRunnable modifiedRunnable = modifiedRunConfig->runnable().as<ProjectExplorer::StandardRunnable>();

    // Compare arguments to expected value:
    Internal::QTestLibArgsParser testArgsParser;
    testArgsParser.setOutputFormat(format);
    testArgsParser.setVerbosity(verbosity);
    QString expectedCmdArgs(QLatin1String("-f " TESTS_DIR "/") + testName + QLatin1String("/Makefile check"));
    if (!testArgsParser.toString().isEmpty())
        expectedCmdArgs.append(QString(QLatin1String(" TESTARGS=\"%1\"")).arg(testArgsParser.toString()));
    QCOMPARE(modifiedRunnable.commandLineArguments, expectedCmdArgs);

    testFactory(modifiedRunConfig, result);
}

void QMakeXUnitXMLQTestLibParserFactoryTest::testFactory(ProjectExplorer::RunConfiguration* testRunConfig, bool result)
{
    auto isQMakeFactory = [] (Internal::AbstractTestParserFactory* factory) {
        QVariant base = factory->property("baseFactory");
        if (!base.isValid())
            return false;
        QObject *baseObject = qvariant_cast<QObject*>(base);
        if (baseObject == NULL)
            return false;
        return (qobject_cast<Internal::BaseQMakeQTestLibParserFactory*>(baseObject) != NULL);
    };
    QTestLibPlugin::Internal::XUnitXMLQTestLibParserFactory* parserFactory = ExtensionSystem::PluginManager::getObject<QTestLibPlugin::Internal::XUnitXMLQTestLibParserFactory>(isQMakeFactory);
    QVERIFY(parserFactory != NULL);
    QCOMPARE(parserFactory->canParse(testRunConfig), result);
    QTestLibPlugin::Internal::AbstractTestParser* parser = parserFactory->getParserInstance(testRunConfig);
    QCOMPARE(parser != NULL, result);
    delete parser;
}


} // Test
} // QTestLibPlugin
