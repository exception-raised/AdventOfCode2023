#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#define MAX_RED   12
#define MAX_GREEN  13
#define MAX_BLUE 14

struct ColorCounts {
    int Red;
    int Green;
    int Blue;
};

typedef struct ColorCounts* ColorSet;

int ReadFromFile(const char* File, char** Buffer) {
    FILE* file;
    file = fopen(File, "r");

    if (file == NULL) {
        perror("Failed to open file.");
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long FileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    *Buffer = (char*)malloc(FileSize + 1);

    if (*Buffer == NULL) {
        perror("Failed to allocate memory.");
        fclose(file);
        return 0;
    }

    fread(*Buffer, 1, FileSize, file);
    (*Buffer)[FileSize] = '\0';
    fclose(file);
    return 1;
}

ColorSet CreateSet(){
    ColorSet Colors = calloc(1, sizeof(struct ColorCounts));
    if (!Colors) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
}

ColorSet GetColors(char* game) {
    ColorSet Colors = CreateSet();
    char *Pattern = "([0-9]+) (red|blue|green)";
    size_t MaxGroups = 3;

    regex_t RegexC;
    regmatch_t GroupArray[MaxGroups];

    if (regcomp(&RegexC, Pattern, REG_EXTENDED)) {
        printf("Could not compile regular expression.\n");
        return Colors;
    }

    char *CurrentPosition = game;
    while (regexec(&RegexC, CurrentPosition, MaxGroups, GroupArray, 0) == 0) {
        char MatchValue[20];
        strncpy(MatchValue, CurrentPosition + GroupArray[1].rm_so, GroupArray[1].rm_eo - GroupArray[1].rm_so);
        MatchValue[GroupArray[1].rm_eo - GroupArray[1].rm_so] = '\0'; 

        int quantity = atoi(MatchValue);

        char color[20];
        strncpy(color, CurrentPosition + GroupArray[2].rm_so, GroupArray[2].rm_eo - GroupArray[2].rm_so);
        color[GroupArray[2].rm_eo - GroupArray[2].rm_so] = '\0'; 
        
        if (strcmp(color, "red") == 0 && Colors->Red < quantity) {
            Colors->Red = quantity;
        } else if (strcmp(color, "blue") == 0 && Colors->Blue < quantity) {
            Colors->Blue = quantity;
        } else if (strcmp(color, "green") == 0 && Colors->Green < quantity) {
            Colors->Green = quantity;
        }

        CurrentPosition += GroupArray[0].rm_eo;
    }

    regfree(&RegexC);

    return Colors;
}


int GetGameId(char* game) {
    char *Pattern = "Game ([0-9]+)";
    size_t MaxGroups = 2;

    regex_t RegexC;
    regmatch_t GroupArray[MaxGroups];

    if (regcomp(&RegexC, Pattern, REG_EXTENDED)) {
        printf("Could not compile regular expression.\n");
        return 1;
    }
    int GameId = 0;

    char *CurrentPosition = game;
    while (regexec(&RegexC, CurrentPosition, MaxGroups, GroupArray, 0) == 0) {
        char MatchValue[20];
        strncpy(MatchValue, CurrentPosition + GroupArray[1].rm_so, GroupArray[1].rm_eo - GroupArray[1].rm_so);
        MatchValue[GroupArray[1].rm_eo - GroupArray[1].rm_so] = '\0'; // Null-terminate the extracted value

        GameId = atoi(MatchValue);

        CurrentPosition += GroupArray[0].rm_eo;
    }
    regfree(&RegexC);
    return GameId;
}

int IsImpossible(struct ColorCounts* Colors) {
    return (Colors->Red > MAX_RED || Colors->Green > MAX_GREEN || Colors->Blue > MAX_BLUE);
}


int GetPowerSetOfCubes(struct ColorCounts* Colors) {
    int ColorsMin[3] = {Colors->Red, Colors->Green, Colors->Blue};

    int Result = 1;
    for (int i = 0; i < 3; ++i) {
        if (ColorsMin[i] > 0) {
            Result *= ColorsMin[i];
        }
    }

    return Result;
}

void ProcessLines(char* Buffer, int* PossibleGames, int* MinGames) {
    char* Line = strtok(Buffer, "\n");

    while (Line != NULL) {
        int GameId = GetGameId(Line);
        struct ColorCounts* Colors = GetColors(Line);
        if (!IsImpossible(Colors)) {
            *PossibleGames += GameId;
        }
        int sum = GetPowerSetOfCubes(Colors);
        *MinGames += sum;

        free(Colors); 
        Line = strtok(NULL, "\n");
    }
}



int main(){
    char* Buffer;
    int PossibleGames = 0;
    int MinimumGames  = 0;
    if (ReadFromFile("input.txt", &Buffer)) {
        ProcessLines(Buffer, &PossibleGames, &MinimumGames);
    }
    printf("Possible Games: %d\nMinimum Games: %d\n", PossibleGames, MinimumGames);
    return EXIT_SUCCESS;
}
