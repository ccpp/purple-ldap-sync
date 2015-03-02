#define _GNU_SOURCE	/* asprintf */
#define LDAP_DEPRECATED 1	/* We need authentication without SASL */
#include <ldap.h>
#include <stdio.h>
#include <stdlib.h>

#define SEARCH_FILTER_ALL "(objectclass=*)"
#define SEARCH_FILTER_NAME(name) "(&(objectCategory=person)(cn=" name "))"
#define SEARCH_FILTER_MANAGER(name) "(manager=" name ")"

int main(int argc, char *argv[]);
int urllist_proc(LDAP *ld, LDAPURLDesc **urllist, LDAPURLDesc **url, void *params);
LDAP *ldaptest_connect(char *url, char *user, char *password);
int ldaptest_search(LDAP *ldap, char *base, char *filter);

int main(int argc, char *argv[]) {
	LDAP *ldap;
	char *filter;

	if(argc != 6) {
		fprintf(stderr, "Usage: %s ldap://... user password ou=people name\n", argv[0]);
		return 1;
	}

	ldap = ldaptest_connect(argv[1], argv[2], argv[3]);
	printf("LDAP connection Initialized.\n");

	asprintf(&filter, SEARCH_FILTER_NAME("%s*"), argv[5]);
	ldaptest_search(ldap, argv[4], filter);

	ldap_unbind(ldap);
	// ldap_destroy(ldap);
	return 0;
}

int ldaptest_search(LDAP *ldap, char *base, char *filter) {
	int error, errcode;
	LDAPMessage *result, *entry;
	char *matcheddnp, *errmsgp, **referralsp;
	LDAPControl **serverctrls;

	printf("Searching with base \"%s\" and filter \"%s\"...\n", base, filter);

	error = ldap_search_ext_s(ldap, base, LDAP_SCOPE_ONELEVEL, filter, NULL, 0, NULL, NULL, LDAP_NO_LIMIT, LDAP_NO_LIMIT, &result);
	if (error != LDAP_SUCCESS) {
		printf("Could not search in LDAP: %s\n", ldap_err2string(error));
		return 1;
	}

	printf("Message count: %d\n", ldap_count_messages(ldap, result));

	if (ldap_msgtype(result) != LDAP_RES_SEARCH_ENTRY && ldap_msgtype(result) != LDAP_RES_SEARCH_RESULT) {
		fprintf(stderr, "Message is not a search result message\n");
		fprintf(stderr, "Message type: 0x%x\n", ldap_msgtype(result));
		return 1;
	}

	for(entry = ldap_first_entry(ldap, result); entry; entry = ldap_next_entry(ldap, entry)) {
		char *a, **vals;
		BerElement *ber;
		int i;

		printf("Entry: type: %d\n", ldap_msgtype(entry));

		for ( a = ldap_first_attribute( ldap, entry, &ber );
		       a != NULL; a = ldap_next_attribute( ldap, entry, ber ) ) {
			/* For each attribute, print the attribute name and values. */
			if ((vals = ldap_get_values( ldap, entry, a)) != NULL ) {
				for ( i = 0; vals[i] != NULL; i++ ) {
					printf( "\t%s: %s\n", a, vals[i] );
				}
				ldap_value_free( vals );
		       }
		       ldap_memfree( a );
		}
		if (ber != NULL)
			ber_free(ber, 0);

		error = ldap_parse_result(ldap, result, &errcode, &matcheddnp, &errmsgp, &referralsp, &serverctrls, 0);
		if (error != LDAP_SUCCESS) {
			printf("Could not parse LDAP message: %s\n", ldap_err2string(error));
			return 1;
		}
		if (errcode != LDAP_SUCCESS) {
			printf("LDAP parsing message: %s\n", ldap_err2string(errcode));
			return 1;
		}
		printf("matcheddnp: %s, errmsgp: %s\n", matcheddnp, errmsgp);
		printf("\n");
	}

	ldap_msgfree(result);

	return 0;
}

int urllist_proc(LDAP *ld, LDAPURLDesc **urllist, LDAPURLDesc **url, void *params) {
	LDAPURLDesc *cur_url = *urllist;
	for (cur_url = *urllist; cur_url; cur_url = cur_url->lud_next) {
		printf("URLLIST: %s\n", ldap_url_desc2str(cur_url));
	}
	for (cur_url = *url; cur_url; cur_url = cur_url->lud_next) {
		printf("URL: %s\n", ldap_url_desc2str(*url));
	}
	return 0;
}

LDAP *ldaptest_connect(char *url, char *user, char *password) {
	LDAP *ldap = NULL;
	int error;

	error = ldap_initialize(&ldap, url);
	if (error != LDAP_SUCCESS) {
		fprintf(stderr, "Could not open LDAP connection: %s\n", ldap_err2string(error));
		exit(1);
	}

	ldap_set_urllist_proc(ldap, urllist_proc, NULL);

	error = ldap_simple_bind_s(ldap, user, password);
	if (error != LDAP_SUCCESS) {
		fprintf(stderr, "Could not bind to LDAP directory: %s\n", ldap_err2string(error));
		ldap_destroy(ldap);
		exit(1);
	}

	return ldap;
}
