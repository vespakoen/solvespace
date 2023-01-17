# distutils: language=c++
# -*- coding: utf-8 -*-
# cython: language_level=3

"""Wrapper header of Solvespace.

author: Yuan Chang
copyright: Copyright (C) 2016-2019
license: GPLv3+
email: pyslvs@gmail.com
"""

from libc.stdint cimport uint32_t
from libcpp.vector cimport vector
from libcpp cimport bool

cdef extern from "lib.h" nogil:
    cdef cppclass Vector:
        Vector() except +
    cdef cppclass Vector4:
        Vector4() except +
    cdef cppclass hEntity:
        hEntity() except +
        uint32_t v
    cdef cppclass hParam:
        uint32_t v
    cdef cppclass hConstraint:
        uint32_t v
    cdef cppclass hGroup:
        uint32_t v

    cdef cppclass EntityBase:
        EntityBase() except +
        EntityBase FREE_IN_3D
        int tag
        Type type
        vector[hEntity] point
        hEntity h, workplane
        hGroup group
        vector[hParam] params
        bool HasVector()
        # Vector PointGetNum()

    cdef enum Type "EntityBase::Type":
        POINT_IN_3D            =  2000
        POINT_IN_2D            =  2001
        POINT_N_TRANS          =  2010
        POINT_N_ROT_TRANS      =  2011
        POINT_N_COPY           =  2012
        POINT_N_ROT_AA         =  2013
        POINT_N_ROT_AXIS_TRANS =  2014
        NORMAL_IN_3D           =  3000
        NORMAL_IN_2D           =  3001
        NORMAL_N_COPY          =  3010
        NORMAL_N_ROT           =  3011
        NORMAL_N_ROT_AA        =  3012
        DISTANCE               =  4000
        DISTANCE_N_COPY        =  4001
        FACE_NORMAL_PT         =  5000
        FACE_XPROD             =  5001
        FACE_N_ROT_TRANS       =  5002
        FACE_N_TRANS           =  5003
        FACE_N_ROT_AA          =  5004
        FACE_ROT_NORMAL_PT     =  5005
        FACE_N_ROT_AXIS_TRANS  =  5006
        WORKPLANE              = 10000
        LINE_SEGMENT           = 11000
        CUBIC                  = 12000
        CUBIC_PERIODIC         = 12001
        CIRCLE                 = 13000
        ARC_OF_CIRCLE          = 14000
        TTF_TEXT               = 15000
        IMAGE                  = 16000

    cdef cppclass ConstraintBase:
        int tag
        # bool Equals(ConstraintBase c)
        # bool ConstrainArcLineTangent(ConstraintBase c, EntityBase line, EntityBase arc)

# cpdef tuple quaternion_u(double qw, double qx, double qy, double qz)
# cpdef tuple quaternion_v(double qw, double qx, double qy, double qz)
# cpdef tuple quaternion_n(double qw, double qx, double qy, double qz)
# cpdef tuple make_quaternion(double ux, double uy, double uz, double vx, double vy, double vz)

# cdef class Params:

#     cdef vector[Slvs_hParam] param_list

#     @staticmethod
#     cdef Params create(Slvs_hParam *p, size_t count)

# cdef class Entity:

#     cdef int t
#     cdef readonly Slvs_hEntity[12] point
#     cdef Slvs_hEntity h, wp
#     cdef Slvs_hGroup g
#     cdef readonly Params params

#     @staticmethod
#     cdef Entity create(Slvs_Entity *e)
#     cpdef list points(self)
#     cpdef bint is_3d(self)
#     cpdef bint is_none(self)
#     cpdef bint is_point_2d(self)
#     cpdef bint is_point_3d(self)
#     cpdef bint is_point(self)
#     cpdef bint is_normal_2d(self)
#     cpdef bint is_normal_3d(self)
#     cpdef bint is_normal(self)
#     cpdef bint is_distance(self)
#     cpdef bint is_work_plane(self)
#     cpdef bint is_line_2d(self)
#     cpdef bint is_line_3d(self)
#     cpdef bint is_line(self)
#     cpdef bint is_cubic(self)
#     cpdef bint is_circle(self)
#     cpdef bint is_arc(self)


