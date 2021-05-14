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

#include "html_printer.h"

#include "json_util.h"

#include "frictionless_data_util.h"

#include "jansson.h"


typedef struct
{
	Printer hp_printer;
} HTMLPrinter;


/*
 * static declarations
 */

static bool PrintHTMLHeader (Printer *printer_p, const char *title_s, const char *text_s);

static bool PrintHTMLFooter (Printer *printer_p, const char *text_s);

static bool PrintHTMLText (Printer *printer_p, const char *value_s);

static bool PrintHTMLString (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s);

static bool PrintHTMLInteger (Printer *printer_p, const char *key_s, const json_int_t *value_p, const bool required_flag, const char *format_s);

static bool PrintHTMLNumber (Printer *printer_p, const char *key_s, const double *value_p, const bool required_flag, const char *format_s);

static bool PrintHTMLBoolean (Printer *printer_p, const char *key_s, const bool *value_p, const bool required_flag, const char *format_s);

static bool PrintHTMLJSON (Printer *printer_p, const char *key_s, const json_t *value_p, const bool required_flag, const char *format_s);

static void FreeHTMLPrinter (Printer *printer_p);

static bool PrintEmptyHTMLValue (Printer *printer_p, const char *key_s);



static bool PrintHTMLSectionStart (Printer *printer_p, const char *value_s);

static bool PrintHTMLSectionEnd (Printer *printer_p, const char *value_s);

/*
 * api definitions
 */


Printer *AllocateHTMLPrinter (void)
{
	HTMLPrinter *printer_p = (HTMLPrinter *) malloc (sizeof (HTMLPrinter));

	if (printer_p)
		{
			InitFDPrinter (& (printer_p -> hp_printer), PrintHTMLHeader, PrintHTMLFooter, PrintHTMLText, PrintHTMLSectionStart,
										 PrintHTMLSectionEnd, PrintHTMLString,
									 PrintHTMLInteger, PrintHTMLNumber, PrintHTMLBoolean, PrintHTMLJSON,  FreeHTMLPrinter);

			return (& (printer_p -> hp_printer));
		}

	return NULL;
}


/*
 * static definitions
 */


static bool PrintHTMLString (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s)
{
	bool success_flag = false;
	const char *req_s = "";

	if (required_flag)
		{
			req_s = " *";
		}


	if (value_s)
		{
			bool printed_flag = false;

			if (format_s)
				{
					if (strcmp (format_s, FD_TYPE_STRING_FORMAT_URI) == 0)
						{
							success_flag = (fprintf (printer_p -> pr_out_f, "<li><strong>%s%s</strong>: <a href =\"%s\">%s</a></li>\n", key_s, req_s, value_s, value_s) > 0);
							printed_flag = true;
						}
					else if (strcmp (format_s, FD_TYPE_STRING_FORMAT_EMAIL) == 0)
						{
							success_flag = (fprintf (printer_p -> pr_out_f, "<li><strong>%s%s</strong>: <a href =\"mailto:%s\">%s</a></li>\n", key_s, req_s, value_s, value_s) > 0);
							printed_flag = true;
						}
				}

			if (!printed_flag)
				{
					success_flag = (fprintf (printer_p -> pr_out_f, "<li><strong>%s%s</strong>: %s</li>\n", key_s, req_s, value_s) > 0);
				}

		}		/* if (value_s) */
	else
		{
			success_flag = PrintEmptyHTMLValue (printer_p, key_s);
		}




	return success_flag;
}



static bool PrintHTMLInteger (Printer *printer_p, const char *key_s, const json_int_t *value_p, const bool required_flag, const char *format_s)
{
	bool res;

	if (value_p)
		{
			res = (fprintf (printer_p -> pr_out_f, "<li><strong>%s</strong>: %" JSON_INTEGER_FORMAT "</li>\n", key_s, *value_p) > 0);
		}
	else
		{
			res = PrintEmptyHTMLValue (printer_p, key_s);
		}

	return res;
}


