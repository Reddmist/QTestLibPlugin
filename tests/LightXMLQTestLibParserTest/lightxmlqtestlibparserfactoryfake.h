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

#ifndef LIGHTXMLQTESTLIBPARSERFACTORYFAKE_H
#define LIGHTXMLQTESTLIBPARSERFACTORYFAKE_H

#include "testmodelfactory.h"

namespace QTestLibPlugin {
namespace Internal {

class LightXMLQTestLibParserFactory : public AbstractTestParserFactory
{
    Q_OBJECT
public:
    inline LightXMLQTestLibParserFactory(QObject *parent = NULL):
        AbstractTestParserFactory(parent) {}
    inline bool canParse(ProjectExplorer::RunConfiguration *runConfiguration) const {Q_UNUSED(runConfiguration); return true;}
    AbstractTestParser* getParserInstance(ProjectExplorer::RunConfiguration *runConfiguration) const;
};

} // namespace Internal
} // namespace QTestLibPlugin

#endif // LIGHTXMLQTESTLIBPARSERFACTORYFAKE_H
