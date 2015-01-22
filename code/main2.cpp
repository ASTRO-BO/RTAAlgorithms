#include <iostream>
#include <CTABuffer.h>
#include "RTAWaveformExtractor.h"
#include "RTACleaning.h"
#include "Producer.h"

using namespace std;
using namespace CTAConfig;
using namespace CTAAlgorithm;
using namespace RTAAlgorithm;

int main(int argc, char *argv[])
{
	
	string ctarta;
	
	if(argc > 1) {
		const char* home = getenv("CTARTA");
		
		if (!home)
		{
			std::cerr << "ERROR: CTARTA environment variable is not defined." << std::endl;
			return 0;
		}
		
		ctarta = home;
		
	} else {
		cerr << "ERROR: Please, provide the .raw" << endl;
		return 0;
	}

	
	if(argc <= 2) {
		std::cerr << "ERROR: Please, provide the .raw, .stream, number of threads" << std::endl;
		std::cerr << "Example ./alg1 $CTARTA/data/Aar.prod2.a2.FADC.NEWCOMP.NEWPACKET.BIGTRUE.raw rta_fadc_all_BIGTRUE.stream 8" << std::endl;
		return 0;
	}

	
	CTABuffer buff_cameradata("camfullwaveform", 100);
	CTABuffer buff_cameradata_int("camintegrated", 100);
	CTABuffer buff_cameradata_cleaned("camcleaned", 100);
	
	CTAMDArray array;
	string baseconf = ctarta;
	baseconf += "/share/ctaconfig/";
	array.loadConfig("AARPROD2", "PROD2_telconfig.fits.gz", "Aar.conf", baseconf);
	
	
	RTAWaveformExtractor waveextractor(&array, &buff_cameradata, &buff_cameradata_int, 6 );
	RTACleaning cleaning(&array, &buff_cameradata_int, &buff_cameradata_cleaned, 10, 30);
	
	string rawfilename = argv[1];
	string configFileName = argv[2];
	
	ProducerCameraData p(&buff_cameradata);
	p.init(configFileName, rawfilename);
	p.start();
	
	CTABufferCleaner delbuff(&buff_cameradata_cleaned);
	delbuff.start();
	
	
	int nthreadswave=std::atoi(argv[3]);
	CTAProcessorThread* thwave = new CTAProcessorThread[nthreadswave];
	for(int i=0; i<nthreadswave; i++) {
		
		thwave[i].init(&waveextractor);
		thwave[i].start();
	}
	
	int nthreadscleaning=std::atoi(argv[3]);
	CTAProcessorThread* thclean = new CTAProcessorThread[nthreadscleaning];
	for(int i=0; i<nthreadscleaning; i++) {
		
		thclean[i].init(&cleaning);
		thclean[i].start();
	}
	
	
	p.join();
	delbuff.join();
}
