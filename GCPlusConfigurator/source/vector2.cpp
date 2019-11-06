#include <math.h>
#include "vector2.h"

Vector2::Vector2 () {
    updated = true;
    magnitude = 0.0f;
    x = 0.0f;
    y = 0.0f;
}

Vector2::Vector2 (const float &X, const float &Y) {
    updated = true;
    x = X;
    y = Y;
}

void Vector2::X(const float & X) {
    updated = true;
    x = X;
}

float Vector2::X() const {
    return x;
}

void Vector2::Y(const float & Y) {
    updated = true;
    y = Y;
}

float Vector2::Y() const {
    return y;
}

void Vector2::Set (const float &X, const float &Y) {
    updated = true;
    x = X;
    y = Y;
}

void Vector2::Set (const Vector2 &vec) {
    updated = true;
    x = vec.x;
    y = vec.y;
}

Vector2 Vector2::operator + (const Vector2 &vec) const {
    Vector2 temp = *this;
    temp.updated = true;
    temp.x += vec.x;
    temp.y += vec.y;
    return temp;
}

Vector2 Vector2::operator - (const Vector2 &vec) const {
    Vector2 temp = *this;
    temp.updated = true;
    temp.x -= vec.x;
    temp.y -= vec.y;
    return temp;
}

Vector2 Vector2::operator += (const Vector2 &vec) {
    updated = true;
    x += vec.x;
    y += vec.y;
    return *this;
}

Vector2 Vector2::operator -= (const Vector2 &vec) {
    updated = true;
    x -= vec.x;
    y -= vec.y;
    return *this;
}

float Vector2::operator * (const Vector2 &vec) const {
    float temp = x * vec.x + y * vec.y;
    return temp;
}

Vector2 operator * (const Vector2 &vec, const float &scalar) {
    Vector2 temp = vec;
    temp.Set(temp.X() * scalar, temp.Y() * scalar);
    return temp;
}

Vector2 operator * (const float &scalar, const Vector2 &vec) {
    Vector2 temp = vec;
    temp.Set(temp.X() * scalar, temp.Y() * scalar);
    return temp;
}

Vector2 operator / (const Vector2 &vec, const float &scalar) {
    Vector2 temp = vec;
    temp.Set(temp.X() / scalar, temp.Y() / scalar);
    return temp;
}

Vector2 operator / (const float &scalar, const Vector2 &vec) {
    Vector2 temp = vec;
    temp.Set(temp.X() / scalar, temp.Y() / scalar);
    return temp;
}

bool Vector2::operator == (const Vector2 &vec) const {
    if(x == vec.x && y == vec.y) return true;
    return false;
}

bool Vector2::operator != (const Vector2 &vec) const {
    if(x != vec.x || y != vec.y) return true;
    return false;
}

float Vector2::operator [] (const int &index) const {
    if (index == 0)
    {
        return x;
    }
    else if (index == 1)
    {
        return y;
    }

    return 0.0f;
}

//Returns the magnitude of the Vector
float Vector2::Magnitude () {
    if(updated)
    {
        magnitude = sqrtf(x * x + y * y);
        updated = false;
    }
    return magnitude;
}

//Returns the squared magnitude of the Vector
float Vector2::SqrMagnitude () {
    return (x * x + y * y);
}

//Normalize the vector
void Vector2::Normalize () {
    float magnitude = Magnitude ();
    if (magnitude != 0.0f)
    {
        x /= magnitude;
        y /= magnitude;
        magnitude = 0.0f;
    }
    else
    {
        x = 0.0f;
        y = 0.0f;
        magnitude = 1.0f;
    }

    updated = false;
}

//Reflect the vector towards a normal
void Vector2::Reflect (const Vector2 &normal) {
    //V - 2 N V*N
    Vector2 temp;
    temp = *this;
    temp = temp - 2 * (normal * temp) * normal;
    *this = temp;
}
