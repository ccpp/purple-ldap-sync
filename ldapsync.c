#define PURPLE_PLUGINS

#include <glib.h>

#include "notify.h"
#include "plugin.h"
#include "version.h"
#include "ldap.h"

static gboolean
plugin_load(PurplePlugin *plugin) {
    return TRUE;
}

static PurplePluginInfo info = {
    PURPLE_PLUGIN_MAGIC,
    PURPLE_MAJOR_VERSION,
    PURPLE_MINOR_VERSION,
    PURPLE_PLUGIN_STANDARD,
    NULL,
    0,
    NULL,
    PURPLE_PRIORITY_DEFAULT,

    "ldap-sync",
    "LDAP synchronization",
    "1.0",

    "Synchronizes persons by a LDAP connection",          
    "",          
    "Christian Plattner <ccpp@gmx.at>",                          
    "",     
    
    plugin_load,                   
    NULL,                          
    NULL,                          
                                   
    NULL,                          
    NULL,                          
    NULL,                        
    NULL,                   
    NULL,                          
    NULL,                          
    NULL,                          
    NULL                           
};                               
    
static void                        
init_plugin(PurplePlugin *plugin)
{                                  
}

PURPLE_INIT_PLUGIN(hello_world, init_plugin, info)
