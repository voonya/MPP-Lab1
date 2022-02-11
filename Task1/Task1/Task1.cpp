#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

const string FILENAME = "input.txt";
const int countWordsToOutput = 25;

int main()
{
    // initializing vars

    int arrayLength = 25;
    int countWordsInArray = 0;

    int index = 0;
    int j = 0;

    string* words = new string[arrayLength];
    int* counts = new int[arrayLength];


    const int countStopWord = 9;
    string stopWords[countStopWord] = { "the", "and", "a", "an", "to", "in", "at", "for", "of" };


    bool isStopWord;
    bool isEqual;
    string inputBuffer;
    string processedWord;


    string bufferWord;
    int bufferCount;
    // open file to read
    ifstream input(FILENAME);
    if (input) {
        // cycle to proccess input data
    inputCycle:
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

            if (processedWord == "") {
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
                        counts[index]++;
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
                int* new_counts = new int[arrayLength];

                index = 0;
            fillNewArray:
                if (index < countWordsInArray) {
                    new_words[index] = words[index];
                    new_counts[index] = counts[index];

                    index++;
                    goto fillNewArray;
                }
                delete[]words;
                delete[]counts;
                words = new_words;
                counts = new_counts;
            }
            // add new word
            words[countWordsInArray] = processedWord;
            counts[countWordsInArray] = 1;
            countWordsInArray++;
            goto inputCycle;
        }

        input.close();
    }
    // sort arrrays

    index = 0;
outerCycle:
    if (index < countWordsInArray - 1) {
        j = 0;
    innerCycle:
        if (j < countWordsInArray - index - 1) {
            if (counts[j] < counts[j + 1]) {
                bufferWord = words[j];
                words[j] = words[j + 1];
                words[j + 1] = bufferWord;

                int bufferCount = counts[j];
                counts[j] = counts[j + 1];
                counts[j + 1] = bufferCount;
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
    if (index < countWordsInArray && index < countWordsToOutput) {
        outResult << words[index] << " - " << counts[index] << endl;
        index++;
        goto outputCycle;
    }
    outResult.close();
    return 0;
}

