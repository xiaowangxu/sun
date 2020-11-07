#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <initializer_list>

class Vector3;
class Matrix4;
class TriFace;

class Vector3
{
	friend class Matrix4;

public:
	double _x, _y, _z, _w;

public:
	Vector3() : _x(0), _y(0), _z(0), _w(1) {}
	Vector3(const double &x, const double &y, const double &z) : _x(x), _y(y), _z(z), _w(1) {}
	Vector3(const Vector3 &copy)
	{
		this->_x = copy._x;
		this->_y = copy._y;
		this->_z = copy._z;
		this->_w = copy._w;
	}

	Vector3 &operator=(const Vector3 &copy)
	{
		_x = copy._x;
		_y = copy._y;
		_z = copy._z;
		_w = copy._w;
		return *this;
	}
	Vector3 operator+(const Vector3 &b) const
	{
		Vector3 ans(this->_x + b._x, this->_y + b._y, this->_z + b._z);
		return ans;
	}
	Vector3 operator-(const Vector3 &b) const
	{
		Vector3 ans(this->_x - b._x, this->_y - b._y, this->_z - b._z);
		return ans;
	}
	Vector3 operator*(const double &a) const
	{
		Vector3 ans(this->_x * a, this->_y * a, this->_z * a);
		return ans;
	}
	friend Vector3 operator*(const double &a, const Vector3 &b)
	{
		Vector3 ans(b._x * a, b._y * a, b._z * a);
		return ans;
	}
	double operator*(const Vector3 &b) const
	{
		double ans = this->_x * b._x + this->_y * b._y + this->_z * b._z;
		return ans;
	}
	Vector3 &operator*=(const double &a)
	{
		_x *= a;
		_y *= a;
		_z *= a;
		return *this;
	}
	Vector3 &operator+=(const Vector3 &b)
	{
		_x += b._x;
		_y += b._y;
		_z += b._z;
		return *this;
	}
	friend Vector3 operator/(const Vector3 &b, const double &a)
	{
		Vector3 ans(b._x / a, b._y / a, b._z / a);
		return ans;
	}
	friend Vector3 &operator/=(Vector3 &b, const double &a)
	{
		if (a == 0)
			return b;
		b._x /= a;
		b._y /= a;
		b._z /= a;
		b._w /= a;
		return b;
	}
	double length() const
	{
		return sqrt(_x * _x + _y * _y + _z * _z);
	}
	Vector3 normalize() const
	{
		double len = this->length();
		return ((*this) / len);
	}
	Vector3 &normalized()
	{
		double len = this->length();
		_x /= len;
		_y /= len;
		_z /= len;
		return *this;
	}
	Vector3 cross(const Vector3 &b) const
	{
		Vector3 ans(_y * b._z - _z * b._y,
					_z * b._x - _x * b._z, _x * b._y - _y * b._x);
		return ans;
	}
	Vector3 &crossed(const Vector3 &b)
	{
		double x = _x, y = _y, z = _z;
		set(y * b._z - z * b._y,
			z * b._x - x * b._z, x * b._y - y * b._x);
		return *this;
	}
	Vector3 &set(const double &x, const double &y, const double &z)
	{
		_x = x;
		_y = y;
		_z = z;
		return *this;
	}

	friend std::ostream &operator<<(std::ostream &out, const Vector3 &a)
	{
		out << "(" << a._x << ", " << a._y << ", " << a._z << ", " << a._w << ")";
		return out;
	}

	// static
	static Vector3 ONE()
	{
		Vector3 ans(1, 1, 1);
		return ans;
	}
	static Vector3 ZERO()
	{
		Vector3 ans(0, 0, 0);
		return ans;
	}
	static Vector3 UP()
	{
		Vector3 ans(0, 1, 0);
		return ans;
	}
	static Vector3 FRONT()
	{
		Vector3 ans(0, 0, 1);
		return ans;
	}
	static Vector3 RIGHT()
	{
		Vector3 ans(1, 0, 0);
		return ans;
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
		_normal = _line1.crossed(_line2).normalized();
		return *this;
	}
};

class Plane
{
public:
	Vector3 _position;
	Vector3 _normal;

public:
	Plane(){};
	Plane(const Vector3 &pos, const Vector3 &nor) : _position(pos), _normal(nor.normalize()){};
	Plane(const Plane &p) : _position(p._position), _normal(p._normal){};

	double distance(const Vector3 &pos) const
	{
		return _normal._x * pos._x + _normal._y * pos._y + _normal._z * pos._z - (_position * _normal);
	}

