#pragma once

typedef enum {
    UNKNOWN = -1,
    GET,
    POST,
    OPTIONS,
    HEAD,
    PUT,
    PATCH,
    DELETE,
    CONNECT,
} RequestMethod;