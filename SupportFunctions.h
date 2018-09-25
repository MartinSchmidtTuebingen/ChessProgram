#include <string>

#ifndef SUPPORTFUNCTIONS_H
#define SUPPORTFUNCTIONS_H

int RandomNumber(int maxnumber);
short NextID();
bool OutOfBound(short searchedfile, short searchedrank);
std::string GetColor(short color);
std::string GetField(short file, short rank);
std::string GetFile(short file);
std::string GetRank(short file);
std::string GetPieceShortName(short type);
short GetTypeFromString(std::string input);
short GetTypeFromSingleString(std::string input);
short GetFileFromString(std::string input);
short GetFileFromSingleString(std::string input);
short GetRankFromString(std::string input); 
short GetRankFromSingleString(std::string input);
int parseLine(char* line);
int getCurrentMemoryUbuntu();

#endif