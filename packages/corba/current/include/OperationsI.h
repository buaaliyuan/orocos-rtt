/***************************************************************************
  tag: Peter Soetens  Mon Jun 26 13:25:58 CEST 2006  ExecutionI.h 

                        ExecutionI.h -  description
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
 
 
// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from 
// be/be_codegen.cpp:859

// must be outside of #ifdef macro.
#include "../DataSource.hpp"

#ifndef INCLUDE_EXECUTIONI_H_
#define INCLUDE_EXECUTIONI_H_

#include "OperationsS.h"
#include "CorbaConversion.hpp"
#include "../CommandInterface.hpp"
#include "../Logger.hpp"

namespace RTT
{
    class CommandC;
}

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template< class DataType>
class  Orocos_Expression_i
  : public virtual POA_RTT::Corba::Expression,
    public virtual PortableServer::RefCountServantBase
{
protected:
    typename RTT::DataSource<DataType>::const_ptr morig;
    typename RTT::DataSource<DataType>::value_t last_value;
    PortableServer::POA_var mpoa;
public:
    typedef DataType SourceType;
    typedef typename RTT::DataSource<DataType>::value_t ResultType;

  // Constructor 
  Orocos_Expression_i (typename RTT::DataSource<SourceType>::const_ptr orig, PortableServer::POA_ptr the_poa)
      : morig( orig ), last_value(), mpoa( PortableServer::POA::_duplicate(the_poa) )
    {}

    PortableServer::POA_ptr _default_POA()
    {
        return PortableServer::POA::_duplicate(mpoa);
    }
  
  // Destructor 
    virtual ~Orocos_Expression_i (void) {}
  
  virtual
  CORBA::Any* value (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return RTT::AnyConversion<ResultType>::createAny( last_value );
  }
  
  virtual
  CORBA::Boolean evaluate (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      last_value = morig->get();
      return 1;
  }
  
  virtual
  CORBA::Any * get (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return RTT::AnyConversion<ResultType>::createAny( morig->get() );
  }
  
  virtual
  char * getType (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( morig->getType().c_str() );
  }

  virtual
  char * getTypeName (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( morig->getTypeName().c_str() );
  }

  virtual void destroyExpression()
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
 {
  }
};

/**
 * A servant which serves a RTT::DataSource through the 'Any' methods.
 * This servant is inferior to the template based ones.
 */
class  Orocos_AnyExpression_i
  : public virtual POA_RTT::Corba::Expression,
    public virtual PortableServer::RefCountServantBase
{
protected:
    RTT::DataSourceBase::const_ptr morig;
    CORBA::Any_var last_value;
    PortableServer::POA_var mpoa;
public:
    typedef RTT::DataSourceBase::const_ptr SourceType;
    typedef CORBA::Any                             ResultType;

    virtual void copy( RTT::DataSourceBase::shared_ptr new_ds ) {
        morig = new_ds;
    }

  // Constructor 
  Orocos_AnyExpression_i (RTT::DataSourceBase::const_ptr orig, PortableServer::POA_ptr the_poa)
      : morig( orig ), last_value( morig->createAny() ) // create default Any.
        , mpoa( PortableServer::POA::_duplicate(the_poa) )
    {}

    PortableServer::POA_ptr _default_POA()
    {
        return PortableServer::POA::_duplicate(mpoa);
    }
  
  // Destructor 
    virtual ~Orocos_AnyExpression_i (void) {}
  
  virtual
  CORBA::Any* value (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return new CORBA::Any( last_value.in() );
  }
  
  virtual
  CORBA::Boolean evaluate (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      last_value = morig->getAny();
      bool result = true;
      // if it is a bool, update result and return it, otherwise, just return true:
      RTT::AnyConversion<bool>::update( last_value.in(), result );
      return result;
  }
  
  virtual
  CORBA::Any * get (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return morig->getAny();
  }
  
  virtual
  char * getType (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( morig->getType().c_str() );
  }

  virtual
  char * getTypeName (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( morig->getTypeName().c_str() );
  }

  virtual void destroyExpression()
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
 {
  }

};

template<>
class  Orocos_Expression_i<void>
  : public virtual POA_RTT::Corba::Expression,
    public virtual PortableServer::RefCountServantBase
{
protected:
    RTT::DataSource<void>::const_ptr morig;
    PortableServer::POA_var mpoa;
public:
    typedef void SourceType;
    typedef void ResultType;

  // Constructor 
  Orocos_Expression_i (RTT::DataSource<void>::const_ptr orig, PortableServer::POA_ptr the_poa)
      : morig( orig ), mpoa( PortableServer::POA::_duplicate(the_poa) )
    {}

    PortableServer::POA_ptr _default_POA()
    {
        return PortableServer::POA::_duplicate(mpoa);
    }
  
  // Destructor 
    virtual ~Orocos_Expression_i (void) {}
  
  virtual
  CORBA::Any* value (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return new CORBA::Any();
  }
  
  virtual
  CORBA::Boolean evaluate (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      morig->get();
      return 1;
  }
  
  virtual
  CORBA::Any * get (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      morig->get();
      return new CORBA::Any();
  }
  
  virtual
  char * getType (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( morig->getType().c_str() );
  }

  virtual
  char * getTypeName (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( morig->getTypeName().c_str() );
  }

  virtual void destroyExpression()
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
 {
  }

};

