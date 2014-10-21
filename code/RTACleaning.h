/***************************************************************************
 RTACleaning.h
 -------------------
 copyright            : (C) 2014 Alessio Aboudan, Andrea Bulgarelli
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

#ifndef _RTACLEANING_H
#define _RTACLEANING_H

#include "RTAProcessor.h"
#include "RTADataProto.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "rtautils/Thread.h"

namespace RTAAlgorithm {
	
	
	
	
	class RTACleaning : public RTAProcessor {
		
			
	public:
		//window is the size of the window of the waveform extractor
		RTACleaning(CTAConfig::CTAMDArray* array, RTABuffer* buffer_input = 0, RTABuffer* buffer_output = 0, uint16_t selThreshold = 0, uint16_t sumThreshold = 0);
		
		void init();
		
		RTAData* process(RTAData* input);
		
	protected:
		
		void clean(int16_t **inPixId, uint16_t *counts);
		void clean2(int16_t **inPixId, uint16_t *counts);
		
	private:
		
		int16_t n1, n2;
		
		CTAGridMap *gm;
		
	public:
		
		int16_t **outPixId;

		
		
	};
}

#endif