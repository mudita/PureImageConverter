/*
 * @file MPIConverter.h
 * @author Robert Borzecki (robert.borzecki@mudita.com)
 * @date 20 cze 2019
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */
#ifndef SRC_MPICONVERTER_H_
#define SRC_MPICONVERTER_H_

/*
 *
 */
class MPIConverter {
public:
	MPIConverter();
	static void exportMPI( std::string filename, uint8_t* buffer, uint32_t w, uint32_t h );
	virtual ~MPIConverter();
};

#endif /* SRC_MPICONVERTER_H_ */
