/**
* @author Anthony Grieco
* Date: 3/25/2022
* 
* Description: This program simulates how two page replacement algorithms ("Least Recently Used" [LRU] 
*       and "First in First Out" [FIFO]) works with a computer's Visual Memory. Basically, in this 
*       example, the "Frame" is the array structure holding up to five numeric values, which represent 
*       the "Pages" that are derived from a process (running program) when it's executed and requires
*       more memory space than it normally has access to. This program calculates the Number of Page 
*       Faults that occur, the Hit Percentage, and Miss Percentage for the pages as they are sent to the
*       Frame for both page replacement algorithms. A "Page Fault" is defined as an occurrence where the
*       Page (in this case represented by a numeric value set by the user) is not currently loaded in one
*       of the five possible storage locations in the Frame (array) and must instead be retrieved from
*       else where in memory, which takes up extra time. A "Hit" is defined as when the Page in question
*       is already being stored by the Frame, which would save both time and resources in an actual 
*       computer because the computer could then easily call upon that data from its Virtual Memory
*       without having to look anywhere else for the data. A "Miss" is defined as any time that a Page 
*       is not being stored by the Frame. In this case, the computer would be required to look elsewhere
*       for the necessary data. Finally, this program will simulate Pure Demand Paging for simplicity 
*       purposes.
* 
* How User Input should be Formatted: Each "Page" (number) should be followed by a comma with the 
*        exception of the final number in the list. Spaces are ignored either way.
*
*     - Acceptable Inputs: "43, 12, 53, -4" or "43,12,53,-4" or "43, 12,53, - 4"
* 
*     - Note: As long as the above format is followed, this program can handle as many "Pages" (numbers)
*             as possible. If a character that is not a number, comma, dash, or space is used, it will 
*             be ignored and which may affect the results. It's also worth pointing out that Numbers 
*             must be integers (decimals will not work because '.' is an ignored input).
* 
* Reason for Build: This is the Extra Credit for Assignment #3 in CS220 Computer Architecture
*/

#include <iostream>
#include <string>
#include <array>
using namespace std;

