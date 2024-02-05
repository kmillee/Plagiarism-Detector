#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>


char* Instru[] = {"break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "if", "int", "long", "return", "short", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "while"};


typedef struct segment {        // the structure for the text's lines
    char* text;
    struct segment* next;
    int line;
    int original_line;
    char* original_text;
} Segment;

typedef struct {                // the structure uniting the lines
    Segment* head;
} List;


Segment* SegCreate() {
    Segment* s = malloc(sizeof(Segment));
    if (s == NULL) {
        printf("ERROR: problem to allocate memory in SegCreate\n");
        return NULL;
    }

    s->text = "";
    s->next = NULL;
    s->line = -1;
    s->original_line = -1;
    s->original_text = "";
    return s;
}


List* ListCreate() {
    List* L = malloc(sizeof(List));
    if (L == NULL) {
        printf("ERROR: problem to allocate memory in ListCreate\n");
        return NULL;
    }
    L->head = NULL;
    return L;
}


// gives the length of the text contained in the Segment
int SegSize(const Segment* s) {
    return strlen(s->text);
}


int ListSize(const List* L) {
    if (L == NULL) {
        printf("ERROR: the list in ListSize doesn't exist\n");
        return -1;
    }
    else {
        Segment* s = L->head;
        int cpt = 0;
        while (s != NULL) {
            cpt++;
            s = s->next;
        }
        return cpt;
    }
}


// function from the TD, to free an List and all its Segments from the memory
void free_List(List* L) {
    if (L == NULL) {
        printf("We aren't pointing at any List\n");
        return;
    }

    Segment* s = L->head;
    Segment* temp = NULL;
    while (s != NULL) {                 // to know if we're still in the List
        temp = s;                       // we anchor our element before moving to the next one
        s = s->next;                    // we move now to the next one, before it's eliminated (with its connections)
        free(temp->text);
        free(temp->original_text);
        free(temp);                     // we delete the overall structure
    }

    free(L);                            // once every Segment is freed, we free the remaining List
    L = NULL;
}


void free_Mat(float** M, int n, int m){
    for (int i = 0 ; i < n; i++) {
        free(M[i]);                     // we free the matrix column per column
    }
    free(M);
}


void printmat(float** D, int n, int m) {
    for (int i = 0 ; i < n ; i++) {
        printf("\n");
        for (int j = 0 ; j < m ; j++) {
            printf("%.2f\t", D[i][j]);
        }
    }
    printf("\n\n");
}


int maximum(int a, int b) {
    if (a>b) return a;
    return b;
}


int minimum(int a, int b) {
    if (a<b) return a;
    return b;
}


int minimum3(int a, int b, int c) {
    int min1 = minimum(a, b);
    return minimum(min1, c);
}


// names the PGM file according to the inpu
void NamePGM(char* final, char* type, char* n1, char* n2) {
    strcpy(final, type);
    strcat(final, "-");
    strcat(final, n1);
    strcat(final, "-");
    strcat(final, n2);
    strcat(final, ".pgm\0");
    return;
}


// takes a file's name and a pointer on a integer as a value, inserts in the latter the longest line's length
int LineMax(const char* fileName, int* max) {
    FILE* f = fopen(fileName, "r");
    int cpt = 0;                                        // counts the number of characters per line
    *max = 0;

    if (f == NULL) {
        printf("ERROR: the file '%s' in LineMax doesn't exist\n", fileName);
        return 0;
    }

    int a = feof(f);                                    // verifying is the file is empty

    while (a != EOF) {                                  // to travel within the file until its ending
        a = fgetc(f);
        if (a != '\n') {
            cpt++;
        }

        else {                                          // we're at the end of the line
            cpt++;                                      // we add the '\n' character
            if (cpt > (*max)) {
                (*max) = cpt;                           // now max points to the larger number of characters
            }
            cpt = 0;
        }
    }

    if (*max == 0) {                                    // we never found a character
        printf("ERROR: the file in LineMax is empty\n");
        return 1;
    }

    *max += 1;                                          // includes now the '\0' character
    int r = fclose(f);
    if (r != 0) {
        printf("ERROR: file \"%s\" in LineMax wasn't properly close\n",fileName);
        return 0;
    }

    return 1;
}

// verifies if a string belongs to an array of strings, both as inputs
int inArray(const char* ch, char** Bibli, int length) {
    for (int i = 0 ; i < length ; i++) {
        if (!strcmp(ch, Bibli[i])) {            // the string corresponds to an element of the array
            return 1;
        }
    }
    return 0;                                   // we never found a similar string, it doesn't belong there
}


// takes the translated line, and adds a letter to replace an entire word
void replace_word(char* treated, char* word, int* i) {
    if (word[0] != '\0') {                      // verify if there is a word to be replaced
        if (inArray(word, Instru, 27)) {        // if the word is among the key ones
            treated[(*i)++] = 'm';              // we replace it by 'w' in the treated line
        }
        else {
            treated[(*i)++] = 'w';              // it's not a keyword, it corresponds to a 'w'
        }
        treated[*i] = '\0';
        word[0] = '\0';                         // reinitializing the word
    }

    else {                                      // we can't replace translate a word
        treated[*i] = '\0';                     // we only close the string
    }
}


// verifies is the string contains other characters than spaces or tabulations
int only_spaces(const char* ch) {
    for (int i = 0; ch[i] != '\0'; i++) {
        if ((ch[i] != ' ') && (ch[i] != '\t')) {
            return 0;                           // no, there aren't only spaces
        }
    }

    return 1;                                   // we never met another type of character: there only are spaces or tabulations
}


// we look to translate and simplify the file, while conserving it as a linked list
List* pre_treatment(const char* file) {
    int* MAX;
    int tmp = 0;
    MAX = &tmp;
    int a = LineMax(file, MAX);                 // we get the longest possible line
    if (!a) {
       printf("File not properly opened\n");
       return NULL;
    }

    List* L = ListCreate();
    FILE* f = fopen(file, "r");
    a = feof(f);
    if (a) {                        // we already reached the end of the file
        printf("The file is empty\n");
        free(L);
        fclose(f);
        return NULL;
    }

    int k = 1;                      // indicates the number of the segment
    int number_l = 0;               // indicates the segment's original line's number

    int in_string = 0;              // indicates whether we're in a string
    int in_quote = 0;               // indicates whether we're in a character quote
    int for_slash = 0;              // indicates whether we're after a backslash '\'
    int equal = 0;                  // indicates whether we're after an equal '=' sign
    int end_line = 0;               // indicates whether the ';' character serves as end of the line
    int bibl = 0;                   // indicates whether we're treating a library's inclusion
    int for_ens = 1;                // indicates whether we're entering an array after an equal sign
    int first = 0;                  // indicates the beginning of the array with '{'
    int sec = 0;                    // indicates the ending of the array with '}'

    int for_word = 0;               // the string "word"'s index
    int for_treat = 0;              // the string "treated"'s index
    int for_original = 0;           // the string "original"'s index

    char treated[*MAX];             // string in which we copy the translated lines (before being inserted into the segments)
    char line[*MAX];                // string where we copy each line of the file at the time
    char word[(*MAX)];              // string to cobbel the original words (to replace them by a single letter)
    char original_text[*MAX];       // string to preserve the original lines in the segments (corresponding to their translated ones)

    word[0] = '\0';
    treated[0] = '\0';
    original_text[0] = '\0';

    Segment* head = SegCreate();        // initializing the possible head of the list

    int t = 0;                          // indicates whether we already found a head for the list or not
    while (!a) {                        // to know if we attained the end of the file

        fgets(line, *MAX, f);           // we copy the next line (with the supposed maximal size)

        a = feof(f);
        if (a) {                                            // the new line is the last one
            if ((line == NULL) || (line[0] == '\0')) {      // that last line is empty
                break;                                      // we ignore it, and have ended the whole translation
            }
        }

        if (line == NULL) {
            printf("The line %d wasn't properly obtained\n", k);
            fclose(f);
            free(head);
            free(L);
            return NULL;
        }

        //@printf("\n");

        if (line[0] == '\0') {              // it's the last line, and it's empty
            break;
        }

        else if (line[0] == '\n') {         // the line is empty, but not the last
            number_l += 1;                  // we passed a line
        }


        else if (line[0] != '\n') {         // the line isn't empty

            for (int i = 0 ; line[i] != '\0' ; i++) {   // we pass through each character of the line

                if (line[i] == '\n') {                  // we arrived at the end, and it's not a ; or { or }

                    if (treated[0] == '#') {    // the line is a library's import
                        bibl = 1;               // we indicate that it's the case
                    }
                    else {
                        replace_word(treated, word, &for_treat);    // we translated the last copied word
                        for_word = 0;
                    }

                    number_l += 1;              // we passed through a line
                }

                else if (line[i] == '\\') {

                    original_text[for_original++] = line[i];
                    original_text[for_original] = '\0';

                    if (for_slash) {            // it's the second '\' in a row
                        for_slash = 0;          // this one doesn't count and is annihilated by the latest
                    }

                    else {                      // it doesn't follow another '\'
                        for_slash = 1;
                    }

                    continue;
                }


                else if (line[i] == '\"') {

                    original_text[for_original++] = line[i];
                    original_text[for_original] = '\0';

                    if (for_slash) {            // it's the character ", not the start of a string
                        for_slash = 0;          // we ignore it, it's forcibly in a string or character quote
                    }

                    else {                      // it's a real string's indicator
                        if (in_string) {in_string = 0;}     // we already were in a string, we're exiting it
                        else {
                            in_string = 1;                  // we're entering the string
                            replace_word(treated, word, &for_treat);
                        }
                        for_word = 0;
                        treated[for_treat++] = '\"';        // in any case, we add the character to the new line
                        treated[for_treat] = '\0';
                    }
                }

                else if (line[i] == '\'') {

                    original_text[for_original++] = line[i];
                    original_text[for_original] = '\0';
                    if (for_slash) {                        // it's the character ', not an actual one
                        for_slash = 0;                      // we ignore it, as we're in a character quote
                    }

                    else {    // it's a real character quote's indication
                        if (in_quote) {in_quote = 0;}       // we're exiting it
                        else if (in_string) {continue;}     // we're in a string, we still ignore it so
                        else {
                            in_quote = 1;                   // we're entering a character quote
                            replace_word(treated, word, &for_treat);
                        }
                        for_word = 0;
                        treated[for_treat++] = '\'';        // in any case, we add the ' character to the new line
                        treated[for_treat] = '\0';

                        continue;
                    }
                }


                else if ((in_string) || (in_quote)) {       // we're within a string or character quote

                    original_text[for_original++] = line[i];
                    original_text[for_original] = '\0';
                    if ((for_slash) && (line[i] != ' ') && (line[i] != '\t')) {     // we inform that we aren't after a '\ anymore'
                        for_slash = 0;
                    }

                    continue;  // we don't copy anything
                }


                // now that we're sure we're out of strings or quotes
                else if ( (line[i] == '/') && (line[i+1] == '/') ) {        // to indicate if it's the start of a comment

                    replace_word(treated, word, &for_treat);                // we translate the line's last word
                    for_word = 0;
                    number_l += 1;                                          // we're stopping the line here, we add a new one to the count
                    break;                                                  // we don't copy anything anymore
                }


                // to indicate if it's an alphanumerical character
                else if ( ((line[i] > 47) && (line[i] < 58)) || ((line[i] > 64) && (line[i] < 91)) || ((line[i] > 96) && (line[i] < 123)) || (line[i] == '_')) {
                    // we include the underscore, as it can be part of a function or variable's name
                    original_text[for_original++] = line[i];
                    original_text[for_original] = '\0';

                    word[for_word++] = line[i];         // we add that character to the word that will be translated
                    word[for_word] = '\0';
                    for_slash = 0;

                    continue;
                }


                else if ( (line[i] == ' ') || (line[i] == '\t') ) {         // those specific characters shouldn't be copied at all

                    original_text[for_original++] = line[i];
                    original_text[for_original] = '\0';

                    if (!isdigit(word[0])) {                        // the word isn't a number, or else it could contain arbitrary spaces in it
                        replace_word(treated, word, &for_treat);    // we finished the word, we translate it now
                        for_word = 0;
                    }
                }

                else {                                  // the character is a symbol whatsoever

                    original_text[for_original++] = line[i];
                    original_text[for_original] = '\0';


                    if (line[i] == '=') {               // to indicate that we'll be next after an '=' sign
                        equal = 1;
                    }

                    else if (equal) {                   // the last character in the treated string is an '='
                        if (line[i] == '{') {           // if the first character after '=' is this one...
                            for_ens = 1;                // ... then it's the beginning of an array, not a bloc of instructions
                        }

                        else if ((line[i] != ' ') && (line[i] != '\t')) {   // those are to be ignored in front of the '='
                            equal = 0;                  // there isn't an array right after the '=', we don't have any problem
                        }
                    }

                    if ((line[i] == '{') && (!equal)) {first = 1;}    // it's the real start of an instructions' group, where the command ends

                    if (line[i] == '}') {
                        if (!for_ens) {                         // we aren't in an array...
                            sec = 1;                            // ... then it's the actual end of an instructions bloc
                        }
                    }

                    if (line[i] == ';') {end_line = 1;}         // to indicate that we'll stop the code line right here

                    replace_word(treated, word, &for_treat);    // we translate our last word
                    for_word = 0;
                    treated[for_treat++] = line[i];             // we add that symbol, whichever it is
                    treated[for_treat] = '\0';
                }


                if ((bibl) || (end_line) || (first) || (sec)) {     // all those Boolean indicate that it's the end of the code instruction
                    end_line = 0;
                    bibl = 0;
                    first = 0;
                    sec = 0;
                    for_ens = 0;

                    if (t) {                // we already have placed a head
                        k++;
                        Segment* cont = SegCreate();                // to create the new Segment containing the treated text
                        cont -> text = malloc(sizeof(char) * (strlen(treated)+1));
                        strcpy(cont -> text, treated);
                        cont -> original_text = malloc(sizeof(char) * (strlen(original_text)+1));
                        strcpy(cont -> original_text, original_text);
                        cont -> line = k;
                        cont -> original_line = number_l + 1;       // we still haven't incremented that counter, we foresee that
                        cont -> next = NULL;
                        head -> next = cont;                        // we place the Segment right afte the last one
                        head = cont;                                // we point now to the last Segment
                    }

                    else {                                          // we still haven't placed a head. We're creating it now
                        t = 1;                                      // to indicate that now we're out of that case
                        head -> text = malloc(sizeof(char) * (strlen(treated)+1));
                        strcpy(head -> text, treated);
                        head -> original_text = malloc(sizeof(char) * (strlen(original_text)+1));
                        strcpy(head -> original_text, original_text);
                        head -> next = NULL;
                        head -> line = 1;                           // it's the List's first Segment
                        head -> original_line = number_l + 1;
                        L -> head = head;                           // we place the Segment as the head
                    }

                    treated[0] = '\0';
                    word[0] = '\0';
                    original_text[0] = '\0';                        // we reboot those arrays as empty ones
                    for_treat = 0;
                    for_word = 0;
                    for_original = 0;
                }

                // we passed through every possible case
            }

            if ((treated[0] == '\0') || (only_spaces(treated))) {       // there wasn't anything in the line to copy
                word[0] = '\0';
            }
        }

        a = feof(f);                                    // to check if we've reached the end now
        if (a) {                                        // to know before entering the next line if it there are more after
            break;
        }
    }

    if (head -> line == -1) {                           // the file being empty, we never found a head
        printf("Le fichier %s est vide\n", file);
        free(head);                                     // we free the unused Segment
        fclose(f);
        return L;                                       // we return the empty List
    }

    int end = fclose(f);
    if (end != 0) {
        printf("The file doesn't properly close\n");
    }

    return L;                                           // we finally translated the file in a linked List
}


// separates the Segment into a diagram (an array of strings)
void diagram(const Segment* seg, char** diag) {
    char ch[SegSize(seg)+1];                            // will get the Segment's content
    strcpy(ch, seg->text);
    int len = strlen(ch);
    if (len == 0) {
        diag[0][0] = ' ';
        diag[0][1] = ' ';
        diag[0][2] = '\0';
    }

    else if (len == 1) {
        int r = strcmp(ch, "}");
        if (r == 0) {
            strcpy(diag[0], "} \0");
        }
        else  {
            strncpy(diag[0], ch, 1);
        }
    }

    else {
        for (int i = 1 ; ch[i] != '\0' ; i++) {     // we can't stop the loop at the last character befire \0, thus we start at 1 and adapt right after
            diag[i-1][0] = ch[i-1];
            diag[i-1][1] = ch[i];
            diag[i-1][2] = '\0';
        }
    }
}


// counts the diagrams in common between two Segments of the List
int inter(char** d1, char** d2,int t1, int t2) {
    int c = 0;
    char y[3] = "yy";
    char z[3] = "zz";                       // to remove later the diagrams already counted once
    for (int i = 0 ; i < t1 ; i++) {
        for (int j = 0 ; j < t2 ; j++) {
            if (strcmp(d1[i], d2[j]) == 0) {
                c++;
                strcpy(d1[i], y);
                strcpy(d2[j], z);
            }
        }
    }

    return c;
}


int DiagSize(const Segment* s) {
    int t = SegSize(s);
    if (t <= 1) {
        return t;
    }
    return t-1;
}


// frees all the diagrams created from the Segment
void free_diag(char** d, int x) {
    if (d == NULL) {
        printf("ERROR: no array pointed at in free_diag\n ");
    }

    for (int i = 0 ; i < x ; i++) {
        free(d[i]);
    }

    free(d);
}


// calculates the Dice distance between the Segments' texts
float Dice(const Segment* s1, const Segment* s2) {
    int x = DiagSize(s1);
    int y = DiagSize(s2);
    char** d1 = malloc(x * sizeof(char*));
    char** d2 = malloc(y * sizeof(char*));

    if (d1 == NULL) {
        printf("ERROR: problem to allocate memory in Dice\n");
        free_diag(d2, y);
        return -1;
    }

    for (int i = 0 ; i < x ; i++) {
        d1[i] = malloc(3*sizeof(char));         // allocates n floats for each line
        d1[i][0] = ' ';
        d1[i][1] = ' ';
        d1[i][2] = '\0';
        if (d1[i] == NULL) {
            printf("ERROR: problem to allocate memory in Dice\n");
            free_diag(d1, i-1);
            free_diag(d2, y);
            return -1;
        }
    }

    if (d2 == NULL) {
        printf("ERROR: problem to allocate memory in Dice\n");
        free_diag(d1, x);
        return -1;
    }

    for (int i = 0 ; i < y ; i++) {
        d2[i] = malloc(3*sizeof(char));         // once again allocates n floats per line
        d2[i][0] = ' ';
        d2[i][1] = ' ';
        d2[i][2] = '\0';
        if (d2[i] == NULL) {
            printf("ERROR: problem to allocate memory in Dice\n");
            free_diag(d2, i-1);
            free_diag(d1, x);
            return -1;
        }
    }

    diagram(s1, d1);
    diagram(s2, d2);
    float c = inter(d1, d2, x, y);          // gets the number of diagrams in common between d1 et d2
    float d = 1 - ((2*c)/(x+y));            // then calculates the distance between the Segments

    free_diag(d1, x);
    free_diag(d2, y);                       // we don't need the segments anymore, we erase them now

    return d;
}


// calculates the Levenshtein distance between the Segments' texts
float Levenshtein(const char* ch1, const char* ch2) {
    int n = strlen(ch1);
    int m = strlen(ch2);
    n++;
    m++;
    float** L;
    L = malloc(n * sizeof(float*));             // allocates n pointers for lines of floats

    if (L == NULL) {
      printf("ERROR: problem to allocate memory in Levenshtein\n");
      return -1;
    }

    for (int i = 0 ; i < n ; i++) {
      L[i] = malloc(m * sizeof(float));         // allocates m ints per line
      if (L[i] == NULL) {
        printf("ERROR: problem to allocate memory in Levenshtein\n");
        return -1;
      }
    }

    int cout;
    for (int i = 0 ; i < n ; i++) {
        L[i][0] = i;
    }
    for (int j = 0 ; j < m ; j++) {
        L[0][j] = j;
    }

    for (int i = 1 ; i < n ; i++) {
        for (int j = 1 ; j < m ; j++) {
            if (ch1[i-1] == ch2[j-1]) {
                cout = 0;
            }
            else {
                cout = 1;
            }

            L[i][j] = minimum3(L[i-1][j] + 1, L[i][j-1] + 1, L[i-1][j-1] + cout);
        }
    }
    n--;
    m--;
    float l = L[n][m];
    l = l/(maximum(m, n));
    free_Mat(L, ++n, ++m);
    return l;
}


float** DiceMatCreate(const List* L1, const List* L2) {
    int n = ListSize(L1);
    int m = ListSize(L2);
    if ((n < 0) ||(m < 0)) {
        return NULL;
    }

    //$$int first=0;                            // counts if we already found two equal segments

    float** D;                                  // creates an n * m matrix
    D = malloc(n * sizeof(float*));             // reserves n lines

    if (D == NULL) {
        printf("ERROR: problem to allocate memory in DiceMat\n");
            return NULL;
    }

    for (int i = 0 ; i < n ; i++) {
        D[i] = malloc(m * sizeof(float));       // allocates m integers per line
        if (D[i] == NULL) {
        printf("ERROR: problem to allocate memory in DiceMat\n");
        return NULL;
        }
    }

    Segment* s1 = L1->head;
    Segment* s2 = L2->head;
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < m ; j++) {
            if ((Dice(s1, s2) < 0) || (Dice(s1, s2) > 1)) {
                printf("Non valide Dice distance. \nSegment %d (File 1): %s \nSegment %d (File 2): %s \nValue: %f\n", i, s1->text, j, s2->text, Dice(s1, s2));
            }

            else {
                D[i][j] = Dice(s1, s2);        // we fill the matrix with the Dice distance between each couple of Segments

                //--- to display similar lines detected in files, supress the next comment part and the one with "$$" above (ctrl+f to find it)
                /*if (Dice(s1, s2) == 0) {
                    if (first == 0) {
                        first++;
                        printf("List of identical lines in files:\n");
                    }
                    printf("* %s (line #%d) and %s (line #%d)\n", s1->original_text, s1->original_line, s2->original_text, s2->original_line);
                }*/

            }
            s2 = s2->next;
        }

        s1 = s1->next;
        s2 = L2->head;
    }

    printf("\n");
    return D;
}


