#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "sunMatLib.h"
#include "sun3D.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
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
		mesh = TriFaceMesh::LOAD_OBJ("untitled1.obj");
		Matrix4 rotateY = Matrix4::ROTATE_Y(0);
		Matrix4 rotateZ = Matrix4::ROTATE_Z(0 * 0.5);
		Matrix4 rotateX = Matrix4::ROTATE_X(0);
		Matrix4 move = Matrix4::TRANSLATE(0, 0, 4);
		Matrix4 transform = rotateY * rotateZ * rotateX * move;
		cout << transform;
		return true;
	}

	void draw_Mesh()
	{
		float angle = time;
		Matrix4 rotateY = Matrix4::ROTATE_Y(angle);
		Matrix4 rotateZ = Matrix4::ROTATE_Z(angle * 0.5);
		Matrix4 rotateX = Matrix4::ROTATE_X(angle);
		Matrix4 move = Matrix4::TRANSLATE(0, 0, 0);
		Matrix4 transform = rotateY * rotateZ * rotateX * move;
		vector<TriFace> faces = mesh.get_Faces();

		for (TriFace face : faces)
		{

			Vector3 p1 = transform * face.get_Point1();
			Vector3 p2 = transform * face.get_Point2();
			Vector3 p3 = transform * face.get_Point3();

			Vector3 translatePos(0, 0, 3);
			p1 += translatePos;
			p2 += translatePos;
			p3 += translatePos;

			TriFace newface(p1, p2, p3);

			double angle = (newface.get_Normal() * (newface.get_Point1() - camera._position));

			if (angle >= 0)
				continue;

			Vector3 pf1 = projection * p1;
			Vector3 pf2 = projection * p2;
			Vector3 pf3 = projection * p3;

			Vector3 translate(1, 1, 0);
			pf1 += translate;
			pf2 += translate;
			pf3 += translate;

			double width = ScreenWidth() / 2;

			pf1 *= width;
			pf2 *= width;
			pf3 *= width;

			Vector3 light(0, 0, -1);

			double alpha = max(newface.get_Normal() * light, 0);

			olc::Pixel color(uint8_t(255 * alpha), uint8_t(0 * alpha), uint8_t(0 * alpha));

			FillTriangle(olc::vi2d(pf1._x, pf1._y), olc::vi2d(pf2._x, pf2._y), olc::vi2d(pf3._x, pf3._y), color);

			/*DrawLine(pf1._x, pf1._y, pf2._x, pf2._y);
			DrawLine(pf2._x, pf2._y, pf3._x, pf3._y);
			DrawLine(pf1._x, pf1._y, pf3._x, pf3._y);*/
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
	TriFaceMesh mesh;
	Matrix4 projection = Matrix4::PROJECTION(1, 1 / tan(90.0 / 2 / 180 * acos(-1)), 0.1, 10000);
	Camera camera;
};

int main()
{
	Example demo;
	if (demo.Construct(512, 512, 1, 1))
		demo.Start();

	return 0;
}