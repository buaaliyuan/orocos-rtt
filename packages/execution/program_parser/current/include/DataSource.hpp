/***************************************************************************
  tag: Peter Soetens  Mon Jan 19 14:11:25 CET 2004  DataSource.hpp 

                        DataSource.hpp -  description
                           -------------------
    begin                : Mon January 19 2004
    copyright            : (C) 2004 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be
 
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
 
#ifndef DATASOURCE_HPP
#define DATASOURCE_HPP

#include <boost/intrusive_ptr.hpp>
#include <boost/type_traits.hpp>

namespace
{
  // combines boost::remove_reference and boost::remove_const, is
  // basically a type manipulator that returns T for a type const T&,
  // and T for other types T...
  template<typename T>
  struct remove_cr
  {
    typedef typename boost::remove_const<
      typename boost::remove_reference<T>::type>::type type;
  };
}

namespace ORO_Execution
{
  /**
   * @brief The base class for all DataSource's
   *
   * We make all DataSource's inherit a base class, so that we can
   * treat them alike.  Among other things, it allows us to have a
   * single DataSourceFactory per component.  The base class has no
   * methods, users are expected to use C++ RTTI and casting ( read:
   * dynamic_cast ) to get hold of the subclasses..
   *
   * DataSource's are reference counted.  Use
   * DataSourceBase::shared_ptr or DataSource<T>::shared_ptr to deal
   * with this automatically, or don't forget to call ref and deref..
   *
   * @see DataSource
   */
  class DataSourceBase
  {
    // We keep the refcount ourselves.  We aren't using
    // boost::shared_ptr, because boost::intrusive_ptr is better,
    // exactly because it can be used with refcounts that are stored
    // in the class itself.  Advantages are that the shared_ptr's for
    // derived classes use the same refcount, which is of course very
    // much desired, and that refcounting happens in an efficient way,
    // which is also nice :)
    int refcount;
  public:
    typedef boost::intrusive_ptr<DataSourceBase> shared_ptr;

    DataSourceBase() : refcount( 0 ) {};
    void ref() { ++refcount; };
    void deref() { if ( --refcount <= 0 ) delete this; };
    virtual ~DataSourceBase();

    virtual void reset();

  };

  /**
   * DataSource is a base class representing a generic way to get a
   * piece of data from somewhere.  A standard VariableDataSource,
   * which just keeps a value and returns it every time in its get()
   * method is included below ( @ref VariableDataSource ).
   *
   * NOTES/TODO: Condition is remarkably similar to DataSource<bool>,
   * and perhaps it would be useful to somehow merge the two..  Or
   * perhaps just providing adaptors in two directions will do ?
   * Currently, the second approach is taken, with adapters in
   * interfacing/src/ConditionBoolDataSource.hpp and
   * parser/src/DataSourceCondition.hpp.
   */
  template<typename T>
  class DataSource
    : public DataSourceBase
  {
  public:
      typedef typename boost::intrusive_ptr<DataSource<T> > shared_ptr;

      virtual ~DataSource();
      /**
       * return the data you need to return..
       */
      virtual T get() const = 0;

      /**
       * Clone Software Pattern.
       */
      virtual DataSource<T>* clone() const = 0;
  };

  template<typename T>
  DataSource<T>::~DataSource()
  {
  }

  /**
   * A simple, yet very useful DataSource, which keeps a value, and
   * returns it in its get() method.  This used to be named
   * ConstantDataSource, but I have recently added support for
   * changing the value..
   */
  template<typename T>
  class VariableDataSource
    : public DataSource<T>
  {
      T mdata;
  public:
      typedef boost::intrusive_ptr<VariableDataSource<T> > shared_ptr;

      VariableDataSource( T data )
          : mdata( data )
      {
      }

      T get() const
      {
          return mdata;
      }

      void set( T t )
      {
          mdata = t;
      }
      
      VariableDataSource<T>* duplicate() const
      {
          return new VariableDataSource<T>(mdata);
      }

      virtual VariableDataSource<T>* clone() const
      {
          return new VariableDataSource<T>(mdata);
      }
  };

  // No longer seems too useful, now that I've seen what Property's
  // are mostly used for in OROCOS.
//   /**
//    * Another generic and often useful DataSource, which gets its data
//    * from a Property every time..
//    */
//   template<typename T>
//   class PropertyDataSource
//     : public DataSource<T>
//   {
//     const Property<T>& mprop;
//     typedef typename DataSource<T>::value_t value_t;
//   public:
//     PropertyDataSource( const Property<T>& prop )
//       : mprop( prop )
//       {
//       }

