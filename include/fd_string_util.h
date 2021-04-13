/*
 * fd_string_utils.h
 *
 *  Created on: 12 Apr 2021
 *      Author: billy
 */

#ifndef CLIENTS_FRICTIONLESS_DATA_INCLUDE_FD_STRING_UTIL_H_
#define CLIENTS_FRICTIONLESS_DATA_INCLUDE_FD_STRING_UTIL_H_

/*
** Copyright 2014-2016 The Earlham Institute
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/


#include <stdio.h>

#include "typedefs.h"

#ifdef __cplusplus
	extern "C" {
#endif


/**
 * Copy a string to a newly created string.
 *
 * @param src_p The string to copy.
 * @param length_to_copy If this is greater than zero, copy this many characters from src_p
 * or its entirety, whichever is smaller. If this is zero or less then all of src_p is copied.
 * @param trim Whether to trim left and right trailing whitespace or not.
 * @return A newly allocated copy of the source string or NULL if there was an error. If
 * the string was successfully created, its deallocation must be done with FreeCopiedString()
 * when you are finished with it.
 * @ingroup utility_group
 */
char *CopyToNewString (const char * const src_p, const size_t length_to_copy, bool trim);


/**
 * Copy a string to a newly created string. This calls CopyToNewString() with length_to_copy set to
 * the length of the source string and trim set to <code>false</code>.
 *
 * @param src_s he string to copy.
 * @return A newly allocated copy of the source string or NULL if there was an error. If
 * the string was successfully created, its deallocation must be done with FreeCopiedString()
 * when you are finished with it.
 * @ingroup utility_group
 */
char *EasyCopyToNewString (const char * const src_s);


/**
 * Delete a string previously allocated using CopyToNewString.
 *
 * @param str_p The string to deallocate. This is checked for being <code>NULL</code> in which
 * case this function becomes a no-op routine.
 * @ingroup utility_group
 */
void FreeCopiedString (char *str_p);


/**
 * Read a line from a file with no buffer overflow issues.
 *
 * @param in_f The File to read from.
 * @param buffer_pp Pointer to a buffer string where the line will get written to.
 * After being used, *buffer_pp muet be freed.
 * @return TRUE if the line was read successfully, false if there was an error.
 * @ingroup utility_group
 */
bool GetLineFromFile (FILE *in_f, char **buffer_pp);


/**
 * Free a buffer that has been allocated with GetLineFromFile.
 *
 * @param buffer_p Pointer to a buffer to be freed.
 * @ingroup utility_group
 */
void FreeLineBuffer (char *buffer_p);


/**
 * Clone a valid string to a destination string pointer.
 *
 * @param src_s The string to clone. If this is <code>NULL</code> then
 * the function will treat it as a valid clone.
 * @param dest_ss The destination where the cloned string will be stored
 * if successful. If src_s is <code>NULL</code>, then this will be set to <code>
 * NULL</code> too.
 * @return <code>true</code> if the string was successfully cloned or
 * it src_s is <code>NULL</code>, <code>false</code> otherwise.
 */
bool CloneValidString (const char *src_s, char **dest_ss);


/**
 * Format a string containing a number by removing all of its
 * trailing zeroes, e.g.
 *
 *		7.0000 would become 7
 *		7.5000 would become 7.5
 *
 * @param numeric_string_p The string to formatted.
 * @ingroup utility_group
 */
void NullifyTrailingZeroes (char *numeric_string_p);


/**
 * Test whether a string is NULL or contains just whitespace.
 *
 * @param value_s The string to test.
 * @return <code>true</code> If the string is <code>NULL</code> or just whitespace, <code>false</code> otherwise.
 * @ingroup utility_group
 */
bool IsStringEmpty (const char *value_s);


/**
 * Do a case-insensitive comparison between two strings.
 * This is a case-insensitive version of the standard ANSI function strcmp.
 *
 * @param c0_s The first string.
 * @param c1_s The second string.
 * @return Less than zero if the first string is less than the second,
 * greater than zero if the first string is greater than the second and
 * zero if the strings match.
 * @ingroup utility_group
 */
int Stricmp (const char *c0_s, const char *c1_s);


/**
 * Do a case-insensitive comparison between the initial portions of two strings.
 * This is a case-insensitive version of the standard ANSI function strncmp.
 *
 * @param c0_s The first string.
 * @param c1_s The second string.
 * @param length The maximum number of characters in each string to compare.
 * @return Less than zero if the first string is less than the second,
 * greater than zero if the first string is greater than the second and
 * zero if the strings match.
 * @ingroup utility_group
 */
int Strnicmp (const char *c0_s, const char *c1_s, size_t length);


/**
 * Find a substring within a string. The search is case-insensitive.
 * This is a case-insensitive version of the standard ANSI function strstr.
 *
 * @param value_s The string to search within.
 * @param substring_s The string to search for
 * @return The substring within the larger string or <code>NULL</code> if it was
 * not found.
 * @ingroup utility_group
 */
const char *Stristr (const char *value_s, const char *substring_s);



/**
 * Find the last occurence of a substring within a string.
 *
 * @param haystack_s The string to search within.
 * @param needle_s The string to search for
 * @return The substring within the larger string or <code>NULL</code> if it was
 * not found.
 * @ingroup utility_group
 */
char *Strrstr (const char *haystack_s, const char *needle_s);






/**
 * Make a copy of a string.
 *
 * @param dest_ss Pointer to the string where the source string
 * will be copied to.
 * @param src_s The string to copy.
 * @return <code>true</code> if successful, <code>false</code> otherwise.
 * If successful, any string that dest_ss currently points to will be freed
 * prior to pointing at the copy. If the function is unsuccessful then the value
 * pointed to by dest_ss will remain intact.
 * @ingroup utility_group
 */
bool ReplaceStringValue (char **dest_ss, const char * const src_s);


/**
 * Replace each instance of a character within a string with another
 *
 * @param value_s The string to replace the character in.
 * @param old_data The character to be replaced.
 * @param new_data The replacement character.
 * @ingroup utility_group
 */
void ReplaceChars (char *value_s, char old_data, char new_data);


/**
 * Concatenate a va_list of strings.
 *
 * @param value_s The varargs-style array of <code>NULL</code> terminated strings to append. The final entry
 * in this varargs-array must be a <code>NULL</code>.
 * @return The newly-allocated string that should be freed with FreeCopiedString() when
 * no longer needed or <code>NULL</code> upon failure.
 * @ingroup utility_group
 */
char *ConcatenateVarargsStrings (const char *value_s, ...);



char *ConcatenateStrings (const char * const first_s, const char * const second_s);



char *ConvertSizeTToString (const size_t value);


/**
 * Test whether a given string contains any whitespace characters.
 *
 * @param value_s The string to check.
 * @return <code>true</code> if the string contains any whitespace characters, <code>false</code> otherwise.
 * @ingroup utility_group
 */
bool DoesStringContainWhitespace (const char *value_s);


bool DoesStringStartWith (const char *value_s, const char *start_s);


bool DoesStringEndWith (const char *value_s, const char *ending_s);


char *GetStringAsLowerCase (const char *value_s);


void FreeStringArray (char **values_ss);



#ifdef __cplusplus
}
#endif





#endif /* CLIENTS_FRICTIONLESS_DATA_INCLUDE_FD_STRING_UTIL_H_ */
