// ********************************************************************
//
// eASTPrimaryGeneratorAction.hh
//   Header file of Gorad primary generator action class
//
// History
//  May 8th, 2021 : first implementation - Makoto Asai (SLAC)
//  June 23rd, 2021 : Add eASTHepMC3Interface - Makoto Asai (SLAC)
//
// ********************************************************************

#ifndef eASTPrimaryGeneratorAction_h
#define eASTPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;

#ifdef eAST_USE_HepMC3
class eASTHepMC3Interface;
#endif // eAST_USE_HepMC3

class eASTPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    eASTPrimaryGeneratorAction(G4bool,G4bool,G4bool);
    virtual ~eASTPrimaryGeneratorAction();
    
    virtual void GeneratePrimaries(G4Event*);
 
  private:
    G4ParticleGun* fParticleGun = nullptr;
    G4GeneralParticleSource* fParticleSource = nullptr;
#ifdef eAST_USE_HepMC3
  eASTHepMC3Interface* fHepMC3Interface = nullptr;
#endif // eAST_USE_HepMC3
};

#endif
