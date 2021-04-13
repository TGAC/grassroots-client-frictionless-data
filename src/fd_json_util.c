/*
 * json_util.c
 *
 *  Created on: 7 Apr 2021
 *      Author: billy
 */

#include "fd_json_util.h"
#include "fd_curl_util.h"
#include "fd_string_util.h"


static bool SetIntegerFromJSON (const json_t *json_p, int32 *value_p);


json_t *GetWebJSON (const char *url_s)
{
	json_t *data_p = NULL;
	char *data_s = GetWebData (url_s);

	if (data_s)
		{
			json_error_t err;
			data_p = json_loads (data_s, 0, &err);

			FreeCopiedString (data_s);
		}

	return data_p;
}



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



bool GetJSONInteger (const json_t *json_p, const char * const key_s, int *value_p)
{
	bool success_flag = false;
	json_t *json_value_p = json_object_get (json_p, key_s);

	if (json_value_p)
		{
			success_flag = SetIntegerFromJSON (json_value_p, value_p);
		}

	return success_flag;
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


static bool SetIntegerFromJSON (const json_t *json_p, int32 *value_p)
{
	bool success_flag = false;

	if (json_is_integer (json_p))
		{
			*value_p = json_integer_value (json_p);
			success_flag = true;
		}
	else
		{
			printf ("JSON value is of the wrong type, %d not integer", json_p -> type);
		}

	return success_flag;
}

