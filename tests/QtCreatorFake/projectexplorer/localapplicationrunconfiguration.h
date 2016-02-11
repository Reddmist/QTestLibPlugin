/* Copyright 2015 Pascal COMBES <pascom@orange.fr>
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

#ifndef LOCALAPPLICATIONRUNCONFIGURATION_H
#define LOCALAPPLICATIONRUNCONFIGURATION_H

#include "runconfiguration.h"

namespace ProjectExplorer {

class LocalApplicationRunConfiguration : public RunConfiguration
{
    Q_OBJECT
public:
    inline LocalApplicationRunConfiguration(ProjectExplorer::Target *target) :
        RunConfiguration(target) {}

    inline QString executable() const {return mExe;}
    inline void setExecutable(const QString& exe) {mExe = exe;}
    inline QString workingDirectory() const {return mDir;}
    inline void setWorkingDirectory(const QString& dir) {mDir = dir;}
    inline QString commandLineArguments() const {return mArgs;}
    inline void setCommandLineArguments(const QString& args) {mArgs = args;}

    inline QString displayName(void) const {return mExe.section('/', -1, -1, QString::SectionSkipEmpty).section('.', 0, 0, QString::SectionSkipEmpty);}
private:
    QString mExe;
    QString mArgs;
    QString mDir;
};

} // ProjectExplorer

#endif // LOCALAPPLICATIONRUNCONFIGURATION_H
