#pragma once
#include <iostream>
#include <fstream>
#include "..\includes.hpp"
class materials : public singleton< materials > {
public:
void setup();
void remove();
};