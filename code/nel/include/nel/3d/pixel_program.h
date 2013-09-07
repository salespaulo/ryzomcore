/** \file pixel_program.h
 * Pixel program definition
 */

/* Copyright, 2000, 2001 Nevrax Ltd.
 *
 * This file is part of NEVRAX NEL.
 * NEVRAX NEL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * NEVRAX NEL is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with NEVRAX NEL; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#ifndef NL_PIXEL_PROGRAM_H
#define NL_PIXEL_PROGRAM_H

#include <nel/misc/types_nl.h>
#include <nel/misc/smart_ptr.h>
#include <nel/3d/gpu_program.h>
#include <nel/3d/gpu_program_source.h>

#include <list>

namespace NL3D {

/**
 * \brief CPixelProgramInfo
 * \date 2013-09-07 15:00GMT
 * \author Jan Boon (Kaetemi)
 * Read-only information structure.
 */
struct CPixelProgramInfo
{
public:
	std::string DisplayName;
	
	enum TFeatures
	{
		/// Use texture stages from CMaterial as texture parameters
		MaterialTextures = 0x0001, 
		/// Set driver fog parameters on this program
		Fog = 0x0002, 
		/// Adds an enabled/disabled parameter to the fog, for user shaders
		DynamicFog = 0x0004, 
	};

	// Bitfield containing features used by this pixel program
	uint Features;

	// Indices of parameters used by features
	uint FogEnabledIdx; // (Fog && DynamicFog) nlFogEnabled, fog enabled
	uint FogStartEndIdx; // (Fog) nlFogStartEnd, start and end of fog
	uint FogColorIdx; // (Fog) nlFogColor, fog color
};

class CPixelProgram : public IGPUProgram
{
public:
	/// Constructor
	CPixelProgram(CGPUProgramSourceCont *programSource);
	/// Destructor
	virtual ~CPixelProgram ();

	/// Build feature information
	void buildInfo(const char *displayName, uint features);
	/// Get feature information
	inline const CPixelProgramInfo *getInfo() const { return _Info; }

private:

	/// Feature information
	CPixelProgramInfo							*_Info;
};

} // NL3D


#endif // NL_PIXEL_PROGRAM_H

/* End of vertex_program.h */
