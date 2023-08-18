//
// General formula:
//      inner(A_r, B_s) = grade(A_r * B_s, |r - s|)
//      outer(A_r, B_s) = grade(A_r * B_s, (r + s))
//
//

typedef _e0_e1e2 _e0_______e1e2; // for text alignment purposes

//======================== _e0 ========================//

//             operator == (_e0 a, _e0 b) { already exists; }
//             operator != (_e0 a, _e0 b) { already exists; }

//             operator + (_e0 a, _e0 b) { already exists; }
//             operator - (_e0 a, _e0 b) { already exists; }
//             operator * (_e0 a, _e0 b) { already exists; }
//             operator / (_e0 a, _e0 b) { already exists; }
// _e0            inner      (_e0 a, _e0 b) { return (a * b); }
// _e0            outer      (_e0 a, _e0 b) { return (a * b); }

// _e0_e1_e2_e1e2 operator + (_e0 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = a; r._1 =  b._1; r._2 = 0.f; r._3 = 0.f; return r; }
// _e0_e1_e2_e1e2 operator - (_e0 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = a; r._1 = -b._1; r._2 = 0.f; r._3 = 0.f; return r; }
   _e1         operator * (_e0 a, _e1 b) { _e1 r; r._1 = (a * b._1); return r; }
//    _e1         inner      (_e0 a, _e1 b) { _e1 r; r._1 = (a * b._1); return r; } // |0-1| = 1
//    _e1         outer      (_e0 a, _e1 b) { _e1 r; r._1 = (a * b._1); return r; } // (0+1) = 1

// _e0_e1_e2_e1e2 operator + (_e0 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = a; r._1 = 0.f; r._2 =  b._2; r._3 = 0.f; return r; }
// _e0_e1_e2_e1e2 operator - (_e0 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = a; r._1 = 0.f; r._2 = -b._2; r._3 = 0.f; return r; }
      _e2      operator * (_e0 a, _e2 b) { _e2 r; r._2 = (a * b._2); return r; }
//       _e2      inner      (_e0 a, _e2 b) { _e2 r; r._2 = (a * b._2); return r; } // |0-1| = 1
//       _e2      outer      (_e0 a, _e2 b) { _e2 r; r._2 = (a * b._2); return r; } // (0+1) = 1

// _e0_e1_e2_e1e2 operator + (_e0 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = a; r._1 =  b._1; r._2 =  b._2; r._3 = 0.f; return r; }
// _e0_e1_e2_e1e2 operator - (_e0 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = a; r._1 = -b._1; r._2 = -b._2; r._3 = 0.f; return r; }
   _e1_e2      operator * (_e0 a, _e1_e2 b) { _e1_e2 r; r._1 = (a * b._1); r._2 = (a * b._2); return r; }
//    _e1_e2      inner      (_e0 a, _e1_e2 b) { _e1_e2 r; r._1 = (a * b._1); r._2 = (a * b._2); return r; } // |0-1| = 1
//    _e1_e2      outer      (_e0 a, _e1_e2 b) { _e1_e2 r; r._1 = (a * b._1); r._2 = (a * b._2); return r; } // (0+1) = 1

_e0_______e1e2 operator + (_e0 a, _e1e2 b) { _e0_e1e2 r; r._0 = a; r._3 =  b._3; return r; }
// _e0_______e1e2 operator - (_e0 a, _e1e2 b) { _e0_e1e2 r; r._0 = a; r._3 = -b._3; return r; }
         _e1e2 operator * (_e0 a, _e1e2 b) { _e1e2 r; r._3 = (a * b._3); return r; }
//          _e1e2 inner      (_e0 a, _e1e2 b) { _e1e2 r; r._3 = (a * b._3); return r; } // |0-2| = 2
//          _e1e2 outer      (_e0 a, _e1e2 b) { _e1e2 r; r._3 = (a * b._3); return r; } // (0+2) = 2

// _e0_______e1e2 operator + (_e0 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = (a + b._0); r._3 =  b._3; return r; }
// _e0_______e1e2 operator - (_e0 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = (a - b._0); r._3 = -b._3; return r; }
_e0_______e1e2 operator * (_e0 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = (a * b._0); r._3 = (a * b._3); return r; }
// _e0_______e1e2 inner      (_e0 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = (a * b._0); r._3 = (a * b._3); return r; } // |0-0| = 0; |0-2| = 2
// _e0_______e1e2 outer      (_e0 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = (a * b._0); r._3 = (a * b._3); return r; } // (0+0) = 0; (0+2) = 2

// _e0_e1_e2_e1e2 operator + (_e0 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a + b._0); r._1 =  b._1; r._2 =  b._2; r._3 =  b._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e0 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a - b._0); r._1 = -b._1; r._2 = -b._2; r._3 = -b._3; return r; }
// _e0_e1_e2_e1e2 operator * (_e0 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a * b._0); r._1 = (a * b._1); r._2 = (a * b._2); r._3 = (a * b._3); return r; }
// _e0_e1_e2_e1e2 inner      (_e0 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a * b._0); r._1 = (a * b._1); r._2 = (a * b._2); r._3 = (a * b._3); return r; } // |0-0| = 0; |0-1| = 1; |0-2| = 2
// _e0_e1_e2_e1e2 outer      (_e0 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a * b._0); r._1 = (a * b._1); r._2 = (a * b._2); r._3 = (a * b._3); return r; } // (0+0) = 0; (0+1) = 1; (0+2) = 2

// //             operator - (_e0 a) { already exists; }
// void           conjugate  (_e0& a) {}
// _e0            conjugated (_e0  a) { return a; }
// void           normalize  (_e0& a) { a = math::sign(a); }
// _e0            normalized (_e0  a) { normalize(a); return a; }
// _e0            length_squared(_e0 a) { return (a * a); }
// _e0            length     (_e0 a) { return math::absolute(a); }
// _e0            grade_0    (_e0 a) { return a; }
//    _e1_e2      grade_1    (_e0 a) { _e1_e2 r; r._1 = 0.f; r._2 = 0.f; return r; }
//          _e1e2 grade_2    (_e0 a) { _e1e2 r; r._3 = 0.f; return r; }

// //======================== _e1 ========================//

// bool           operator == (_e1 a, _e1 b) { return (a._1 == b._1); }
// bool           operator != (_e1 a, _e1 b) { return (a._1 != b._1); }

