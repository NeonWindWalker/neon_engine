#include "right.h"

namespace DrawerLib
{

void RightDrawer::draw(DrawTask& task)
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
	
	context->clearBuffers(true, task.useClearColor ? &task.clearColor : 0);
	context->begin();
	

	float viewPassDepthCoef = task.view.far / task.view.near;
	Matrix34 toEyeMatrix;
	Matrix34 invViewPlace = task.view.place.transpose();
	Vector3 forwardDirection = -task.view.place.z;
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
		
		Matrix44 viewAndProj;
		mult(viewAndProj, projectionMatrix, Matrix44(invViewPlace, Vector4(0,0,0,1)));

		context->depthLessEqual();
		context->blendNo();

		foreach(baseBatchIt, pass->solidBatchesSet)
			drawBatch(**baseBatchIt, viewAndProj, toEyeMatrix, task.sensitivity);

		foreach(baseBatchIt, pass->solidAlphatestedBatchesSet)
			drawBatch(**baseBatchIt, viewAndProj, toEyeMatrix, task.sensitivity, true);

		context->blendAdd();

		foreach(baseBatchIt, pass->solidComplanarAlphatestedBatchesSet)
			drawBatch(**baseBatchIt, viewAndProj, toEyeMatrix, task.sensitivity, true);

		foreach(baseBatchIt, pass->solidComplanarBatchesSet)
			drawBatch(**baseBatchIt, viewAndProj, toEyeMatrix, task.sensitivity);

		context->blendXAlpha();
		foreach(baseBatchIt, pass->blendBatchesSet)
			drawBatch(**baseBatchIt, viewAndProj, toEyeMatrix, task.sensitivity);

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
	context->blendAlpha();
	
	Matrix23 interfaceProjMatrix;
	buildPixelToPixelMatrix(interfaceProjMatrix, task.renderTarget->size().x, task.renderTarget->size().y, (float)task.interfaceScreenSize.x, (float)task.interfaceScreenSize.y);
	context->program(*interfaceQuadsProg);
	context->parameter(interfaceQuadsProg_nFullTransformMatrix_M23, interfaceProjMatrix);
	
	foreach(it, task.interfaceBatches)
	{
		BatchBase* baseBatch = *it;
		switch(baseBatch->type)
		{
			case EBatchType_InterfaceQuads:
			{
				InterfaceQuadsBatch& batch = *static_cast<InterfaceQuadsBatch*>(baseBatch);
				TextureBase* tex = toBaseTex(batch.texture);
				if(!tex)
					break;
				
				context->texture(interfaceQuadsProg_Tex, *tex);
				context->textureDisableFrom(1);
				context->drawQuads(batch.buffer, EVertexFormat_InterfaceQuad, batch.bufferOffset, batch.quadsCount);
				break;
			}	
		}
	}

	context->end();
}

