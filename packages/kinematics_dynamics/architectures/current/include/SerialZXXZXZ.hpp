/***************************************************************************
  tag: Peter Soetens  Thu Oct 10 16:21:19 CEST 2002  SerialZXXZXZ.hpp 

                        SerialZXXZXZ.hpp -  description
                           -------------------
    begin                : Thu October 10 2002
    copyright            : (C) 2002 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be
 
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
 


#ifndef SerialZXXZXZ_HPP
#define SerialZXXZXZ_HPP

#include "Serial321.hpp"

#include <math.h>    /* cos,sin,fabs */


namespace ORO_KinDyn
{
    /**
     * Solves the kinematics for a type ZXXZXZ robot.
     */
    class SerialZXXZXZ : public Serial321
    {

        public:
            virtual bool jacobianForward( const ORO_CoreLib::Double6D& q, double J[ 6 ][ 6 ], Singularity& s ) const;

            virtual bool jacobianInverse( const ORO_CoreLib::Double6D& q, double J[ 6 ][ 6 ], Singularity& s ) const;

            virtual bool positionForward( const ORO_CoreLib::Double6D& q, ORO_Geometry::Frame& mp_base, Singularity& s ) const;

            virtual bool positionInverse( const ORO_Geometry::Frame& mp_base, Configuration c,  ORO_CoreLib::Double6D& q, Singularity& s) const;

            virtual bool velocityInverse( const ORO_Geometry::Frame& pos_base, const ORO_Geometry::Twist& vel_base, Configuration c, ORO_CoreLib::Double6D& q, ORO_CoreLib::Double6D& qdot, Singularity& s ) const;

            virtual bool velocityInverse( const ORO_CoreLib::Double6D& q, const ORO_Geometry::Twist& vel_base, ORO_CoreLib::Double6D& qdot, Singularity& s ) const;

            virtual bool velocityForward( const ORO_CoreLib::Double6D& q, const ORO_CoreLib::Double6D& qdot, ORO_Geometry::Frame& pos_base, ORO_Geometry::Twist& vel_base, Singularity& s ) const;

            virtual void configurationGet(const ORO_CoreLib::Double6D& q, Configuration& c) const;
    };

   

}

#endif
