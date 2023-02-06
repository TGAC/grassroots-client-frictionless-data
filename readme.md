# Grassroots Frictionless Data Tool

This is a command-line tool to extract the resources within a [Frictionless Data Package](https://frictionlessdata.io/) into a variety of formats such as Markdown, HTML, CSV, *etc.* It will be available for as many different platforms as possible. It uses the schemas for each resource within the Data Package
to generate the reports. It has in-built support for tabular-data-resources and will download and parse any web-based schemas from the resource profiles and use these when they are specified. It will output a file for each Data Resource within the Data Package.

## Installation

The *grassroots_fd_tool* package can be downloaded from [https://github.com/TGAC/grassroots-client-frictionless-data/releases/](https://github.com/TGAC/grassroots-client-frictionless-data/releases/). Currently there are versions for Linux and Windows with more platforms to follow. For Linux, you will also need to install libcurl which you can do with  

```
sudo apt install libcurl4-openssl-dev
```

or using a similar method depending upon your distribution.


## Usage

Currently this is a shell-based tool so needs to be run in a Power Shell or DOS prompt on Windows or in a terminal on Linux. To run it type `grassroots_fd_tool` with the following command line parameters:

 * **--in** \<filename\>: The Frictionless Data Package filename to extract the resources from.
 * **--out-dir** \<directory\>: The directory where the output files will be written to.
 * **--data-fmt** \<format\>: The format to write data resources in. Currently the options are:
    * **html**: Write the files in HTML format (default)
    * **markdown**: Write the files in Markdown format
 * **--table-fmt** \<format\>: The format to write tabular data resources in. Currently the options are:
    * **csv**: Write the files in csv format (default)
 * **--full**: If this is set, all key-value pairs are generated even when the values are missing. By
default, any key-value pairs where the values are not set will not be added to the output files.
 * **--ver**: Display the version information.

On Linux, you need to make sure that the required libraries are in the runtime library search path. You can so this using the enclosed `run_grassroots_frictionless_data_tool.sh` within the archive. Alternatively, you can type 

```
cd <path to where you extracted the grassroots_fd_tool files> 
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:fd_libs
```

in a terminal and then you can use `grassroots_fd_tool` as above.


## History

 * 0.9.1 (2022/01/25): Fixed error messages saying "failed to parse" for child schemas when thay had actually succeeded
 * 0.9.0 (2021/05/25): Initial version
