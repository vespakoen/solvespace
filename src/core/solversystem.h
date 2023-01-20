#ifndef SOLVESPACE_SOLVERSYSTEM_H
#define SOLVESPACE_SOLVERSYSTEM_H

#include <cstdio>
#include "data.h"
#include "group.h"
#include "system.h"
#include "constraintbase.h"
#include "sketch.h"

struct SolverResult {
    SolveResult status;
    int dof;
    int rank;
    std::vector<hConstraint> bad;
};

class SolverSystem {
public:
  SolverResult Solve(Group *g);
};

#endif