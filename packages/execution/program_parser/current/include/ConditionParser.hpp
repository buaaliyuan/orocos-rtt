/***************************************************************************
  tag: Peter Soetens  Mon Jan 19 14:11:25 CET 2004  ConditionParser.hpp 

                        ConditionParser.hpp -  description
                           -------------------
    begin                : Mon January 19 2004
    copyright            : (C) 2004 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be
 
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
 
#ifndef CONDITIONPARSER_HPP
#define CONDITIONPARSER_HPP

#include "parser-types.hpp"
#include "CommonParser.hpp"
#include "ExpressionParser.hpp"
#include "DataSource.hpp"

#include <memory>
#include <stack>
#include <utility>

#pragma interface

namespace ORO_Execution
{
  /**
   * This is a class containing a parse function for conditions.  It
   * is used by ProgramParser, and probably other parser's too in
   * the future...
   */
  class ConditionParser
  {
      DataSource<bool>::shared_ptr ds_bool;

    void seendonecondition();
    void seenexpression();

    rule_t condition;

    TaskContext* context;
    CommonParser commonparser;
    ExpressionParser expressionparser;

  public:
    ConditionParser( TaskContext* );
    ~ConditionParser();

      TaskContext* setContext(TaskContext* tc);
      TaskContext* setStack(TaskContext* tc);

    rule_t& parser()
      {
        return condition;
      };

    /**
     * Call this to get the parsed condition.  If you use it, you
     * should subsequently call reset(), otherwise it will be deleted
     * in the ConditionParser destructor..
     */
      ConditionInterface* getParseResult();

      /**
       * Retrieve the result as a command, condition pair.
       */
      std::pair<CommandInterface*,ConditionInterface*> getParseResultAsCommand();

    void reset();
  };
}

#endif
