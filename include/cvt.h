#ifndef __CVT_H__
#define __CVT_H__

#include "cJSON.h"
#include <protobuf-c/protobuf-c.h>

int cvt_json_2_pb(ProtobufCMessage* msg, cJSON* root);
int cvt_pb_2_json(ProtobufCMessage* msg, cJSON* root);

#endif