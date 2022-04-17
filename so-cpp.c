#include "so-cpp.h"

hashmap_t *hashmapAlloc(void) {
	hashmap_t *hashmap = malloc(sizeof(hashmap_t));

	hashmap->key = NULL;
	hashmap->value = NULL;
	hashmap->next = NULL;
    return hashmap;
}

string_array_t *arrayAlloc(void){
	string_array_t *list = malloc(sizeof(string_array_t));

	list->data = NULL;
	list->next = NULL;
	return list;
}

void hashmapDealloc(hashmap_t *hashmap) {
    hashmap_t *head;

	if (hashmap == NULL)
		return;

    for (head = hashmap->next; head != NULL;) {
		hashmap_t *tmp = head;

		head = head->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
    }

    free(hashmap);
}

void arrayDealloc(string_array_t *directories){
    string_array_t *head;

	if (directories == NULL)
		return;

    for (head = directories->next; head != NULL;) {
		string_array_t *tmp = head;

		head = head->next;
		free(tmp->data);
		free(tmp);
    }

    free(directories);
}

void hashmapDelItem(hashmap_t *hashmap, char *key) {
    hashmap_t *ptr;

	if (hashmap->next == NULL)
		return;

	if (strcmp(hashmap->next->key, key) == 0) {
		hashmap_t *tmp = hashmap->next;

		hashmap->next = hashmap->next->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		return;
	}

	for (ptr = hashmap->next; ptr && ptr->next != NULL; ptr = ptr->next)
		if (strcmp(ptr->next->key, key) == 0) {
			hashmap_t *tmp = ptr->next;

			ptr->next = ptr->next->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			break;
		}
}

void hashmapAddItem(hashmap_t *hashmap, char *key, char* value) {
    hashmapDelItem(hashmap, key);
    hashmap_t *h = malloc(sizeof(struct hashmap_struct));

    h->key = malloc(strlen(key) + 1);
    strcpy(h->key, key);

	h->value = malloc(strlen(value) + 1);
	strcpy(h->value, value);

	h->next = NULL;

    if (hashmap->next == NULL)
		hashmap->next = h;
	else {
		hashmap_t *tmp = hashmap->next;

		hashmap->next = h;
		h->next = tmp;
	}
}

void arrayAddItem(string_array_t *directories, char* value) {
    string_array_t *d = malloc(sizeof(struct string_array_struct));

    d->data = malloc(strlen(value) + 1);
	d->next = NULL;
    strcpy(d->data, value);
	
	if (directories->next == NULL)
		directories->next = d;
	else {
		string_array_t *tmp = directories->next;

		directories->next = d;
		d->next = tmp;
	}
}

void ArrayAppendItem(string_array_t *lines_to_write, char* value)
{
    string_array_t *new_node = malloc(sizeof(struct string_array_struct));
    string_array_t *last = lines_to_write;
  
    new_node->data  = malloc(strlen(value) + 1);
	new_node->next = NULL;
    strcpy(new_node->data, value);
 
    new_node->next = NULL;
 
    while (last->next != NULL)
		last = last->next;
	
	last->next = new_node; 
}

void printArray(string_array_t *directories){
    string_array_t *ptr;

    for (ptr = directories->next; ptr != NULL; ptr = ptr->next) {
        printf("%s\n", ptr->data);
    };
}

void printHashmap(hashmap_t *hashmap){
    hashmap_t *ptr;

    for (ptr = hashmap->next; ptr != NULL; ptr = ptr->next) {
        printf("{key: %s, value: %s}\n", ptr->key, ptr->value);
    }
}

void hashmapSetItem(hashmap_t *hashmap, char *key, char* value) {
    hashmap_t *ptr;

    for (ptr = hashmap; ptr != NULL; ptr = ptr->next) {
        if (strcmp(ptr->key, key) == 0) {
            ptr->value = value;
        }
    }
}

