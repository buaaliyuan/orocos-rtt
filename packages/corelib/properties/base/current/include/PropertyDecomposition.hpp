/***************************************************************************
  tag: Peter Soetens  Mon Jan 19 14:11:20 CET 2004  PropertyDecomposition.hpp 

                        PropertyDecomposition.hpp -  description
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
 
#ifndef PROPERTYDECOMPOSITION_HPP
#define PROPERTYDECOMPOSITION_HPP


#include "Property.hpp"
#include "VectorComposition.hpp"

#include <pkgconf/system.h>
#ifdef OROPKG_GEOMETRY
#include <geometry/MotionProperties.hpp>
#endif

namespace ORO_CoreLib
{
	/** 
	 * @file PropertyDecomposition.hpp
     * The decomposeProperty function is a helper function which converts composed 
	 * types into property bags of primitive types. The primitive types are then 
     * passed on to the PropertyIntrospection.
	 *
	 * Every new type used as a property will automatically get a generated
	 * function like the one below. It is your task to copy this function and
     * rewrite it for your type so that <YourType> is decomposed in primitive
     * property types. This enables you to
	 * use properties of any type without being forced to extend your existing
     * PropertyIntrospection classes.
	 *
	 * To be able to introspect this new type, you need to write a
	 * specialized implementation of this function for your specific type.
	 * This function should use the PropertyIntrospection interface to identify your
	 * type. Include then the file containing the new function in your program and
     * it will be used automatically.
     *
     * Example for type MyClass :
     * \code 
     *  void decomposeProperty(PropertyIntrospection *pi, const Property< MyClass > &c)
     *  {
     *      // Decode c into primitive properties Var1 and Var2 which are in a PropertyBag of the
     *      // type "MyClass".
     *	    Property<PropertyBag> result("Class Name","Description", c.name, PropertyBag("MyClass") );
     *	    Property<bool>   var1("Var1","", c.var1);
     *	    Property<double> var2("Var2","", c.var2);
     *
     *      // Put var1 and var2 in the bag
     *      result.value().add(var1);
     *      result.value().add(var2);
     *	    
     *      // introspect the bag.
	 *	    pi->introspect(result);
     *      // done !
     *  } 
     * \endcode 
     *  @see PropertyComposition.hpp
     * @note If you see this message apearing, and do not know why or where it is generated,
     *  comment (//... ) this function and recompile, a compile time error will be generated
     *  at places where this function would have been used. Reasons why the compiler uses this function
     *  instead of yours are : 
     *  <il><li>Your decomposeProperty function does not live in 
     *  the ORO_CoreLib namespace. Solution : put it in the ORO_CoreLib namespace.</li>
     *  <li>There is no decomposeProperty function for your class. Solution : write your own
     *  decomposeProperty function.</li>
     *  <li> At compile time, your decomposeProperty function could not be 'seen' by the compiler.
     *  Solution : write using ORO_CoreLib::decomposeProperty; at top of the file causing this
     *  error.</li><il>
	 */
	template<typename T>
	void decomposeProperty(PropertyIntrospection *pi, const Property<T> &b)
	{
		Property<std::string> error(b.getName(),
                                    "PropertyIntrospection Error. "+ std::string("Unknown type, see ") + std::string(__FILE__) + std::string(" for more information "),
                                    std::string("Err") );
		pi->introspect(error);
	} 
	
}

#endif