// _e0_e1_e2_e1e2 operator + (_e1 a, _e0 b) { _e0_e1_e2_e1e2 r; r._0 =  b; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; return r; }
// _e0_e1_e2_e1e2 operator - (_e1 a, _e0 b) { _e0_e1_e2_e1e2 r; r._0 = -b; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; return r; }
//    _e1         operator * (_e1 a, _e0 b) { _e1 r; r._1 = (a._1 * b); return r; }
//    _e1         operator / (_e1 a, _e0 b) { _e1 r; r._1 = (a._1 / b); return r; }
//    _e1         inner      (_e1 a, _e0 b) { _e1 r; r._1 = (a._1 * b); return r; } // |1-0| = 1
//    _e1         outer      (_e1 a, _e0 b) { _e1 r; r._1 = (a._1 * b); return r; } // (1+0) = 1

//    _e1         operator + (_e1 a, _e1 b) { _e1 r; r._1 = (a._1 + b._1); return r; }
//    _e1         operator - (_e1 a, _e1 b) { _e1 r; r._1 = (a._1 - b._1); return r; }
// _e0            operator * (_e1 a, _e1 b) { return (a._1 * b._1); }
// _e0            inner      (_e1 a, _e1 b) { return (a._1 * b._1); } // |1-1| = 0
//          _e1e2 outer      (_e1 a, _e1 b) { _e1e2 r; r._3 = 0.f; return r; } // (1+1) = 2

   _e1_e2      operator + (_e1 a, _e2 b) { _e1_e2 r; r._1 = a._1; r._2 =  b._2; return r; }
   _e1_e2      operator - (_e1 a, _e2 b) { _e1_e2 r; r._1 = a._1; r._2 = -b._2; return r; }
         _e1e2 operator * (_e1 a, _e2 b) { _e1e2 r; r._3 = (a._1 * b._2); return r; }
// _e0            inner      (_e1 a, _e2 b) { return 0.f; } // |1-1| = 0
//          _e1e2 outer      (_e1 a, _e2 b) { _e1e2 r; r._3 = (a._1 * b._2); return r; } // (1+1) = 2

//    _e1_e2      operator + (_e1 a, _e1_e2 b) { _e1_e2 r; r._1 = (a._1 + b._1); r._2 =  b._2; return r; }
//    _e1_e2      operator - (_e1 a, _e1_e2 b) { _e1_e2 r; r._1 = (a._1 - b._1); r._2 = -b._2; return r; }
_e0_______e1e2 operator * (_e1 a, _e1_e2 b) { _e0_e1e2 r; r._0 = (a._1 * b._1); r._3 = (a._1 * b._2); return r; }
// _e0            inner      (_e1 a, _e1_e2 b) { return (a._1 * b._1); } // |1-1| = 0
//          _e1e2 outer      (_e1 a, _e1_e2 b) { _e1e2 r; r._3 = (a._1 * b._2); return r; } // (1+1) = 2

// _e0_e1_e2_e1e2 operator + (_e1 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 =  a._1; r._2 = 0.f; r._3 =  b._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e1 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = -a._1; r._2 = 0.f; r._3 = -b._3; return r; }
//       _e2      operator * (_e1 a, _e1e2 b) { _e2 r; r._2 = (a._1 * b._3); return r; }
//       _e2      inner      (_e1 a, _e1e2 b) { _e2 r; r._2 = (a._1 * b._3); return r; } // |1-2| = 1
// _e0            outer      (_e1 a, _e1e2 b) { return 0.f; } // (1+2) = 3

// _e0_e1_e2_e1e2 operator + (_e1 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 =  b._0; r._1 = a._1; r._2 = 0.f; r._3 =  b._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e1 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = a._1; r._2 = 0.f; r._3 = -b._3; return r; }
   _e1_e2      operator * (_e1 a, _e0_e1e2 b) { _e1_e2 r; r._1 = (a._1 * b._0); r._2 = (a._1 * b._3); return r; }
//    _e1_e2      inner      (_e1 a, _e0_e1e2 b) { _e1_e2 r; r._1 = (a._1 * b._0); r._2 = (a._1 * b._3); return r; } // |1-0| = 1; |1-2| = 1
//    _e1         outer      (_e1 a, _e0_e1e2 b) { _e1 r; r._1 = (a._1 * b._0); return r; } // (1+0) = 1; (1+2) = 3

// _e0_e1_e2_e1e2 operator + (_e1 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 =  b._0; r._1 = (a._1 + b._1); r._2 =  b._2; r._3 =  b._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e1 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = (a._1 - b._1); r._2 = -b._2; r._3 = -b._3; return r; }
// _e0_e1_e2_e1e2 operator * (_e1 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._1 * b._1); r._1 = (a._1 * b._0); r._2 = (a._1 * b._3); r._3 = (a._1 * b._2); return r; }
// _e0_e1_e2_e1e2 inner      (_e1 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._1 * b._1); r._1 = (a._1 * b._0); r._2 = (a._1 * b._3); r._3 = 0.f; return r; } // |1-0| = 1; |1-1| = 0; |1-2| = 1
// _e0_e1_e2_e1e2 outer      (_e1 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = (a._1 * b._0); r._2 = 0.f; r._3 = (a._1 * b._2); return r; } // (1+0) = 1; (1+1) = 2; (1+2) = 3

//    _e1         operator - (_e1  a) { _e1 r; r._1 = -a._1; return r; }
// void           conjugate  (_e1& a) {}
//    _e1         conjugated (_e1  a) { return a; }
// void           normalize  (_e1& a) { a._1 = math::sign(a._1); }
//    _e1         normalized (_e1  a) { normalize(a); return a; }
// _e0            length_squared(_e1 a) { return (a._1 * a._1); }
// _e0            length     (_e1 a) { return math::absolute(a._1); }
// _e0            grade_0    (_e1 a) { return 0.f; }
//    _e1_e2      grade_1    (_e1 a) { _e1_e2 r; r._1 = a._1; r._2 = 0.f; return r; }
//          _e1e2 grade_2    (_e1 a) { _e1e2 r; r._3 = 0.f; return r; }

// //======================== _e2 ========================//