void RightDrawer::setLightProbeParameters(const VertexShaderAndParameters& prog, const LightProbeBase& lp)
{
	switch(lp.type)
	{
	case ELightProbeType_Single:
		{
			const LightProbeSingle* probe = static_cast<const LightProbeSingle*>(&lp);
			Vector4 p[3];
			float energy = max(abs(probe->lightColor.x), abs(probe->lightColor.y), abs(probe->lightColor.z));
			float conversionScale = 5.0f;
			p[0] = Vector4(probe->toLightDirection * conversionScale, energy * conversionScale * conversionScale);
			p[1] = Vector4(probe->lightColor, energy);
			p[2] = Vector4(probe->ambientColor, 0.0f);
			context->parameterArray(prog.LightProbeSingle_probe, p, 3);
			context->parameter(prog.ToLightDirection, probe->toLightDirection);
			context->parameter(prog.LightColor, probe->lightColor);
			context->parameter(prog.AmbientColor, probe->ambientColor);
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


void RightDrawer::drawBatch(BatchBase& baseBatch, const Matrix44& viewAndProj, const Matrix34& toEyeMatrix, float sensitivity, boolean alphaTest)
{
	switch(baseBatch.type)
	{
		case EBatchType_Mesh:
		{
			MeshBatch& batch = *static_cast<MeshBatch*>(&baseBatch);
			if(!batch.lightProbeBase || !batch.material[0])
				return;

			TextureBase* pTex0 = toBaseTex(batch.material[0]->texture[0]);
			TextureBase* pTex1 = toBaseTex(batch.material[0]->texture[1]);
			if(!pTex0 || !pTex1)
				return;

			TextureBase* pTex02 = 0;
			TextureBase* pTex12 = 0;
			if(batch.material[1]){
				pTex02 = toBaseTex(batch.material[1]->texture[0]);
				pTex12 = toBaseTex(batch.material[1]->texture[1]);
			}

			TextureBase* pEnvTex = toBaseTex(batch.enviromentTexture);

			MeshShaderProgramIndex params;
			params.vs.format = batch.geometryList.first()->vbFormat;
			params.vs.lightProbe = batch.lightProbeBase->type;
			params.vs.twoSided = boolean01FromBoolean(batch.material[0]->twosided);

			params.ps.singleLightProbe = isSingleLightProbe(batch.lightProbeBase->type, batch.geometryList.first()->vbFormat);
			params.ps.dualLayer = pTex02 && pTex12;
			params.ps.opacity = boolean01FromBoolean(batch.material[0]->opacity | alphaTest);
			params.ps.backlight = boolean01FromBoolean(batch.material[0]->backlight);
			params.ps.anisotropic = boolean01FromBoolean(batch.material[0]->anisotropic);
			params.ps.envCubeTex = pEnvTex != 0;
			params.ps.alphaTest = boolean01FromBoolean(alphaTest);

			const MeshShaderProgramAndParameters& prog = getMeshProgram(params);
			if(!prog.program)
				return;
			context->program(*prog.program);

			Matrix44 fullTransformMatrix;
			mult(fullTransformMatrix, viewAndProj, Matrix44(batch.matrix, Vector4(0,0,0,1)));
			context->parameter(prog.FullTransformMatrix, fullTransformMatrix);
			context->parameter(prog.TexCoordMatrix, batch.texCoordMatrix);
			context->parameter(prog.ToEyeVectorMatrix, toEyeMatrix * batch.matrix );
			context->parameter(prog.TangentBinormalMatrix, batch.matrix.m33() );
			context->parameter(prog.NormalMatrix, Matrix33(batch.matrix.x / batch.matrix.x.lenghtSquare(), batch.matrix.y / batch.matrix.y.lenghtSquare(), batch.matrix.z / batch.matrix.z.lenghtSquare()) );
			context->parameter(prog.NoLightBackFacesMultCoef, 1.0f - batch.material[0]->lightBackFacesTreshold);
			context->parameter(prog.NoLightBackFacesAddCoef, batch.material[0]->lightBackFacesTreshold);
			setLightProbeParameters(prog, *batch.lightProbeBase);

			if(batch.localBoneMatrixArray)
				context->parameterArray(prog.BoneArray, &batch.localBoneMatrixArray->row[0], batch.localBoneCount * 3);
			if(batch.lightProbes.size())
				context->parameterArray(prog.BoneArray, &batch.lightProbes.first()->toLightVectorXIntensivityAndIntensivity, batch.lightProbes.size() * 2);

			Vector4 opacityCoefs[2];
			opacityCoefs[0].setZero();
			opacityCoefs[1].setZero();
			Assert(batch.layerIndex[0] < 4);
			Assert(batch.layerIndex[1] < 4);
			opacityCoefs[0][batch.layerIndex[0]] = 1.0f;
			opacityCoefs[1][batch.layerIndex[1]] = 1.0f;
			context->parameterArray(prog.OpacityCoefs, opacityCoefs, 2);

			//ps
			context->parameter(prog.EnvColor, batch.enviromentColor);
			context->parameter(prog.NormalMapScale, batch.material[0]->normalMapScale);
			context->parameter(prog.OpacityScale, batch.material[0]->opacityScale);
			context->parameter(prog.GlossinesScale, batch.material[0]->glossinesScale);
			context->parameter(prog.SpecularMaskScale, batch.material[0]->specularMaskScale);
			context->parameter(prog.GlossinesDeltha, batch.material[0]->glossinesDeltha);
			context->parameter(prog.SpecularMaskDeltha, batch.material[0]->specularMaskDeltha);
			context->parameter(prog.Frensel, batch.material[0]->frensel);
			context->parameter(prog.Metal, batch.material[0]->metal);
			//context->parameter(prog.ColorMatrix, batch.material[0]->colorMatrix);
			context->parameter(prog.ColorMultiplyer, sensitivity);
			context->parameter(prog.RoughtnessSqrExp2Coef, -9.0f);
			context->parameter(prog.BacklightMultCoef, 1.0f - batch.material[0]->lightBackFacesTreshold);
			context->parameter(prog.BacklightAddCoef, batch.material[0]->lightBackFacesTreshold);

			if(pTex02 && pTex12)
			{
				context->parameter(prog.NormalMapScale2, batch.material[1]->normalMapScale);
				context->parameter(prog.OpacityScale2, batch.material[1]->opacityScale);
				context->parameter(prog.GlossinesScale2, batch.material[1]->glossinesScale);
				context->parameter(prog.SpecularMaskScale2, batch.material[1]->specularMaskScale);
				context->parameter(prog.GlossinesDeltha2, batch.material[1]->glossinesDeltha);
				context->parameter(prog.SpecularMaskDeltha2, batch.material[1]->specularMaskDeltha);
				context->parameter(prog.Frensel2, batch.material[1]->frensel);
				context->parameter(prog.Metal2, batch.material[1]->metal);
				//context->parameter(prog.ColorMatrix2, batch.material[1]->colorMatrix);
			}

			context->texture(prog.Tex0, *pTex0);
			context->texture(prog.Tex1, *pTex1);

			if(pTex02 && pTex12)
			{
				context->texture(prog.Tex02, *pTex02);
				context->texture(prog.Tex12, *pTex12);
			}

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
			if(!pTex || !pEnvTex)
				return;

			WaterPlaneShaderProgramIndex params;
			params.vs.format = batch.geometryList.first()->vbFormat;
			params.vs.lightProbe = batch.lightProbeBase->type;
			params.ps.singleLightProbe = isSingleLightProbe(batch.lightProbeBase->type, batch.geometryList.first()->vbFormat);

			const WaterPlaneShaderProgramAndParameters& prog = getWaterPlaneProgram(params);
			if(!prog.program)
				return;
			context->program(*prog.program);

			Matrix44 fullTransformMatrix;
			mult(fullTransformMatrix, viewAndProj, Matrix44(batch.matrix, Vector4(0,0,0,1)));
			context->parameter(prog.FullTransformMatrix, fullTransformMatrix);
			context->parameter(prog.TexCoordMatrix, batch.texCoordMatrix);
			context->parameter(prog.ToEyeVectorMatrix, toEyeMatrix * batch.matrix );
			context->parameter(prog.TangentBinormalMatrix, batch.matrix.m33() );
			context->parameter(prog.NormalMatrix, Matrix33(batch.matrix.x / batch.matrix.x.lenghtSquare(), batch.matrix.y / batch.matrix.y.lenghtSquare(), batch.matrix.z / batch.matrix.z.lenghtSquare()) );
			context->parameter(prog.NoLightBackFacesMultCoef, 1.0f - batch.lightBackFacesTreshold );
			context->parameter(prog.NoLightBackFacesAddCoef, batch.lightBackFacesTreshold);

			setLightProbeParameters(prog, *batch.lightProbeBase);

			Vector4 opacityCoefs[2];
			opacityCoefs[0].set(1.0f, 0.0f, 0.0f, 0.0f);
			opacityCoefs[1].set(0.0f, 1.0f, 0.0f, 0.0f);
			context->parameterArray(prog.OpacityCoefs, opacityCoefs, 2);

			//ps
			context->parameter(prog.EnvColor, batch.enviromentColor);
			Vector3 bledCoefs(batch.normalMapScale * (1.0f - batch.layerBlendCoef) * 2.0f, batch.normalMapScale * batch.layerBlendCoef * 2.0f, - batch.normalMapScale);
			context->parameter(prog.BlendCoefs, bledCoefs );
			context->parameter(prog.OpacityScale, batch.opacityScale);
			context->parameter(prog.Albedo, batch.albedo * batch.albedo);
			context->parameter(prog.SpecularMask, batch.specularMask * batch.specularMask);
			context->parameter(prog.Frensel, batch.frensel);
			context->parameter(prog.RoughtnessSqr, exp2(-9.0f * batch.glossines));
			context->parameter(prog.EnvCoordScale, batch.envCoordScale);
			context->parameter(prog.EnvTexCoordScale, batch.envTexCoordScale);
			context->parameter(prog.ColorMultiplyer, sensitivity);

			context->texture(prog.Tex, *pTex);
			context->texture(prog.EnvTex, *pEnvTex);

			foreach(it, batch.geometryList)
				context->draw(it->vb, it->vbFormat, it->vbOffset, it->vbSize, it->ib, it->ibFirst, it->trisCount);

			break;
		}
	}
}

}
 
