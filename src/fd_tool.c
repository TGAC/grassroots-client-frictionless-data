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
#include "fd_string_util.h"
#include "fd_filesystem_util.h"

#include "html_printer.h"
#include "markdown_printer.h"


typedef struct
{
	const char *jp_key_s;
	const json_t *jp_value_p;
} JSONProperty;


/*
 * static declarations
 */

static bool CreateCSVFile (const char *filename_s, const char *col_sep_s, const char *row_sep_s, const json_t *headers_p, const json_t *data_p);



static int SortPropertiesByOrder (const void *v0_p, const void *v1_p);

static bool ParsePackageFromSchema (const json_t *data_p, const json_t *schema_p, Printer *printer_p, const bool full_flag);



/*
 * api definitions
 */

int main (int argc, char *argv [])
{
	int res = 0;

	if (argc < 3)
		{
			printf (
					"USAGE: grassroots-fd-tool\n"
					"\t--in <filename>, the Frictionless Data Package filename to extract the resources from.\n"
					"\t--out-dir <directory>, the directory where the output files will be written to.\n"
					"\t--data-fmt <format>, the format to write data resources in. Currently the options are:\n"
					"\t\thtml, write the files in html format (default).\n"
					"\t\md, write the files in markdown format.\n"
					"\t--table-fmt <format>, the format to write data resources in. Currently the options are:\n"
					"\t\tcsv, write the files in csv format (default).\n"
					"\t--full, show all properties even when the values are empty"
					);

		}		/* if (argc < 3) */
	else
		{
			int i = 1;
			const char *fd_file_s = NULL;
			const char *out_dir_s = NULL;
			const char *table_format_s = "csv";
			bool full_flag = false;

			typedef enum
			{
				PRINTER_FORMAT_HTML,
				PRINTER_FORMAT_MARKDOWN
			} PrinterFormat;

			PrinterFormat data_format = PRINTER_FORMAT_HTML;
			const char *data_format_s = "html";

			while (i < argc)
				{
					if (strcmp (argv [i], "--in") == 0)
						{
							if ((i + 1) < argc)
								{
									fd_file_s = argv [++ i];
								}
							else
								{
									printf ("input argument missing");
								}
						}
					else if (strcmp (argv [i], "--out-dir") == 0)
						{
							if ((i + 1) < argc)
								{
									out_dir_s = argv [++ i];
								}
							else
								{
									printf ("output directory argument missing");
								}
						}
					else if (strcmp (argv [i], "--data-fmt") == 0)
						{
							if ((i + 1) < argc)
								{
									const char *format_s = argv [++ i];

									if (strcmp (format_s, "html") == 0)
										{
											data_format = PRINTER_FORMAT_HTML;
										}
									else if (strcmp (format_s, "markdown") == 0)
										{
											data_format = PRINTER_FORMAT_MARKDOWN;
										}
									else
										{
											printf ("Unknown data format: \"%s\"\n", format_s);
										}

								}
							else
								{
									printf ("data format argument missing");
								}
						}
					else if (strcmp (argv [i], "--table-fmt") == 0)
						{
							if ((i + 1) < argc)
								{
									table_format_s = argv [++ i];
								}
							else
								{
									printf ("table format argument missing");
								}
						}
					else if (strcmp (argv [i], "--full") == 0)
						{
							full_flag = true;
						}
					else
						{
							printf ("Unknown argument: \"%s\"", argv [i]);
						}

					++ i;
				}

			if (fd_file_s)
				{
				  Printer *printer_p = NULL;
					const char *data_ext_s = NULL;

				  switch (data_format)
						{
							case PRINTER_FORMAT_HTML:
								{
									printer_p = AllocateHTMLPrinter ();
									data_ext_s = "html";
								}
								break;

							case PRINTER_FORMAT_MARKDOWN:
								{
									printer_p = AllocateMarkdownPrinter ();
									data_ext_s = "md";
								}
								break;
						}


				  if (printer_p)
				  	{
				  		json_error_t err;
				  	  json_t *fd_p = json_load_file (fd_file_s, 0, &err);

				  	  if (fd_p)
				  	  	{
				  	  		const json_t *resources_p = json_object_get (fd_p, FD_RESOURCES_S);

				  	  		if (resources_p)
				  	  			{
				  	  				size_t j;
				  	  				const json_t *resource_p;

				  	  				json_array_foreach (resources_p, j, resource_p)
				  							{

				  	  						const char *profile_s = GetJSONString (resource_p, FD_PROFILE_S);

				  	  						if (profile_s)
				  	  							{
						  	  						const char *name_s = GetJSONString (resource_p, FD_NAME_S);
						  	  						char *filename_s = NULL;


				  	  								if (DoesStringStartWith (profile_s, "http"))
				  	  									{
				  	  										json_t *schema_p = GetWebJSON (profile_s);

				  	  										if (schema_p)
				  	  											{
				  					  	  						if (name_s)
				  					  	  							{
				  					  	  								filename_s = GetOutputFilename (out_dir_s, name_s, data_ext_s);
				  					  	  							}		/* if (name_s) */
				  					  	  						else
				  					  	  							{
				  					  	  								char *temp_s = ConvertSizeTToString (j);

				  					  	  								if (temp_s)
				  					  	  									{
				  					  	  										filename_s = GetOutputFilename (out_dir_s, temp_s, data_ext_s);
				  					  	  										FreeCopiedString (temp_s);
				  					  	  									}

				  					  	  							}

				  					  	  						if (filename_s)
				  					  	  							{
				  					  	  								if (OpenFDPrinter (printer_p, filename_s))
				  					  	  									{
				  					  	  										PrintHeader (printer_p, name_s, NULL);
								  	  												ParsePackageFromSchema (resource_p, schema_p, printer_p, full_flag);


				  					  	  										PrintFooter (printer_p, profile_s);
				  					  	  										CloseFDPrinter (printer_p);
				  					  	  									}		/* if (OpenPrinter (printer_p, filename_s)) */


				  							  	  						FreeCopiedString (filename_s);
				  					  	  							}		/* if (filename_s) */

				  	  											}		/* if (schema_p) */
				  	  									}
				  	  								else if (strcmp (profile_s, FD_PROFILE_TABULAR_RESOURCE_S) == 0)
				  	  									{
						  	  								const json_t *schema_p = json_object_get (resource_p, FD_SCHEMA_S);
				  	  										const json_t *data_p = json_object_get (resource_p, FD_DATA_S);
				  	  										const json_t *headers_p = NULL;

				  	  										if (schema_p)
				  	  											{
				  	  												headers_p = json_object_get (schema_p, FD_TABLE_FIELDS_S);
				  	  											}

				  	  										if (data_p)
				  	  											{
				  	  												const char *col_sep_s = ",";
				  	  												const char *row_sep_s = "\n";

				  					  	  						if (name_s)
				  					  	  							{
				  					  	  								filename_s = GetOutputFilename (out_dir_s, name_s, table_format_s);
				  					  	  							}		/* if (name_s) */
				  					  	  						else
				  					  	  							{
				  					  	  								char *temp_s = ConvertSizeTToString (j);

				  					  	  								if (temp_s)
				  					  	  									{
				  					  	  										filename_s = GetOutputFilename (out_dir_s, temp_s, table_format_s);
				  					  	  										FreeCopiedString (temp_s);
				  					  	  									}

				  					  	  							}

				  					  	  						if (filename_s)
				  					  	  							{
						  	  												if (CreateCSVFile (filename_s, col_sep_s, row_sep_s, headers_p, data_p))
						  	  													{

						  	  													}

				  							  	  						FreeCopiedString (filename_s);
				  					  	  							}		/* if (filename_s) */


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


				  		FreeFDPrinter (printer_p);
				  	}		/* if (printer_p) */

				}		/* if (fd_file_s) */

		}		/* if (argc < 3) else */

  return res;
}


/*
 * static definitions
 */



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
							const char *header_s = GetJSONString (header_p, FD_TABLE_FIELD_NAME);

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
											const char *header_s = GetJSONString (header_p, FD_TABLE_FIELD_NAME);

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




static bool ParsePackageFromSchema (const json_t *data_p, const json_t *schema_p, Printer *printer_p, const bool full_flag)
{
	bool result = false;
	const json_t *required_entries_p = json_object_get (schema_p, "required");
	const json_t *properties_p = json_object_get (schema_p, "properties");

	if (properties_p)
		{

			/*
			 * Get the properties sorted by their propertyOrder values
			 */
			const size_t num_properties = json_object_size (properties_p);
			JSONProperty *sorted_properties_p = calloc (num_properties, sizeof (JSONProperty));

			if (sorted_properties_p)
				{
					const char *key_s;
					json_t *value_p;
					JSONProperty *sorted_property_p = sorted_properties_p;
					size_t i = num_properties;

					json_object_foreach (properties_p, key_s, value_p)
						{
							sorted_property_p -> jp_key_s = key_s;
							sorted_property_p -> jp_value_p = value_p;

							++ sorted_property_p;
						}		/* json_object_foreach (properties_p, key_s, value_p) */

					/*
					 * Sort the keys into order
					 */
					qsort (sorted_properties_p, num_properties, sizeof (JSONProperty), SortPropertiesByOrder);

					/*
					 * Now read in the values in order
					 */
					for (sorted_property_p = sorted_properties_p; i > 0; -- i, ++ sorted_property_p)
						{
							const json_t *property_p = sorted_property_p -> jp_value_p;
							const char *type_s;

							key_s = sorted_property_p -> jp_key_s;

							type_s = GetJSONString (property_p, FD_TABLE_FIELD_TYPE);

							if (type_s)
								{
									const char *format_s = GetJSONString (property_p, FD_TABLE_FIELD_FORMAT);
									bool required_flag = false;

									if (required_entries_p)
										{
											size_t num_required_entries = json_array_size (required_entries_p);
											size_t j = 0;

											for (j = 0; j < num_required_entries; ++ j)
												{
													json_t *entry_p = json_array_get (required_entries_p, j);

													if (json_is_string (entry_p))
														{
															const char *req_s = json_string_value (entry_p);

															if (strcmp (key_s, req_s) == 0)
																{
																	required_flag = true;
																	j = num_required_entries;
																}

														}
												}

										}


									if (strcmp (type_s, FD_TYPE_STRING) == 0)
										{
											const char *value_s = GetJSONString (data_p, key_s);

											if (value_s || full_flag)
												{
													/*
													 * profiles may be a url so check for this
													 */
													if ((strcmp (key_s, FD_PROFILE_S) == 0) && (DoesStringStartWith (value_s, "http")))
														{
															format_s = FD_TYPE_STRING_FORMAT_URI;
														}

													PrintString (printer_p, key_s, value_s, required_flag, format_s);
												}

										}		/* if (strcmp (type_s, FD_TYPE_STRING) == 0) */
									else if (strcmp (type_s, FD_TYPE_INTEGER) == 0)
										{
											bool print_flag = full_flag;
											json_int_t value;
											json_int_t *int_value_p = NULL;

											if (GetJSONInteger (data_p, key_s, &value))
												{
													int_value_p = &value;
													print_flag = true;
												}

											if (print_flag)
												{
													PrintInteger (printer_p, key_s, int_value_p, required_flag, format_s);
												}

										}		/* else if (strcmp (type_s, FD_TYPE_INTEGER) == 0) */
									else if (strcmp (type_s, FD_TYPE_NUMBER) == 0)
										{
											bool print_flag = full_flag;
											double value;
											double *number_value_p = NULL;

											if (GetJSONNumber (data_p, key_s, &value))
												{
													number_value_p = &value;
													print_flag = true;
												}

											if (print_flag)
												{
													PrintNumber (printer_p, key_s, number_value_p, required_flag, format_s);
												}

										}		/* else if (strcmp (type_s, FD_TYPE_NUMBER) == 0) */
									else if (strcmp (type_s, FD_TYPE_BOOLEAN) == 0)
										{
											bool print_flag = full_flag;
											bool value;
											bool *bool_value_p = NULL;

											if (GetJSONBoolean (data_p, key_s, &value))
												{
													bool_value_p = &value;
													print_flag = true;
												}

											if (print_flag)
												{
													PrintBoolean (printer_p, key_s, bool_value_p, required_flag, format_s);
												}

										}		/* else if (strcmp (type_s, FD_TYPE_BOOLEAN) == 0) */
									else if (strcmp (type_s, FD_TYPE_JSON_ARRAY) == 0)
										{
											/*
											 * Do we have a schema?
											 */


										}		/* else if (strcmp (type_s, FD_TYPE_JSON_ARRAY) == 0) */

								}		/* if (type_s) */

						}		/* for (sorted_key_pp = sorted_keys_pp; i > 0; -- i, ++ sorted_key_pp) */

					free (sorted_properties_p);
				}		/* if ((sorted_keys_pp) */

		}		/* if (properties_p) */

	return result;
}


static int SortPropertiesByOrder (const void *v0_p, const void *v1_p)
{
	int res = 0;
	const JSONProperty *json_0_p = (const JSONProperty *) v0_p;
	const JSONProperty *json_1_p = (const JSONProperty *) v1_p;

	json_int_t prop_order_0;

	if (GetJSONInteger (json_0_p -> jp_value_p, FD_PROFILE_PROPERTY_ORDER_S, &prop_order_0))
		{
			json_int_t prop_order_1;

			if (GetJSONInteger (json_1_p -> jp_value_p, FD_PROFILE_PROPERTY_ORDER_S, &prop_order_1))
				{
					res = prop_order_0 - prop_order_1;
				}

		}

	return res;
}