// bool           operator == (_e2 a, _e2 b) { return (a._2 == b._2); }
// bool           operator != (_e2 a, _e2 b) { return (a._2 != b._2); }

// _e0_e1_e2_e1e2 operator + (_e2 a, _e0 b) { _e0_e1_e2_e1e2 r; r._0 =  b; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; return r; }
// _e0_e1_e2_e1e2 operator - (_e2 a, _e0 b) { _e0_e1_e2_e1e2 r; r._0 = -b; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; return r; }
//       _e2      operator * (_e2 a, _e0 b) { _e2 r; r._2 = (a._2 * b); return r; }
//       _e2      operator / (_e2 a, _e0 b) { _e2 r; r._2 = (a._2 / b); return r; }
//       _e2      inner      (_e2 a, _e0 b) { _e2 r; r._2 = (a._2 * b); return r; } // |1-0| = 1
//       _e2      outer      (_e2 a, _e0 b) { _e2 r; r._2 = (a._2 * b); return r; } // (1+0) = 1

//    _e1_e2      operator + (_e2 a, _e1 b) { _e1_e2 r; r._1 =  b._1; r._2 = a._2; return r; }
//    _e1_e2      operator - (_e2 a, _e1 b) { _e1_e2 r; r._1 = -b._1; r._2 = a._2; return r; }
//          _e1e2 operator * (_e2 a, _e1 b) { _e1e2 r; r._3 = -(a._2 * b._1); return r; }
// _e0            inner      (_e2 a, _e1 b) { return 0.f; } // |1-1| = 0
//          _e1e2 outer      (_e2 a, _e1 b) { _e1e2 r; r._3 = -(a._2 * b._1); return r; } // (1+1) = 2

//       _e2      operator + (_e2 a, _e2 b) { _e2 r; r._2 = (a._2 + b._2); return r; }
//       _e2      operator - (_e2 a, _e2 b) { _e2 r; r._2 = (a._2 - b._2); return r; }
// _e0            operator * (_e2 a, _e2 b) { return (a._2 * b._2); }
// _e0            inner      (_e2 a, _e2 b) { return (a._2 * b._2); } // |1-1| = 0
//          _e1e2 outer      (_e2 a, _e2 b) { _e1e2 r; r._3 = 0.f; return r; } // (1+1) = 2

//    _e1_e2      operator + (_e2 a, _e1_e2 b) { _e1_e2 r; r._1 =  b._1; r._2 = (a._2 + b._2); return r; }
//    _e1_e2      operator - (_e2 a, _e1_e2 b) { _e1_e2 r; r._1 = -b._1; r._2 = (a._2 - b._2); return r; }
// _e0_______e1e2 operator * (_e2 a, _e1_e2 b) { _e0_e1e2 r; r._0 = (a._2 * b._2); r._3 = -(a._2 * b._1); return r; }
// _e0            inner      (_e2 a, _e1_e2 b) { return (a._2 * b._2); } // |1-1| = 0; |1-1| = 0
//          _e1e2 outer      (_e2 a, _e1_e2 b) { _e1e2 r; r._3 = (a._2 * b._1); return r; } // (1+1) = 2; (1+1) = 2

// _e0_e1_e2_e1e2 operator + (_e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 =  b._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 = -b._3; return r; }
//    _e1         operator * (_e2 a, _e1e2 b) { _e1 r; r._1 = -(a._2 * b._3); return r; }
//    _e1_e2      inner      (_e2 a, _e1e2 b) { _e1_e2 r; r._1 = 0.f; r._2 = 0.f; return r; } // |1-2| = 1
// _e0            outer      (_e2 a, _e1e2 b) { return 0.f; } // (1+2) = 3

// _e0_e1_e2_e1e2 operator + (_e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 =  b._0; r._1 = 0.f; r._2 = a._2; r._3 =  b._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = 0.f; r._2 = a._2; r._3 = -b._3; return r; }
//    _e1_e2      operator * (_e2 a, _e0_e1e2 b) { _e1_e2 r; r._1 = -(a._2 * b._3); r._2 = (a._2 * b._0); return r; }
//    _e1_e2      inner      (_e2 a, _e0_e1e2 b) { _e1_e2 r; r._1 = -(a._2 * b._3); r._2 = (a._2 * b._0); return r; } // |1-0| = 1; |1-2| = 1
//    _e1_e2      outer      (_e2 a, _e0_e1e2 b) { _e1_e2 r; r._1 = 0.f; r._2 = (a._2 * b._0); return r; } // (1+0) = 1; (1+2) = 3

// _e0_e1_e2_e1e2 operator + (_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 =  b._0; r._1 =  b._1; r._2 = (a._2 + b._2); r._3 =  b._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = -b._1; r._2 = (a._2 - b._2); r._3 = -b._3; return r; }
// _e0_e1_e2_e1e2 operator * (_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._2 * b._2); r._1 = -(a._2 * b._3); r._2 = (r._2 * b._0); r._3 = -(a._2 * b._1); return r; }
// _e0_e1_e2_e1e2 inner      (_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._2 * b._2); r._1 = -(a._2 * b._3); r._2 = 0.f; r._3 = 0.f; return r; } // |1-0| = 1; |1-1| = 0; |1-1| = 0; |1-2| = 1;
// _e0_e1_e2_e1e2 outer      (_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._2 = (a._2 * b._0); r._3 = -(a._2 * b._1); return r; } // (1+0) = 1; (1+1) = 2; (1+1) = 2; (1+2) = 3;

//       _e2      operator - (_e2  a) { _e2 r; r._2 = -a._2; return r; }
// void           conjugate  (_e2& a) {}
//       _e2      conjugated (_e2  a) { return a; }
// void           normalize  (_e2& a) { a._2 = math::sign(a._2); }
//       _e2      normalized (_e2  a) { normalize(a); return a; }
// _e0            length_squared(_e2 a) { return (a._2 * a._2); }
// _e0            length     (_e2 a) { return math::absolute(a._2); }
// _e0            grade_0    (_e2 a) { return 0.f; }
//    _e1_e2      grade_1    (_e2 a) { _e1_e2 r; r._1 = 0.f; r._2 = a._2; return r; }
//          _e1e2 grade_2    (_e2 a) { _e1e2 r; r._3 = 0.f; return r; }

// //======================== _e1_e2 =====================//

