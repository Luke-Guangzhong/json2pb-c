#include "log.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>

char*
strtok_str(char* restrict string, const char* restrict delimiters_str)
{
    static char* begin_ptr = NULL;
    char*        rtn       = NULL;
    char*        end_ptr   = NULL;

    if (NULL == delimiters_str || strlen(delimiters_str) == 0) {
        ERROR("param delimiters_str invalid!\n");
        return rtn;
    }

    if (NULL != string && strlen(string) != 0) {
        // new string
        begin_ptr = string;
        // skip leading delimiters
        if (memcmp(string, delimiters_str, strlen(delimiters_str)) == 0) {
            begin_ptr = string + strlen(delimiters_str);
        }
    }

    if (NULL == begin_ptr || (*begin_ptr) == '\0') {
        DEBUG("\n\t[ACT]begin_ptr reach end!\n");
        return rtn;
    }

    end_ptr = strstr(begin_ptr, delimiters_str);
    rtn     = begin_ptr;
    if (NULL != end_ptr) {
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
remove_char_from_str(char* restrict s, const char c)
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
write2File(const char* restrict filepath, const char* restrict msg)
{
    int   rtn = -2;
    FILE* fp  = NULL;
    if (NULL == filepath || strlen(filepath) <= 0) {
        ERROR("filepath empty!(%s)\n", __FUNCTION__);
        return rtn;
    }
    if (NULL == msg || strlen(msg) <= 0) {
        ERROR("message empty!(%s)\n", __FUNCTION__);
        return rtn;
    }

    fp = fopen(filepath, "w");
    if (NULL == fp) {
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
readFromFile(const char* restrict file_name, char* restrict* restrict data)
{
    assert(NULL != file_name);
    assert(NULL != data);

    FILE* fp;
    int   ch_count = 0;

    fp = fopen(file_name, "r+");
    if (NULL == fp) {
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