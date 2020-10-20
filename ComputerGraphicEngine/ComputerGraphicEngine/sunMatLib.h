#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <initializer_list>

class Vector3;
class Matrix4;

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
		this->_w = 1;
	}

	Vector3 &operator=(const Vector3 &copy)
	{
		_x = copy._x;
		_y = copy._y;
		_z = copy._z;
		_w = copy._w;
		return *this;
	}
	Vector3 operator+(const Vector3 &b)
	{
		Vector3 ans(this->_x + b._x, this->_y + b._y, this->_z + b._z);
		return ans;
	}
	Vector3 operator-(const Vector3 &b)
	{
		Vector3 ans(this->_x - b._x, this->_y - b._y, this->_z - b._z);
		return ans;
	}
	Vector3 operator*(const double &a)
	{
		Vector3 ans(this->_x * a, this->_y * a, this->_z * a);
		return ans;
	}
	friend Vector3 operator*(const double &a, const Vector3 &b)
	{
		Vector3 ans(b._x * a, b._y * a, b._z * a);
		return ans;
	}
	double operator*(const Vector3 &b)
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
	Vector3 cross(const Vector3 &b)
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
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				ans._mat[r][c] = _mat[r][0] * b._mat[0][c] + _mat[r][1] * b._mat[1][c] + _mat[r][2] * b._mat[2][c] + _mat[r][3] * b._mat[3][c];
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
		if (ans._w != 0)
		{
			ans._x /= ans._w;
			ans._y /= ans._w;
			ans._z /= ans._w;
			ans._w = 1;
			return ans;
		}
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
	static Matrix4 PROJECTION(const double &aspect_ratio, const double &fov_rad, const double &near_panel, const double &far_panel)
	{
		double f_n = far_panel - near_panel;
		double list[4][4] = {{aspect_ratio * fov_rad, 0, 0, 0},
							 {0, fov_rad, 0, 0},
							 {0, 0, far_panel / f_n, 1},
							 {0, 0, (-far_panel * near_panel) / f_n, 0}};
		Matrix4 ans(list);
		return ans;
	}

	static Matrix4 ROTATE_X(const double &angle)
	{
		double list[4][4] = {{1, 0, 0, 0},
							 {0, cos(angle), -sin(angle), 0},
							 {0, sin(angle), cos(angle), 0},
							 {0, 0, 0, 1}};
		Matrix4 ans(list);
		return ans;
	}

	static Matrix4 ROTATE_Y(const double &angle)
	{
		double list[4][4] = {{cos(angle), 0, sin(angle), 0},
							 {0, 1, 0, 0},
							 {-sin(angle), 0, cos(angle), 0},
							 {0, 0, 0, 1}};
		Matrix4 ans(list);
		return ans;
	}

	static Matrix4 ROTATE_Z(const double &angle)
	{
		double list[4][4] = {{cos(angle), -sin(angle), 0, 0},
							 {sin(angle), cos(angle), 0, 0},
							 {0, 0, 1, 0},
							 {0, 0, 0, 1}};
		Matrix4 ans(list);
		return ans;
	}

	static Matrix4 ROTATE(const double &x, const double &y, const double &z)
	{
		Matrix4 ans = Matrix4::ROTATE_X(x);
		ans = ans * Matrix4::ROTATE_Y(y);
		ans = ans * Matrix4::ROTATE_Z(z);
		return ans;
	}

	static Matrix4 TRANSLATE(const double &x, const double &y, const double &z)
	{
		double list[4][4] = {{1, 0, 0, x},
							 {0, 1, 0, y},
							 {0, 0, 1, z},
							 {0, 0, 0, 1}};
		Matrix4 ans(list);
		return ans;
	}
};