// bool           operator == (_e1_e2 a, _e1_e2 b) { return (a._1 == b._1) && (a._2 == b._2); }
// bool           operator != (_e1_e2 a, _e1_e2 b) { return (a._1 != b._1) || (a._2 != b._2); }

// _e0_e1_e2_e1e2 operator + (_e1_e2 a, _e0 b) { _e0_e1_e2_e1e2 r; r._0 =  b; r._1 = a._1; r._2 = a._2; r._3 = 0.f; return r; }
// _e0_e1_e2_e1e2 operator - (_e1_e2 a, _e0 b) { _e0_e1_e2_e1e2 r; r._0 = -b; r._1 = a._1; r._2 = a._2; r._3 = 0.f; return r; }
//    _e1_e2      operator * (_e1_e2 a, _e0 b) { _e1_e2 r; r._1 = (a._1 * b); r._2 = (a._2 * b); return r; }
//    _e1_e2      operator / (_e1_e2 a, _e0 b) { _e1_e2 r; r._1 = (a._1 / b); r._2 = (a._2 / b); return r; }
//    _e1_e2      inner      (_e1_e2 a, _e0 b) { _e1_e2 r; r._1 = (a._1 * b); r._2 = (a._2 * b); return r; } // |1-0| = 1
//    _e1_e2      outer      (_e1_e2 a, _e0 b) { _e1_e2 r; r._1 = (a._1 * b); r._2 = (a._2 * b); return r; } // (1+0) = 1

//    _e1_e2      operator + (_e1_e2 a, _e1 b) { _e1_e2 r; r._1 = (a._1 + b._1); r._2 = a._2; return r; }
//    _e1_e2      operator - (_e1_e2 a, _e1 b) { _e1_e2 r; r._1 = (a._1 - b._1); r._2 = a._2; return r; }
// _e0_______e1e2 operator * (_e1_e2 a, _e1 b) { _e0_e1e2 r; r._0 = (a._1 * b._1); r._3 = -(a._2 * b._1); return r; }
// _e0            inner(_e1_e2 a, _e1 b) { return (a._1 * b._1); } // |1-1| = 0
//          _e1e2 outer(_e1_e2 a, _e1 b) { _e1e2 r; r._3 = -(a._2 * b._1); return r; } // (1+1) = 2; (1+1) = 2

//    _e1_e2      operator + (_e1_e2 a, _e2 b) { _e1_e2 r; r._1 = a._1; r._2 = (a._2 + b._2); return r; }
//    _e1_e2      operator - (_e1_e2 a, _e2 b) { _e1_e2 r; r._1 = a._1; r._2 = (a._2 - b._2); return r; }
// _e0_______e1e2 operator * (_e1_e2 a, _e2 b) { _e0_e1e2 r; r._0 = (a._2 * b._2); r._3 = (a._1 * b._2); return r; }
// _e0            inner      (_e1_e2 a, _e2 b) { return (a._2 * b._2); } // |1-1| = 0
//          _e1e2 outer      (_e1_e2 a, _e2 b) { _e1e2 r; r._3 = (a._1 * b._2); return r; } // (1+1) = 2; (1+1) = 2

//    _e1_e2      operator + (_e1_e2 a, _e1_e2 b) { _e1_e2 r; r._1 = (a._1 + b._1); r._2 = (a._2 + b._2); return r; }
   _e1_e2      operator - (_e1_e2 a, _e1_e2 b) { _e1_e2 r; r._1 = (a._1 - b._1); r._2 = (a._2 - b._2); return r; }
// _e0_______e1e2 operator * (_e1_e2 a, _e1_e2 b) { _e0_e1e2 r; r._0 = (a._1 * b._1) + (a._2 * b._2); r._3 = (a._1 * b._2) - (a._2 * b._1); return r; }
_e0            inner      (_e1_e2 a, _e1_e2 b) { return (a._1 * b._1) + (a._2 * b._2); } // |1-1| = 0
//          _e1e2 outer      (_e1_e2 a, _e1_e2 b) { _e1e2 r; r._3 = (a._1 * b._2) - (a._2 * b._1); return r; } // (1+1) = 2

// _e0_e1_e2_e1e2 operator + (_e1_e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = a._1; r._2 = a._2; r._3 =  b._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e1_e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = a._1; r._2 = a._2; r._3 = -b._3; return r; }
   _e1_e2      operator * (_e1_e2 a, _e1e2 b) { _e1_e2 r; r._1 = -(a._2 * b._3); r._2 = (a._1 * b._3); return r; }
//    _e1_e2      inner      (_e1_e2 a, _e1e2 b) { _e1_e2 r; r._1 = -(a._2 * b._3); r._2 = (a._1 * b._3); return r; } // |1-2| = 1; |1-2| = 1
// _e0            outer      (_e1_e2 a, _e1e2 b) { return 0.f; } // (1+2) = 3

// _e0_e1_e2_e1e2 operator + (_e1_e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 =  b._0; r._1 = a._1; r._2 = a._2; r._3 =  b._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e1_e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = a._1; r._2 = a._2; r._3 = -b._3; return r; }
   _e1_e2      operator * (_e1_e2 a, _e0_e1e2 b) { _e1_e2 r; r._1 = (a._1 * b._0) - (a._2 * b._3); r._2 = (a._1 * b._3) + (a._2 * b._0); return r; }
//    _e1_e2      inner      (_e1_e2 a, _e0_e1e2 b) { _e1_e2 r; r._1 = (a._1 * b._0) - (a._2 * b._3); r._2 = (a._2 * b._0) + (a._1 * b._3); return r; } // |1-0| = 1; |1-2| = 1
// _e0_e1_e2_e1e2 outer      (_e1_e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._1 = a._1 * b._0; r._2 = a._2 * b._0; return r; } // (1+0) = 1; (1+2) = 3

