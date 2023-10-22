#ifndef parser_h
#define PARSER_H
#define OPTLIST     "ani:o:prv"

#include <getopt.h>
#include <stdbool.h>

typedef struct {
    char *regex;
} globs;

typedef struct {
    bool a_flag;
    bool r_flag;
    bool n_flag;
    bool p_flag;
    bool v_flag;
    bool i_flag;
    bool o_flag;
    int num_i_flags;
    int num_o_flags;
    globs *i_flag_obj;
    globs *o_flag_obj;
} Flags;

void parseArguments(int argc, char *argv[]);

#endif // PARSER_H
