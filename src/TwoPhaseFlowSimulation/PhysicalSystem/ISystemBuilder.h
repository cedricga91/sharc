// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
#ifndef DATAMODEL_ISYSTEMBUILDER_H
#define DATAMODEL_ISYSTEMBUILDER_H
/* Author : desrozis at Thu Jan 12 15:46:17 2012
 * Generated by createNew
 */

//#include "ArcRes/Entities/System.h"
#include "ArcRes/Entities/System.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


class ISystemBuilder
{
public:
  
  virtual ~ISystemBuilder() {}
  
public:
  
  virtual ArcRes::System buildSystem() = 0;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif /* DATAMODEL_ISYSTEMBUILDER_H */