//     value_t get() const
//       {
//         return mprop.get();
//       }
//   };

  /**
   * A generic binary composite DataSource.  It takes a function
   * object which is a model of the STL Adaptable Binary Function
   * concept, and two DataSource's with result types matching the
   * argument types of the given function.  Its get() method then
   * applies the function to the results of the get() methods of the
   * two DataSource's given.
   *
   * The parser system uses a hell of a lot of these for evaluating
   * expressions, comparisons etc.  I must say I think this is pretty
   * cool ;)  C++ rocks ! ;)
   */
  template<typename function>
  class BinaryDataSource
    : public DataSource<typename function::result_type>
  {
    typedef typename function::result_type value_t;
    typedef typename remove_cr<typename function::first_argument_type>::type first_arg_t;
    typedef typename remove_cr<typename function::second_argument_type>::type second_arg_t;
    typename DataSource<first_arg_t>::shared_ptr ma;
    typename DataSource<second_arg_t>::shared_ptr mb;
    function fun;
  public:
    typedef boost::intrusive_ptr<BinaryDataSource<function> > shared_ptr;

    BinaryDataSource( DataSource<first_arg_t>* a,
                      DataSource<second_arg_t>* b,
                      function f )
      : ma( a ), mb( b ), fun( f )
      {
      };

    virtual value_t get() const
      {
        first_arg_t a = ma->get();
        second_arg_t b = mb->get();
        return fun( a, b );
      }

    virtual void reset()
      {
        ma->reset();
        mb->reset();
      }

      virtual DataSource<typename function::result_type>* clone() const
      {
          return new BinaryDataSource<function>(ma->clone(), mb->clone(), fun);
      }
  };

  /**
   * The extension of BinaryDataSource to ternary functions..
   */
  template<typename function>
  class TernaryDataSource
    : public DataSource<typename function::result_type>
  {
    typedef typename function::result_type value_t;
    typedef typename remove_cr<typename function::first_argument_type>::type first_arg_t;
    typedef typename remove_cr<typename function::second_argument_type>::type second_arg_t;
    typedef typename remove_cr<typename function::third_argument_type>::type third_arg_t;
    typename DataSource<first_arg_t>::shared_ptr ma;
    typename DataSource<second_arg_t>::shared_ptr mb;
    typename DataSource<third_arg_t>::shared_ptr mc;
    function fun;
  public:
    typedef boost::intrusive_ptr<TernaryDataSource<function> > shared_ptr;

    TernaryDataSource( DataSource<first_arg_t>* a,
                       DataSource<second_arg_t>* b,
                       DataSource<third_arg_t>* c,
                       function f )
      : ma( a ), mb( b ), mc( c ), fun( f )
      {
      };

    virtual value_t get() const
      {
        first_arg_t a = ma->get();
        second_arg_t b = mb->get();
        third_arg_t c = mc->get();
        return fun( a, b, c );
      }

    virtual void reset()
      {
        ma->reset();
        mb->reset();
        mc->reset();
      }

      virtual DataSource<typename function::result_type>* clone() const
      {
          return new TernaryDataSource<function>(ma->clone(), mb->clone(), mc->clone(), fun);
      }
  };

  /**
   * The extension of BinaryDataSource to unary functions..
   */
  template <typename function>
  class UnaryDataSource
    : public DataSource<typename function::result_type>
  {
    typedef typename function::result_type value_t;
    typedef typename remove_cr<typename function::argument_type>::type arg_t;
    typename DataSource<arg_t>::shared_ptr ma;
    function fun;
  public:
    typedef boost::intrusive_ptr<UnaryDataSource<function> > shared_ptr;

    UnaryDataSource( DataSource<arg_t>* a, function f )
      : ma( a ), fun( f )
      {
      };

    virtual value_t get() const
      {
        return fun( ma->get() );
      }

    void reset()
      {
        ma->reset();
      }

      virtual DataSource<typename function::result_type>* clone() const
      {
          return new UnaryDataSource<function>(ma->clone(), fun);
      }
  };
}

void intrusive_ptr_add_ref( ORO_Execution::DataSourceBase* p );
void intrusive_ptr_release( ORO_Execution::DataSourceBase* p );

#endif