// _e0_e1_e2_e1e2 operator + (_e1_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 =  b._0; r._1 = (a._1 + b._1); r._2 = (a._2 + b._2); r._3 =  b._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e1_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = (a._1 - b._1); r._2 = (a._2 - b._2); r._3 = -b._3; return r; }
// _e0_e1_e2_e1e2 operator * (_e1_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._1 * b._1) + (a._2 * b._2); r._1 = (a._1 * b._0) - (a._2 * b._3); r._2 = (a._1 * b._3) + (a._2 * b._0); r._3 = (a._1 * b._2) - (a._2 * b._1); return r; }
// _e0_e1_e2_e1e2 inner      (_e1_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._1 * b._1) + (a._2 * b._2); r._1 = (a._1 * b._0) - (a._2 * b._3); r._2 = (a._2 * b._0) + (a._1 * b._3); r._3 = 0.f; return r; } // |1-0| = 1; |1-1| = 0; |1-2| = 1
// _e0_e1_e2_e1e2 outer      (_e1_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = (a._1 * b._0); r._2 = (a._2 * b._0); r._3 = (a._1 * b._2) - (a._2 * b._1); return r; } // (1+0) = 1; (1+1) = 2; (1+2) = 3

   _e1_e2      operator - (_e1_e2  a) { _e1_e2 r; r._1 = -a._1; r._2 = -a._2; return r; }
// void           conjugate  (_e1_e2& a) {}
//    _e1_e2      conjugated (_e1_e2  a) { return a; }
// void           normalize  (_e1_e2& a) { _e0 norm = math::square_root(inner(a, a)); a._1 = (a._1 / norm); a._2 = (a._2 / norm); }
//    _e1_e2      normalized (_e1_e2  a) { normalize(a); return a; }
// _e0            length_squared(_e1_e2 a) { return (a._1*a._1) + (a._2*a._2); }
// _e0            length     (_e1_e2 a) { return math::square_root(a._1*a._1 + a._2*a._2); }
// _e0            grade_0    (_e1_e2 a) { return 0.f; }
//    _e1_e2      grade_1    (_e1_e2 a) { _e1_e2 r; r._1 = a._1; r._2 = a._2; return r; }
//          _e1e2 grade_2    (_e1_e2 a) { _e1e2 r; r._3 = 0.f; return r; }

// //======================== _e1e2 ======================//

// bool           operator == (_e1e2 a, _e1e2 b) { return (a._3 == b._3); }
// bool           operator != (_e1e2 a, _e1e2 b) { return (a._3 != b._3); }

// _e0_______e1e2 operator + (_e1e2 a, _e0 b) { _e0_e1e2 r; r._0 =  b; r._3 = a._3; return r; }
// _e0_______e1e2 operator - (_e1e2 a, _e0 b) { _e0_e1e2 r; r._0 = -b; r._3 = a._3; return r; }
//          _e1e2 operator * (_e1e2 a, _e0 b) { _e1e2 r; r._3 = (a._3 * b); return r; }
//          _e1e2 operator / (_e1e2 a, _e0 b) { _e1e2 r; r._3 = (a._3 / b); return r; }
//          _e1e2 inner      (_e1e2 a, _e0 b) { _e1e2 r; r._3 = (a._3 * b); return r; } // |2-0| = 2
//          _e1e2 outer      (_e1e2 a, _e0 b) { _e1e2 r; r._3 = (a._3 * b); return r; } // (2+0) = 2

// _e0_e1_e2_e1e2 operator + (_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 =  b._1; r._2 = 0.f; r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = -b._1; r._2 = 0.f; r._3 = a._3; return r; }
//       _e2      operator * (_e1e2 a, _e1 b) { _e2 r; r._2 = -(a._3 * b._1); return r; }
//       _e2      inner      (_e1e2 a, _e1 b) { _e2 r; r._2 = -(a._3 * b._1); return r; } // |2-1| = 1
// _e0            outer      (_e1e2 a, _e1 b) { return 0.f; } // (2 + 1) = 3

// _e0_e1_e2_e1e2 operator + (_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = 0.f; r._2 =  b._2; r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = 0.f; r._2 = -b._2; r._3 = a._3; return r; }
//    _e1         operator * (_e1e2 a, _e2 b) { _e1 r; r._1 = (a._3 * b._2); return r; }
//    _e1         inner      (_e1e2 a, _e2 b) { _e1 r; r._1 = (a._3 * b._2); return r; } // |2-1| = 1
// _e0            outer      (_e1e2 a, _e2 b) { return 0.f; } // (2+1) = 3

// _e0_e1_e2_e1e2 operator + (_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 =  b._1; r._2 =  b._2; r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = -b._1; r._2 = -b._2; r._3 = a._3; return r; }
//    _e1_e2      operator * (_e1e2 a, _e1_e2 b) { _e1_e2 r; r._1 = (a._3 * b._2); r._2 = -(a._3 * b._1); return r; }
//    _e1_e2      inner      (_e1e2 a, _e1_e2 b) { _e1_e2 r; r._1 = (a._3 * b._2); r._2 = -(a._3 * b._1); return r; } // |2-1| = 1
// _e0            outer      (_e1e2 a, _e1_e2 b) { return 0.f; } // (2+1) = 3

//          _e1e2 operator + (_e1e2 a, _e1e2 b) { _e1e2 r; r._3 = (a._3 + b._3); return r; }
//          _e1e2 operator - (_e1e2 a, _e1e2 b) { _e1e2 r; r._3 = (a._3 - b._3); return r; }
// _e0            operator * (_e1e2 a, _e1e2 b) { return -(a._3 * b._3); }
// _e0            inner      (_e1e2 a, _e1e2 b) { return -(a._3 * b._3); } // |2-2| = 0
// _e0            outer      (_e1e2 a, _e1e2 b) { return 0.f; } // (2+2) = 4

// _e0_______e1e2 operator + (_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = b._0; r._3 = (a._3 + b._3); return r; }
// _e0_______e1e2 operator - (_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = b._0; r._3 = (a._3 - b._3); return r; }
// _e0_______e1e2 operator * (_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = -(a._3 * b._3); r._3 = (a._3 * b._0); return r; }
// _e0_______e1e2 inner      (_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = -(a._3 * b._3); r._3 = (a._3 * b._0); return r; } // |2-0| = 2; |2-2| = 0
//          _e1e2 outer      (_e1e2 a, _e0_e1e2 b) { _e1e2 r; r._3 = (a._3 * b._0); return r; } // (2+0) = 2; (2+2) = 4

