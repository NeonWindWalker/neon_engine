#include "classic.h"

namespace DrawerLib
{

void ClassicDrawer::draw(DrawTask& task, boolean interfaceSubViewport)
{
	if(interfaceSubViewport)
	{
		if(task.viewportSize.x == 0 || task.viewportSize.y == 0)
			return;

		context->viewport(task.viewportPosition.x, task.viewportPosition.y, task.viewportSize.x, task.viewportSize.y);
		context->clearBuffers(true, task.useClearColor ? &task.clearColor : 0);
	}
	else
	{
		if(!task.renderTarget)
			return;

		if(task.renderTarget->type() == EResourceType_FrameWindow)
		{
			FrameWindow* target = static_cast<FrameWindow*>(task.renderTarget);
			context->setRenderTargetWindowMode(*target);
		}
		else if(task.renderTarget->type() == EResourceType_FrameTexture)
		{
			FrameTexture* target = static_cast<FrameTexture*>(task.renderTarget);
			context->setRenderTargetCompositMode();
			context->setRenderTargetColor(0, *target);
		}
		context->viewportFull();
		context->clearBuffers(true, task.useClearColor ? &task.clearColor : 0);
		context->begin();
	}

	task.debugInfo.clear();
	

	float viewPassDepthCoef = task.view.far / task.view.near;
	Matrix34 toEyeMatrix;
	Matrix34 invViewPlace = task.view.place.transpose();
	Vector2_t forwardDirection = -task.view.place.z;
	if(task.view.ortho)
	{
		toEyeMatrix.x.setZero();
		toEyeMatrix.y.setZero();
		toEyeMatrix.z.setZero();
		toEyeMatrix.p = task.view.place.z;
	}
	else
	{
		toEyeMatrix.x.set(-1.0f, 0.0f, 0.0f);
		toEyeMatrix.y.set(0.0f, -1.0f, 0.0f);
		toEyeMatrix.z.set(0.0f, 0.0f, -1.0f);
		toEyeMatrix.p = task.view.place.p;
	}
	
	float near = task.view.near;
	float depthRangeStart = 0.0f;
	foreach(pass, task.depthPasses)
	{
		Matrix44 projectionMatrix;
		float depthRangeEnd = depthRangeStart + 1.0f / (float)task.depthPasses.size();
		context->depthRange(depthRangeStart, depthRangeEnd);
		float far = task.view.far;

		if(task.view.ortho)
		{
			buildOrthoProjectionMatrix(projectionMatrix, task.view.orthoHalfWidth, task.view.orthoHalfHeight, near, far);
		}
		else
		{
			if(pass == task.depthPasses.last())
			{
				far = 1e32f;
				buildPerspectiveProjectionMatrix(projectionMatrix, task.view.perspectiveTanHalfWidth, task.view.perspectiveTanHalfHeight, near); 
			}
			else
				buildPerspectiveProjectionMatrix(projectionMatrix, task.view.perspectiveTanHalfWidth, task.view.perspectiveTanHalfHeight, near, far);
		}
		
		Matrix44 viewAndProj = projectionMatrix * Matrix44::makeProjectiveMatrix(invViewPlace);

		context->depthLessEqual();
		context->blendNo();

		foreach(baseBatchIt, pass->solidBatchesSet)
			drawBatch(**baseBatchIt, viewAndProj, toEyeMatrix, task.sensitivity, false);

		foreach(baseBatchIt, pass->solidAlphatestedBatchesSet)
			drawBatch(**baseBatchIt, viewAndProj, toEyeMatrix, task.sensitivity, true);

		context->blendAdd();

		foreach(baseBatchIt, pass->solidComplanarAlphatestedBatchesSet)
			drawBatch(**baseBatchIt, viewAndProj, toEyeMatrix, task.sensitivity, true);

		foreach(baseBatchIt, pass->solidComplanarBatchesSet)
			drawBatch(**baseBatchIt, viewAndProj, toEyeMatrix, task.sensitivity, false);

		context->colorNo();

		foreach(baseBatchIt, pass->postSolidDepthOnlyBatchesSet)
			drawBatch(**baseBatchIt, viewAndProj, toEyeMatrix, task.sensitivity, false);

		context->depthLessEqual(false);
		context->blendXAlpha();
		foreach(baseBatchIt, pass->blendBatchesSet)
			drawBatch(**baseBatchIt, viewAndProj, toEyeMatrix, task.sensitivity, false);

		//simple primitives
		context->depthLessEqual();
		context->blendXAlpha();

		context->program(*simplePrimitivesProg);
		context->parameter(simplePrimitivesProg_nFullTransformMatrix_M44, viewAndProj);
		context->textureDisableFrom(0);

		foreach(it, task.primitiveBatches)
		{
			BatchBase* baseBatch = *it;
			switch(baseBatch->type)
			{
				case EBatchType_Points:
				{
					PointsBatch& batch = *static_cast<PointsBatch*>(baseBatch);
					context->drawSimplePoints(batch.buffer, batch.bufferOffset, batch.pointsCount);
					break;
				}
				case EBatchType_Lines:
				{
					LinesBatch& batch = *static_cast<LinesBatch*>(baseBatch);
					context->drawSimpleLines(batch.buffer, batch.bufferOffset, batch.linesCount);
					break;
				}
				case EBatchType_Triangles:
				{
					TrianglesBatch& batch = *static_cast<TrianglesBatch*>(baseBatch);
					context->drawSimpleTriangles(batch.buffer, batch.bufferOffset, batch.trianglesCount);
					break;
				}
			}
		}

		near = pass->far;
		depthRangeStart = depthRangeEnd;
	}
	
	// interface
	context->depthNo();
	context->blendXAlpha();
	
	Matrix23 interfaceProjMatrix;
	Vector2i viewportSize = context->viewportSize();	
	buildPixelToPixelMatrix(interfaceProjMatrix, viewportSize.x, viewportSize.y, (float)task.interfaceScreenSize.x, (float)task.interfaceScreenSize.y);
	
	foreach(it, task.interfaceBatches)
	{
		BatchBase* baseBatch = *it;
		switch(baseBatch->type)
		{
			case EBatchType_InterfaceQuads:
			{
				InterfaceQuadsBatch& batch = *static_cast<InterfaceQuadsBatch*>(baseBatch);
				TextureBase* tex = toBaseTex(batch.texture);
				TextureBase* alphaTex = toBaseTex(batch.alphaTexture);
				if(!tex)
					break;

				if(alphaTex)
				{
					context->program(*interfaceQuadsProgA);
					context->parameter(interfaceQuadsProgA_nFullTransformMatrix_M23, interfaceProjMatrix);
					context->texture(interfaceQuadsProgA_Tex, *tex, ETextureSetMode_DefaultGUI);
					context->texture(interfaceQuadsProgA_AlphaTex, *alphaTex, ETextureSetMode_DefaultGUI);
				}
				else
				{
					context->program(*interfaceQuadsProg);
					context->parameter(interfaceQuadsProg_nFullTransformMatrix_M23, interfaceProjMatrix);
					context->texture(interfaceQuadsProg_Tex, *tex, ETextureSetMode_DefaultGUI);
				}
	
				context->drawQuads(batch.buffer, EVertexFormat_InterfaceQuad, batch.bufferOffset, batch.quadsCount);
				break;
			}
			case EBatchType_InterfaceFontGlyphs:
				{
					InterfaceFontGlyphsBatch& batch = *static_cast<InterfaceFontGlyphsBatch*>(baseBatch);
					TextureBase* tex = toBaseTex(batch.texture);
					if(!tex || batch.mode >= EFontGlyphsMode_EnumCount)
						break;

					context->program(*interfaceFontProg[batch.mode]);
					context->parameter(interfaceFontProg_nFullTransformMatrix_M23[batch.mode], interfaceProjMatrix);
					if(batch.mode == EFontGlyphsMode_BitCompressedMultisampled){

						context->parameter(interfaceFontProg_DTexCrd, Vector2(0.25f / tex->width(), 0.25f / tex->height()));
					}

					context->texture(interfaceFontProg_Tex[batch.mode], *tex, batch.mode != EFontGlyphsMode_SingleChannel ? ETextureSetMode_GUIHardFont : ETextureSetMode_GUIFont);
					context->textureDisableFrom(1);
					context->drawQuads(batch.buffer, EVertexFormat_InterfaceFont, batch.bufferOffset, batch.quadsCount);
					break;
				}
			case EBatchType_DrawTask:
			{
				DrawTask& otherTask = *static_cast<DrawTask*>(baseBatch);
				Vector2i p = context->viewportPosition();
				Vector2i s = context->viewportSize();

				draw(otherTask, true);

				context->viewport(p.x, p.y, s.x, s.y);
				context->depthNo();
				context->blendAlpha();
				context->program(*interfaceQuadsProg);
				context->parameter(interfaceQuadsProg_nFullTransformMatrix_M23, interfaceProjMatrix);
				break;
			}
		}
	}

	if(task.colorGetter)
		static_cast<AsyncReader*>(task.colorGetter)->push(*context, task.time);

	if(!interfaceSubViewport)
		context->end();

	task.debugInfo.shaderChangesCount = context->shaderChangesCount;
	task.debugInfo.textureChangesCount = context->textureChangesCount;
	task.debugInfo.drawsCount = context->drawsCount;
	task.debugInfo.trianglesCount = context->trianglesCount;
}

void ClassicDrawer::setLightProbeParameters(const VertexShaderAndParameters& prog, const LightProbeBase& lp)
{
	switch(lp.type)
	{
	case ELightProbeType_Single:
		{
			const LightProbeSingle* probe = static_cast<const LightProbeSingle*>(&lp);
			Vector4 p[3];
			float energy = max(abs(probe->lightIntensivity.x), max(abs(probe->lightIntensivity.y), abs(probe->lightIntensivity.z)));
			float conversionScale = 5.0f;
			p[0] = Vector4(probe->toLightDirection * conversionScale, energy * conversionScale * conversionScale);
			p[1] = Vector4(probe->lightIntensivity, energy);
			p[2] = Vector4(probe->ambientIntensivity, 0.0f);
			context->parameterArray(prog.LightProbeSingle_probe, p, 3);
			context->parameter(prog.ToLightDirection, probe->toLightDirection);
			context->parameter(prog.LightIntensivity, probe->lightIntensivity);
			context->parameter(prog.AmbientIntensivity, probe->ambientIntensivity);
		}
		break;
	case ELightProbeType_Octo:
		{
			const LightProbeOcto* probe = static_cast<const LightProbeOcto*>(&lp);
			Vector4 ma[2];
			ma[0] = Vector4( probe->invSize, 0.0 );
			ma[1] = Vector4( - probe->zeroPoint * probe->invSize, 0.0);
			context->parameterArray(prog.LightProbeOcto_coordMAdd, ma, 2);
			context->parameterArray(prog.LightProbeOcto_probes, &probe->probe[0][0][0].toLightVectorXIntensivityAndIntensivity, 24);
		}
		break;
	case ELightProbeType_Star7:
		{
			const LightProbeStar7* probe = static_cast<const LightProbeStar7*>(&lp);
			Vector4 ma[2];
			ma[0] = Vector4( probe->invScale, 0.0);
			ma[1] = Vector4( - probe->center * probe->invScale, 0.0);
			context->parameterArray(prog.LightProbeStar7_coordMAdd, ma, 2);
			context->parameterArray(prog.LightProbeStar7_probes, &probe->c.toLightVectorXIntensivityAndIntensivity, 21);
		}
		break;
	}
}


void ClassicDrawer::drawBatch(BatchBase& baseBatch, const Matrix44& viewAndProj, const Matrix34& toEyeMatrix, float sensitivity, boolean alphaTest)
{
	switch(baseBatch.type)
	{
		case EBatchType_Mesh:
		{
			MeshBatch& batch = *static_cast<MeshBatch*>(&baseBatch);
			if(!batch.lightProbeBase)
				return;

			TextureBase* pTex0 = toBaseTex(batch.texture[0][0]);
			TextureBase* pTex1 = toBaseTex(batch.texture[0][1]);
			TextureBase* pTex2 = toBaseTex(batch.texture[0][2]);
			if(!pTex0 || !pTex1)
				return;

			TextureBase* pTex02 = toBaseTex(batch.texture[1][0]);
			TextureBase* pTex12 = toBaseTex(batch.texture[1][1]);
			TextureBase* pTex22 = toBaseTex(batch.texture[1][2]);
			TextureBase* pEnvTex = toBaseTex(batch.enviromentTexture);
			TextureBase* pCausticTex = batch.waterPlane ? toBaseTex(batch.waterPlane->caustic) : 0;

			MeshShaderProgramIndex params;
			params.vs.format = batch.geometryList.first()->vbFormat;
			params.vs.lightProbe = batch.lightProbeBase->type;
			params.vs.twoSided = boolean01FromBoolean(batch.twosided);
			params.vs.caustic = params.ps.caustic = pCausticTex != 0;			
			
			params.ps.dualLayer = pTex02 && pTex12;
			params.ps.envCubeTex = pEnvTex != 0;
			params.ps.metal =  boolean01FromBoolean(batch.metal);
			params.ps.alphaTest = boolean01FromBoolean(alphaTest);
			params.ps.specular = params.vs.specular = !batch.disableSpecular;
			params.ps.brutalFast = params.vs.brutalFast = batch.brutalFastMode;
			params.ps.hueMatrix = batch.albedoHueMatrix != 0;
			params.ps.saturation = batch.albedoSaturation != 0;
			params.ps.tripleTexuresMode = pTex2 != 0;
			
			const MeshShaderProgramAndParameters& prog = getMeshProgram(params);
			if(!prog.program)
				return;
			context->program(*prog.program);

			Matrix44 fullTransformMatrix = viewAndProj * Matrix44::makeProjectiveMatrix(batch.matrix);
			context->parameter(prog.FullTransformMatrix, fullTransformMatrix);
			context->parameter(prog.TexCoordMatrix, batch.texCoordMatrix);
			context->parameter(prog.ToEyeVectorMatrix, toEyeMatrix * batch.matrix );
			context->parameter(prog.TangentBinormalMatrix, batch.matrix.m33() );
			context->parameter(prog.NormalMatrix, Matrix33(batch.matrix.x / batch.matrix.x.lenghtSquare(), batch.matrix.y / batch.matrix.y.lenghtSquare(), batch.matrix.z / batch.matrix.z.lenghtSquare()) );
			context->parameter(prog.NoLightBackFacesMultCoef, 1.0f - batch.lightBackFacesTreshold);
			context->parameter(prog.NoLightBackFacesAddCoef, batch.lightBackFacesTreshold);
			setLightProbeParameters(prog, *batch.lightProbeBase);

			if(batch.localBoneMatrixArray)
				context->parameterArray(prog.BoneArray, &batch.localBoneMatrixArray->row[0], batch.localBoneCount * 3);
			if(batch.lightProbes.size())
				context->parameterArray(prog.BoneArray, &batch.lightProbes.first()->toLightVectorXIntensivityAndIntensivity, batch.lightProbes.size() * 2);

			Vector4 opacityCoefs[4];
			opacityCoefs[0].setZero();
			opacityCoefs[1].setZero();
			opacityCoefs[2].setZero();
			opacityCoefs[3].setZero();
			Assert(batch.layerIndex[0] < 8);
			Assert(batch.layerIndex[1] < 8);
			opacityCoefs[0].a[batch.layerIndex[0]] = batch.opacity[0];
			opacityCoefs[2].a[batch.layerIndex[1]] = batch.opacity[1];
			context->parameterArray(prog.OpacityCoefs, opacityCoefs, 4);
			context->parameter(prog.Opacity0Coef, batch.opacity[0]);
			context->parameter(prog.Opacity1Coef, batch.opacity[1]);
			context->parameter(prog.IntensivityMultiplyer, sensitivity);

			if(batch.waterPlane)
			{
				context->parameter(prog.CausticTexCoordMatrix, batch.matrix * batch.waterPlane->causticTexCoordMatrix);
				context->parameter(prog.CausticPlane, batch.waterPlane->plane / batch.waterPlane->fullBeginDistance);
				context->parameter(prog.CausticLayer7Scale, batch.waterPlane->heightmapLayerParam7Scale * 2.0f / batch.waterPlane->fullBeginDistance);
				context->parameter(prog.CausticScale, batch.waterPlane->causticScale);
				context->parameter(prog.CausticFadeBack, batch.waterPlane->fullEndDistance / batch.waterPlane->fullBeginDistance);
			}

			//ps
			context->parameter(prog.EnvColor, batch.enviromentColor);
			if(batch.albedoHueMatrix)
				context->parameter(prog.HueMatrix,  *batch.albedoHueMatrix);
			if(batch.albedoSaturation)
			{
				Vector4 SaturationCoefs[2];
				SaturationCoefs[0].set(batch.albedoSaturation->scale, 0.0f);
				SaturationCoefs[1].set(batch.albedoSaturation->componentSummScale, 0.0f);
				context->parameterArray(prog.SaturationCoefs, SaturationCoefs, 2);
			}

			context->texture(prog.Tex0, *pTex0);
			context->texture(prog.Tex1, *pTex1);
			if(pTex2)
				context->texture(prog.Tex2, *pTex2);
			if(pCausticTex)
				context->texture(prog.CausticTex, *pCausticTex);

			if(prog.SpecularFuncTex.deviceIndex >= 0)
				context->texture(prog.SpecularFuncTex, *getSpecularTexture(), ETextureSetMode_ClampTablic);

			if(pTex02 && pTex12)
			{
				context->texture(prog.Tex02, *pTex02);
				context->texture(prog.Tex12, *pTex12);
			}
			if(pTex22)
				context->texture(prog.Tex22, *pTex22);

			if(pEnvTex)
				context->texture(prog.EnvTex, *pEnvTex);

			foreach(it, batch.geometryList)
				context->draw(it->vb, it->vbFormat, it->vbOffset, it->vbSize, it->ib, it->ibFirst, it->trisCount );

			break;
		}

		case EBatchType_WaterPlane:
		{
			WaterPlaneBatch& batch = *static_cast<WaterPlaneBatch*>(&baseBatch);
			if(!batch.lightProbeBase || !batch.layer)
				return;

			TextureBase* pTex = toBaseTex(batch.layer);
			TextureBase* pEnvTex = toBaseTex(batch.enviromentTexture);
			TextureBase* pFoamTex = toBaseTex(batch.foamTexture);
			if(!pTex || !pEnvTex)
				return;

			WaterPlaneShaderProgramIndex params;
			params.vs.format = batch.geometryList.first()->vbFormat;
			params.vs.lightProbe = batch.lightProbeBase->type;
			params.vs.waterReflection = 1;
			params.ps.specular = batch.glossines && !batch.disableSpecular;
			params.ps.brutalFast = params.vs.brutalFast = batch.brutalFastMode;
			params.ps.foam = pFoamTex != 0;

			const WaterPlaneShaderProgramAndParameters& prog = getWaterPlaneProgram(params);
			if(!prog.program)
				return;
			context->program(*prog.program);

			Matrix44 fullTransformMatrix = viewAndProj * Matrix44::makeProjectiveMatrix(batch.matrix);
			context->parameter(prog.FullTransformMatrix, fullTransformMatrix);
			context->parameter(prog.TexCoordMatrix, batch.texCoordMatrix);
			context->parameter(prog.ToEyeVectorMatrix, toEyeMatrix * batch.matrix );
			context->parameter(prog.TangentBinormalMatrix, batch.matrix.m33() );
			context->parameter(prog.NormalMatrix, Matrix33(batch.matrix.x / batch.matrix.x.lenghtSquare(), batch.matrix.y / batch.matrix.y.lenghtSquare(), batch.matrix.z / batch.matrix.z.lenghtSquare()) );
			context->parameter(prog.NoLightBackFacesMultCoef, 1.0f - batch.lightBackFacesTreshold );
			context->parameter(prog.NoLightBackFacesAddCoef, batch.lightBackFacesTreshold);
			context->parameter(prog.ReflectionPalneCoordOffsetAndScale, Vector2_t(batch.envCoordShift, batch.envCoordScale));

			setLightProbeParameters(prog, *batch.lightProbeBase);

			Vector4 opacityCoefs[4];
			opacityCoefs[0].set(batch.opacityScale, 0.0f, 0.0f, 0.0f);
			opacityCoefs[1].set(0.0f, 0.0f, 0.0f, 0.0f);
			opacityCoefs[2].set(0.0f, 1.0f, 0.0f, 0.0f);
			opacityCoefs[3].set(0.0f, 0.0f, 0.0f, 0.0f);
			context->parameterArray(prog.OpacityCoefs, opacityCoefs, 4);
			context->parameter(prog.IntensivityMultiplyer, sensitivity);

			//ps
			context->parameter(prog.FrenselBase, batch.frensel);
			if(batch.brutalFastMode)
			{
				context->parameter(prog.EnvIntensivity, batch.enviromentColor * batch.specularMask * sqrt(batch.frensel));
				context->parameter(prog.SpecularIntensivity, Vector2_t::getOne() * batch.specularMask);
			}
			else
			{
				context->parameter(prog.EnvIntensivity, batch.enviromentColor * batch.enviromentColor * batch.specularMask * batch.specularMask);
				context->parameter(prog.SpecularIntensivity, Vector2_t::getOne() * batch.specularMask * batch.specularMask);
			}
			context->parameter(prog.NormalScale, batch.normalMapScale);
			context->parameter(prog.RoughtnessSqr, exp2(-9.0f * batch.glossines));
			context->parameter(prog.Glossines, batch.glossines);
			context->parameter(prog.FoamTexScale, batch.foamTexCoordScale);
			context->parameter(prog.FoamBumpScale, batch.foamBumpScale);

			context->texture(prog.Tex, *pTex);
			context->texture(prog.EnvTex, *pEnvTex);
			if(pFoamTex)
				context->texture(prog.FoamTex, *pFoamTex);

			if(prog.SpecularFuncTex.deviceIndex >= 0)
				context->texture(prog.SpecularFuncTex, *getSpecularTexture(), ETextureSetMode_ClampTablic);

			foreach(it, batch.geometryList)
				context->draw(it->vb, it->vbFormat, it->vbOffset, it->vbSize, it->ib, it->ibFirst, it->trisCount);

			break;
		}

		case EBatchType_Particles:
			{
				ParticlesBatch& batch = *static_cast<ParticlesBatch*>(&baseBatch);
				TextureBase* pTex = toBaseTex(batch.texture);
				if(!pTex || !batch.quadsCount)
					return;

				if(batch.dualMode)
				{
					context->program(*particlesProg2);
					context->parameter(particlesProg2_nFullTransformMatrix_M44, viewAndProj);
					context->parameter(particlesProg2_ColorScale, sensitivity);
					context->texture(particlesProg2_Tex, *pTex);
				}
				else
				{
					context->program(*particlesProg1);
					context->parameter(particlesProg1_nFullTransformMatrix_M44, viewAndProj);
					context->parameter(particlesProg1_ColorScale, sensitivity);
					context->texture(particlesProg1_Tex, *pTex);
				}
				context->drawQuads(batch.vertxBuffer, batch.vertxBufferFormat, batch.vertxBufferOffset, batch.quadsCount);
				break;
			}
		case EBatchType_DepthOnly:
			{
				DepthOnlyMeshBatch& batch = *static_cast<DepthOnlyMeshBatch*>(&baseBatch);
				TextureBase* pTex = toBaseTex(batch.alphaTestTexture);
				
				DepthOnlyMeshShaderProgramIndex params;
				params.vs.format = batch.geometryList.first()->vbFormat;
				params.ps.alphaTest = pTex != 0;

				const DepthOnlyMeshShaderProgramAndParameters& prog = getDepthOnlyMeshProgram(params);
				if(!prog.program)
					return;
				context->program(*prog.program);

				Matrix44 fullTransformMatrix = viewAndProj * Matrix44::makeProjectiveMatrix(batch.matrix);
				context->parameter(prog.FullTransformMatrix, fullTransformMatrix);
				context->parameter(prog.TexCoordMatrix, batch.texCoordMatrix);

				if(batch.localBoneMatrixArray)
					context->parameterArray(prog.BoneArray, &batch.localBoneMatrixArray->row[0], batch.localBoneCount * 3);
				
				if(pTex)
					context->texture(prog.Tex, *pTex);
			
				foreach(it, batch.geometryList)
					context->draw(it->vb, it->vbFormat, it->vbOffset, it->vbSize, it->ib, it->ibFirst, it->trisCount );

				break;
			}
        default:
            Assert(false);
	}
}

}
 