template< class DataType>
class  Orocos_AssignableExpression_i
  : public virtual POA_RTT::Corba::AssignableExpression,
    public virtual PortableServer::RefCountServantBase
{
    typename RTT::AssignableDataSource<DataType>::shared_ptr massign;
    typename RTT::DataSource<DataType>::value_t last_value;
    PortableServer::POA_var mpoa;
public:
    typedef DataType SourceType;
    typedef typename RTT::DataSource<DataType>::value_t ResultType;

  // Constructor 
  Orocos_AssignableExpression_i (typename RTT::AssignableDataSource<SourceType>::shared_ptr assign, PortableServer::POA_ptr the_poa )
      : massign( assign ), last_value(), mpoa( PortableServer::POA::_duplicate(the_poa) )
      {}

    PortableServer::POA_ptr _default_POA()
    {
        return PortableServer::POA::_duplicate(mpoa);
    }
  
  // Destructor 
    virtual ~Orocos_AssignableExpression_i (void) {}
  
  virtual
  CORBA::Boolean set (
      const ::CORBA::Any & value
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      
      
      if ( !RTT::AnyConversion<ResultType>::update( value, last_value ) ) {
          RTT::log(RTT::Error) << "Corba::AssignableExpression: Could not assign Any to "<<massign->getType() <<"." <<RTT::endlog()
							   <<" Tried to assign as "<< RTT::DataSource<ResultType>::GetType() << " to native type "<< RTT::DataSource<SourceType>::GetType()<< RTT::endlog();
          return false;
      }
      massign->set(last_value);
      return true;
  }
      
  virtual
  CORBA::Any* value (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return RTT::AnyConversion<ResultType>::createAny( last_value );
  }
  
  virtual
  CORBA::Boolean evaluate (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      last_value = massign->get();
      return 1;
  }
  
  virtual
  CORBA::Any * get (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return RTT::AnyConversion<ResultType>::createAny( massign->get() );
  }
  
  virtual
  char * getType (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( massign->getType().c_str() );
  }

  virtual
  char * getTypeName (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( massign->getTypeName().c_str() );
  }

  virtual void destroyExpression()
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
 {
  }

};

class  Orocos_AnyAssignableExpression_i
    : public Orocos_AnyExpression_i,
      public virtual POA_RTT::Corba::AssignableExpression
{
    RTT::DataSourceBase::shared_ptr mset;
public:

    virtual void copy( RTT::DataSourceBase::shared_ptr new_ds ) {
        mset = new_ds;
        morig = new_ds;
    }

  // Constructor 
  Orocos_AnyAssignableExpression_i (RTT::DataSourceBase::shared_ptr orig, PortableServer::POA_ptr the_poa)
      : Orocos_AnyExpression_i(orig, the_poa), mset( orig )
   {}
  
  // Destructor 
    virtual ~Orocos_AnyAssignableExpression_i (void) {}

  virtual
  CORBA::Boolean set (
      const ::CORBA::Any & value
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return mset->update( value );
  }
      
};


// Boolean Specialisations, like in RTT::DataSource<bool>:

template<>
class  Orocos_Expression_i<bool>
  : public virtual POA_RTT::Corba::Expression,
    public virtual PortableServer::RefCountServantBase
{
protected:
    RTT::DataSource<bool>::const_ptr morig;
    RTT::DataSource<bool>::value_t last_value;
    PortableServer::POA_var mpoa;
public:
    typedef bool SourceType;
    typedef bool ResultType;

  // Constructor 
  Orocos_Expression_i (RTT::DataSource<bool>::const_ptr orig, PortableServer::POA_ptr the_poa)
      : morig( orig ), last_value(), mpoa( PortableServer::POA::_duplicate(the_poa) )
    {}

    PortableServer::POA_ptr _default_POA()
    {
        return PortableServer::POA::_duplicate(mpoa);
    }

  // Destructor 
    virtual ~Orocos_Expression_i (void) {}
  
  virtual
  CORBA::Any* value (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return RTT::AnyConversion<bool>::createAny( last_value );
  }
  
  virtual
  CORBA::Boolean evaluate (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      last_value = morig->get();
      return last_value;
  }
  
  virtual
  CORBA::Any * get (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      last_value = morig->get();
      return RTT::AnyConversion<bool>::createAny( last_value );
  }
  
  virtual
  char * getType (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( morig->getType().c_str() );
  }

  virtual
  char * getTypeName (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( morig->getTypeName().c_str() );
  }

  virtual void destroyExpression()
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
 {
  }

};

