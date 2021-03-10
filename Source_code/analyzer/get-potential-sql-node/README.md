WP-SQL-SINK
===========

Extracts strings and/or un/installs wrappers on SQL sinks (all query functions and methods)  for Wordpress (and any other PHP project)

===========
This code is made to extract string fragments from Wordpress, filter them to get possible SQL strings, install wrappers on SQL sinks and perform positive taint inference research, but can be used for a variety of purposes.

It parses a PHP project (a folder containing many levels of PHP files, parsed separately), either extracting strings from parsed files and storing them in text files (it can store fragments of wordpress plugins differently), or replacing certain functions with others (usually wrappers, defined in another file which is automatically included in the index.php file of the project).

It can also uninstall the wrappers, but the code will be a little changed, because the parser rewrites the code. (Only code style will differ, not its logic and semantics, e.g only whitespaces and tabs and enters).

