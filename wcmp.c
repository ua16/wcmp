#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#include <windows.h>

#include "include/raudio.h"
#include "curses.h"



// FUNCTIONS FOR DEAILING WITH TRACK NAMES -------------------------------------------------//

int checkIfValidFile(const char *fileName, int fileTypes) {
    // 1 for Audio 2 for playlists. 
    // Check if the file is a supported audio file

    if (fileName[0] == '\0') {
        return 1;
    }

    int i = 0;
    while (fileName[i] != '\0'){
        i++;
    }

    i = i - 4;
    
    if (i < 0) {
        return 1;
    }

    char fileExtension[5];

    strncpy(fileExtension,fileName+(i), 5);

    if (fileTypes == 1) {

        if (
                strcmp(fileExtension, ".wav") == 0 ||
                strcmp(fileExtension, ".mp3") == 0 ||
                strcmp(fileExtension, ".ogg") == 0 ||
                strcmp(fileExtension, "flac") == 0 ||
                strcmp(fileExtension, ".qoa") == 0 ||
                strcmp(fileExtension, ".mod") == 0 ||
                strcmp(fileExtension + 1, ".xm") == 0
           ){
            return 0;
        }
        else {
            return 1;
        }
    }
    if (fileTypes == 2) {
        if (strcmp(fileExtension, ".pla") == 0){
            return 0;
        }
        else {
            return 1;
        }
    }
}


// DEALING WITH THE FILESYSTEM -------------------------------------------------------------//

void listDir(const char *path) { 
    WIN32_FIND_DATA findFileData; // Basically currentFile like returned from readdir
    HANDLE hFind = INVALID_HANDLE_VALUE; // windows specific thing that's used to keep a directory open. 

    char searchPath[MAX_PATH];

    snprintf(searchPath, sizeof(searchPath), "%s\\*", path);

    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Some error %lu\n", GetLastError());
    }

    do {

        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
            continue;
        }

        printf("%s ", findFileData.cFileName); 

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            printf(" -> Directory\n");
            char subPath[MAX_PATH];

            snprintf(subPath, sizeof(subPath), "%s\\%s", path, findFileData.cFileName);

            listDir(subPath);
        } else {
            printf(" -> File \n");
        }
        
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}


void getListOfFiles(const char *path, char (*fileList)[MAX_PATH], char (*playlistList)[MAX_PATH], int *currentListIndex, int *currentPlayListIndex){

    WIN32_FIND_DATA findFileData; // Basically currentFile like returned from readdir
    HANDLE hFind = INVALID_HANDLE_VALUE; // windows specific thing that's used to keep a directory open. 

    char searchPath[MAX_PATH];

    snprintf(searchPath, sizeof(searchPath), "%s\\*", path);

    hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Some error %lu\n", GetLastError());
    }

    do {

        // skip over previous directory and current directory
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
            continue;
        }


        char subPath[MAX_PATH];
        snprintf(subPath, sizeof(subPath), "%s\\%s", path, findFileData.cFileName);


        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            getListOfFiles(subPath, fileList, playlistList, currentListIndex, currentPlayListIndex);
        } else {
            if (checkIfValidFile(findFileData.cFileName, 1) == 0) {
                // Add this song to the list 
                strcpy(fileList[*currentListIndex], subPath);
                *currentListIndex = *currentListIndex + 1;
            }
            if (checkIfValidFile(findFileData.cFileName, 2) == 0) {
                // Add this song to the list 
                strcpy(playlistList[*currentPlayListIndex], subPath);
                *currentPlayListIndex = *currentPlayListIndex + 1;
            }
        }
        
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

}



// ARRAY HELPER FUNCTIONS ------------------------------------------------------------------//

void initializeTrackArray ( char (*array)[MAX_PATH], int legnth) {
    int i = 0; 
    while (i < legnth) {
        array[i][0] = '\0';
        i++;
    }
}


void printStringArray (char (*array)[MAX_PATH] ) {
    int i = 0;
    while (array[i][0] != '\0') {
        puts(array[i]);
        i++;
    }
}

void appendToTrackList ( char (*array)[MAX_PATH], const char *fileName) {
    // Call only after initializeTrackArray

    int i = 0;
    while (array[i][0] != '\0') {
        i++;
    }
    strncpy(array[i], fileName, MAX_PATH);
}

