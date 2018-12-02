/********************************************************************************
	CrackVelocityField.hpp
	nairn-mpm-fea
 
	Created by John Nairn on 11 August 2009.
	Copyright (c) 2009 John A. Nairn, All rights reserved.
 
	Dependencies
		MatVelocityField.hpp
********************************************************************************/

#ifndef _CRACKVELOCITYFIELD_

#define _CRACKVELOCITYFIELD_

typedef struct {
	int numParticles;
	Vector cmVel;
	Vector cmAcc;
} CenterMassField;

// when activated, extrapolated momenta in symmetry directions are set to zero
// A development topic - difference seems small with slight preference to activate this option
#define ADJUST_EXTRAPOLATED_PK_FOR_SYMMETRY

#define FIRST_CRACK 0
#define SECOND_CRACK 1

#define MASS_MOMENTUM_CALL 1
#define UPDATE_MOMENTUM_CALL 2
#define UPDATE_STRAINS_LAST_CALL 4

#include "Nodes/MatVelocityField.hpp"

class MPMBase;
class NodalPoint;
class MaterialContactNode;

class CrackVelocityField
{
	public:
		// variables (changed in MPM time step)
		short loc[2];				// crack location's
		int crackNum[2];			// crack numbers
		Vector norm[2];				// crack normals
		DispField *df;				// For J and K calculations
		CenterMassField *cm;		// center of mass data when moving crack planes by ctr mass

		// constants (not changed in MPM time step)
	
		// constructors and destructors
        CrackVelocityField(int,short,int);
        virtual ~CrackVelocityField();
		virtual MatVelocityField *CreateMatVelocityField(int);
		virtual void Zero(short,int,bool);
		virtual void ZeroMatFields(void) = 0;
		virtual void AddMatVelocityField(int);
        virtual bool NeedsMatVelocityField(int) const;
		virtual void MatchRealFields(CrackVelocityField *);
		virtual void MatchMatVelocityFields(MatVelocityField **);
		
		// specific task methods
		virtual void AddMomentumTask1(int,Vector *,Vector *,int);
		virtual void AddMass(int,double);
		virtual void AddMassTask1(int,double,int);
		virtual double GetTotalMassAndCount(bool &) = 0;
		virtual void AddVolumeGradient(int,MPMBase *,double,double,double);
		virtual void CopyVolumeGradient(int,Vector *);
		virtual void CopyMassAndMomentum(NodalPoint *);
        virtual void CopyMassAndMomentumLast(NodalPoint *);
        virtual void RezeroNodeTask6(double) = 0;
        void AddMomentumTask6(int,double,Vector *);
		void AddRigidVelocityAndFlags(Vector *,double,int);
		int ReadAndZeroRigidVelocity(Vector *);
	
		virtual void AddFtotTask3(int,Vector *);
		virtual void CopyGridForces(NodalPoint *);
		virtual void AddFtotSpreadTask3(Vector *) = 0;
		virtual void AddGravityAndBodyForceTask3(Vector *) = 0;
		virtual void RestoreMomenta(void) = 0;
	
		virtual void UpdateMomentaOnField(double) = 0;
		virtual void IncrementDelvaTask5(int,double,GridToParticleExtrap *) const;
	
		void CreateStrainField(void);
		void DeleteStrainField(void);

		short IncrementDelvTask8(double,Vector *,Vector *,double *);
		void SetCMVelocityTask8(Vector *,int,Vector *);
		bool GetCMVelocityTask8(Vector *,Vector *) const;
		bool CollectMomentaTask8(Vector *,double *,Vector *) const;
	
		void AddNormals(Vector *,int);
		void AddDisplacement(int,double,Vector *);
		void AddVolume(int,double);
	
		// methods
		virtual void MaterialContactOnCVF(MaterialContactNode *,double,int);
		virtual bool HasVolumeGradient(int) const;
		virtual void GetVolumeGradient(int,const NodalPoint *,Vector *,double) const;
		virtual void CalcVelocityForStrainUpdate(void) = 0;
        virtual void AdjustForSymmetry(NodalPoint *,Vector *,bool) const;
	
		// boundary conditions
        virtual void SetMomVel(Vector *,int) = 0;
        virtual void AddMomVel(Vector *,double,int) = 0;
		virtual void ReflectMomVel(Vector *,CrackVelocityField *,double,double,int) = 0;
        virtual void SetFtotDirection(Vector *,double,Vector *) = 0;
        virtual void AddFtotDirection(Vector *,double,double,Vector *) = 0;
		virtual void ReflectFtotDirection(Vector *,double,CrackVelocityField *,double,double,Vector *) = 0;
	
		// accessors
		short location(int);
		int crackNumber(int);
		int OppositeCrackTo(int,int,int *);
		void SetLocationAndCrack(short,int,int);
		MatVelocityField **GetMaterialVelocityFields(void);
		MatVelocityField *GetMaterialVelocityField(int);
		virtual double GetTotalMass(bool) const = 0;
		virtual void AddKineticEnergyAndMass(double &,double &) = 0;
		virtual double GetVolumeNonrigid(bool) const = 0;
		virtual double GetVolumeTotal(NodalPoint *) const = 0;
		virtual Vector GetCMatMomentum(bool &,double *,Vector *) const = 0;
		virtual Vector GetCMDisplacement(NodalPoint *,bool) const = 0;
		virtual Vector GetCMatFtot(void) = 0;
		virtual void ChangeCrackMomentum(Vector *,int,double) = 0;
		virtual int CopyFieldMomenta(Vector *,int) = 0;
#ifdef ADJUST_EXTRAPOLATED_PK_FOR_SYMMETRY
		virtual void AdjustForSymmetryBC(NodalPoint *) = 0;
#endif
		virtual int PasteFieldMomenta(Vector *,int) = 0;
		Vector GetVelocity(int);
		virtual int GetNumberPoints(void);
		virtual int GetNumberMaterials(void);
		virtual void SetNumberPoints(int);
		virtual bool HasPointsNonrigid(void) const;
		virtual void Describe(void) const;
		virtual void SumAndClearRigidContactForces(Vector *,bool,double,Vector *);
		virtual int GetFieldNum(void) const;
		virtual int HasPointsThatSeeCracks(void);
		virtual int GetNumberNonrigidMaterials(void);
	
		// class methods
		static bool ActiveField(CrackVelocityField *);
        static bool ActiveCrackField(CrackVelocityField *);
		static bool ActiveNonrigidField(CrackVelocityField *);
		static bool ActiveNonrigidField(CrackVelocityField *,int);
		static CrackVelocityField *CreateCrackVelocityField(int,short,int);
	
	protected:
		// variables (changed in MPM time step)
		int fieldNum;				// field number [0] to [3]
		int numberPoints;			// total number of materials points in this field/field [0] changed to sum of all in task 8
		MatVelocityField **mvf;		// material velocity fields
        bool hasCrackPoints;        // shows a particle sees this field during initialization
		// unscaled nonrigid volume (ignores dilation) only used for imperfect interface forces and material contact
		// unscaleRigidVolume is due to rigid contaft materials (type 8) (always zero unless multimaterial mode)
};

#endif