static bool PrintHTMLNumber (Printer *printer_p, const char *key_s, const double *value_p, const bool required_flag, const char *format_s)
{
	bool res;

	if (value_p)
		{
			res = (fprintf (printer_p -> pr_out_f, "<li><strong>%s</strong>: %lf</li>\n", key_s, *value_p) > 0);
		}
	else
		{
			res = PrintEmptyHTMLValue (printer_p, key_s);
		}

	return res;
}


static bool PrintHTMLBoolean (Printer *printer_p, const char *key_s, const bool *value_p, const bool required_flag, const char *format_s)
{
	bool res;

	if (value_p)
		{
			res = (fprintf (printer_p -> pr_out_f, "<li><strong>%s</strong>: %s</li>\n", key_s, *value_p ? "true" : "false") > 0);
		}
	else
		{
			res = PrintEmptyHTMLValue (printer_p, key_s);
		}

	return res;
}


static bool PrintHTMLJSON (Printer *printer_p, const char *key_s, const json_t *value_p, const bool required_flag, const char *format_s)
{
	bool success_flag = false;

	if (value_p)
		{
			char *json_s = json_dumps (value_p, JSON_INDENT (2));

			if (json_s)
				{
					success_flag = (fprintf (printer_p -> pr_out_f, "<li><strong>%s</strong>: %s</li>\n", key_s, json_s) > 0);

					free (json_s);
				}		/* if (json_s) */
		}
	else
		{
			success_flag = PrintEmptyHTMLValue (printer_p, key_s);
		}

	return success_flag;
}


static bool PrintEmptyHTMLValue (Printer *printer_p, const char *key_s)
{
	return (fprintf (printer_p -> pr_out_f, "<li><strong>%s</strong>: </li>\n", key_s) > 0);
}


static void FreeHTMLPrinter (Printer *printer_p)
{
	HTMLPrinter *html_printer_p = (HTMLPrinter *) printer_p;

//ClosePrinter (printer_p);

	free (html_printer_p);
}


static bool PrintHTMLHeader (Printer *printer_p, const char *title_s, const char *text_s)
{
	bool res;

	if (text_s)
		{
			res = (fprintf (printer_p -> pr_out_f, "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<title>%s</title>\n</head>\n<body><h1>%s</h1>\n<section>%s\n<ul>\n", title_s, title_s, text_s) > 0);
		}
	else
		{
			res = (fprintf (printer_p -> pr_out_f, "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<title>%s</title>\n</head>\n<body><h1>%s</h1>\n<section>\n<ul>\n", title_s, title_s) > 0);
		}

	return res;
}


static bool PrintHTMLFooter (Printer *printer_p, const char *value_s)
{
	bool res;

	if (value_s)
		{
			res = (fprintf (printer_p -> pr_out_f, "</ul>\n</section>\n<footer>\n%s\n</footer>\n</body>\n</html>\n", value_s) > 0);
		}
	else
		{
			res = (fprintf (printer_p -> pr_out_f, "</ul>\n</section></body>\n</html>\n") > 0);
		}

	return res;
}


static bool PrintHTMLText (Printer *printer_p, const char *value_s)
{
	bool res = (fprintf (printer_p -> pr_out_f, "<p>%s</p>\n", value_s) > 0);

	return res;
}


static bool PrintHTMLSectionStart (Printer *printer_p, const char *value_s)
{
	bool res = (fprintf (printer_p -> pr_out_f, "<section><h2>%s</h2>\n", value_s) > 0);

	return res;
}

static bool PrintHTMLSectionEnd (Printer *printer_p, const char *value_s)
{
	bool res;

	if (value_s)
		{
			res = (fprintf (printer_p -> pr_out_f, "%s</section>\n", value_s) > 0);
		}
	else
		{
			res = (fprintf (printer_p -> pr_out_f, "</section>\n") > 0);
		}

	return res;
}

