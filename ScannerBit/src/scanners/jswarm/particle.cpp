//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  j-Swarm: particle swarm optimisation with
///  meta-optimisation a la jDE.
///
///  Implementations of particle class
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///
///  \author Pat Scott
///          (pat.scott@uq.edu.au)
///  \date 2019 Oct
///
///  *********************************************

#include "gambit/ScannerBit/scanners/jswarm/1.0.0/jswarm.hpp"


namespace Gambit
{

  namespace jswarm_1_0_0
  {

    /// Constructor
    particle::particle(int nPar, std::shared_ptr<double> lb, std::shared_ptr<double> ub, rng_type& generator)
    {
      if (nPar <= 0) scan_error().raise(LOCAL_INFO, "You must set nPar positive before creating a particle!");
      x.resize(nPar);
      y.resize(nPar);
      lowerbounds = lb;
      upperbounds = ub;
      rng = &generator;
    }

    /// Initialise a particle with a random position and zero/random velocity
    void particle::init(bool zero_vel)
    {
      for (int i = 0; i < x.size(); i++)
      {
        double extent = upperbounds[i] - lowerbounds[i];
        // Set position randomly
        x[i] = lowerbounds[i] + extent * std::generate_canonical<double, 32>(*rng);
        // Reset velocity to either zero or a random number in [-extent,extent].  Velocity is in units of [x] per generation 'timestep'.
        v[i] = (zero_vel ? 0.0 : extent * (2.0 * std::generate_canonical<double, 32>(*rng) - 1.0));
      }
    }

    /// Update the particle's personal best fit seen so far
    void particle::update_personal_best()
    {
      if (p.lnlike > p.personal_best_value)
      {
        p.personal_best_value = p.lnlike;
        p.personal_best_x = p.x
      }
    }

    /// Reflect a particle's position and velocity off the walls of the prior box
    void particle::reflect()
    {
      for (int i = 0; i < x.size(); i++)
      {
        double bound_violated, other_bound;
        double lb = lowerbounds[i];
        double ub = upperbounds[i];

        // Work out whether it is the upper, lower, or no bound exceeded for this dimension
        if (p.x[i] < lb)
        {
          bound_violated = lb;
          other_bound = ub;
        }
        else if (p.x[i] > ub)
        {
          bound_violated = ub;
          other_bound = lb;
        }
        else continue;

        // Work out how many prior box lengths the particle has overshot (= how many reflections needed)
        double overshoot_factor = (p.x[i] - bound) / (ub - lb);
        int reflections = (int) overshoot_factor;
        double offset = (overshoot_factor - reflections) *  (ub - lb);

        // Perform the reflections
        if (k%2 == 1) // odd number
        {
          x[i] = bound_violated - offset;
          v[i] = -v[i];
        }
        else x[i] = other_bound + offset; //even number
      }

    }


  }

}