float** LevMatCreate(const List* L1, const List* L2) {
    int n = ListSize(L1);
    int m = ListSize(L2);
    if ((n < 0) ||(m < 0)) {
        return NULL;
    }

    //$$int first=0;                            // counts if we already found two equal segments

    float** D;                                  // n * m matrix
    D = malloc(n * sizeof(float*));             // allocates n pointeurs for lines of ints

    if (D == NULL) {
        printf("ERROR: problem to allocate memory in LevMat\n");
        return NULL;
    }

    for (int i = 0 ; i < n ; i++) {
        D[i] = malloc(m * sizeof(float));       // allocates m ints per line
        if (D[i] == NULL) {
            printf("ERROR: problem to allocate memory in LevMat\n");
            return NULL;
        }
    }

    Segment* s1 = L1->head;
    Segment* s2 = L2->head;
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < m ; j++) {
            if ((Levenshtein(s1->text, s2->text) < 0) || (Levenshtein(s1->text, s2->text) > 1)) {
                printf("Non valide Levenshtein Distance. \nSegment %d (File 1): %s \nSegment %d (File 2): %s \nValue: %f\n",i, s1->text, j, s2->text, Levenshtein(s1->text, s2->text));
            }

            else {
                D[i][j] = Levenshtein(s1->text, s2->text);          // we fill the matrix with the distance between each Segment
                //--- to display similar lines detected in files, supress the next comment part and the one with "$$" above (ctrl+f to find it)
                /*if (Levenshtein(s1, s2) == 0) {
                    if (first == 0) {
                        first++;
                        printf("List of identical lines in files:\n");
                    }
                    printf("* %s (line #%d) and %s (line #%d)\n", s1->original_text, s1->original_line, s2->original_text, s2->original_line);
                }*/
            }

            s2 = s2->next;
        }

    s1 = s1->next;
    s2 = L2->head;
    }

    return D;
}


