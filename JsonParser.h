/******************************************************************************
* #Author       : Zafer Satılmış
* #Revision     : 1.0
* #Date         : Oct 9, 2019 - 1:51:47 PM
* #File Name    : JsonParser.h
* #File Path    : /MyJson/src/JsonParser.h
*
*******************************************************************************/

/******************************************************************************
* 
*    ---------------------- Example -----------------
#include <stdio.h>
#include "JsonParser.h"

static const char *JSON_STRING =
    "{\"user\": \"johndoe\", \"admin\": 55, \"uid\": 100,\n  "
    "\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";

int main()
{
    jsmnData data = {0};
    jsonGetData(JSON_STRING, "admin", &data);

    if (JSMN_PRIMITIVE == data.type)
    {
        printf(" int val :%d", data.val);
    }

    return 0;
}
******************************************************************************/
/******************************IFNDEF & DEFINE********************************/
#ifndef JSONPARSER_H_
#define JSONPARSER_H_
/*********************************INCLUDES*************************************/

/******************************MACRO DEFINITIONS*******************************/

/*******************************TYPE DEFINITIONS ******************************/

/**
 * JSON type identifier. Basic types are:
 *  o Object
 *  o Array
 *  o String
 *  o Other primitive: number, boolean (true/false) or null
 */
typedef enum {
  JSMN_UNDEFINED = 0,
  JSMN_OBJECT = 1,
  JSMN_ARRAY = 2,
  JSMN_STRING = 3,
  JSMN_PRIMITIVE = 4
} jsmntype_t;

enum jsmnerr {
  /* Not enough tokens were provided */
  JSMN_ERROR_NOMEM = -1,
  /* Invalid character inside JSON string */
  JSMN_ERROR_INVAL = -2,
  /* The string is not a full JSON packet, more bytes expected */
  JSMN_ERROR_PART = -3
};

/**
 * JSON token description.
 * type     type (object, array, string etc.)
 * start    start position in JSON data string
 * end      end position in JSON data string
 */
typedef struct {
  jsmntype_t type;
  int start;
  int end;
  int size;
#ifdef JSMN_PARENT_LINKS
  int parent;
#endif
} jsmntok_t;

typedef struct
{
    jsmntype_t type;
    int val;
    char str[256];
}jsmnData;

/**
 * JSON parser. Contains an array of token blocks available. Also stores
 * the string being parsed now and current position in that string.
 */
typedef struct
{
  unsigned int pos;     /* offset in the JSON string */
  unsigned int toknext; /* next token to allocate */
  int toksuper;         /* superior token node, e.g. parent object or array */
} jsmn_parser;

/************************* GLOBAL VARIBALE REFERENCES *************************/

/************************* GLOBAL FUNCTION DEFINITIONS ************************/

/**
 * Create JSON parser over an array of tokens
 */
void jsmn_init(jsmn_parser *parser);

/**
 * Run JSON parser. It parses a JSON data string into and array of tokens, each
 * describing
 * a single JSON object.
 */
int jsmnParse(jsmn_parser *parser, const char *js, unsigned int len, jsmntok_t *tokens, unsigned int num_tokens);

int jsonGetData(const char *json, const char *key, jsmnData *data);

#endif /* JSONPARSER_H_ */

/********************************* End Of File ********************************/
