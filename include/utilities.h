#ifndef __UTITILITIES_H__
#define __UTITILITIES_H__

/**
 * @brief
 *
 * @param string
 * @param delimiters_str
 * @return char*
 */
char* strtok_str(char* restrict string, const char* restrict delimiters_str);

/**
 * @brief
 *
 * @param s
 * @param c
 * @return char*
 */
char* remove_char_from_str(char* restrict s, const char c);

/**
 * @brief
 *
 * @param filepath
 * @param msg
 * @return int
 */
int write2File(const char* restrict filepath, const char* restrict msg);

/**
 * @brief
 *
 * @param file_name
 * @param data
 * @return int
 */
int readFromFile(const char* restrict file_name, char* restrict* restrict data);

#endif