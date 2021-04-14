/*
 * markdown_printer.c
 *
 *  Created on: 13 Apr 2021
 *      Author: billy
 */


#include <stdlib.h>

#include "markdown_printer.h"

#include "fd_json_util.h"

#include "frictionless_data_util.h"


typedef struct
{
	Printer mp_printer;
} MarkdownPrinter;


/*
 * static declarations
 */

static bool PrintMarkdownHeader (Printer *printer_p, const char *title_s, const char *text_s);

static bool PrintMarkdownFooter (Printer *printer_p, const char *text_s);


static bool PrintMarkdownString (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s);

static bool PrintMarkdownInteger (Printer *printer_p, const char *key_s, const json_int_t *value_p, const bool required_flag, const char *format_s);

static bool PrintMarkdownNumber (Printer *printer_p, const char *key_s, const double *value_p, const bool required_flag, const char *format_s);

static bool PrintMarkdownBoolean (Printer *printer_p, const char *key_s, const bool *value_p, const bool required_flag, const char *format_s);

static bool PrintMarkdownJSON (Printer *printer_p, const char *key_s, const json_t *value_p, const bool required_flag, const char *format_s);

static void FreeMarkdownPrinter (Printer *printer_p);

static bool PrintEmptyMarkdownValue (Printer *printer_p, const char *key_s);


/*
 * api definitions
 */


Printer *AllocateMarkdownPrinter (void)
{
	MarkdownPrinter *printer_p = (MarkdownPrinter *) malloc (sizeof (MarkdownPrinter));

	if (printer_p)
		{
			InitFDPrinter (& (printer_p -> mp_printer), PrintMarkdownHeader, PrintMarkdownFooter, PrintMarkdownString,
									 PrintMarkdownInteger, PrintMarkdownNumber, PrintMarkdownBoolean, PrintMarkdownJSON,  FreeMarkdownPrinter);

			return (& (printer_p -> mp_printer));
		}

	return NULL;
}


/*
 * static definitions
 */


static bool PrintMarkdownString (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s)
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
							success_flag = (fprintf (printer_p -> pr_out_f, " * **%s**%s: [%s](%s)\n", key_s, req_s, value_s, value_s) > 0);
							printed_flag = true;
						}
					else if (strcmp (format_s, FD_TYPE_STRING_FORMAT_EMAIL) == 0)
						{
							success_flag = (fprintf (printer_p -> pr_out_f, " * **%s**%s: [%s](mailto:%s)\n", key_s, req_s, value_s, value_s) > 0);
							printed_flag = true;
						}
				}

			if (!printed_flag)
				{
					success_flag = (fprintf (printer_p -> pr_out_f, " * **%s**%s: %s\n", key_s, req_s, value_s) > 0);
				}

		}		/* if (value_s) */
	else
		{
			success_flag = PrintEmptyMarkdownValue (printer_p, key_s);
		}


	return success_flag;
}



static bool PrintMarkdownInteger (Printer *printer_p, const char *key_s, const json_int_t *value_p, const bool required_flag, const char *format_s)
{
	bool res;

	if (value_p)
		{
			res = (fprintf (printer_p -> pr_out_f, " * **%s**: %" JSON_INTEGER_FORMAT "\n", key_s, *value_p) > 0);
		}
	else
		{
			res = PrintEmptyMarkdownValue (printer_p, key_s);
		}

	return res;
}


static bool PrintMarkdownNumber (Printer *printer_p, const char *key_s, const double *value_p, const bool required_flag, const char *format_s)
{
	bool res;

	if (value_p)
		{
			res = (fprintf (printer_p -> pr_out_f, " * **%s**: %lf\n", key_s, *value_p) > 0);
		}
	else
		{
			res = PrintEmptyMarkdownValue (printer_p, key_s);
		}

	return res;
}


static bool PrintMarkdownBoolean (Printer *printer_p, const char *key_s, const bool *value_p, const bool required_flag, const char *format_s)
{
	bool res;

	if (value_p)
		{
			res = (fprintf (printer_p -> pr_out_f, " * **%s**: %s\n", key_s, *value_p ? "true" : "false") > 0);
		}
	else
		{
			res = PrintEmptyMarkdownValue (printer_p, key_s);
		}

	return res;
}


static bool PrintMarkdownJSON (Printer *printer_p, const char *key_s, const json_t *value_p, const bool required_flag, const char *format_s)
{
	bool success_flag = false;

	if (value_p)
		{
			char *json_s = json_dumps (value_p, JSON_INDENT (2));

			if (json_s)
				{
					success_flag = (fprintf (printer_p -> pr_out_f, " * **%s**: ```json{%s}\n", key_s, json_s) > 0);

					free (json_s);
				}		/* if (json_s) */
		}
	else
		{
			success_flag = PrintEmptyMarkdownValue (printer_p, key_s);
		}

	return success_flag;
}


static bool PrintEmptyMarkdownValue (Printer *printer_p, const char *key_s)
{
	return (fprintf (printer_p -> pr_out_f, " * **%s**:\n", key_s) > 0);
}


static void FreeMarkdownPrinter (Printer *printer_p)
{
	MarkdownPrinter *markdown_printer_p = (MarkdownPrinter *) printer_p;

//ClosePrinter (printer_p);

	free (markdown_printer_p);
}


static bool PrintMarkdownHeader (Printer *printer_p, const char *title_s, const char *text_s)
{
	bool res;

	if (text_s)
		{
			res = (fprintf (printer_p -> pr_out_f, "# %s\n\n %s", title_s, text_s) > 0);
		}
	else
		{
			res = (fprintf (printer_p -> pr_out_f, "# %s\n\n", title_s) > 0);
		}

	return res;
}


static bool PrintMarkdownFooter (Printer *printer_p, const char *value_s)
{
	return true;
}
