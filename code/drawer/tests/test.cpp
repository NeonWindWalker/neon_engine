#include "../include.h"
#include "../../entrypoint/include.h"

namespace EntryPoint
{
int gInitialWindowWidth = 1280;
int gInitialWindowHeight = 720;
const char* gcGameName = "Render Tests";
Input::State gInputState;
}


namespace DrawerLib
{

IFrameWindow* gRenderWindow = 0;
IDevice* gRenderDevice = 0;
DrawTask gDrawTask;
int gCadrNumber = 0;
boolean gPressW = false;
boolean gPressS = false;
boolean gPressA = false;
boolean gPressD = false;
Timer* gTimer = 0;
float gAverageFps = 0;
InterfaceDrawHelper gInterfaceDrawHelper;
ITexture* gTexture0 = 0;
ITexture* gTexture1 = 0;
IVertexBuffer* gVertexBuffer_Mesh;
IVertexBuffer* gVertexBuffer_Hightmap;
IIndexBuffer* gIndexBuffer;
TempAllocator gTempAllocator;

void testSimplePrimitives();
void buildTextures();
void testVertexFormats();


boolean testInit(FileStorageLib::IStorage* packedge, FileStorageLib::IStorage* homedir)
{
	logInfo << "testInit";
	EntryPoint::renderInit(gRenderDevice, gRenderWindow);

	gRenderDevice->setSyncInterval(1);
	gDrawTask.clearColor.set(0.0f, 0.0f, 0.15f, 1.0f);
	gDrawTask.useClearColor = true;
	gDrawTask.renderTarget = gRenderWindow;
	gDrawTask.view.place.p.set(64.0f, -15.0f, 15.0f);
	gDrawTask.view.near = 2.0f;

	gTimer = new Timer();

	gInterfaceDrawHelper.device = gRenderDevice;
	gInterfaceDrawHelper.drawTaskParams = &gDrawTask;

	buildTextures();

	return true;
}

float moveSpeed = 100.0f;

boolean testStep(Range<const Input::EventBase* const> events)
{
	EntryPoint::renderWindowPrepare(gRenderWindow);

	float dt = (float)gTimer->dt();
	gCadrNumber ++;

	float rotAngleX = 0, rotAngleY = 0;
	for(const Input::EventBase* const * pe = events.first(); pe != events.end(); ++pe)
	{
		switch((*pe)->type())
		{
			case Input::EEventType_Key:
			{
				const Input::EventKey* e = static_cast<const Input::EventKey*>(*pe);

				switch(e->key)
				{
				case Input::EKeyType_W : gPressW = e->down; break;
				case Input::EKeyType_S : gPressS = e->down; break;
				case Input::EKeyType_A : gPressA = e->down; break;
				case Input::EKeyType_D : gPressD = e->down; break;
				}
			}
			break;

			case Input::EEventType_Motion:
			{
				const Input::EventMotion* e = static_cast<const Input::EventMotion*>(*pe);
				rotAngleX += e->dx;
				rotAngleY += e->dy;
			}
			break;

			case Input::EEventType_Touch:
			{
				const Input::EventTouch* e = static_cast<const Input::EventTouch*>(*pe);
				rotAngleX += e->dx;
				rotAngleY += e->dy;
				if(e->down)
					gPressW = true;
				if(e->up)
					gPressW = false;

				moveSpeed = 20.0f;
			}
			break;
		}
	}


	float mouseSensitivity = 0.005f;
	if(gPressW) gDrawTask.view.place.p -= gDrawTask.view.place.z * moveSpeed * dt;
	if(gPressS) gDrawTask.view.place.p += gDrawTask.view.place.z * moveSpeed * dt;
	if(gPressA) gDrawTask.view.place.p -= gDrawTask.view.place.x * moveSpeed * dt;
	if(gPressD) gDrawTask.view.place.p += gDrawTask.view.place.x * moveSpeed * dt;
	cameraRotate(gDrawTask.view.place.m33(), -rotAngleX * mouseSensitivity, -rotAngleY * mouseSensitivity);

	Vector2 s = gRenderWindow->size();
	gDrawTask.interfaceScreenSize = s;
	gDrawTask.view.perspectiveTanHalfHeight = 0.5f;
	gDrawTask.view.perspectiveTanHalfWidth = gDrawTask.view.perspectiveTanHalfHeight * s.x / s.y;
	float fps = 1.0f / dt;
	gAverageFps = gAverageFps*0.95f + fps * 0.05f;
	String text;
	utf8* end = EntryPoint::gInputState.text[0].string.find('\r');
	text << _CS("fps = ") << print((int)fps) << _CS("\navfps = ") << print((int)gAverageFps) << _CS("\n\n") << ConstString(EntryPoint::gInputState.text[0].string.first(), end);
	gInterfaceDrawHelper.text(Vector2f(0,0), text);

	testSimplePrimitives();

	testVertexFormats();



	gInterfaceDrawHelper.submit(gDrawTask);

	gRenderDevice->tempDataUpload();

	gRenderDevice->draw(gDrawTask);

	gDrawTask.clear();

	gRenderDevice->freeDeletedResources();

	EntryPoint::renderWindowPresent(gRenderWindow);

	gTempAllocator.freeTimeCoherenceOptimizedMode();
	
	return true;
}


void testDeInit()
{
	delete gTimer;
	gTimer = 0;
	EntryPoint::renderDeInit(gRenderDevice, gRenderWindow);
}

void testSimplePrimitives()
{
	Matrix34 mat;
	mat.setIdentity();
	gInterfaceDrawHelper.matrix(mat);

	gInterfaceDrawHelper.triangle(Vector2_t(1,0,0), ColorRGBA(0,255,255,255), Vector2_t(0,1,0), ColorRGBA(255,255,0,255), Vector2_t(0,0,1), ColorRGBA(255,0,255,255));

	//gInterfaceDrawHelper.triangle(Vector3(0,0,0), ColorRGBA(0,0,0,255), Vector3(100,0,0), ColorRGBA(255,0,0,255), Vector3(100,100,0), ColorRGBA(255,255,0,255));
	//gInterfaceDrawHelper.triangle(Vector3(100,100,0), ColorRGBA(255,255,0,255), Vector3(0,100,0), ColorRGBA(0,255,0,255), Vector3(0,0,0), ColorRGBA(0,0,0,255));
}

void buildTextures()
{
	gTexture0 = gRenderDevice->createTexture();
	gTexture1 = gRenderDevice->createTexture();

	Image<ColorRGBA> img0, img1;
	img0.resize(1024, 1024);
	img1.resize(1024, 1024);
	for(uint j=0; j < 1024; j++)
		for(uint i=0; i < 1024; i++)
		{
			float fi = float(i) / 1024.0f;
			float fj = float(j) / 1024.0f;
			float ci, si;
			cosSin(ci,si, fi * gcfPi * 10.0f);
			float cj, sj;
			cosSin(cj,sj, fj * gcfPi * 10.0f);
			Vector2 n(ci * sj, cj * si);
			img0(i,j).r = img0(i,j).g = img0(i,j).b = 128;
			img0(i,j).a = uint8(255.0f * (n.x * 0.5f + 0.5f));
			img1(i,j).r = 128;
			img1(i,j).g = 128;
			img1(i,j).b = 255;
			img1(i,j).a = uint8(255.0f * (n.y * 0.5f + 0.5f));
		}

	gTexture0->init(img0);
	gTexture1->init(img1);


	uint16* ind = new uint16[128 * 128 * 2 * 3];
	gIndexBuffer = gRenderDevice->createIndexBuffer();
	for(uint j=0; j < 128; j++)
		for(uint i=0; i < 128; i++)
		{
			uint16* p = &ind[(j * 128 + i) * 6];
			p[0] = j * 129 + i;
			p[1] = j * 129 + i + 1;
			p[2] = (j + 1) * 129 + i + 1;
			p[3] = p[2];
			p[4] = (j + 1) * 129 + i;
			p[5] = p[0];
		}
	gIndexBuffer->init(128 * 128 * 2 * 3, ind);
	delete[] ind;

	{
		MeshVertexFormat* data = new MeshVertexFormat[129 * 129];
		HeightMapVertexFormat* dataHM = new HeightMapVertexFormat[129 * 129];
		
		for(uint j=0; j < 129; j++)
			for(uint i=0; i < 129; i++)
			{
				MeshVertexFormat& d = data[j * 129 + i];
				float r = Vector2((float)i - 64.0f, (float)j - 64.0f).lenght();
				d.position.set((float)i, (float)j, cos(r * 0.3f) * 3.0f - 3.0f * r / 64.0f);
				d.texcoord.x = i * 128;
				d.texcoord.y = j * 128;
				convert(d.tangent, Vector2_t(1,0,0));
				convert(d.binormal, Vector2_t(0,1,0));
				convert(d.normal, Vector2_t(0,0,1));

				HeightMapVertexFormat& dHM = dataHM[j * 129 + i];
				dHM.i = i;
				dHM.j = j;
				dHM.height = d.position.z;
				dHM.layer[0] = 128;
				dHM.layer[1] = 128;
			}
		for(uint j=0; j < 129; j++)
			for(uint i=0; i < 129; i++)
			{
				MeshVertexFormat& c = data[j * 129 + i];
				MeshVertexFormat& l = data[j * 129 + (i + 129 - 1) % 129];
				MeshVertexFormat& r = data[j * 129 + (i + 1) % 129];
				MeshVertexFormat& b = data[(j + 129 - 1) % 129 * 129 + i];
				MeshVertexFormat& t = data[(j + 1) % 129 * 129 + i];
				Vector2 derivata(r.position.z - l.position.z, t.position.z - b.position.z);
				derivata *= 0.5f;
				Vector2_t tan(1.0f, 0.0f, derivata.x);
				Vector2_t bin(0.0, 1.0f, derivata.y);
				Vector2_t nrm(-derivata.x, -derivata.y, 1.0f);

				convert(c.tangent, tan.normal());
				convert(c.binormal, bin.normal());
				convert(c.normal, nrm.normal());

				HeightMapVertexFormat& dHM = dataHM[j * 129 + i];
				dHM.derivataX = floori16(derivata.x * 256);
				dHM.derivataY = floori16(derivata.y * 256);
			}

		gVertexBuffer_Mesh = gRenderDevice->createVertexBuffer();
		gVertexBuffer_Mesh->init(129 * 129 * sizeof(MeshVertexFormat), data);
		delete[] data;

		gVertexBuffer_Hightmap = gRenderDevice->createVertexBuffer();
		gVertexBuffer_Hightmap->init(129 * 129 * sizeof(HeightMapVertexFormat), dataHM);
		delete[] dataHM;
	}
}

void testVertexFormats()
{
	Vector2_t locSunPos(36, 42, 24);
	float intensiv = 144.0f;

    for(uint i=0; i < 2; ++i)
    {
		MeshBatch* batch = new(gTempAllocator) MeshBatch();
		batch->matrix.p.x = 128.0f * float(i);
		batch->material[0] = new(gTempAllocator) Material(); 
		batch->material[0]->texture[0] = gTexture0;
		batch->material[0]->texture[1] = gTexture1;

		BatchBuffersData* buffdata = new(gTempAllocator) BatchBuffersData();
		buffdata->ib = gIndexBuffer;
		buffdata->trisCount = 128 * 128 * 2;
		
		if(i == 0)
		{
			buffdata->vb = gVertexBuffer_Mesh;
			buffdata->vbFormat = EVertexFormat_Mesh;
			buffdata->vbSize = gVertexBuffer_Mesh->size();
			batch->texCoordMatrix.x.x = 64.0f;
			batch->texCoordMatrix.y.y = 64.0f;
		}
		else if(i == 1)
		{
			buffdata->vb = gVertexBuffer_Hightmap;
			buffdata->vbFormat = EVertexFormat_HeightMap;
			buffdata->vbSize = gVertexBuffer_Hightmap->size();
			batch->texCoordMatrix.x.x = 0.25f;
			batch->texCoordMatrix.y.y = 0.25f;

			batch->material[1] = batch->material[0]; 
		}

		batch->geometryList.insert(buffdata->lel);

		batch->material[0]->normalMapScale = 0.3f;

		LightProbeStar7* probe = new(gTempAllocator) LightProbeStar7();
		probe->center.set(64.0f, 64.0f, 0.0f);
		probe->invScale.set(1.0/128.0f, 1.0/128.0f, 1.0/64.0f);

		probe->c.ambientColor.set(0.0f, 0.0f, 0.1f, 0.0f);
		probe->xn.ambientColor = probe->c.ambientColor;
		probe->xp.ambientColor = probe->c.ambientColor;
		probe->yn.ambientColor = probe->c.ambientColor;
		probe->yp.ambientColor = probe->c.ambientColor;
		probe->zn.ambientColor = probe->c.ambientColor;
		probe->zp.ambientColor = probe->c.ambientColor;

		probe->c.lightColorXEnergyAndEnergy.set(1.0f, 0.0f, 0.0f, 1.0f);
		probe->xn.lightColorXEnergyAndEnergy.set(1.0f, 1.0f, 1.0f, 1.0f);
		probe->xp.lightColorXEnergyAndEnergy.set(1.0f, 1.0f, 1.0f, 1.0f);
		probe->yn.lightColorXEnergyAndEnergy.set(1.0f, 1.0f, 1.0f, 1.0f);
		probe->yp.lightColorXEnergyAndEnergy.set(1.0f, 1.0f, 1.0f, 1.0f);
		probe->zn.lightColorXEnergyAndEnergy.set(1.0f, 1.0f, 1.0f, 1.0f);
		probe->zp.lightColorXEnergyAndEnergy.set(1.0f, 1.0f, 1.0f, 1.0f);

		probe->c.toLightVectorXIntensivityAndIntensivity = Vector4(locSunPos - Vector2_t(0,0,0) , 1.0f) * intensiv;
		probe->xn.toLightVectorXIntensivityAndIntensivity = Vector4(locSunPos - Vector2_t(-128,0,0), 1.0f) * intensiv;
		probe->xp.toLightVectorXIntensivityAndIntensivity = Vector4(locSunPos - Vector2_t(128,0,0), 1.0f) * intensiv;
		probe->yn.toLightVectorXIntensivityAndIntensivity = Vector4(locSunPos - Vector2_t(0,-128,0), 1.0f) * intensiv;
		probe->yp.toLightVectorXIntensivityAndIntensivity = Vector4(locSunPos - Vector2_t(0,128,0), 1.0f) * intensiv;
		probe->zn.toLightVectorXIntensivityAndIntensivity = Vector4(locSunPos - Vector2_t(0,0,-64), 1.0f) * intensiv;
		probe->zp.toLightVectorXIntensivityAndIntensivity = Vector4(locSunPos - Vector2_t(0,0,64), 1.0f) * intensiv;

		/*
		Vector3 locpos[7];
		locpos[0] = Vector3(0,0,0);
		locpos[1] = Vector3(-128,0,0);
		locpos[2] = Vector3(128,0,0);
		locpos[3] = Vector3(0,-128,0);
		locpos[4] = Vector3(0,128,0);
		locpos[5] = Vector3(0,0,-64);
		locpos[6] = Vector3(0,0,64);

		for(uint np=0; np < 7; np++)
		{
			LightProbeUnpucked& pr = probe->probe(np);
			pr.ambientColor.set(0.0f, 0.0f, 0.1f, 0.0f);
			Vector3 l = locSunPos - locpos[np];
			float energy = intensiv / l.lenghtSquare();
			pr.normalizedToLightDirectionXEnergyAndEnergy = Vector4(l.normal() * energy, energy);
			pr.lightColorXEnergy.set(Vector3(1.0f, 1.0f, 1.0f) * energy, 0.0f);
			pr.ambientColor.set(0.0f, 0.0f, 0.1f, 0.0f);
		}
		*/

		batch->lightProbeBase = probe;

		gDrawTask.depthPasses[0].solidBatchesSet.push(batch);
    }

	/*
	for(uint i=0; i < 2; ++i)
	{
		HightmapBatch* batch = new(gTempAllocator) HightmapBatch();
		batch->matrix.p.x = 128.0f * float(i);
		batch->matrix.p.y = 128.0f;
		batch->vb = gVertexBuffer_Hightmap;
		batch->vbFormat = EVertexFormat_HeightMap;
		batch->vbSize = gVertexBuffer_Hightmap->size();
		batch->ib = gIndexBuffer;
		batch->trisCount = 128 * 128 * 2;
		batch->layers[0].textures[0] = gTexture0;
		batch->layers[0].textures[1] = gTexture1;
		batch->layers[0].normalMapScale = 0.0f;
		batch->layers[0].index = 0;
		batch->texCoordMatrix.x.x = 0.5f;
		batch->texCoordMatrix.y.y = 0.5f;

		batch->lightProbe.zeroPoint.set(0.0f, 0.0f, -16.0f);
		batch->lightProbe.invSize.set(1.0/128.0f, 1.0/128.0f, 1.0f/32.0f);

		batch->lightProbe.probe[0][0][0].ambientColor.set(0.0f, 0.0f, 0.1f, 0.0f);
		batch->lightProbe.probe[0][0][0].lightColorXEnergyAndEnergy.set(1.0f, 1.0f, 1.0f, 1.0f);
		batch->lightProbe.probe[0][0][1] = batch->lightProbe.probe[0][0][0];
		batch->lightProbe.probe[0][1][0] = batch->lightProbe.probe[0][0][0];
		batch->lightProbe.probe[0][1][1] = batch->lightProbe.probe[0][0][0];
		batch->lightProbe.probe[1][0][0] = batch->lightProbe.probe[0][0][0];
		batch->lightProbe.probe[1][0][1] = batch->lightProbe.probe[0][0][0];
		batch->lightProbe.probe[1][1][0] = batch->lightProbe.probe[0][0][0];
		batch->lightProbe.probe[1][1][1] = batch->lightProbe.probe[0][0][0];

		for(uint k=0; k < 2; ++k)
			for(uint j=0; j < 2; ++j)
				for(uint i=0; i < 2; ++i)
					batch->lightProbe.probe[k][j][i].toLightVectorXIntensivityAndIntensivity = Vector4(locSunPos - Vector3(i * 128 - 64, j * 128 - 64, k * 32 - 16) , 1.0f) * intensiv;

		gDrawTask.depthPasses[0].solidBatchesSet.push(batch);
	}
	*/
}

}

void EntryPoint::registerDelegates()
{
	gInitDelegate = makeDelegate(DrawerLib::testInit);
	gStepDelegate = makeDelegate(DrawerLib::testStep);
	gDeInitDelegate = makeDelegate(DrawerLib::testDeInit);
}



