#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2
{
private:
    bool updated;
    float magnitude;
    float x, y;
public:
    Vector2 ();
    Vector2 (const float &, const float &);

    void X(const float &); //Set
    float X() const; //Get
    void Y(const float &); //Set
    float Y() const; //Get

    void Set(const float &, const float &);
    void Set(const Vector2 &);

    //Operators overloading
    Vector2 operator + (const Vector2 &) const;
    Vector2 operator - (const Vector2 &) const;
    Vector2 operator += (const Vector2 &);
    Vector2 operator -= (const Vector2 &);

    float operator * (const Vector2 &) const; //Dot product
    bool operator == (const Vector2 &) const;
    bool operator != (const Vector2 &) const;

    float operator [] (const int &) const;

    float Magnitude ();
    float SqrMagnitude ();
    void Normalize ();
    void Reflect (const Vector2 &);
};

//Other operators overloading
Vector2 operator * (const Vector2 &, const float &);
Vector2 operator * (const float &, const Vector2 &);
Vector2 operator / (const Vector2 &, const float &);
Vector2 operator / (const float &, const Vector2 &);

#endif // VECTOR2_H
