/***************************************************************************
  tag: Peter Soetens  Thu Mar 2 08:30:18 CET 2006  DataConnectionInterface.hpp 

                        DataConnectionInterface.hpp -  description
                           -------------------
    begin                : Thu March 02 2006
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
 
 
#ifndef ORO_EXECUTION_DATA_CONNECTION_INTERFACE_HPP
#define ORO_EXECUTION_DATA_CONNECTION_INTERFACE_HPP

#include "DataObjectInterfaces.hpp"
#include "ConnectionInterface.hpp"

namespace RTT
{
    /**
     * This is the interface of a connection which
     * transfers data unbuffered between two Ports of a TaskContext.
     */
    template<class T>
    class DataConnectionInterface
        : public ConnectionInterface
    {
    public:
        typedef boost::intrusive_ptr< DataConnectionInterface<T> > shared_ptr;

        /**
         * Return the DataObject of this Connection.
         */
        virtual const DataObjectInterface<T>* data() const = 0;

        /**
         * Return the DataObject of this Connection.
         */
        virtual DataObjectInterface<T>* data() = 0;
    };

}

#endif
