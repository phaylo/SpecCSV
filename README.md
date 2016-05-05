# SpecCSV
command-line csv editor and inspector

SpecCSV is command-line tool built from the scratch to edit and inspect already existed CSV files, support for creating new CSV files will eventually  be added at a later time.

Currently, SpecCSV is focused in CSV files that are compatible  with R different functions like `read.csv()` and such.

SpecCSV include the following features:
* Reading contents from existing  CSV files
* Editing the content: appending, editing, deleting, etc...
* Detailed view for each line, even during editing or adding a line
* Data analysis features

SpecCSV treat the whole file as a *Frame* and each line as an *Observation* with the exception for the first line which is stored as a special line called **meta**. It'll also getting ready for more advanced data analysis features (inspired by R and numpy)

SpecCSV usage is straight forward, typing the filename alone will greats you with the help text. you can also type `help` as a command to display the program usage.
