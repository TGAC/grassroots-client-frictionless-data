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

static bool PrintHTMLHeader (Printer *printer_p, const char *title_s);

static bool PrintHTMLFooter (Printer *printer_p);


static bool PrintHTMLString (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s);

static bool PrintHTMLInteger (Printer *printer_p, const char *key_s, const json_int_t value, const bool required_flag, const char *format_s);

static bool PrintHTMLNumber (Printer *printer_p, const char *key_s, const double value, const bool required_flag, const char *format_s);

static bool PrintHTMLBoolean (Printer *printer_p, const char *key_s, const bool value, const bool required_flag, const char *format_s);

static bool PrintHTMLJSON (Printer *printer_p, const char *key_s, const json_t *value_p, const bool required_flag, const char *format_s);

static void FreeHTMLPrinter (Printer *printer_p);


/*
 * api definitions
 */


Printer *AllocateHTMLPrinter (void)
{
	HTMLPrinter *printer_p = (HTMLPrinter *) malloc (sizeof (HTMLPrinter));

	if (printer_p)
		{
			InitPrinter (& (printer_p -> hp_printer), PrintHTMLHeader, PrintHTMLFooter, PrintHTMLString,
									 PrintHTMLInteger, PrintHTMLNumber, PrintHTMLInteger, PrintHTMLNumber,  FreeHTMLPrinter);

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


static bool PrintHTMLBoolean (Printer *printer_p, const char *key_s, const bool value, const bool required_flag, const char *format_s)
{
	HTMLPrinter *html_printer_p = (HTMLPrinter *) printer_p;

	return (fprintf (html_printer_p -> hp_out_f, "<li><strong>%s</strong>: %s</li>\n", key_s, value ? "true" : "false") > 0);
}


static bool PrintHTMLJSON (Printer *printer_p, const char *key_s, const json_t *value_p, const bool required_flag, const char *format_s)
{
	HTMLPrinter *html_printer_p = (HTMLPrinter *) printer_p;
	bool success_flag = false;
	char *json_s = json_dumps (value_p, JSON_INDENT (2));

	if (json_s)
		{
			success_flag = (fprintf (html_printer_p -> hp_out_f, "<li><strong>%s</strong>: %s</li>\n", key_s, json_s) > 0);

			free (json_s);
		}		/* if (json_s) */

	return success_flag;
}


static void FreeHTMLPrinter (Printer *printer_p)
{
	HTMLPrinter *html_printer_p = (HTMLPrinter *) printer_p;

	if (printer_p -> pr_out_f)
		{
			ClosePrinter (printer_p);
		}

	free (html_printer_p);
}


static bool PrintHTMLHeader (Printer *printer_p, const char *title_s)
{
	bool res = (fprintf (printer_p -> pr_out_f, "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<title>%s</title>\n</head>\n<body>\n<ul>\n", title_s) > 0);

	return res;
}


static bool PrintHTMLFooter (Printer *printer_p)
{
	bool res = (fprintf (printer_p -> pr_out_f, "</ul>\n</body>\n</html>\n") > 0);

	return res;
}

