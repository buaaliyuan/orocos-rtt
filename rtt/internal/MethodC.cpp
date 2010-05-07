/***************************************************************************
  tag: Peter Soetens  Wed Jan 18 14:11:40 CET 2006  MethodC.cxx

                        MethodC.cxx -  description
                           -------------------
    begin                : Wed January 18 2006
    copyright            : (C) 2006 Peter Soetens
    email                : peter.soetens@mech.kuleuven.be

 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public                   *
 *   License as published by the Free Software Foundation;                 *
 *   version 2 of the License.                                             *
 *                                                                         *
 *   As a special exception, you may use this file as part of a free       *
 *   software library without restriction.  Specifically, if other files   *
 *   instantiate templates or use macros or inline functions from this     *
 *   file, or you compile this file and link it with other files to        *
 *   produce an executable, this file does not by itself cause the         *
 *   resulting executable to be covered by the GNU General Public          *
 *   License.  This exception does not however invalidate any other        *
 *   reasons why the executable file might be covered by the GNU General   *
 *   Public License.                                                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public             *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/


#include "MethodC.hpp"
#include "../interface/FactoryExceptions.hpp"
#include "DataSourceCommand.hpp"
#include "../interface/ServiceProvider.hpp"
#include "../Logger.hpp"
#include "Exceptions.hpp"
#include <vector>

namespace RTT {
    using namespace detail;


    class MethodC::D
    {
    public:
        OperationRepositoryPart* ofp;
        ExecutionEngine* caller;
        std::string mname;
        std::vector<DataSourceBase::shared_ptr> args;
        DataSourceBase::shared_ptr rta;
        DataSourceBase::shared_ptr m;
        DataSourceBase::shared_ptr s;

        void checkAndCreate() {
            Logger::In in("MethodC");
            if ( ofp ) {
                size_t sz = ofp->arity();
                if ( sz == args.size() ) {
                    // may throw or return nill
                    m = ofp->produce(args, caller );
                    s = ofp->produceSend(args, caller );
                    args.clear();
                    if ( !m )
                        return;
                    if (rta)
                        try {
                            m = new DataSourceCommand( rta->updateAction( m.get() ) );
                        } catch( bad_assignment& ba ) {
                            log(Error) << "Error in MethodC::ret : can not convert return value of type "<< m->getType() << " to given type "<< rta->getType()<<endlog();
                        }

                }
            }
        }

        void newarg(DataSourceBase::shared_ptr na)
        {
            this->args.push_back( na );
            this->checkAndCreate();
        }

        void ret(AttributeBase* r)
        {
            this->rta = r->getDataSource();
        }

        void ret(DataSourceBase::shared_ptr d)
        {
            this->rta = d;
        }

        D( OperationRepositoryPart* mr, const std::string& name, ExecutionEngine* caller)
            : ofp(mr), caller(caller), mname(name), rta(), m(), s()
        {
            this->checkAndCreate();
        }

        D(const D& other)
            : ofp(other.ofp), caller(other.caller), mname(other.mname),
              args( other.args ), rta( other.rta ), m( other.m ), s(other.s)
        {
        }

        ~D()
        {
        }

    };

    MethodC::MethodC()
        : d(0), m()
    {
    }

    MethodC::MethodC(OperationRepositoryPart* mr, const std::string& name, ExecutionEngine* caller)
        : d( mr ? new D( mr, name, caller) : 0 ), m(), ofp(mr), mname(name)
    {
        if ( d && d->m ) {
            this->m = d->m;
            this->s = d->s;
            delete d;
            d = 0;
        } else {
            if (mr == 0)
                log(Error) <<"Can not construct MethodC for '"<<name<<"' from null OperationRepositoryPart."<<endlog();
        }
    }

    MethodC::MethodC(const MethodC& other)
        : d( other.d ? new D(*other.d) : 0 ), m( other.m ? other.m : 0), ofp(other.ofp), mname(other.mname)
    {
    }

    MethodC& MethodC::operator=(const MethodC& other)
    {
        if ( &other == this )
            return *this;
        delete d;
        d = ( other.d ? new D(*other.d) : 0 );
        m = other.m;
        s = other.s;
        ofp = other.ofp;
        mname = other.mname;
        return *this;
    }

    MethodC::~MethodC()
    {
        delete d;
    }

    MethodC& MethodC::arg( DataSourceBase::shared_ptr a )
    {
        if (d)
            d->newarg( a );
        else {
            Logger::log() <<Logger::Warning << "Extra argument discarded for MethodC."<<Logger::endl;
        }
        if ( d && d->m ) {
            this->m = d->m;
            this->s = d->s;
            delete d;
            d = 0;
        }
        return *this;
    }

    MethodC& MethodC::ret( AttributeBase* r )
    {
        if (d)
            d->ret( r );
        else {
            if (m) {
                try {
                    m = new DataSourceCommand(r->getDataSource()->updateAction( m.get() ) );
                } catch( bad_assignment& ba ) {
                    log(Error) << "Error in MethodC::ret : can not convert return value of type "<< m->getType() << " to given type "<< r->getDataSource()->getType()<<endlog();
                }
            } else
                log(Error) <<"Can not add return argument to invalid MethodC."<<endlog();
        }
        return *this;
    }

    MethodC& MethodC::ret(DataSourceBase::shared_ptr r)
    {
        if (d)
            d->ret( r );
        else {
            // no d, store manually:
            if (m)
                m = new DataSourceCommand(r->updateAction( m.get() ) );
            else
                log(Error) <<"Can not add return argument to invalid MethodC."<<endlog();
        }
        return *this;
    }


    bool MethodC::call() {
        if (m)
            return m->evaluate();
        else {
            Logger::log() <<Logger::Error << "call() called on incomplete MethodC."<<Logger::endl;
            if (d) {
                size_t sz;
                sz = d->ofp->arity();
                Logger::log() <<Logger::Error << "Wrong number of arguments provided for method '"+d->mname+"'"<<Logger::nl;
                Logger::log() <<Logger::Error << "Expected "<< sz << ", got: " << d->args.size() <<Logger::endl;
            }
        }
        return false;
    }

    void MethodC::check() {
        if (d) {
            // something went wrong, let producer throw
            if (d->ofp)
                DataSourceBase::shared_ptr dummy = d->ofp->produce( d->args, d->caller );
            else
                throw name_not_found_exception( d->mname );
        }
    }

    SendHandleC MethodC::send() {
        if (s) {
            DataSourceBase::shared_ptr h = ofp->produceHandle();
            // evaluate and copy result of s to handle and pass handle to SendHandleC.
            bool result = h->update( s.get() );
            assert( result );
            return SendHandleC( h, ofp, mname );
        }
        else {
            Logger::log() <<Logger::Error << "send() called on incomplete MethodC."<<Logger::endl;
            if (d) {
                size_t sz;
                sz = d->ofp->arity();
                Logger::log() <<Logger::Error << "Wrong number of arguments provided for method '"+d->mname+"'"<<Logger::nl;
                Logger::log() <<Logger::Error << "Expected "<< sz << ", got: " << d->args.size() <<Logger::endl;
            }
        }
        return SendHandleC();
    }

    bool MethodC::ready() const
    {
        return m;
    }

    DataSourceBase::shared_ptr MethodC::getCallDataSource() { return m; }
    DataSourceBase::shared_ptr MethodC::getSendDataSource() { return s; }
}
