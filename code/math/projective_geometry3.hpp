#ifndef MATH_PROJECTIVE_GEOMETRY3_HPP
#define MATH_PROJECTIVE_GEOMETRY3_HPP


namespace pg3
{


struct multivector
{
    float32 _0; // Scalar
    float32 _1, _2, _3, _4; // Vectors
    float32 _23, _31, _12, _43, _42, _41; // Bivectors
    float32 _321, _412, _431, _423; // Trivectors / Antivectors
    float32 _1234; // Quadrivector / Antiscalar
};


int32 gr(multivector); // Grade
int32 ag(multivector); // Antigrade

multivector geometric(multivector, multivector);
multivector dot(multivector, multivector);

multivector anti_geometric(multivector, multivector);
multivector anti_dot

//
// In G_{3, 0, 1} a point P is a vector:
//    P = px e1 + py e2 + pz e3 + pw e4
//
//

}


#endif // MATH_PROJECTIVE_GEOMETRY3_HPP
