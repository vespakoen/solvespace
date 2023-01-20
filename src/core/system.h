#ifndef SOLVESPACE_SYSTEM_H
#define SOLVESPACE_SYSTEM_H

#include <iostream>
#include <cassert>
#define EIGEN_NO_DEBUG
#undef Success
#include <Eigen/SparseCore>
#include <Eigen/Core>
#include <Eigen/SparseQR>

#include "data.h"
#include "expr.h"
#include "utils.h"
#include "group.h"
#include "entitybase.h"
#include "constraintbase.h"
#include "sketch.h"

class System {
public:
    enum { MAX_UNKNOWNS = 2048 };

    // EntityList entity;
    ParamList param;
    IdList<Equation, hEquation> eq;

    // std::vector<EntityBase> entity;
    // std::vector<Param> param;
    // std::vector<Equation> eq;

    // A list of parameters that are being dragged; these are the ones that
    // we should put as close as possible to their initial positions.
    List<hParam> dragged;

    enum {
        // In general, the tag indicates the subsys that a variable/equation
        // has been assigned to; these are exceptions for variables:
        VAR_SUBSTITUTED = 10000,
        VAR_DOF_TEST    = 10001,
        // and for equations:
        EQ_SUBSTITUTED = 20000
    };

    // The system Jacobian matrix
    struct {
        // The corresponding equation for each row
        std::vector<Equation *> eq;

        // The corresponding parameter for each column
        std::vector<hParam> param;

        // We're solving AX = B
        int m, n;
        struct {
            // This only observes the Expr - does not own them!
            Eigen::SparseMatrix<Expr *> sym;
            Eigen::SparseMatrix<double> num;
        } A;

        Eigen::VectorXd scale;
        Eigen::VectorXd X;

        struct {
            // This only observes the Expr - does not own them!
            std::vector<Expr *> sym;
            Eigen::VectorXd num;
        } B;
    } mat;

    static const double CONVERGE_TOLERANCE;
    int CalculateRank();
    bool TestRank(int *dof = NULL);
    static bool SolveLinearSystem(const Eigen::SparseMatrix<double> &A, const Eigen::VectorXd &B,
                                  Eigen::VectorXd *X);
    bool SolveLeastSquares();

    bool WriteJacobian(int tag);
    void EvalJacobian();

    void WriteEquationsExceptFor(hConstraint hc, Group *g);
    void FindWhichToRemoveToFixJacobian(Group *g, List<hConstraint> *bad, bool forceDofCheck);
    void SolveBySubstitution();

    bool IsDragged(hParam p);

    bool NewtonSolve(int tag);

    void MarkParamsFree(bool findFree);

    SolveResult Solve(Group *g, int *rank = NULL, int *dof = NULL, List<hConstraint> *bad = NULL,
                      bool andFindBad = false, bool andFindFree = false,
                      bool forceDofCheck = false);

    SolveResult SolveRank(Group *g, int *rank = NULL, int *dof = NULL,
                          List<hConstraint> *bad = NULL, bool andFindBad = false,
                          bool andFindFree = false);

    void Clear();
    Param *GetLastParamSubstitution(Param *p);
    void SubstituteParamsByLast(Expr *e);
    void SortSubstitutionByDragged(Param *p);
};

#endif