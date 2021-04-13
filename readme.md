# Grassroots Frictionless Data Tool

This is a command-line tool to extract the resources within a [Frictionless Data Package](https://frictionlessdata.io/) into a variety of formats such as Markdown, HTML, CSV, *etc.* It will be available for as many different platforms as possible. It uses the schemas for each resource within the Data Package
to generate the reports. It has in-built support for tabular-data-resources and can will get any web-based schemas from the resource profiles and use these when they are specified.
 
## Usage

To run it type `grassroots-fd-tool` with the following command line parameters:

 * **--in** \<filename\>: the Frictionless Data Package filename to extract the resources from.
 * **--out-dir** \<directory\>: the directory where the output files will be written to.
 * **--data-fmt** \<format\>: the format to write data resources in. Currently the options are:
    * **html**: write the files in html format (default)
 * **--table-fmt** \<format\>: the format to write tabular data resources in. Currently the options are:
    * **csv**: write the files in csv format (default)
 * **--full**: If this is set, all key-value pairs are generated even when the values are missing. By
default, any key-value pairs where the values are not set will not be added to the output files.


 
