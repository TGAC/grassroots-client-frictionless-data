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


#include "printer.h"
#include "fd_json_util.h"

/*
 * static declarations
 */

static bool CreateCSVFile (const char *filename_s, const char *col_sep_s, const char *row_sep_s, const json_t *headers_p, const json_t *data_p);

static const char *GetChildJSONString (const json_t *entry_p, const char * const key_s);


static int SortPropertiesByOrder (const void *v0_p, const void *v1_p);
;

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
  								const char * const GRASSROOTS_STUDY_RESOURCE_S = "https://grassroots.tools/frictionless_data/profiles/study.json";
  								const char * const GRASSROOTS_PROGRAMME_RESOURCE_S = "https://grassroots.tools/frictionless_data/profiles/programme.json";
  								const char * const GRASSROOTS_TRIAL_RESOURCE_S = "https://grassroots.tools/frictionless_data/profiles/trial.json";

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
  								else if (strcmp (profile_s, GRASSROOTS_STUDY_RESOURCE_S))
  									{
  										/*
  										 * We have a Study
  										 */
  									}
  								else if (strcmp (profile_s, GRASSROOTS_PROGRAMME_RESOURCE_S))
  									{
  										/*
  										 * We have a Programme
  										 */
  									}
  								else if (strcmp (profile_s, GRASSROOTS_TRIAL_RESOURCE_S))
  									{
  										/*
  										 * We have a Trial
  										 */
  									}


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

													if (add_sep_flag)
														{
															fprintf (csv_f, "%s ", col_sep_s);
														}
													else
														{
															add_sep_flag = true;
														}


													if (value_p)
														{
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




static bool ParsePackageFromSchema (const json_t *data_p, const json_t *schema_p, Printer *printer_p)
{
	bool result = false;
	const json_t *required_entries_p = json_object_get (schema_p, "required");
	const json_t *properties_p = json_object_get (schema_p, "propeties_p");

	if (properties_p)
		{
			/*
			 * Get the properties sorted by their propertyOrder values
			 */
			const size_t num_properties = json_object_size (properties_p);
			const char **sorted_keys_pp = calloc (num_properties, sizeof (const char *));

			if (sorted_keys_pp)
				{
					const char *key_s;
					json_t *value_p;
					const char **sorted_key_pp = sorted_keys_pp;
					size_t i = num_properties;

					json_object_foreach (properties_p, key_s, value_p)
						{
							*sorted_key_pp = key_s;
							++ sorted_key_pp;
						}		/* json_object_foreach (properties_p, key_s, value_p) */

					/*
					 * Sort the keys into order
					 */
					qsort (sorted_keys_pp, num_properties, sizeof (const char *), SortPropertiesByOrder);

					/*
					 * Now read in the values in order
					 */
					for (sorted_key_pp = sorted_keys_pp; i > 0; -- i, ++ sorted_key_pp)
						{
							const json_t *property_p = json_object_get (properties_p, *sorted_key_pp);
							const char *type_s = GetJSONString (property_p, FD_TABLE_FIELD_TYPE);

							if (type_s)
								{
									if (strcmp (type_s, FD_TYPE_STRING) == 0)
										{

										}		/* if (strcmp (type_s, FD_TYPE_STRING) == 0) */
									else if (strcmp (type_s, FD_TYPE_INTEGER) == 0)
										{

										}		/* else if (strcmp (type_s, FD_TYPE_INTEGER) == 0) */
									else if (strcmp (type_s, FD_TYPE_NUMBER) == 0)
										{

										}		/* else if (strcmp (type_s, FD_TYPE_NUMBER) == 0) */
									else if (strcmp (type_s, FD_TYPE_BOOLEAN) == 0)
										{

										}		/* else if (strcmp (type_s, FD_TYPE_BOOLEAN) == 0) */
									else
										{

										}

								}		/* if (type_s) */

						}		/* for (sorted_key_pp = sorted_keys_pp; i > 0; -- i, ++ sorted_key_pp) */

					free (sorted_keys_pp);
				}		/* if ((sorted_keys_pp) */

		}		/* if (properties_p) */

	return result;
}


static int SortPropertiesByOrder (const void *v0_p, const void *v1_p)
{
	const char **key_0_ss = (const char **) v0_p;
	const char **key_1_ss = (const char **) v1_p;

	int res = strcmp (*key_0_ss, *key_1_ss);

	return res;
}


