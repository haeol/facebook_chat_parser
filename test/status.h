#ifndef _PARSER_STATUS_H_
#define _PARSER_STATUS_H_

#define SPAN_CLOSE_LENGTH 7
#define MESSAGE_CLOSE_LENGTH 4

typedef enum {
    NONE,
    USER_OPEN,
    META_OPEN,
    SPAN_CLOSE,
    MESSAGE_OPEN,
    MESSAGE_CLOSE,
    CHAT_THREAD_OPEN,
    DIV_CLOSE
} status_t;

/*
status_t closingTag(status_t status) {
    status_t s = NONE;
    switch (status) {
        case USER_OPEN:    s = SPAN_CLOSE;    break;
        case META_OPEN:    s = SPAN_CLOSE;    break;
        case MESSAGE_OPEN: s = MESSAGE_CLOSE; break;
        default: break;
    }
    return s;
}

*/

#endif // _PARSER_STATUS_H_