int pgmCreate(const char* name, float** D, int n, int m) {
    FILE* f = fopen(name, "w");         // we first write the PGM's attributes
    fputs("P2\n", f);
    fprintf(f, "%d %d\n%d\n", n, m, 255);

    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < m ; j++) {
            int a = (255 - (D[i][j]) * 255);            // the PGM's content: the distance * 100
            fprintf(f, "%d ", a);
            if (j == m-1) {
                fputs("\n", f);
            }
        }
    }

    int r = fclose(f);
    if (r != 0) {
        printf("ERROR: the \"%s\" in pgmCreate doesn't properly close\n", name);
        return 0;
    }

    return 1;
}


// looks for the smallest coefficient in the matrix
float matrix_min(float** D, int n, int m, int* ind1, int* ind2) {
    float min = 1.0;
    *ind1 = 0;
    *ind2 = 0;
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < m ; j++) {
            if (D[i][j] < min) {
                min = D[i][j];
                *ind1 = i;
                *ind2 = j;
            }
        }
    }
    return min;
}


float** CopyMat(float** D, int n, int m) {
    float** C;                                  // we create an n * m matrix
    C = malloc(n * sizeof(float*));             // of n lines

    if (C == NULL) {
        printf("ERROR: problem to allocate memory in CopyMat\n");
        return NULL;
    }

    for (int i = 0 ; i < n ; i++) {
        C[i] = malloc(m * sizeof(float));       // each line contains m spaces
        if (C[i] == NULL) {
            printf("ERROR: problem to allocate memory in CopyMat\n");
            return NULL;
        }
    }
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < m ; j++) {
            C[i][j] = D[i][j];                  // we copy one matrix' coefficients to another
        }
    }

    return C;
}