// _e0_e1_e2_e1e2 operator + (_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 =  b._0; r._1 =  b._1; r._2 =  b._2; r._3 = (a._3 + b._3); return r; }
// _e0_e1_e2_e1e2 operator - (_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = -b._1; r._2 = -b._2; r._3 = (a._3 - b._3); return r; }
// _e0_e1_e2_e1e2 operator * (_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -(a._3 * b._3); r._1 = (a._3 * b._2); r._2 = -(a._3 * b._1); r._3 = (a._3 * b._0); return r; }
// _e0_e1_e2_e1e2 inner      (_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -(a._3 * b._3); r._1 = (a._3 * b._2); r._2 = -(a._3 * b._1); r._3 = (a._3 * b._0); return r; } // |2-0| = 2; |2-1| = 1; |2-2| = 0
//          _e1e2 outer      (_e1e2 a, _e0_e1_e2_e1e2 b) { _e1e2 r; r._3 = (a._3 * b._0); return r; } // (2+0) = 2; 3, 4...

//          _e1e2 operator - (_e1e2  a) { _e1e2 r; r._3 = -a._3; return r; }
// void           conjugate  (_e1e2& a) { a._3 = -a._3; }
//          _e1e2 conjugated (_e1e2  a) { conjugate(a); return a; }
// void           normalize  (_e1e2& a) { a._3 = math::sign(a._3); }
//          _e1e2 normalized (_e1e2  a) { normalize(a); return a; }
// _e0            length_squared(_e1e2 a) { return (a._3*a._3); }
// _e0            length     (_e1e2 a) { return math::absolute(a._3); }
// _e0            grade_0    (_e1e2 a) { return 0.f; }
//    _e1_e2      grade_1    (_e1e2 a) { _e1_e2 r; r._1 = 0.f; r._2 = 0.f; return r; }
//          _e1e2 grade_2    (_e1e2 a) { _e1e2 r; r._3 = a._3; return r; }

// //======================== _e0_e1e2 ===================//

// bool           operator == (_e0_e1e2 a, _e0_e1e2 b) { return (a._0 == b._0) && (a._3 == b._3); }
// bool           operator != (_e0_e1e2 a, _e0_e1e2 b) { return (a._0 != b._0) || (a._3 != b._3); }

// _e0_______e1e2 operator + (_e0_e1e2 a, _e0 b) { _e0_e1e2 r; r._0 = (a._0 + b); r._3 = a._3; return r; }
// _e0_______e1e2 operator - (_e0_e1e2 a, _e0 b) { _e0_e1e2 r; r._0 = (a._0 - b); r._3 = a._3; return r; }
_e0_______e1e2 operator * (_e0_e1e2 a, _e0 b) { _e0_e1e2 r; r._0 = (a._0 * b); r._3 = (a._3 * b); return r; }
_e0_______e1e2 operator / (_e0_e1e2 a, _e0 b) { _e0_e1e2 r; r._0 = (a._0 / b); r._3 = (a._3 / b); return r; }
// _e0_______e1e2 inner      (_e0_e1e2 a, _e0 b) { _e0_e1e2 r; r._0 = (a._0 * b); r._3 = (a._3 * b); return r; } // |0-0| = 0; |2-0| = 2
// _e0_______e1e2 outer      (_e0_e1e2 a, _e0 b) { _e0_e1e2 r; r._0 = (a._0 * b); r._3 = (a._3 * b); return r; } // (0+0) = 0; (2+0) = 2

// _e0_e1_e2_e1e2 operator + (_e0_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 =  b._1; r._2 = 0.f; r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e0_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = -b._1; r._2 = 0.f; r._3 = a._3; return r; }
//    _e1_e2      operator * (_e0_e1e2 a, _e1 b) { _e1_e2 r; r._1 = (a._0 * b._1); r._2 = -(a._3 * b._1); return r; }
//    _e1_e2      inner      (_e0_e1e2 a, _e1 b) { _e1_e2 r; r._1 = (a._0 * b._1); r._2 = -(a._3 * b._1); return r; } // |0-1| = 1; |2-1| = 1
//    _e1         outer      (_e0_e1e2 a, _e1 b) { _e1 r; r._1 = (a._0 * b._1); return r; } // (0+1) = 1; (2+1) = 3

// _e0_e1_e2_e1e2 operator + (_e0_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = 0.f; r._2 =  b._2; r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e0_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = 0.f; r._2 = -b._2; r._3 = a._3; return r; }
//    _e1_e2      operator * (_e0_e1e2 a, _e2 b) { _e1_e2 r; r._1 = (a._3 * b._2); r._2 = (a._0 * b._2); return r; }
//    _e1_e2      inner      (_e0_e1e2 a, _e2 b) { _e1_e2 r; r._1 = (a._3 * b._2); r._2 = (a._0 * b._2); return r; } // |0-1| = 1; |2-1| = 1
//       _e2      outer      (_e0_e1e2 a, _e2 b) { _e2 r; r._2 = (a._0 * b._2); return r; } // (0+1) = 1; (2+1) = 3

// _e0_e1_e2_e1e2 operator + (_e0_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 =  b._1; r._2 =  b._2; r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e0_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = -b._1; r._2 = -b._2; r._3 = a._3; return r; }
//    _e1_e2      operator * (_e0_e1e2 a, _e1_e2 b) { _e1_e2 r; r._1 = (a._0 * b._1) + (a._3 * b._2); r._2 = (a._0 * b._2) - (a._3 * b._1); return r; }
//    _e1_e2      inner      (_e0_e1e2 a, _e1_e2 b) { _e1_e2 r; r._1 = (a._0 * b._1) + (a._3 * b._2); r._2 = (a._0 * b._2) - (a._3 * b._1); return r; } // |0-1| = 1; |2-1| = 1
//    _e1_e2      outer      (_e0_e1e2 a, _e1_e2 b) { _e1_e2 r; r._1 = (a._0 * b._1); r._2 = (a._0 * b._2); return r; } // (0+1) = 1; (2+1) = 3

// _e0_______e1e2 operator + (_e0_e1e2 a, _e1e2 b) { _e0_e1e2 r; r._0 = a._0; r._3 = (a._3 + b._3); return r; }
// _e0_______e1e2 operator - (_e0_e1e2 a, _e1e2 b) { _e0_e1e2 r; r._0 = a._0; r._3 = (a._3 - b._3); return r; }
_e0_______e1e2 operator * (_e0_e1e2 a, _e1e2 b) { _e0_e1e2 r; r._0 = -(a._3 * b._3); r._3 = (a._0 * b._3); return r; }
// _e0_______e1e2 inner      (_e0_e1e2 a, _e1e2 b) { _e0_e1e2 r; r._0 = -(a._3 * b._3); r._3 = (a._0 * b._3); return r; } // |0-2| = 2; |2-2| = 0
//          _e1e2 outer      (_e0_e1e2 a, _e1e2 b) { _e1e2 r; r._3 = (a._0 * b._3); return r; } // (0+2) = 2; (2+2) = 4

