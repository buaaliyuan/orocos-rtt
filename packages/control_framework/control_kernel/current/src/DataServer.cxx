/***************************************************************************
  tag: Peter Soetens  Tue Dec 21 22:43:08 CET 2004  DataServer.cxx 

                        DataServer.cxx -  description
                           -------------------
    begin                : Tue December 21 2004
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
 
 

#ifdef ORO_PRAGMA_INTERFACE
#pragma implementation
#endif
#include "control_kernel/DataServer.hpp"

namespace ORO_CoreLib
{
    // decomposeProperty for the NullType.
    void decomposeProperty(ORO_CoreLib::PropertyIntrospection* pi, 
                           ORO_CoreLib::Property<Loki::NullType> const& nt) {
        Property<std::string> error(nt.getName(), nt.getDescription(), std::string("NullType") );
        pi->introspect(error);
    }
        
}
