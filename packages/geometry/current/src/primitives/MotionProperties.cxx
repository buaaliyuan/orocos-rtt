/***************************************************************************
  tag: Peter Soetens  Mon May 10 19:10:36 CEST 2004  MotionProperties.cxx 

                        MotionProperties.cxx -  description
                           -------------------
    begin                : Mon May 10 2004
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

#include <rtt/Property.hpp>
#include <rtt/PropertyIntrospection.hpp>
#include "geometry/MotionProperties.hpp"
#include <pkgconf/geometry.h>
#include <rtt/Logger.hpp>

namespace RTT
{
    template class Property<ORO_Geometry::Frame>;
    template class Property<Double6D>;
    template class Property<ORO_Geometry::Wrench>;
    template class Property<ORO_Geometry::Twist>;
    template class Property<ORO_Geometry::Rotation>;
    template class Property<ORO_Geometry::Vector>;
}

namespace ORO_Geometry
{
    using namespace RTT;

    /**
     *
     * MotionControl::Double6D
     *
     */
    
    /**
     * A Decomposer for Decomposing a Double6D or Property<Double6D>
     * into a typed Property<PropertyBag>.
     */
    class Double6DDecomposer
    {
        PropertyBag resultBag;
        Property<double> D1;
        Property<double> D2;
        Property<double> D3;
        Property<double> D4;
        Property<double> D5;
        Property<double> D6;
        
    public: 
        
        Double6DDecomposer( const Double6D& v );
        
        PropertyBag& result() { return resultBag; }
    };
        
    /**
     * An Composer for constructing a Double6D or Property<Double6D>
     * from a typed PropertyBag.
     */
    class Double6DComposer
    {
        const PropertyBag& bag;
    public:
        Double6DComposer( const PropertyBag& _bag )
            : bag(_bag)
        {}

        bool getResult( Double6D& res );
    };
            
    /**
     * @todo TODO : Put the strings/names in a separate file/struct/...
     */
    Double6DDecomposer::Double6DDecomposer( const Double6D& d)
        : resultBag("MotCon::Double6D"), // bag_type
          D1("D1","D1 Value", d[0]),
          D2("D2","D2 Value", d[1]),
          D3("D3","D3 Value", d[2]),
          D4("D4","D4 Value", d[3]),
          D5("D5","D5 Value", d[4]),
          D6("D6","D6 Value", d[5])
    {
        resultBag.add(D1.clone());
        resultBag.add(D2.clone());
        resultBag.add(D3.clone());
        resultBag.add(D4.clone());
        resultBag.add(D5.clone());
        resultBag.add(D6.clone());
    }

    bool Double6DComposer::getResult( Double6D& res )
    {
        if ( bag.getType() == "MotCon::Double6D" )
            {
                Property<double>* d1 = dynamic_cast<Property<double>*>( bag.find("D1") );
                Property<double>* d2 = dynamic_cast<Property<double>*>( bag.find("D2") );
                Property<double>* d3 = dynamic_cast<Property<double>*>( bag.find("D3") );
                Property<double>* d4 = dynamic_cast<Property<double>*>( bag.find("D4") );
                Property<double>* d5 = dynamic_cast<Property<double>*>( bag.find("D5") );
                Property<double>* d6 = dynamic_cast<Property<double>*>( bag.find("D6") );
                // found it.
                if ( d1 != 0 && d2 != 0  && d3 != 0 && d4 != 0 && d5 != 0  && d6 != 0)
                    {
                        res[0] = d1->get();
                        res[1] = d2->get();
                        res[2] = d3->get();
                        res[3] = d4->get();
                        res[4] = d5->get();
                        res[5] = d6->get();
                        return true;
                    } else {
                        std::string element = !d1 ? "D1" : !d2 ? "D2" : !d3 ? "D3" : !d4 ? "D4" : !d5 ? "D5" :  "D6";
                        Logger::log() << Logger::Error << "Aborting composition of Property< Double6D > "
                                      << ": Missing element '" <<element<<"'." <<Logger::endl;
                        return false;
                    }
            } else {
                Logger::log() << Logger::Error << "Aborting composition of Property< Double6D > "
                              << ": Expected type 'MotCon::Double6D', got type '"<< bag.getType() <<"'."
                              <<Logger::endl;
            }
        return false;
    }

    /**
     *
     *    ORO_Geometry::Vector
     *
     */

    /**
     * A Decomposer for Decomposing a Vector or Property<Vector>
     * into a typed Property<PropertyBag>.
     */
    class VectorDecomposer
    {
        PropertyBag resultBag;
        Property<double> X;
        Property<double> Y;
        Property<double> Z;
        
    public: 
        
        VectorDecomposer( const ORO_Geometry::Vector& v);
        
        PropertyBag& result() { return resultBag; }
    };
        
    /**
     * An Composer for constructing a Vector or Property<Vector>
     * from a typed PropertyBag.
     */
    class VectorComposer
    {
        const PropertyBag& bag;
    public:
        VectorComposer( const PropertyBag& _bag )
            : bag(_bag)
        {}

        bool getResult( ORO_Geometry::Vector& res);
    };
            
    VectorDecomposer::VectorDecomposer( const ORO_Geometry::Vector& v )
        : resultBag("MotCon::Vector"), // bag_type
          X("X","X Value", v[0]),
          Y("Y","Y Value", v[1]),
          Z("Z","Z Value", v[2])
    {
        resultBag.add(X.clone());
        resultBag.add(Y.clone());
        resultBag.add(Z.clone());
    }

    bool VectorComposer::getResult( ORO_Geometry::Vector& res)
    {
        if ( bag.getType() == "MotCon::Vector" )
            {
                Property<double>* px = dynamic_cast<Property<double>*>( bag.find("X") );
                Property<double>* py = dynamic_cast<Property<double>*>( bag.find("Y") );
                Property<double>* pz = dynamic_cast<Property<double>*>( bag.find("Z") );
                // found it.
                if ( px != 0 && py != 0  && pz != 0)
                    {
                        res = ORO_Geometry::Vector( px->get(),py->get(),pz->get() );
                        return true;
                    } else {
                        std::string element = !px ? "X" : !py ? "Y" : "Z";
                        Logger::log() << Logger::Error << "Aborting composition of Property< Vector > "
                                      << ": Missing element '" <<element<<"'." <<Logger::endl;
                        return false;
                    }
            } else {
                Logger::log() << Logger::Error << "Aborting composition of Property< Vector > "
                              << ": Expected type 'MotCon::Vector', got type '"<< bag.getType() <<"'."
                              <<Logger::endl;
            }
        return false;
    }


    /**
     *
     *    ORO_Geometry::Rotation
     *
     */

    /**
     * A Decomposer for Converting a Rotation or Property<Rotation>
     * into a typed Property<PropertyBag>.
     */
    class RotationDecomposer
    {
        PropertyBag resultBag;
        Property<double> X_x;
        Property<double> X_y;
        Property<double> X_z;
        Property<double> Y_x;
        Property<double> Y_y;
        Property<double> Y_z;
        Property<double> Z_x;
        Property<double> Z_y;
        Property<double> Z_z;
    public: 
        
        RotationDecomposer( const ORO_Geometry::Rotation& r );
        
        PropertyBag& result() { return resultBag; }
    };
    
    /**
     * An Composer for constructing a Rotation or Property<Rotation>
     * from a typed PropertyBag.
     */
    class RotationComposer
    {
        const PropertyBag& bag;
    public:
        RotationComposer( const PropertyBag& _bag )
            :  bag(_bag)
        {}

        bool getResult( ORO_Geometry::Rotation& res );
    };
            
    RotationDecomposer::RotationDecomposer( const ORO_Geometry::Rotation& r)
        : resultBag("MotCon::Rotation"),
          X_x("X_x","", r(0,0) ),
          X_y("X_y","", r(0,1) ),
          X_z("X_z","", r(0,2) ),
          Y_x("Y_x","", r(1,0) ),
          Y_y("Y_y","", r(1,1) ),
          Y_z("Y_z","", r(1,2) ),
          Z_x("Z_x","", r(2,0) ),
          Z_y("Z_y","", r(2,1) ),
          Z_z("Z_z","", r(2,2) )
    {
        resultBag.add(X_x.clone());
        resultBag.add(X_y.clone());
        resultBag.add(X_z.clone());
        resultBag.add(Y_x.clone());
        resultBag.add(Y_y.clone());
        resultBag.add(Y_z.clone());
        resultBag.add(Z_x.clone());
        resultBag.add(Z_y.clone());
        resultBag.add(Z_z.clone());
    }

    bool RotationComposer::getResult( ORO_Geometry::Rotation& res)
    {
        if ( bag.getType() == "MotCon::Rotation" )
            {
                Property<double>* X_x = dynamic_cast<Property<double>*>( bag.find("X_x") );
                Property<double>* X_y = dynamic_cast<Property<double>*>( bag.find("X_y") );
                Property<double>* X_z = dynamic_cast<Property<double>*>( bag.find("X_z") );
                Property<double>* Y_x = dynamic_cast<Property<double>*>( bag.find("Y_x") );
                Property<double>* Y_y = dynamic_cast<Property<double>*>( bag.find("Y_y") );
                Property<double>* Y_z = dynamic_cast<Property<double>*>( bag.find("Y_z") );
                Property<double>* Z_x = dynamic_cast<Property<double>*>( bag.find("Z_x") );
                Property<double>* Z_y = dynamic_cast<Property<double>*>( bag.find("Z_y") );
                Property<double>* Z_z = dynamic_cast<Property<double>*>( bag.find("Z_z") );
                // found it.
                if (  X_x != 0 && X_y != 0  && X_z != 0 &&
                      Y_x != 0 && Y_y != 0  && Y_z != 0 &&
                      Z_x != 0 && Z_y != 0  && Z_z != 0 )
                    {
                        res = ORO_Geometry::Rotation( 
                                                     X_x->get(), Y_x->get(),Z_x->get(),
                                                     X_y->get(),Y_y->get(),Z_y->get(),
                                                     X_z->get(),Y_z->get(),Z_z->get() 
                                                     );
                        return true;
                    }
            }
        return false;
    }

    /**
     * A Decomposer for Converting a Rotation or Property<Rotation>
     * into a typed Property<PropertyBag> with the EulerZYX convention.
     */
    class EulerZYXDecomposer
    {
        PropertyBag resultBag;
        Property<double> _a;
        Property<double> _b;
        Property<double> _g;

    public: 
        
        EulerZYXDecomposer( const ORO_Geometry::Rotation& r);
        
        PropertyBag& result() { return resultBag; }
    };
    
    /**
     * An Composer for constructing an EulerZYX Rotation or Property<Rotation>
     * from a typed PropertyBag.
     */
    class EulerZYXComposer
    {
        const PropertyBag& bag;
    public:
        EulerZYXComposer( const PropertyBag& _bag )
            :  bag(_bag)
        {}

        bool getResult( ORO_Geometry::Rotation& res );
    };
            
    EulerZYXDecomposer::EulerZYXDecomposer( const ORO_Geometry::Rotation& r)
        : resultBag("MotCon::EulerZYX"),
          _a("alpha","First Rotate around the Z axis with alpha in radians" ),
          _b("beta","Then Rotate around the new Y axis with beta in radians" ),
          _g("gamma","Then Rotation around the new X axis with gamma in radians" )
    {
        r.GetEulerZYX(_a.set(), _b.set(), _g.set());
        resultBag.add(_a.clone());
        resultBag.add(_b.clone());
        resultBag.add(_g.clone());
    }

    bool EulerZYXComposer::getResult( ORO_Geometry::Rotation& res )
    {
        if ( bag.getType() == "MotCon::EulerZYX" )
            {
                // ZYX is deprecated, use alpha, beta, gamma. also alpha maps to Z and gamma to X !
                Property<double>* _a = dynamic_cast<Property<double>*>( bag.find("alpha") );
                if ( !_a)
                    _a = dynamic_cast<Property<double>*>( bag.find("Z") );
                Property<double>* _b = dynamic_cast<Property<double>*>( bag.find("beta") );
                if ( !_b)
                    _b = dynamic_cast<Property<double>*>( bag.find("Y") );
                Property<double>* _g = dynamic_cast<Property<double>*>( bag.find("gamma") );
                if ( !_g)
                    _g = dynamic_cast<Property<double>*>( bag.find("X") );

                // found it.
                if (  _a != 0 && _b != 0  && _g != 0 )
                    {
                        res = ORO_Geometry::Rotation::EulerZYX(_a->get(), _b->get(), _g->get() );
                        return true;
                    } else {
                        std::string element = !_a ? "alpha" : !_b ? "beta" : "gamma";
                        Logger::log() << Logger::Error << "Aborting composition of (EulerZYX) Property< Rotation > "
                                      << ": Missing element '" <<element<<"'." <<Logger::endl;
                        return false;
                    }
            }
        return false;
    }

    /**
     * A Decomposer for Converting a Rotation or Property<Rotation>
     * into a typed Property<PropertyBag> with the RPY convention.
     */
    class RPYDecomposer
    {
        PropertyBag resultBag;
        Property<double> _r;
        Property<double> _p;
        Property<double> _y;

    public: 
        
        RPYDecomposer( const ORO_Geometry::Rotation& r);
        
        PropertyBag& result() { return resultBag; }
    };
    
    /**
     * An Composer for constructing an RPY Rotation or Property<Rotation>
     * from a typed PropertyBag.
     */
    class RPYComposer
    {
        const PropertyBag& bag;
    public:
        RPYComposer( const PropertyBag& _bag )
            :  bag(_bag)
        {}

        bool getResult( ORO_Geometry::Rotation& res);
    };
    RPYDecomposer::RPYDecomposer( const ORO_Geometry::Rotation& r)
        : resultBag("MotCon::RPY" ),
          _r("R","First rotate around X with R(oll) in radians" ),
          _p("P","Next rotate around old Y with P(itch) in radians" ),
          _y("Y","Next rotate around old Z with Y(aw) in radians" )
    {
        r.GetRPY(_r.set(), _p.set(), _y.set());
        resultBag.add(_r.clone());
        resultBag.add(_p.clone());
        resultBag.add(_y.clone());
    }

    bool RPYComposer::getResult( ORO_Geometry::Rotation& res)
    {
        if ( bag.getType() == "MotCon::RPY" )
            {
                Property<double>* _r = dynamic_cast<Property<double>*>( bag.find("R") );
                Property<double>* _p = dynamic_cast<Property<double>*>( bag.find("P") );
                Property<double>* _y = dynamic_cast<Property<double>*>( bag.find("Y") );

                // found it.
                if (  _r != 0 && _p != 0  && _y != 0 )
                    {
                        res = ORO_Geometry::Rotation::RPY(_r->get(), _p->get(), _y->get() );
                        return true;
                    } else {
                        std::string element = !_r ? "R" : !_p ? "P" : "Y";
                        Logger::log() << Logger::Error << "Aborting composition of (RPY) Property< Rotation > "
                                      << ": Missing element '" <<element<<"'." <<Logger::endl;
                        return false;
                    }
            }
        return false;
    }

    void decomposeProperty(const Double6D &v, PropertyBag& targetbag)
    {
        // construct a property with same name and description, but containing a typed PropertyBag.
        Double6DDecomposer vco(v);
        targetbag = vco.result();
    }

    bool composeProperty(const PropertyBag& bag, Double6D &v)
    {
        Double6DComposer vas( bag );
        return vas.getResult(v);
    }

    void decomposeProperty(const Vector &v, PropertyBag& targetbag)
    {
        // construct a property with same name and description, but containing a typed PropertyBag.
        VectorDecomposer vco(v);
        targetbag = vco.result();
    }

    bool composeProperty(const PropertyBag& bag, Vector &v)
    {
        VectorComposer vas( bag );
        return vas.getResult(v);
    }

    void decomposeProperty(const Rotation &b, PropertyBag& targetbag)
    {
        // construct a property with same name and description, but containing a typed PropertyBag.
#ifdef OROSEM_GEOMETRY_ROTATION_PROPERTIES_EULER
        EulerZYXDecomposer rot(b);
#else
# ifdef OROSEM_GEOMETRY_ROTATION_PROPERTIES_RPY
        RPYDecomposer rot(b);
# else
        RotationDecomposer rot(b);
# endif
#endif
        targetbag = rot.result();
    }

    bool composeProperty(const PropertyBag& bag, Rotation &r)
    {
        // try all three, see which one works, that one will fill in r.
        RPYComposer      rpyc(bag);
        EulerZYXComposer eulc(bag);
        RotationComposer rotc(bag);

        if ( rpyc.getResult( r ) || eulc.getResult( r ) || rotc.getResult( r ) )
            return true;
        else {
            Logger::log() << Logger::Error << "Aborting composition of Property< Rotation > "
                          << ": Expected type 'MotCon::Rotation','MotCon::EulerZYX' or 'MotCon::RPY', got type '"<< bag.getType() <<"'."
                          <<Logger::endl;
        }
        return false;
    }

    void decomposeProperty(const Twist &t, PropertyBag& targetbag)
    {
        targetbag.setType("MotCon::Twist"); // bag_type

        VectorDecomposer vel( t.vel );
        VectorDecomposer rot( t.rot );

        targetbag.add( new Property<PropertyBag>("Trans_Vel","Translational Velocity", vel.result() ) );
        targetbag.add( new Property<PropertyBag>("Rot_Vel","Rotational Velocity",rot.result() ));
    }

    bool composeProperty(const PropertyBag& bag, Twist &t)
    {
        if ( bag.getType() == std::string("MotCon::Twist") )
            {
                // pass the subbag to the vector Composers
                Property<PropertyBag>* subbag = bag.getProperty<PropertyBag>("Trans_Vel");
                if (! subbag ) {
                    Logger::log() << Logger::Error << "Aborting composition of Property< Twist > "
                                  << ": Trans_Vel not found."
                                  <<Logger::endl;
                    return false;
                }
                VectorComposer vas_vel( subbag->value() );

                subbag = bag.getProperty<PropertyBag>("Rot_Vel");
                if (! subbag ) {
                    Logger::log() << Logger::Error << "Aborting composition of Property< Twist > "
                                  << ": Rot_Vel not found."
                                  <<Logger::endl;
                    return false;
                }
                VectorComposer vas_rot( subbag->value() );

                return vas_vel.getResult( t.vel ) && vas_rot.getResult( t.rot );
            } else {
                Logger::log() << Logger::Error << "Aborting composition of Property< Twist > "
                              << ": Expected type 'MotCon::Twist', got type '"<< bag.getType() <<"'."
                              <<Logger::endl;
            }
        return false;
    }

    void decomposeProperty(const Wrench &b, PropertyBag& targetbag)
    {
        // construct a property with same name and description, but containing a typed PropertyBag.
        targetbag.setType("MotCon::Wrench"); // bag_type

        VectorDecomposer force( b.force );
        VectorDecomposer torque( b.torque );

        targetbag.add( new Property<PropertyBag>("Force", "Axial Force", force.result() ) );
        targetbag.add( new Property<PropertyBag>("Torque", "Axial Torque", torque.result() ) );
    }

    bool composeProperty(const PropertyBag& bag,Wrench &w)
    {
        if ( bag.getType() == std::string("MotCon::Wrench") )
            {
                // pass this bag to the vector Composers
                Property<PropertyBag>* subbag = bag.getProperty<PropertyBag>("Force");
                if (! subbag ) {
                    Logger::log() << Logger::Error << "Aborting composition of Property< Wrench > "
                                  << ": Force not found."
                                  <<Logger::endl;
                    return false;
                }
                VectorComposer vas_force( subbag->value() );

                subbag = bag.getProperty<PropertyBag>("Torque");
                if (! subbag ) {
                    Logger::log() << Logger::Error << "Aborting composition of Property< Wrench > "
                                  << ": Torque not found."
                                  <<Logger::endl;
                    return false;
                }
                VectorComposer vas_torque( subbag->value() );

                return vas_force.getResult( w.force ) && vas_torque.getResult( w.torque );
            } else {
                Logger::log() << Logger::Error << "Aborting composition of Property< Wrench > "
                              << ": Expected type 'MotCon::Wrench', got type '"<< bag.getType() <<"'."
                              <<Logger::endl;
                return false;
            }
        return false;
    }

    void decomposeProperty(const Frame &f, PropertyBag& targetbag )
    {
        // construct a typed PropertyBag.
        targetbag.setType("MotCon::Frame");

        VectorDecomposer vel( f.p );
#ifdef OROSEM_GEOMETRY_ROTATION_PROPERTIES_EULER
        EulerZYXDecomposer rot( f.M );
#else
# ifdef OROSEM_GEOMETRY_ROTATION_PROPERTIES_RPY
        RPYDecomposer rot( f.M );
# else
        RotationDecomposer rot( f.M );
# endif
#endif

        targetbag.add( new Property<PropertyBag>("Position","", vel.result() ) );
        targetbag.add( new Property<PropertyBag>("Rotation","", rot.result() ) );
    }

    bool composeProperty(const PropertyBag& f_bag, Frame &f)
    {
        if ( f_bag.getType() == std::string("MotCon::Frame") )
            {
                // pass this bag to the vector Composers
                Property<PropertyBag>* subbag = f_bag.getProperty<PropertyBag>("Position");
                if (! subbag ) {
                    Logger::log() << Logger::Error << "Aborting composition of Property< Frame > "
                                  << ": Position not found."
                                  <<Logger::endl;
                    return false;
                }
                VectorComposer vas_pos( subbag->value() );

                subbag = f_bag.getProperty<PropertyBag>("Rotation");
                if (! subbag ) {
                    Logger::log() << Logger::Error << "Aborting composition of Property< Frame > "
                                  << ": Rotation not found."
                                  <<Logger::endl;
                    return false;
                }
                RPYComposer vas_rpy( subbag->value() );
                EulerZYXComposer vas_eul( subbag->value() );
                RotationComposer vas_rot( subbag->value() );
                bool result = vas_pos.getResult( f.p );
                if (!result )
                    {
                        Logger::log() << Logger::Error << "Aborting composition of Property< Frame > "
                                      << ": element 'Position' has wrong format." <<Logger::endl;
                        return false;
                    }
                result = vas_rpy.getResult( f.M) ||
                    vas_eul.getResult( f.M ) ||
                    vas_rot.getResult( f.M );
                if (!result )
                    {
                        Logger::log()
                            << Logger::Error << "Aborting composition of Property< Frame > "
                            << ": Could not compose 'Rotation' type 'MotCon::Rotation','MotCon::EulerZYX' or 'MotCon::RPY', got type '"
                            << subbag->get().getType() <<"'."<<Logger::endl;
                        return false;
                    }
                // OK: exit.
                return true;
            } else {
                Logger::log() << Logger::Error << "Aborting composition of Property< Frame > "
                              << ": Expected type 'MotCon::Frame', got type '"<< f_bag.getType() <<"'."
                              <<Logger::endl;
                return false;
            }
        return false;
    }
} // MotionControl
