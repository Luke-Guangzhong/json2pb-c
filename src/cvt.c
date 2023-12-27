#include "cvt.h"
#include "log.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

int
cvt_json_2_pb(ProtobufCMessage* msg, cJSON* root)
{
    assert(NULL != msg);
    assert(NULL != root);

    int                       rtn        = EXIT_FAILURE;
    ProtobufCFieldDescriptor* field_desc = NULL;
    cJSON*                    item       = root->child;

    for (size_t i = 0; i < msg->descriptor->n_fields; i++) {
        if (NULL == item) {
            break;
        }

        if (NULL == (field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->descriptor, item->string))) {
            INFO("json field %s doesn't match any field in message %s\n", item->string, msg->descriptor->name);
            continue;
        }

        switch (field_desc->type) {
        case PROTOBUF_C_TYPE_INT32:
        case PROTOBUF_C_TYPE_SINT32:
        case PROTOBUF_C_TYPE_SFIXED32:
            DEBUG("set %s to (int32)%s\n", cJSON_Print(item), field_desc->name);
            if (cJSON_IsNumber(item)) {
                *(int32_t*)((void*)msg + field_desc->offset) = (int32_t)item->valuedouble;
            } else {
                ERROR("JSON field %s is not a number\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_BOOL:
            DEBUG("set %d to (bool)%s\n", cJSON_Print(item), field_desc->name);
            if (cJSON_IsBool(item)) {
                cJSON_IsTrue(item) ? ((*(bool*)((void*)msg + field_desc->offset)) = true)
                                   : ((*(bool*)((void*)msg + field_desc->offset)) = false);
            } else {
                ERROR("JSON field %s is not a bool\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_ENUM:
            break;
        case PROTOBUF_C_TYPE_STRING:
            break;
        case PROTOBUF_C_TYPE_INT64:
        case PROTOBUF_C_TYPE_SINT64:
        case PROTOBUF_C_TYPE_SFIXED64:
        case PROTOBUF_C_TYPE_UINT32:
        case PROTOBUF_C_TYPE_FIXED32:
        case PROTOBUF_C_TYPE_UINT64:
        case PROTOBUF_C_TYPE_FIXED64:
        case PROTOBUF_C_TYPE_FLOAT:
        case PROTOBUF_C_TYPE_DOUBLE:
        case PROTOBUF_C_TYPE_BYTES:
        case PROTOBUF_C_TYPE_MESSAGE:
        default:
            INFO("field %s cannot processed in json for now", field_desc->name);
            break;
        }
        item = item->next;
    }

    rtn = EXIT_SUCCESS;

ERROR_EXIT_POINT:

    return rtn;
}

int
cvt_pb_2_json(ProtobufCMessage* msg, cJSON* root)
{
    return EXIT_SUCCESS;
}