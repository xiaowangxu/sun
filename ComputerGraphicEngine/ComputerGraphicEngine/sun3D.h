#pragma once
#include "sunMatLib.h"
#include <vector>
#include <fstream>
#include <string>
#include <strstream>
using namespace std;

class TriFaceMesh;
class Camera;

class Camera
{
public:
	Vector3 _position;
	Vector3 _lookdir;
	Vector3 _up;
	Plane _nearplane;
	double _near, _far;

public:
	Camera()
	{
		_near = 1.1;
		_far = 1000;
		_position.set(0, 0, 0);
		_lookdir.set(0, 0, 1);
		_up.set(0, 1, 0);
		Vector3 origin(0, 0, _near);
		Vector3 normal(0, 0, 1);
		_nearplane._position = origin;
		_nearplane._normal = normal;
	}

	Matrix4 cameraMatrix()
	{
		Vector3 target = _position + _lookdir;
		return Matrix4::POINTAT(_position, target, _up);
	}
};

class TriFaceMesh
{
public:
	vector<TriFace> _faces;

public:
	TriFaceMesh() {}
	TriFaceMesh(const TriFaceMesh &copy)
	{
		_faces = copy._faces;
	}

	void add_Face(const TriFace &face)
	{
		_faces.push_back(face);
	}

	vector<TriFace> &get_Faces()
	{
		return this->_faces;
	}

	// static
	static TriFaceMesh CUBE(double size = 1.0)
	{
		TriFaceMesh mesh;
		Vector3 a(0, 0, 0), b(0, size, 0), c(size, size, 0);
		TriFace face(a, b, c);
		mesh.add_Face(face);
		a.set(0, 0, 0);
		b.set(size, size, 0);
		c.set(size, size, 0);
		face.set(a, b, c);
		mesh.add_Face(face);
		a.set(size, 0, 0);
		b.set(size, size, 0);
		c.set(size, size, size);
		face.set(a, b, c);
		mesh.add_Face(face);
		a.set(size, 0, 0);
		b.set(size, size, size);
		c.set(size, 0, size);
		face.set(a, b, c);
		mesh.add_Face(face);
		a.set(size, 0, size);
		b.set(size, size, size);
		c.set(0, size, size);
		face.set(a, b, c);
		mesh.add_Face(face);
		a.set(size, 0, size);
		b.set(0, size, size);
		c.set(0, 0, size);
		face.set(a, b, c);
		mesh.add_Face(face);
		a.set(0, 0, size);
		b.set(0, size, size);
		c.set(0, size, 0);
		face.set(a, b, c);
		mesh.add_Face(face);
		a.set(0, 0, size);
		b.set(0, size, 0);
		c.set(0, 0, 0);
		face.set(a, b, c);
		mesh.add_Face(face);
		a.set(0, size, 0);
		b.set(0, size, size);
		c.set(size, size, size);
		face.set(a, b, c);
		mesh.add_Face(face);
		a.set(0, size, 0);
		b.set(size, size, size);
		c.set(size, size, 0);
		face.set(a, b, c);
		mesh.add_Face(face);
		a.set(size, 0, size);
		b.set(0, 0, size);
		c.set(0, 0, 0);
		face.set(a, b, c);
		mesh.add_Face(face);
		a.set(size, 0, size);
		b.set(0, 0, 0);
		c.set(size, 0, 0);
		face.set(a, b, c);
		mesh.add_Face(face);
		return mesh;
	}

	static TriFaceMesh LOAD_OBJ(const string &path)
	{
		TriFaceMesh mesh;
		ifstream loader(path);
		if (loader.is_open())
		{

			// Local cache of verts
			vector<Vector3> points;

			while (!loader.eof())
			{
				string line;
				getline(loader, line);

				stringstream linestream;
				linestream << line;

				char type;

				if (line[0] == 'v')
				{
					Vector3 point;
					double x, y, z;
					linestream >> type >> x >> y >> z;
					point.set(x, y, z);
					points.push_back(point);
				}

				if (line[0] == 'f')
				{
					int p1, p2, p3;
					linestream >> type >> p1 >> p2 >> p3;
					TriFace face;
					face.set(points[p1 - 1], points[p2 - 1], points[p3 - 1]);
					mesh.add_Face(face);
				}
			}

			loader.close();
		}
		return mesh;
	}
};