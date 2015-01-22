/***************************************************************************
 RTAWaveformExtractor.cpp
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

#include <packet/PacketLibDefinition.h>
#include "RTADataProto.h"
#include "RTAWaveformExtractor.h"

using namespace PacketLib;
using namespace CTAAlgorithm;

//#define DEBUG 1
//#define PRINTALG 1

namespace RTAAlgorithm {

RTAWaveformExtractor::RTAWaveformExtractor(CTAConfig::CTAMDArray* array, CTABuffer* buffer_input, CTABuffer* buffer_output, uint8_t window) : CTAProcessor(array, buffer_input, buffer_output) {
	maxres = new unsigned short[3000];
	timeres = new unsigned short[3000];
	this->window = window;
}

void RTAWaveformExtractor::init() {

}

CTAData* RTAWaveformExtractor::process(CTAData* input) {
	RTAData_Camera* camera = (RTAData_Camera*) input;
	RTAData_CameraExtracted* integrated = 0;
	if(camera->id == 0) {
		
		calcWaveformExtraction(camera->data, camera->npix, camera->nsamp, this->window, maxres, timeres);
		
		
		integrated = new RTAData_CameraExtracted;
		integrated->pixelvalue = new uint16_t[camera->npix];
		memcpy(integrated->pixelvalue, maxres, camera->npix * sizeof(camera->npix));
		integrated->timevalue = new uint16_t[camera->npix];
		memcpy(integrated->timevalue, timeres, camera->npix * sizeof(camera->npix));
		integrated->npix = camera->npix;
		integrated->telID = camera->telID;
		integrated->evtID = camera->evtID;
		integrated->ntelsEvt = camera->ntelsEvt;
		
	}
	return integrated;
}

//---------------------------------------------------------
// WAVEFORM EXTRACTION
//---------------------------------------------------------

void RTAWaveformExtractor::calcWaveformExtraction(uint16_t* buffer, int16_t npixels, int16_t nsamples, uint8_t ws, unsigned short* maxresext, unsigned short* timeresext) {
	uint16_t *b =  buffer; //should be pedestal subtractred
	
	//unsigned short* maxres  = new unsigned short[npixels];
	//unsigned short* timeres = new unsigned short[npixels];
	
	unsigned short* maxres  = maxresext;
	unsigned short* timeres = timeresext;

	
#ifdef PRINTALG
	std::cout << "npixels = " << npixels << std::endl;
	std::cout << "nsamples = " << nsamples << std::endl;
#endif
	
	for(int pixel = 0; pixel<npixels; pixel++) {
		uint16_t* s = b + pixel * nsamples;
		
#ifdef PRINTALG
		cout << "pixel " << pixel << " samples ";
		for(int k=0; k<nsamples; k++)
			cout << s[k] << " ";
		cout << endl;
#endif
		
		unsigned short max = 0;
		double maxt = 0;
		long sumn = 0;
		long sumd = 0;
		//long maxj = 0;
		double t = 0;
		
		for(int j=0; j<=ws-1; j++) {
			sumn += s[j] * j;
			sumd += s[j];
		}
		
		max = sumd;
		if(sumd != 0)
			t = sumn / (double)sumd;
		maxt = t;
		//maxj = 0;
		
		for(int j=1; j<nsamples-ws; j++) {
			sumn = sumn - s[j-1] * (j-1) + s[j+ws-1] * (j+ws-1);
			sumd = sumd - s[j-1] + s[j+ws-1];
			
			if(sumd > max) {
				max = sumd;
				if(sumd != 0)
					t = sumn / (double)sumd;
				maxt = t;
				//maxj = j;
			}
		}
		
		maxres[pixel] = max;
		timeres[pixel] =  maxt;
		
#ifdef PRINTALG
		cout << "result " << maxt << " " << maxres[pixel] << " " << timeres[pixel] << " " << endl;
#endif
	}
	return;
	/*
	memcpy(maxresext, maxres, sizeof(unsigned short) * npixels);
	memcpy(timeresext, timeres, sizeof(unsigned short) * npixels);
	delete[] maxres;
	delete[] timeres;
	*/
}

}
