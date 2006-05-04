/***************************************************************************
  tag: Peter Soetens  Mon Jan 19 14:11:21 CET 2004  EdgeCondition.hpp

                        EdgeCondition.hpp -  description
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

#ifndef EDGECONDITION_HPP
#define EDGECONDITION_HPP

#include <map>
// adjacency_list has some very short template parameter names,
// which may be defined as macros on some OS's. So undef here.
#undef DS	
#undef OEL
#undef VL
#undef VP
#undef EP
#undef GP
#undef EL
#include <boost/graph/adjacency_list.hpp>
using boost::property;

enum edge_condition_t { edge_condition };

namespace boost {
    BOOST_INSTALL_PROPERTY(edge, condition);
}

namespace ORO_CoreLib
{
    class ConditionInterface;
    class DataSourceBase;
}

namespace ORO_Execution
{
    /**
     * @brief This class represents a conditional branch in a program
     * tree.  It contains a condition which must be satisfied
     * to pass to the next node.
     */
    class EdgeCondition
    {
        static int condition_counter;
    public:
        typedef property<edge_condition_t, EdgeCondition,
                         property< boost::edge_index_t, int> > EdgeProperty;

        /**
         * Create a default EdgeCondition which evaluates always as true.
         */
        EdgeCondition();

        /**
         * EdgeCondition specifies a condition to
         * check.
         * This EdgeCondition takes ownership of cond.
         */
        EdgeCondition(ORO_CoreLib::ConditionInterface* cond );

        ~EdgeCondition();

        EdgeCondition( const EdgeCondition& orig );

        EdgeCondition& operator=( const EdgeCondition& orig );

        EdgeCondition copy( std::map<const ORO_CoreLib::DataSourceBase*, ORO_CoreLib::DataSourceBase*>& replacementdss ) const;

        /**
         * Reset the condition in this
         * EdgeCondition.  See the documentation
         * of ConditionInterface::reset() for more
         * details..
         */
        void reset();

        /**
         * Evaluates the condition coupled with this termination clause.
         */
        bool evaluate();

        /**
         * Return the condition coupled with this termination clause.
         */
        ORO_CoreLib::ConditionInterface* getCondition() const;

        int getRank() const { return rank; }
    protected:

        void setCondition(ORO_CoreLib::ConditionInterface* cond);

    private:
        ORO_CoreLib::ConditionInterface* condition;
        int rank;
    };

}

#endif