char* hashmapGetItem(hashmap_t *hashmap, char *key) {
    hashmap_t *ptr;

    for (ptr = hashmap->next; ptr != NULL; ptr = ptr->next) {
        if (strcmp(ptr->key, key) == 0) {
            return ptr->value;
        }
    }

    return NULL;
}

int arrayLen(char **array){
    int len = 0, i = 0;

    while (strlen(array[i]) > 0){
        len++;
        i++;
    }

    return len;
}

void ParseUndef(hashmap_t *hashmap, char* file_line){

    char **tokens = splitString(file_line, " \t\n\r");
	int i;
    hashmapDelItem(hashmap, tokens[1]);

	for (i = 0; i < 20; i++)
		free(tokens[i]);

	free(tokens);
}

void parseDefine(hashmap_t *hashmap, char* file_line){

    char **tokens = splitString(file_line, " \t\n\r");
	int i;

    if (arrayLen(tokens) > 3){
        char *expr = (char *) malloc(40 * sizeof(char));

		expr[0] = '\0';

        for (i = 2; i < arrayLen(tokens); i++){

            if (hashmapGetItem(hashmap, tokens[i]) != NULL){
                strcat(expr, hashmapGetItem(hashmap, tokens[i]));
                strcat(expr, " ");
            }
            else{
                strcat(expr, tokens[i]);
                strcat(expr, " ");
            }
        }

        expr[strlen(expr) - 1] = '\0';
        hashmapAddItem(hashmap, tokens[1], expr);
		free(expr);
    }
    else{
        if (tokens[2] != NULL){
            if (hashmapGetItem(hashmap, tokens[2]) != NULL){
                hashmapAddItem(hashmap, tokens[1], hashmapGetItem(hashmap, tokens[2]));
            }
            else{
                hashmapAddItem(hashmap, tokens[1], tokens[2]);
            }
        }
        else{
            hashmapAddItem(hashmap, tokens[1], "");
        }
    }

	for (i = 0; i < 20; i++)
		free(tokens[i]);

	free(tokens);
}

char* mapVariable(char* file_line, char* variable, char* value)
{
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(value);
    int oldWlen = strlen(variable);

    for (i = 0; file_line[i] != '\0'; i++) {
        if (strstr(&file_line[i], variable) == &file_line[i]) {
            cnt++;

            i += oldWlen - 1;
        }
    }
  
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);
  
    i = 0;
    while (*file_line) {
        if (strstr(file_line, variable) == file_line) {
            strcpy(&result[i], value);
            i += newWlen;
            file_line += oldWlen;
        }
        else
            result[i++] = *file_line++;
    }
  
    result[i] = '\0';
    return result;
}

void mapValueInLine(hashmap_t *hashmap, char* file_line){
    hashmap_t *ptr;

    for (ptr = hashmap->next; ptr != NULL; ptr = ptr->next) {
        if (strstr(file_line, ptr->key) != NULL){
			char *mapVar = mapVariable(file_line, ptr->key, ptr->value);

            strcpy(file_line, mapVar);
			free(mapVar);
        }
    }
}

int ParseIfCondition(hashmap_t *hashmap, char *condition){

    char **tokens = splitString(condition, " \n\r");
	int res, i;

    if (tokens[1] == NULL)
        res = 0;

    if (hashmapGetItem(hashmap, tokens[1]) != NULL){
        if (atoi(hashmapGetItem(hashmap, tokens[1])) > 0){
            res = 1;
        } else
			res = 0;
    }
    else if (atoi(tokens[1]) > 0){
        res = 1;
    }
    else{
		res = 0;
    }

	for (i = 0; i < 20; i++)
		free(tokens[i]);

	free(tokens);
	return res;
}

int ParseIfdefCondition(hashmap_t *hashmap, char *condition){

    char **tokens = splitString(condition, " ");
	int res, i;

    if (tokens[1] == NULL)
        res = 0;

    if (hashmapGetItem(hashmap, tokens[1]) != NULL){
        res = 1;
    }
    else{
        res = 0;
    }

	for (i = 0; i < 20; i++)
		free(tokens[i]);

	free(tokens);
	return res;
}

