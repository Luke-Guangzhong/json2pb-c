#include "cJSON.h"
#include "cvt.h"
#include "log.h"
#include "msg.h"
#include "msg.pb-c.h"
#include "utilities.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum test_event {
    unknown,
    j2r,
    r2j
} Event;

int parse_command_line(int argc, char const* argv[]);
int test_cvt_json_2_pb(void);
int test_cvt_pb_2_json(void);

char                       json_file[128] = {'\0'};
Event                      mode           = unknown;
const char*                option_string  = "j::p::m:";
static const struct option long_options[] = {
    {"json_file", optional_argument, NULL, 'j'},
    {"pb_file",   optional_argument, NULL, 'p'},
    {"mode",      required_argument, NULL, 'm'},
    {0,           0,                 0,    0  }
};

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
    return EXIT_SUCCESS;
}

int
main(int argc, char const* argv[])
{
    printf("hello world!\n");
    printf("cJSON version = %s\n", cJSON_Version());
    printf("Protobuf-c version = %s\n", protobuf_c_version());
    int status = EXIT_SUCCESS;
    parse_command_line(argc, argv);
    switch (mode) {
    case j2r:
        status = test_cvt_json_2_pb();
        break;
    default:
        INFO("Unknown mode");
        break;
    }

    return status;
}

int
test_cvt_json_2_pb(void)
{
    int           status   = EXIT_FAILURE;
    char*         raw_json = NULL;
    cJSON*        root     = NULL;
    J2b__Request* req_msg  = NULL;

    readFromFile(json_file, &raw_json);
    if (NULL == raw_json) {
        ERROR("read file failed!\n,file name = %s\n", json_file);
        goto ERROR_EXIT_POINT;
    }

    root = cJSON_Parse(raw_json);
    if (NULL == root) {
        ERROR("cJSON Parse Failed!\n");
        goto ERROR_EXIT_POINT;
    }
    req_msg = (J2b__Request*)calloc(1, sizeof(J2b__Request));
    assert(NULL != req_msg);
    j2b__request__init(req_msg);

    cvt_json_2_pb((ProtobufCMessage*)req_msg, root);

    printf("json->pb\njson=%s\n", cJSON_Print(root));

    printf("pb=%s\n", request_print(*req_msg));

    status = EXIT_SUCCESS;

ERROR_EXIT_POINT:
    if (NULL != raw_json) {
        free(raw_json);
        raw_json = NULL;
    }
    if (NULL != root) {
        cJSON_Delete(root);
        root = NULL;
    }
    if (NULL != req_msg) {
        protobuf_c_message_free_unpacked((ProtobufCMessage*)req_msg, NULL);
        req_msg = NULL;
    }

    return status;
}

int
test_cvt_pb_2_json(void)
{
    return EXIT_SUCCESS;
}