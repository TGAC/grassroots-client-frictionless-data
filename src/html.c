/*
** Copyright 2014-2020 The Earlham Institute
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
 * html.c
 *
 *  Created on: 6 Apr 2021
 *      Author: billy
 */

#include <stdlib.h>

#include "html.h"

#include "fd_json_util.h"



typedef struct
{
	Printer hp_printer;

	FILE *hp_out_f;
} HTMLPrinter;


/*
 * static declarations
 */

static bool PrintHeader (FILE *out_f, const char *title_s);

static bool PrintFooter (FILE *out_f);


static bool PrintHTMLString (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s);
static bool PrintHTMLInteger (Printer *printer_p, const char *key_s, const json_int_t value, const bool required_flag, const char *format_s);
static bool PrintHTMLNumber (Printer *printer_p, const char *key_s, const double value, const bool required_flag, const char *format_s);
static void FreeHTMLPrinter (Printer *printer_p);


/*
 * api definitions
 */


Printer *AllocateHTMLPrinter (const char *output_filename_s)
{
	FILE *out_f = fopen (output_filename_s, "w");

	if (out_f)
		{
			if (PrintHeader (out_f, "NEED TO ADD TITLE"))
				{
					HTMLPrinter *printer_p = (HTMLPrinter *) malloc (sizeof (HTMLPrinter));

					if (printer_p)
						{
							printer_p -> hp_out_f = out_f;

							InitPrinter (& (printer_p -> hp_printer), PrintHTMLString, PrintHTMLInteger, PrintHTMLNumber, FreeHTMLPrinter);

							return (& (printer_p -> hp_printer));
						}

					fclose (out_f);

				}

		}

	return NULL;
}


bool PrintProgrammeToHTML (const json_t *programme_json_p, const char *output_filename_s)
{
	bool result = false;
	FILE *out_f = fopen (output_filename_s, "w");

	if (out_f)
		{
			const char *title_s = GetJSONString (programme_json_p, "name");

			if (title_s)
				{
					if (PrintHeader (out_f, title_s))
						{

							if (PrintFooter (out_f))
								{
									result = true;
								}		/* if (PrintFooter (out_f)) */

						}		/* if (PrintHeader (out_f, title_s)) */

				}		/* if (title_s) */

			fclose (out_f);
		}		/* if (out_f) */

	return result;
}


/*
 * static definitions
 */


static bool PrintHTMLString (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s)
{
	bool success_flag = false;
	HTMLPrinter *html_printer_p = (HTMLPrinter *) printer_p;

	if (required_flag)
		{
			success_flag = (fprintf (html_printer_p -> hp_out_f, "<li><strong>%s *</strong>: %s</li>\n", key_s, value_s) > 0);
		}
	else
		{
			success_flag = (fprintf (html_printer_p -> hp_out_f, "<li><strong>%s</strong>: %s</li>\n", key_s, value_s) > 0);
		}

	return success_flag;
}



static bool PrintHTMLInteger (Printer *printer_p, const char *key_s, const json_int_t value, const bool required_flag, const char *format_s)
{
	HTMLPrinter *html_printer_p = (HTMLPrinter *) printer_p;

	return (fprintf (html_printer_p -> hp_out_f, "<li><strong>%s</strong>: %" JSON_INTEGER_FORMAT "</li>\n", key_s, value) > 0);
}


static bool PrintHTMLNumber (Printer *printer_p, const char *key_s, const double value, const bool required_flag, const char *format_s)
{
	HTMLPrinter *html_printer_p = (HTMLPrinter *) printer_p;

	return (fprintf (html_printer_p -> hp_out_f, "<li><strong>%s</strong>: %lf</li>\n", key_s, value) > 0);
}


static void FreeHTMLPrinter (Printer *printer_p)
{
	HTMLPrinter *html_printer_p = (HTMLPrinter *) printer_p;

	if (html_printer_p -> hp_out_f)
		{
			PrintFooter (html_printer_p -> hp_out_f);

			fclose (html_printer_p -> hp_out_f);
		}


	free (html_printer_p);
}



static bool PrintProgrammeHTML (FILE *out_f, const json_t *programme_json_p)
{
	bool res = false;

	if (fprintf (out_f, "<ul>") > 0)
		{

			if (fprintf (out_f, "</ul>") > 0)
				{
					res = true;
				}		/* if (fprintf (out_f, "<ul>") > 0) */

		}		/* if (fprintf (out_f, "<ul>") > 0) */

	return res;
}


static bool PrintHeader (FILE *out_f, const char *title_s)
{
	bool res = (fprintf (out_f, "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<title>%s</title>\n</head><body><ul>\n", title_s) > 0);

	return res;
}


static bool PrintFooter (FILE *out_f)
{
	bool res = (fprintf (out_f, "</ul></body>\n</html>\n") > 0);

	return res;
}


static bool PrintListItem (FILE *out_f, const char *key_s, const char *value_s)
{
	bool res = (fprintf (out_f, "<li><strong>%s</strong>: %s</li>\n", key_s, value_s) > 0);

	return res;
}
