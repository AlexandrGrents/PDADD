#include <cmath>
#pragma once

const int NIL = -1;

//const char inputFileName[] = "input1.csv";
//const int m = 20;
//const int n = 4;
//const float R = 1.5;

const char inputFileName[] = "input.csv";
const int m = 1000;
const int n = 10;
const float R = 1;

const int N = m - n + 1;
const int p = 1;
const float dq = 0.39;
const int L = ceil(dq*N + 1);


const float r = pow(R, 2);
