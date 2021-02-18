#pragma once

class q_angles
{
public:
    q_angles(void)
    {
        Init();
    }
    q_angles(float X, float Y, float Z)
    {
        Init(X, Y, Z);
    }
    q_angles(const float* clr)
    {
        Init(clr[0], clr[1], clr[2]);
    }

    void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
    {
        pitch = ix;
        yaw = iy;
        roll = iz;
    }

    float operator[](int i) const
    {
        return ((float*)this)[i];
    }
    float& operator[](int i)
    {
        return ((float*)this)[i];
    }

    q_angles& operator+=(const q_angles& v)
    {
        pitch += v.pitch; yaw += v.yaw; roll += v.roll;
        return *this;
    }
    q_angles& operator-=(const q_angles& v)
    {
        pitch -= v.pitch; yaw -= v.yaw; roll -= v.roll;
        return *this;
    }
    q_angles& operator*=(float fl)
    {
        pitch *= fl;
        yaw *= fl;
        roll *= fl;
        return *this;
    }
    q_angles& operator*=(const q_angles& v)
    {
        pitch *= v.pitch;
        yaw *= v.yaw;
        roll *= v.roll;
        return *this;
    }
    q_angles& operator/=(const q_angles& v)
    {
        pitch /= v.pitch;
        yaw /= v.yaw;
        roll /= v.roll;
        return *this;
    }
    q_angles& operator+=(float fl)
    {
        pitch += fl;
        yaw += fl;
        roll += fl;
        return *this;
    }
    q_angles& operator/=(float fl)
    {
        pitch /= fl;
        yaw /= fl;
        roll /= fl;
        return *this;
    }
    q_angles& operator-=(float fl)
    {
        pitch -= fl;
        yaw -= fl;
        roll -= fl;
        return *this;
    }

    q_angles& operator=(const q_angles &vOther)
    {
        pitch = vOther.pitch; yaw = vOther.yaw; roll = vOther.roll;
        return *this;
    }

    q_angles operator-(void) const
    {
        return q_angles(-pitch, -yaw, -roll);
    }
    q_angles operator+(const q_angles& v) const
    {
        return q_angles(pitch + v.pitch, yaw + v.yaw, roll + v.roll);
    }
    q_angles operator-(const q_angles& v) const
    {
        return q_angles(pitch - v.pitch, yaw - v.yaw, roll - v.roll);
    }
    q_angles operator*(float fl) const
    {
        return q_angles(pitch * fl, yaw * fl, roll * fl);
    }
    q_angles operator*(const q_angles& v) const
    {
        return q_angles(pitch * v.pitch, yaw * v.yaw, roll * v.roll);
    }
    q_angles operator/(float fl) const
    {
        return q_angles(pitch / fl, yaw / fl, roll / fl);
    }
    q_angles operator/(const q_angles& v) const
    {
        return q_angles(pitch / v.pitch, yaw / v.yaw, roll / v.roll);
    }

    float Length() const
    {
        return sqrt(pitch*pitch + yaw*yaw + roll*roll);
    }
    float LengthSqr(void) const
    {
        return (pitch*pitch + yaw*yaw + roll*roll);
    }
    bool IsZero(float tolerance = 0.01f) const
    {
        return (pitch > -tolerance && pitch < tolerance &&
            yaw > -tolerance && yaw < tolerance &&
            roll > -tolerance && roll < tolerance);
    }

	float Normalize() const
	{
		q_angles res = *this;
		float l = res.Length();
		if (l != 0.0f)
		{
			res /= l;
		}
		else
		{
			res[0] = res[1] = res[2] = 0.0f;
		}
		return l;
	}

    float pitch;
    float yaw;
    float roll;
};

inline q_angles operator*(float lhs, const q_angles& rhs)
{
    return rhs * lhs;
}
inline q_angles operator/(float lhs, const q_angles& rhs)
{
    return rhs / lhs;
}
