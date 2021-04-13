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

#include "typedefs.h"

const char *GetJSONString (const json_t *json_p, const char * const key_s);

bool GetJSONInteger (const json_t *json_p, const char * const key_s, json_int_t *value_p);

bool GetJSONNumber (const json_t *json_p, const char * const key_s, double *value_p);

bool GetJSONBoolean (const json_t *json_p, const char * const key_s, bool *value_p);

int PrintJSONObject (FILE *out_f, const json_t * const json_p, const char * const prefix_s);

json_t *GetWebJSON (const char *url_s);

#endif /* CLIENTS_FRICTIONLESS_DATA_INCLUDE_FD_JSON_UTIL_H_ */
