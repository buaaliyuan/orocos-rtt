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
 
 
#ifndef GENERIC_TASK_H
#define GENERIC_TASK_H

#include <cppunit/extensions/HelperMacros.h>

#include <TaskContext.hpp>
#include <string>

using namespace RTT;
using namespace RTT;

class Generic_TaskTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( Generic_TaskTest );

    CPPUNIT_TEST( testMethod );
    CPPUNIT_TEST( testMethodFromDS );
    CPPUNIT_TEST( testMethodFactory );
    CPPUNIT_TEST( testDSMethod );
    CPPUNIT_TEST( testCRMethod );
    CPPUNIT_TEST( testMethodsC );
    CPPUNIT_TEST( testAddMethod );
    
    CPPUNIT_TEST_SUITE_END();

    TaskContext* tc;
    ActivityInterface* tsim;
    TaskObject* createMethodFactory();

    // ref/const-ref tests:
    double ret;
    double& m0r() { return ret; }
    const double& m0cr() { return ret; }

    // test const std::string& argument for command_ds
    bool comstr(const std::string& cs) { return !cs.empty(); }

    double m1r(double& a) { a = 2*a; return a; }
    double m1cr(const double& a) { return a; }

    // plain argument tests:
    double m0() { return -d0(); }
    double m1(int i) { return -d1(i); }
    double m2(int i, double d) { return -d2(i,d); }
    double m3(int i, double d, bool c) { return -d3(i,d,c); }
    double m4(int i, double d, bool c, std::string s) { return -d4(i,d,c,s); }

    double d0() const { return 1.0; }
    double d1(int i) const { return 2.0; }
    double d2(int i, double d) const { return 3.0; }
    double d3(int i, double d, bool c) const { return 4.0; }
    double d4(int i, double d, bool c, std::string s) const { return 5.0; }

    bool assertBool( bool );
public:

    void setUp();
    void tearDown();

    void testMethodsC();
    void testAddMethod();
    void testCRMethod();
    void testMethod();
    void testMethodFromDS();
    void testMethodFactory();
    void testDSMethod();
};

#endif
