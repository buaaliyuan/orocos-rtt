/***************************************************************************
  tag: Peter Soetens  Mon Jun 26 13:25:57 CEST 2006  StatementProcessor.hpp 

                        StatementProcessor.hpp -  description
                           -------------------
    begin                : Mon June 26 2006
    copyright            : (C) 2006 Peter Soetens
    email                : peter.soetens@fmtc.be
 
 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/
 
 
#ifndef ORO_EXECUTION_STATEMENT_PROCESSOR_HPP
#define ORO_EXECUTION_STATEMENT_PROCESSOR_HPP

#include <string>
#include "CommandC.hpp"

namespace RTT
{
    class TaskContext;

    /**
     * This class parses and executes a single scripting
     * statement.
     */
    class StatementProcessor
    {
        class D;
        D* d;
    public:
        StatementProcessor(TaskContext* tc);

        ~StatementProcessor();
        /**
         * Execute a script statement.
         * @retval -1 a parse error occured.
         * @retval 0 statement executed
         * @return a positive number if it is a command, which
         * is the ticket number of the command.
         */
        int execute(const std::string& code);

        /**
         * Check if commands are finished and free resources if so.
         */
        void checkFinished();

        /**
         * Return the command with the ticket number \a cnr.
         */
        CommandC getCommand( int cnr );
    };


}


#endif
