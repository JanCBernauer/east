////////////////////////////////////////////////////////////////////////////////
//
//  eASTProtonPhysics.cc
//  Proton hadronic physics constructor for eASTPhysicsList
//
//    Jun.21.2018 : original implementation - Dennis H. Wright (SLAC)
//    May.06.2021 : migration to eAST - Makoto Asai (SLAC)
//
////////////////////////////////////////////////////////////////////////////////


#include "eASTProtonPhysics.hh"

#include "G4ProcessManager.hh"
#include "G4ProtonInelasticProcess.hh"
#include "G4HadronElasticProcess.hh"

#include "G4CascadeInterface.hh"
#include "G4TheoFSGenerator.hh"
#include "G4FTFModel.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4LundStringFragmentation.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4ChipsElasticModel.hh"

#include "G4BGGNucleonInelasticXS.hh"
#include "G4ChipsProtonElasticXS.hh"

#include "G4SystemOfUnits.hh"


eASTProtonPhysics::eASTProtonPhysics()
{}


eASTProtonPhysics::~eASTProtonPhysics()
{
  delete stringDecay;
  delete stringModel;
  delete fragModel;
  delete preCompoundModel;
}


void eASTProtonPhysics::ConstructProcess()
{
  // Low energy elastic model
  G4ChipsElasticModel* elMod = new G4ChipsElasticModel();

  // Use Bertini cascade for low energies
  G4CascadeInterface* loInelModel = new G4CascadeInterface;
  loInelModel->SetMinEnergy(0.0);
  loInelModel->SetMaxEnergy(12.0*GeV);

  // Use FTFP for high energies   ==>>   eventually replace this with new class FTFPInterface
  ftfp = new G4TheoFSGenerator("FTFP");
  stringModel = new G4FTFModel;
  stringDecay =
    new G4ExcitedStringDecay(fragModel = new G4LundStringFragmentation);
  stringModel->SetFragmentationModel(stringDecay);
  preCompoundModel = new G4GeneratorPrecompoundInterface();

  ftfp->SetHighEnergyGenerator(stringModel);
  ftfp->SetTransport(preCompoundModel); 
  ftfp->SetMinEnergy(5*GeV);
  ftfp->SetMaxEnergy(100*TeV);

  // Inelastic cross section
  G4BGGNucleonInelasticXS* inelCS = new G4BGGNucleonInelasticXS(G4Proton::Proton() );
  G4ChipsProtonElasticXS* elCS = new G4ChipsProtonElasticXS;

  G4ProcessManager* procMan = G4Proton::Proton()->GetProcessManager();

  // Elastic process
  G4HadronElasticProcess* pProcEl = new G4HadronElasticProcess;
  pProcEl->RegisterMe(elMod);
  pProcEl->AddDataSet(elCS);
  procMan->AddDiscreteProcess(pProcEl);

  // Inelastic process
  G4ProtonInelasticProcess* pProcInel = new G4ProtonInelasticProcess;
  pProcInel->RegisterMe(loInelModel);
  pProcInel->RegisterMe(ftfp);
  pProcInel->AddDataSet(inelCS);
  procMan->AddDiscreteProcess(pProcInel);

}


void eASTProtonPhysics::ConstructParticle()
{}

