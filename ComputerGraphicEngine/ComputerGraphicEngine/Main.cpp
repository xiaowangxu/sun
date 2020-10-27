#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "sunMatLib.h"
#include <algorithm>
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
		//double list[4][4] = { {0.1 * 720.0 / 1080,0,0,0}, {0,0.1,0,0},{0,0,0,0},{0,0,0,1} };
		//projection = Matrix4(list);
		mesh = TriFaceMesh::LOAD_OBJ("untitled1.obj");
		return true;
	}

	void draw_Mesh()
	{
		float angle = time * 0.6;
		Matrix4 rotateY = Matrix4::ROTATE_Y(angle);
		Matrix4 rotateZ = Matrix4::ROTATE_Z(angle * 0.35);
		Matrix4 rotateX = Matrix4::ROTATE_X(angle);
		Matrix4 move = Matrix4::TRANSLATE(0, 0, 5);
		Matrix4 view = camera.cameraMatrix().quickInvert();
		Matrix4 transform = move * (rotateX * (rotateY * rotateZ));
		//cout << transform << endl;
		vector<TriFace> list;

		for (TriFace face : mesh._faces)
		{

			Vector3 p1 = transform * face.get_Point1();
			Vector3 p2 = transform * face.get_Point2();
			Vector3 p3 = transform * face.get_Point3();

			TriFace newface(p1, p2, p3);

			double angle = (newface.get_Normal() * (newface.get_Point1() - camera._position));

			if (angle >= 0)
				continue;

			Vector3 pf1 = projection * (view * p1);
			Vector3 pf2 = projection * (view * p2);
			Vector3 pf3 = projection * (view * p3);

			pf1 /= pf1._w;
			pf2 /= pf2._w;
			pf3 /= pf3._w;

			pf1._x *= -1;
			pf1._y *= -1;
			pf2._x *= -1;
			pf2._y *= -1;
			pf3._x *= -1;
			pf3._y *= -1;

			TriFace renderface(pf1, pf2, pf3);

			renderface.get_Normal() = newface.get_Normal();

			list.emplace_back(renderface);
		}

		sort(list.begin(), list.end(), [](TriFace &t1, TriFace &t2) {
			return (t1.get_Point1()._z + t1.get_Point2()._z + t1.get_Point3()._z) / 3.0f > (t2.get_Point1()._z + t2.get_Point2()._z + t2.get_Point3()._z) / 3.0f;
		});

		const double width = ScreenWidth() / 2;
		const double height = ScreenHeight() / 2;
		const Vector3 translate(1, 1, 0);
		const Vector3 light(1, 1, -1);

		for (TriFace face : list)
		{
			Vector3 pf1 = face.get_Point1();
			Vector3 pf2 = face.get_Point2();
			Vector3 pf3 = face.get_Point3();

			pf1 += translate;
			pf2 += translate;
			pf3 += translate;

			pf1._x *= width;
			pf1._y *= height;
			pf2._x *= width;
			pf2._y *= height;
			pf3._x *= width;
			pf3._y *= height;

			double alpha1 = pow(max((face.get_Normal() * light.normalize()), 0), 1);

			olc::Pixel color(uint8_t(255 * alpha1), uint8_t(255 * alpha1), uint8_t(255 * alpha1));

			FillTriangle(olc::vi2d(pf1._x, pf1._y), olc::vi2d(pf2._x, pf2._y), olc::vi2d(pf3._x, pf3._y), color);

			//DrawLine(pf1._x, pf1._y, pf2._x, pf2._y);
			//DrawLine(pf2._x, pf2._y, pf3._x, pf3._y);
			//DrawLine(pf1._x, pf1._y, pf3._x, pf3._y);
		}
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(olc::LEFT).bHeld)
			camera._position._x += 8.0f * fElapsedTime;	// Travel Upwards

		if (GetKey(olc::RIGHT).bHeld)
			camera._position._x -= 8.0f * fElapsedTime;	// Travel Downwards

		if (GetKey(olc::UP).bHeld)
			camera._position._y += 8.0f * fElapsedTime;	// Travel Upwards

		if (GetKey(olc::DOWN).bHeld)
			camera._position._y -= 8.0f * fElapsedTime;	// Travel Downwards

		if (GetKey(olc::W).bHeld)
			camera._position._z += 8.0f * fElapsedTime;	// Travel Upwards

		if (GetKey(olc::S).bHeld)
			camera._position._z -= 8.0f * fElapsedTime;	// Travel Downwards


		Clear(olc::Pixel(0, 0, 0, 255));
		draw_Mesh();
		time += fElapsedTime * 0.1;
		// std::cout << time << std::endl;

		return true;
	}

private:
	float time = 0;
	TriFaceMesh mesh;
	Matrix4 projection = Matrix4::PROJECTION(864.0 / 1536.0, 1 / tan(60.0 / 2 / 180 * acos(-1)), 0.1, 1000);
	Camera camera;
};

int main()
{
	Example demo;
	if (demo.Construct(1536, 864, 1, 1))
		demo.Start();

	return 0;
}