void popFirstFromTrackList (char (*array)[MAX_PATH]) {
    // Remove the first element and move the other ones up
    int i = 0;
    while (array[i + 1][0] != '\0'){
        strcpy(array[i], array[i + 1]);
        i++;
    }
    array[i][0] = '\0';
}


// OTHER HELPER FUNCTIONS ------------------------------------------------------------------//

int kbhit(void)
{
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

void formatTimeString(float time, char *string) {
    // Formats time like 1:30
    int iTime = time;
    snprintf(string, 20, "%02d:%02d", (iTime/60), (iTime%60));
}

int shouldIncScrollOffset(int currentScroll, int maxY ) {
    if (currentScroll < (maxY / 2) ) {
        return 0;
    }
    else {
        return 1;
    }
}

int shouldDecScrollOffset(int offset){
    if (offset > 0) {
        return 1;
    }
    else {
        return 0;
    }
}

void adjustScrollOffset (int *offset, int currentScroll, int maxY, int add) {
    // add is whether to increment or decrement (1 for +1, 0 for -1)
    // set to 0
    if (currentScroll < maxY/2) {
        *offset = 0;
        return;
    }
    // increment
    if ((currentScroll > maxY/2) & (add == 1)) {
        *offset = *offset + 1;
        return;
    }
    // decrement
    if ((add == 0) & (*offset > 0)){
        *offset = *offset - 1;
        return;
    }
}

int rowHighlightCalc(int row, int offset) {
    if (offset == 0) {
        return row;
    }
    else {
        return row + offset;
    }
}

void insertIntoArrayAtN(char (*array)[MAX_PATH], int nIndex, const char *string, int arrayLength){
    // Replace the Nth element and shift others accordingly
    // Make sure not to pass an nIndex surrounded by null characters. 

    int i = nIndex;
    char swap[MAX_PATH];
    strcpy(swap, array[nIndex]);
    strcpy(array[nIndex], string);

    char swap2[MAX_PATH];
    while ((array[i][0] != '\0') & (i < arrayLength)){
        strcpy(swap2, array[i+1]);
        strcpy(array[i+1], swap);
        strcpy(swap, swap2);
        i++;
    }
}

void removeNthFromArray(char (*array)[MAX_PATH], int nIndex, int arrayLength){
    int i = nIndex;
    if (nIndex == arrayLength) {
        array[i][0] = '\0';
    }
    while ((array[i][0] != '\0') & (i < arrayLength)){
        strcpy(array[i], array[i+1]);
        i++;
    }
}

bool isValidFileTextChar(int character){
    if ( 
            (character >= 65 & character <= 90) || // Upper Case letters 
            (character >= 97 & character <= 122) || // Lower Case letters 
            (character == 32) || // Space
            (character >= 48 & character <= 57) || // Numbers
            (character == 40 || character == 41) // Parentheses
       ) {
        return true;
    }
    else {
        return false;
    }
}

void popLastChar (char *string){
    int i = 0;
    while (string[i] != '\0') {
        i++;
    }
    string[i-1] = '\0';
}

void removeNewline(char *string){
    int i = 0;
    while (string[i] != '\0') {
        if (string[i] == '\n'){
            string[i] = '\0';
        }
        i++;
    }
}

void printListToFile(char (*array)[MAX_PATH], const char *fileName) {
    // Create a playlist, fileName is a full Path
    FILE *playListFile;
    char fullFilePath[MAX_PATH];
    snprintf(fullFilePath, sizeof(fullFilePath), "%s", fileName);

    playListFile = fopen(fullFilePath, "w");

    int i = 0;
    while (array[i][0] != '\0'){
        fprintf(playListFile, "%s\n", array[i]);
        i++;
    }
    printf("Wrote to %s\n", fullFilePath);
    fclose(playListFile);
}

void loadPlayList(char (*array)[MAX_PATH], const char *fileName){
    // Load playlist line by line and append to playQueue
    FILE *playListFile;
    playListFile = fopen(fileName, "r");

    int i = 0;
    while (array[i][0] != '\0') {
        i++;
    }
    
    // Load the lines into the queue
    char songPath[MAX_PATH];

    while (fgets(songPath, sizeof(songPath), playListFile)) {
        removeNewline(songPath);
        strncpy(array[i], songPath, MAX_PATH);
        i++;
    }

    fclose(playListFile);
}


// MAIN FUNCTION ---------------------------------------------------------------------------//


int main() {

    // INITIALIZE --------------------------------------------------------------------------//


    // Load Music Files And Playlists -----------  //
    int maxFiles = 400;
    char fileList[maxFiles][MAX_PATH];
    char playlists[maxFiles][MAX_PATH]; // A list of playlists
    int currentFileListIndex = 0;
    int currentPlaylistListIndex = 0;

    initializeTrackArray(fileList, maxFiles);
    initializeTrackArray(playlists, maxFiles);

    char musicDirPath[MAX_PATH];
    GetEnvironmentVariable("USERPROFILE", musicDirPath, MAX_PATH);

    strcat(musicDirPath, "\\Music");

    getListOfFiles(
            musicDirPath,
            fileList,
            playlists,
            &currentFileListIndex,
            &currentPlaylistListIndex
            );

    int dirPathOffset = strlen(musicDirPath) + 1; // We use this to omit the C://Whatever/Music

    currentFileListIndex = 0;

    // Start up the audio device ----------------  //

    InitAudioDevice();

    // Initialize pdcurses ----------------------  //

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    curs_set(0);
    scrollok(stdscr, FALSE);

    int screenY, screenX; // Remember y gets passed before x with curses
    getmaxyx(stdscr, screenY, screenX);



    // Initialize Colors ------------------------  //

    start_color();
    init_pair(42, COLOR_BLACK, COLOR_YELLOW);
    init_pair(43, COLOR_BLACK, COLOR_GREEN);

    // Initialize UI Elements -------------------  //

    char windowTitle[100];
    strcpy(windowTitle, "Windows Curses Music Player");

    char currentSong[MAX_PATH];
    strcpy(currentSong, fileList[0]);


    float timePlayed = 0.0f; // Time played in seconds 
    char strTimePlayed[20];
    formatTimeString(timePlayed, strTimePlayed);
    int updateTimePlayed = 0; //Update time at 500

    float currentSongLength = 0.0f;
    char strCurrentSongLength[20];
    formatTimeString(currentSongLength, strCurrentSongLength);

    bool pause = true;

    char cutBuffer[MAX_PATH];

    // Manage entering text 
    bool textEnter = false; // If text is being entered blocks other key presses
    char textInputBuffer[50]; // Stores the text into the buffer 
    memset(textInputBuffer, '\0', sizeof(textInputBuffer));
    
    // 1 for library, 2 for play queue
    int currentMenu = 1;

    // list scroll amount
    int listScroll = 0;
    int scrollOffset = 0;
    int prevScrollOffset = 0;



    // Initialize play queue --------------------  //

    char playQueue[maxFiles][MAX_PATH];
    initializeTrackArray(playQueue, MAX_PATH);
    int playQueueLength = 0;

    // MAIN LOOP STUFF ---------------------------------------------------------------------//

    // Initialize music so we can load into it
    // music is basically whatever stream is loaded and playing 
    Music music = LoadMusicStream(fileList[0]);
    PlayMusicStream(music);
    PauseMusicStream(music);
    currentSongLength = GetMusicTimeLength(music);
    formatTimeString(currentSongLength, strCurrentSongLength);



    // set up for the loop 
    static unsigned char key = 0;
    static unsigned char inputKey = 0; // Used for exclusively for text input

    // 27 is the ESC key
    while (key != 27) {

        key = 0; 

        if (kbhit()) key = getch();

        // Allow for text input 
        if (textEnter == true) {
            inputKey = key; 
            key = 0;
        }

            
        // Pausing 
        if (key == ' ') {
            if (pause == false) {
                PauseMusicStream(music);
            }
            else {
                ResumeMusicStream(music);
            }
            pause = !pause;
        }

        // scrolling 

        prevScrollOffset = scrollOffset;

        if ((key == 'j') & (listScroll < maxFiles - 1)){

            if ((currentMenu == 1) & (fileList[listScroll + 1][0] != '\0')){
                listScroll++;
                adjustScrollOffset(&scrollOffset, listScroll, screenY, 1);
            }
            if ((currentMenu == 2) & (playQueue[listScroll + 1][0] != '\0')){
                listScroll++;
                adjustScrollOffset(&scrollOffset, listScroll, screenY, 1);
            }
            if ((currentMenu == 3) & (playlists[listScroll + 1][0] != '\0')){
                listScroll++;
                adjustScrollOffset(&scrollOffset, listScroll, screenY, 1);
            }
        }
        if ((key == 'k') & (listScroll > 0)) {
            listScroll--;
            adjustScrollOffset(&scrollOffset, listScroll, screenY, 0);
        }


        // Play on pressing enter 
        if (key == '\n') {
            // Library Menu
            if ((currentMenu == 1) & (fileList[listScroll][0] != '\0')) {
                UnloadMusicStream(music);
                music = LoadMusicStream(fileList[listScroll]);
                strcpy(currentSong, fileList[listScroll]);
                currentSongLength = GetMusicTimeLength(music);
                formatTimeString(currentSongLength, strCurrentSongLength);

                PlayMusicStream(music);
                clear();
            }
            // Play Queue Menu
            if ((currentMenu == 2) & (playQueue[listScroll][0] != '\0')){
                UnloadMusicStream(music);
                music = LoadMusicStream(playQueue[listScroll]);
                strcpy(currentSong, playQueue[listScroll]);
                currentSongLength = GetMusicTimeLength(music);
                formatTimeString(currentSongLength, strCurrentSongLength);

                PlayMusicStream(music);
                clear();
            }
        }

        // Library Screen Logic ------------------ //

        if (key == 'a' & currentMenu == 1) { 
            appendToTrackList(playQueue, fileList[listScroll]);
        }

        // Play Queue Screen Logic --------------- //

        if (key == 'd' & currentMenu == 2 & playQueue[listScroll][0] != '\0') {
            strcpy(cutBuffer, playQueue[listScroll]);
            removeNthFromArray(playQueue, listScroll, maxFiles);
            if (listScroll > 0){
                listScroll--;
            }
        }

        if (key == 'p' & currentMenu == 2) {
            insertIntoArrayAtN(playQueue, listScroll + 1, cutBuffer, maxFiles);
        }

        if (key == 'P' & currentMenu == 2 ) {
            insertIntoArrayAtN(playQueue, listScroll , cutBuffer, maxFiles);
        }

        // Playlist Loading Screen Logic --------- //

        if (key == 'a' & currentMenu == 3) {
            loadPlayList(playQueue, playlists[listScroll]);
        }

        if (key == '\n' & currentMenu == 3) {
            initializeTrackArray(playQueue, maxFiles);
            loadPlayList(playQueue, playlists[listScroll]);
            key = 'n';
        }

        // Playlist Creation Screen Logic -------- //

        if (inputKey == 27) {
            textEnter = false;
            curs_set(0);
        }

        if (isValidFileTextChar(inputKey)) {
            snprintf(textInputBuffer, sizeof(textInputBuffer) - 3, "%s%c", textInputBuffer, inputKey);
        }

        if (inputKey == 8){
            popLastChar(textInputBuffer);
            clear();
        }

        if (inputKey == '\n' & currentMenu == 4 & textInputBuffer[0] != '\0'){
            char playListPath[MAX_PATH];
            snprintf(playListPath, MAX_PATH - 1, "%s\\%s.pla", musicDirPath, textInputBuffer);
            printListToFile(playQueue, playListPath);
            textEnter = false;
            curs_set(0);
            currentMenu = 2;
            inputKey = 0;
            clear();

            // Add to list of playlists 
            appendToTrackList(playlists, playListPath);
        }


        // Menu Switching Logic ------------------ //
        // 1 takes you to library 
        // 2 takes you to play queue
        // 3 takes you to play list library 
        // 4 is playlist creation 
        // 5 is search
        if (key == '1') {
            currentMenu = 1;
            clear();
            listScroll = 0;
            scrollOffset = 0;
        }
        if (key == '2') {
            currentMenu = 2;
            clear();
            listScroll = 0;
            scrollOffset = 0;
        }

        if (key == '3') {
            currentMenu = 3;
            clear();
            listScroll = 0;
            scrollOffset = 0;
        }

        if (key == 's') {
            currentMenu = 4;
            // Play list function 
            // Start with string input
            textEnter = true;
            clear();
            listScroll = 0;
            scrollOffset = 0;
            curs_set(1);
        }

        // Screen indpenedent Logic -------------- //

        if (key == 'n') {
            // Change Song Location
            if (playQueue[0][0] != '\0'){
                UnloadMusicStream(music);
                music = LoadMusicStream(playQueue[0]);
                strcpy(currentSong, playQueue[0]);
                popFirstFromTrackList(playQueue);
                currentSongLength = GetMusicTimeLength(music);
                formatTimeString(currentSongLength, strCurrentSongLength);

                PlayMusicStream(music);
                clear();
            }
        }

        // Update variables

        UpdateMusicStream(music);

        timePlayed = GetMusicTimePlayed(music);
        formatTimeString(timePlayed, strTimePlayed);

        if (timePlayed > currentSongLength - 0.9) {
            if (playQueue[0][0] != '\0'){
                UnloadMusicStream(music);
                music = LoadMusicStream(playQueue[0]);
                strcpy(currentSong, playQueue[0]);
                popFirstFromTrackList(playQueue);
                currentSongLength = GetMusicTimeLength(music);
                formatTimeString(currentSongLength, strCurrentSongLength);

                PlayMusicStream(music);
                clear();
            }
        }


        // Print Stuff to Screen with curses ----- //

        if (key != 0) { clear(); }

        attron(COLOR_PAIR(42));
        mvprintw(0, 0, windowTitle);
        attroff(COLOR_PAIR(42));

        char timeStat[25];
        snprintf(timeStat, 25, "%s / %s", strTimePlayed, strCurrentSongLength);

        attron(A_REVERSE);
        mvprintw(screenY - 1, 0, currentSong + dirPathOffset);
        mvprintw(screenY - 1, screenX - 15, timeStat);
        attroff(A_REVERSE);
        

        // Print Song List 
        if (currentMenu == 1) {

            mvprintw(1, 0, "Library");

            int row = 0;
            while ((row < screenY - 3) & (fileList[row + scrollOffset][0] != '\0')) {

                if (rowHighlightCalc(row, scrollOffset) == listScroll) {
                    attron(COLOR_PAIR(43));
                    mvprintw(row + 2, 0, fileList[row + scrollOffset] + dirPathOffset);
                    attroff(COLOR_PAIR(43));
                }
                else {
                    mvprintw(row + 2, 0, fileList[row + scrollOffset] + dirPathOffset);
                }
                row++;
            }
        }

        if (currentMenu == 2) {
            mvprintw(1, 0, "Play Queue");

            int row = 0;
            while ((row < screenY - 3) & (playQueue[row + scrollOffset][0] != '\0')) {

                if (rowHighlightCalc(row, scrollOffset) == listScroll) {
                    attron(COLOR_PAIR(43));
                    mvprintw(row + 2, 0, playQueue[row + scrollOffset] + dirPathOffset);
                    attroff(COLOR_PAIR(43));
                }
                else {
                    mvprintw(row + 2, 0, playQueue[row + scrollOffset] + dirPathOffset);
                }
                row++;
            }
        }

        if (currentMenu == 3) {
            mvprintw(1, 0, "Playlists");

            int row = 0;
            while ((row < screenY - 3) & (playlists[row + scrollOffset][0] != '\0')) {

                if (rowHighlightCalc(row, scrollOffset) == listScroll) {
                    attron(COLOR_PAIR(43));
                    mvprintw(row + 2, 0, playlists[row + scrollOffset] + dirPathOffset);
                    attroff(COLOR_PAIR(43));
                }
                else {
                    mvprintw(row + 2, 0, playlists[row + scrollOffset] + dirPathOffset);
                }
                row++;
            }
        }

        if (currentMenu == 4) { 
            mvprintw(1,0, "Create new playlist from play queue");
            mvprintw(2,0, "Save Playlist: ");
            mvprintw(2, 15, textInputBuffer);
        }

        refresh();
        Sleep(5);

        // End Of Redrawing the screen
    }


    // UNLOAD AND DEINITIALIZE -------------------------------------------------------------//


    // Unload the audio stuff -------------------  //
    UnloadMusicStream(music);
    CloseAudioDevice();

    // Unload the curses stuff ------------------  //
    refresh();
    endwin();

    return 0;
}
