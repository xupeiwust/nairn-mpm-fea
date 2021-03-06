/********************************************************************************
	CrackVelocityFieldSingle.hpp
	nairn-mpm-fea
 
	Created by John Nairn on 21 August 2009.
	Copyright (c) 2009 John A. Nairn, All rights reserved.
 
	Dependencies
		CrackVelocityField.hpp,MatVelocityField.hpp
********************************************************************************/

#ifndef _CRACKVELOCITYFIELDSINGLE_

#define _CRACKVELOCITYFIELDSINGLE_

#include "Nodes/CrackVelocityField.hpp"

class CrackVelocityFieldSingle : public CrackVelocityField
{
	public:
		
        // constructors and destructors
		CrackVelocityFieldSingle(int,short,int);
        virtual ~CrackVelocityFieldSingle();
		virtual void ZeroMatFields(void);
	
		// specific task methods
		virtual double GetTotalMassAndCount(bool &);
	
		virtual void AddFtotSpreadTask3(Vector *);
		virtual void AddGravityAndBodyForceTask3(Vector *);
		virtual void RestoreMomenta(void);
	
		virtual void UpdateMomentaOnField(double);

		virtual void RezeroNodeTask6(double);
	
		virtual void CalcVelocityForStrainUpdate(void);
	
		// boundary conditions
        virtual void SetMomVel(Vector *,int);
        virtual void AddMomVel(Vector *,double,int);
		virtual void ReflectMomVel(Vector *,CrackVelocityField *,double,double,int);
        virtual void SetFtotDirection(Vector *,double,Vector *);
        virtual void AddFtotDirection(Vector *,double,double,Vector *);
		virtual void ReflectFtotDirection(Vector *,double,CrackVelocityField *,double,double,Vector *);
	
		// accessors
		virtual double GetTotalMass(bool) const;
		virtual void AddKineticEnergyAndMass(double &,double &);
		virtual double GetVolumeNonrigid(bool) const;
		virtual double GetVolumeTotal(NodalPoint *) const;
		virtual Vector GetCMatMomentum(bool &,double *,Vector *) const;
		virtual Vector GetCMDisplacement(NodalPoint *,bool) const;
		virtual Vector GetCMatFtot(void);
		virtual void ChangeCrackMomentum(Vector *,int,double);
		virtual int CopyFieldMomenta(Vector *,int);
#ifdef ADJUST_EXTRAPOLATED_PK_FOR_SYMMETRY
		virtual void AdjustForSymmetryBC(NodalPoint *);
#endif
		virtual int PasteFieldMomenta(Vector *,int);
		virtual void Describe(void) const;
	
};

#endif
