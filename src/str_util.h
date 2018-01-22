/*******************************************************************************
 * Stefan Bylund 2017
 *
 * String utility functions.
 ******************************************************************************/

#ifndef _STR_UTIL_H
#define _STR_UTIL_H

#include <stdio.h>

/*
 * Convert the given string to lower-case and return it.
 */
char *str_lower_case(char *s);

/*
 * Remove any carriage return characters from the given string, which is
 * returned.
 */
char *str_remove_cr(char *s);

/*
 * Remove any trailing line feed character from the given string, which is
 * returned.
 */
char *str_remove_trailing_lf(char *s);

/*
 * Word wrap the given string (i.e. replace space characters with newline
 * characters) so that it fits the width of the specified FZX output terminal
 * on an even word boundary. Any existing newline characters are respected.
 * The word wrapped string is returned.
 *
 * NOTE: Any carriage return charaters should be removed prior to the call.
 */
char *str_word_wrap(FILE *out, char *s);

#endif
