#include "mysync.h"
#include <unistd.h>
#include <stdio.h>
#include "cmd_parser.h"
#include "utils.h"
#include "glob2regex.h"

extern Flags flags;

#if	defined(__linux__)
extern	char	*strdup(char *string);
#endif

void parseArguments(int argc, char *argv[]) {

    int flag_nums = 0;
    int opt;
    int num_o_flags = 0;
    int num_i_flags = 0;

    while ((opt = getopt(argc, argv, OPTLIST)) != -1) {
        flag_nums++;
        switch(opt) {
            case 'a':
                flags.a_flag = true;
                break;
            case 'i':   // TODO: add check for next arg and arg set pattern in struct
                flags.i_flag = true;
                flags.i_flag_obj = realloc(flags.i_flag_obj, sizeof(globs) * (num_i_flags + 1));
                CHECK_ALLOC(flags.i_flag_obj);
                flags.i_flag_obj[num_i_flags].regex = glob2regex(strdup(optarg));

                if (flags.i_flag_obj[num_i_flags].regex == NULL) {
                    fprintf(stderr, "Error: Invalid regex pattern.\n");
                    usage();
                    exit(EXIT_FAILURE);
                }

                num_i_flags++;
                break;
            case 'n':
                flags.n_flag = true;
                flags.v_flag = true;
                break;
            case 'o':
                flags.o_flag = true;
                flags.o_flag_obj = realloc(flags.o_flag_obj, sizeof(globs) * (num_o_flags + 1));
                CHECK_ALLOC(flags.o_flag_obj);

                flags.o_flag_obj[num_o_flags].regex = glob2regex(strdup(optarg));

                if (flags.o_flag_obj[num_o_flags].regex == NULL) {
                    fprintf(stderr, "Error: Invalid regex pattern.\n");
                    usage();
                    exit(EXIT_FAILURE);
                }

                num_o_flags++;
                break;
            case 'p':
                flags.p_flag = true;
                break;
            case 'r':
                flags.r_flag = true;
                break;
            case 'v':
                flags.v_flag = true;
                break;
            default:
                usage();
                break;
        }
    }

    flags.i_flag_obj = realloc(flags.i_flag_obj, sizeof(globs) * (num_i_flags + 1));
    flags.i_flag_obj[num_i_flags].regex = NULL;
    flags.o_flag_obj = realloc(flags.o_flag_obj, sizeof(globs) * (num_o_flags + 1));
    flags.o_flag_obj[num_o_flags].regex = NULL;

    flags.num_i_flags = num_i_flags;
    flags.num_o_flags = num_o_flags;
}
