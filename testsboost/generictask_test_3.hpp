/***************************************************************************
  tag: Peter Soetens  Mon Jun 26 13:26:02 CEST 2006  generictask_test.hpp

                        generictask_test.hpp -  description
                           -------------------
    begin                : Mon June 26 2006
    copyright            : (C) 2006 Peter Soetens
    email                : peter.soetens@fmtc.be

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef GENERIC_TASK_3_H
#define GENERIC_TASK_3_H

#include "TaskContext.hpp"
#include <string>

using namespace RTT;

class StatesTC;

class Generic_TaskTest_3
{
public:
	TaskContext* tc;
    StatesTC* stc;
    ActivityInterface* tsim;
    ActivityInterface* stsim;

public:
	Generic_TaskTest_3(){ setUp();};
	~Generic_TaskTest_3(){ tearDown();};
    void setUp();
    void tearDown();

    void testPeriod();
    void testExecutionEngine();
    void testProperties();
    void testAttributes();
    void testPorts();
    void testConnections();
    void testPortObjects();
    void testTCStates();
    void testSpecialTCStates();
    void testFailingTCStates();

};

#endif