# cdef class SolverSystem:

#     cdef int dof_v
#     cdef Slvs_hGroup g
#     cdef vector[Slvs_Param] param_list
#     cdef vector[Slvs_Entity] entity_list
#     cdef vector[Slvs_Constraint] cons_list
#     cdef vector[Slvs_hConstraint] failed_list

#     cpdef SolverSystem copy(self)
#     cpdef void clear(self)
#     cpdef void set_group(self, size_t g)
#     cpdef int group(self)
#     cpdef void set_params(self, Params p, object params)
#     cpdef list params(self, Params p)
#     cpdef int dof(self)
#     cpdef object constraints(self)
#     cpdef list failures(self)
#     cdef int solve_c(self) nogil

#     cpdef size_t param_len(self)
#     cpdef size_t entity_len(self)
#     cpdef size_t cons_len(self)

#     cpdef Entity create_2d_base(self)
#     cdef Slvs_hParam new_param(self, double val) nogil
#     cdef Slvs_hEntity eh(self) nogil

#     cpdef Entity add_point_2d(self, double u, double v, Entity wp)
#     cpdef Entity add_point_3d(self, double x, double y, double z)
#     cpdef Entity add_normal_2d(self, Entity wp)
#     cpdef Entity add_normal_3d(self, double qw, double qx, double qy, double qz)
#     cpdef Entity add_distance(self, double d, Entity wp)
#     cpdef Entity add_line_2d(self, Entity p1, Entity p2, Entity wp)
#     cpdef Entity add_line_3d(self, Entity p1, Entity p2)
#     cpdef Entity add_cubic(self, Entity p1, Entity p2, Entity p3, Entity p4, Entity wp)
#     cpdef Entity add_arc(self, Entity nm, Entity ct, Entity start, Entity end, Entity wp)
#     cpdef Entity add_circle(self, Entity nm, Entity ct, Entity radius, Entity wp)
#     cpdef Entity add_work_plane(self, Entity origin, Entity nm)
#     cpdef int add_constraint(
#         self,
#         int c_type,
#         Entity wp,
#         double v,
#         Entity p1,
#         Entity p2,
#         Entity e1,
#         Entity e2,
#         Entity e3 = *,
#         Entity e4 = *,
#         int other = *,
#         int other2 = *
#     )

#     cpdef int coincident(self, Entity e1, Entity e2, Entity wp = *)
#     cpdef int distance(self, Entity e1, Entity e2, double value, Entity wp = *)
#     cpdef int equal(self, Entity e1, Entity e2, Entity wp = *)
#     cpdef int equal_angle(self, Entity e1, Entity e2, Entity e3, Entity e4, Entity wp = *)
#     cpdef int equal_point_to_line(self, Entity e1, Entity e2, Entity e3, Entity e4, Entity wp = *)
#     cpdef int ratio(self, Entity e1, Entity e2, double value, Entity wp = *)
#     cpdef int symmetric(self, Entity e1, Entity e2, Entity e3 = *, Entity wp = *)
#     cpdef int symmetric_h(self, Entity e1, Entity e2, Entity wp)
#     cpdef int symmetric_v(self, Entity e1, Entity e2, Entity wp)
#     cpdef int midpoint(self, Entity e1, Entity e2, Entity wp = *)
#     cpdef int horizontal(self, Entity e1, Entity wp, Entity e2 = *)
#     cpdef int vertical(self, Entity e1, Entity wp, Entity e2 = *)
#     cpdef int diameter(self, Entity e1, double value)
#     cpdef int same_orientation(self, Entity e1, Entity e2)
#     cpdef int angle(self, Entity e1, Entity e2, double value, Entity wp = *, bint inverse = *)
#     cpdef int perpendicular(self, Entity e1, Entity e2, Entity wp = *, bint inverse = *)
#     cpdef int parallel(self, Entity e1, Entity e2, Entity wp = *)
#     cpdef int tangent(self, Entity e1, Entity e2, Entity wp = *)
#     cpdef int distance_proj(self, Entity e1, Entity e2, double value)
#     cpdef int dragged(self, Entity e1, Entity wp = *)
#     cpdef int length_diff(self, Entity e1, Entity e2, double value, Entity wp = *)
