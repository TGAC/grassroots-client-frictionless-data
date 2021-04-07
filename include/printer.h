/*
 * printer.h
 *
 *  Created on: 7 Apr 2021
 *      Author: billy
 */

#ifndef CLIENTS_FRICTIONLESS_DATA_INCLUDE_PRINTER_H_
#define CLIENTS_FRICTIONLESS_DATA_INCLUDE_PRINTER_H_

#include "jansson.h"

#include "typedefs.h"


typedef struct Printer Printer;

struct Printer
{
	bool (*pr_print_string_fn) (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s);
	bool (*pr_print_integer_fn) (Printer *printer_p, const char *key_s, const json_int_t value, const bool required_flag, const char *format_s);
	bool (*pr_print_number_fn) (Printer *printer_p, const char *key_s, const double value, const bool required_flag, const char *format_s);
	void (*pr_free_fn) (Printer *printer_p);
};


void InitPrinter (Printer *printer_p,
									bool (*print_string_fn) (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s),
									bool (*print_integer_fn) (Printer *printer_p, const char *key_s, const json_int_t value, const bool required_flag, const char *format_s),
									bool (*print_number_fn) (Printer *printer_p, const char *key_s, const double value, const bool required_flag, const char *format_s),
									void (*free_fn) (Printer *printer_p));


bool PrintString (Printer *printer_p, const char *key_s, const char *value_s, const bool required_flag, const char *format_s);

bool PrintInteger (Printer *printer_p, const char *key_s, const json_int_t value, const bool required_flag, const char *format_s);

bool PrintNumber (Printer *printer_p, const char *key_s, const double value, const bool required_flag, const char *format_s);

void FreePrinter (Printer *printer_p);

#endif /* CLIENTS_FRICTIONLESS_DATA_INCLUDE_PRINTER_H_ */
