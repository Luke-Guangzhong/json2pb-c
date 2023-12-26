#include "log.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>

char*
strtok_str(char* string, const char* delimiters_str)
{
    static char* begin_ptr = NULL;
    char*        rtn       = NULL;
    char*        end_ptr   = NULL;

    if (delimiters_str == NULL || strlen(delimiters_str) == 0) {
        ERROR("param delimiters_str invalid!\n");
        return rtn;
    }

    if (string != NULL && strlen(string) != 0) {
        // new string
        begin_ptr = string;
        // skip leading delimiters
        if (memcmp(string, delimiters_str, strlen(delimiters_str)) == 0) {
            begin_ptr = string + strlen(delimiters_str);
        }
    }

    if (begin_ptr == NULL || (*begin_ptr) == '\0') {
        DEBUG("\n\t[ACT]begin_ptr reach end!\n");
        return rtn;
    }

    end_ptr = strstr(begin_ptr, delimiters_str);
    rtn     = begin_ptr;
    if (end_ptr != NULL) {
        // find delimiters
        memset(end_ptr, 0, strlen(delimiters_str));
        begin_ptr = end_ptr + strlen(delimiters_str);
    } else {
        // end
        begin_ptr = NULL;
    }
    return rtn;
}

char*
remove_char_from_str(char* s, const char c)
{
    int i   = 0;
    int j   = 0;
    int len = 0;

    len = strlen(s);
    for (i = j = 0; i < len; i++) {
        if (s[i] != c) {
            s[j++] = s[i];
        }
    }
    s[j] = '\0';
}

int
write2File(const char* filepath, const char* msg)
{
    int   rtn = -2;
    FILE* fp  = NULL;
    if (filepath == NULL || strlen(filepath) <= 0) {
        ERROR("filepath empty!(%s)\n", __FUNCTION__);
        return rtn;
    }
    if (msg == NULL || strlen(msg) <= 0) {
        ERROR("message empty!(%s)\n", __FUNCTION__);
        return rtn;
    }

    fp = fopen(filepath, "w");
    if (fp == NULL) {
        ERROR("file open err(filepath=%s)\n", filepath);
        return rtn;
    }

    rtn = fputs(msg, fp);
    if (rtn < 0) {
        ERROR("fputs err (%s)\n", __FUNCTION__);
    }

    fclose(fp);
    return rtn;
}

int
readFromFile(const char* file_name, char** data)
{
    assert(NULL != file_name);
    assert(NULL != data);

    FILE* fp;
    int   ch_count = 0;

    fp = fopen(file_name, "r+");
    if (fp == NULL) {
        ERROR("Failed to open file %s (errno %d)\n", file_name, errno);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    ch_count = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    *data = (char*)malloc(sizeof(char) * (ch_count + 1));
    fread(*data, 1, ch_count, fp);
    (*data)[ch_count] = '\0';

    fclose(fp);
    return 0;
}