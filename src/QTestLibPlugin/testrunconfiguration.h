#ifndef TESTRUNCONFIGURATION_H
#define TESTRUNCONFIGURATION_H

#include "qtestlibargsparser.h"

#include <utils/fileutils.h>

#include <projectexplorer/localapplicationrunconfiguration.h>
#include <projectexplorer/applicationlauncher.h>

#include <QWidget>

class QLabel;
class QPushButton;
class QSpinBox;


namespace ProjectExplorer {
    class Kit;
}

namespace QTestLibPlugin {
namespace Widgets {
    class FileTypeValidatingLineEdit;
}

namespace Internal {

class TestRunConfigurationData
{
public:
    TestRunConfigurationData(ProjectExplorer::Target* target);

    inline QString makeExe(void) const {return mMakeExe.isNull() ? mAutoMakeExe.toString() : mMakeExe.toString();}
    inline bool usesDefaultMakeExe(void) const {return mMakeExe.isNull();}
    inline void useDefaultMakeExe(void) {mMakeExe = Utils::FileName();}
    void setMakeExe(const QString& path);

    inline Utils::FileName makefile(void) const {return mMakefile.isNull() ? mAutoMakefile : mMakefile;}
    inline bool usesDefaultMakefile(void) const {return mMakefile.isNull();}
    inline void useDefaultMakefile(void) {mMakefile = Utils::FileName();}
    inline void setAutoMakefile(const Utils::FileName& path) {mAutoMakefile = path;}
    inline void setMakefile(const QString& path) {setMakefile(Utils::FileName::fromUserInput(path));}
    inline void setMakefile(const Utils::FileName& makefile) {mMakefile = (mAutoMakefile == makefile  ? Utils::FileName() : makefile);}

    QVariantMap toMap(QVariantMap& map) const;
    bool fromMap(const QVariantMap& map);

    QStringList commandLineArguments(void) const;

    int jobNumber;
    QString testRunner;
    QString workingDirectory;
private:
    Utils::FileName mAutoMakeExe;
    Utils::FileName mMakeExe;
    Utils::FileName mAutoMakefile;
    Utils::FileName mMakefile;
};

class TestRunConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    TestRunConfigurationWidget(TestRunConfigurationData* data, QWidget* parent = NULL);
private slots:
    void updateWorkingDirectory(bool valid);
    void updateWorkingDirectory(void);
    void browseWorkingDirectory(void);

    void updateMakefile(bool valid);
    void updateMakefile(void);
    void autoDetectMakefile(void);
    void browseMakefile(void);

    void updateMakeExe(bool valid);
    void updateMakeExe(void);
    void autoDetectMakeExe(void);
    void browseMakeExe(void);

    void updateTestRunner(bool valid);
    void updateTestRunner(void);
    void browseTestRunner(void);

    void updateJubNumber(int jobNumber);
private:
    TestRunConfigurationData* mData;

    QLabel* mWorkingDirectoryLabel;
    Widgets::FileTypeValidatingLineEdit* mWorkingDirectoryEdit;
    QPushButton* mWorkingDirectoryButton;
    QLabel* mMakefileLabel;
    Widgets::FileTypeValidatingLineEdit* mMakefileEdit;
    QPushButton* mMakefileDetectButton;
    QPushButton* mMakefileBrowseButton;
    QLabel* mMakeExeLabel;
    Widgets::FileTypeValidatingLineEdit* mMakeExeEdit;
    QPushButton* mMakeExeDetectButton;
    QPushButton* mMakeExeBrowseButton;
    QLabel* mTestRunnerLabel;
    Widgets::FileTypeValidatingLineEdit* mTestRunnerEdit;
    QPushButton* mTestRunnerButton;
    QLabel* mJobsLabel;
    QSpinBox* mJobsSpin;

};

class TestRunConfiguration : public ProjectExplorer::LocalApplicationRunConfiguration
{
    Q_OBJECT
public:
    ~TestRunConfiguration();

    inline QWidget* createConfigurationWidget(void) {return new TestRunConfigurationWidget(mData);}

    virtual inline QString executable() const {return mData->makeExe();}
    virtual inline ProjectExplorer::ApplicationLauncher::Mode runMode(void) const {return ProjectExplorer::ApplicationLauncher::Gui;}
    virtual inline QString workingDirectory(void) const {return mData->workingDirectory;}
    virtual QString commandLineArguments(void) const;

    void setMakefile(const Utils::FileName& makefile);
    inline Utils::FileName makefile(void) const {return mData->makefile();}

    QVariantMap toMap(void) const;
    bool fromMap(const QVariantMap& map);
private:
    TestRunConfiguration(ProjectExplorer::Target *parent, Core::Id id);

    TestRunConfigurationData* mData;

    friend class QMakeTestRunConfigurationFactory;
};

} // Internal
} // QTestLibPlugin

#endif // TESTRUNCONFIGURATION_H
