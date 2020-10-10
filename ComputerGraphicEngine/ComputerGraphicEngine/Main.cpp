#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "sunMatLib.h"
#include "sun3D.h"
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

class Example : public olc::PixelGameEngine
{
	friend class TriFaceMesh;

public:
	Example()
	{
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	void draw_Mesh()
	{
		float angle = time;
		Matrix4 rotateY = Matrix4::ROTATE_Y(angle);
		Matrix4 rotateZ = Matrix4::ROTATE_Z(angle * 0.5);
		Matrix4 rotatex= Matrix4::ROTATE_X(angle);
		vector<TriFace> faces = cube.get_Faces();

		for (TriFace face : faces)
		{
			Vector3 p1 = rotatex * (rotateZ * (rotateY * face.get_Point1()));
			Vector3 p2 = rotatex * (rotateZ * (rotateY * face.get_Point2()));
			Vector3 p3 = rotatex * (rotateZ * (rotateY * face.get_Point3()));

			Vector3 translatePos(0, 0, 3);
			p1 = p1 + translatePos;
			p2 = p2 + translatePos;
			p3 = p3 + translatePos;

			Vector3 pf1 = projection * p1;
			Vector3 pf2 = projection * p2;
			Vector3 pf3 = projection * p3;

			Vector3 translate(1, 1, 0);
			pf1 = pf1 + translate;
			pf2 = pf2 + translate;
			pf3 = pf3 + translate;

			pf1 = pf1 * 256;
			pf2 = pf2 * 256;
			pf3 = pf3 * 256;

			DrawLine(pf1._x, pf1._y, pf2._x, pf2._y);
			DrawLine(pf2._x, pf2._y, pf3._x, pf3._y);
			DrawLine(pf1._x, pf1._y, pf3._x, pf3._y);
		}
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		Clear(olc::Pixel(0, 0, 0, 255));
		draw_Mesh();
		time += fElapsedTime;
		// std::cout << time << std::endl;

		return true;
	}

private:
	float time = 0;
	TriFaceMesh cube = TriFaceMesh::CUBE();
	Matrix4 projection = Matrix4::PROJECTION(1, 1 / tan(90.0 / 2 / 180 * acos(-1)), 0.1, 10000);
};

int main()
{
	Example demo;
	if (demo.Construct(512, 512, 2, 2))
		demo.Start();

	return 0;
}