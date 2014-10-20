/***************************************************************************
Producer.h
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

#ifndef _PRODUCERCAMERADATA_H
#define _PRODUCERCAMERADATA_H

#include "rtautils/Thread.h"
#include "rtautils/mac_clock_gettime.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include <packet/PacketBufferV.h>
#include "RTADataProto.h"
#include "RTABuffer.h"
#include "RTAProducer.h"

using namespace PacketLib;

namespace RTAAlgorithm {
	
	
	
	class ProducerCameraData : public Thread, public RTAProducer {
	private:
		
		double sizeMB;
		long sharedIndex;
		long long npackets;
		
		std::vector<RTAData_Camera*> sharedDataAndInfoBuffer;
		
		string configFileName;
		string rawfilename;
		struct timespec starttime, stoptime;
		
		
	protected:
		
		void createBuffer(string configFileName, string rawfilename);

	public:
		
		ProducerCameraData(RTABuffer* buffer_output);
		
		void init(string configFileName, string rawfilename);
		
		void *run();
		
		RTAData_Camera* getNext();
		
	};
	
}

#endif


