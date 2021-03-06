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

#ifndef TESTOUTPUTPANE_H
#define TESTOUTPUTPANE_H

#include "qtestlibargsparser.h"

#include <coreplugin/ioutputpane.h>
#include <utils/outputformat.h>
#include <QVector>

class QTreeView;
class QSettings;
class QComboBox;

namespace ProjectExplorer {
    class RunControl;
}

namespace QTestLibPlugin {
namespace Internal {

class TestSuiteModel;
class TestProxyModel;

// TODO some functions will be inlined
class TestOutputPane : public Core::IOutputPane
{
    Q_OBJECT
public:
    TestOutputPane(TestSuiteModel *model);
    bool canFocus(void) const {return false;}
    bool canNavigate(void) const {return true;}
    bool canNext(void) const {return false;}
    bool canPrevious(void) const {return false;}
    void clearContents(void);
    QString displayName(void) const {return trUtf8("Test output");}
    void goToNext(void) {}
    void goToPrev(void) {}
    bool hasFocus(void) const {return false;}
    QWidget* outputWidget(QWidget * parent);
    int priorityInStatusBar(void) const {return 10;}
    void setFocus(void) {}
    QList<QWidget *> toolBarWidgets(void) const;
    void visibilityChanged(bool visible) {Q_UNUSED(visible);}

    QTestLibArgsParser::TestOutputFormat userForceParser(void) const;

    void loadSettings(QSettings* settings);
    void saveSettings(QSettings* settings);
private:
    void loadColumnWidth(QSettings* settings, int column, const QString& key);
    QString formatToString(QTestLibArgsParser::TestOutputFormat format) const;
    TestProxyModel *mProxy;
    TestSuiteModel *mModel;
    QTreeView *mOutputWidget;
    QList<QWidget *> mToolbarWidgets;
    QComboBox *mUserForceParserCombo;
    QVector<int> mColumnWidths;
//    Core::MessageOutputWindow *mDebug;
};

} // namespace Internal
} // namespace QTestLibPlugin

#endif // TESTOUTPUTPANE_H