void ParseConditionalStack(hashmap_t *hashmap, string_array_t *conditional_stack, string_array_t *lines_to_write){

    string_array_t *ptr;

    for (ptr = conditional_stack->next; ptr != NULL; ptr = ptr->next) {

        if ((strstr(ptr->data, "#if") != NULL && strstr(ptr->data, "#ifdef") == NULL && strstr(ptr->data, "#ifndef") == NULL)
            || strstr(ptr->data, "#elif") != NULL){
            if (ParseIfCondition(hashmap, ptr->data) > 0){
                ptr = ptr->next;
                for (; ptr != NULL; ptr = ptr->next) {
                    if (ptr->data[0] != '#') {
                        ArrayAppendItem(lines_to_write, ptr->data);
                    }
                    else{
                        break;
                    }
                }
                return;
            }
        }
        else if (strstr(ptr->data, "#ifdef") != NULL){
            if (ParseIfdefCondition(hashmap, ptr->data) > 0){
                ptr = ptr->next;
                for (; ptr != NULL; ptr = ptr->next) {
                    if (ptr->data[0] != '#') {
                        ArrayAppendItem(lines_to_write, ptr->data);
                    }
                    else{
                        break;
                    }
                }
                return;
            }
        }
        else if (strstr(ptr->data, "#else") != NULL){
            ptr = ptr->next;
            for (; ptr != NULL; ptr = ptr->next) {
                if (ptr->data[0] != '#') {
                    ArrayAppendItem(lines_to_write, ptr->data);
                }
                else{
                    break;
                }
            }
            return;
        }
        else if (strstr(ptr->data, "#define") != NULL){
            parseDefine(hashmap, ptr->data);
            return;
        }
        else if (strstr(ptr->data, "#undef") != NULL){
            ParseUndef(hashmap, ptr->data);
            return;
        }
    }
}


void CheckDirectories(hashmap_t *hashmap, string_array_t *directories, string_array_t *lines_to_write, char* header_file){

    char **tokens = splitString(header_file, " ");
    string_array_t *ptr;
    int headerFileExists = 0, i;

    char *file_name = tokens[1];
    char *current_dir = malloc(100 * sizeof(char));
    strcpy(current_dir, "_test/inputs/");

    file_name += 1;
    file_name[strlen(file_name) - 1] = '\0';

    strcat(current_dir, file_name);
    FILE* fp = fopen(current_dir, "r+t");

    if (fp != NULL) {
        headerFileExists = 1;
        parseFile(hashmap, directories, lines_to_write, current_dir);
    }
    else{

        for (ptr = directories; ptr != NULL; ptr = ptr->next) {

            char *full_path = malloc(100 * sizeof(char));

            strcpy(full_path, ptr->data);
            strcat(full_path, "/");
            strcat(full_path, file_name);

            FILE* fp = fopen(full_path, "r+t");

            if (fp != NULL){
                headerFileExists = 1;
                parseFile(hashmap, directories, lines_to_write, full_path);
            }
        }
    }

    if (headerFileExists == 0){
        exit(127);
    }

	for (i = 0; i < 20; i++)
		free(tokens[i]);

	free(tokens);
}

void parseFile(hashmap_t *hashmap, string_array_t *directories, string_array_t *lines_to_write, char* filename){
    FILE* fp = fopen(filename, "r+t");

    if(!fp){
        exit(127);
    }

    char *file_line = malloc(100 * sizeof(char));

    while (fgets(file_line, 100, fp)) {

        if (file_line[strlen(file_line) - 1] == '\n'){
            file_line[strlen(file_line) - 1] = '\0';
        }

        if (strstr(file_line, "#define") != NULL){
            parseDefine(hashmap, file_line);
        }
        else if (strstr(file_line, "#undef") != NULL){
            ParseUndef(hashmap, file_line);
        }
        else if (strstr(file_line, "#if") != NULL){
            string_array_t *conditional_stack = arrayAlloc();

            while(strstr(file_line, "#endif") == NULL){

                if (file_line[strlen(file_line) - 1] == '\n'){
                    file_line[strlen(file_line) - 1] = '\0';
                }

                ArrayAppendItem(conditional_stack, file_line);
                fgets(file_line, 100, fp);
            }
            ParseConditionalStack(hashmap, conditional_stack, lines_to_write);
			arrayDealloc(conditional_stack);
        }
        else if (strstr(file_line, "#include") != NULL){

            CheckDirectories(hashmap, directories, lines_to_write, file_line);
        }
        else if (strlen(file_line) > 0){
            mapValueInLine(hashmap, file_line);
            ArrayAppendItem(lines_to_write, file_line);
        }
    }

	free(file_line);
    fclose(fp);
}


