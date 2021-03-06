/*****************************************************************************/
/*                                                                           */
/*              8888888         88888888         88888888                    */
/*            8                8                8                            */
/*           8                 8                8                            */
/*           8                  88888888         88888888                    */
/*           8      8888                8                8                   */
/*            8       8                 8                8                   */
/*              888888         888888888        888888888                    */
/*                                                                           */
/*       A Two-Dimensional General Purpose Semiconductor Simulator.          */
/*                                                                           */
/*  GSS material database Version 0.4                                        */
/*  Last update: Feb 17, 2006                                                */
/*                                                                           */
/*  Gong Ding                                                                */
/*  gdiso@ustc.edu                                                           */
/*  NINT, No.69 P.O.Box, Xi'an City, China                                   */
/*                                                                           */
/*****************************************************************************/
//
// Material Type: GaAs


#include "PMI.h"

class GSS_GaAs_Thermal : public PMIS_Thermal
{

private:
  PetscScalar A_SP_HEA;	// First parameter for the specific heat model of the material.
  PetscScalar B_SP_HEA;	// Second parameter for the specific heat model of the material.
  PetscScalar C_SP_HEA;	// Third parameter for the specific heat model of the material.
  PetscScalar D_SP_HEA;	// Fourth parameter for the specific heat model of the material.
  PetscScalar F_SP_HEA;	// Fifth parameter for the specific heat model of the material.
  PetscScalar G_SP_HEA;	// Sixth parameter for the specific heat model of the material.
  PetscScalar A_TH_CON;	// First parameter for the thermal conductivity model of the material.
  PetscScalar B_TH_CON;	// Second parameter for the thermal conductivity model of the material.
  PetscScalar C_TH_CON;	// Third parameter for the thermal conductivity model of the material.
  PetscScalar E_TH_CON;	// Fifth parameter for the thermal conductivity model of the material.
  PetscScalar D_TH_CON;	// Fourth parameter for the thermal conductivity model of the material.
  PetscScalar T300;
  void   Thermal_Init()
  {
    A_SP_HEA =  1.815000e+02*J/kg/K;
    B_SP_HEA =  8.225000e-01*J/kg/std::pow(K,2);
    C_SP_HEA =  -1.350000e-03*J/kg/std::pow(K,3);
    D_SP_HEA =  0.000000e+00*J/kg*K;
    F_SP_HEA =  7.500000e-07*J/kg/std::pow(K,4);
    G_SP_HEA =  0.000000e+00*J/kg/std::pow(K,5);
    A_TH_CON =  2.964400e+00*cm*K/J*s;
    B_TH_CON =  -1.392550e-02*cm/J*s;
    C_TH_CON =  5.104750e-05*cm/J*s/K;
    E_TH_CON =  0.000000e+00;
    D_TH_CON =  -3.807500e-08*cm/J*s*std::pow(K,1-E_TH_CON);
    T300     =  300.0*K;
#ifdef __CALIBRATE__
    parameter_map.insert(para_item("A.SP.HEA",  PARA("A.SP.HEA",    "Constant term in the specific heat model.", "J kg^-1 K^-1", J/kg/K, &A_SP_HEA)) );
    parameter_map.insert(para_item("B.SP.HEA",  PARA("B.SP.HEA",    "Prefactor of the linear term in the specific heat model.", "J kg^-1 K^-2", J/kg/std::pow(K,2), &B_SP_HEA)) );
    parameter_map.insert(para_item("C.SP.HEA",  PARA("C.SP.HEA",    "Prefactor of the quadratic term in the specific heat model.", "J kg^-1 K^-3", J/kg/std::pow(K,3), &C_SP_HEA)) );
    parameter_map.insert(para_item("D.SP.HEA",  PARA("D.SP.HEA",    "Prefactor of the inverse-squared term in the specific heat model.", "J kg^-1 K", J/kg*K, &D_SP_HEA)) );
    parameter_map.insert(para_item("F.SP.HEA",  PARA("F.SP.HEA",    "Prefactor of the cubic term in the specific heat model.", "J kg^-1 K^-4", J/kg/std::pow(K,4), &F_SP_HEA)) );
    parameter_map.insert(para_item("G.SP.HEA",  PARA("G.SP.HEA",    "Prefactor of the 4th-order term in the specific heat model.", "J kg^-1 K^-5", J/kg/std::pow(K,5), &G_SP_HEA)) );
    
    parameter_map.insert(para_item("A.TH.CON",  PARA("A.TH.CON",    "Constant term in the denominator of the thermal conductivity model.", "cm s K/J/s", cm*K/J*s, &A_TH_CON)) );
    parameter_map.insert(para_item("B.TH.CON",  PARA("B.TH.CON",    "Prefactor of the linear term in the denominator of the thermal conductivity model.", "cm s/J", cm/J*s, &B_TH_CON)) );
    parameter_map.insert(para_item("C.TH.CON",  PARA("C.TH.CON",    "Prefactor of the quadratic term in the denominator of the thermal conductivity model.", "cm s/J/K", cm/J*s/K, &C_TH_CON)) );
    parameter_map.insert(para_item("D.TH.CON",  PARA("D.TH.CON",    "Prefactor of the last term in the denominator of the thermal conductivity model.", "cm s/J [K^(1-E.TH.CON)]", cm/J*s*std::pow(K,1-E_TH_CON), &D_TH_CON)) );
    parameter_map.insert(para_item("E.TH.CON",  PARA("E.TH.CON",    "Exponent of the last term in the denominator of the thermal conductivity model.", "-", 1.0, &E_TH_CON)) );
#endif
  }
public:
  //---------------------------------------------------------------------------
  // Heat Capacity
  PetscScalar HeatCapacity  (const PetscScalar &Tl) const
  {
    return A_SP_HEA + B_SP_HEA*Tl + C_SP_HEA*Tl*Tl + D_SP_HEA/Tl/Tl
           + F_SP_HEA*Tl*Tl*Tl + G_SP_HEA*Tl*Tl*Tl*Tl;
  }
  AutoDScalar HeatCapacity  (const AutoDScalar &Tl) const
  {
    return A_SP_HEA + B_SP_HEA*Tl + C_SP_HEA*Tl*Tl + D_SP_HEA/Tl/Tl
           + F_SP_HEA*Tl*Tl*Tl + G_SP_HEA*Tl*Tl*Tl*Tl;
  }

  //---------------------------------------------------------------------------
  // Heat Conduction  
  PetscScalar HeatConduction(const PetscScalar &Tl) const
  {
    return 1.0/(A_TH_CON + B_TH_CON*Tl + C_TH_CON*Tl*Tl + D_TH_CON*std::pow(Tl,E_TH_CON));
  }
  AutoDScalar HeatConduction(const AutoDScalar &Tl) const
  {
    return 1.0/(A_TH_CON + B_TH_CON*Tl + C_TH_CON*Tl*Tl);
  }

  

// constructor and destructor  
public:   
  GSS_GaAs_Thermal(const PMIS_Environment &env):PMIS_Thermal(env)
  {
    Thermal_Init();
  }
  ~GSS_GaAs_Thermal()
  {
  }
}
;

extern "C"
{
  DLL_EXPORT_DECLARE  PMIS_Thermal* PMIS_GaAs_Thermal_Default (const PMIS_Environment& env)
  {
    return new GSS_GaAs_Thermal(env);
  }
}