	Vector3 intersect_Line(Vector3 &start, Vector3 &end) const
	{
		double plane_d = -(_position * _normal);
		double ad = start * _normal;
		double bd = end * _normal;
		double t = (-plane_d - ad) / (bd - ad);
		Vector3 lineStartToEnd = end - start;
		Vector3 lineToIntersect = lineStartToEnd * t;
		return start + lineToIntersect;
	}

	int clip_TriFace(TriFace &in, TriFace &out1, TriFace &out2) const
	{
		Vector3 *inside_point[3];
		int inside_count = 0;
		Vector3 *outside_point[3];
		int outside_count = 0;

		double d0 = distance(in.get_Point1());
		double d1 = distance(in.get_Point2());
		double d2 = distance(in.get_Point3());

		if (d0 >= 0)
		{
			inside_point[inside_count++] = &in.get_Point1();
		}
		else
		{
			outside_point[outside_count++] = &in.get_Point1();
		}

		if (d1 >= 0)
		{
			inside_point[inside_count++] = &in.get_Point2();
		}
		else
		{
			outside_point[outside_count++] = &in.get_Point2();
		}

		if (d2 >= 0)
		{
			inside_point[inside_count++] = &in.get_Point3();
		}
		else
		{
			outside_point[outside_count++] = &in.get_Point3();
		}

		if (inside_count == 0)
		{
			// No remain triface
			return 0;
		}

		if (inside_count == 3)
		{
			// origin triface
			out1 = in;
			return 1;
		}

		if (inside_count == 1 && outside_count == 2)
		{
			// one remain triface
			Vector3 p2 = intersect_Line(*inside_point[0], *outside_point[0]);
			Vector3 p3 = intersect_Line(*inside_point[0], *outside_point[1]);
			out1.set(*inside_point[0], p2, p3);
			return 1;
		}

		if (inside_count == 2 && outside_count == 1)
		{
			// two new triface
			Vector3 p12 = intersect_Line(*inside_point[0], *outside_point[0]);
			out1.set(*inside_point[0], *inside_point[1], p12);

			Vector3 p22 = intersect_Line(*inside_point[1], *outside_point[0]);
			out2.set(*inside_point[1], p12, p22);

			return 2;
		}
	}
};

class Matrix4
{
	friend class Vector3;

private:
	double _mat[4][4];

public:
	Matrix4()
	{
		for (u_int i = 0; i < 4; ++i)
			for (u_int j = 0; j < 4; ++j)
			{
				_mat[i][j] = 0;
			}
	}

	Matrix4(double copy[4][4])
	{
		for (u_int i = 0; i < 4; ++i)
			for (u_int j = 0; j < 4; ++j)
			{
				_mat[i][j] = copy[i][j];
			}
	}

	Matrix4(const Matrix4 &copy)
	{
		for (u_int i = 0; i < 4; ++i)
			for (u_int j = 0; j < 4; ++j)
			{
				_mat[i][j] = copy._mat[i][j];
			}
	}

	Matrix4 quickInvert()
	{
		double m30 = -(_mat[0][3] * _mat[0][0] + _mat[1][3] * _mat[1][0] + _mat[2][3] * _mat[2][0]);
		double m31 = -(_mat[0][3] * _mat[0][1] + _mat[1][3] * _mat[1][1] + _mat[2][3] * _mat[2][1]);
		double m32 = -(_mat[0][3] * _mat[0][2] + _mat[1][3] * _mat[1][2] + _mat[2][3] * _mat[2][2]);
		double list[4][4] =
			{{_mat[0][0], _mat[1][0], _mat[2][0], m30},
			 {_mat[0][1], _mat[1][1], _mat[2][1], m31},
			 {_mat[0][2], _mat[1][2], _mat[2][2], m32},
			 {0, 0, 0, 1}};

		Matrix4 ans(list);
		return ans;
	}

	Matrix4 &operator=(const Matrix4 &copy)
	{
		for (u_int i = 0; i < 4; ++i)
			for (u_int j = 0; j < 4; ++j)
			{
				_mat[i][j] = copy._mat[i][j];
			}
		return *this;
	}

	Matrix4 operator*(const Matrix4 &b)
	{
		Matrix4 ans;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				ans._mat[i][j] = _mat[i][0] * b._mat[0][j] + _mat[i][1] * b._mat[1][j] + _mat[i][2] * b._mat[2][j] + _mat[i][3] * b._mat[3][j];
		return ans;
	}

