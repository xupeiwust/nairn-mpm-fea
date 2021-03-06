/********************************************************************************
	UpdateMomentaTask.cpp
	nairn-mpm-fea

	Created by John Nairn on July 22, 2010
	Copyright (c) 2010 John A. Nairn, All rights reserved.
 
	The tasks are:
	-------------
	* Updates momenta on the nodes using
		pk(i+1) = pk(i) + ftot(i)*dt
	  for each velocity field on each node.
	* If transport activated, find transport rate by dividing
	  transport flow by transport mass
	* Once get new momenta, check for material contact. Crack contact
	  is checked in a separate step outside the main loop. The material
	  contact checks all nodes. The crack contact	looks only at nodes known
	  to have cracks
	  Note: If either contact changes momenta, change force too to keep consistent with
	  momentum change (because not in post-update tasks)
********************************************************************************/

#include "stdafx.h"
#include "NairnMPM_Class/UpdateMomentaTask.hpp"
#include "NairnMPM_Class/NairnMPM.hpp"
#include "Custom_Tasks/TransportTask.hpp"
#include "Nodes/NodalPoint.hpp"
#include "Cracks/CrackNode.hpp"
#include "Exceptions/CommonException.hpp"
#include "Nodes/MaterialContactNode.hpp"
#include "Boundary_Conditions/NodalVelBC.hpp"

#pragma mark CONSTRUCTORS

UpdateMomentaTask::UpdateMomentaTask(const char *name) : MPMTask(name)
{
}

#pragma mark REQUIRED METHODS

// Update grid momenta and transport rates
// throws CommonException()
void UpdateMomentaTask::Execute(void)
{
#pragma omp parallel for
	for(int i=1;i<=*nda;i++)
	{	NodalPoint *ndptr = nd[nda[i]];
		
		// update nodal momenta
		ndptr->UpdateMomentaOnNode(timestep);
		
		// get grid transport rates
		TransportTask::GetTransportRatesOnNode(ndptr);
	}
	
	// contact and BCs
	ContactAndMomentaBCs(UPDATE_MOMENTUM_CALL);
}

// do contact calculations and impose momenta conditions
void UpdateMomentaTask::ContactAndMomentaBCs(int passType)
{
	// material contact
	MaterialContactNode::ContactOnKnownNodes(timestep,passType);
	
	// adjust momenta and forces for crack contact on known nodes
	CrackNode::ContactOnKnownNodes(timestep,passType);

	// Impose velocity BCs
	NodalVelBC::GridMomentumConditions(passType);

}
