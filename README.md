# facebook_chat_parser
Lightweight HTML parser in C to convert Facebook message data into readable TSV files separated by message threads.

Since Facebook messages commonly have commas, a tab delimiter is more practical to read.

## Usage

Place your messages.htm file in the root directory and run the parser.
```
./parser
```

The program creates an index.txt which contains the thread number and the Facebook ID's of the users in the thread and a directory called chatthreads which contains each individual thread in its own TSV file.

Example:
```
1: [123456, 654321, 1100011010, 133221333123111]
```
1 is the thread ID, and the list corresponds to the Facebook ID's


TSV header: name, date, message

## TODO

Merge threads (Facebook seems to split message threads occassionally)

Translate HTML-encodings into ASCII characters