void greedy(float** C, float** D, int n, int m) {
    int* ind1 = malloc(sizeof(int));
    int* ind2 = malloc(sizeof(int));

    *ind1 = 0;                              // indices du coeff min, modifie par min
    *ind2 = 0;

    int stop = minimum(n, m);               // we need to have as many minumums as the fewer number of Segments in any List
    int cpt = 0;                            // counts how many minimums we have now

    while (cpt < stop) {
        matrix_min(D, n, m, ind1, ind2);    // we have the actual's matrix smallest value
        cpt++;
        for (int i = 0 ; i < n ; i++) {
            for (int j = 0 ; j < m ; j++) {
                if ((i == *ind1) || (j == *ind2)) {     // assures that that coefficient as one of the coordinates, but not both
                    if ((i != *ind1) || (j != *ind2)) {
                        C[i][j] = 1;
                        D[i][j] = 1;
                    }
                    else {
                        D[i][j] = 2;        // we remove that minimum so we can pick another one next
                    }
                }
            }
        }
    }

    free(ind1);
    free(ind2);
}


// detects the similar Segments and places them in another matrix
float** postfilter(float** C, int n, int m) {
    float f = 0;                            // corresponds to F's coefficient
    int cpt = 0;                            // counts C's coefficients summed in F
    float** F;                              // an n * m matrix
    F = malloc(n * sizeof(float*));         // allocates n lines for floats
    if (F == NULL) {
        printf("ERROR: problem to allocate memory in Postfilter\n");
        return NULL;
    }

    for (int i = 0 ; i < n ; i++) {
        F[i] = malloc(m * sizeof(float));   // allocates m floats per line
        if (F[i] == NULL) {
            printf("ERROR: problem to allocate memory in Postfilter\n");
            return NULL;
        }
    }

    for (int i = 0 ; i < n ; i++) {          // we calculate the coefficient at the "middle" of the matrix
        for (int j = 0 ; j < m ; j++) {
            for (int k = -2 ; k <= 2 ; k++) {
                if ((i+k >= 0) && (i+k < n) && (j+k >= 0) && (j+k < m)) {       // we only sum the values on the diagonal
                    f += C[i+k][j+k];
                        cpt++;                                      // we count how many values we sum
                }
            }

            f = f/cpt;
            if (f < 0.7) F[i][j] = f;
            else F[i][j] = 1;

            f = 0;
            cpt = 0;
        }
    }

    return F;
}


