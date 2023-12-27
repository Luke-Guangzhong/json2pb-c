#include "cJSON.h"
#include "cvt.h"
#include "log.h"
#include "utilities.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum test_event {
    unknown,
    j2r,
    r2j
} Event;

const char* option_string = "j::p::m:";

static const struct option long_options[] = {
    {"json_file", optional_argument, NULL, 'j'},
    {"pb_file",   optional_argument, NULL, 'p'},
    {"mode",      required_argument, NULL, 'm'},
    {0,           0,                 0,    0  }
};

char json_file[128] = {'\0'};

Event mode = unknown;

int
parse_command_line(int argc, char const* argv[])
{
    int item      = 0;
    int opt_index = 0;
    while (-1 != (item = getopt_long(argc, argv, option_string, long_options, &opt_index))) {
        switch (item) {
        case 'j':
            INFO("Option j read %s\n", optarg);
            snprintf(json_file, 127, "%s", optarg);
            break;
        case 'p':
            INFO("Option p read %s\n", optarg);
            break;
        case 'm':
            INFO("Option m read %s\n", optarg);
            if (strcmp("j2r", optarg) == 0) {
                mode = j2r;
            } else {
                mode = unknown;
            }
            break;
        case '?':
            break;
        default:
            break;
        }
    }
}

int
main(int argc, char const* argv[])
{
    printf("hello world!\n");
    int status = EXIT_SUCCESS;
    parse_command_line(argc, argv);
    switch (mode) {
    case j2r:
        // status = cvt_json_2_pb(json_file);
        break;
    default:
        INFO("Unknown mode");
        break;
    }

    return status;
}
