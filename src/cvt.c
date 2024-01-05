#include "cvt.h"
#include "base64.h"
#include "log.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

int
cvt_json_2_pb(ProtobufCMessage* restrict msg, cJSON* restrict root)
{
    assert(NULL != msg);
    assert(NULL != root);
    assert(cJSON_IsObject(root));

    int                       rtn        = EXIT_FAILURE;
    ProtobufCFieldDescriptor* field_desc = NULL;

    for (cJSON* item = root->child;; item = item->next) {
        if (NULL == item) {
            break;
        }
        assert(NULL != item);

        if (NULL == (field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->descriptor, item->string))) {
            INFO("json field %s doesn't match any field in message %s\n", item->string, msg->descriptor->name);
            continue;
        }
        assert(NULL != field_desc);

        switch (field_desc->type) {
        case PROTOBUF_C_TYPE_INT32:
        case PROTOBUF_C_TYPE_SINT32:
        case PROTOBUF_C_TYPE_SFIXED32:
            DEBUG("set %s to %s.(int32)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsNumber(item)) {
                *(int32_t*)((void*)msg + field_desc->offset) = (int32_t)item->valuedouble;
            } else if (cJSON_IsString(item) && NULL != cJSON_GetStringValue(item)) {
                *(int32_t*)((void*)msg + field_desc->offset) = strtol(cJSON_GetStringValue(item), NULL, 0);
            } else {
                ERROR("JSON field %s is not a number\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_INT64:
        case PROTOBUF_C_TYPE_SINT64:
        case PROTOBUF_C_TYPE_SFIXED64:
            DEBUG("set %s to %s.(int64)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsNumber(item)) {
                *(int64_t*)((void*)msg + field_desc->offset) = (int64_t)item->valuedouble;
            } else if (cJSON_IsString(item) && NULL != cJSON_GetStringValue(item)) {
                *(int64_t*)((void*)msg + field_desc->offset) = strtoll(cJSON_GetStringValue(item), NULL, 0);
            } else {
                ERROR("JSON field %s is not a number\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_UINT32:
        case PROTOBUF_C_TYPE_FIXED32:
            DEBUG("set %s to %s.(uint32)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsNumber(item)) {
                *(uint32_t*)((void*)msg + field_desc->offset) = (uint32_t)item->valuedouble;
            } else if (cJSON_IsString(item) && NULL != cJSON_GetStringValue(item)) {
                *(uint32_t*)((void*)msg + field_desc->offset) = strtoul(cJSON_GetStringValue(item), NULL, 0);
            } else {
                ERROR("JSON field %s is not a number\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_UINT64:
        case PROTOBUF_C_TYPE_FIXED64:
            DEBUG("set %s to %s.(uint64)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsNumber(item)) {
                *(uint64_t*)((void*)msg + field_desc->offset) = (uint64_t)item->valuedouble;
            } else if (cJSON_IsString(item) && NULL != cJSON_GetStringValue(item)) {
                *(uint64_t*)((void*)msg + field_desc->offset) = strtoull(cJSON_GetStringValue(item), NULL, 0);
            } else {
                ERROR("JSON field %s is not a number\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_FLOAT:
            DEBUG("set %s to %s.(float)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsNumber(item)) {
                *(float*)((void*)msg + field_desc->offset) = (float)item->valuedouble;
            } else if (cJSON_IsString(item) && NULL != cJSON_GetStringValue(item)) {
                *(float*)((void*)msg + field_desc->offset) = strtof(cJSON_GetStringValue(item), NULL);
            } else {
                ERROR("JSON field %s is not a number\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_DOUBLE:
            DEBUG("set %s to %s.(double)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
            if (cJSON_IsNumber(item)) {
                *(double*)((void*)msg + field_desc->offset) = item->valuedouble;
            } else if (cJSON_IsString(item) && NULL != cJSON_GetStringValue(item)) {
                *(double*)((void*)msg + field_desc->offset) = strtod(cJSON_GetStringValue(item), NULL);
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
            if (cJSON_IsString(item)) {
                if (NULL != cJSON_GetStringValue(item) && strlen(cJSON_GetStringValue(item)) > 0) {
                    *(int*)((void*)msg + field_desc->offset) =
                        NULL != protobuf_c_enum_descriptor_get_value_by_name(field_desc->descriptor, cJSON_GetStringValue(item))
                            ? protobuf_c_enum_descriptor_get_value_by_name(field_desc->descriptor, cJSON_GetStringValue(item))->value
                            : (ERROR_EXPR("enum string %s is not include in enum %s\n", cJSON_GetStringValue(item), field_desc->name),
                               (int)field_desc->default_value);
                } else {
                    ERROR("JSON field %s is empty string\n", field_desc->name);
                }
            } else if (cJSON_IsNumber(item)) {
                *(int*)((void*)msg + field_desc->offset) =
                    NULL != protobuf_c_enum_descriptor_get_value(field_desc->descriptor, (int)item->valuedouble)
                        ? protobuf_c_enum_descriptor_get_value(field_desc->descriptor, (int)item->valuedouble)->value
                        : (ERROR_EXPR("enum number %d is not include in enum %s\n", (int)item->valuedouble, field_desc->name),
                           (int)field_desc->default_value);
            } else {
                ERROR("JSON field %s is not valid\n", field_desc->name);
            }
            DEBUG("set %s to %s(enum)%s\n", cJSON_Print(item), msg->descriptor->name, field_desc->name);
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

        case PROTOBUF_C_TYPE_BYTES:
            DEBUG("set %s to (string)%s\n", cJSON_Print(item), field_desc->name);
            if (cJSON_IsString(item)) {
                if (NULL != cJSON_GetStringValue(item) && strlen(cJSON_GetStringValue(item)) > 0) {
                    int len = strlen(cJSON_GetStringValue(item));

                    *(char**)((void*)msg + field_desc->offset) = (char*)calloc(BASE64_DECODE_OUT_SIZE(len), 1);
                    if (NULL == *(char**)((void*)msg + field_desc->offset)) {
                        goto ERROR_EXIT_POINT;
                    }

                    if (base64_decode(cJSON_GetStringValue(item), len, *(char**)((void*)msg + field_desc->offset)) <= 0) {
                        free(*(char**)((void*)msg + field_desc->offset));
                        *(char**)((void*)msg + field_desc->offset) = NULL;
                        goto ERROR_EXIT_POINT;
                    }

                } else {
                    ERROR("JSON field %s is empty string\n", field_desc->name);
                }
            } else {
                ERROR("JSON field %s is not a string\n", field_desc->name);
            }
            break;

        case PROTOBUF_C_TYPE_MESSAGE:
            if (cJSON_IsObject(item)) {
                ProtobufCMessageDescriptor* sub_msg_desc = (ProtobufCMessageDescriptor*)field_desc->descriptor;
                ProtobufCMessage*           sub_msg      = (ProtobufCMessage*)calloc(1, sub_msg_desc->sizeof_message);

                sub_msg_desc->message_init(sub_msg);

                if (cvt_json_2_pb(sub_msg, item) == EXIT_SUCCESS) {
                    *(ProtobufCMessage**)((void*)msg + field_desc->offset) = sub_msg;
                } else {
                    ERROR("cvt json 2 pb failed!\n");
                    protobuf_c_message_free_unpacked(sub_msg, NULL);
                }

            } else {
                ERROR("JSON field %s is not an object\n", field_desc->name);
            }
            break;

        default:
            INFO("field %s cannot processed in json for now\n", field_desc->name);
            break;
        }
    }

    rtn = EXIT_SUCCESS;

ERROR_EXIT_POINT:

    return rtn;
}

int
cvt_pb_2_json(ProtobufCMessage* restrict msg, cJSON* restrict root)
{
    return EXIT_SUCCESS;
}