#pragma once
#include "sunMatLib.h"
#include <vector>
#include <fstream>
#include <string>
#include <strstream>
using namespace std;

class TriFace;
class TriFaceMesh;
class Camera;

class Camera
{
public:
	Vector3 _position;
	Vector3 _lookat;

public:
	Camera()
	{
		_position.set(0, 0, 0);
		_lookat.set(0, 0, -1);
	}
};

class TriFace
{
private:
	Vector3 _p1, _p2, _p3, _normal;

public:
	TriFace()
	{
		_p1 = Vector3(0, 0, 0);
		_p2 = Vector3(0, 0, 0);
		_p3 = Vector3(0, 0, 0);
		_normal = Vector3(0, 0, 0);
	}
	TriFace(Vector3 &point1, Vector3 &point2, Vector3 &point3)
	{
		_p1 = point1;
		_p2 = point2;
		_p3 = point3;
		_normal = (point2 - point1).crossed(point3 - point1).normalized();
	}
	TriFace(const TriFace &copy)
	{
		_p1 = copy._p1;
		_p2 = copy._p2;
		_p3 = copy._p3;
		_normal = copy._normal;
	}
	Vector3 &get_Normal()
	{
		return _normal;
	}
	Vector3 &get_Point1()
	{
		return _p1;
	}
	Vector3 &get_Point2()
	{
		return _p2;
	}
	Vector3 &get_Point3()
	{
		return _p3;
	}

	TriFace &set(Vector3 &point1, Vector3 &point2, Vector3 &point3)
	{
		_p1 = point1;
		_p2 = point2;
		_p3 = point3;
		Vector3 _line1 = point2 - point1;
		Vector3 _line2 = point3 - point1;
		_normal = _line1.cross(_line2);
		return *this;
	}
};

class TriFaceMesh
{
private:
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