int main()
{
    bool userFinished = false; //Indicates if the user wants to end the program
    bool elemFound = false; //Indicates if the current "pageElem" can be found in the "frame" array
    bool zeroExceptionAllowed = true; //Allows the first zero to pass through if it is found within the first 5 elements of "userInput" so that the zero can be added as a true "Page" can be added to the "frame" array (so that it isn't confused with the 'nulled' reset values of the "frame") and the "queueValue_FIFO" can be incremented like normal
    string userInput = "";
    string pageElem = ""; //Temporary Storage location for the Pages after being extracted 
    array<string, 5> frame; //The array structure that simulates a Frame used in Virtual Memory. Stores "Pages" (numbers) and when it doesn't hold the value being searched for, a Page Fault [and therefore a miss] occurs. Depending on the algorithm being implemented, an according index of the array/frame would then be replaced by the page value in question.
    float pageFaultCounter; //Indicates the number of Page Faults that have occurred
    float elemTotal; //Determines how many total page elements have been derived from "userInput"
    int queueValue_FIFO; //Determines which value has been in the "frame" for the longest amount of time (for FIFO only)
    float hitPercentage = 0; // Formula: ((elemTotal - pageFaultCounter) / elemTotal) * 100
    float missPercentage = 0; //Formula: (pageFaultCounter / elemTotal) * 100
    
    cout << "This program simulates how Virtual Memory works in a computer.\nThe values you input below will act as \"pages\" being sent through a Frame and will be used to calculate the number of\nPage Faults that occur for each page replacement algorithm: \"Least Recently Used\" (LRU) and \"First in First Out\" (FIFO). \nThe Page Faults will be used to indicate the Hit Percentage and Miss Percentage for each algorithm, which in turn will\nindicate which algorithm would have been better at processing the given program." << endl << endl;

    cout << "Note: When inputting values, if you are copying and pasting from somewhere else (such as the Homework Assignment PDF) \nplease be sure to avoid hidden carriage returns in your submission as this will cut off values that were intended to \nbe entered into the system and will alter the correct answer." << endl << endl;

    do {
        cout << "Please input a string of values (separated by commas) to be sent through the Frame: " << endl;
        getline(cin, userInput);
        cout << endl;
        
        //-----------------------------------------------
        //The following are reset for the LRU algorithm
        for (int i = 0; i < frame.size(); i++) {
            frame[i] = '0';
        }
        pageFaultCounter = 0;
        elemTotal = 0;
        
        //-----------------------------------------------
        //Least Recently Used (LRU) Page Replacement Algorithm
        for (int i = 0; i < userInput.length(); i++) {

            //Extracts the numbers from "userInput" and sends them to "pageElem"
            if (isdigit(userInput[i]) || userInput[i] == '-') {
                pageElem += userInput[i];
            }

            //When program finds a comma or hits the end of the userInput string, then the number currently stored by pageElem is compared with the values held by the "frame" array
            if (userInput[i] == ',' || i == userInput.length() - 1) {
                elemFound = false; //Reset
                for (int j = 0; j < frame.size(); j++) {
                    
                    //if "pageElem" value found in "frame" array, then nothing happens and the search is terminated
                    if (frame[j] == pageElem) {
                        elemFound = true;

                        //"Pages" already present in the "frame" but are "accessed" again are brought back to the front of the queue to simulate them being used again [most recently manipulated data is at the front of the array, while least recently used data is held at the back of the array
                        for (int k = j; k >= 0; k--) {
                            if (k - 1 >= 0) {
                                frame[k] = frame[k - 1];
                            }
                            else {
                                frame[k] = pageElem;
                            }
                        }
                        break;
                    }
                }
                
                //If the "pageElem" value is not already within "frame" (or when the "frame" has less than 5 elements filled in it so that a '0' that happens to be in the first 5 slots of "userInput" isn't overlooked), then a Page Fault occurs (counter incremented by 1) and the current "pageElem" replaces the "pageElem" that has been in the "frame" array for the longest amount of time.
                if ((elemFound == false && (pageElem != "" && pageElem != "-")) || (elemTotal < 5 && pageElem == "0" && zeroExceptionAllowed == true)) {
                    if (pageElem == "0") {
                        zeroExceptionAllowed = false;
                    }
                    
                    pageFaultCounter++;
                    
                    //The "frame" array is shuffled so that the most recently manipulated data is stored at the front of the array, while least recently used data is held at the back of the array and is eventually phased out as new data arrives]
                    for (int k = frame.size() - 1; k >= 0; k--) {
                        if (k - 1 >= 0) {
                            frame[k] = frame[k - 1];
                        }
                        else {
                            frame[k] = pageElem;
                        }
                    }
                }
                //This fixed a bug that occurred with the "hitPercentage" and "missPercentage" when a user entered only non-digit characters
                if (pageElem != "" && pageElem != "-") {
                    elemTotal++;
                }
                pageElem = ""; //Reset so that next "Page" can be extracted from "userInput"
            }
        }

        hitPercentage = ((elemTotal - pageFaultCounter) / elemTotal) * 100;
        missPercentage = (pageFaultCounter / elemTotal) * 100;

        cout << "Least Recently Used (LRU) Page Replacement Algorithm:" << endl;
        cout << "\tNumber of Page Faults: " << pageFaultCounter << endl;
        cout << "\tHit Percentage: " << hitPercentage << "%" << endl;
        cout << "\tMiss Percentage: " << missPercentage << "%" << endl;
        cout << endl;

        //-----------------------------------------------
        //The following are reset for the FIFO algorithm
        for (int i = 0; i < frame.size(); i++) {
            frame[i] = '0';
        }
        pageFaultCounter = 0;
        elemTotal = 0;
        queueValue_FIFO = 0;
        zeroExceptionAllowed = true;

        //-----------------------------------------------
        //First in First Out (FIFO) Page Replacement Algorithm
        for (int i = 0; i < userInput.length(); i++) {

            //Extracts the numbers from "userInput" and sends them to "pageElem"
            if (isdigit(userInput[i]) || userInput[i] == '-') {
                pageElem += userInput[i];
            }

            //When program finds a comma or hits the end of the userInput string, then the number currently stored by pageElem is compared with the values held by the "frame" array
            if (userInput[i] == ',' || i == userInput.length() - 1) {
                elemFound = false; //Reset
                for (int j = 0; j < frame.size(); j++) {
                    
                    //if "pageElem" value found in "frame" array, then nothing happens and the search is terminated
                    if (frame[j] == pageElem) {
                        elemFound = true;
                        break;
                    }
                }
                
                //If the "pageElem" value is not already within "frame" (or when the "frame" has less than 5 elements filled in it so that a '0' that happens to be in the first 5 slots of "userInput" isn't overlooked), then a Page Fault occurs (counter incremented by 1) and the current "pageElem" replaces the "pageElem" that has been in the "frame" array for the longest amount of time.
                if ((elemFound == false && (pageElem != "" && pageElem != "-")) || (elemTotal < 5 && pageElem == "0" && zeroExceptionAllowed == true)) {
                    if (pageElem == "0") {
                        zeroExceptionAllowed = false;
                    }
                    pageFaultCounter++;
                    
                    frame[queueValue_FIFO] = pageElem;

                    queueValue_FIFO++;
                    if (queueValue_FIFO == 5) { //"5" because maximum "frame" size is 5 (can only store values in slots 0-4)
                        queueValue_FIFO = 0;
                    }
                }
                //This fixed a bug that occurred with the "hitPercentage" and "missPercentage" when a user entered only non-digit characters
                if (pageElem != "" && pageElem != "-") {
                    elemTotal++;
                }
                pageElem = ""; //Reset so that next "Page" can be extracted from "userInput"
            }
        }

        hitPercentage = ((elemTotal - pageFaultCounter) / elemTotal) * 100;
        missPercentage = (pageFaultCounter / elemTotal) * 100;

        cout << "First in First Out (FIFO) Page Replacement Algorithm:" << endl;
        cout << "\tNumber of Page Faults: " << pageFaultCounter << endl;
        cout << "\tHit Percentage: " << hitPercentage << "%" << endl;
        cout << "\tMiss Percentage: " << missPercentage << "%" << endl;
        cout << endl;

        //-----------------------------------------------
        //Asks user if they want to run the program again
        cout << "Would you like to try another set of numbers? ";
        getline(cin, userInput);
        cout << endl;

        for (int i = 0; i < userInput.length(); i++) {
            userInput[i] = tolower(userInput[i]);
        }

        if (userInput == "no") {
            userFinished = true;
        }

    } while (userFinished == false);

    cout << "Thank you for using this program. Have a great rest of your day!" << endl;
}