#include <iostream>
#include <string>
#include <Producer.h>
#include "RTAWaveformExtractor.h"
#include "RTACleaning.h"

using namespace std;
using namespace RTAAlgorithm;

int main(int argc, char *argv[])
{
	const char* home = getenv("CTARTA");
	
	if (!home)
	{
		cerr << "ERROR: CTARTA environment variable is not defined." << endl;
		return 0;
	}
	
	string ctarta = home;
	
	if(argc <= 2) {
		cerr << "ERROR: Please, provide the .raw, .stream, number of threads" << endl;
		cerr << "Example ./alg1 $CTARTA/data/Aar.prod2.a2.FADC.NEWCOMP.NEWPACKET.BIGTRUE.raw rta_fadc_all_BIGTRUE.stream 8" << endl;
		return 0;
	}
	
	CTAConfig::CTAMDArray array;
	string baseconf = ctarta;
	baseconf += "/share/ctaconfig/";
	array.loadConfig("AARPROD2", "PROD2_telconfig.fits.gz", "Aar.conf", baseconf);

	
	
	RTAWaveformExtractor waveextractor(0, 0, 0, 6);
	RTACleaning cleaning(&array, 0, 0, 10, 30);
	
	string rawfilename = argv[1];
	string configFileName = argv[2];
	
	ProducerCameraData fullcamData(0);
	fullcamData.init(configFileName, rawfilename);
	
	while(true) {
		RTAData_Camera* cam = fullcamData.getNext();
		RTAData_CameraExtracted* integrated = (RTAData_CameraExtracted*) waveextractor.process(cam);
		//cout << integrated->id << endl;
		//cleaning.process(integrated);
		
		delete integrated;

	}
	
	
}
