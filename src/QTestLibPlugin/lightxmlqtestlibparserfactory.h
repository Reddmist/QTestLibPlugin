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

#ifndef LIGHTXMLQTESTLIBPARSERFACTORY_H
#define LIGHTXMLQTESTLIBPARSERFACTORY_H

#include "testmodelfactory.h"

namespace QTestLibPlugin {
namespace Internal {

/*!
 * \brief The LightXMLQTestLibParserFactory class is a factory for LightXMLQTestLibParser
 *
 * As an implementation of AbstractTestParserFactory,
 * this class is in charge of checking that the associated parser, LightXMLQTestLibParser,
 * may parse the ProjectExplorer::RunConfiguration output and
 * allocate instances of the associated parser if needed.
 * For this it uses the methods of a base factory injected in the constructor.
 */
class LightXMLQTestLibParserFactory : public AbstractTestParserFactory
{
    Q_OBJECT
    Q_PROPERTY(AbstractTestParserFactory* baseFactory READ baseFactory)
public:
    /*!
     * \brief Constructor
     *
     * Just call the parent class constructor.
     *
     * \param base The base of the factory
     */
    inline LightXMLQTestLibParserFactory(AbstractTestParserFactory* base):
        AbstractTestParserFactory(base), mBase(base) {}
    /*!
     * \brief Base factory
     *
     * Returns the base factory (used by the canParse() method).
     * \return The base factory
     */
    inline AbstractTestParserFactory* baseFactory(void) const {return mBase;}
    /*!
     * \brief \copybrief AbstractTestParserFactory::canParse()
     *
     * Uses the base factory AbstractTestParserFactory::canParse() method, if available,
     * to tell whether the associated parser can parse the test.
     * \param runConfiguration he run configuration to test
     * \return true, if the associated parser may parse the test output.
     */
    inline bool canParse(ProjectExplorer::RunConfiguration* runConfiguration) const {return (mBase != nullptr) ? mBase->canParse(runConfiguration) : false;}
    AbstractTestParser* getParserInstance(ProjectExplorer::RunConfiguration *runConfiguration) const;
private:
    AbstractTestParserFactory* mBase;
};

} // namespace Internal
} // namespace QTestLibPlugin

#endif // LIGHTXMLQTESTLIBPARSERFACTORY_H
