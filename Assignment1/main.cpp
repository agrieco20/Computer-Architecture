/**
 * @author:Anthony Grieco
 * Date: 2/4/2022
 *
 * Description: This project is intended to convert Base 10 numbers (given by the user) and convert them into
 * both Base 2 (Binary) and Base 16 (Hexadecimal). It converts both positive and negative integers.
 *
 * Reason for Build: This is the Extra Credit for Assignment #1 in CS220 Computer Architecture
 */

#include <iostream>
#include <list>
#include <iterator>
#include <cmath>

using namespace std;

int main() {
    bool userFinished = false; //used to validate the end of the program
    bool ineligibleValueGiven = false; //Used to help validate whether the given value for Base 10 can be converted (ex: a word wasn't submitted)
    string userInput = "";
    string userInputFormatted = ""; //Used to format userInput to specific conditions for input validation
    double base10Value = 0.0; //Submitted by the user and all calculations are based off of this given number
    int divisible = 0; //Used to reflect the remaining "base10Value" with the calculation "base10Value / 2" (with integer division) after a % calculation has been performed to determine whether the binary value will be a 1 or 0
    string binaryPlacementHolder;//Holds the binary values in reverse order after the % calculation has been performed. These values are reversed and then sent to "binaryNum" as a final answer for the binary value

    double originalNum = false; //If the requested Base 10 Value is negative, then this indicates that the bits will need to be flipped (and 1 added)
    string binaryNum = ""; //Holds final binary number after conversion is complete from Base 10 number
    string hexNum = ""; //Holds final hex number after conversion is complete from Binary number

    list <string> hexPlaceHolder = {}; //Used to store all the 4 bit groupings that are used for the hexadecimal conversion
    list <string>::iterator it; //Used to iterate through the "hexPlaceHolder" arrayList
    string groupBuilder; //Used to split the reset "binaryPlacementHolder" into groups of 4 numbers apiece
    int binaryRemainder; //Used to determine how many extra zeroes (which are later converted to ones) are needed for negative binary numbers
    int hexRemainder; //Used to determine how many extra zeroes need to be added to the front of the binary number for a Hex Conversion

    int binaryCount; //Used to keep track of the remaining binary bit values during hexadecimal conversion
    int hexCount; //Used to keep track of the growing Hex number for each grouping of 4 bits before being sent to "hexNum"
    int counter; //Increments exponent in Hexadecimal conversion

    do {
        //Input Validation for Base 10 Number
        do {
            ineligibleValueGiven = false;
            cout << "Please input a number (in Base 10) that you would like converted into both Base 2 (Binary)\nand Base 16 (Hexadecimal):" << endl;
            cout << "Base 10:";
            getline (cin, userInput);
            cout << endl;

            //Only positive numbers allowed (no letters)
            try {
                stod(userInput);
            }
            catch(std::invalid_argument){
                ineligibleValueGiven = true;
                cout << "Ineligible value entered. You must input a numeric value." << endl;
            }
        } while(ineligibleValueGiven == true);

        //Determines if value is negative (a positive number is sent through the program and then at the end, before the hexadecimal conversion, if the original number was negative then the bits will be flipped and 1 is added)
        originalNum = stod(userInput);

        //Binary Calculation
        base10Value = abs(stod(userInput));
        binaryPlacementHolder = ""; //Reset
        binaryNum = ""; //Reset

        //--------------------------------------------------------
        //Test - Works
        /*
        for (int i = 1; i<10; i++){
            cout << i << endl;
            binaryPlacementHolder += to_string(i);
        }
        cout << "Test: " << binaryPlacementHolder << endl;
         */
        //--------------------------------------------------------

        //Actual calculation/conversion for Binary
        divisible = (int) base10Value;
        //cout << "Divisible: " << divisible << endl; //Temporary
        //cout << "base10Value: " << base10Value << endl; //Temporary

        while (divisible != 0) {
            divisible /= 2;
            //cout << "Divisible: " << divisible << endl; //Temporary

            binaryPlacementHolder += to_string((int) base10Value % 2);
            base10Value = (int) base10Value / 2;
            //cout << "base10Value: " << base10Value << endl; //Temporary
            //cout << "Reverse Order - Binary Value(0 or 1): " << binaryPlacementHolder << endl; //Temporary
        }

        //Using Two's Compliment, if original Base 10 Number was NEGATIVE, then all the bits are swapped and 1 is added (after being reversed, this change would need to be done to "binaryNum")
        if (originalNum < 0){

            //Adds extra zeroes to the original binary number (which will later be flipped to 1's)
            binaryRemainder = binaryPlacementHolder.length() % 4;
            if (binaryRemainder != 0) {
                for (int i = 0; i < 4 - binaryRemainder; i++) {
                    binaryPlacementHolder += "0";
                }
            }

            //Reverses the Bits
            for (int i = 0; i < binaryPlacementHolder.length(); i++){
                //for (int i = binaryNum.length() - 1; i >= 0; i++){
                binaryNum += to_string(((int)(binaryPlacementHolder[i]) + 1) % 2);
            }

            //1 is Added if the value isn't already a '1' (if it is a '1', the value flipped to a '0' to simulate binary addition)
            for (int i = 0; i < binaryNum.length(); i++){
                //if(((int)(binaryNum[i]) + 1) % 2 == 1){
                if(binaryNum[i] == '0'){
                    binaryNum[i] = (int)(binaryNum[i]) + 1;
                    break;
                }
                else {
                    binaryNum[i] = '0';
                }
            }
            binaryPlacementHolder = binaryNum; //Sets up the bit placement reversion (ex: not 0 -> 1, 'current index 0' becomes 'index binaryPlacementHolder.length()-1') later on in the program and sets up the "binaryNum" variable to catch said reversion
        }
        //cout << "Negative Test:" << endl; //Temporary
        //cout << "binaryNum: " << binaryNum << endl; //Temporary
        //cout << "binaryPlacementHolder: " << binaryPlacementHolder << endl; //Temporary

        //Reverses all the values held in "binaryPlaceHolder" and sent to "binaryNum" so that it actually reflects what the true binary value is supposed to be
        binaryNum = ""; //Reset
        for (int i = binaryPlacementHolder.length() - 1; i >= 0; i--){
            //cout << binaryPlacementHolder[i] <<  ", "; //TEMPORARY
            binaryNum += binaryPlacementHolder[i];
        }

        //Hexadecimal Calculation
        hexRemainder = binaryNum.length() % 4;
        //hexRemainder = binaryPlacementHolder.length() % 4;

        //Adds extra zeroes to the front of the binary number so that the hexadecimal conversion can be completed with 4 numbers in each grouping [see below]
        binaryPlacementHolder = "";
        if (hexRemainder != 0){
            for (int i = 0; i < (4 - hexRemainder); i++){
                binaryPlacementHolder += "0";
            }
        }

        binaryPlacementHolder += binaryNum; //Adds the reversed "binaryNum" string [which is now in the correct order] to the reset "binaryPlacementHolder" they may have had extra zeroes already added to the front of it in preparation for Hexadecimal conversion
        //cout << "BinaryNum: " << binaryNum<< endl; //Temporary
        //cout << "BinaryPlacementHolder: " << binaryPlacementHolder << endl; //Temporary

        //Splits the values held by "binaryPlacementHolder" into groups of 4, which will later be converted into Hexadecimal
        for (int i = 0; i < (binaryPlacementHolder.length() + hexRemainder) / 4 ; i++){
            groupBuilder = "";
            for (int j = 0; j < 4; j++){
                if (binaryPlacementHolder.length() - 1 < j + (4 * i)){
                    //if (binaryPlacementHolder[j + (4 * i)] > binaryPlacementHolder.length()){
                    groupBuilder += "0";
                }
                else {
                    groupBuilder += binaryPlacementHolder[j + (4 * i)];
                }
            }
            hexPlaceHolder.push_back(groupBuilder);
        }
        /*
        //Tests to see whether "hexPlaceHolder" actually picked up the necessary elements
        cout << "Groups: "; //Temporary
        for (it = hexPlaceHolder.begin(); it != hexPlaceHolder.end(); ++it){
            cout << *it << ", ";
        }
        */

        //Actual calculation/conversion for Hexadecimal
        hexNum = ""; //Reset
        for (it = hexPlaceHolder.begin(); it != hexPlaceHolder.end(); ++it){
            hexCount = 0; //Reset
            binaryCount = stoi(*it); //Reset
            counter = 3; //Reset
            while (binaryCount > 0){
                if (binaryCount >= pow(10, counter)){
                    binaryCount -= pow(10, counter);
                    hexCount += pow(2, counter);
                }
                counter--;
            }

            if (hexCount == 15){
                hexNum += "F";
            }
            else if (hexCount == 14){
                hexNum += "E";
            }
            else if (hexCount == 13){
                hexNum += "D";
            }
            else if (hexCount == 12){
                hexNum += "C";
            }
            else if (hexCount == 11){
                hexNum += "B";
            }
            else if (hexCount == 10){
                hexNum += "A";
            }
            else {
                hexNum += to_string(hexCount);
            }
        }

        //Resets the "hexPlaceHolder" arrayList
        while (hexPlaceHolder.empty() == false){
            hexPlaceHolder.pop_back();
        }

        //Prints Out Converted Values
        cout << "***Converted Values***" << endl;
        cout << "Base 2 (Binary): " << binaryNum << endl;
        cout << "Base 16 (Hexadecimal): " << hexNum << endl;
        cout << endl;

        //Repeats Program if user would like to convert another number
        cout << "Would you like to convert another number?";
        getline(cin, userInput);
        cout << endl;

        for (int i = 0; i<userInput.size(); i++){
            if (userInput[i] != ' '){
                userInputFormatted += toupper(userInput[i]);
            }
        }
        if (userInputFormatted == "NO" || userInputFormatted == "N") {
            userFinished = true;
        }

        userInputFormatted = "";
        userInput = "";
        cin.clear();

    }while(userFinished == false);

    cout<<"Thank you for using this converter. Have a nice day!" << endl;
    return 0;
}