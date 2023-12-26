#ifndef __UTITILITIES_H__
#define __UTITILITIES_H__

/**
 * @brief
 *
 * @param string
 * @param delimiters_str
 * @return char*
 */
char* strtok_str(char* string, const char* delimiters_str);

/**
 * @brief
 *
 * @param s
 * @param c
 * @return char*
 */
char* remove_char_from_str(char* s, const char c);

/**
 * @brief
 *
 * @param filepath
 * @param msg
 * @return int
 */
int write2File(const char* filepath, const char* msg);

/**
 * @brief
 *
 * @param file_name
 * @param data
 * @return int
 */
int readFromFile(const char* file_name, char** data);

#endif