char** splitString(char *argument, char *delim){
    
    char **result = (char**)malloc(20 * sizeof(char*));
	int i;
    
    for (i = 0; i < 20; i++) {
        result[i] = (char *) malloc(40 * sizeof(char));
		result[i][0] = '\0';
	}

    char *token = strtok(argument, delim);

    i = 0;
    while( token != NULL ) {
        strcpy(result[i], token);
        token = strtok(NULL, delim);
        i++;
    }

    return result;
}

void parseSymbolMapping(hashmap_t *hashmap, char *argument){

    char **tokens = splitString(argument, "=");
	int i;
    hashmapAddItem(hashmap, tokens[0], (tokens[1] != NULL) ? tokens[1] : "");

	for (i = 0; i < 20; i++)
		free(tokens[i]);

	free(tokens);
}

void parseCLArguments(hashmap_t *hashmap, string_array_t *directories, string_array_t *lines_to_write , char **argv, int argc, char *outfile){
	int i;

    for (i = 1; i < argc; i++){
        if (strstr(argv[i], "-D") != NULL){
            if (strlen(argv[i]) > 2){
                argv[i] += 2;
                parseSymbolMapping(hashmap, argv[i]);
            }
            else if (argv[i + 1] != NULL){
                parseSymbolMapping(hashmap, argv[i + 1]);
                i++;
            }
        }

        else if (strstr(argv[i], "-I") != NULL){
            if (strlen(argv[i]) > 2){
                argv[i] += 2;
                arrayAddItem(directories, argv[i]);
            }
            else if (argv[i + 1] != NULL){
                arrayAddItem(directories, argv[i + 1]);
                i++;
            }
        }

        else if (strstr(argv[i], "-o") != NULL){
            if (strlen(argv[i]) > 2){
                argv[i] += 2;
                strcpy(outfile, argv[i]);
            }
            else if (argv[i + 1] != NULL){
                strcpy(outfile, argv[i + 1]);
                i++;
            }
        }

        else
            parseFile(hashmap, directories, lines_to_write, argv[i]);
    }
}

void write_to_file(string_array_t *lines_to_write, char *outfile){

    FILE* file = fopen(outfile, "w"); 
    string_array_t *ptr;

    if(!file){
        exit(127);
    }

    for (ptr = lines_to_write->next; ptr->next != NULL; ptr = ptr->next) {
        fprintf(file, "%s", ptr->data);
        fprintf(file, "%s", "\n");
    }

    fprintf(file, "%s", ptr->data);

    fclose(file);
}

int main(int argc, char **argv){

    char *outfile = malloc(100 * sizeof(char));
    hashmap_t *hashmap = hashmapAlloc();
    string_array_t *directories = arrayAlloc();
    string_array_t *lines_to_write = arrayAlloc();

	outfile[0] = '\0';

    if (argc > 1){

        parseCLArguments(hashmap, directories, lines_to_write, argv, argc, outfile);
    }
    else{
        exit(127);
    }

    if (strlen(outfile) > 0){
        write_to_file(lines_to_write, outfile);
    }
    else{
        printArray(lines_to_write);
    }

    arrayDealloc(directories);
    arrayDealloc(lines_to_write);
    hashmapDealloc(hashmap);
	free(outfile);
    return 0;
}