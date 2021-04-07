/*
 * json_util.h
 *
 *  Created on: 7 Apr 2021
 *      Author: billy
 */

#ifndef CLIENTS_FRICTIONLESS_DATA_INCLUDE_FD_JSON_UTIL_H_
#define CLIENTS_FRICTIONLESS_DATA_INCLUDE_FD_JSON_UTIL_H_

#include <stdio.h>

#include "jansson.h"

const char *GetJSONString (const json_t *json_p, const char * const key_s);

int PrintJSONObject (FILE *out_f, const json_t * const json_p, const char * const prefix_s);


#endif /* CLIENTS_FRICTIONLESS_DATA_INCLUDE_FD_JSON_UTIL_H_ */
