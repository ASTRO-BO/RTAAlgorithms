/***************************************************************************
 RTAWaveformExtractor.h
 -------------------
 copyright            : (C) 2014 Andrea Bulgarelli
 email                : bulgarelli@iasfbo.inaf.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software for non commercial purpose              *
 *   and for public research institutes; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License.          *
 *   For commercial purpose see appropriate license terms                  *
 *                                                                         *
 ***************************************************************************/

#ifndef _RTAWAVEFORMEXTRACTOR_H
#define _RTAWAVEFORMEXTRACTOR_H

#include "RTAProcessor.h"
#include "RTADataProto.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "rtautils/Thread.h"

namespace RTAAlgorithm {
	
	
	
	
	class RTAWaveformExtractor : public RTAProcessor {
		
			
	public:
		//window is the size of the window of the waveform extractor
		RTAWaveformExtractor(CTAConfig::CTAMDArray* array, RTABuffer* buffer_input = 0, RTABuffer* buffer_output = 0, uint8_t window = 6);
		
		void init();
		
		RTAData* process(RTAData* input);
		
	protected:
		
		void calcWaveformExtraction(uint16_t* buffer, int16_t npixels, int16_t nsamples, uint8_t ws, unsigned short* maxresext, unsigned short* timeresext);
		
	private:
		
		unsigned short* maxres;
		unsigned short* timeres;
		uint8_t window;
		
		
		
	};
}

#endif