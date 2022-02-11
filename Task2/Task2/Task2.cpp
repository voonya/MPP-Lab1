#include <iostream>
#include <fstream>


using namespace std;
const string FILENAME = "input.txt";
const int maxCountPages = 100;
const int rowsInPage = 45;

int main()
{
    // initialize
    int arrayLength = 25;
    int** pages = new int* [arrayLength];
    string* words = new string[arrayLength];
    int* counts = new int[arrayLength];
    int currentRow = 1;
    int index = 0;
    int j = 0;
    int letter = 0;
    int countWordsInArray = 0;

    bool needSwap;
    const int countStopWord = 9;
    string stopWords[countStopWord] = { "the", "and", "a", "an", "to", "in", "at", "for", "of" };


    bool isStopWord;
    bool isEqual;
    string inputBuffer;
    string processedWord;


    string bufferWord;
    int bufferCount;
    int* bufferPages;
fillPages:
    if (index < arrayLength) {
        pages[index] = new int[102]; // first element is a count of pages in array

        index++;
        goto fillPages;
    }
    // open file
    ifstream input(FILENAME);

    // cycle to proccess input data
inputCycle:
    // check if it is an end of line
    if (input.peek() == '\n') {
        currentRow++;
        input.get();
        goto inputCycle;
    }
    // read word
    if (input >> inputBuffer) {
        // normalize word
        index = 0;
        processedWord = "";
    prepareWord:
        if (inputBuffer[index] != '\0') {
            if (inputBuffer[index] >= 'A' && inputBuffer[index] <= 'Z' ||
                inputBuffer[index] >= 'a' && inputBuffer[index] <= 'z' ||
                (inputBuffer[index] == '-' && index != 0)
                ) {
                if (inputBuffer[index] >= 'A' && inputBuffer[index] <= 'Z') {
                    processedWord += 'a' - 'A' + inputBuffer[index];
                }
                else {
                    processedWord += inputBuffer[index];
                }

            }

            index++;
            goto prepareWord;
        }

        if (processedWord == "" || processedWord[0] > 'z' || processedWord[0] < 'a') {
            goto inputCycle;
        }
        // check is it stop-word
        index = 0;
        isStopWord = false;
    checkStopWord:
        if (index < countStopWord) {
            j = 0;
            isStopWord = true;
        checkCycle:
            if (stopWords[index][j] != '\0' && processedWord[j] != '\0') {
                if (stopWords[index][j] != processedWord[j]) {
                    isStopWord = false;
                    goto continueCheck;
                }
                j++;
                goto checkCycle;
            }
            if (stopWords[index][j] != '\0' && processedWord[j] == '\0' ||
                stopWords[index][j] == '\0' && processedWord[j] != '\0') {
                isStopWord = false;
            }
        continueCheck:
            if (isStopWord) {
                goto inputCycle;
            }
            index++;
            goto checkStopWord;
        }


        // check if this word is already exist in array
        index = 0;
    inArrayCycle:
        if (index < countWordsInArray) {
            if (index < countWordsInArray) {
                j = 0;
                isEqual = true;
            checkCycle2:
                if (words[index][j] != '\0' && processedWord[j] != '\0') {
                    if (words[index][j] != processedWord[j]) {
                        isEqual = false;
                        goto continueCheck2;
                    }
                    j++;
                    goto checkCycle2;
                }
                if (words[index][j] != '\0' && processedWord[j] == '\0' ||
                    words[index][j] == '\0' && processedWord[j] != '\0') {
                    isEqual = false;
                }
            continueCheck2:
                if (isEqual) {
                    // add page
                    if (pages[index][0] < maxCountPages + 1) {
                        j = ++pages[index][0];
                        pages[index][j] = currentRow / rowsInPage + 1;
                    }
                    goto inputCycle;
                }
                index++;
                goto inArrayCycle;
            }
        }
        // check if array needs to relocate
        if (countWordsInArray > arrayLength * 0.7) {
            arrayLength *= 2;
            string* new_words = new string[arrayLength];
            int** new_pages = new int* [arrayLength];

            index = 0;
        fillNewArray:
            if (index < countWordsInArray) {
                new_words[index] = words[index];
                new_pages[index] = pages[index];

                index++;
                goto fillNewArray;
            }
        fillFullArray:
            if (index < arrayLength) {
                new_pages[index] = new int[maxCountPages + 2];
                index++;
                goto fillFullArray;
            }
            delete[]words;
            delete[]pages;
            words = new_words;
            pages = new_pages;
        }
        // add new word
        words[countWordsInArray] = processedWord;
        pages[countWordsInArray][0] = 1;
        pages[countWordsInArray][1] = currentRow / rowsInPage + 1;
        countWordsInArray++;
        goto inputCycle;
    }
    input.close();
    // sort arrrays

    index = 0;
outerCycle:
    if (index < countWordsInArray - 1) {
        j = 0;
    innerCycle:
        if (j < countWordsInArray - index - 1) {
            letter = 0;
            needSwap = false;
        cmpWords:
            if (words[j][letter] != '\0' && words[j + 1][letter] != '\0') {
                if (words[j][letter] > words[j + 1][letter]) {
                    needSwap = true;
                    goto swap;
                }
                else if (words[j][letter] == words[j + 1][letter]) {
                    letter++;
                    goto cmpWords;
                }

            }
            if (words[j + 1][letter] == '\0' && words[j][letter] != '\0') {
                needSwap = true;
            }
        swap:
            if (needSwap) {
                bufferWord = words[j];
                words[j] = words[j + 1];
                words[j + 1] = bufferWord;

                bufferPages = pages[j];
                pages[j] = pages[j + 1];
                pages[j + 1] = bufferPages;
            }
            j++;
            goto innerCycle;
        }
        index++;
        goto outerCycle;
    }
    // output arrays in file
    ofstream outResult("result.txt");
    index = 0;
outputCycle:
    if (index < countWordsInArray) {
        if (pages[index][0] < maxCountPages + 1) {
            outResult << words[index] << " - ";
            j = 1;
        outPages:
            if (j < pages[index][0]) {
                outResult << pages[index][j] << ", ";
                j++;
                goto outPages;
            }
            outResult << pages[index][j] << endl;
        }

        index++;
        goto outputCycle;
    }
    outResult.close(); 
    return 0;
}