float output(float** F, int n, int m) {
    float min = minimum(m, n);
    float sum = 0;
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < m ; j++) {
            sum += 1 - F[i][j];
        }
    }

    float delta = 1 - sum/min;
    return delta;
}



int main(int argc, char* argv[]) {

    if ((argv[1] == NULL) || (argv[2] == NULL)) {
        printf("At least two files needed in input\n");
        return 0;
    }

    if (!strcmp(argv[1], argv[2])) {
        printf("Please give two different files to compare\n");
        return 0;
    }

    if (argc >= 3) {
        int i = 1;
        float** mat = malloc(sizeof(float*) * (argc-1));    // creates the square matrix to compare every file with each other
        for (int k = 0 ; k < argc-1 ; k++) {
            mat[k] = malloc(sizeof(float) * (argc-1));      // every file is compared with the next ones
        }
        for (int j = 0 ; j < argc-1 ; j++) {
            mat[j][j] = 0;                                  // the diagonal is full of zeros (comparison between the file and itself)
        }

        while (i < argc) {
            for (int k = i + 1 ; k < argc ; k++) {
                List* L1 = pre_treatment(argv[i]);
                List* L2 = pre_treatment(argv[k]);
                //---float** D = LevMatCreate(L1, L2);
                float** D = DiceMatCreate(L1, L2);          //---Remove comment to use Dice distance

                int n = ListSize(L1);
                int m = ListSize(L2);
                if ((n < 0) ||(m < 0)) {
                    return 0;
                }

                int total = strlen("dice") + strlen(argv[i]) + strlen(argv[k]) + 7;              // size of the final name
                char* final = malloc(sizeof(char) * total);
                total = 0;
                NamePGM(final, "dice", argv[i], argv[k]);
                int r = pgmCreate(final, D, n, m);
                if (r != 1) {
                    free(final);
                    free_Mat(D, n, m);
                    free_Mat(mat, argc-1, argc-1);
                    free_List(L1);
                    free_List(L2);
                    return -1;
                }
                float** C = CopyMat(D, n, m);

                greedy(C, D, n, m);
                total = strlen("couplage") + strlen(argv[i]) + strlen(argv[k]) + 7;
                final = realloc(final, sizeof(char) * total);
                total = 0;
                NamePGM(final, "couplage", argv[i], argv[k]);
                r = pgmCreate(final, C, n, m);
                if (r != 1) {
                    free_Mat(D, n, m);
                    free_Mat(C, n, m);
                    free_Mat(mat, argc-1, argc-1);
                    free_List(L1);
                    free_List(L2);
                    free(final);
                    return -1;
                }

                //post-filtrage
                float** F = postfilter(C, n, m);
                total = strlen("filtr") + strlen(argv[i]) + strlen(argv[k]) + 7;
                final = realloc(final, sizeof(char) * total);
                total = 0;
                NamePGM(final, "filtr", argv[i], argv[k]);
                r = pgmCreate(final, F, n, m);
                if (r != 1) {
                    free_Mat(D, n, m);
                    free_Mat(C, n, m);
                    free_Mat(F, n, m);
                    free_List(L1);
                    free_List(L2);
                    free(final);
                    free_Mat(mat, argc-1, argc-1);
                    return -1;
                }

                float d = output(F, n, m);
                printf("\nResemblance rate between \"%s\" and \"%s\": %.2f%% (distance: %.2f)\n", argv[i], argv[k], (1-d)*100,d);

                // we free all the memory
                free(final);
                free_Mat(D, n, m);
                free_Mat(C, n, m);
                free_Mat(F, n, m);
                free_List(L1);
                free_List(L2);

                mat[i-1][k-1] = d;
                mat[k-1][i-1] = d;
            }

            i ++;           // to the next file in argv[]
        }

        if (argc > 3) {
            printf("\nResemblance matrix of the %d files:\n", argc-1);
            printmat(mat, argc-1, argc-1);
        }
        free_Mat(mat, argc-1, argc-1);

        return 0;
    }

}
