﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
// -*- C++ -*-
#ifndef ARCNUM_NUMERICS_SOLVER_MATRIX_H
#define ARCNUM_NUMERICS_SOLVER_MATRIX_H
/* Author : desrozis at Wed Aug 29 08:39:27 2012
 * Generated by createNew
 */

#include "ArcGeoSim/Numerics/TwoPointsScheme/TwoPointsStencil.h"

#include "ArcGeoSim/Physics/Law2/Contribution/Contribution.h"

#include <alien/ref/data/block/BlockMatrix.h>
#include <alien/ref/handlers/block/ProfiledBlockMatrixBuilder.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace ArcNum {

  /*---------------------------------------------------------------------------*/
  /*---------------------------------------------------------------------------*/

  class Bound;

  /*---------------------------------------------------------------------------*/
  /*---------------------------------------------------------------------------*/

  class Matrix
  {
    struct InternalData
    { 
      InternalData(Alien::BlockMatrix& alien_block_matrix,
		   Arcane::ConstArray2View<Arcane::Integer> all_indexes)
	: m_i(-1)
	, m_j(-1)
	, m_equation(-1)
	, m_well_index(-1)
	, m_alien_block_matrix(alien_block_matrix)
	, m_all_indexes(all_indexes)
	, m_alien_builder(m_alien_block_matrix, Alien::ProfiledBlockMatrixBuilderOptions::eResetValues)
    {
      Alien::allocateData(m_values_2d, m_alien_block_matrix.block().size(), m_alien_block_matrix.block().size());
    }
    Arcane::Integer m_i;
    Arcane::Integer m_j;
    Arcane::Integer m_equation;
    Arcane::Integer m_well_index;
    Alien::BlockMatrix& m_alien_block_matrix;
    Alien::UniqueArray2<Real> m_values_2d;
    Alien::ProfiledBlockMatrixBuilder m_alien_builder;
    Arcane::ConstArray2View<Arcane::Integer> m_all_indexes;
  };
  
public:

  struct MatrixAdder
  {
    MatrixAdder(InternalData& data)
      : m_data(data) {}

    void operator+=(const Law::Contribution& values)
    {
      Alien::fill(m_data.m_values_2d, 0.);
      const Arcane::RealConstArrayView contrib(values.size(),values.derivatives());
      const Arcane::Integer size = contrib.size();
      //ARCANE_ASSERT((size == row.size()),("Sizes are not equal"));
      for(Arcane::Integer k = 0; k < size; ++k) {
	m_data.m_values_2d[m_data.m_equation][k] += contrib[k];
      }
      m_data.m_alien_builder(m_data.m_i, m_data.m_j) += Alien::view(m_data.m_values_2d);
    }
    // TODO (XT)
    void operator-=(const Law::Contribution& values)
    {
      const Arcane::RealConstArrayView contrib(values.size(),values.derivatives());
      const Arcane::Integer size = contrib.size();
      //ARCANE_ASSERT((size == row.size()),("Sizes are not equal"));
      //for(Arcane::Integer k = 0; k < size; ++k) {
      //m_data.m_values_2d[m_data.m_equation][k] -= contrib[k];
      //}
    }
    // TODO (XT)
    void operator=(const Law::Contribution& values)
    {
      const Arcane::RealConstArrayView contrib(values.size(),values.derivatives());
      const Arcane::Integer size = contrib.size();
      //ARCANE_ASSERT((size == row.size()),("Sizes are not equal"));
      //for(Arcane::Integer k = 0; k < size; ++k) {
      //m_data.m_values_2d[m_data.m_equation][k] = contrib[k];
      //
    }
    InternalData& m_data;
  };

  struct MatrixAdderFV
  {
    MatrixAdderFV(InternalData& data,
                  const ArcNum::TwoPointsStencil& stencil)
      : m_data(data)
      , m_stencil(stencil) {}

    void operator+=(const Law::Contribution& values)
    {
      Alien::fill(m_data.m_values_2d, 0.);
      const Arcane::RealConstArrayView v(values.size(),values.derivatives());
      {
	const Arcane::Integer size = m_data.m_values_2d.dim1Size();
	const Arcane::Integer j = m_data.m_all_indexes[m_stencil.back().cell().localId()][0]/size;
        const Arcane::RealConstArrayView contrib = v.subConstView(0, size);
        for(Arcane::Integer k = 0; k < size; ++k) {
	  m_data.m_values_2d[m_data.m_equation][k] += contrib[k];
        }
	m_data.m_alien_builder(m_data.m_i, j) += Alien::view(m_data.m_values_2d);
      }
      {
	Alien::fill(m_data.m_values_2d, 0.);
	const Arcane::Integer size = m_data.m_values_2d.dim1Size();
        const Arcane::Integer j = m_data.m_all_indexes[m_stencil.front().cell().localId()][0]/size;
        const Arcane::RealConstArrayView contrib = v.subConstView(size, size);
        for(Arcane::Integer k = 0; k < size; ++k) {
	  m_data.m_values_2d[m_data.m_equation][k] += contrib[k];
        }
	m_data.m_alien_builder(m_data.m_i, j) += Alien::view(m_data.m_values_2d);
      }
    }
    void operator-=(const Law::Contribution& values)
    {
      const Arcane::RealConstArrayView v(values.size(),values.derivatives());
      {
	Alien::fill(m_data.m_values_2d, 0.);
	const Arcane::Integer size = m_data.m_values_2d.dim1Size();
	const Arcane::Integer j = m_data.m_all_indexes[m_stencil.back().cell().localId()][0]/size;
        const Arcane::RealConstArrayView contrib = v.subConstView(0, size);
        for(Arcane::Integer k = 0; k < size; ++k) {
	  m_data.m_values_2d[m_data.m_equation][k] -= contrib[k];
        }
	m_data.m_alien_builder(m_data.m_i, j) += Alien::view(m_data.m_values_2d);
      }
      {
	Alien::fill(m_data.m_values_2d, 0.);
	const Arcane::Integer size = m_data.m_values_2d.dim1Size();
	const Arcane::Integer j = m_data.m_all_indexes[m_stencil.front().cell().localId()][0]/size;
        const Arcane::RealConstArrayView contrib = v.subConstView(size, size);
        for(Arcane::Integer k = 0; k < size; ++k) {
	  m_data.m_values_2d[m_data.m_equation][k] -= contrib[k];
        }
	m_data.m_alien_builder(m_data.m_i, j) += Alien::view(m_data.m_values_2d);
      }
    }
    // TODO : To complete (and test) (XT)
    void operator=(const Law::Contribution& values)
    {
      const Arcane::RealConstArrayView v(values.size(),values.derivatives());// = values.derivatives();
      {
	const Arcane::Integer size = m_data.m_values_2d.dim1Size();
        const Arcane::Integer j = m_data.m_all_indexes[m_stencil.back().cell().localId()][0]/size;
        const Arcane::RealConstArrayView contrib = v.subConstView(0, size);
        for(Arcane::Integer k = 0; k < size; ++k) {
          //row[k] = contrib[k];
        }
      }
      {
	const Arcane::Integer size = m_data.m_values_2d.dim1Size();
        const Arcane::Integer j = m_data.m_all_indexes[m_stencil.front().cell().localId()][0]/size;
        const Arcane::RealConstArrayView contrib = v.subConstView(size, size);
        for(Arcane::Integer k = 0; k < size; ++k) {
          //row[k] = contrib[k];
        }
      }
    }
    InternalData& m_data;
    const ArcNum::TwoPointsStencil& m_stencil;
  };

  struct MatrixProxyCol
  {
    MatrixProxyCol(InternalData& data)
      : m_data(data) {}    
    MatrixAdder operator[](const Arcane::Item& j) 
    {
      m_data.m_j = m_data.m_all_indexes[j.localId()][0]/m_data.m_alien_block_matrix.block().size();
      return MatrixAdder(m_data);
    }
    MatrixAdder operator[](const Arcane::ItemEnumerator& j)
    {
      m_data.m_j = m_data.m_all_indexes[j->localId()][0]/m_data.m_alien_block_matrix.block().size();
      return MatrixAdder(m_data);
    }
    InternalData& m_data;
  };
  
  struct MatrixProxyColFV
  {
    MatrixProxyColFV(InternalData& data)
      : m_data(data) {}    
    MatrixAdderFV operator[](const ArcNum::TwoPointsStencil& j)
    {
      return MatrixAdderFV(m_data,j);
    }
    InternalData& m_data;
  };

  struct MatrixProxyRow
  {
    MatrixProxyRow(InternalData& data)
      : m_data(data) {}
    MatrixProxyCol operator[](const Arcane::Item& i) 
    {
      m_data.m_i = m_data.m_all_indexes[i.localId()][0]/m_data.m_alien_block_matrix.block().size();
      return MatrixProxyCol(m_data);
    }
    MatrixProxyCol operator[](const Arcane::ItemEnumerator& i)
    {
      m_data.m_i = m_data.m_all_indexes[i->localId()][0]/m_data.m_alien_block_matrix.block().size();
      return MatrixProxyCol(m_data);
    }
    MatrixProxyColFV operator[](const Law::Cell& i)
    {
      m_data.m_i = m_data.m_all_indexes[i.localId()][0]/m_data.m_alien_block_matrix.block().size();
      return MatrixProxyColFV(m_data);
    }
    InternalData& m_data;
  };
  
public:
  
  Matrix(Alien::BlockMatrix& alien_block_matrix,
	 Arcane::ConstArray2View<Arcane::Integer> indexes)
    : m_data(alien_block_matrix, indexes) {}
  
public:

MatrixProxyRow operator[](const Arcane::Integer equation) {
  m_data.m_equation = equation;
  return MatrixProxyRow(m_data); 
}

Arcane::Real& operator()(const Arcane::Integer i,
			 const Arcane::Cell k,
			 const Arcane::Integer j,
			 const Arcane::Cell l)
{
  return m_data.m_values_2d[0][0];
}

Arcane::Real& operator()(const Arcane::Integer i,
			 const Arcane::CellEnumerator k,
			 const Arcane::Integer j,
			 const Arcane::CellEnumerator l)
{
  return m_data.m_values_2d[0][0];
}

Arcane::ConstArray2View<Arcane::Integer> allIndexes() const
{
  return m_data.m_all_indexes;
}

const Alien::BlockMatrix& alienBlockMatrix() const
{
  return m_data.m_alien_block_matrix;
}

private:
 
InternalData m_data;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif /* ARCNUM_NUMERICS_SOLVER_MATRIX_H */