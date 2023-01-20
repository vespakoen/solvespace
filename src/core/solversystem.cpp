#include "solversystem.h"

SolverResult SolverSystem::Solve(Group *g) {
  System sys;

  std::cout << "Adding params\n";
  // copy params from sketch to system
  for (Param &param : SK.param) {
    Param *p = &param;
    p->known = false;
    p->val   = SK.GetParam(p->h)->val;
    sys.param.Add(p);
  }

  std::cout << "Adding constraints\n";
  // copy params from sketch constraints to system and mark dragged
  for(ConstraintBase &con : SK.constraint) {
      ConstraintBase *c = &con;
      if(c->group != g->h)
        continue;
      std::cout << "Calling generate\n";
      c->Generate(&(sys.param));
      if (c->type == ConstraintBase::Type::WHERE_DRAGGED) {
        std::cout << "Where dragged, getting entity\n";
        EntityBase *pt = SK.GetEntity(c->ptA);
        sys.dragged.Add(&(pt->param[0]));
        sys.dragged.Add(&(pt->param[1]));
      }
      // IdList<Param, hParam> constraintParams = {};
      // c->Generate(&constraintParams);
      // if(!constraintParams.IsEmpty()) {
      //   for(Param &p : constraintParams) {
      //       p.h    = param.AddAndAssignId(&p);
      //       c->valP = p.h;
      //       // sys.param.Add(&p);
      //   }
      //   constraintParams.Clear();
      //   c->ModifyToSatisfy();
      // }
  }

  int rank = 0;
  int dof = 0;
  List<hConstraint> badList;
  std::cout << "Solving\n";
  SolveResult status = sys.Solve(g, &rank, &dof, &badList, false, false, false);

  std::vector<hConstraint> bad;
  for (hConstraint &b : badList) {
      bad.push_back(b);
  }

  SolverResult result;
  result.status = status;
  result.bad = bad;
  result.rank = rank;
  result.dof = dof;

  return result;
}