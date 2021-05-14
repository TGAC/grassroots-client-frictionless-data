/*
 * printer.c
 *
 *  Created on: 7 Apr 2021
 *      Author: billy
 */


#include "printer.h"



void InitPrinter (Printer *printer_p,
									bool (*print_header_fn) (Printer *printer_p, const char *title_s, const char *text_s),
									bool (*print_footer_fn) (Printer *printer_p, const char *text_s),
									bool (*print_string_fn) (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s),
									bool (*print_integer_fn) (Printer *printer_p, const char *key_s, const json_int_t *value_p, const bool required_flag, const char *format_s),
									bool (*print_number_fn) (Printer *printer_p, const char *key_s, const double *value_p, const bool required_flag, const char *format_s),
									bool (*print_boolean_fn) (Printer *printer_p, const char *key_s, const bool *value_p, const bool required_flag, const char *format_s),
									bool (*print_json_fn) (Printer *printer_p, const char *key_s, const json_t *value_p, const bool required_flag, const char *format_s),
									void (*free_fn) (Printer *printer_p))
{
	printer_p -> pr_out_f = NULL;

	printer_p -> pr_print_header_fn = print_header_fn;
	printer_p -> pr_print_footer_fn = print_footer_fn;

	printer_p -> pr_print_string_fn = print_string_fn;
	printer_p -> pr_print_integer_fn = print_integer_fn;
	printer_p -> pr_print_number_fn = print_number_fn;
	printer_p -> pr_print_boolean_fn = print_boolean_fn;
	printer_p -> pr_print_json_fn = print_json_fn;
	printer_p -> pr_free_fn = free_fn;
}


bool OpenFDPrinter (Printer *printer_p, const char *filename_s)
{
	bool success_flag = false;

	if (CloseFDPrinter (printer_p))
		{
			printer_p -> pr_out_f = fopen (filename_s, "w");

			if (printer_p -> pr_out_f)
				{
					success_flag = true;
				}
		}

	return success_flag;
}


bool CloseFDPrinter (Printer *printer_p)
{
	bool success_flag = true;

	if (printer_p -> pr_out_f)
		{
			int res = fclose (printer_p -> pr_out_f);

			if (res != 0)
				{
					success_flag = false;
				}

			printer_p -> pr_out_f = NULL;
		}

	return success_flag;
}


bool PrintHeader (Printer *printer_p, const char *title_s, const char *text_s)
{
	return (printer_p -> pr_print_header_fn (printer_p, title_s, text_s));
}


bool PrintFooter (Printer *printer_p, const char *text_s)
{
	return (printer_p -> pr_print_footer_fn (printer_p, text_s));
}


bool PrintString (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s)
{
	return (printer_p -> pr_print_string_fn (printer_p, key_s, value_s, required_flag, format_s));
}


bool PrintInteger (Printer *printer_p, const char *key_s, const json_int_t *value_p, const bool required_flag, const char *format_s)
{
	return (printer_p -> pr_print_integer_fn (printer_p, key_s, value_p, required_flag, format_s));
}


bool PrintNumber (Printer *printer_p, const char *key_s, const double *value_p, const bool required_flag, const char *format_s)
{
	return (printer_p -> pr_print_number_fn (printer_p, key_s, value_p, required_flag, format_s));
}


bool PrintBoolean (Printer *printer_p, const char *key_s, const bool *value_p, const bool required_flag, const char *format_s)
{
	return (printer_p -> pr_print_boolean_fn (printer_p, key_s, value_p, required_flag, format_s));
}


bool PrintJSON (Printer *printer_p, const char *key_s, const json_t *value_p, const bool required_flag, const char *format_s)
{
	return (printer_p -> pr_print_json_fn (printer_p, key_s, value_p, required_flag, format_s));
}



void FreePrinter (Printer *printer_p)
{
	printer_p -> pr_free_fn (printer_p);
}
