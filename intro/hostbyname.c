#include "unp.h"

int main (int argc, char**argv) 
{
	char *ptr, **pptr;
	char str[INET_ADDRSTRLEN];
	struct hostent *hptr;

	// This loop is called for each command-line argument
	while (--argc > 0) {
		ptr = *++argv;
		if ((hptr = gethostbyname(ptr)) == NULL) {
			err_msg("gethostbyname error for host: %s: %s", ptr, hstrerror(h_errno));
			continue;
		}
		// Prints the official hostname
		printf("official hostname: %s\n", hptr->h_name);

		// List of aliases is printed for the official hostname
		for (pptr = hptr->h_aliases; *pptr != NULL; pptr++) {
			printf("\talias: %s\n", *pptr);
		}

		// Pointer to a list of pointers
		pptr = hptr->h_addr_list;

		// For each address, inet_ntop is called and the string is printed
		for ( ; *pptr != NULL; pptr++) {
			printf("\taddress: %s\n", Inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
		}
	}

	exit(0);
}