// _e0_______e1e2 operator + (_e0_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = (a._0 + b._0); r._3 = (a._3 + b._3); return r; }
// _e0_______e1e2 operator - (_e0_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = (a._0 - b._0); r._3 = (a._3 - b._3); return r; }
_e0_______e1e2 operator * (_e0_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = (a._0 * b._0) - (a._3 * b._3); r._3 = (a._0 * b._3) + (a._3 * b._0); return r; }
// _e0_______e1e2 inner      (_e0_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = (a._0 * b._0) - (a._3 * b._3); r._3 = (a._0 * b._3) + (a._3 * b._0); return r; } // |0-0| = 0; |0-2| = 2; |2-2| = 0
// _e0_______e1e2 outer      (_e0_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = (a._0 * b._0); r._3 = (a._0 * b._3) + (a._3 * b._0); return r; } // (0+0) = 0; (0+2) = 2; (2+2) = 4

// _e0_e1_e2_e1e2 operator + (_e0_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 + b._0); r._1 =  b._1; r._2 =  b._2; r._3 = (a._3 + b._3); return r; }
// _e0_e1_e2_e1e2 operator - (_e0_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 - b._0); r._1 = -b._1; r._2 = -b._2; r._3 = (a._3 - b._3); return r; }
// _e0_e1_e2_e1e2 operator * (_e0_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 * b._0) - (a._3 * b._3); r._1 = (a._0 * b._1) + (a._3 * b._2); r._2 = (a._0 * b._2) - (a._3 * b._1); r._3 = (a._0 * b._3) + (a._3 * b._0); return r; }
// _e0_e1_e2_e1e2 inner      (_e0_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 * b._0) - (a._3 * b._3); r._1 = (a._0 * b._1) + (a._3 * b._2); r._2 = (a._0 * b._2) - (a._3 * b._1); r._3 = (a._0 * b._3) + (a._3 * b._0); return r; } // |0-0| = 0; |0-1| = 1; |0-2| = 2; |2-0| = 2; |2-1| = 1; |2-2| = 0
// _e0_e1_e2_e1e2 outer      (_e0_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 * b._0); r._1 = (a._0 * b._1); r._2 = (a._0 * b._2); r._3 = (a._0 * b._3) + (a._3 * b._0); return r; }


// _e0_______e1e2 operator - (_e0_e1e2  a) { _e0_e1e2 r; r._0 = -a._0; r._3 = -a._3; return r; }
void           conjugate  (_e0_e1e2& a) { a._3 = -a._3; }
_e0_______e1e2 conjugated (_e0_e1e2  a) { conjugate(a); return a; }
// void           normalize  (_e0_e1e2& a) { _e0 norm = math::square_root(a._0*a._0 + a._3*a._3); a._0 = (a._0 / norm); a._3 = (a._3 / norm); }
// _e0_______e1e2 normalized (_e0_e1e2  a) { normalize(a); return a; }
_e0            length_squared(_e0_e1e2 a) { return (a._0*a._0) + (a._3*a._3); }
// _e0            length     (_e0_e1e2 a) { return math::square_root(a._0*a._0 + a._3*a._3); }
// _e0            grade_0    (_e0_e1e2 a) { return a._0; }
//    _e1_e2      grade_1    (_e0_e1e2 a) { _e1_e2 r; r._1 = 0.f; r._2 = 0.f; return r; }
//          _e1e2 grade_2    (_e0_e1e2 a) { _e1e2 r; r._3 = a._3; return r; }

// //======================== _e0_e1_e2_e1e2 =============//

// bool           operator == (_e0_e1_e2_e1e2 a, _e0_e1_e2_e1e2 b) { return (a._0 == b._0) && (a._1 == b._1) && (a._2 == b._2) && (a._3 == b._3); }
// bool           operator != (_e0_e1_e2_e1e2 a, _e0_e1_e2_e1e2 b) { return (a._0 != b._0) || (a._1 != b._1) || (a._2 != b._2) || (a._3 != b._3); }

// _e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, _e0 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 + b); r._1 = a._1; r._2 = a._2; r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, _e0 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 - b); r._1 = a._1; r._2 = a._2; r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, _e0 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 * b); r._1 = (a._1 * b); r._2 = (a._2 * b); r._3 = (a._3 * b); return r; }
// _e0_e1_e2_e1e2 inner      (_e0_e1_e2_e1e2 a, _e0 b) { _e0_e1_e2_e1e2 r; r._0 = a._0 * b; r._1 = a._1 * b; r._2 = a._2 * b; r._3 = a._3 * b; return r; } // |0-0| = 0; |1-0| = 1; ...
// _e0_e1_e2_e1e2 outer      (_e0_e1_e2_e1e2 a, _e0 b) { _e0_e1_e2_e1e2 r; r._0 = a._0 * b; r._1 = a._1 * b; r._2 = a._2 * b; r._3 = a._3 * b; return r; } // (0+0) = 0; (1+0) = 1; (2+0) = 2;

// _e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = (a._1 + b._1); r._2 = a._2; r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = (a._1 - b._1); r._2 = a._2; r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = (a._1 * b._1); r._1 = (a._0 * b._1); r._2 = -(a._3 * b._1); r._3 = -(a._2 * b._1); return r; }
// _e0_e1_e2_e1e2 inner      (_e0_e1_e2_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = (a._1 * b._1); r._1 = (a._0 * b._1) - (a._3 * b._1); r._2 = 0.f; r._3 = 0.f; return r; } // |0-1| = 1; |1-1| = 0; |2-1| = 1
// _e0_e1_e2_e1e2 outer      (_e0_e1_e2_e1e2 a, _e1 b) {
//    _e0_e1_e2_e1e2 r;
//    r._0 = 0.f;
//    r._1 = (a._0 * b._1);
//    r._2 = 0.f;
//    r._3 = -(a._2 * b._1);
//    return r;
// } // (0+1) = 1; (1+1) = 2; (2+1) = 3

