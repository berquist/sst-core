// Copyright 2009-2010 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
// 
// Copyright (c) 2009-2010, Sandia Corporation
// All rights reserved.
// 
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.


#include <sst_config.h>

#include <utility>

#include <sst/core/link.h>
#include <sst/core/simulation.h>

namespace SST { 

Link::Link() :
    rFunctor( NULL ),
    defaultTimeBase( NULL ),
    latency(1)
{
    recvQueue = Simulation::getSimulation()->getEventQueue();
}

Link::~Link() {
    if ( type == POLL ) {
	delete recvQueue;
    }
}

void Link::setPolling() {
    type = POLL;
    recvQueue = new EventQueue_t;
}

    
void Link::setLatency(Cycle_t lat) {
    latency = lat;
}
    
    
void Link::Send( SimTime_t delay, TimeConverter* tc, CompEvent* event ) {
//     _LINK_DBG("delay=%lu sendQueue=%p event=%p sFunctor=%p\n",
//               (unsigned long) delay,sendQueue,event,sFunctor);
    if ( tc == NULL ) {
	_abort(Link,"Cannot send an event on Link with NULL TimeConverter\n");
    }

    Cycle_t cycle = Simulation::getSimulation()->getCurrentSimCycle() +
                    tc->convertToCoreTime(delay) + latency;

    _LINK_DBG( "cycle=%lu\n", (unsigned long)cycle );

    std::pair<EventHandlerBase<bool,Event*>*,Event*> envelope;
    envelope.first = pair_link->rFunctor;
    envelope.second = event;
    pair_link->recvQueue->insert( cycle, envelope );
}
    

CompEvent* Link::Recv()
{
    Event* event = NULL;
    Simulation *simulation = Simulation::getSimulation();

    if ( !recvQueue->empty() ) {
	_LINK_DBG("key=%lu current=%lu\n",(unsigned long)recvQueue->key(),
		  (unsigned long)simulation->getCurrentSimCycle());
	if ( recvQueue->key() <=  simulation->getCurrentSimCycle() ) {
	    event = recvQueue->top().second;
	    recvQueue->pop();
	}
    }
    return static_cast<CompEvent*>(event);
} 
    
void Link::setDefaultTimeBase(TimeConverter* tc) {
  defaultTimeBase = tc;
}
    
} // namespace SST

