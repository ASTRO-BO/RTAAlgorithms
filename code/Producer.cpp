/***************************************************************************
 Producer.cpp
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
#include "Producer.h"
#include <iomanip>
#include <vector>
#include <pthread.h>

using namespace RTAAlgorithm;

ProducerCameraData::ProducerCameraData(RTABuffer* buffer_output) :RTAProducer(buffer_output) {

}

void ProducerCameraData::createBuffer(string configFileName, string rawfilename)
{
	
	PacketBufferV buff(configFileName, rawfilename);
	buff.load();
	int npackets = buff.size();
	std::cout << "Loaded " << npackets << " packets " << std::endl;
	
	PacketStream* ps = new PacketStream(configFileName.c_str());
	
	int npix_idx = 0;
	int nsamp_idx = 0;
	int telID_idx = 0;
	int evtn_idx = 0;
	int ntelsevt_idx = 0;
	try {
		Packet *p = ps->getPacketType("triggered_telescope1_30GEN");
		npix_idx = p->getPacketSourceDataField()->getFieldIndex("Number of pixels");
		nsamp_idx = p->getPacketSourceDataField()->getFieldIndex("Number of samples");
		telID_idx = p->getPacketSourceDataField()->getFieldIndex("TelescopeID");
		evtn_idx = p->getPacketSourceDataField()->getFieldIndex("eventNumber");
		ntelsevt_idx = p->getPacketSourceDataField()->getFieldIndex("numberOfTriggeredTelescopes");
	} catch (PacketException* e)
	{
		cout << "Error during encoding: ";
		cout << e->geterror() << endl;
	}
	for(int i=0; i<buff.size(); i++)
	{
		ByteStreamPtr rawPacket = buff.getNext();
		Packet *p = ps->getPacket(rawPacket);
		// get npixel and nsamples
		
		int npix = p->getPacketSourceDataField()->getFieldValue(npix_idx);
		int nsamp = p->getPacketSourceDataField()->getFieldValue(nsamp_idx);
		int telID = p->getPacketSourceDataField()->getFieldValue(telID_idx);
		int evtID = p->getPacketSourceDataField()->getFieldValue(evtn_idx);
		int ntelsevt = p->getPacketSourceDataField()->getFieldValue(ntelsevt_idx);
#ifdef DEBUG
		if(p->getPacketID() == 0) {
			std::cerr << "No packet type recognized" << std::endl;
			continue;
		}
#endif
		
		RTAData_Camera* elem = new RTAData_Camera;
		ByteStreamPtr data = p->getData();
		
		
#ifdef ARCH_BIGENDIAN
		if(!data->isBigendian())
			data->swapWord();
#else
		if(data->isBigendian())
			data->swapWord();
#endif
		elem->nbytes = data->size();
		
		//elem->data = new uint16_t[elem->nbytes / 2];
		//memcpy(elem->data, data->getStream(), data->size());
		elem->data = (uint16_t*)data->getStream();
		elem->npix = npix;
		elem->nsamp = nsamp;
		elem->telID = telID;
		elem->evtID = evtID;
		elem->ntelsEvt = ntelsevt;
		sharedDataAndInfoBuffer.push_back(elem);
	}
	
	//return outBuff;
}

void ProducerCameraData::init(string configFileName, string rawfilename) {
	this->configFileName = configFileName;
	this->rawfilename = rawfilename;
	sizeMB = 0;
	sharedIndex = 0;
	npackets = 0;
	createBuffer(configFileName, rawfilename);
	clock_gettime(CLOCK_MONOTONIC, &starttime);
}

void *ProducerCameraData::run() {
	
	cout << "Thread started" << endl;
	
	clock_gettime(CLOCK_MONOTONIC, &starttime);
	while (true) {
		RTAData_Camera* cam = getNext();
		buffer_output->put(cam);
	}
	return 0;
	
}

RTAData_Camera* ProducerCameraData::getNext() {
	RTAData_Camera* cam = sharedDataAndInfoBuffer[sharedIndex];
	sharedIndex = (sharedIndex+1) % sharedDataAndInfoBuffer.size();
	sizeMB += (cam->nbytes / 1000000.0);
	
	//cout << npackets << endl;
	
	npackets++;
	//cout << npackets << endl;
	if(npackets % 10000 == 0) {
		clock_gettime(CLOCK_MONOTONIC, &stoptime);
		
		// get results
		double time = timediff(starttime, stoptime);
		std::cout << "Producer: -----" << endl;
		std::cout << "Producer: Result: it took  " << time << " s" << std::endl;
		std::cout << "Producer: Result: rate: " << setprecision(10) << sizeMB / time << " MB/s" << std::endl;
		std::cout << "Producer: Result: rate: " << setprecision(10) << npackets / time << " Hz" << std::endl;
		std::cout << "Producer: Input buffer size: " << setprecision(10) << sizeMB << " MB " << std::endl;
		npackets = 0;
		sizeMB = 0;
		clock_gettime(CLOCK_MONOTONIC, &starttime);
	}
	return cam;
}
