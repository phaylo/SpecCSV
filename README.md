# SpecCSV
command-line csv editor and inspector

SpecCSV is a command-line CSV (Comma Separated Values file) editor and inspector, powered with simple data analysis features. It was designed to be compatible with R Programming Language, while offering useful and easy to use features to fully manipulate CSV files.

SpecCSV includes the following features:
* Read and show the content of a CSV file in various ways.
* Display information about the file.
* Edit, delete, insert and append observations (new lines) to the file.
* Simple data analysis features (sum, mean, max, etc...).

SpecCSV treats the whole file as a *Data Frame*, and each line as an *Observation*, with exception for the first line which is stored as a special line called **meta**. The design and trait of this program are inspired by R and numpy.

SpecCSV usage is straight forward, typing the filename alone will greats you with the help text. you can also type `help` as a command to display the program usage.
