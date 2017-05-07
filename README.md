# facebook_chat_parser
Lightweight HTML parser in C to convert Facebook message data into readable TSV files separated by message threads.

Since Facebook messages commonly have commas, a tab delimiter is more practical to read.

## Usage

TSV header: name, date, message

The program creates an index.txt which contains the thread number and the Facebook ID's of the users in the thread.

Example:
```
1: [123456, 654321, 1100011010, 133221333123111]
```
1 is the thread ID, and the list corresponds to the Facebook ID's

## TODO

Merge threads (Facebook seems to split message threads occassionally)

Translate HTML-encodings into ASCII characters
