#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "sunMatLib.h"
#include <algorithm>
#include "sun3D.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <list>
using namespace std;

class Example : public olc::PixelGameEngine
{
	friend class TriFaceMesh;

public:
	Example()
	{
		sAppName = "SHU ComputerGraphic Group 22";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		//double list[4][4] = { {0.1 * 720.0 / 1080,0,0,0}, {0,0.1,0,0},{0,0,0,0},{0,0,0,1} };
		//projection = Matrix4(list);
		meshes[0] = TriFaceMesh::LOAD_OBJ("untitled1.obj");
		meshes[1] = TriFaceMesh::LOAD_OBJ("teapot.obj");
		meshes[2] = TriFaceMesh::LOAD_OBJ("untitled.obj");
		return true;
	}

	void draw_Mesh()
	{
		float angle = time;
		double s = sin(angle * 20.0) * 0.5 + 1;
		Matrix4 scale = Matrix4::SCALE(1, 1, 1);
		Matrix4 rotateY = Matrix4::ROTATE_Y(angle);
		Matrix4 rotateZ = Matrix4::ROTATE_Z(angle * 0.35);
		Matrix4 rotateX = Matrix4::ROTATE_X(angle);
		Matrix4 move = Matrix4::TRANSLATE(0, 0, 10);
		Matrix4 view = camera.cameraMatrix().quickInvert();
		Matrix4 transform = move * (rotateX * (rotateY * (rotateZ * scale)));
		//cout << transform << endl;
		vector<TriFace> list;

		for (TriFace face : (*mesh)._faces)
		{

			Vector3 p1 = transform * face.get_Point1();
			Vector3 p2 = transform * face.get_Point2();
			Vector3 p3 = transform * face.get_Point3();

			TriFace newface(p1, p2, p3);

			double angle = (newface.get_Normal() * (newface.get_Point1() - camera._position));

			if (angle >= 0)
				continue;

			Vector3 pf1 = (view * p1);
			Vector3 pf2 = (view * p2);
			Vector3 pf3 = (view * p3);

			TriFace viewedface(pf1, pf2, pf3);

			TriFace clipped[2];
			int tri_count = camera._nearplane.clip_TriFace(viewedface, clipped[0], clipped[1]);

			for (int i = 0; i < tri_count; i++)
			{
				pf1 = projection * clipped[i].get_Point1();
				pf2 = projection * clipped[i].get_Point2();
				pf3 = projection * clipped[i].get_Point3();

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
		}

		sort(list.begin(), list.end(), [](TriFace &t1, TriFace &t2) {
			return (t1.get_Point1()._z + t1.get_Point2()._z + t1.get_Point3()._z) / 3.0f > (t2.get_Point1()._z + t2.get_Point2()._z + t2.get_Point3()._z) / 3.0f;
		});

		const double width = ScreenWidth() / 2;
		const double height = ScreenHeight() / 2;
		const Vector3 translate(1, 1, 0);
		const Vector3 light(1, 1, -1);

		// Raster
		for (TriFace &face : list)
		{
			TriFace clipped[2];
			int new_count = 1;
			std::list<TriFace> trifacelist;

			trifacelist.push_back(face);

			for (int p = 0; p < 4; p++)
			{
				int new_tri = 0;
				while (new_count > 0)
				{
					TriFace test = trifacelist.front();
					trifacelist.pop_front();
					new_count--;

					if (p == 0) //top
					{
						const Vector3 pos(0, -1, 0);
						const Vector3 normal(0, 1, 0);
						Plane clip_plane(pos, normal);
						new_tri = clip_plane.clip_TriFace(test, clipped[0], clipped[1]);
					}
					else if (p == 1) //bottom
					{
						const Vector3 pos(0, 1, 0);
						const Vector3 normal(0, -1, 0);
						Plane clip_plane(pos, normal);
						new_tri = clip_plane.clip_TriFace(test, clipped[0], clipped[1]);
					}
					else if (p == 2) //left
					{
						const Vector3 pos(-1, 0, 0);
						const Vector3 normal(1, 0, 0);
						Plane clip_plane(pos, normal);
						new_tri = clip_plane.clip_TriFace(test, clipped[0], clipped[1]);
					}
					else if (p == 3) //right
					{
						const Vector3 pos(1, 0, 0);
						const Vector3 normal(-1, 0, 0);
						Plane clip_plane(pos, normal);
						new_tri = clip_plane.clip_TriFace(test, clipped[0], clipped[1]);
					}

					for (int i = 0; i < new_tri; i++)
					{
						trifacelist.push_back(clipped[i]);
					}
				}
				new_count = trifacelist.size();
			}

			for (TriFace &new_face : trifacelist)
			{
				Vector3 pf1 = new_face.get_Point1();
				Vector3 pf2 = new_face.get_Point2();
				Vector3 pf3 = new_face.get_Point3();

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

				olc::Pixel color(uint8_t(40 + 215 * alpha1), uint8_t(40 + 215 * alpha1), uint8_t(40 + 215 * alpha1));

				if (wireframe)
				{
					DrawLine(pf1._x, pf1._y, pf2._x, pf2._y, color);
					DrawLine(pf2._x, pf2._y, pf3._x, pf3._y, color);
					DrawLine(pf1._x, pf1._y, pf3._x, pf3._y, color);
				}
				else
				{
					FillTriangle(olc::vi2d(pf1._x, pf1._y), olc::vi2d(pf2._x, pf2._y), olc::vi2d(pf3._x, pf3._y), color);
				}
			}
		}
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(olc::LEFT).bHeld)
			camera._position._x += 2.0f * fElapsedTime; // Travel Upwards

		if (GetKey(olc::RIGHT).bHeld)
			camera._position._x -= 2.0f * fElapsedTime; // Travel Downwards

		if (GetKey(olc::UP).bHeld)
			camera._position._y += 2.0f * fElapsedTime; // Travel Upwards

		if (GetKey(olc::DOWN).bHeld)
			camera._position._y -= 2.0f * fElapsedTime; // Travel Downwards

		if (GetKey(olc::W).bHeld)
			camera._position._z += 2.0f * fElapsedTime; // Travel Upwards

		if (GetKey(olc::S).bHeld)
			camera._position._z -= 2.0f * fElapsedTime; // Travel Downwards

		// if (GetKey(olc::A).bHeld)
		// {
		// 	fov = min(180.0, fov + 20 * fElapsedTime);
		// 	projection = Matrix4::PROJECTION(720.0 / 1280.0, 1 / tan(fov / 2 / 180 * acos(-1)), 0.1, 1000);
		// }

		// if (GetKey(olc::D).bHeld)
		// {
		// 	fov = max(45.0, fov - 20 * fElapsedTime);
		// 	projection = Matrix4::PROJECTION(720.0 / 1280.0, 1 / tan(fov / 2 / 180 * acos(-1)), 0.1, 1000);
		// }

		if (GetKey(olc::SPACE).bPressed)
			wireframe = !wireframe; // Travel Downwards

		if (GetKey(olc::K).bPressed)
		{
			current = (current + 1) % 3;
			mesh = &meshes[current];
		}

		Clear(olc::Pixel(0, 0, 0, 255));
		draw_Mesh();
		time += fElapsedTime * 0.1;
		// std::cout << time << std::endl;

		return true;
	}

private:
	float time = 0;
	TriFaceMesh meshes[3];
	TriFaceMesh *mesh = &meshes[0];
	int current = 0;
	Matrix4 projection = Matrix4::PROJECTION(720.0 / 1280.0, 1 / tan(60.0 / 2 / 180 * acos(-1)), 0.1, 1000);
	Camera camera;
	double fov = 60.0;
	bool wireframe = false;
};

int main()
{
	Example demo;
	if (demo.Construct(1280, 720, 1, 1))
		demo.Start();

	return 0;
}