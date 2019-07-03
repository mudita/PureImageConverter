/*
 * @file VPIConverter.h
 * @author Robert Borzecki (robert.borzecki@mudita.com)
 * @date 20 cze 2019
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */
#ifndef SRC_VPICONVERTER_H_
#define SRC_VPICONVERTER_H_

#include <string>
#include <cstdint>

/*
 *
 */
class VPIConverter {
public:
	VPIConverter();
	static void exportVPI( std::string filename, uint8_t* buffer, uint32_t w, uint32_t h, uint32_t alpha );
	virtual ~VPIConverter();
};

#endif /* SRC_VPICONVERTER_H_ */