template<>
class  Orocos_AssignableExpression_i<bool>
  : public virtual POA_RTT::Corba::AssignableExpression,
    public virtual PortableServer::RefCountServantBase
{
    RTT::AssignableDataSource<bool>::shared_ptr massign;
    bool last_value;
    PortableServer::POA_var mpoa;
public:
    typedef bool SourceType;
    typedef bool ResultType;

  // Constructor 
  Orocos_AssignableExpression_i (RTT::AssignableDataSource<bool>::shared_ptr assign , PortableServer::POA_ptr the_poa)
      : massign( assign ), last_value(), mpoa( PortableServer::POA::_duplicate(the_poa) )
    {}

    PortableServer::POA_ptr _default_POA()
    {
        return PortableServer::POA::_duplicate(mpoa);
    }
  
  // Destructor 
    virtual ~Orocos_AssignableExpression_i (void) {}
  
  virtual
  CORBA::Boolean set (
      const ::CORBA::Any & value
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return RTT::AnyConversion<bool>::update( value, massign->set() );
  }
      
  virtual
  CORBA::Any* value (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return RTT::AnyConversion<bool>::createAny( last_value );
  }
  
  virtual
  CORBA::Boolean evaluate (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      last_value = massign->evaluate();
      return last_value;
  }
  
  virtual
  CORBA::Any * get (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      last_value =massign->get();
      return RTT::AnyConversion<bool>::createAny( last_value );
  }
  
  virtual
  char * getType (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( massign->getType().c_str() );
  }

  virtual
  char * getTypeName (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( massign->getTypeName().c_str() );
  }

  virtual void destroyExpression()
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
 {
  }

};

class  Orocos_Action_i
    : public virtual POA_RTT::Corba::Action,
      public virtual PortableServer::RefCountServantBase
{
    RTT::CommandInterface* mcom;
    PortableServer::POA_var mpoa;
public:
  //Constructor 
  Orocos_Action_i ( RTT::CommandInterface* com, PortableServer::POA_ptr the_poa );

    PortableServer::POA_ptr _default_POA()
    {
        return PortableServer::POA::_duplicate(mpoa);
    }
  
  //Destructor 
    virtual ~Orocos_Action_i (void);
  
  virtual
  CORBA::Boolean execute (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      ));

  virtual
  void reset (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      ));

  virtual void destroyAction()
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
 {
  }

};

template<class T>
class  Orocos_Method_i
    : public virtual Orocos_Expression_i<T>,
      public virtual POA_RTT::Corba::Method
{
public:
    typedef T SourceType;
    typedef typename RTT::DataSource<T>::value_t ResultType;
    typename RTT::DataSource<SourceType>::shared_ptr mmethod;
  //Constructor 
  Orocos_Method_i ( typename RTT::DataSource<SourceType>::shared_ptr datas, PortableServer::POA_ptr the_poa )
      : Orocos_Expression_i<SourceType>( datas, the_poa ), mmethod( datas )
    {}
  
  //Destructor 
    virtual ~Orocos_Method_i (void) {}
  
  virtual
  CORBA::Boolean execute (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return this->evaluate();
  }

  virtual
  void reset(
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      this->mmethod->reset();
  }
};

class  Orocos_AnyMethod_i
    : public virtual Orocos_AnyExpression_i,
      public virtual POA_RTT::Corba::Method
{
public:
    RTT::DataSourceBase::shared_ptr mmethod;

    virtual void copy( RTT::DataSourceBase::shared_ptr new_ds ) {
        mmethod = new_ds;
        morig = new_ds;
    }

  //Constructor 
  Orocos_AnyMethod_i ( RTT::DataSourceBase::shared_ptr datas, PortableServer::POA_ptr the_poa )
      : Orocos_AnyExpression_i( datas, the_poa ), mmethod( datas )
    {}
  
  //Destructor 
    virtual ~Orocos_AnyMethod_i (void) {}
  
  virtual
  CORBA::Boolean execute (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return this->evaluate();
  }

  virtual
  void reset(
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      this->mmethod->reset();
  }

  virtual void destroyAction()
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
 {
  }

};


class  Orocos_Command_i
    : public virtual POA_RTT::Corba::Command,
      public virtual PortableServer::RefCountServantBase
{
    RTT::CommandC* morig;
    PortableServer::POA_var mpoa;
public:
  //Constructor 
  Orocos_Command_i (RTT::CommandC& c, PortableServer::POA_ptr the_poa);

    PortableServer::POA_ptr _default_POA()
    {
        return PortableServer::POA::_duplicate(mpoa);
    }
  
  //Destructor 
  virtual ~Orocos_Command_i (void);
  
  virtual
  CORBA::Boolean execute (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  CORBA::Boolean sent (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  CORBA::Boolean accepted (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  CORBA::Boolean executed (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  CORBA::Boolean valid (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  CORBA::Boolean done (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void reset (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual void destroyCommand()
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
};

#endif /* INCLUDE_EXPRESSIONI_H_  */

