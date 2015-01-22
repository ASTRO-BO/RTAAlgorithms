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

#include <CTAProcessor.h>
#include <ctautils/Thread.h>
#include <CTAMDArray.h>
#include <CTAGridMap.h>

namespace RTAAlgorithm {
	
	class RTACleaning : public CTAAlgorithm::CTAProcessor {
		
	public:
		//window is the size of the window of the waveform extractor
		RTACleaning(CTAConfig::CTAMDArray* array, CTAAlgorithm::CTABuffer* buffer_input = 0, CTAAlgorithm::CTABuffer* buffer_output = 0, uint16_t selThreshold = 0, uint16_t sumThreshold = 0);
		
		void init();
		
		CTAAlgorithm::CTAData* process(CTAAlgorithm::CTAData* input);
		
	protected:
		
		void clean(int16_t **inPixId, uint16_t *counts, CTAConfig::CTAGridMap* gm, int16_t** outPixId);
		void clean2(int16_t **inPixId, uint16_t *counts, CTAConfig::CTAGridMap* gm, int16_t** outPixId);
		
	private:
		
		int16_t n1, n2;
	};
}

#endif
