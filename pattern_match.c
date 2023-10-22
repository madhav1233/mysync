#include <stdio.h>
#include <stdbool.h>
#include <regex.h>
#include "pattern_match.h"

bool match_regex(const char *regex, const char *filename){
	regex_t compiled_regex;
	int result;

	result = regcomp(&compiled_regex, regex, REG_NOSUB | REG_EXTENDED);
	if(result != 0){
		char error_message[256];
		regerror(result, &compiled_regex, error_message, sizeof(error_message));
		fprintf(stderr, "Regex compilation failed: %s\n", error_message);
		return false;
	}

	// Compare the filename with the compiled regex using regexec
	result = regexec(&compiled_regex, filename, 0, NULL, 0);

	// Cleanup: Free the memory associated with the compiled regex
	regfree(&compiled_regex);

	// Return true if match, otherwise false
    return (result ==0);
}
