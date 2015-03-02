#define PURPLE_PLUGINS

#include <glib.h>

#include <libpurple/notify.h>
#include <libpurple/plugin.h>
#include <libpurple/version.h>
#include <libpurple/debug.h>
#include <libpurple/blist.h>

#include <ldap.h>

static void init_plugin(PurplePlugin *plugin);
static gboolean plugin_load(PurplePlugin *plugin);
static GList *plugin_actions(PurplePlugin *plugin, gpointer context);
static void notify_configure_ldap_cb(PurplePluginAction *action);
static void notify_scan_buddy_list_cb(PurplePluginAction *action);
static void foreach_buddy_cb(gpointer data, gpointer user_data);

static PurplePlugin *_plugin;
#define PLUGIN_ID "core-ccpp-ldapsync"

static PurplePluginInfo info = {
    PURPLE_PLUGIN_MAGIC,
    PURPLE_MAJOR_VERSION,
    PURPLE_MINOR_VERSION,
    PURPLE_PLUGIN_STANDARD,
    NULL,
    0,
    NULL,
    PURPLE_PRIORITY_DEFAULT,

    PLUGIN_ID,
    "LDAP synchronization",
    "1.0",

    "Synchronizes persons by a LDAP connection",          
    "",          
    "Christian Plattner <ccpp@gmx.at>",                          
    "",     
    
    plugin_load,                   
    NULL,                          
    NULL,                          
                                   
    (PurplePluginUiInfo*) NULL,
    (PurplePluginLoaderInfo*) NULL,
    (PurplePluginUiInfo*) NULL,
    plugin_actions,
    NULL,                          
    NULL,                          
    NULL,                          
    NULL                           
};                               
PURPLE_INIT_PLUGIN(ldapsync, init_plugin, info)
    

static gboolean
plugin_load(PurplePlugin *plugin) {

	_plugin = plugin;
	return TRUE;
}

static void                        
init_plugin(PurplePlugin *plugin)
{                                  
}

static GList *
plugin_actions(PurplePlugin *plugin, gpointer context)
{
	GList *actions = NULL;

	actions = g_list_prepend(actions,
			purple_plugin_action_new("Configure LDAP", notify_configure_ldap_cb));

	actions = g_list_prepend(actions,
			purple_plugin_action_new("Scan Buddy List", notify_scan_buddy_list_cb));

	return actions;
}

static void
notify_configure_ldap_cb(PurplePluginAction *action)
{
	purple_notify_message(_plugin, PURPLE_NOTIFY_MSG_INFO, "LDAP sync",
	                 "TODO configure LDAP sync", NULL, NULL, NULL);
}

static void
notify_scan_buddy_list_cb(PurplePluginAction *action)
{
	/* This returns nothing. Scan groups?
	PurpleBlistNode *root = purple_blist_get_root();
	PurpleBlistNode *node = purple_blist_node_get_first_child(root);
	int i = 0;
	while(node) {
		const char *email = purple_blist_node_get_string(node, "email");
		printf("Buddy email: %s\n", email);
		
		// node = purple_blist_node_next(node, FALSE);
		node = purple_blist_node_get_sibling_next(node);
		i++;
	}
	printf("%d buddys found (purple_blist_get_root)\n", 0);
	*/

	GSList *blist = purple_blist_get_buddies();
	printf("%d buddys found (purple_blist_get_buddies)\n", g_slist_length(blist));
	g_slist_foreach(blist, foreach_buddy_cb, NULL);
	g_slist_free(blist);
}

static void
foreach_buddy_cb(gpointer data, gpointer user_data)
{
	PurpleBuddy *buddy = data;
	PurpleContact *contact = purple_buddy_get_contact(buddy);
	const char *buddy_name = purple_buddy_get_name(buddy);

	purple_debug_info(PLUGIN_ID,
		"Buddy <%s> (alias: %s)\n", buddy_name, purple_buddy_get_alias(buddy));

	if (contact) {
	}
}
