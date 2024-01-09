Initial data
The initial data is located into a data structure consisting of arrays of pointers, headers and items.
Declarations of items as C / C++ structs are presented in file Items.h. There are 10 different types of items
(ITEM1, ITEM2, …, ITEM10). Declarations of headers as C / C++ structs are presented in file Headers.h.
There are 5 different types of headers (HEADER_A, HEADER_B, HEADER_C, HEADER_D, HEADER_E). The
both files are stored in Instructor's stuff.
There are 5 different types of data structures (Struct1, Struct2, Struct3, Struct4, Struct5). To generate the
initial data structure you have to use functions from ICS0017DataSource.dll. This DLL is implemented by
instructor and stored in Instructor's stuff. It needs auxiliary file Colors.txt, created from
https://en.m.wikipedia.org/wiki/Lists_of_colors.
To understand the building principles of our data structures analyse the examples on the following pages.
Let us emphasize that they are just examples: the actual presence and absence of items and headers is
determined by the work of item generator built into ICS0017DataSource.dll and is largerly occasional.
The DLL imports 6 public functions declared in file ICS0017DataSource.h (also stored in Instructor's stuff).
Five of them create data structure and return the pointer to it. The sixth function (GetItem()) constructs a
stand-alone item and returns the pointer to it. There is also a password-protected function for the
instructor. Comments explaining the usage of public functions are in ICS0017DataSource.h.
To know which item and data structure you have to use see the table.
