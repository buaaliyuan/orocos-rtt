#ifndef ORO_SERVICEPLUGIN_HPP_
#define ORO_SERVICEPLUGIN_HPP_

#include <string>
#include "../TaskContext.hpp"

namespace RTT {
    namespace plugin {

    }
}

#define ORO_SERVICEPLUGIN_xstr(s) ORO_SERVICEPLUGIN_str(s)
#define ORO_SERVICEPLUGIN_str(s) #s


/**
 * You can use this macro to make any ServiceProvider available as a
 * plugin.
 * @param SERVICE A class that inherits from ServiceProvider and takes a TaskContext*
 * as argument in a constructor.
 * @param NAME A string being the name of the plugin.
 *
 * @note Do not use this macro inside a namespace !
 * For example: ORO_SERVICE_NAMED_PLUGIN ( KDL::KDLService, "kdl" )
 * where KDL::KDLService is a class name.
 */
#define ORO_SERVICE_NAMED_PLUGIN( SERVICE, NAME ) \
    extern "C" {                      \
        bool loadRTTPlugin(RTT::TaskContext* tc) {    \
            if (tc == 0) return true; \
            RTT::interface::ServiceProvider::shared_ptr sp( new SERVICE( tc ) ); \
            return tc->provides()->addService( sp ); \
        } \
        std::string getRTTPluginName() { \
            return NAME; \
        } \
        std::string getRTTTargetName() { \
            return OROCOS_TARGET_NAME; \
        } \
    }
/**
 * You can use this macro to make any ServiceProvider available as a
 * plugin.
 * @param SERVICE A class that inherits from ServiceProvider and takes a TaskContext*
 * as argument in a constructor. The name of the plugin is equal to SERVICE, but the
 * name of the service (ie ServiceProvider::getName() ) may be different.
 *
 * @note Do not use this macro inside a namespace !
 * For example: ORO_SERVICE_PLUGIN ( KDL::KDLService )
 * where KDL::KDLService is a class name.
 */
#define ORO_SERVICE_PLUGIN( SERVICE ) \
    extern "C" {                      \
        bool loadRTTPlugin(RTT::TaskContext* tc) {    \
            if (tc == 0) return true; \
            RTT::interface::ServiceProvider::shared_ptr sp( new SERVICE( tc ) ); \
            return tc->provides()->addService( sp ); \
        } \
        std::string getRTTPluginName() { \
            return ORO_SERVICEPLUGIN_xstr(SERVICE); \
        } \
        std::string getRTTTargetName() { \
            return OROCOS_TARGET_NAME; \
        } \
    }

#endif /* ORO_SERVICEPLUGIN_HPP_ */
