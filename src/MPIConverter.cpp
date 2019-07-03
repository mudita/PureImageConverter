/*
 * @file MPIConverter.cpp
 * @author Robert Borzecki (robert.borzecki@mudita.com)
 * @date 20 cze 2019
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */
#include <cstdint>
#include <set>
#include <vector>
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <iomanip>
#include <fstream>
#include "MPIConverter.h"

static std::vector<std::string> splitpath(
  const std::string& str
  , const std::set<char> delimiters)
{
  std::vector<std::string> result;

  char const* pch = str.c_str();
  char const* start = pch;
  for(; *pch; ++pch)
  {
    if (delimiters.find(*pch) != delimiters.end())
    {
      if (start != pch)
      {
        std::string str(start, pch);
        result.push_back(str);
      }
      else
      {
        result.push_back("");
      }
      start = pch + 1;
    }
  }
  result.push_back(start);

  return result;
}

MPIConverter::MPIConverter() {
}

void MPIConverter::exportMPI( std::string filename, uint8_t* buffer, uint32_t w, uint32_t h ) {
	// TODO Auto-generated constructor stub
	std::set<char> delims{'\\', '/'};
	std::vector<std::string> path = splitpath(filename, delims);
	std::cout<<"file: " << path.back() << std::endl;

	//find last dot and remove extension
	std::string name = path.back();
	uint32_t found = name.find_last_of(".");

	//if dont was found remove it
	if( found != std::string::npos ) {
		name = name.substr(0,found);
		std::cout<<"name: "<<name<<std::endl;
	}
	//add pure's vector image extension
	name += ".mpi";

	//write data to file
	std::string outpath;
	char cwd[1024]={0};
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		outpath+=cwd;
	}
	outpath+="/"+name;
	std::ofstream file;
	file.open( outpath, std::ios::out | std::ios::binary);
	file.write( (char*)&w, sizeof(uint16_t));
	file.write( (char*)&h, sizeof(uint16_t));
	file.write( (char*)buffer, w*h );

	file.close();
}



MPIConverter::~MPIConverter() {
	// TODO Auto-generated destructor stub
}