// _e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = a._1; r._2 = (a._2 + b._2); r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = a._1; r._2 = (a._2 - b._2); r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._2 * b._2); r._1 = (a._3 * b._2); r._2 = (a._0 * b._2); r._3 = (a._1 * b._2); return r; }
// _e0_e1_e2_e1e2 inner      (_e0_e1_e2_e1e2 a, _e2 b) {
//    _e0_e1_e2_e1e2 r;
//    r._0 = (a._2 * b._2);
//    r._1 = (a._3 * b._2);
//    r._2 = (a._0 * b._2);
//    r._3 = 0.f;
//    return r;
// }
// // |0-1| = 1; |1-1| = 0; |2-1| = 1;
// _e0_e1_e2_e1e2 outer(_e0_e1_e2_e1e2 a, _e2 b) {
//    _e0_e1_e2_e1e2 r;
//    r._0 = 0.f;
//    r._1 = 0.f;
//    r._2 = (a._0 * b._2);
//    r._3 = (a._1 * b._2);
//    return r;
// }
// // (0+1) = 1; (1+1) = 2;
// _e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = (a._1 + b._1); r._2 = (a._2 + b._2); r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = (a._1 - b._1); r._2 = (a._2 - b._2); r._3 = a._3; return r; }
// _e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._1 * b._1) + (a._2 * b._2); r._1 = (a._0 * b._1) + (a._3 * b._2); r._2 = (a._0 * b._2) - (a._3 * b._1); r._3 = (a._1 * b._2) - (a._2 * b._1); return r; }
// _e0_e1_e2_e1e2 inner      (_e0_e1_e2_e1e2 a, _e1_e2 b) {
//    _e0_e1_e2_e1e2 r;
//    r._0 = (a._1 * b._1) + (a._2 * b._2);
//    r._1 = (a._0 * b._1) + (a._3 * b._2);
//    r._2 = (a._0 * b._2) - (a._3 * b._1);
//    r._3 = 0.f;
//    return r;
// }
// // |0-1| = 1; |1-1| = 0; |2-1| = 1;
// _e0_e1_e2_e1e2 outer      (_e0_e1_e2_e1e2 a, _e1_e2 b) {
//    _e0_e1_e2_e1e2 r;
//    r._0 = 0.f;
//    r._1 = (a._0 * b._1);
//    r._2 = (a._0 * b._2);
//    r._3 = (a._1 * b._2) - (a._2 * b._1);
//    return r;
// }
// // (0+1) = 1; (1+1) = 2;

// _e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = (a._3 + b._3); return r; }
// _e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = (a._3 - b._3); return r; }
// _e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -(a._3 * b._3); r._1 = -(a._2 * b._3); r._2 = (a._1 * b._3); r._3 = (a._0 * b._3); return r; }
// _e0 inner(_e0_e1_e2_e1e2 a, _e1e2 b) { return -(a._3 * b._3); }
// _e0_e1e2 outer(_e0_e1_e2_e1e2 a, _e1e2 b) { _e0_e1e2 r = {}; r._3 += a._0 * b._3; return r; }

// _e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 + b._0); r._1 = a._1; r._2 = a._2; r._3 = (a._3 + b._3); return r; }
// _e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 - b._0); r._1 = a._1; r._2 = a._2; r._3 = (a._3 - b._3); return r; }
// _e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 * b._0) - (a._3 * b._3); r._1 = (a._1 * b._0) - (a._2 * b._3); r._2 = (a._1 * b._3) + (a._2 * b._0); r._3 = (a._0 * b._3) + (a._3 * b._0); return r; }
// _e0 inner(_e0_e1_e2_e1e2 a, _e0_e1e2 b) { return (a._0 * b._0) - (a._3 * b._3); }
// _e0_e1_e2_e1e2 outer(_e0_e1_e2_e1e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._0 += a._0 * b._0; r._3 += a._0 * b._3; r._1 += a._1 * b._0; r._2 += a._2 * b._0; r._3 += a._3 * b._0; return r; }

// _e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 + b._0); r._1 = (a._1 + b._1); r._2 = (a._2 + b._2); r._3 = (a._3 + b._3); return r; }
// _e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 - b._0); r._1 = (a._1 - b._1); r._2 = (a._2 - b._2); r._3 = (a._3 - b._3); return r; }
// _e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = (a._0 * b._0) + (a._1 * b._1) + (a._2 * b._2) - (a._3 * b._3); r._1 = (a._0 * b._1) + (a._1 * b._0) - (a._2 * b._3) + (a._3 * b._2); r._2 = (a._0 * b._2) + (a._1 * b._3) + (a._2 * b._0) - (a._3 * b._1); r._3 = (a._0 * b._3) + (a._1 * b._2) - (a._2 * b._1) + (a._3 * b._0); return r; }
// _e0 inner(_e0_e1_e2_e1e2 a, _e0_e1_e2_e1e2 b) { return (a._0 * b._0) + (a._1 * b._1) + (a._2 * b._2) - (a._3 * b._3); }
// _e0_e1_e2_e1e2 outer(_e0_e1_e2_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._0 += a._0 * b._0; r._1 += a._0 * b._1; r._2 += a._0 * b._2; r._3 += a._0 * b._3; r._1 += a._1 * b._0; r._3 += a._1 * b._2; r._2 += a._2 * b._0; r._3 -= a._2 * b._1; r._3 += a._3 * b._0; return r; }

// _e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a) { _e0_e1_e2_e1e2 r; r._0 = -a._0; r._1 = -a._1; r._2 = -a._2; r._3 = -a._3; return r; }
// void           conjugate  (_e0_e1_e2_e1e2& a) { a._3 = -a._3; }
// _e0_e1_e2_e1e2 conjugated (_e0_e1_e2_e1e2 a) { conjugate(a); return a; }
// _e0            grade_0    (_e0_e1_e2_e1e2  a) { return a._0; }
//    _e1_e2      grade_1    (_e0_e1_e2_e1e2  a) { _e1_e2 r; r._1 = a._1; r._2 = a._2; return r; }
//          _e1e2 grade_2    (_e0_e1_e2_e1e2  a) { _e1e2 r; r._3 = a._3; return r; }

