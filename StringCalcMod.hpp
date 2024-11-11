#pragma once
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>



using namespace std;


enum class Priority {scobe, num, plus, mult, unar};
enum LexemeType {lsc, rsc, num, binary, unary, diff, unknown};

struct Lexeme
{
	LexemeType type;
	Priority priority;
	string value;
};

enum class Status { StartStatus, NumberStatus, LScobeStatus, RScobeStatus, UnaryStatus, BinaryStatus, DifferentStatus, ErrorStatus, EndStatus };
