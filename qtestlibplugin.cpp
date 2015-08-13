#include "qtestlibplugin.h"
#include "qtestlibpluginconstants.h"

#include "testoutputpane.h"
#include "testsuitemodel.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <projectexplorer/projectexplorer.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

#include <QtPlugin>

using namespace QTestLibPlugin::Internal;

QTestLibPluginPlugin::QTestLibPluginPlugin()
{
    mModel = new TestSuiteModel(this);
}

QTestLibPluginPlugin::~QTestLibPluginPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool QTestLibPluginPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    TestOutputPane *outputPane = new TestOutputPane(mModel);
    addAutoReleasedObject(outputPane);

    connect(ProjectExplorer::ProjectExplorerPlugin::instance(), SIGNAL(runControlStarted(ProjectExplorer::RunControl*)),
            mModel, SLOT(appendTestRun(ProjectExplorer::RunControl*)));
    /*connect(ProjectExplorer::ProjectExplorerPlugin::instance(), SIGNAL(runControlFinished(ProjectExplorer::RunControl*)),
            outputPane, SLOT(reloadModel()));*/

    /* TODO remove it */
    QAction *action = new QAction(tr("QTestLibPlugin action"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
    connect(action, SIGNAL(triggered()), this, SLOT(triggerAction()));

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("QTestLibPlugin"));
    menu->addAction(cmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);
    /* End remove */

    return true;
}

void QTestLibPluginPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag QTestLibPluginPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void QTestLibPluginPlugin::triggerAction()
{
    QMessageBox::information(Core::ICore::mainWindow(),
                             tr("Action triggered"),
                             tr("This is an action from QTestLibPlugin."));
}

