#include <ldap.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	LDAP *ldap;
	int error = ldap_initialize(&ldap, "ldap://localhost:1389/ou=people");
	if (error != LDAP_SUCCESS) {
		fprintf(stderr, "Could not open LDAP connection: %s\n", ldap_err2string(error));
		return 1;
	}
	printf("Connected.\n");
	// ldap_sasl_bind
	//error = ldap_search_ext(ldap);
	//if (error != LDAP_SUCCESS) {
	//	perror("Could not search in LDAP");
	//	return 1;
	//}
	ldap_destroy(ldap);
	return 0;
}
