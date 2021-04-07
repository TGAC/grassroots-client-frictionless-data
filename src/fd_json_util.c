/*
 * json_util.c
 *
 *  Created on: 7 Apr 2021
 *      Author: billy
 */

#include "fd_json_util.h"



const char *GetJSONString (const json_t *json_p, const char * const key_s)
{
	const char *value_s = NULL;
	json_t *value_p = json_object_get (json_p, key_s);

	if (value_p)
		{
			if (json_is_string (value_p))
				{
					value_s = json_string_value (value_p);
				}
		}

	return value_s;
}


int PrintJSONObject (FILE *out_f, const json_t * const json_p, const char * const prefix_s)
{
	int result = 0;
	char *json_s = NULL;

	if (prefix_s)
		{
			fprintf (out_f, "%s", prefix_s);
		}

	if (json_p)
		{
			json_s = json_dumps (json_p, JSON_INDENT (2));

			if (json_s)
				{
					fprintf (out_f, "%s\n", json_s);
					free (json_s);
				}
			else
				{
					fprintf (out_f, "ERROR DUMPING!!\n");
					result = -1;
				}
		}
	else
		{
			fprintf (out_f, "NULL JSON OBJECT!!\n");
		}


	return result;
}

