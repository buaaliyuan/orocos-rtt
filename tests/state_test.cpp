/***************************************************************************
  tag: Peter Soetens  Mon Jan 10 15:59:51 CET 2005  state_test.cpp 

                        state_test.cpp -  description
                           -------------------
    begin                : Mon January 10 2005
    copyright            : (C) 2005 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be
 
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
 

#include <rtt-config.h>
#include "state_test.hpp"
#include <unistd.h>
#include <iostream>
#include <sstream>
#ifndef NOPARSER
#include <scripting/ParsedStateMachine.hpp>
#include <scripting/DumpObject.hpp>
#endif
#include <SimulationThread.hpp>
#include <CommandFunctor.hpp>
#include <Method.hpp>
#include <Command.hpp>
#include <StateMachine.hpp>

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( StateTest );


StateTest::StateTest()
    :gtc("root"),
     gtask( 0.01, gtc.engine() )
{}


void 
StateTest::setUp()
{
    d_event = Event<void(double)>("d_event");
    b_event = Event<void(bool)>("b_event");


    // ltc has a test object
    gtc.addObject(this->createObject("test") );

    gtc.events()->addEvent( &d_event, "D", "a1", "arg1 D" );
    gtc.events()->addEvent( &b_event, "B", "a1", "arg1 B" );
    i = 0;
}


void 
StateTest::tearDown()
{
    // if a test failed, we must still stop :
    SimulationThread::Instance()->stop();
    gtask.stop();
}


bool StateTest::assertBool( bool b) { 
    return b;
}
bool StateTest::assertMsg( bool b, const std::string& msg) {
    if ( b == false )
        cout << "Asserted :" << msg << endl;
    return true; // allow to continue to check other commands.
}

int StateTest::increase() {
    return ++i;
}

void StateTest::reset() {
    i = 0;
}


TaskObject* StateTest::createObject(string a)
{
    TaskObject* dat = new TaskObject(a);

    dat->methods()->addMethod( method( "assert", &StateTest::assertBool, this),
                              "Assert", "bool", "" );
    dat->methods()->addMethod( method( "increase", &StateTest::increase, this),
                                "Return increasing i"  );
    dat->methods()->addMethod( method( "reset", &StateTest::reset, this),
                              "Reset i" );
    dat->methods()->addMethod( method( "assertMsg", &StateTest::assertMsg, this),
                                 "Assert message", "bool", "", "text", "text"  );
    dat->methods()->addMethod( method( "isTrue", &StateTest::assertBool, this),
                              "Identity function", "bool", "" );
    dat->methods()->addMethod( method( "i", &StateTest::getI, this),
                         "Return the current number"  );
    dat->commands()->addCommand( command( "instantDone", &StateTest::true_genCom,
                                      &StateTest::true_gen, this, gtc.engine()->commands()),
                                      "returns immediately" );
    dat->commands()->addCommand( command( "neverDone", &StateTest::true_genCom,
                                    &StateTest::false_gen, this, gtc.engine()->commands()),
                                    "returns never" );
    dat->commands()->addCommand( command( "instantNotDone", &StateTest::true_genCom,
                                         &StateTest::true_gen, this, gtc.engine()->commands(), false),
                                         "returns never");
    dat->commands()->addCommand( command( "instantFail", &StateTest::false_genCom,
                                      &StateTest::true_gen, this, gtc.engine()->commands()),
                                      "fails immediately" );
    dat->commands()->addCommand( command( "totalFail", &StateTest::false_genCom,
                                    &StateTest::false_gen, this, gtc.engine()->commands()),
                                    "fails in command and condition" );
    return dat;
}


void StateTest::testParseState()
{
    // a state which should never fail
    string prog = string("StateMachine X {\n")
        + " param int isten\n"
        + " param bool isflse\n"
        + " param bool isok\n"
        + " param double isnegative\n"
        + " var bool istrrue = true\n"
        + " var double d_dummy = -1.0\n"
        + " var int    i_dummy = -1\n"
        + " var bool   varinit = false\n"
        + " var bool   b_dummy = true\n"
        + " initial state INIT {\n"
        // XXX bug : preconditions are not checked in the initial state.
//         + " preconditions {\n"
//         + "     if (istrrue == false) || (isflse == true) || (isten != 10) ||( isnegative >= 0. )  then select PRE_PARAMFAIL\n"
//         + "     if false then select PRE_ERROR\n"
//         + "     if (isnegative != -1.) then select PRE_PARAMFAIL\n"
//         + "     if (istrrue != true) then select PRE_PARAMFAIL\n"
//         + "     if (isflse != false) then select PRE_PARAMFAIL\n"
//         + "     if (isten != 10 ) then select PRE_PARAMFAIL\n"
//         + "     if (d_dummy != -1.) || (i_dummy != -1) then select PRE_VARFAIL\n"
//         + " }\n"
        + " entry {\n"
        + "     set varinit = (d_dummy != -1.) || (i_dummy != -1) \n"
        + "     do test.instantDone()\n"
        + "     set d_dummy = 1.234\n"
        + "     set i_dummy = -2\n"
        + "     do test.instantDone()\n"
        + " }\n"
        + " handle {\n"
        + "     do test.instantDone()\n"
        + " }\n"
        + " exit {\n"
        + "     do test.instantDone()\n"
        + "     set d_dummy = 0.0\n"
        + "     set i_dummy = 0\n"
        + " }\n"
        + " transitions {\n"
        + "     if false then select ERROR\n"
        + "     if varinit == true then select PRE_VARFAIL\n"
        + "     if (d_dummy != 1.234) || (i_dummy != -2)  then select ENTRYFAIL\n"
        + "     if (istrrue == false) || (isflse == true) || (isten != 10) ||( isnegative >= 0. )  then select PARAMFAIL\n"
        + "     if isok == false then select PARAMFAIL\n"
        + "     select FINI\n"
        + "     select ERROR\n" // do not reach
        + " }\n"
        + " }\n"
        + " state PRE_ERROR { entry { do test.assert(false) }\n"
        + " }\n"
        + " state PRE_PARAMFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " state PRE_VARFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " state ERROR { entry { do test.assert(false) }\n"
        + " }\n"
        + " state PARAMFAIL {\n"
        + "      entry { \n"
        + "      do test.assertMsg( isten == 10, \"isten parameter not correctly initialised\")\n"
        + "      do test.assertMsg( istrrue == true, \"istrrue parameter not correctly initialised\")\n"
        + "      do test.assertMsg( isok == true, \"isok parameter not correctly initialised\")\n"
        + "      do test.assertMsg( isflse == false, \"isflse parameter not correctly initialised\")\n"
        + "      do test.assertMsg( true == true, \"true ident not correctly done\")\n"
        + "      do test.assertMsg( false == false, \"false ident not correctly done\")\n"
        + "      do test.assertMsg( isnegative == -1.0, \"isnegative parameter not correctly initialised\")\n"
        + "      }\n"
        + " }\n"
        + " state VARFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " state EXITFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " state ENTRYFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " final state FINI {\n"
        + " preconditions {\n"
        + "     if (d_dummy != 0.) || (i_dummy != 0)  then select EXITFAIL\n"
        + "     if false then select ERROR\n"
        + " }\n"
        + " entry {\n"
        + "     do test.instantDone()\n"
        + "     set d_dummy = -1.\n"
        + "     set i_dummy = -1\n"
        + " }\n"
        + " handle {\n"
        + "     do test.instantDone()\n"
        + " }\n"
        + " exit {\n"
        + "     do test.instantDone()\n"
        + " }\n"
        + " transitions {\n"
        + "     if false then select ERROR\n"
        + "     select INIT\n"
        + "     select ERROR\n" // do not reach
        + " }\n"
        + " }\n"
        + " }\n"
        //        + " RootMachine X x( isten = 10, isflse = false, isnegative = -1.0) \n" // instantiate a non hierarchical SC
        + " RootMachine X x( isten = 10, isok = true, isflse=false, isnegative = -1.0) \n" // instantiate a non hierarchical SC
        ;

    this->doState( prog, &gtc );
    this->finishState( &gtc, "x");
}

void StateTest::testStateFailure()
{
    // test _command_ (through methods though) failure detection on several places.
    // it is an incomplete test, even more that parsing should fail on the second
    // run since the type 'X' is defined twice.
    string prog = string("StateMachine X {\n")
        + " initial state INIT {\n"
        + " entry {\n"
        + "     do test.increase()\n"                // set i to i+1
        + "     do test.assert( test.i() != 1)\n" // fail if i == 1
        + " }\n"
        + " handle {\n"
        + "     do test.assert( test.i != 2)\n" 
        + " }\n"
        + " exit {\n"
        + "     do test.assert( test.i != 3)\n" 
        + " }\n"
        + " transitions {\n"
        + "     select FINI\n"
        + " }\n"
        + " }\n"
        + " state ERROR { entry { do test.assert(false) }\n"
        + " }\n"
        + " final state FINI {\n"
        + " entry {\n"
        + "     do test.assert( test.i != 4)\n" 
        + " }\n"
        + " handle {\n"
        + "     do test.assert( test.i != 5)\n" 
        + " }\n"
        + " exit {\n"
        + "     do test.assert( test.i != 6)\n" 
        + " }\n"
        + " transitions {\n"
        + "     select INIT\n"
        + " }\n"
        + " }\n"
        + " }\n"
        + " RootMachine X x\n" // instantiate a non hierarchical SC
        ;

    // should fail each time
    while ( i < 6 ) {
        this->doState( prog, &gtc, false );

        // assert that an error happened :
        CPPUNIT_ASSERT( gtc.engine()->states()->getStateMachineStatus("x") == StateMachine::Status::error );
        
        this->finishState( &gtc, "x");
    }
}
void StateTest::testStateChildren()
{
    // instantiate two children and check init of vars and params
    string prog = string("StateMachine Y {\n")
        + " param double isnegative\n"
        + " var   double t = 1.0\n"
        + " initial state INIT {\n"
        + " entry {\n"
        + "     do test.instantDone()\n"
        + " }\n"
        + " transitions {\n"
        + "     if isnegative >= 0. then select PARAMFAIL\n"
        + "     select FINI\n"
        + " }\n"
        + " }\n"
        + " state ERROR { entry { do test.assert(false) }\n"
        + " }\n"
        + " state PARAMFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " state VARFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " state EXITFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " state ENTRYFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " final state FINI {\n"
        + " entry {\n"
        + "     do test.instantDone()\n"
        + " }\n"
        + " transitions {\n"
        + "     select INIT\n"
        + " }\n"
        + " }\n"
        + " }\n"
        + string("StateMachine Z {\n")
        + " param double neg\n"
        + " initial state INIT {\n"
        + " transitions {\n"
        + "     if neg >= 0. then select PARAMFAIL\n"
        + "     select FINI\n"
        + " }\n"
        + " }\n"
        + " state PARAMFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " final state FINI {\n"
        + " transitions {\n"
        + "     select INIT\n"
        + " }\n"
        + " }\n"
        + " }\n"
        + string("StateMachine X {\n")
        + " param double isnegative\n"
        + " var double d_dummy = -2.0\n"
        + " var int    i_dummy = -1\n"
        + " SubMachine Y y1(isnegative = d_dummy)\n"
        + " SubMachine Y y2(isnegative = -3.0)\n"
        + " SubMachine Y y3(isnegative = isnegative)\n"
        + " SubMachine Z z1( neg = d_dummy)\n"
        + " initial state INIT {\n"
        + " entry {\n"
        + "     do test.instantDone()\n"
        + "     do y1.activate()\n"
        + "     do y2.activate()\n"
        + "     do y3.activate()\n"
        + "     do z1.activate()\n"
        + " }\n"
        + " exit {\n"
        + "     do y1.start()\n"
        + "     do y2.start()\n"
        + "     do y3.start()\n"
        + "     do z1.start()\n"
        + " }\n"
        + " transitions {\n"
        + "     select FINI\n"
        + " }\n"
        + " }\n"
        + " state ERROR { entry { do test.assert(false) }\n"
        + " }\n"
        + " state PARAMFAIL {\n"
        + "      entry { \n"
        + "      do test.assertMsg( y3.isnegative == isnegative, \"y3 parameter not correctly initialised\")\n"
        + "      do test.assertMsg( y2.isnegative == -3.0, \"y2 parameter not correctly initialised\")\n"
        + "      do test.assertMsg( y1.isnegative == d_dummy, \"y1 parameter not correctly initialised\")\n"
        + "      do test.assertMsg( z1.neg == d_dummy, \"z1 parameter not correctly initialised\")\n"
        + "      }\n"
        + " }\n"
        + " state VARFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " state EXITFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " state ENTRYFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " final state FINI {\n"
        + " entry {\n"
        + "     do test.instantDone()\n"
        + "     do y1.stop()\n"
        + "     do y2.stop()\n"
        + "     do y3.stop()\n"
        + "     do z1.stop()\n"
        + " }\n"
        + " exit {\n"
        + "     do test.instantDone()\n"
        + "     do y1.deactivate()\n"
        + "     do y2.deactivate()\n"
        + "     do y3.deactivate()\n"
        + "     do z1.deactivate()\n"
        + " }\n"
        + " transitions {\n"
        + "     if z1.neg != d_dummy then select PARAMFAIL\n"
        + "     if y1.isnegative != d_dummy then select PARAMFAIL\n"
        + "     if y2.isnegative != -3.0 then select PARAMFAIL\n"
        + "     if y3.isnegative != isnegative then select PARAMFAIL\n"
        + "     select INIT\n"
        + " }\n"
        + " }\n"
        + " }\n"
        + " RootMachine X x( isnegative = -1.0) \n" // instantiate a hierarchical SC
        ;
    this->doState( prog, &gtc );
    this->finishState( &gtc, "x");
}

void StateTest::testStateEmpty()
{
    // test processing of completely empty states
    string prog = string("StateMachine X {\n")
        + " initial state INIT {\n"
        + " transitions {\n"
        + "     select TEST\n" // only a transition
        + " }\n"
        + " }\n"
        + " state TEST {\n"  // not even used
        + " }\n"
        + " final state FINI {\n" // completely empty
        + " }\n"
        + " }\n"
        + " RootMachine X x\n" // instantiate a non hierarchical SC
        ;
     this->doState( prog, &gtc );
     this->finishState( &gtc, "x");
}

void StateTest::testStateSubStateVars()
{
    // test get/set access of substate variables and parameters
    string prog = string("StateMachine Y {\n")
        + " param double isnegative\n"
        + " var   double t = 1.0\n"
        + " initial state INIT {\n"
        + " transitions {\n"
        + "     if isnegative >= 0. then select PARAMFAIL\n"
        + "     if t >= 0. then select VARFAIL\n"
        + "     select FINI\n"
        + " }\n"
        + " exit { set isnegative = +1.0 }\n"
        + " }\n" // 11
        + " state ERROR { entry { do test.assert(false) }\n"
        + " }\n"
        + " state PARAMFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " state VARFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " state EXITFAIL { entry { do test.assert(false) }\n"
        + " }\n"
        + " state ENTRYFAIL { entry { do test.assert(false) }\n"
        + " }\n" // 21
        + " final state FINI {\n"
        + " transitions {\n"
        + "     if isnegative <= 0. then select PARAMFAIL\n"
        + " }\n"
        + " }\n"
        + " }\n"
        + string("StateMachine X {\n")
        + " param double isnegative\n"
        + " var double d_dummy = -2.0\n"
        + " var int    i_dummy = -1\n" //31
        + " SubMachine Y y1(isnegative = d_dummy)\n"
        + " initial state INIT {\n"
        + " entry {\n"
        + "     do y1.activate()\n"
        + "     set y1.t = -1.0 \n"
        + " }\n"
        + " exit {\n"
        + "     do y1.start()\n"
        + " }\n"
        + " transitions {\n"
        + "     select FINI\n"
        + " }\n"
        + " }\n"
        + " final state FINI {\n"
        + " entry {\n"
        + "     do y1.stop()\n"
        + " }\n"
        + " exit {\n"
        + "     set y1.isnegative = -1.0 \n"
        + "     do y1.deactivate()\n"
        + " }\n"
        + " transitions {\n"
        + "     select INIT\n"
        + " }\n"
        + " }\n"
        + " }\n"
        + " RootMachine X x( isnegative = -1.0) \n" // instantiate a hierarchical SC
        ;

     this->doState( prog, &gtc );
     this->finishState( &gtc, "x");
}

void StateTest::testStateSubStateCommands()
{
    // test get/set access of substate variables and parameters
    string prog = string("StateMachine Y {\n")
        + " param double isnegative\n"
        + " var   double t = 1.0\n"
        + " initial state INIT {\n"
        + " transitions {\n"
        + "     if isnegative < 0. then select ISNEGATIVE\n"
        + "     if t >= 0. then select ISPOSITIVE\n"
        + "     select DEFAULT\n"
        + " }\n"
        + " }\n"
        + " state ISNEGATIVE {\n"
        + " transitions {\n"
        + "      select INIT\n"
        + " }\n"
        + " }\n"
        + " state ISPOSITIVE {\n"
        + " transitions {\n"
        + "      select INIT\n"
        + " }\n"
        // 20 :
        + " }\n"
        + " state DEFAULT {\n"
        + " transitions {\n"
        + "      select FINI\n"
        + " }\n"
        + " }\n"
        + " final state FINI {\n"
        + " }\n"
        + " }\n"
        + string("StateMachine X {\n") // 1
        + " SubMachine Y y1(isnegative = -1.0)\n"
        + " initial state INIT {\n"
        + " entry {\n"
        + "     set y1.t = -1.0 \n"
        + "     do y1.activate()\n"
        + "     do y1.requestState(\"ISNEGATIVE\")\n"
        + "     do test.assert( y1.inState(\"ISNEGATIVE\") )\n"
        + "     do y1.requestState(\"INIT\")\n"
        + "     do test.assert( y1.inState(\"INIT\") )\n"
        + "     set y1.isnegative = +1.0 \n"
        + "     try y1.requestState(\"ISNEGATIVE\") \n "
        + "     catch \n{\n"
        + "         do test.assert( y1.inState(\"INIT\") )\n" // do not leave INIT
        + "     }\n"
        + "     do test.assert( y1.inState(\"INIT\") )\n" // do not leave INIT
        + "     do y1.requestState(\"FINI\")\n"      // request final state
        + "     do test.assert( y1.inState(\"FINI\") )\n"
        + " }\n"
        + " exit {\n"
        + "     do y1.requestState(\"INIT\")\n"      // request initial state
        + "     do test.assert( y1.inState(\"INIT\") )\n"
        + "     set y1.isnegative = +1.0 \n"
        + "     set y1.t = -1.0 \n"
        + "     do y1.start()\n"  // must reach FINI after a while.
        + "     while ! y1.inState(\"FINI\") \n"
        + "        do nothing\n"
        + " }\n"
        + " transitions {\n"
        + "     select FINI\n"
        + " }\n"
        + " }\n"
        + " final state FINI {\n"
        + " entry {\n"
        + "     do y1.stop()\n"
        + " }\n"
        + " exit {\n"
        + "     do y1.deactivate()\n"
        + " }\n"
        + " transitions {\n"
        + "     select INIT\n"
        + " }\n"
        + " }\n"
        + " }\n"
        + " RootMachine X x() \n" // instantiate a hierarchical SC
        ;

     this->doState( prog, &gtc );
     this->finishState( &gtc, "x");
}

void StateTest::testStateEvents()
{
    // test event reception in sub states.
    string prog = string("StateMachine Y {\n")
        + " var   double t = 0.0\n"
        + " var   double et = 0.0\n"
        + " var   bool eb = false\n"
        + " initial state INIT {\n"
        + " transition d_event(et)\n"
        + "     if et < 0. then select ISNEGATIVE\n"
        + "     else select ISPOSITIVE\n"
        + " transition b_event(eb)\n"
        + "     if eb == false then select ISFALSE\n"
        + "     else select ISTRUE\n"
        + " }\n"
        + " state ISNEGATIVE {\n"
        + " transitions {\n"
        + "      select INIT\n"
        + " }\n"
        + " }\n"
        + " state ISPOSITIVE {\n"
        + " transitions {\n"
        + "      select INIT\n" // 20
        + "      }\n"
        + " }\n"
        + " state ISTRUE {\n"
        + " transitions {\n"
        + "      select INIT\n"
        + "      }\n"
        + " }\n"
        + " state ISFALSE {\n"
        + " transitions {\n"
        + "      select INIT\n"
        + " }\n"
        + " }\n"
        + " state DEFAULT {\n"
        + " transitions {\n"
        + "      select FINI\n"
        + "      }\n"
        + " }\n"
        + " final state FINI {\n"
        + " }\n"
        + " }\n" // 40
        + string("StateMachine X {\n") // 1
        + " SubMachine Y y1()\n"
        + " initial state INIT {\n"
        + " run {\n"
        //        + "     do test.assert(false)\n"
        + "     do y1.activate()\n"
        + "     do d_event(-1.0)\n"
        + "     do nothing\n"
        + "     do test.assert( !y1.inState(\"INIT\") )\n"
        + "     do test.assert( !y1.inState(\"ISPOSITIVE\") )\n"
        + "     do test.assert( y1.inState(\"ISNEGATIVE\") )\n"
        + "     do y1.requestState(\"INIT\")\n"
        + "     do test.assert( y1.inState(\"INIT\") )\n"
        + "     do d_event(+1.0)\n"
//         + "     do nothing\n"
        + "     if ( !y1.inState(\"ISPOSITIVE\") ) then\n"
        + "          do test.assertMsg( false, y1.getState() )\n"
        + "     do test.assert( y1.inState(\"ISPOSITIVE\") )\n"
        + "     do y1.requestState(\"INIT\")\n"
        + "     do test.assert( y1.inState(\"INIT\") )\n"
        + "     do b_event(true)\n"
//         + "     do nothing\n"
        + "     if ( !y1.inState(\"ISTRUE\") ) then\n"
        + "          do test.assertMsg(false, y1.getState() )\n"
        + "     do test.assertMsg( y1.inState(\"ISTRUE\"), y1.getState() )\n"
        + "     do y1.requestState(\"INIT\")\n"
        + "     do test.assert( y1.inState(\"INIT\") )\n"
        + "     do b_event(false)\n"
//         + "     do nothing\n"
        + "     if ( !y1.inState(\"ISFALSE\") ) then\n"
        + "          do test.assertMsg(false, y1.getState() )\n"
        + "     do test.assert( y1.inState(\"ISFALSE\") )\n"
        + "     do y1.requestState(\"INIT\")\n"
        + "     do test.assert( y1.inState(\"INIT\") )\n"
        //+ "     do y1.deactivate()\n"
        + " }\n"
        + " transitions {\n"
        + "     select FINI\n"
        + " }\n"
        + " }\n"
        + " final state FINI {\n"
        + " entry {\n"
        + "     do y1.deactivate()\n"
        //+ "     do test.assert(false)\n"
        + " }\n"
        + " transitions {\n"
        + "     select INIT\n"
        + " }\n"
        + " }\n"
        + " }\n"
        + " RootMachine X x() \n" // instantiate a hierarchical SC
        ;

     this->doState( prog, &gtc );
     this->finishState( &gtc, "x");
}

void StateTest::testStateUntil()
{
//     this->doState( prog, &gtc );
//     this->finishState( &gtc, "x");
}

void StateTest::testStateUntilFail()
{
//     this->doState( prog, &gtc, false );

//     CPPUNIT_ASSERT( gprocessor.getStateMachineStatus("x") == Processor::StateMachineStatus::error );

//     this->finishState( &gtc, "x");
}

void StateTest::doState( const std::string& prog, TaskContext* tc, bool test )
{
    CPPUNIT_ASSERT( tc->engine() );
    CPPUNIT_ASSERT( tc->engine()->states());

#if 0
    // Classical way: use parser directly.
    Parser::ParsedStateMachines pg_list;
    try {
        pg_list = parser.parseStateMachine( prog, tc );
    }
    catch( const file_parse_exception& exc )
        {
            CPPUNIT_ASSERT_MESSAGE( exc.what(), false );
        }
    catch( const parse_exception& exc )
        {
            CPPUNIT_ASSERT_MESSAGE( exc.what(), false );
        }
    catch( ... ) {
            CPPUNIT_ASSERT_MESSAGE( "Uncaught Parse Exception", false );
    }
    if ( pg_list.empty() )
        {
            CPPUNIT_ASSERT( false );
        }
#endif
    // Alternative way: test ScriptingAccess as well.
    try {
        tc->scripting()->loadStateMachines( prog, std::string("state_test.cpp"), true );
    } 
    catch( const file_parse_exception& exc )
        {
            CPPUNIT_ASSERT_MESSAGE( exc.what(), false );
        }
    catch( const parse_exception& exc )
        {
            CPPUNIT_ASSERT_MESSAGE( exc.what(), false );
        }
    catch( const program_load_exception& e)
        {
            CPPUNIT_ASSERT_MESSAGE( e.what(), false );
        }
    catch( ... ) {
            CPPUNIT_ASSERT_MESSAGE( "Uncaught Processor load exception", false );
    }
    CPPUNIT_ASSERT( gtask.start() );
    StateMachinePtr sm = tc->engine()->states()->getStateMachine("x");
    CPPUNIT_ASSERT( sm );
    CommandInterface* ca = newCommandFunctor(boost::bind(&StateMachine::activate, sm ));
    CommandInterface* cs = newCommandFunctor(boost::bind(&StateMachine::automatic,sm ));
//      cerr << "Before activate :"<<endl;
//      tc->getPeer("states")->getPeer("x")->debug(true);
    CPPUNIT_ASSERT( ca->execute()  );
    CPPUNIT_ASSERT_MESSAGE( "Error : Activate Command for '"+sm->getName()+"' did not have effect.", sm->isActive() );
//      cerr << "After activate :"<<endl;
//      tc->getPeer("states")->getPeer("x")->debug(true);
    CPPUNIT_ASSERT( gtc.engine()->commands()->process( cs ) != 0 );
//     while (1)
    CPPUNIT_ASSERT( SimulationThread::Instance()->run(1000) );
    delete ca;
    delete cs;
//     cerr << "After run :"<<endl;
//     tc->getPeer("states")->getPeer("x")->debug(true);
// //     tc->getPeer("__states")->getPeer("X")->debug(false);
//     if( tc->getPeer("__states")->getPeer("Y"))
//         tc->getPeer("__states")->getPeer("Y")->debug(false);
//     cerr << "               x.y1 running : "<< (gprocessor.getStateMachineStatus("x.y1") == Processor::StateMachineStatus::running) << endl;
//     cerr << "               x running : "<< (gprocessor.getStateMachineStatus("x") == Processor::StateMachineStatus::running) << endl;
    string sline;
    if (test ) {
        // check error status of parent :
        CPPUNIT_ASSERT_MESSAGE( "Error : State Context '"+sm->getName()+"' did not get activated.", sm->isActive() );
        stringstream errormsg;
        int line = sm->getLineNumber();
        errormsg <<" in StateMachine "+sm->getName()
                 <<" in state "<< sm->currentState()->getName()
                 <<" on line " << line <<" of that StateMachine:"<<endl;
        stringstream sctext( sm->getText() );
        int cnt = 1;
        while ( cnt++ <line && sctext ) {
            string garbage;
            getline( sctext, garbage, '\n' );
        }
        getline( sctext, sline, '\n' );
        errormsg <<"here  > " << sline << endl;
        if ( sm->inError() ) {
            RTT::detail::DumpObject( tc );
        }
        CPPUNIT_ASSERT_MESSAGE( "Runtime error (inError() == true) encountered" + errormsg.str(), sm->inError() == false );
        // check error status of all children:
        StateMachine::ChildList cl = sm->getChildren();
        StateMachine::ChildList::iterator clit = cl.begin();
        while( clit != cl.end() ) {
            stringstream cerrormsg;
            if ( (*clit)->currentState() )
                cerrormsg <<" in state "<<(*clit)->currentState()->getName()<< " on line " <<  (*clit)->getLineNumber() <<" of that StateMachine."<<endl <<"here  > " << sline << endl;
            else
                cerrormsg <<" (deactivated) on line " <<  (*clit)->getLineNumber() <<" of that StateMachine."<<endl<<"here  > " << sline << endl;
            CPPUNIT_ASSERT_MESSAGE( "Runtime error (inError() == true) encountered in child "+(*clit)->getName() + cerrormsg.str(), (*clit)->inError() == false );
            ++clit;
        }
    }
    tc->engine()->states()->getStateMachine( sm->getName() )->stop();
    CPPUNIT_ASSERT( SimulationThread::Instance()->run(500) );
    stringstream errormsg;
    errormsg << " on line " << sm->getLineNumber() <<", status is "<< gtc.engine()->states()->getStateMachineStatusStr("x") <<endl <<"here  > " << sline << endl;;
    CPPUNIT_ASSERT_MESSAGE( "StateMachine stalled " + errormsg.str(), sm->isStopped() );
}

void StateTest::finishState(TaskContext* tc, std::string prog_name)
{
    // you can call deactivate even when the proc is not running.
    // but deactivation may be 'in progress if exit state has commands in it.
    CPPUNIT_ASSERT( tc->engine()->states()->getStateMachine( prog_name )->deactivate() );
    CPPUNIT_ASSERT( SimulationThread::Instance()->run(200) );
    CPPUNIT_ASSERT( tc->engine()->states()->getStateMachine( prog_name )->isActive() == false );

    // only stop now, since deactivate won't work if simtask not running.
    CPPUNIT_ASSERT( gtask.stop() );

    try {
        tc->engine()->states()->unloadStateMachine( prog_name );
    }
    catch( const program_unload_exception& e)
        {
            CPPUNIT_ASSERT_MESSAGE( e.what(), false );
        }
    catch( ... ) {
            CPPUNIT_ASSERT_MESSAGE( "Uncaught Processor unload exception", false );
    }

}

    