	Vector3 operator*(const Vector3 &b)
	{
		Vector3 ans;
		// std::cout << b << " !!!" << std::endl;
		ans._x = b._x * _mat[0][0] + b._y * _mat[0][1] + b._z * _mat[0][2] + b._w * _mat[0][3];
		ans._y = b._x * _mat[1][0] + b._y * _mat[1][1] + b._z * _mat[1][2] + b._w * _mat[1][3];
		ans._z = b._x * _mat[2][0] + b._y * _mat[2][1] + b._z * _mat[2][2] + b._w * _mat[2][3];
		ans._w = b._x * _mat[3][0] + b._y * _mat[3][1] + b._z * _mat[3][2] + b._w * _mat[3][3];
		return ans;
	}

	friend std::ostream &operator<<(std::ostream &out, const Matrix4 &a)
	{
		out << "[" << a._mat[0][0] << "\t," << a._mat[0][1] << "\t," << a._mat[0][2] << "\t," << a._mat[0][3] << "]\n";
		out << "[" << a._mat[1][0] << "\t," << a._mat[1][1] << "\t," << a._mat[1][2] << "\t," << a._mat[1][3] << "]\n";
		out << "[" << a._mat[2][0] << "\t," << a._mat[2][1] << "\t," << a._mat[2][2] << "\t," << a._mat[2][3] << "]\n";
		out << "[" << a._mat[3][0] << "\t," << a._mat[3][1] << "\t," << a._mat[3][2] << "\t," << a._mat[3][3] << "]\n";
		return out;
	}

	// static
	inline static Matrix4 PROJECTION(const double &aspect_ratio, const double &fov_rad, const double &near_panel, const double &far_panel)
	{
		double f_n = far_panel - near_panel;
		double list[4][4] = {{aspect_ratio * fov_rad, 0, 0, 0},
							 {0, fov_rad, 0, 0},
							 {0, 0, 1.0 / f_n, (-near_panel) / f_n},
							 {0, 0, 1, 0}};
		Matrix4 ans(list);
		return ans;
	}

	inline static Matrix4 ROTATE_X(const double &angle)
	{
		double list[4][4] = {{1, 0, 0, 0},
							 {0, cos(angle), -sin(angle), 0},
							 {0, sin(angle), cos(angle), 0},
							 {0, 0, 0, 1}};
		Matrix4 ans(list);
		return ans;
	}

	inline static Matrix4 ROTATE_Y(const double &angle)
	{
		double list[4][4] = {{cos(angle), 0, sin(angle), 0},
							 {0, 1, 0, 0},
							 {-sin(angle), 0, cos(angle), 0},
							 {0, 0, 0, 1}};
		Matrix4 ans(list);
		return ans;
	}

	inline static Matrix4 ROTATE_Z(const double &angle)
	{
		double list[4][4] = {{cos(angle), -sin(angle), 0, 0},
							 {sin(angle), cos(angle), 0, 0},
							 {0, 0, 1, 0},
							 {0, 0, 0, 1}};
		Matrix4 ans(list);
		return ans;
	}

	inline static Matrix4 ROTATE(const double &x, const double &y, const double &z)
	{
		Matrix4 ans = Matrix4::ROTATE_X(x);
		ans = ans * Matrix4::ROTATE_Y(y);
		ans = ans * Matrix4::ROTATE_Z(z);
		return ans;
	}

	inline static Matrix4 SCALE(const double &x, const double &y, const double &z)
	{
		double list[4][4] = {{x, 0, 0, 0},
							 {0, y, 0, 0},
							 {0, 0, z, 0},
							 {0, 0, 0, 1}};
		Matrix4 ans(list);
		return ans;
	}

	inline static Matrix4 TRANSLATE(const double &x, const double &y, const double &z)
	{
		double list[4][4] = {{1, 0, 0, x},
							 {0, 1, 0, y},
							 {0, 0, 1, z},
							 {0, 0, 0, 1}};
		Matrix4 ans(list);
		return ans;
	}

	inline static Matrix4 POINTAT(Vector3 pos, Vector3 target, Vector3 up)
	{
		Vector3 forward = (target - pos).normalize();
		Vector3 a = forward * (up * forward);
		Vector3 newup = (up - a).normalize();

		Vector3 right = newup.cross(forward);

		double list[4][4] =
			{{right._x, newup._x, forward._x, pos._x},
			 {right._y, newup._y, forward._y, pos._y},
			 {right._z, newup._z, forward._z, pos._z},
			 {0, 0, 0, 1}};

		Matrix4 ans(list);
		return ans;
	}
};