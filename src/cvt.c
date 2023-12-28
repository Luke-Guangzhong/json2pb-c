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
            DEBUG("set %s to %s.(int32)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsNumber(item)) {
                *(int32_t*)((void*)msg + field_desc->offset) = (int32_t)item->valuedouble;
            } else {
                ERROR("JSON field %s is not a number\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_BOOL:
            DEBUG("set %s to %s.(bool)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsBool(item)) {
                cJSON_IsTrue(item) ? ((*(bool*)((void*)msg + field_desc->offset)) = true)
                                   : ((*(bool*)((void*)msg + field_desc->offset)) = false);
            } else {
                ERROR("JSON field %s is not a bool\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_ENUM:
            DEBUG("set %s to %s(enum)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsString(item)) {
                if (NULL != cJSON_GetStringValue(item) && strlen(cJSON_GetStringValue(item)) > 0) {
                    *(int*)((void*)msg + field_desc->offset) =
                        protobuf_c_enum_descriptor_get_value_by_name(field_desc->descriptor, cJSON_GetStringValue(item))
                            ->value;
                } else {
                    ERROR("JSON field %s is empty string\n", field_desc->name);
                }
            } else if (cJSON_IsNumber(item)) {
                *(int*)((void*)msg + field_desc->offset) = (int)item->valuedouble;
            } else {
                ERROR("JSON field %s is not valid\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_STRING:
            DEBUG("set %s to (string)%s\n", cJSON_Print(item), field_desc->name);
            if (cJSON_IsString(item)) {
                if (NULL != cJSON_GetStringValue(item) && strlen(cJSON_GetStringValue(item)) > 0) {
                    if (asprintf((char**)((void*)msg + field_desc->offset), "%s", cJSON_GetStringValue(item)) < 0) {
                        goto ERROR_EXIT_POINT;
                    }
                } else {
                    ERROR("JSON field %s is empty string\n", field_desc->name);
                }
            } else {
                ERROR("JSON field %s is not a string\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_INT64:
        case PROTOBUF_C_TYPE_SINT64:
        case PROTOBUF_C_TYPE_SFIXED64:
            DEBUG("set %s to %s.(int64)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsNumber(item)) {
                *(int64_t*)((void*)msg + field_desc->offset) = (int64_t)item->valuedouble;
            } else {
                ERROR("JSON field %s is not a number\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_UINT32:
        case PROTOBUF_C_TYPE_FIXED32:
            DEBUG("set %s to %s.(uint32)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsNumber(item)) {
                *(uint32_t*)((void*)msg + field_desc->offset) = (uint32_t)item->valuedouble;
            } else {
                ERROR("JSON field %s is not a number\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_UINT64:
        case PROTOBUF_C_TYPE_FIXED64:
            DEBUG("set %s to %s.(uint64)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsNumber(item)) {
                *(uint64_t*)((void*)msg + field_desc->offset) = (uint64_t)item->valuedouble;
            } else {
                ERROR("JSON field %s is not a number\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_FLOAT:
            DEBUG("set %s to %s.(float)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsNumber(item)) {
                *(float*)((void*)msg + field_desc->offset) = (float)item->valuedouble;
            } else {
                ERROR("JSON field %s is not a number\n", field_desc->name);
            }
            break;
        case PROTOBUF_C_TYPE_DOUBLE:
            DEBUG("set %s to %s.(double)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsNumber(item)) {
                *(double*)((void*)msg + field_desc->offset) = item->valuedouble;
            } else {
                ERROR("JSON field %s is not a number\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_BYTES:
        case PROTOBUF_C_TYPE_MESSAGE:
        default:
            INFO("field %s cannot processed in json for now\n", field_desc->name);
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