/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 * Copyright (C) 2006-2010 - Frictional Games
 *
 * This file is part of HPL1 Engine.
 *
 * HPL1 Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HPL1 Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HPL1 Engine.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HPL_MATERIAL_ENVMAP_REFLECT_H
#define HPL_MATERIAL_ENVMAP_REFLECT_H

#include "hpl1/engine/graphics/Material.h"

namespace hpl {

//----------------------------------------------------

class cEnvMapReflect_SetUp : public iMaterialProgramSetup {
public:
	void Setup(iGpuProgram *apProgram, cRenderSettings *apRenderSettings);
	void SetupMatrix(cMatrixf *apModelMatrix, cRenderSettings *apRenderSettings);
};

//----------------------------------------------------

class cGLState_EnvMapReflect : public iGLStateProgram {
public:
	cGLState_EnvMapReflect();

	void Bind();
	void UnBind();

private:
	void InitData() {}
};

//----------------------------------------------------

class cMaterial_EnvMap_Reflect : public iMaterial {
public:
	cMaterial_EnvMap_Reflect(const tString &asName, iLowLevelGraphics *apLowLevelGraphics,
							 cImageManager *apImageManager, cTextureManager *apTextureManager,
							 cRenderer2D *apRenderer, cGpuProgramManager *apProgramManager,
							 eMaterialPicture aPicture, cRenderer3D *apRenderer3D);

	virtual ~cMaterial_EnvMap_Reflect();

	tTextureTypeList GetTextureTypes();

	bool UsesType(eMaterialRenderType aType);

	iGpuProgram *GetVertexProgram(eMaterialRenderType aType, int alPass, iLight3D *apLight);
	bool VertexProgramUsesLight(eMaterialRenderType aType, int alPass, iLight3D *apLight);
	bool VertexProgramUsesEye(eMaterialRenderType aType, int alPass, iLight3D *apLight);

	iMaterialProgramSetup *GetVertexProgramSetup(eMaterialRenderType aType, int alPass, iLight3D *apLight);

	iGpuProgram *GetFragmentProgram(eMaterialRenderType aType, int alPass, iLight3D *apLight);

	eMaterialAlphaMode GetAlphaMode(eMaterialRenderType aType, int alPass, iLight3D *apLight);
	eMaterialBlendMode GetBlendMode(eMaterialRenderType aType, int alPass, iLight3D *apLight);
	eMaterialChannelMode GetChannelMode(eMaterialRenderType aType, int alPass, iLight3D *apLight);

	iTexture *GetTexture(int alUnit, eMaterialRenderType aType, int alPass, iLight3D *apLight);
	eMaterialBlendMode GetTextureBlend(int alUnit, eMaterialRenderType aType, int alPass, iLight3D *apLight);

	int GetNumOfPasses(eMaterialRenderType aType, iLight3D *apLight) { return 1; }

	//////////////////////////////////////////////////////////////////
	// Old and worthless stuff, only used by 2D renderer
	void Compile() {}
	bool StartRendering(eMaterialRenderType aType, iCamera *apCam, iLight *pLight) { return false; }
	void EndRendering(eMaterialRenderType aType) {}
	tVtxBatchFlag GetBatchFlags(eMaterialRenderType aType) { return 0; }
	bool NextPass(eMaterialRenderType aType) { return false; }
	bool HasMultiplePasses(eMaterialRenderType aType) { return false; }
	eMaterialType GetType(eMaterialRenderType aType) { return eMaterialType_Diffuse; }
	void EditVertexes(eMaterialRenderType aType, iCamera *apCam, iLight *pLight,
					  tVertexVec *apVtxVec, cVector3f *apTransform, unsigned int alIndexAdd) {}
};

class cMaterialType_EnvMap_Reflect : public iMaterialType {
public:
	bool IsCorrect(tString asName) {
		return cString::ToLowerCase(asName) == "environmentmapreflect";
	}

	iMaterial *Create(const tString &asName, iLowLevelGraphics *apLowLevelGraphics,
					  cImageManager *apImageManager, cTextureManager *apTextureManager,
					  cRenderer2D *apRenderer, cGpuProgramManager *apProgramManager,
					  eMaterialPicture aPicture, cRenderer3D *apRenderer3D) {
		return hplNew(cMaterial_EnvMap_Reflect, (asName, apLowLevelGraphics,
												 apImageManager, apTextureManager, apRenderer,
												 apProgramManager, aPicture, apRenderer3D));
	}
};

};     // namespace hpl
#endif // HPL_MATERIAL_ENVMAP_REFLECT_H
