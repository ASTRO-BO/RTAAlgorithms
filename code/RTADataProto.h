/***************************************************************************
 RTADataProto.h
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

#ifndef _RTADATAPROTO_H
#define _RTADATAPROTO_H

#include <CTAData.h>
#include <ctautils/Matrix.h>

namespace RTAAlgorithm {
	
	class RTAData_Camera : public CTAAlgorithm::CTAData {
	public:
		//pointer to array of array of waveforms
		uint16_t* data;
		//size in byte of the array
		uint16_t nbytes;
		int16_t npix;
		int16_t nsamp;
		int16_t telID;
		int32_t evtID;
		int16_t ntelsEvt;
		
		RTAData_Camera() {
			this->id = 0;
			data = 0;
			nbytes = 0;
			npix = nsamp = telID = -1;
			evtID = -1;
			ntelsEvt = 0;
		};
		
		~RTAData_Camera() {
			delete[] data;
		};
	};
	
	class RTAData_CameraExtracted : public CTAAlgorithm::CTAData {
	public:
		//pointer to array of array of pixels of extracted waveforms
		uint16_t* pixelvalue;
		uint16_t* timevalue;

		int16_t npix;
		int16_t telID;
		int32_t evtID;
		int16_t ntelsEvt;
		
		//cleaned image
		int16_t **cleanedPixId;
		int16_t cleanedPixId_rows;
		int16_t cleanedPixId_cols;
		
		RTAData_CameraExtracted() {
			this->id = 1;
			pixelvalue = timevalue = 0;
			npix = telID = -1;
			evtID = -1;
			ntelsEvt = 0;
			cleanedPixId = 0;
			cleanedPixId_rows = cleanedPixId_cols = 0;
			
		};
		
		~RTAData_CameraExtracted() {
			delete[] pixelvalue;
			delete[] timevalue;
			matrixDestroy<int16_t>(cleanedPixId_rows, cleanedPixId_cols, cleanedPixId);
		}
	};

}

#endif
