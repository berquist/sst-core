// Copyright 2009-2015 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2015, Sandia Corporation
// All rights reserved.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.


#include <sst_config.h>
#include "constant.h"

SSTConstantDistribution::SSTConstantDistribution(double v) :
	SSTRandomDistribution() {

	mean = v;
}

SSTConstantDistribution::~SSTConstantDistribution() {

}

double SSTConstantDistribution::getNextDouble() {
	return mean;
}

double SSTConstantDistribution::getMean() {
	return mean;
}
