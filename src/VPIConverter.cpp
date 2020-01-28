/*
 * @file VPIConverter.cpp
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
#include "VPIConverter.h"
#include "MainWindow.h"

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

VPIConverter::VPIConverter( ) {


}

void VPIConverter::exportVPI( std::string filename, uint8_t* buffer, uint32_t w, uint32_t h, uint32_t alpha ) {
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
	name += ".vpi";

	typedef struct {
		uint32_t offset; //distance from the begining of the line or from the last painted pixel
		uint32_t length;
		uint32_t color;

	} vec;

	uint8_t pixel;
	uint32_t offset = 0;
	uint32_t lineOffset = 0;
	std::vector<std::vector<vec>* >rows;

	for( uint32_t y=0; y<h; y++ ) {
		std::vector<vec>* row = new std::vector<vec>();
		rows.push_back(row);

		//initial vec for line
		pixel = buffer[offset];
		vec v = { 0, 0, pixel };
		lineOffset = 0;

		for( uint32_t x=0; x<w; ++x ) {
			pixel = buffer[offset];
			if( pixel == v.color ) {
				v.length++;
				if( v.length == 0X0100 ) {
					v.length = 0xFF;
					row->push_back(v);
					v = { 0, 1, pixel };
					lineOffset = 0;
				}
			}
			else {
				row->push_back(v);
				v = { 0, 1, pixel };
				lineOffset = 0;
			}

			++offset;
			++lineOffset;
		}
		row->push_back(v);
	}

	//print info about vectors
	uint32_t totalBytes = 0;
	for( auto it = rows.begin(); it != rows.end(); it++ ) {
		totalBytes+=sizeof(uint16_t);
		std::vector<vec>* row = *it;
		std::cout << std::setfill (' ') << std::setw (10) << row->size() <<" ";
		for( vec v : *row ) {
		        if (v.color == MainWindow::COLOUR_FOR_TRANSPARENT)
                          std::cout<<"["<<v.offset<<","<<v.length<<","<<"–"<<"]";
                        else
			  std::cout<<"["<<v.offset<<","<<v.length<<","<<v.color<<"]";
			totalBytes+=(sizeof(uint16_t)+sizeof(uint8_t)+sizeof(uint8_t))*(v.length/255+1);
		}
		std::cout<<std::endl;
	}

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
	file.write( (char*)&alpha, sizeof(uint8_t));
	for( auto it = rows.begin(); it != rows.end(); it++ ) {
		std::vector<vec>* row = *it;

		uint16_t count = row->size();
		//check if there is a need to add dummy vectors for vectors that have offset greater than 255
		//this will also calculate number of vectors
//		for( vec v : *row ) {
//			if( v.offset > 255)
//				count += v.offset/255 + 1;
//			else
//				count ++;
//		}

		file.write( (char*)&count, sizeof(uint16_t));
		//write number of vectors in line
		for( vec v : *row ) {

//			uint8_t dummy = 0xFF;
//			//insert dummy vectors
//			while( v.offset > 255 ) {
//				file.write( (char*)&dummy, sizeof(uint16_t));
//				file.write( (char*)&dummy, sizeof(uint8_t));
//				file.write( (char*)&dummy, sizeof(uint8_t));
//			}

			uint8_t color = v.color;
			uint8_t length = v.length;
			uint16_t offset = v.offset;

			file.write( (char*)&offset, sizeof(uint16_t));
			file.write( (char*)&length, sizeof(uint8_t));
			file.write( (char*)&color, sizeof(uint8_t));
		}
	}
	file.close();

	//release memory
	for( uint32_t i=0; i<rows.size(); ++i )
		delete rows[i];
}

VPIConverter::~VPIConverter() {
	// TODO Auto-generated destructor stub
}

