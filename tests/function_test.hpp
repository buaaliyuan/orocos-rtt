#ifndef FUNCTIONTEST_H
#define FUNCTIONTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include <execution/Parser.hpp>
#include <execution/Processor.hpp>
#include <execution/TaskContext.hpp>
#include <execution/Factories.hpp>
#include <string>
#include <corelib/TaskSimulation.hpp>

using namespace ORO_CoreLib;
using namespace ORO_Execution;

class FunctionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( FunctionTest );
    CPPUNIT_TEST( testSimpleFunction );
    CPPUNIT_TEST( testExportFunction );
    CPPUNIT_TEST( testRecFunction );
    CPPUNIT_TEST( testParseFunction );
    CPPUNIT_TEST( testFunctionStack );
    CPPUNIT_TEST( testFunctionArgs );
    CPPUNIT_TEST( testFunctionFail );
    CPPUNIT_TEST_SUITE_END();

    Parser parser;
    Processor gprocessor;
    TaskContext gtc;
    TaskSimulation gtask;
    MethodFactoryInterface* createMethodFactory();
    CommandFactoryInterface* createCommandFactory();
    DataSourceFactoryInterface* createDataSourceFactory();
    bool assertBool( bool );
    bool assertMsg( bool, const std::string& msg);
    int increase();
    void reset();
    void doFunction( const std::string& prog, TaskContext*, bool test=true );
    void finishFunction( TaskContext* , std::string );

    bool true_genCom() { return true; }
    bool false_genCom() { return false; }
    bool true_gen() const { return true; }
    bool false_gen() const { return false; }

    bool bool_gen( bool b ) const { return b; }
    int i;
public:
    FunctionTest();

    void setUp();
    void tearDown();

    void testSimpleFunction();
    void testExportFunction();
    void testRecFunction();
    void testParseFunction();
    void testFunctionStack();
    void testFunctionArgs();
    void testFunctionFail();

};

#endif
