#include "../include.h"
#include "tests.h"

#define MATRIX_SIZE 128

void Base::testFloatingPointOperations()
{
#ifdef INSTRUCTIONS_SIMD
	{
	FloatVector v1234(1.0f,2.0f,3.0f,4.0f);

	Float fx = v1234.x;
	Float fy = v1234.y;
	Float fz = v1234.z;
	Float fw = v1234.w;

	SeriosAssert(fx.x == 1.0f && fx.y == 1.0f && fx.z == 1.0f && fx.w == 1.0f);
	SeriosAssert(fy.x == 2.0f && fy.y == 2.0f && fy.z == 2.0f && fy.w == 2.0f);
	SeriosAssert(fz.x == 3.0f && fz.y == 3.0f && fz.z == 3.0f && fz.w == 3.0f);
	SeriosAssert(fw.x == 4.0f && fw.y == 4.0f && fw.z == 4.0f && fw.w == 4.0f);

	FloatVector v10203040(10.0f,20.0f,30.0f,40.0f);
	FloatVector v;
	v = v10203040;
	v.x = v1234.w;
	SeriosAssert(v.x == 4.0f && v.y == 20.0f && v.z == 30.0f && v.w == 40.0f);
	v = v10203040;
	v.x = v1234.z;
	SeriosAssert(v.x == 3.0f && v.y == 20.0f && v.z == 30.0f && v.w == 40.0f);
	v = v10203040;
	v.x = v1234.y;
	SeriosAssert(v.x == 2.0f && v.y == 20.0f && v.z == 30.0f && v.w == 40.0f);
	v = v10203040;
	v.x = v1234.x;
	SeriosAssert(v.x == 1.0f && v.y == 20.0f && v.z == 30.0f && v.w == 40.0f);
	v = v10203040;
	v.x = fz;
	SeriosAssert(v.x == 3.0f && v.y == 20.0f && v.z == 30.0f && v.w == 40.0f);

	v = v10203040;
	v.y = v1234.w;
	SeriosAssert(v.y == 4.0f && v.x == 10.0f && v.z == 30.0f && v.w == 40.0f);
	v = v10203040;
	v.y = v1234.z;
	SeriosAssert(v.y == 3.0f&& v.x == 10.0f && v.z == 30.0f && v.w == 40.0f);
	v = v10203040;
	v.y = v1234.y;
	SeriosAssert(v.y == 2.0f&& v.x == 10.0f && v.z == 30.0f && v.w == 40.0f);
	v = v10203040;
	v.y = v1234.x;
	SeriosAssert(v.y == 1.0f&& v.x == 10.0f && v.z == 30.0f && v.w == 40.0f);
	v = v10203040;
	v.y = fz;
	SeriosAssert(v.y == 3.0f&& v.x == 10.0f && v.z == 30.0f && v.w == 40.0f);

	v = v10203040;
	v.z = v1234.w;
	SeriosAssert(v.z == 4.0f && v.x == 10.0f && v.y == 20.0f && v.w == 40.0f);
	v = v10203040;
	v.z = v1234.z;
	SeriosAssert(v.z == 3.0f && v.x == 10.0f && v.y == 20.0f && v.w == 40.0f);
	v = v10203040;
	v.z = v1234.y;
	SeriosAssert(v.z == 2.0f && v.x == 10.0f && v.y == 20.0f && v.w == 40.0f);
	v = v10203040;
	v.z = v1234.x;
	SeriosAssert(v.z == 1.0f && v.x == 10.0f && v.y == 20.0f && v.w == 40.0f);
	v = v10203040;
	v.z = fz;
	SeriosAssert(v.z == 3.0f && v.x == 10.0f && v.y == 20.0f && v.w == 40.0f);

	v = v10203040;
	v.w = v1234.w;
	SeriosAssert(v.w == 4.0f && v.x == 10.0f && v.y == 20.0f && v.z == 30.0f);
	v = v10203040;
	v.w = v1234.z;
	SeriosAssert(v.w == 3.0f && v.x == 10.0f && v.y == 20.0f && v.z == 30.0f);
	v = v10203040;
	v.w = v1234.y;
	SeriosAssert(v.w == 2.0f && v.x == 10.0f && v.y == 20.0f && v.z == 30.0f);
	v = v10203040;
	v.w = v1234.x;
	SeriosAssert(v.w == 1.0f && v.x == 10.0f && v.y == 20.0f && v.z == 30.0f);
	v = v10203040;
	v.w = fz;
	SeriosAssert(v.w == 3.0f && v.x == 10.0f && v.y == 20.0f && v.z == 30.0f);


	SeriosAssert(v1234.x * v10203040.x == 10.0f);
	SeriosAssert(v1234.x * v10203040.y == 20.0f);
	SeriosAssert(v1234.x * v10203040.z == 30.0f);
	SeriosAssert(v1234.x * v10203040.w == 40.0f);
	SeriosAssert(v1234.y * v10203040.x == 20.0f);
	SeriosAssert(v1234.y * v10203040.y == 40.0f);
	SeriosAssert(v1234.y * v10203040.z == 60.0f);
	SeriosAssert(v1234.y * v10203040.w == 80.0f);
	SeriosAssert(v1234.z * v10203040.x == 30.0f);
	SeriosAssert(v1234.z * v10203040.y == 60.0f);
	SeriosAssert(v1234.z * v10203040.z == 90.0f);
	SeriosAssert(v1234.z * v10203040.w == 120.0f);
	SeriosAssert(v1234.w * v10203040.x == 40.0f);
	SeriosAssert(v1234.w * v10203040.y == 80.0f);
	SeriosAssert(v1234.w * v10203040.z == 120.0f);
	SeriosAssert(v1234.w * v10203040.w == 160.0f);

	SeriosAssert(equals(Float(9.0f) + Float(3.0f), 12.0f, 0.001f));
	SeriosAssert(equals(Float(9.0f) - Float(3.0f), 6.0f, 0.001f));
	SeriosAssert(equals(Float(9.0f) * Float(3.0f), 27.0f, 0.001f));
	SeriosAssert(equals(Float(9.0f) / Float(3.0f), 3.0f, 0.001f));
	SeriosAssert(equals(rcp(Float(0.5f)), 2.0f, 0.001f));
	SeriosAssert(equals(rsq(Float(4.0f)), 0.5f, 0.001f));
	SeriosAssert(equals(sqrt(Float(9.0f)), 3.0f, 0.001f));
	SeriosAssert(equals(max(Float(9.0f), Float(-7.0f)), 9.0f, 0.001f));
	SeriosAssert(equals(min(Float(9.0f), Float(-7.0f)), -7.0f, 0.001f));
	SeriosAssert(equals(clamp(Float(9.0f), Float(-10.0f), Float(-7.0f)), -7.0f, 0.001f));
	SeriosAssert(equals(abs(Float(5.0f)), 5.0f, 0.001f));
	SeriosAssert(equals(abs(Float(-5.0f)), 5.0f, 0.001f));
	SeriosAssert(equals(sign(Float(5.0f)), 1.0f, 0.001f));
	SeriosAssert(equals(sign(Float(-5.0f)), -1.0f, 0.001f));
	}
#endif

	{
		Vector2f v12(1.0f, 2.0f);
		Vector2f v1020(10.0f, 20.0f);

		SeriosAssert(equals(v12 + v1020, Vector2f(11.0f, 22.0f), 0.001f));
		SeriosAssert(equals(v12 - v1020, Vector2f(-9.0f, -18.0f), 0.001f));
		SeriosAssert(equals(v12 * v1020, Vector2f(10.0f, 40.0f), 0.001f));
		SeriosAssert(equals(v12 / v1020, Vector2f(0.1f, 0.1f), 0.001f));
		SeriosAssert(equals(v12.dot(v1020), 50.0f, 0.001f));

		SeriosAssert(equals(abs(v12), Vector2f(1.0f, 2.0f), 0.001f));
		SeriosAssert(equals(sign(v12), Vector2f(1.0f, 1.0f), 0.001f));
		SeriosAssert(equals(min(v12, v1020), Vector2f(1.0f, 2.0f), 0.001f));
		SeriosAssert(equals(max(v12, v1020), Vector2f(10.0f, 20.0f), 0.001f));
		SeriosAssert(equals(clamp(Vector2f(-1.0f, 50.0f), v12, v1020), Vector2f(1.0f, 20.0f), 0.001f));
	}

	{
		Vector3f v123(1.0f, 2.0f, 3.0f);
		Vector3f v102030(10.0f, 20.0f, 30.0f);

		SeriosAssert(equals(v123 + v102030, Vector3f(11.0f, 22.0f, 33.0f), 0.001f));
		SeriosAssert(equals(v123 - v102030, Vector3f(-9.0f, -18.0f, -27.0f), 0.001f));
		SeriosAssert(equals(v123 * v102030, Vector3f(10.0f, 40.0f, 90.0f), 0.001f));
		SeriosAssert(equals(v123 / v102030, Vector3f(0.1f, 0.1f, 0.1f), 0.001f));
		SeriosAssert(equals(v123.dot(v102030), 140.0f, 0.001f));

		SeriosAssert(equals(abs(v123), Vector3f(1.0f, 2.0f, 3.0f), 0.001f));
		SeriosAssert(equals(sign(v123), Vector3f(1.0f, 1.0f, 1.0f), 0.001f));
		SeriosAssert(equals(min(v123, v102030), Vector3f(1.0f, 2.0f, 3.0f), 0.001f));
		SeriosAssert(equals(max(v123, v102030), Vector3f(10.0f, 20.0f, 30.0f), 0.001f));
		SeriosAssert(equals(clamp(Vector3f(-1.0f, 50.0f, 5.0f), v123, v102030), Vector3f(1.0f, 20.0f, 5.0f), 0.001f));
	}

	{
		Vector4f v1234(1.0f, 2.0f, 3.0f, 4.0f);
		Vector4f v10203040(10.0f, 20.0f, 30.0f, 40.0f);

		SeriosAssert(equals(v1234 + v10203040, Vector4f(11.0f, 22.0f, 33.0f, 44.0f), 0.001f));
		SeriosAssert(equals(v1234 - v10203040, Vector4f(-9.0f, -18.0f, -27.0f, -36.0f), 0.001f));
		SeriosAssert(equals(v1234 * v10203040, Vector4f(10.0f, 40.0f, 90.0f, 160.0f), 0.001f));
		SeriosAssert(equals(v1234 / v10203040, Vector4f(0.1f, 0.1f, 0.1f, 0.1f), 0.001f));
		SeriosAssert(equals(v1234.dot(v10203040), 300.0f, 0.001f));

		SeriosAssert(equals(abs(v1234), Vector4f(1.0f, 2.0f, 3.0f, 4.0f), 0.001f));
		SeriosAssert(equals(sign(v1234), Vector4f(1.0f, 1.0f, 1.0f, 1.0f), 0.001f));
		SeriosAssert(equals(min(v1234, v10203040), Vector4f(1.0f, 2.0f, 3.0f, 4.0f), 0.001f));
		SeriosAssert(equals(max(v1234, v10203040), Vector4f(10.0f, 20.0f, 30.0f, 40.0f), 0.001f));
		SeriosAssert(equals(clamp(Vector4f(-1.0f, 50.0f, 5.0f, 0.0f), v1234, v10203040), Vector4f(1.0f, 20.0f, 5.0f, 4.0f), 0.001f));
	}

	{
		Vector3f point(rand(-10.0f,10.0f), rand(-10.0f,10.0f),rand(-10.0f,10.0f));
		Vector3f angles(rand(-10.0f,10.0f), rand(-10.0f,10.0f),rand(-10.0f,10.0f));
		Matrix33f matrix;
		matrix.setRotationMatrixFromAnglesXYZ(angles);
		Quaternionf quat = matrix.toQuaternion();
		Matrix33f matrix2;
		matrix2.setRotationMatrixFromQuaternion(quat);
		Vector3f r1 = matrix * point;
		Vector3f r2 = quat.rotateVector(point);
		Vector3f r3 = matrix2 * point;
		Vector3f r4 = (quat * Quaternionf(point, 0.0f) * quat.conjurate()).v4().xyz();

		SeriosAssert(equals(r1, r2, 0.01f));
		SeriosAssert(equals(r1, r3, 0.01f));
		SeriosAssert(equals(r1, r4, 0.01f));

		float xxx = atan2(0.0f, 0.0f);

		Vector3f r1_i = matrix.transpose() * point;
		r1_i = -r1_i;
		Vector3f r2_i = quat.invRotateVector(point);
		r2_i = -r2_i;
		SeriosAssert(equals(r1_i, r2_i, 0.01f));
		
	}

#ifdef DEBUG
	Base::uint matrixcycles = 10;
#else
	Base::uint matrixcycles = 1000;
#endif


	Base::Vector4f B[MATRIX_SIZE];
	Base::Vector4f A[MATRIX_SIZE][MATRIX_SIZE];
	Base::Vector4f I[MATRIX_SIZE];

	for(int i=0; i < MATRIX_SIZE; ++i){
		for(int j=0; j < MATRIX_SIZE; ++j)
			A[j][i].setOne();

		B[i].setZero();
		I[i].setZero();
	}

	float xxx = 0;
	Base::Timer timer;
	for(Base::uint i=0; i < matrixcycles; ++i)
	{
		for (Base::uint k=0; k<MATRIX_SIZE; k++) {
			Base::Vector4f R = B[k] * MATRIX_SIZE;
			for (Base::uint j=0; j<MATRIX_SIZE; ){
				R += I[j] * A[k][j]; ++j;
				R += I[j] * A[k][j]; ++j;
				R += I[j] * A[k][j]; ++j;
				R += I[j] * A[k][j]; ++j;
				R += I[j] * A[k][j]; ++j;
				R += I[j] * A[k][j]; ++j;
				R += I[j] * A[k][j]; ++j;
				R += I[j] * A[k][j]; ++j;
			}
			I[k] = R;
			xxx = xxx + 1.0f / (R.lenghtSquare() + 1.0f);
		}
	}
	logInfoSkim<<"256Matrix_Mul operator style flops (xxx= " << xxx << ") time ns = " << timer.dt() * 1e9 / matrixcycles / MATRIX_SIZE / MATRIX_SIZE / 64;

	for(int i=0; i < MATRIX_SIZE; ++i){
		for(int j=0; j < MATRIX_SIZE; ++j)
			A[j][i].setOne();

		B[i].setZero();
		I[i].setZero();
	}

	xxx = 0;
	timer.dt();
	for(Base::uint i=0; i < matrixcycles; ++i)
	{
		for (Base::uint k=0; k<MATRIX_SIZE; k++) {
			Base::Vector4f R = B[k] * MATRIX_SIZE;
			for (Base::uint j=0; j<MATRIX_SIZE; )
			{
				R.x += I[j].x * A[k][j].x;
				R.y += I[j].y * A[k][j].y;
				R.z += I[j].z * A[k][j].z;
				R.w += I[j].w * A[k][j].w;
				j++;
				R.x += I[j].x * A[k][j].x;
				R.y += I[j].y * A[k][j].y;
				R.z += I[j].z * A[k][j].z;
				R.w += I[j].w * A[k][j].w;
				j++;
				R.x += I[j].x * A[k][j].x;
				R.y += I[j].y * A[k][j].y;
				R.z += I[j].z * A[k][j].z;
				R.w += I[j].w * A[k][j].w;
				j++;
				R.x += I[j].x * A[k][j].x;
				R.y += I[j].y * A[k][j].y;
				R.z += I[j].z * A[k][j].z;
				R.w += I[j].w * A[k][j].w;
				j++;
				R.x += I[j].x * A[k][j].x;
				R.y += I[j].y * A[k][j].y;
				R.z += I[j].z * A[k][j].z;
				R.w += I[j].w * A[k][j].w;
				j++;
				R.x += I[j].x * A[k][j].x;
				R.y += I[j].y * A[k][j].y;
				R.z += I[j].z * A[k][j].z;
				R.w += I[j].w * A[k][j].w;
				j++;
				R.x += I[j].x * A[k][j].x;
				R.y += I[j].y * A[k][j].y;
				R.z += I[j].z * A[k][j].z;
				R.w += I[j].w * A[k][j].w;
				j++;
				R.x += I[j].x * A[k][j].x;
				R.y += I[j].y * A[k][j].y;
				R.z += I[j].z * A[k][j].z;
				R.w += I[j].w * A[k][j].w;
				j++;
			}
			I[k] = R;
			xxx = xxx + 1.0f / (R.lenghtSquare() + 1.0f);
		}
	}
	logInfoSkim<<"256Matrix_Mul inline style flops (xxx= " << xxx << ") time ns = " << timer.dt() * 1e9 / matrixcycles / MATRIX_SIZE / MATRIX_SIZE / 64;

	matrixcycles *= 50;

	Matrix34f M0(Vector3f(rand(-0.1f,0.1f),rand(0.1f,-0.1f),rand(0.1f,-0.1f)), Vector3f(rand(-0.1f,0.1f),rand(0.1f,-0.1f),rand(0.1f,-0.1f)), Vector3f(rand(-0.1f,0.1f),rand(0.1f,-0.1f),rand(0.1f,-0.1f)), Vector3f(rand(-0.1f,0.1f),rand(0.1f,-0.1f),rand(0.1f,-0.1f)));
	Matrix34f m = M0;
	timer.dt();
	for(Base::uint i=0; i < matrixcycles; ++i)
	{
		m *= m; Assert(m.x.lenght() < 10000.0f && m.y.lenght() < 10000.0f && m.z.lenght() < 10000.0f && m.p.lenght() < 10000.0f);
		m *= m; Assert(m.x.lenght() < 10000.0f && m.y.lenght() < 10000.0f && m.z.lenght() < 10000.0f && m.p.lenght() < 10000.0f);
		m *= m; Assert(m.x.lenght() < 10000.0f && m.y.lenght() < 10000.0f && m.z.lenght() < 10000.0f && m.p.lenght() < 10000.0f);
		m *= m; Assert(m.x.lenght() < 10000.0f && m.y.lenght() < 10000.0f && m.z.lenght() < 10000.0f && m.p.lenght() < 10000.0f);
		m *= m; Assert(m.x.lenght() < 10000.0f && m.y.lenght() < 10000.0f && m.z.lenght() < 10000.0f && m.p.lenght() < 10000.0f);
		m *= m; Assert(m.x.lenght() < 10000.0f && m.y.lenght() < 10000.0f && m.z.lenght() < 10000.0f && m.p.lenght() < 10000.0f);
		m *= m; Assert(m.x.lenght() < 10000.0f && m.y.lenght() < 10000.0f && m.z.lenght() < 10000.0f && m.p.lenght() < 10000.0f);
		m *= m; Assert(m.x.lenght() < 10000.0f && m.y.lenght() < 10000.0f && m.z.lenght() < 10000.0f && m.p.lenght() < 10000.0f);
		m *= m; Assert(m.x.lenght() < 10000.0f && m.y.lenght() < 10000.0f && m.z.lenght() < 10000.0f && m.p.lenght() < 10000.0f);
		m *= m; Assert(m.x.lenght() < 10000.0f && m.y.lenght() < 10000.0f && m.z.lenght() < 10000.0f && m.p.lenght() < 10000.0f);

		m *= 0.000001f;
		m += M0;
	}
	logInfoSkim<<"Matrix34 * Matrix34 time ns = "<< timer.dt() * 1e9 / 10 / matrixcycles << " " << (m.x.lenghtSquare() + m.y.lenghtSquare() + m.z.lenghtSquare() + m.p.lenghtSquare());

	Vector3f V0(rand(-0.1f,0.1f),rand(-0.1f,0.1f),rand(-0.1f,0.1f));
	Vector3f v = V0;
	timer.dt();
	for(Base::uint i=0; i < matrixcycles; ++i)
	{
		v = M0 * v;
		v = M0 * v;
		v = M0 * v;
		v = M0 * v;
		v = M0 * v;
		v = M0 * v;
		v = M0 * v;
		v = M0 * v;
		v = M0 * v;
		v = M0 * v;

		v *= 0.01f;
		v += V0;
	}
	logInfoSkim<<"Matrix34 * Vector3 time ns = "<< timer.dt() * 1e9 / 10 / matrixcycles << " " << v.lenghtSquare();

	float f = 1.0f;
	timer.dt();
	for(Base::uint i=0; i < matrixcycles; ++i)
	{
		f = Base::cos(f);
		f = Base::cos(f);
		f = Base::cos(f);
		f = Base::cos(f);
		f = Base::cos(f);
		f = Base::cos(f);
		f = Base::cos(f);
		f = Base::cos(f);
		f = Base::cos(f);
		f = Base::cos(f);
	}
	logInfoSkim<<"cos time ns = "<< timer.dt() * 1e9 / 10 / matrixcycles << " " << f;


	Vector4f v40(rand(-1.0f,1.0f),rand(-1.0f,1.0f),rand(-1.0f,1.0f),rand(-1.0f,1.0f));
	Vector4f v4 = v40;
	v4 *= timer.dt();
	for(Base::uint i=0; i < matrixcycles; ++i)
	{
		v4 *= v4;
		v4 *= v4;
		v4 *= v4;
		v4 *= v4;
		v4 *= v4;
		v4 *= v4;
		v4 *= v4;
		v4 *= v4;
		v4 *= v4;
		v4 *= v4;

		v4 *= 0.01f;
		v4 += v40;
	}
	logInfoSkim<<"v4 * v4 time ns = "<< timer.dt() * 1e9 / 10 / matrixcycles << " " << v4.lenghtSquare();


	int v4i0[4] = {rand(-100,100),rand(-100,100),rand(-100,100),rand(-100,100)};
	int v4i[4] = {v4i0[0],v4i0[1],v4i0[2],v4i0[3]};
	v4i[0] *= timer.dt();
	for(Base::uint i=0; i < matrixcycles; ++i)
	{
		v4i[0] *= v4i[0];
		v4i[1] *= v4i[1];
		v4i[2] *= v4i[2];
		v4i[3] *= v4i[3];

		v4i[0] *= v4i[0];
		v4i[1] *= v4i[1];
		v4i[2] *= v4i[2];
		v4i[3] *= v4i[3];

		v4i[0] *= v4i[0];
		v4i[1] *= v4i[1];
		v4i[2] *= v4i[2];
		v4i[3] *= v4i[3];

		v4i[0] *= v4i[0];
		v4i[1] *= v4i[1];
		v4i[2] *= v4i[2];
		v4i[3] *= v4i[3];

		v4i[0] *= v4i[0];
		v4i[1] *= v4i[1];
		v4i[2] *= v4i[2];
		v4i[3] *= v4i[3];

		v4i[0] *= v4i[0];
		v4i[1] *= v4i[1];
		v4i[2] *= v4i[2];
		v4i[3] *= v4i[3];

		v4i[0] *= v4i[0];
		v4i[1] *= v4i[1];
		v4i[2] *= v4i[2];
		v4i[3] *= v4i[3];

		v4i[0] *= v4i[0];
		v4i[1] *= v4i[1];
		v4i[2] *= v4i[2];
		v4i[3] *= v4i[3];

		v4i[0] *= v4i[0];
		v4i[1] *= v4i[1];
		v4i[2] *= v4i[2];
		v4i[3] *= v4i[3];

		v4i[0] *= v4i[0];
		v4i[1] *= v4i[1];
		v4i[2] *= v4i[2];
		v4i[3] *= v4i[3];

		v4i[0] += v4i0[0];
		v4i[1] += v4i0[1];
		v4i[2] += v4i0[2];
		v4i[3] += v4i0[3];
	}
	logInfoSkim<<"v4i * v4i time ns = "<< timer.dt() * 1e9 / 10 / matrixcycles << " " << (v4i[0] + v4i[1] + v4i[2] + v4i[3]);

	//uint64 fasfs= 'rererfs';
	/*Matrix44f ma4, mb4;
	timer.dt();
	for(Base::uint i=0; i < matrixcycles; ++i)
	{
		madd(mb4,ma4,ma4);
		ma4 = mb4;
		mult(mb4,ma4,ma4);
		ma4 = mb4;
		mult(mb4,ma4,ma4);
		ma4 = mb4;
		mult(mb4,ma4,ma4);
		ma4 = mb4;
		mult(mb4,ma4,ma4);
		ma4 = mb4;
		mult(mb4,ma4,ma4);
		ma4 = mb4;
		mult(mb4,ma4,ma4);
		ma4 = mb4;
		mult(mb4,ma4,ma4);
		ma4 = mb4;
		mult(mb4,ma4,ma4);
		ma4 = mb4;
		mult(mb4,ma4,ma4);
		ma4 = mb4;
	}
	logInfoSkim<<"madv4 time ns = "<< timer.dt() * 1e9 / 10 / matrixcycles << " " << ma4.x.lenghtSquare();*/
}
