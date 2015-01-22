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

#include <iostream>
#include <ctautils/Thread.h>
#include <ctautils/mac_clock_gettime.h>
#include <packet/PacketBufferV.h>
#include <CTABuffer.h>
#include <CTAProducer.h>
#include "RTADataProto.h"

namespace RTAAlgorithm {
	
	class ProducerCameraData : public Thread, public CTAAlgorithm::CTAProducer {
	private:
		
		double sizeMB;
		long sharedIndex;
		long long npackets;
		
		std::vector<RTAData_Camera*> sharedDataAndInfoBuffer;
		
		std::string configFileName;
		std::string rawfilename;
		struct timespec starttime, stoptime;
		
	protected:
		
		void createBuffer(string configFileName, string rawfilename);

	public:
		
		ProducerCameraData(CTAAlgorithm::CTABuffer* buffer_output);
		
		void init(std::string configFileName, std::string rawfilename);
		
		void *run();
		
		RTAData_Camera* getNext();
		
	};
}

#endif


