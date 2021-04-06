/*
** Copyright 2014-2021 The Earlham Institute
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
/*
 * program_jobs.c
 * 
 *  Created on: 03 Apr 2021
 *      Author: billy
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "jansson.h"

#include "typedefs.h"

#define ALLOCATE_FD_UTIL_TAGS (1)
#include "frictionless_data_util.h"


/*
 * static declarations
 */

static const char *GetJSONString (const json_t *json_p, const char * const key_s);

static bool CreateCSVFile (const char *filename_s, const char *col_sep_s, const char *row_sep_s, const json_t *headers_p, const json_t *data_p);

static const char *GetChildJSONString (const json_t *entry_p, const char * const key_s);

static int PrintJSONObject (FILE *out_f, const json_t * const json_p, const char * const prefix_s);


/*
 * api definitions
 */

int main (int argc, char *argv [])
{
  int ret = 0;
  const char *fd_file_s = argv [1];
  const char *csv_file_s = argv [2];
	json_error_t err;
  json_t *fd_p = json_load_file (fd_file_s, 0, &err);

  if (fd_p)
  	{
  		const json_t *resources_p = json_object_get (fd_p, FD_RESOURCES_S);

  		if (resources_p)
  			{
  				size_t i;
  				const json_t *resource_p;

  				json_array_foreach (resources_p, i, resource_p)
						{
  						const char *profile_s = GetJSONString (resource_p, FD_PROFILE_S);

  						if (profile_s)
  							{
  								if (strcmp (profile_s, FD_PROFILE_TABULAR_RESOURCE_S) == 0)
  									{
  										const json_t *headers_p = NULL;
  										const json_t *schema_p = json_object_get (resource_p, FD_SCHEMA_S);
  										const json_t *data_p = json_object_get (resource_p, FD_DATA_S);

  										if (schema_p)
  											{
  												headers_p = json_object_get (schema_p, FD_TABLE_FIELDS_S);
  											}

  										if (data_p)
  											{
  												const char *col_sep_s = ",";
  												const char *row_sep_s = "\n";

  												if (CreateCSVFile (csv_file_s, col_sep_s, row_sep_s, headers_p, data_p))
  													{

  													}

  											}



  									}		/* if (strcmp (profile_s, FD_PROFILE_TABULAR_RESOURCE_S) == 0) */

  							}		/* if (profile_s) */


						}		/* json_array_foreach (resources_p, i, resource_p) */

  			}		/* if (resources_p) */
  	  else
  	  	{
  	  		fprintf (stderr, "%s does not contain a resources array so nothing to do!\n", fd_file_s);
  	  	}

  		json_decref (fd_p);
  	}		/* if (fd_p) */
  else
  	{
  		fprintf (stderr, "Failed to load %s as a JSON file\n", fd_file_s);
  	}


  return ret;
}


/*
 * static definitions
 */

static const char *GetChildJSONString (const json_t *entry_p, const char * const key_s)
{
	const char *value_s = NULL;
	const json_t *header_name_p = json_object_get (entry_p, key_s);

	if (header_name_p)
		{
			if (json_is_string (header_name_p))
				{
					value_s = json_string_value (header_name_p);
				}
			else
				{
					PrintJSONObject (stderr, entry_p, "header for %s is not a string");
				}
		}

	return value_s;
}


static bool CreateCSVFile (const char *filename_s, const char *col_sep_s, const char *row_sep_s, const json_t *headers_p, const json_t *data_p)
{
	bool success_flag = false;
	FILE *csv_f = fopen (filename_s, "w");

	/* open the output file */
	if (csv_f)
		{
			if (headers_p)
				{
					/*
					 * write the column headers
					 */
					const size_t num_columns = json_array_size (headers_p);
					size_t i;

					success_flag = true;

					for (i = 0; i < num_columns; ++ i)
						{
							const json_t *header_p = json_array_get (headers_p, i);
							const char *header_s = GetChildJSONString (header_p, FD_TABLE_FIELD_NAME);

							if (header_s)
								{
									const char *sep_s;

									if (i == num_columns - 1)
										{
											sep_s = row_sep_s;
										}
									else
										{
											sep_s = col_sep_s;
										}

									fprintf (csv_f, "\"%s\"%s ", header_s, sep_s);
								}		/* if (header_s) */
							else
								{
									success_flag = false;
								}

						}		/* for (i = 0; i < num_columns; ++ i) */

					/*
					 * write the data in the same order as the headers
					 */
					if (success_flag)
						{
							const size_t num_rows = json_array_size (data_p);

							for (i = 0; i < num_rows; ++ i)
								{
									const json_t *row_p = json_array_get (data_p, i);
									size_t j;
									bool add_sep_flag = false;

									for (j = 0; j < num_columns; ++ j)
										{
											const json_t *header_p = json_array_get (headers_p, j);
											const char *header_s = GetChildJSONString (header_p, FD_TABLE_FIELD_NAME);

											if (header_s)
												{
													const json_t *value_p = json_object_get (row_p, header_s);

													if (value_p)
														{
															if (add_sep_flag)
																{
																	fprintf (csv_f, "%s ", col_sep_s);
																}
															else
																{
																	add_sep_flag = true;
																}

															if (json_is_string (value_p))
																{
																	const char *value_s = json_string_value (value_p);

																	if (value_s)
																		{
																			fprintf (csv_f, "\"%s\"", value_s);
																		}
																	else
																		{
																			fprintf (csv_f, " ");
																		}
																}
															else if (json_is_integer (value_p))
																{
																	const json_int_t value = json_integer_value (value_p);

																	fprintf (csv_f, "%" JSON_INTEGER_FORMAT, value);
																}
															else if (json_is_real (value_p))
																{
																	const double value = json_real_value (value_p);

																	fprintf (csv_f, "%lf", value);
																}
															else
																{
																	PrintJSONObject (stderr, value_p, "Unknown JSON type: ");
																}
														}

												}		/* if (header_s) */
											else
												{
													success_flag = false;
												}

										}		/* for (i = 0; i < num_columns; ++ i) */

									fprintf (csv_f, "%s", row_sep_s);
								}

						}

				}		/* if (headers_p) */
			else
				{
					/*
					 * no headers so just write out in an arbitrary order
					 */
				}

			fclose (csv_f);
		}		/* if (csv_f) */
	else
		{
			fprintf (stderr, "Failed to open CSV output file \"%s\"\n", filename_s);
		}

	return success_flag;
}



static const char *GetJSONString (const json_t *json_p, const char * const key_s)
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


static int PrintJSONObject (FILE *out_f, const json_t * const json_p, const char * const prefix_s)
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



