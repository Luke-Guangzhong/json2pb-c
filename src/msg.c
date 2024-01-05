#include "msg.h"
#include "log.h"

char*
request_print(J2b__Request msg)
{
    char* buffer = NULL;
    asprintf(&buffer,
             "[\n"
             "\t[method]:%s\n"
             "\t[rssi]:%d\n"
             "\t[ip]:%s\n"
             "\t[timestamp]:%lu\n"
             "\t[tx_bytes]:%lu\n"
             "\t[rx_bytes]:%lu\n"
             "\t[laser]:%s\n"
             "\t[appendix]:%s\n"
             "\t[coordinate]:%s,%f,%f,%f,%f\n"
             "\t[status]:%s\n"
             "\t[weather]:%s\n"
             "\t[raw_file]:%s\n"
             "]\n",
             NULL != protobuf_c_enum_descriptor_get_value(&j2b__request__method__descriptor, msg.method)
                 ? protobuf_c_enum_descriptor_get_value(&j2b__request__method__descriptor, msg.method)->name
                 : (ERROR_EXPR("value %d is not included in enum %s\n", msg.method, j2b__request__method__descriptor.name),
                    protobuf_c_message_descriptor_get_field_by_name(&msg.base, "method")->default_value),
             msg.rssi, msg.ip, msg.timestamp, msg.tx_bytes, msg.rx_bytes, msg.laser ? "true" : "false", msg.appendix,
             NULL != protobuf_c_enum_descriptor_get_value(&j2b__coordinate__type__descriptor, msg.coordinate->type)
                 ? protobuf_c_enum_descriptor_get_value(&j2b__coordinate__type__descriptor, msg.coordinate->type)->name
                 : "unknown",
             msg.coordinate->longitude, msg.coordinate->latitude, msg.coordinate->angle, msg.coordinate->distance,
             NULL != protobuf_c_enum_descriptor_get_value(&j2b__status__descriptor, msg.status)
                 ? protobuf_c_enum_descriptor_get_value(&j2b__status__descriptor, msg.status)->name
                 : (ERROR_EXPR("value %d is not included in enum %s\n", msg.status, j2b__status__descriptor.name),
                    protobuf_c_message_descriptor_get_field_by_name(&msg.base, "status")->default_value),
             NULL != protobuf_c_enum_descriptor_get_value(&j2b__basic_direction__descriptor, msg.weather->direct)
                 ? protobuf_c_enum_descriptor_get_value(&j2b__basic_direction__descriptor, msg.weather->direct)->name
                 : (ERROR_EXPR("value %d is not included in enum %s\n", msg.weather->direct, j2b__basic_direction__descriptor.name),
                    protobuf_c_message_descriptor_get_field_by_name(&msg.weather->base, "direct")->default_value),
             msg.raw_file);
    return buffer;
}