#include "msg.h"

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
             "\t[coordinate]:\n"
             "\t[status]:%s\n"
             "\t[weather]:\n"
             "]\n",
             protobuf_c_enum_descriptor_get_value(&j2b__request__method__descriptor, msg.method)->name, msg.rssi,
             msg.ip, msg.timestamp, msg.tx_bytes, msg.rx_bytes, msg.laser ? "true" : "false", msg.appendix,
             protobuf_c_enum_descriptor_get_value(&j2b__status__descriptor, msg.status)->name);
    return buffer;
}