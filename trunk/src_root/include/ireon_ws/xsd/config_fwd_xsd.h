// Copyright (C) 2005-2006 Code Synthesis Tools CC
//
// This program was generated by XML Schema Definition Compiler (XSD)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis Tools CC gives
// permission to link this program with the Xerces-C++ library (or with
// modified versions of Xerces-C++ that use the same license as Xerces-C++),
// and distribute linked combinations including the two. You must obey
// the GNU General Public License version 2 in all respects for all of
// the code used other than Xerces-C++. If you modify this copy of the
// program, you may extend this exception to your version of the program,
// but you are not obligated to do so. If you do not wish to do so, delete
// this exception statement from your version.
//
// In addition, Code Synthesis Tools CC makes a special exception for
// the Free/Libre and Open Source Software (FLOSS) which is described
// in the accompanying FLOSSE file.
//

#ifndef CONFIG_FWD_XSD_H
#define CONFIG_FWD_XSD_H

#include <xsd/cxx/version.hxx>

#if (XSD_INT_VERSION != 2030000L)
#error XSD runtime version mismatch
#endif

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/pre.hxx>

#include "common/xsd/common_xsd.h"

// Forward declarations.
//
namespace ireon
{
  namespace ws
  {
    class CWSConfig;
    class CPlayerListener;
    class CRSConnection;
    class CDBconnection;
    class CResources;
    class CDefaultPosition;
    class CIPv4Address;
    class CPort;
    class CConnectionsNumber;
  }
}


#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

#endif // CONFIG_FWD_XSD_H
