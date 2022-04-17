#include "stdio.h"
#include <stdlib.h>
#include <string.h>

typedef struct hashmap_struct {
    char *key;
    char *value;
    struct hashmap_struct *next;
} hashmap_t;

typedef struct string_array_struct {
    char* data;
    struct string_array_struct* next;
} string_array_t;

hashmap_t *hashmapAlloc(void);

string_array_t *arrayAlloc(void);

void hashmapDealloc(hashmap_t *hashmap);

void arrayDealloc(string_array_t *directories);

void hashmapDelItem(hashmap_t *hashmap, char *key);

void hashmapAddItem(hashmap_t *hashmap, char *key, char* value);

void arrayAddItem(string_array_t *directories, char* value);

void ArrayAppendItem(string_array_t *lines_to_write, char* value);

void printArray(string_array_t *directories);

void printHashmap(hashmap_t * hashmap);

void hashmapSetItem(hashmap_t *hashmap, char *key, char* value);

char* hashmapGetItem(hashmap_t *hashmap, char *key);

char** splitString(char *argument, char *delim);

void parseSymbolMapping(hashmap_t *hashmap, char *argument);

int arrayLen(char **array);

void ParseUndef(hashmap_t *hashmap, char* file_line);

void parseDefine(hashmap_t *hashmap, char* file_line);

char* mapVariable(char* file_line, char* variable, char* value);

void mapValueInLine(hashmap_t *hashmap, char* file_line);

int ParseIfCondition(hashmap_t *hashmap, char *condition);

int ParseIfdefCondition(hashmap_t *hashmap, char *condition);

void ParseConditionalStack(hashmap_t *hashmap, string_array_t *conditional_stack, string_array_t *lines_to_write);

void CheckDirectories(hashmap_t *hashmap, string_array_t *directories, string_array_t *lines_to_write, char* header_file);

void parseFile(hashmap_t *hashmap, string_array_t *directories, string_array_t *lines_to_write, char* filename);

void parseCLArguments(hashmap_t *hashmap, string_array_t *directories, string_array_t *lines_to_write , char **argv, int argc, char *outfile);

void write_to_file(string_array_t *lines_to_write, char *outfile);