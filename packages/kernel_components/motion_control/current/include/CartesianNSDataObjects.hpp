/***************************************************************************
  tag: Peter Soetens  Thu Apr 22 20:40:59 CEST 2004  CartesianNSDataObjects.hpp 

                        CartesianNSDataObjects.hpp -  description
                           -------------------
    begin                : Thu April 22 2004
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
 
 
 
#ifndef CARTESIAN_NS_DATAOBJECTS_HPP
#define CARTESIAN_NS_DATAOBJECTS_HPP

#include <geometry/frames.h>
#include <geometry/trajectory.h>
#include <corelib/MultiVector.hpp>

#include <control_kernel/DataServer.hpp>


/**
 * @file CartesianNSDataObjects.hpp
 *
 * This file contains all the DataObjects used by the
 * CartesianNS* components.
 */
    
namespace ORO_ControlKernel
{
    using namespace ORO_Geometry;

    /**
     * @brief The Nameserved Input DataObjects.
     */
    struct CartesianNSSensorInput
        : public ServedTypes<Double6D>
    {
        CartesianNSSensorInput()
        {
            this->insert(make_pair(0,"JointPositions"));
        }
    };

    /**
     * @brief The Nameserved Command DataObjects.
     */
    struct CartesianNSCommand
        : public ServedTypes< Trajectory*, Frame>
    {
        CartesianNSCommand()
        {
            // The numbers 0,1 is the type number as in
            // ServedTypes< Type0, Type1, Type2,... >
            this->insert( make_pair(0, "Trajectory"));
            this->insert( make_pair(1, "TaskFrame"));
            this->insert( make_pair(1, "ToolFrame"));
        }
    };
 
    /**
     * The SetPoint is expressed in the robot base frame.
     */
    struct CartesianNSSetPoint
        : public ServedTypes<Frame>
    {
        CartesianNSSetPoint()
        {
            this->insert( make_pair(0, "EndEffectorFrame"));
        }
    };

    /**
     * The Estimator only estimates the cartesian coordinates.
     */
    struct CartesianNSModel
        : public ServedTypes<Frame>
    {
        CartesianNSModel() {
            this->insert(make_pair(0, "EndEffPosition"));
        }
    };

    /**
     * Calculated joint velocities.
     */
    struct CartesianNSDriveOutput
        : public ServedTypes<Double6D>
    {
        CartesianNSDriveOutput()
        {
            this->insert(make_pair(0, "JointVelocities"));
        }
    };
}

#endif
