#ifndef __CVT_H__
#define __CVT_H__

#include "cJSON.h"
#include <protobuf-c/protobuf-c.h>

/**
 * @brief convert a cjson object to a protobuf msg
 *
 * @param[out] msg a pointer to the initialized message
 * @param[in] root a pointer to the valid cJSON object
 * @return int EXIT_SUCCESS OR EXIT_FAILURE
 * @todo bytes and repeated fields
 */
int cvt_json_2_pb(ProtobufCMessage* msg, cJSON* root);

/**
 * @brief
 *
 * @param msg
 * @param root
 * @return int
 */
int cvt_pb_2_json(ProtobufCMessage* msg, cJSON* root);

#endif