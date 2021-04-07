/*
 * printer.c
 *
 *  Created on: 7 Apr 2021
 *      Author: billy
 */


#include "printer.h"



void InitPrinter (Printer *printer_p,
									bool (*print_string_fn) (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s),
									bool (*print_integer_fn) (Printer *printer_p, const char *key_s, const json_int_t value, const bool required_flag, const char *format_s),
									bool (*print_number_fn) (Printer *printer_p, const char *key_s, const double value, const bool required_flag, const char *format_s),
									void (*free_fn) (Printer *printer_p))
{
	printer_p -> pr_print_string_fn = print_string_fn;
	printer_p -> pr_print_integer_fn = print_integer_fn;
	printer_p -> pr_print_number_fn = print_number_fn;
	printer_p -> pr_free_fn = free_fn;
}


bool PrintString (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s)
{
	return (printer_p -> pr_print_string_fn (printer_p, key_s, value_s, required_flag, format_s));
}


bool PrintInteger (Printer *printer_p, const char *key_s, const json_int_t value, const bool required_flag, const char *format_s)
{
	return (printer_p -> pr_print_integer_fn (printer_p, key_s, value, required_flag, format_s));
}


bool PrintNumber (Printer *printer_p, const char *key_s, const double value, const bool required_flag, const char *format_s)
{
	return (printer_p -> pr_print_number_fn (printer_p, key_s, value, required_flag, format_s));
}


void FreePrinter (Printer *printer_p)
{
	printer_p -> pr_free_fn (printer_p);
}
