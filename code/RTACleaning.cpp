/***************************************************************************
 RTACleaning.cpp
 -------------------
 copyright            : (C) 2014 Alessio Abouda, Andrea Bulgarelli
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

#include <packet/PacketLibDefinition.h>
#include <ctautils/Matrix.h>
#include "RTADataProto.h"
#include "RTACleaning.h"

using namespace PacketLib;
using namespace CTAAlgorithm;
using CTAConfig::CTAGridMap;

//#define DEBUG 1
//#define PRINTALG 1

namespace RTAAlgorithm {

RTACleaning::RTACleaning(CTAConfig::CTAMDArray* array, CTABuffer* buffer_input, CTABuffer* buffer_output, uint16_t selThreshold, uint16_t sumThreshold) : CTAProcessor(array, buffer_input, buffer_output) {
	
	n1 = selThreshold;
	n2 = sumThreshold;
}

void RTACleaning::init() {

}

CTAData* RTACleaning::process(CTAData* input) {
	RTAData_CameraExtracted* integrated = (RTAData_CameraExtracted*) input;
	
	if(integrated->id == 1) {

		CTAGridMap *gm = array->getTelescope(integrated->telID)->getTelescopeType()->getCameraType()->getMap();
		int16_t **outPixId = CTAUtils::matrixCreate<CTAGridMap::Index>(gm->getRowSz(), gm->getColSz());
		
		clean(gm->pixId, integrated->pixelvalue, gm, outPixId);
		
		integrated->cleanedPixId = outPixId;
		integrated->cleanedPixId_rows = gm->getRowSz();
		integrated->cleanedPixId_cols = gm->getColSz();
		
		
	}
	return integrated;
}


void RTACleaning::clean(int16_t **inPixId, uint16_t *counts, CTAGridMap* gm, int16_t** outPixId) {
	
	// Optimize avoiding temporary variables
	
	for (CTAGridMap::Size i = 0; i < gm->getRowSz(); i++) {
		
		for (size_t j = 0; j < gm->getColSz(); j++) {
			
			if (inPixId[i][j] >= 0) {               // Process only valid pixels
				
				//cout << "i " << i << "	" << j << " " << inPixId[i][j] << " " << counts[inPixId[i][j]] << endl;
				
				if (counts[inPixId[i][j]] >= n1) {  // Check selection threshold
					
					CTAGridMap::Index s = 0;
					
					for (CTAGridMap::Size k = 0; k < gm->getNeighborsSz(); k++) {
						
						
						if (gm->nr[i][j][k] >= 0)
						{
							auto id = inPixId[gm->nr[i][j][k]][gm->nc[i][j][k]];
							if(id >= 0)
								s += counts[id];
						}
					}
					
					if (s >= n2)                    // Check sum threshold
						outPixId[i][j] = inPixId[i][j];
					else
						outPixId[i][j] = -1;
					
					
				} else
					outPixId[i][j] = -1;
				
			} else
				outPixId[i][j] = -1;
			
		}
		
	}
	
}

void RTACleaning::clean2(int16_t **inPixId, uint16_t *counts, CTAGridMap* gm, int16_t** outPixId) {
	
	// Optimize avoiding temporary variables
	
	for (CTAGridMap::Size i = 0; i < gm->getRowSz(); i++) {
		
		for (size_t j = 0; j < gm->getColSz(); j++) {
			
			//cout << i << "	" << j << " " << inPixId[i][j] << endl;
			
			if (inPixId[i][j] >= 0) {               // Process only valid pixels
				
				
				
				if (counts[inPixId[i][j]] >= n1) {  // Check selection threshold
					
					
					outPixId[i][j] = inPixId[i][j];
					
				} else
					outPixId[i][j] = -1;
				
			} else
				outPixId[i][j] = -1;
			
			//cout << n1 << " i " << i << "	" << j << " " << inPixId[i][j] << " " << counts[inPixId[i][j]] << " " << outPixId[i][j] <<  endl;
			
		}
		
	}
	
}

}
