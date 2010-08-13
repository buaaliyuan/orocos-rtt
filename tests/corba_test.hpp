#ifndef CORBA_TEST_H
#define CORBA_TEST_H

#include <corba/corba.h>
#include <TaskContext.hpp>
#include <DataPort.hpp>
#include <BufferPort.hpp>
#include <corba/ControlTaskServer.hpp>
#include <corba/ControlTaskProxy.hpp>
#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

using namespace RTT;

class CorbaTest
{
public:
    CorbaTest() { this->setUp(); }
    ~CorbaTest() { this->tearDown(); }

    TaskContext* tc;
    TaskContext* t2;
    TaskContext* tp;
    Corba::ControlTaskServer* ts;
    TaskContext* tp2;
    Corba::ControlTaskServer* ts2;
    TaskObject* createMethodFactory();

    // Ports
    DataPort<double>* md1;
    DataPort<double>* md1bis;
    DataPort<double>* md2;
    ReadDataPort<double>* mdr1;
    ReadDataPort<double>* mdr2;
    WriteDataPort<double>* mdw1;
    WriteDataPort<double>* mdw2;
    BufferPort<double>* mb1;
    BufferPort<double>* mb2;

    // ref/const-ref tests:
    double ret;
    double& m0r() { return ret; }
    const double& m0cr() { return ret; }

    // test const std::string& argument for command_ds
    bool comstr(const std::string& cs) { return !cs.empty(); }

    double m1r(double& a) { a = 2*a; return a; }
    double m1cr(const double& a) { return a; }

    // plain argument tests:
    double m0() { return -1.0; }
    double m1(int i) { return -2.0; }
    double m2(int i, double d) { return -3.0; }
    double m3(int i, double d, bool c) { return -4.0; }
    double m4(int i, double d, bool c, std::string s) { return -5.0; }
    bool mcs(int i, const std::string& s) { BOOST_CHECK( !s.empty() ); BOOST_CHECK( i ); return true; }

    // void(void) function test:
    void vm0(void) { ; }

    bool assertBool( bool );

    void setupCorba();
    void cleanupCorba();

    void setUp();
    void tearDown();

    // helper test functions
    void testPortStats();
    void testPortDisconnect();
};

#endif
