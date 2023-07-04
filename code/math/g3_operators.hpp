_e1 operator - (_e1 a) { _e1 r; r._1 = -a._1; return r; }
_e2 operator - (_e2 a) { _e2 r; r._2 = -a._2; return r; }
_e3 operator - (_e3 a) { _e3 r; r._3 = -a._3; return r; }
_e1_e2_e3 operator - (_e1_e2_e3 a) { _e1_e2_e3 r; r._1 = -a._1; r._2 = -a._2; r._3 = -a._3; return r; }
_e1e2 operator - (_e1e2 a) { _e1e2 r; r._4 = -a._4; return r; }
_e2e3 operator - (_e2e3 a) { _e2e3 r; r._5 = -a._5; return r; }
_e3e1 operator - (_e3e1 a) { _e3e1 r; r._6 = -a._6; return r; }
_e0_e1e2_e2e3_e3e1 operator - (_e0_e1e2_e2e3_e3e1 a) { _e0_e1e2_e2e3_e3e1 r; r._0 = -a._0; r._4 = -a._4; r._5 = -a._5; r._6 = -a._6; return r; }
_e1e2e3 operator - (_e1e2e3 a) { _e1e2e3 r; r._7 = -a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = -a._0; r._1 = -a._1; r._2 = -a._2; r._3 = -a._3; r._4 = -a._4; r._5 = -a._5; r._6 = -a._6; r._7 = -a._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (float32 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = b._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (float32 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = 0.f; r._2 = b._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (float32 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = 0.f; r._2 = 0.f; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (float32 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (float32 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (float32 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = b._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (float32 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1e2_e2e3_e3e1 operator + (float32 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = a + b._0; r._4 = b._4; r._5 = b._5; r._6 = b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (float32 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (float32 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a + b._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e1 operator + (_e1 a, _e1 b) { _e1 r; r._1 = a._1 + b._1; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = b._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = 0.f; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e1_e2_e3 operator + (_e1 a, _e1_e2_e3 b) { _e1_e2_e3 r; r._1 = a._1 + b._1; r._2 = b._2; r._3 = b._3; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = b._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = a._1 + b._1; r._2 = b._2; r._3 = b._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = a._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e2 operator + (_e2 a, _e2 b) { _e2 r; r._2 = a._2 + b._2; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e1_e2_e3 operator + (_e2 a, _e1_e2_e3 b) { _e1_e2_e3 r; r._1 = b._1; r._2 = a._2 + b._2; r._3 = b._3; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; r._4 = b._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; r._4 = 0.f; r._5 = b._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = b._1; r._2 = a._2 + b._2; r._3 = b._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = 0.f; r._2 = 0.f; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = 0.f; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = b._2; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e3 operator + (_e3 a, _e3 b) { _e3 r; r._3 = a._3 + b._3; return r; }
_e1_e2_e3 operator + (_e3 a, _e1_e2_e3 b) { _e1_e2_e3 r; r._1 = b._1; r._2 = b._2; r._3 = a._3 + b._3; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = a._3; r._4 = b._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = a._3; r._4 = 0.f; r._5 = b._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = 0.f; r._2 = 0.f; r._3 = a._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = b._1; r._2 = b._2; r._3 = a._3 + b._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1_e2_e3 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e1_e2_e3 operator + (_e1_e2_e3 a, _e1 b) { _e1_e2_e3 r; r._1 = a._1 + b._1; r._2 = a._2; r._3 = a._3; return r; }
_e1_e2_e3 operator + (_e1_e2_e3 a, _e2 b) { _e1_e2_e3 r; r._1 = a._1; r._2 = a._2 + b._2; r._3 = a._3; return r; }
_e1_e2_e3 operator + (_e1_e2_e3 a, _e3 b) { _e1_e2_e3 r; r._1 = a._1; r._2 = a._2; r._3 = a._3 + b._3; return r; }
_e1_e2_e3 operator + (_e1_e2_e3 a, _e1_e2_e3 b) { _e1_e2_e3 r; r._1 = a._1 + b._1; r._2 = a._2 + b._2; r._3 = a._3 + b._3; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1_e2_e3 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = b._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1_e2_e3 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = 0.f; r._5 = b._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1_e2_e3 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1_e2_e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1_e2_e3 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1_e2_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = a._1 + b._1; r._2 = a._2 + b._2; r._3 = a._3 + b._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = b._2; r._3 = 0.f; r._4 = a._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = b._3; r._4 = a._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = a._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e1e2 operator + (_e1e2 a, _e1e2 b) { _e1e2 r; r._4 = a._4 + b._4; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = b._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = 0.f; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1e2_e2e3_e3e1 operator + (_e1e2 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = b._0; r._4 = a._4 + b._4; r._5 = b._5; r._6 = b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = 0.f; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = a._4 + b._4; r._5 = b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2e3 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = a._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2e3 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = a._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2e3 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = b._2; r._3 = 0.f; r._4 = 0.f; r._5 = a._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2e3 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = b._3; r._4 = 0.f; r._5 = a._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2e3 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = 0.f; r._5 = a._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2e3 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = a._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e2e3 operator + (_e2e3 a, _e2e3 b) { _e2e3 r; r._5 = a._5 + b._5; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2e3 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = a._5; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1e2_e2e3_e3e1 operator + (_e2e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = b._0; r._4 = b._4; r._5 = a._5 + b._5; r._6 = b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2e3 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = a._5; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = b._4; r._5 = a._5 + b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3e1 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3e1 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3e1 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = b._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3e1 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3e1 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3e1 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = 0.f; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3e1 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = b._5; r._6 = a._6; r._7 = 0.f; return r; }
_e3e1 operator + (_e3e1 a, _e3e1 b) { _e3e1 r; r._6 = a._6 + b._6; return r; }
_e0_e1e2_e2e3_e3e1 operator + (_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = b._0; r._4 = b._4; r._5 = b._5; r._6 = a._6 + b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3e1 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = a._6; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = b._4; r._5 = b._5; r._6 = a._6 + b._6; r._7 = b._7; return r; }

_e0_e1e2_e2e3_e3e1 operator + (_e0_e1e2_e2e3_e3e1 a, float32 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = a._0 + b; r._4 = a._4; r._5 = a._5; r._6 = a._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1e2_e2e3_e3e1 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = b._1; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1e2_e2e3_e3e1 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = 0.f; r._2 = b._2; r._3 = 0.f; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1e2_e2e3_e3e1 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = 0.f; r._2 = 0.f; r._3 = b._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1e2_e2e3_e3e1 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1e2_e2e3_e3e1 operator + (_e0_e1e2_e2e3_e3e1 a, _e1e2 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = a._0; r._4 = a._4 + b._4; r._5 = a._5; r._6 = a._6; return r; }
_e0_e1e2_e2e3_e3e1 operator + (_e0_e1e2_e2e3_e3e1 a, _e2e3 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = a._0; r._4 = a._4; r._5 = a._5 + b._5; r._6 = a._6; return r; }
_e0_e1e2_e2e3_e3e1 operator + (_e0_e1e2_e2e3_e3e1 a, _e3e1 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = a._0; r._4 = a._4; r._5 = a._5; r._6 = a._6 + b._6; return r; }
_e0_e1e2_e2e3_e3e1 operator + (_e0_e1e2_e2e3_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = a._0 + b._0; r._4 = a._4 + b._4; r._5 = a._5 + b._5; r._6 = a._6 + b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1e2_e2e3_e3e1 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1e2_e2e3_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0 + b._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = a._4 + b._4; r._5 = a._5 + b._5; r._6 = a._6 + b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2e3 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2e3 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2e3 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = b._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2e3 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2e3 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2e3 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = 0.f; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2e3 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = b._5; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2e3 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = b._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = a._7; return r; }
_e1e2e3 operator + (_e1e2e3 a, _e1e2e3 b) { _e1e2e3 r; r._7 = a._7 + b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = a._7 + b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0 + b; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1 + b._1; r._2 = a._2; r._3 = a._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1; r._2 = a._2 + b._2; r._3 = a._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = a._3 + b._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1 + b._1; r._2 = a._2 + b._2; r._3 = a._3 + b._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = a._4 + b._4; r._5 = a._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = a._4; r._5 = a._5 + b._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = a._4; r._5 = a._5; r._6 = a._6 + b._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0 + b._0; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = a._4 + b._4; r._5 = a._5 + b._5; r._6 = a._6 + b._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = a._7 + b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator + (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0 + b._0; r._1 = a._1 + b._1; r._2 = a._2 + b._2; r._3 = a._3 + b._3; r._4 = a._4 + b._4; r._5 = a._5 + b._5; r._6 = a._6 + b._6; r._7 = a._7 + b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (float32 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = b._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (float32 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = 0.f; r._2 = b._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (float32 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = 0.f; r._2 = 0.f; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (float32 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (float32 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (float32 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = b._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (float32 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1e2_e2e3_e3e1 operator - (float32 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = a - b._0; r._4 = b._4; r._5 = b._5; r._6 = b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (float32 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (float32 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a - b._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e1 operator - (_e1 a, _e1 b) { _e1 r; r._1 = a._1 - b._1; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = b._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = 0.f; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e1_e2_e3 operator - (_e1 a, _e1_e2_e3 b) { _e1_e2_e3 r; r._1 = a._1 - b._1; r._2 = b._2; r._3 = b._3; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = b._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = a._1 - b._1; r._2 = b._2; r._3 = b._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = a._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e2 operator - (_e2 a, _e2 b) { _e2 r; r._2 = a._2 - b._2; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e1_e2_e3 operator - (_e2 a, _e1_e2_e3 b) { _e1_e2_e3 r; r._1 = b._1; r._2 = a._2 - b._2; r._3 = b._3; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; r._4 = b._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; r._4 = 0.f; r._5 = b._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = b._1; r._2 = a._2 - b._2; r._3 = b._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = 0.f; r._2 = 0.f; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = 0.f; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = b._2; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e3 operator - (_e3 a, _e3 b) { _e3 r; r._3 = a._3 - b._3; return r; }
_e1_e2_e3 operator - (_e3 a, _e1_e2_e3 b) { _e1_e2_e3 r; r._1 = b._1; r._2 = b._2; r._3 = a._3 - b._3; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = a._3; r._4 = b._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = a._3; r._4 = 0.f; r._5 = b._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = 0.f; r._2 = 0.f; r._3 = a._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = b._1; r._2 = b._2; r._3 = a._3 - b._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1_e2_e3 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e1_e2_e3 operator - (_e1_e2_e3 a, _e1 b) { _e1_e2_e3 r; r._1 = a._1 - b._1; r._2 = a._2; r._3 = a._3; return r; }
_e1_e2_e3 operator - (_e1_e2_e3 a, _e2 b) { _e1_e2_e3 r; r._1 = a._1; r._2 = a._2 - b._2; r._3 = a._3; return r; }
_e1_e2_e3 operator - (_e1_e2_e3 a, _e3 b) { _e1_e2_e3 r; r._1 = a._1; r._2 = a._2; r._3 = a._3 - b._3; return r; }
_e1_e2_e3 operator - (_e1_e2_e3 a, _e1_e2_e3 b) { _e1_e2_e3 r; r._1 = a._1 - b._1; r._2 = a._2 - b._2; r._3 = a._3 - b._3; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1_e2_e3 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = b._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1_e2_e3 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = 0.f; r._5 = b._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1_e2_e3 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1_e2_e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1_e2_e3 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1_e2_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = a._1 - b._1; r._2 = a._2 - b._2; r._3 = a._3 - b._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = b._2; r._3 = 0.f; r._4 = a._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = b._3; r._4 = a._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = a._4; r._5 = 0.f; r._6 = 0.f; r._7 = 0.f; return r; }
_e1e2 operator - (_e1e2 a, _e1e2 b) { _e1e2 r; r._4 = a._4 - b._4; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = b._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = 0.f; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1e2_e2e3_e3e1 operator - (_e1e2 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = b._0; r._4 = a._4 - b._4; r._5 = b._5; r._6 = b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = 0.f; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = a._4 - b._4; r._5 = b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2e3 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = a._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2e3 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = a._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2e3 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = b._2; r._3 = 0.f; r._4 = 0.f; r._5 = a._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2e3 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = b._3; r._4 = 0.f; r._5 = a._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2e3 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = 0.f; r._5 = a._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2e3 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = a._5; r._6 = 0.f; r._7 = 0.f; return r; }
_e2e3 operator - (_e2e3 a, _e2e3 b) { _e2e3 r; r._5 = a._5 - b._5; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2e3 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = a._5; r._6 = b._6; r._7 = 0.f; return r; }
_e0_e1e2_e2e3_e3e1 operator - (_e2e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = b._0; r._4 = b._4; r._5 = a._5 - b._5; r._6 = b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2e3 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = a._5; r._6 = 0.f; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = b._4; r._5 = a._5 - b._5; r._6 = b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3e1 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3e1 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3e1 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = b._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3e1 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3e1 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3e1 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = 0.f; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3e1 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = b._5; r._6 = a._6; r._7 = 0.f; return r; }
_e3e1 operator - (_e3e1 a, _e3e1 b) { _e3e1 r; r._6 = a._6 - b._6; return r; }
_e0_e1e2_e2e3_e3e1 operator - (_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = b._0; r._4 = b._4; r._5 = b._5; r._6 = a._6 - b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3e1 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = a._6; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = b._4; r._5 = b._5; r._6 = a._6 - b._6; r._7 = b._7; return r; }

_e0_e1e2_e2e3_e3e1 operator - (_e0_e1e2_e2e3_e3e1 a, float32 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = a._0 - b; r._4 = a._4; r._5 = a._5; r._6 = a._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1e2_e2e3_e3e1 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = b._1; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1e2_e2e3_e3e1 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = 0.f; r._2 = b._2; r._3 = 0.f; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1e2_e2e3_e3e1 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = 0.f; r._2 = 0.f; r._3 = b._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1e2_e2e3_e3e1 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = 0.f; return r; }
_e0_e1e2_e2e3_e3e1 operator - (_e0_e1e2_e2e3_e3e1 a, _e1e2 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = a._0; r._4 = a._4 - b._4; r._5 = a._5; r._6 = a._6; return r; }
_e0_e1e2_e2e3_e3e1 operator - (_e0_e1e2_e2e3_e3e1 a, _e2e3 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = a._0; r._4 = a._4; r._5 = a._5 - b._5; r._6 = a._6; return r; }
_e0_e1e2_e2e3_e3e1 operator - (_e0_e1e2_e2e3_e3e1 a, _e3e1 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = a._0; r._4 = a._4; r._5 = a._5; r._6 = a._6 - b._6; return r; }
_e0_e1e2_e2e3_e3e1 operator - (_e0_e1e2_e2e3_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r; r._0 = a._0 - b._0; r._4 = a._4 - b._4; r._5 = a._5 - b._5; r._6 = a._6 - b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1e2_e2e3_e3e1 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1e2_e2e3_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0 - b._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = a._4 - b._4; r._5 = a._5 - b._5; r._6 = a._6 - b._6; r._7 = b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2e3 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2e3 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2e3 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = b._2; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2e3 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2e3 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = 0.f; r._5 = 0.f; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2e3 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = 0.f; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2e3 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = b._5; r._6 = 0.f; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2e3 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = 0.f; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = 0.f; r._5 = 0.f; r._6 = b._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = 0.f; r._2 = 0.f; r._3 = 0.f; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = a._7; return r; }
_e1e2e3 operator - (_e1e2e3 a, _e1e2e3 b) { _e1e2e3 r; r._7 = a._7 - b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = b._0; r._1 = b._1; r._2 = b._2; r._3 = b._3; r._4 = b._4; r._5 = b._5; r._6 = b._6; r._7 = a._7 - b._7; return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0 - b; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1 - b._1; r._2 = a._2; r._3 = a._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1; r._2 = a._2 - b._2; r._3 = a._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = a._3 - b._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1 - b._1; r._2 = a._2 - b._2; r._3 = a._3 - b._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = a._4 - b._4; r._5 = a._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = a._4; r._5 = a._5 - b._5; r._6 = a._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = a._4; r._5 = a._5; r._6 = a._6 - b._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0 - b._0; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = a._4 - b._4; r._5 = a._5 - b._5; r._6 = a._6 - b._6; r._7 = a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = a._4; r._5 = a._5; r._6 = a._6; r._7 = a._7 - b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator - (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0 - b._0; r._1 = a._1 - b._1; r._2 = a._2 - b._2; r._3 = a._3 - b._3; r._4 = a._4 - b._4; r._5 = a._5 - b._5; r._6 = a._6 - b._6; r._7 = a._7 - b._7; return r; }

float32 inner(float32 a, float32 b) { float32 r = +(a * b); return r; }
float32 inner(float32 a, _e1 b) { float32 r = 0.f; return r; }
float32 inner(float32 a, _e2 b) { float32 r = 0.f; return r; }
float32 inner(float32 a, _e3 b) { float32 r = 0.f; return r; }
float32 inner(float32 a, _e1_e2_e3 b) { float32 r = 0.f; return r; }
float32 inner(float32 a, _e1e2 b) { float32 r = 0.f; return r; }
float32 inner(float32 a, _e2e3 b) { float32 r = 0.f; return r; }
float32 inner(float32 a, _e3e1 b) { float32 r = 0.f; return r; }
float32 inner(float32 a, _e0_e1e2_e2e3_e3e1 b) { float32 r = +(a * b._0); return r; }
float32 inner(float32 a, _e1e2e3 b) { float32 r = 0.f; return r; }
float32 inner(float32 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { float32 r = +(a * b._0); return r; }

float32 inner(_e1 a, float32 b) { float32 r = 0.f; return r; }
float32 inner(_e1 a, _e1 b) { float32 r = +(a._1 * b._1); return r; }
float32 inner(_e1 a, _e2 b) { float32 r = 0.f; return r; }
float32 inner(_e1 a, _e3 b) { float32 r = 0.f; return r; }
float32 inner(_e1 a, _e1_e2_e3 b) { float32 r = +(a._1 * b._1); return r; }
float32 inner(_e1 a, _e1e2 b) { float32 r = 0.f; return r; }
float32 inner(_e1 a, _e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e1 a, _e3e1 b) { float32 r = 0.f; return r; }
float32 inner(_e1 a, _e0_e1e2_e2e3_e3e1 b) { float32 r = 0.f; return r; }
float32 inner(_e1 a, _e1e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { float32 r = +(a._1 * b._1); return r; }

float32 inner(_e2 a, float32 b) { float32 r = 0.f; return r; }
float32 inner(_e2 a, _e1 b) { float32 r = 0.f; return r; }
float32 inner(_e2 a, _e2 b) { float32 r = +(a._2 * b._2); return r; }
float32 inner(_e2 a, _e3 b) { float32 r = 0.f; return r; }
float32 inner(_e2 a, _e1_e2_e3 b) { float32 r = +(a._2 * b._2); return r; }
float32 inner(_e2 a, _e1e2 b) { float32 r = 0.f; return r; }
float32 inner(_e2 a, _e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e2 a, _e3e1 b) { float32 r = 0.f; return r; }
float32 inner(_e2 a, _e0_e1e2_e2e3_e3e1 b) { float32 r = 0.f; return r; }
float32 inner(_e2 a, _e1e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { float32 r = +(a._2 * b._2); return r; }

float32 inner(_e3 a, float32 b) { float32 r = 0.f; return r; }
float32 inner(_e3 a, _e1 b) { float32 r = 0.f; return r; }
float32 inner(_e3 a, _e2 b) { float32 r = 0.f; return r; }
float32 inner(_e3 a, _e3 b) { float32 r = +(a._3 * b._3); return r; }
float32 inner(_e3 a, _e1_e2_e3 b) { float32 r = +(a._3 * b._3); return r; }
float32 inner(_e3 a, _e1e2 b) { float32 r = 0.f; return r; }
float32 inner(_e3 a, _e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e3 a, _e3e1 b) { float32 r = 0.f; return r; }
float32 inner(_e3 a, _e0_e1e2_e2e3_e3e1 b) { float32 r = 0.f; return r; }
float32 inner(_e3 a, _e1e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { float32 r = +(a._3 * b._3); return r; }

float32 inner(_e1_e2_e3 a, float32 b) { float32 r = 0.f; return r; }
float32 inner(_e1_e2_e3 a, _e1 b) { float32 r = +(a._1 * b._1); return r; }
float32 inner(_e1_e2_e3 a, _e2 b) { float32 r = +(a._2 * b._2); return r; }
float32 inner(_e1_e2_e3 a, _e3 b) { float32 r = +(a._3 * b._3); return r; }
float32 inner(_e1_e2_e3 a, _e1_e2_e3 b) { float32 r = +(a._1 * b._1) + (a._2 * b._2) + (a._3 * b._3); return r; }
float32 inner(_e1_e2_e3 a, _e1e2 b) { float32 r = 0.f; return r; }
float32 inner(_e1_e2_e3 a, _e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e1_e2_e3 a, _e3e1 b) { float32 r = 0.f; return r; }
float32 inner(_e1_e2_e3 a, _e0_e1e2_e2e3_e3e1 b) { float32 r = 0.f; return r; }
float32 inner(_e1_e2_e3 a, _e1e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e1_e2_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { float32 r = +(a._1 * b._1) + (a._2 * b._2) + (a._3 * b._3); return r; }

float32 inner(_e1e2 a, float32 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2 a, _e1 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2 a, _e2 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2 a, _e3 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2 a, _e1_e2_e3 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2 a, _e1e2 b) { float32 r = -(a._4 * b._4); return r; }
float32 inner(_e1e2 a, _e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2 a, _e3e1 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2 a, _e0_e1e2_e2e3_e3e1 b) { float32 r = -(a._4 * b._4); return r; }
float32 inner(_e1e2 a, _e1e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { float32 r = -(a._4 * b._4); return r; }

float32 inner(_e2e3 a, float32 b) { float32 r = 0.f; return r; }
float32 inner(_e2e3 a, _e1 b) { float32 r = 0.f; return r; }
float32 inner(_e2e3 a, _e2 b) { float32 r = 0.f; return r; }
float32 inner(_e2e3 a, _e3 b) { float32 r = 0.f; return r; }
float32 inner(_e2e3 a, _e1_e2_e3 b) { float32 r = 0.f; return r; }
float32 inner(_e2e3 a, _e1e2 b) { float32 r = 0.f; return r; }
float32 inner(_e2e3 a, _e2e3 b) { float32 r = -(a._5 * b._5); return r; }
float32 inner(_e2e3 a, _e3e1 b) { float32 r = 0.f; return r; }
float32 inner(_e2e3 a, _e0_e1e2_e2e3_e3e1 b) { float32 r = -(a._5 * b._5); return r; }
float32 inner(_e2e3 a, _e1e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { float32 r = -(a._5 * b._5); return r; }

float32 inner(_e3e1 a, float32 b) { float32 r = 0.f; return r; }
float32 inner(_e3e1 a, _e1 b) { float32 r = 0.f; return r; }
float32 inner(_e3e1 a, _e2 b) { float32 r = 0.f; return r; }
float32 inner(_e3e1 a, _e3 b) { float32 r = 0.f; return r; }
float32 inner(_e3e1 a, _e1_e2_e3 b) { float32 r = 0.f; return r; }
float32 inner(_e3e1 a, _e1e2 b) { float32 r = 0.f; return r; }
float32 inner(_e3e1 a, _e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e3e1 a, _e3e1 b) { float32 r = -(a._6 * b._6); return r; }
float32 inner(_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { float32 r = -(a._6 * b._6); return r; }
float32 inner(_e3e1 a, _e1e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { float32 r = -(a._6 * b._6); return r; }

float32 inner(_e0_e1e2_e2e3_e3e1 a, float32 b) { float32 r = +(a._0 * b); return r; }
float32 inner(_e0_e1e2_e2e3_e3e1 a, _e1 b) { float32 r = 0.f; return r; }
float32 inner(_e0_e1e2_e2e3_e3e1 a, _e2 b) { float32 r = 0.f; return r; }
float32 inner(_e0_e1e2_e2e3_e3e1 a, _e3 b) { float32 r = 0.f; return r; }
float32 inner(_e0_e1e2_e2e3_e3e1 a, _e1_e2_e3 b) { float32 r = 0.f; return r; }
float32 inner(_e0_e1e2_e2e3_e3e1 a, _e1e2 b) { float32 r = -(a._4 * b._4); return r; }
float32 inner(_e0_e1e2_e2e3_e3e1 a, _e2e3 b) { float32 r = -(a._5 * b._5); return r; }
float32 inner(_e0_e1e2_e2e3_e3e1 a, _e3e1 b) { float32 r = -(a._6 * b._6); return r; }
float32 inner(_e0_e1e2_e2e3_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { float32 r = +(a._0 * b._0) - (a._4 * b._4) - (a._5 * b._5) - (a._6 * b._6); return r; }
float32 inner(_e0_e1e2_e2e3_e3e1 a, _e1e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e0_e1e2_e2e3_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { float32 r = +(a._0 * b._0) - (a._4 * b._4) - (a._5 * b._5) - (a._6 * b._6); return r; }

float32 inner(_e1e2e3 a, float32 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2e3 a, _e1 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2e3 a, _e2 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2e3 a, _e3 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2e3 a, _e1_e2_e3 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2e3 a, _e1e2 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2e3 a, _e2e3 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2e3 a, _e3e1 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { float32 r = 0.f; return r; }
float32 inner(_e1e2e3 a, _e1e2e3 b) { float32 r = +(a._7 * b._7); return r; }
float32 inner(_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { float32 r = +(a._7 * b._7); return r; }

float32 inner(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, float32 b) { float32 r = +(a._0 * b); return r; }
float32 inner(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1 b) { float32 r = +(a._1 * b._1); return r; }
float32 inner(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2 b) { float32 r = +(a._2 * b._2); return r; }
float32 inner(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3 b) { float32 r = +(a._3 * b._3); return r; }
float32 inner(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1_e2_e3 b) { float32 r = +(a._1 * b._1) + (a._2 * b._2) + (a._3 * b._3); return r; }
float32 inner(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2 b) { float32 r = -(a._4 * b._4); return r; }
float32 inner(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2e3 b) { float32 r = -(a._5 * b._5); return r; }
float32 inner(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3e1 b) { float32 r = -(a._6 * b._6); return r; }
float32 inner(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { float32 r = +(a._0 * b._0) - (a._4 * b._4) - (a._5 * b._5) - (a._6 * b._6); return r; }
float32 inner(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2e3 b) { float32 r = +(a._7 * b._7); return r; }
float32 inner(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { float32 r = +(a._0 * b._0) + (a._1 * b._1) + (a._2 * b._2) + (a._3 * b._3) - (a._4 * b._4) - (a._5 * b._5) - (a._6 * b._6) + (a._7 * b._7); return r; }

float32 outer(float32 a, float32 b) { float32 r = 0.f; r = a * b; return r; }
_e1 outer(float32 a, _e1 b) { _e1 r = {}; r._1 += b._1; return r; }
_e2 outer(float32 a, _e2 b) { _e2 r = {}; r._2 += b._2; return r; }
_e3 outer(float32 a, _e3 b) { _e3 r = {}; r._3 += b._3; return r; }
_e1_e2_e3 outer(float32 a, _e1_e2_e3 b) { _e1_e2_e3 r = {}; r._1 += b._1; r._2 += b._2; r._3 += b._3; return r; }
_e1e2 outer(float32 a, _e1e2 b) { _e1e2 r = {}; r._4 += b._4; return r; }
_e2e3 outer(float32 a, _e2e3 b) { _e2e3 r = {}; r._5 += b._5; return r; }
_e3e1 outer(float32 a, _e3e1 b) { _e3e1 r = {}; r._6 += b._6; return r; }
_e0_e1e2_e2e3_e3e1 outer(float32 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._0 = a * b._0; r._4 += b._4; r._5 += b._5; r._6 += b._6; return r; }
_e1e2e3 outer(float32 a, _e1e2e3 b) { _e1e2e3 r = {}; r._7 += b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(float32 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._0 = a * b._0; r._1 += b._1; r._2 += b._2; r._3 += b._3; r._4 += b._4; r._5 += b._5; r._6 += b._6; r._7 += b._7; return r; }
_e1 outer(_e1 a, float32 b) { _e1 r = {}; r._1 += a._1; return r; }
_e1e2 outer(_e1 a, _e2 b) { _e1e2 r = {}; r._4 += a._1 * b._2; return r; }
_e3e1 outer(_e1 a, _e3 b) { _e3e1 r = {}; r._6 -= a._1 * b._3; return r; }
_e0_e1e2_e2e3_e3e1 outer(_e1 a, _e1_e2_e3 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._4 += a._1 * b._2; r._6 -= a._1 * b._3; return r; }
_e1e2e3 outer(_e1 a, _e2e3 b) { _e1e2e3 r = {}; r._7 += a._1 * b._5; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e1 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += a._1; r._7 += a._1 * b._5; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += a._1; r._4 += a._1 * b._2; r._6 -= a._1 * b._3; r._7 += a._1 * b._5; return r; }
_e2 outer(_e2 a, float32 b) { _e2 r = {}; r._2 += a._2; return r; }
_e1e2 outer(_e2 a, _e1 b) { _e1e2 r = {}; r._4 -= a._2 * b._1; return r; }
_e2e3 outer(_e2 a, _e3 b) { _e2e3 r = {}; r._5 += a._2 * b._3; return r; }
_e0_e1e2_e2e3_e3e1 outer(_e2 a, _e1_e2_e3 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._4 -= a._2 * b._1; r._5 += a._2 * b._3; return r; }
_e1e2e3 outer(_e2 a, _e3e1 b) { _e1e2e3 r = {}; r._7 += a._2 * b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e2 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._2 += a._2; r._7 += a._2 * b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._2 += a._2; r._4 -= a._2 * b._1; r._5 += a._2 * b._3; r._7 += a._2 * b._6; return r; }
_e3 outer(_e3 a, float32 b) { _e3 r = {}; r._3 += a._3; return r; }
_e3e1 outer(_e3 a, _e1 b) { _e3e1 r = {}; r._6 -= a._3 * b._1; return r; }
_e2e3 outer(_e3 a, _e2 b) { _e2e3 r = {}; r._5 -= a._3 * b._2; return r; }
_e0_e1e2_e2e3_e3e1 outer(_e3 a, _e1_e2_e3 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._6 -= a._3 * b._1; r._5 -= a._3 * b._2; return r; }
_e1e2e3 outer(_e3 a, _e1e2 b) { _e1e2e3 r = {}; r._7 += a._3 * b._4; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._3 += a._3; r._7 += a._3 * b._4; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._3 += a._3; r._6 -= a._3 * b._1; r._5 -= a._3 * b._2; r._7 += a._3 * b._4; return r; }
_e1_e2_e3 outer(_e1_e2_e3 a, float32 b) { _e1_e2_e3 r = {}; r._1 += a._1; r._2 += a._2; r._3 += a._3; return r; }
_e0_e1e2_e2e3_e3e1 outer(_e1_e2_e3 a, _e1 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._4 -= a._2 * b._1; r._6 -= a._3 * b._1; return r; }
_e0_e1e2_e2e3_e3e1 outer(_e1_e2_e3 a, _e2 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._4 += a._1 * b._2; r._5 -= a._3 * b._2; return r; }
_e0_e1e2_e2e3_e3e1 outer(_e1_e2_e3 a, _e3 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._6 -= a._1 * b._3; r._5 += a._2 * b._3; return r; }
_e0_e1e2_e2e3_e3e1 outer(_e1_e2_e3 a, _e1_e2_e3 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._4 += a._1 * b._2; r._6 -= a._1 * b._3; r._4 -= a._2 * b._1; r._5 += a._2 * b._3; r._6 -= a._3 * b._1; r._5 -= a._3 * b._2; return r; }
_e1e2e3 outer(_e1_e2_e3 a, _e1e2 b) { _e1e2e3 r = {}; r._7 += a._3 * b._4; return r; }
_e1e2e3 outer(_e1_e2_e3 a, _e2e3 b) { _e1e2e3 r = {}; r._7 += a._1 * b._5; return r; }
_e1e2e3 outer(_e1_e2_e3 a, _e3e1 b) { _e1e2e3 r = {}; r._7 += a._2 * b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e1_e2_e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += a._1; r._7 += a._1 * b._5; r._2 += a._2; r._7 += a._2 * b._6; r._3 += a._3; r._7 += a._3 * b._4; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e1_e2_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += a._1; r._4 += a._1 * b._2; r._6 -= a._1 * b._3; r._7 += a._1 * b._5; r._2 += a._2; r._4 -= a._2 * b._1; r._5 += a._2 * b._3; r._7 += a._2 * b._6; r._3 += a._3; r._6 -= a._3 * b._1; r._5 -= a._3 * b._2; r._7 += a._3 * b._4; return r; }
_e1e2 outer(_e1e2 a, float32 b) { _e1e2 r = {}; r._4 += a._4; return r; }
_e1e2e3 outer(_e1e2 a, _e3 b) { _e1e2e3 r = {}; r._7 += a._4 * b._3; return r; }
_e1e2e3 outer(_e1e2 a, _e1_e2_e3 b) { _e1e2e3 r = {}; r._7 += a._4 * b._3; return r; }
_e1e2 outer(_e1e2 a, _e0_e1e2_e2e3_e3e1 b) { _e1e2 r = {}; r._4 += a._4; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e1e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._4 += a._4; r._7 += a._4 * b._3; return r; }
_e2e3 outer(_e2e3 a, float32 b) { _e2e3 r = {}; r._5 += a._5; return r; }
_e1e2e3 outer(_e2e3 a, _e1 b) { _e1e2e3 r = {}; r._7 += a._5 * b._1; return r; }
_e1e2e3 outer(_e2e3 a, _e1_e2_e3 b) { _e1e2e3 r = {}; r._7 += a._5 * b._1; return r; }
_e2e3 outer(_e2e3 a, _e0_e1e2_e2e3_e3e1 b) { _e2e3 r = {}; r._5 += a._5; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._5 += a._5; r._7 += a._5 * b._1; return r; }
_e3e1 outer(_e3e1 a, float32 b) { _e3e1 r = {}; r._6 += a._6; return r; }
_e1e2e3 outer(_e3e1 a, _e2 b) { _e1e2e3 r = {}; r._7 += a._6 * b._2; return r; }
_e1e2e3 outer(_e3e1 a, _e1_e2_e3 b) { _e1e2e3 r = {}; r._7 += a._6 * b._2; return r; }
_e3e1 outer(_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { _e3e1 r = {}; r._6 += a._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._6 += a._6; r._7 += a._6 * b._2; return r; }
_e0_e1e2_e2e3_e3e1 outer(_e0_e1e2_e2e3_e3e1 a, float32 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._0 = a._0 * b; r._4 += a._4; r._5 += a._5; r._6 += a._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1e2_e2e3_e3e1 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += b._1; r._7 += a._5 * b._1; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1e2_e2e3_e3e1 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._2 += b._2; r._7 += a._6 * b._2; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1e2_e2e3_e3e1 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._3 += b._3; r._7 += a._4 * b._3; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1e2_e2e3_e3e1 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += b._1; r._2 += b._2; r._3 += b._3; r._7 += a._4 * b._3; r._7 += a._5 * b._1; r._7 += a._6 * b._2; return r; }
_e1e2 outer(_e0_e1e2_e2e3_e3e1 a, _e1e2 b) { _e1e2 r = {}; r._4 += b._4; return r; }
_e2e3 outer(_e0_e1e2_e2e3_e3e1 a, _e2e3 b) { _e2e3 r = {}; r._5 += b._5; return r; }
_e3e1 outer(_e0_e1e2_e2e3_e3e1 a, _e3e1 b) { _e3e1 r = {}; r._6 += b._6; return r; }
_e0_e1e2_e2e3_e3e1 outer(_e0_e1e2_e2e3_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._0 = a._0 * b._0; r._4 += b._4; r._5 += b._5; r._6 += b._6; r._4 += a._4; r._5 += a._5; r._6 += a._6; return r; }
_e1e2e3 outer(_e0_e1e2_e2e3_e3e1 a, _e1e2e3 b) { _e1e2e3 r = {}; r._7 += b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1e2_e2e3_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._0 = a._0 * b._0; r._1 += b._1; r._2 += b._2; r._3 += b._3; r._4 += b._4; r._5 += b._5; r._6 += b._6; r._7 += b._7; r._4 += a._4; r._7 += a._4 * b._3; r._5 += a._5; r._7 += a._5 * b._1; r._6 += a._6; r._7 += a._6 * b._2; return r; }
_e1e2e3 outer(_e1e2e3 a, float32 b) { _e1e2e3 r = {}; r._7 += a._7; return r; }
_e1e2e3 outer(_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { _e1e2e3 r = {}; r._7 += a._7; return r; }
_e1e2e3 outer(_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e1e2e3 r = {}; r._7 += a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._0 = a._0 * b; r._1 += a._1; r._2 += a._2; r._3 += a._3; r._4 += a._4; r._5 += a._5; r._6 += a._6; r._7 += a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += b._1; r._4 -= a._2 * b._1; r._6 -= a._3 * b._1; r._7 += a._5 * b._1; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._2 += b._2; r._4 += a._1 * b._2; r._5 -= a._3 * b._2; r._7 += a._6 * b._2; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._3 += b._3; r._6 -= a._1 * b._3; r._5 += a._2 * b._3; r._7 += a._4 * b._3; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += b._1; r._2 += b._2; r._3 += b._3; r._4 += a._1 * b._2; r._6 -= a._1 * b._3; r._4 -= a._2 * b._1; r._5 += a._2 * b._3; r._6 -= a._3 * b._1; r._5 -= a._3 * b._2; r._7 += a._4 * b._3; r._7 += a._5 * b._1; r._7 += a._6 * b._2; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._4 += b._4; r._7 += a._3 * b._4; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._5 += b._5; r._7 += a._1 * b._5; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._6 += b._6; r._7 += a._2 * b._6; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._0 = a._0 * b._0; r._4 += b._4; r._5 += b._5; r._6 += b._6; r._1 += a._1; r._7 += a._1 * b._5; r._2 += a._2; r._7 += a._2 * b._6; r._3 += a._3; r._7 += a._3 * b._4; r._4 += a._4; r._5 += a._5; r._6 += a._6; r._7 += a._7; return r; }
_e1e2e3 outer(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2e3 b) { _e1e2e3 r = {}; r._7 += b._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 outer(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._0 = a._0 * b._0; r._1 += b._1; r._2 += b._2; r._3 += b._3; r._4 += b._4; r._5 += b._5; r._6 += b._6; r._7 += b._7; r._1 += a._1; r._4 += a._1 * b._2; r._6 -= a._1 * b._3; r._7 += a._1 * b._5; r._2 += a._2; r._4 -= a._2 * b._1; r._5 += a._2 * b._3; r._7 += a._2 * b._6; r._3 += a._3; r._6 -= a._3 * b._1; r._5 -= a._3 * b._2; r._7 += a._3 * b._4; r._4 += a._4; r._7 += a._4 * b._3; r._5 += a._5; r._7 += a._5 * b._1; r._6 += a._6; r._7 += a._6 * b._2; r._7 += a._7; return r; }

_e1 operator * (float32 a, _e1 b) { _e1 r = {}; r._1 += (a * b._1); return r; }
_e2 operator * (float32 a, _e2 b) { _e2 r = {}; r._2 += (a * b._2); return r; }
_e3 operator * (float32 a, _e3 b) { _e3 r = {}; r._3 += (a * b._3); return r; }
_e1_e2_e3 operator * (float32 a, _e1_e2_e3 b) { _e1_e2_e3 r = {}; r._1 += (a * b._1); r._2 += (a * b._2); r._3 += (a * b._3); return r; }
_e1e2 operator * (float32 a, _e1e2 b) { _e1e2 r = {}; r._4 += (a * b._4); return r; }
_e2e3 operator * (float32 a, _e2e3 b) { _e2e3 r = {}; r._5 += (a * b._5); return r; }
_e3e1 operator * (float32 a, _e3e1 b) { _e3e1 r = {}; r._6 += (a * b._6); return r; }
_e0_e1e2_e2e3_e3e1 operator * (float32 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._0 += (a * b._0); r._4 += (a * b._4); r._5 += (a * b._5); r._6 += (a * b._6); return r; }
_e1e2e3 operator * (float32 a, _e1e2e3 b) { _e1e2e3 r = {}; r._7 += (a * b._7); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (float32 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._0 += (a * b._0); r._1 += (a * b._1); r._2 += (a * b._2); r._3 += (a * b._3); r._4 += (a * b._4); r._5 += (a * b._5); r._6 += (a * b._6); r._7 += (a * b._7); return r; }

_e1 operator * (_e1 a, float32 b) { _e1 r = {}; r._1 += (a._1 * b); return r; }
float32 operator * (_e1 a, _e1 b) { float32 r = 0.f; r += (a._1 * b._1); return r; }
_e1e2 operator * (_e1 a, _e2 b) { _e1e2 r = {}; r._4 += (a._1 * b._2); return r; }
_e3e1 operator * (_e1 a, _e3 b) { _e3e1 r = {}; r._6 -= (a._1 * b._3); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e1 a, _e1_e2_e3 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._0 += (a._1 * b._1); r._4 += (a._1 * b._2); r._6 -= (a._1 * b._3); return r; }
_e2 operator * (_e1 a, _e1e2 b) { _e2 r = {}; r._2 += (a._1 * b._4); return r; }
_e1e2e3 operator * (_e1 a, _e2e3 b) { _e1e2e3 r = {}; r._7 += (a._1 * b._5); return r; }
_e3 operator * (_e1 a, _e3e1 b) { _e3 r = {}; r._3 -= (a._1 * b._6); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e1 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += (a._1 * b._0); r._2 += (a._1 * b._4); r._7 += (a._1 * b._5); r._3 -= (a._1 * b._6); return r; }
_e2e3 operator * (_e1 a, _e1e2e3 b) { _e2e3 r = {}; r._5 += (a._1 * b._7); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += (a._1 * b._0); r._0 += (a._1 * b._1); r._4 += (a._1 * b._2); r._6 -= (a._1 * b._3); r._2 += (a._1 * b._4); r._7 += (a._1 * b._5); r._3 -= (a._1 * b._6); r._5 += (a._1 * b._7); return r; }

_e2 operator * (_e2 a, float32 b) { _e2 r = {}; r._2 += (a._2 * b); return r; }
_e1e2 operator * (_e2 a, _e1 b) { _e1e2 r = {}; r._4 -= (a._2 * b._1); return r; }
float32 operator * (_e2 a, _e2 b) { float32 r = 0.f; r += (a._2 * b._2); return r; }
_e2e3 operator * (_e2 a, _e3 b) { _e2e3 r = {}; r._5 += (a._2 * b._3); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e2 a, _e1_e2_e3 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._4 -= (a._2 * b._1); r._0 += (a._2 * b._2); r._5 += (a._2 * b._3); return r; }
_e1 operator * (_e2 a, _e1e2 b) { _e1 r = {}; r._1 -= (a._2 * b._4); return r; }
_e3 operator * (_e2 a, _e2e3 b) { _e3 r = {}; r._3 += (a._2 * b._5); return r; }
_e1e2e3 operator * (_e2 a, _e3e1 b) { _e1e2e3 r = {}; r._7 += (a._2 * b._6); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e2 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._2 += (a._2 * b._0); r._1 -= (a._2 * b._4); r._3 += (a._2 * b._5); r._7 += (a._2 * b._6); return r; }
_e3e1 operator * (_e2 a, _e1e2e3 b) { _e3e1 r = {}; r._6 += (a._2 * b._7); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._2 += (a._2 * b._0); r._4 -= (a._2 * b._1); r._0 += (a._2 * b._2); r._5 += (a._2 * b._3); r._1 -= (a._2 * b._4); r._3 += (a._2 * b._5); r._7 += (a._2 * b._6); r._6 += (a._2 * b._7); return r; }

_e3 operator * (_e3 a, float32 b) { _e3 r = {}; r._3 += (a._3 * b); return r; }
_e3e1 operator * (_e3 a, _e1 b) { _e3e1 r = {}; r._6 += (a._3 * b._1); return r; }
_e2e3 operator * (_e3 a, _e2 b) { _e2e3 r = {}; r._5 -= (a._3 * b._2); return r; }
float32 operator * (_e3 a, _e3 b) { float32 r = 0.f; r += (a._3 * b._3); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e3 a, _e1_e2_e3 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._6 += (a._3 * b._1); r._5 -= (a._3 * b._2); r._0 += (a._3 * b._3); return r; }
_e1e2e3 operator * (_e3 a, _e1e2 b) { _e1e2e3 r = {}; r._7 += (a._3 * b._4); return r; }
_e2 operator * (_e3 a, _e2e3 b) { _e2 r = {}; r._2 -= (a._3 * b._5); return r; }
_e1 operator * (_e3 a, _e3e1 b) { _e1 r = {}; r._1 += (a._3 * b._6); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._3 += (a._3 * b._0); r._7 += (a._3 * b._4); r._2 -= (a._3 * b._5); r._1 += (a._3 * b._6); return r; }
_e1e2 operator * (_e3 a, _e1e2e3 b) { _e1e2 r = {}; r._4 += (a._3 * b._7); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._3 += (a._3 * b._0); r._6 += (a._3 * b._1); r._5 -= (a._3 * b._2); r._0 += (a._3 * b._3); r._7 += (a._3 * b._4); r._2 -= (a._3 * b._5); r._1 += (a._3 * b._6); r._4 += (a._3 * b._7); return r; }

_e1_e2_e3 operator * (_e1_e2_e3 a, float32 b) { _e1_e2_e3 r = {}; r._1 += (a._1 * b); r._2 += (a._2 * b); r._3 += (a._3 * b); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e1_e2_e3 a, _e1 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._0 += (a._1 * b._1); r._4 -= (a._2 * b._1); r._6 += (a._3 * b._1); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e1_e2_e3 a, _e2 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._4 += (a._1 * b._2); r._0 += (a._2 * b._2); r._5 -= (a._3 * b._2); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e1_e2_e3 a, _e3 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._6 -= (a._1 * b._3); r._5 += (a._2 * b._3); r._0 += (a._3 * b._3); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e1_e2_e3 a, _e1_e2_e3 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._0 += (a._1 * b._1); r._4 += (a._1 * b._2); r._6 -= (a._1 * b._3); r._4 -= (a._2 * b._1); r._0 += (a._2 * b._2); r._5 += (a._2 * b._3); r._6 += (a._3 * b._1); r._5 -= (a._3 * b._2); r._0 += (a._3 * b._3); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e1_e2_e3 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._2 += (a._1 * b._4); r._1 -= (a._2 * b._4); r._7 += (a._3 * b._4); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e1_e2_e3 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._7 += (a._1 * b._5); r._3 += (a._2 * b._5); r._2 -= (a._3 * b._5); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e1_e2_e3 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._3 -= (a._1 * b._6); r._7 += (a._2 * b._6); r._1 += (a._3 * b._6); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e1_e2_e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += (a._1 * b._0); r._2 += (a._1 * b._4); r._7 += (a._1 * b._5); r._3 -= (a._1 * b._6); r._2 += (a._2 * b._0); r._1 -= (a._2 * b._4); r._3 += (a._2 * b._5); r._7 += (a._2 * b._6); r._3 += (a._3 * b._0); r._7 += (a._3 * b._4); r._2 -= (a._3 * b._5); r._1 += (a._3 * b._6); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e1_e2_e3 a, _e1e2e3 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._5 += (a._1 * b._7); r._6 += (a._2 * b._7); r._4 += (a._3 * b._7); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e1_e2_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += (a._1 * b._0); r._0 += (a._1 * b._1); r._4 += (a._1 * b._2); r._6 -= (a._1 * b._3); r._2 += (a._1 * b._4); r._7 += (a._1 * b._5); r._3 -= (a._1 * b._6); r._5 += (a._1 * b._7); r._2 += (a._2 * b._0); r._4 -= (a._2 * b._1); r._0 += (a._2 * b._2); r._5 += (a._2 * b._3); r._1 -= (a._2 * b._4); r._3 += (a._2 * b._5); r._7 += (a._2 * b._6); r._6 += (a._2 * b._7); r._3 += (a._3 * b._0); r._6 += (a._3 * b._1); r._5 -= (a._3 * b._2); r._0 += (a._3 * b._3); r._7 += (a._3 * b._4); r._2 -= (a._3 * b._5); r._1 += (a._3 * b._6); r._4 += (a._3 * b._7); return r; }

_e1e2 operator * (_e1e2 a, float32 b) { _e1e2 r = {}; r._4 += (a._4 * b); return r; }
_e2 operator * (_e1e2 a, _e1 b) { _e2 r = {}; r._2 -= (a._4 * b._1); return r; }
_e1 operator * (_e1e2 a, _e2 b) { _e1 r = {}; r._1 += (a._4 * b._2); return r; }
_e1e2e3 operator * (_e1e2 a, _e3 b) { _e1e2e3 r = {}; r._7 += (a._4 * b._3); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e1e2 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._2 -= (a._4 * b._1); r._1 += (a._4 * b._2); r._7 += (a._4 * b._3); return r; }
float32 operator * (_e1e2 a, _e1e2 b) { float32 r = 0.f; r -= (a._4 * b._4); return r; }
_e3e1 operator * (_e1e2 a, _e2e3 b) { _e3e1 r = {}; r._6 -= (a._4 * b._5); return r; }
_e2e3 operator * (_e1e2 a, _e3e1 b) { _e2e3 r = {}; r._5 += (a._4 * b._6); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e1e2 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._4 += (a._4 * b._0); r._0 -= (a._4 * b._4); r._6 -= (a._4 * b._5); r._5 += (a._4 * b._6); return r; }
_e3 operator * (_e1e2 a, _e1e2e3 b) { _e3 r = {}; r._3 -= (a._4 * b._7); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e1e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._4 += (a._4 * b._0); r._2 -= (a._4 * b._1); r._1 += (a._4 * b._2); r._7 += (a._4 * b._3); r._0 -= (a._4 * b._4); r._6 -= (a._4 * b._5); r._5 += (a._4 * b._6); r._3 -= (a._4 * b._7); return r; }

_e2e3 operator * (_e2e3 a, float32 b) { _e2e3 r = {}; r._5 += (a._5 * b); return r; }
_e1e2e3 operator * (_e2e3 a, _e1 b) { _e1e2e3 r = {}; r._7 += (a._5 * b._1); return r; }
_e3 operator * (_e2e3 a, _e2 b) { _e3 r = {}; r._3 -= (a._5 * b._2); return r; }
_e2 operator * (_e2e3 a, _e3 b) { _e2 r = {}; r._2 += (a._5 * b._3); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e2e3 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._7 += (a._5 * b._1); r._3 -= (a._5 * b._2); r._2 += (a._5 * b._3); return r; }
_e3e1 operator * (_e2e3 a, _e1e2 b) { _e3e1 r = {}; r._6 += (a._5 * b._4); return r; }
float32 operator * (_e2e3 a, _e2e3 b) { float32 r = 0.f; r -= (a._5 * b._5); return r; }
_e1e2 operator * (_e2e3 a, _e3e1 b) { _e1e2 r = {}; r._4 -= (a._5 * b._6); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e2e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._5 += (a._5 * b._0); r._6 += (a._5 * b._4); r._0 -= (a._5 * b._5); r._4 -= (a._5 * b._6); return r; }
_e1 operator * (_e2e3 a, _e1e2e3 b) { _e1 r = {}; r._1 -= (a._5 * b._7); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._5 += (a._5 * b._0); r._7 += (a._5 * b._1); r._3 -= (a._5 * b._2); r._2 += (a._5 * b._3); r._6 += (a._5 * b._4); r._0 -= (a._5 * b._5); r._4 -= (a._5 * b._6); r._1 -= (a._5 * b._7); return r; }

_e3e1 operator * (_e3e1 a, float32 b) { _e3e1 r = {}; r._6 += (a._6 * b); return r; }
_e3 operator * (_e3e1 a, _e1 b) { _e3 r = {}; r._3 += (a._6 * b._1); return r; }
_e1e2e3 operator * (_e3e1 a, _e2 b) { _e1e2e3 r = {}; r._7 += (a._6 * b._2); return r; }
_e1 operator * (_e3e1 a, _e3 b) { _e1 r = {}; r._1 -= (a._6 * b._3); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e3e1 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._3 += (a._6 * b._1); r._7 += (a._6 * b._2); r._1 -= (a._6 * b._3); return r; }
_e2e3 operator * (_e3e1 a, _e1e2 b) { _e2e3 r = {}; r._5 -= (a._6 * b._4); return r; }
_e1e2 operator * (_e3e1 a, _e2e3 b) { _e1e2 r = {}; r._4 += (a._6 * b._5); return r; }
float32 operator * (_e3e1 a, _e3e1 b) { float32 r = 0.f; r -= (a._6 * b._6); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._6 += (a._6 * b._0); r._5 -= (a._6 * b._4); r._4 += (a._6 * b._5); r._0 -= (a._6 * b._6); return r; }
_e2 operator * (_e3e1 a, _e1e2e3 b) { _e2 r = {}; r._2 -= (a._6 * b._7); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._6 += (a._6 * b._0); r._3 += (a._6 * b._1); r._7 += (a._6 * b._2); r._1 -= (a._6 * b._3); r._5 -= (a._6 * b._4); r._4 += (a._6 * b._5); r._0 -= (a._6 * b._6); r._2 -= (a._6 * b._7); return r; }

_e0_e1e2_e2e3_e3e1 operator * (_e0_e1e2_e2e3_e3e1 a, float32 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._0 += (a._0 * b); r._4 += (a._4 * b); r._5 += (a._5 * b); r._6 += (a._6 * b); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1e2_e2e3_e3e1 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += (a._0 * b._1); r._2 -= (a._4 * b._1); r._7 += (a._5 * b._1); r._3 += (a._6 * b._1); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1e2_e2e3_e3e1 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._2 += (a._0 * b._2); r._1 += (a._4 * b._2); r._3 -= (a._5 * b._2); r._7 += (a._6 * b._2); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1e2_e2e3_e3e1 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._3 += (a._0 * b._3); r._7 += (a._4 * b._3); r._2 += (a._5 * b._3); r._1 -= (a._6 * b._3); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1e2_e2e3_e3e1 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += (a._0 * b._1); r._2 += (a._0 * b._2); r._3 += (a._0 * b._3); r._2 -= (a._4 * b._1); r._1 += (a._4 * b._2); r._7 += (a._4 * b._3); r._7 += (a._5 * b._1); r._3 -= (a._5 * b._2); r._2 += (a._5 * b._3); r._3 += (a._6 * b._1); r._7 += (a._6 * b._2); r._1 -= (a._6 * b._3); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e0_e1e2_e2e3_e3e1 a, _e1e2 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._4 += (a._0 * b._4); r._0 -= (a._4 * b._4); r._6 += (a._5 * b._4); r._5 -= (a._6 * b._4); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e0_e1e2_e2e3_e3e1 a, _e2e3 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._5 += (a._0 * b._5); r._6 -= (a._4 * b._5); r._0 -= (a._5 * b._5); r._4 += (a._6 * b._5); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e0_e1e2_e2e3_e3e1 a, _e3e1 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._6 += (a._0 * b._6); r._5 += (a._4 * b._6); r._4 -= (a._5 * b._6); r._0 -= (a._6 * b._6); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e0_e1e2_e2e3_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._0 += (a._0 * b._0); r._4 += (a._0 * b._4); r._5 += (a._0 * b._5); r._6 += (a._0 * b._6); r._4 += (a._4 * b._0); r._0 -= (a._4 * b._4); r._6 -= (a._4 * b._5); r._5 += (a._4 * b._6); r._5 += (a._5 * b._0); r._6 += (a._5 * b._4); r._0 -= (a._5 * b._5); r._4 -= (a._5 * b._6); r._6 += (a._6 * b._0); r._5 -= (a._6 * b._4); r._4 += (a._6 * b._5); r._0 -= (a._6 * b._6); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1e2_e2e3_e3e1 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._7 += (a._0 * b._7); r._3 -= (a._4 * b._7); r._1 -= (a._5 * b._7); r._2 -= (a._6 * b._7); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1e2_e2e3_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._0 += (a._0 * b._0); r._1 += (a._0 * b._1); r._2 += (a._0 * b._2); r._3 += (a._0 * b._3); r._4 += (a._0 * b._4); r._5 += (a._0 * b._5); r._6 += (a._0 * b._6); r._7 += (a._0 * b._7); r._4 += (a._4 * b._0); r._2 -= (a._4 * b._1); r._1 += (a._4 * b._2); r._7 += (a._4 * b._3); r._0 -= (a._4 * b._4); r._6 -= (a._4 * b._5); r._5 += (a._4 * b._6); r._3 -= (a._4 * b._7); r._5 += (a._5 * b._0); r._7 += (a._5 * b._1); r._3 -= (a._5 * b._2); r._2 += (a._5 * b._3); r._6 += (a._5 * b._4); r._0 -= (a._5 * b._5); r._4 -= (a._5 * b._6); r._1 -= (a._5 * b._7); r._6 += (a._6 * b._0); r._3 += (a._6 * b._1); r._7 += (a._6 * b._2); r._1 -= (a._6 * b._3); r._5 -= (a._6 * b._4); r._4 += (a._6 * b._5); r._0 -= (a._6 * b._6); r._2 -= (a._6 * b._7); return r; }

_e1e2e3 operator * (_e1e2e3 a, float32 b) { _e1e2e3 r = {}; r._7 += (a._7 * b); return r; }
_e2e3 operator * (_e1e2e3 a, _e1 b) { _e2e3 r = {}; r._5 += (a._7 * b._1); return r; }
_e3e1 operator * (_e1e2e3 a, _e2 b) { _e3e1 r = {}; r._6 += (a._7 * b._2); return r; }
_e1e2 operator * (_e1e2e3 a, _e3 b) { _e1e2 r = {}; r._4 += (a._7 * b._3); return r; }
_e0_e1e2_e2e3_e3e1 operator * (_e1e2e3 a, _e1_e2_e3 b) { _e0_e1e2_e2e3_e3e1 r = {}; r._5 += (a._7 * b._1); r._6 += (a._7 * b._2); r._4 += (a._7 * b._3); return r; }
_e3 operator * (_e1e2e3 a, _e1e2 b) { _e3 r = {}; r._3 -= (a._7 * b._4); return r; }
_e1 operator * (_e1e2e3 a, _e2e3 b) { _e1 r = {}; r._1 -= (a._7 * b._5); return r; }
_e2 operator * (_e1e2e3 a, _e3e1 b) { _e2 r = {}; r._2 -= (a._7 * b._6); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._7 += (a._7 * b._0); r._3 -= (a._7 * b._4); r._1 -= (a._7 * b._5); r._2 -= (a._7 * b._6); return r; }
float32 operator * (_e1e2e3 a, _e1e2e3 b) { float32 r = 0.f; r -= (a._7 * b._7); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._7 += (a._7 * b._0); r._5 += (a._7 * b._1); r._6 += (a._7 * b._2); r._4 += (a._7 * b._3); r._3 -= (a._7 * b._4); r._1 -= (a._7 * b._5); r._2 -= (a._7 * b._6); r._0 -= (a._7 * b._7); return r; }

_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, float32 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._0 += (a._0 * b); r._1 += (a._1 * b); r._2 += (a._2 * b); r._3 += (a._3 * b); r._4 += (a._4 * b); r._5 += (a._5 * b); r._6 += (a._6 * b); r._7 += (a._7 * b); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += (a._0 * b._1); r._0 += (a._1 * b._1); r._4 -= (a._2 * b._1); r._6 += (a._3 * b._1); r._2 -= (a._4 * b._1); r._7 += (a._5 * b._1); r._3 += (a._6 * b._1); r._5 += (a._7 * b._1); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._2 += (a._0 * b._2); r._4 += (a._1 * b._2); r._0 += (a._2 * b._2); r._5 -= (a._3 * b._2); r._1 += (a._4 * b._2); r._3 -= (a._5 * b._2); r._7 += (a._6 * b._2); r._6 += (a._7 * b._2); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._3 += (a._0 * b._3); r._6 -= (a._1 * b._3); r._5 += (a._2 * b._3); r._0 += (a._3 * b._3); r._7 += (a._4 * b._3); r._2 += (a._5 * b._3); r._1 -= (a._6 * b._3); r._4 += (a._7 * b._3); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1_e2_e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._1 += (a._0 * b._1); r._2 += (a._0 * b._2); r._3 += (a._0 * b._3); r._0 += (a._1 * b._1); r._4 += (a._1 * b._2); r._6 -= (a._1 * b._3); r._4 -= (a._2 * b._1); r._0 += (a._2 * b._2); r._5 += (a._2 * b._3); r._6 += (a._3 * b._1); r._5 -= (a._3 * b._2); r._0 += (a._3 * b._3); r._2 -= (a._4 * b._1); r._1 += (a._4 * b._2); r._7 += (a._4 * b._3); r._7 += (a._5 * b._1); r._3 -= (a._5 * b._2); r._2 += (a._5 * b._3); r._3 += (a._6 * b._1); r._7 += (a._6 * b._2); r._1 -= (a._6 * b._3); r._5 += (a._7 * b._1); r._6 += (a._7 * b._2); r._4 += (a._7 * b._3); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._4 += (a._0 * b._4); r._2 += (a._1 * b._4); r._1 -= (a._2 * b._4); r._7 += (a._3 * b._4); r._0 -= (a._4 * b._4); r._6 += (a._5 * b._4); r._5 -= (a._6 * b._4); r._3 -= (a._7 * b._4); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._5 += (a._0 * b._5); r._7 += (a._1 * b._5); r._3 += (a._2 * b._5); r._2 -= (a._3 * b._5); r._6 -= (a._4 * b._5); r._0 -= (a._5 * b._5); r._4 += (a._6 * b._5); r._1 -= (a._7 * b._5); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._6 += (a._0 * b._6); r._3 -= (a._1 * b._6); r._7 += (a._2 * b._6); r._1 += (a._3 * b._6); r._5 += (a._4 * b._6); r._4 -= (a._5 * b._6); r._0 -= (a._6 * b._6); r._2 -= (a._7 * b._6); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._0 += (a._0 * b._0); r._4 += (a._0 * b._4); r._5 += (a._0 * b._5); r._6 += (a._0 * b._6); r._1 += (a._1 * b._0); r._2 += (a._1 * b._4); r._7 += (a._1 * b._5); r._3 -= (a._1 * b._6); r._2 += (a._2 * b._0); r._1 -= (a._2 * b._4); r._3 += (a._2 * b._5); r._7 += (a._2 * b._6); r._3 += (a._3 * b._0); r._7 += (a._3 * b._4); r._2 -= (a._3 * b._5); r._1 += (a._3 * b._6); r._4 += (a._4 * b._0); r._0 -= (a._4 * b._4); r._6 -= (a._4 * b._5); r._5 += (a._4 * b._6); r._5 += (a._5 * b._0); r._6 += (a._5 * b._4); r._0 -= (a._5 * b._5); r._4 -= (a._5 * b._6); r._6 += (a._6 * b._0); r._5 -= (a._6 * b._4); r._4 += (a._6 * b._5); r._0 -= (a._6 * b._6); r._7 += (a._7 * b._0); r._3 -= (a._7 * b._4); r._1 -= (a._7 * b._5); r._2 -= (a._7 * b._6); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._7 += (a._0 * b._7); r._5 += (a._1 * b._7); r._6 += (a._2 * b._7); r._4 += (a._3 * b._7); r._3 -= (a._4 * b._7); r._1 -= (a._5 * b._7); r._2 -= (a._6 * b._7); r._0 -= (a._7 * b._7); return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 operator * (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r = {}; r._0 += (a._0 * b._0); r._1 += (a._0 * b._1); r._2 += (a._0 * b._2); r._3 += (a._0 * b._3); r._4 += (a._0 * b._4); r._5 += (a._0 * b._5); r._6 += (a._0 * b._6); r._7 += (a._0 * b._7); r._1 += (a._1 * b._0); r._0 += (a._1 * b._1); r._4 += (a._1 * b._2); r._6 -= (a._1 * b._3); r._2 += (a._1 * b._4); r._7 += (a._1 * b._5); r._3 -= (a._1 * b._6); r._5 += (a._1 * b._7); r._2 += (a._2 * b._0); r._4 -= (a._2 * b._1); r._0 += (a._2 * b._2); r._5 += (a._2 * b._3); r._1 -= (a._2 * b._4); r._3 += (a._2 * b._5); r._7 += (a._2 * b._6); r._6 += (a._2 * b._7); r._3 += (a._3 * b._0); r._6 += (a._3 * b._1); r._5 -= (a._3 * b._2); r._0 += (a._3 * b._3); r._7 += (a._3 * b._4); r._2 -= (a._3 * b._5); r._1 += (a._3 * b._6); r._4 += (a._3 * b._7); r._4 += (a._4 * b._0); r._2 -= (a._4 * b._1); r._1 += (a._4 * b._2); r._7 += (a._4 * b._3); r._0 -= (a._4 * b._4); r._6 -= (a._4 * b._5); r._5 += (a._4 * b._6); r._3 -= (a._4 * b._7); r._5 += (a._5 * b._0); r._7 += (a._5 * b._1); r._3 -= (a._5 * b._2); r._2 += (a._5 * b._3); r._6 += (a._5 * b._4); r._0 -= (a._5 * b._5); r._4 -= (a._5 * b._6); r._1 -= (a._5 * b._7); r._6 += (a._6 * b._0); r._3 += (a._6 * b._1); r._7 += (a._6 * b._2); r._1 -= (a._6 * b._3); r._5 -= (a._6 * b._4); r._4 += (a._6 * b._5); r._0 -= (a._6 * b._6); r._2 -= (a._6 * b._7); r._7 += (a._7 * b._0); r._5 += (a._7 * b._1); r._6 += (a._7 * b._2); r._4 += (a._7 * b._3); r._3 -= (a._7 * b._4); r._1 -= (a._7 * b._5); r._2 -= (a._7 * b._6); r._0 -= (a._7 * b._7); return r; }

void conjugate(_e1& a) { }
void conjugate(_e2& a) { }
void conjugate(_e3& a) { }
void conjugate(_e1_e2_e3& a) { }
void conjugate(_e1e2& a) { a._4 = -a._4; }
void conjugate(_e2e3& a) { a._5 = -a._5; }
void conjugate(_e3e1& a) { a._6 = -a._6; }
void conjugate(_e0_e1e2_e2e3_e3e1& a) { a._4 = -a._4; a._5 = -a._5; a._6 = -a._6; }
void conjugate(_e1e2e3& a) { a._7 = -a._7; }
void conjugate(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3& a) { a._4 = -a._4; a._5 = -a._5; a._6 = -a._6; a._7 = -a._7; }

_e1 conjugated(_e1 a) { _e1 r; r._1 = a._1; return r; }
_e2 conjugated(_e2 a) { _e2 r; r._2 = a._2; return r; }
_e3 conjugated(_e3 a) { _e3 r; r._3 = a._3; return r; }
_e1_e2_e3 conjugated(_e1_e2_e3 a) { _e1_e2_e3 r; r._1 = a._1; r._2 = a._2; r._3 = a._3; return r; }
_e1e2 conjugated(_e1e2 a) { _e1e2 r; r._4 = -a._4; return r; }
_e2e3 conjugated(_e2e3 a) { _e2e3 r; r._5 = -a._5; return r; }
_e3e1 conjugated(_e3e1 a) { _e3e1 r; r._6 = -a._6; return r; }
_e0_e1e2_e2e3_e3e1 conjugated(_e0_e1e2_e2e3_e3e1 a) { _e0_e1e2_e2e3_e3e1 r; r._0 = a._0; r._4 = -a._4; r._5 = -a._5; r._6 = -a._6; return r; }
_e1e2e3 conjugated(_e1e2e3 a) { _e1e2e3 r; r._7 = -a._7; return r; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 conjugated(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a) { _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = a._3; r._4 = -a._4; r._5 = -a._5; r._6 = -a._6; r._7 = -a._7; return r; }

bool32 operator == (float32 a, _e1 b) { return (a == 0.f) && (0.f == b._1); }
bool32 operator == (float32 a, _e2 b) { return (a == 0.f) && (0.f == b._2); }
bool32 operator == (float32 a, _e3 b) { return (a == 0.f) && (0.f == b._3); }
bool32 operator == (float32 a, _e1_e2_e3 b) { return (a == 0.f) && (0.f == b._1) && (0.f == b._2) && (0.f == b._3); }
bool32 operator == (float32 a, _e1e2 b) { return (a == 0.f) && (0.f == b._4); }
bool32 operator == (float32 a, _e2e3 b) { return (a == 0.f) && (0.f == b._5); }
bool32 operator == (float32 a, _e3e1 b) { return (a == 0.f) && (0.f == b._6); }
bool32 operator == (float32 a, _e0_e1e2_e2e3_e3e1 b) { return (a == b._0) && (0.f == b._4) && (0.f == b._5) && (0.f == b._6); }
bool32 operator == (float32 a, _e1e2e3 b) { return (a == 0.f) && (0.f == b._7); }
bool32 operator == (float32 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (a == b._0) && (0.f == b._1) && (0.f == b._2) && (0.f == b._3) && (0.f == b._4) && (0.f == b._5) && (0.f == b._6) && (0.f == b._7); }

bool32 operator == (_e1 a, float32 b) { return (0.f == b) && (a._1 == 0.f); }
bool32 operator == (_e1 a, _e1 b) { return (a._1 == b._1); }
bool32 operator == (_e1 a, _e2 b) { return (a._1 == 0.f) && (0.f == b._2); }
bool32 operator == (_e1 a, _e3 b) { return (a._1 == 0.f) && (0.f == b._3); }
bool32 operator == (_e1 a, _e1_e2_e3 b) { return (a._1 == b._1) && (0.f == b._2) && (0.f == b._3); }
bool32 operator == (_e1 a, _e1e2 b) { return (a._1 == 0.f) && (0.f == b._4); }
bool32 operator == (_e1 a, _e2e3 b) { return (a._1 == 0.f) && (0.f == b._5); }
bool32 operator == (_e1 a, _e3e1 b) { return (a._1 == 0.f) && (0.f == b._6); }
bool32 operator == (_e1 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f == b._0) && (a._1 == 0.f) && (0.f == b._4) && (0.f == b._5) && (0.f == b._6); }
bool32 operator == (_e1 a, _e1e2e3 b) { return (a._1 == 0.f) && (0.f == b._7); }
bool32 operator == (_e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f == b._0) && (a._1 == b._1) && (0.f == b._2) && (0.f == b._3) && (0.f == b._4) && (0.f == b._5) && (0.f == b._6) && (0.f == b._7); }

bool32 operator == (_e2 a, float32 b) { return (0.f == b) && (a._2 == 0.f); }
bool32 operator == (_e2 a, _e1 b) { return (0.f == b._1) && (a._2 == 0.f); }
bool32 operator == (_e2 a, _e2 b) { return (a._2 == b._2); }
bool32 operator == (_e2 a, _e3 b) { return (a._2 == 0.f) && (0.f == b._3); }
bool32 operator == (_e2 a, _e1_e2_e3 b) { return (0.f == b._1) && (a._2 == b._2) && (0.f == b._3); }
bool32 operator == (_e2 a, _e1e2 b) { return (a._2 == 0.f) && (0.f == b._4); }
bool32 operator == (_e2 a, _e2e3 b) { return (a._2 == 0.f) && (0.f == b._5); }
bool32 operator == (_e2 a, _e3e1 b) { return (a._2 == 0.f) && (0.f == b._6); }
bool32 operator == (_e2 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f == b._0) && (a._2 == 0.f) && (0.f == b._4) && (0.f == b._5) && (0.f == b._6); }
bool32 operator == (_e2 a, _e1e2e3 b) { return (a._2 == 0.f) && (0.f == b._7); }
bool32 operator == (_e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f == b._0) && (0.f == b._1) && (a._2 == b._2) && (0.f == b._3) && (0.f == b._4) && (0.f == b._5) && (0.f == b._6) && (0.f == b._7); }

bool32 operator == (_e3 a, float32 b) { return (0.f == b) && (a._3 == 0.f); }
bool32 operator == (_e3 a, _e1 b) { return (0.f == b._1) && (a._3 == 0.f); }
bool32 operator == (_e3 a, _e2 b) { return (0.f == b._2) && (a._3 == 0.f); }
bool32 operator == (_e3 a, _e3 b) { return (a._3 == b._3); }
bool32 operator == (_e3 a, _e1_e2_e3 b) { return (0.f == b._1) && (0.f == b._2) && (a._3 == b._3); }
bool32 operator == (_e3 a, _e1e2 b) { return (a._3 == 0.f) && (0.f == b._4); }
bool32 operator == (_e3 a, _e2e3 b) { return (a._3 == 0.f) && (0.f == b._5); }
bool32 operator == (_e3 a, _e3e1 b) { return (a._3 == 0.f) && (0.f == b._6); }
bool32 operator == (_e3 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f == b._0) && (a._3 == 0.f) && (0.f == b._4) && (0.f == b._5) && (0.f == b._6); }
bool32 operator == (_e3 a, _e1e2e3 b) { return (a._3 == 0.f) && (0.f == b._7); }
bool32 operator == (_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f == b._0) && (0.f == b._1) && (0.f == b._2) && (a._3 == b._3) && (0.f == b._4) && (0.f == b._5) && (0.f == b._6) && (0.f == b._7); }

bool32 operator == (_e1_e2_e3 a, float32 b) { return (0.f == b) && (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == 0.f); }
bool32 operator == (_e1_e2_e3 a, _e1 b) { return (a._1 == b._1) && (a._2 == 0.f) && (a._3 == 0.f); }
bool32 operator == (_e1_e2_e3 a, _e2 b) { return (a._1 == 0.f) && (a._2 == b._2) && (a._3 == 0.f); }
bool32 operator == (_e1_e2_e3 a, _e3 b) { return (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == b._3); }
bool32 operator == (_e1_e2_e3 a, _e1_e2_e3 b) { return (a._1 == b._1) && (a._2 == b._2) && (a._3 == b._3); }
bool32 operator == (_e1_e2_e3 a, _e1e2 b) { return (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == 0.f) && (0.f == b._4); }
bool32 operator == (_e1_e2_e3 a, _e2e3 b) { return (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == 0.f) && (0.f == b._5); }
bool32 operator == (_e1_e2_e3 a, _e3e1 b) { return (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == 0.f) && (0.f == b._6); }
bool32 operator == (_e1_e2_e3 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f == b._0) && (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == 0.f) && (0.f == b._4) && (0.f == b._5) && (0.f == b._6); }
bool32 operator == (_e1_e2_e3 a, _e1e2e3 b) { return (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == 0.f) && (0.f == b._7); }
bool32 operator == (_e1_e2_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f == b._0) && (a._1 == b._1) && (a._2 == b._2) && (a._3 == b._3) && (0.f == b._4) && (0.f == b._5) && (0.f == b._6) && (0.f == b._7); }

bool32 operator == (_e1e2 a, float32 b) { return (0.f == b) && (a._4 == 0.f); }
bool32 operator == (_e1e2 a, _e1 b) { return (0.f == b._1) && (a._4 == 0.f); }
bool32 operator == (_e1e2 a, _e2 b) { return (0.f == b._2) && (a._4 == 0.f); }
bool32 operator == (_e1e2 a, _e3 b) { return (0.f == b._3) && (a._4 == 0.f); }
bool32 operator == (_e1e2 a, _e1_e2_e3 b) { return (0.f == b._1) && (0.f == b._2) && (0.f == b._3) && (a._4 == 0.f); }
bool32 operator == (_e1e2 a, _e1e2 b) { return (a._4 == b._4); }
bool32 operator == (_e1e2 a, _e2e3 b) { return (a._4 == 0.f) && (0.f == b._5); }
bool32 operator == (_e1e2 a, _e3e1 b) { return (a._4 == 0.f) && (0.f == b._6); }
bool32 operator == (_e1e2 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f == b._0) && (a._4 == b._4) && (0.f == b._5) && (0.f == b._6); }
bool32 operator == (_e1e2 a, _e1e2e3 b) { return (a._4 == 0.f) && (0.f == b._7); }
bool32 operator == (_e1e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f == b._0) && (0.f == b._1) && (0.f == b._2) && (0.f == b._3) && (a._4 == b._4) && (0.f == b._5) && (0.f == b._6) && (0.f == b._7); }

bool32 operator == (_e2e3 a, float32 b) { return (0.f == b) && (a._5 == 0.f); }
bool32 operator == (_e2e3 a, _e1 b) { return (0.f == b._1) && (a._5 == 0.f); }
bool32 operator == (_e2e3 a, _e2 b) { return (0.f == b._2) && (a._5 == 0.f); }
bool32 operator == (_e2e3 a, _e3 b) { return (0.f == b._3) && (a._5 == 0.f); }
bool32 operator == (_e2e3 a, _e1_e2_e3 b) { return (0.f == b._1) && (0.f == b._2) && (0.f == b._3) && (a._5 == 0.f); }
bool32 operator == (_e2e3 a, _e1e2 b) { return (0.f == b._4) && (a._5 == 0.f); }
bool32 operator == (_e2e3 a, _e2e3 b) { return (a._5 == b._5); }
bool32 operator == (_e2e3 a, _e3e1 b) { return (a._5 == 0.f) && (0.f == b._6); }
bool32 operator == (_e2e3 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f == b._0) && (0.f == b._4) && (a._5 == b._5) && (0.f == b._6); }
bool32 operator == (_e2e3 a, _e1e2e3 b) { return (a._5 == 0.f) && (0.f == b._7); }
bool32 operator == (_e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f == b._0) && (0.f == b._1) && (0.f == b._2) && (0.f == b._3) && (0.f == b._4) && (a._5 == b._5) && (0.f == b._6) && (0.f == b._7); }

bool32 operator == (_e3e1 a, float32 b) { return (0.f == b) && (a._6 == 0.f); }
bool32 operator == (_e3e1 a, _e1 b) { return (0.f == b._1) && (a._6 == 0.f); }
bool32 operator == (_e3e1 a, _e2 b) { return (0.f == b._2) && (a._6 == 0.f); }
bool32 operator == (_e3e1 a, _e3 b) { return (0.f == b._3) && (a._6 == 0.f); }
bool32 operator == (_e3e1 a, _e1_e2_e3 b) { return (0.f == b._1) && (0.f == b._2) && (0.f == b._3) && (a._6 == 0.f); }
bool32 operator == (_e3e1 a, _e1e2 b) { return (0.f == b._4) && (a._6 == 0.f); }
bool32 operator == (_e3e1 a, _e2e3 b) { return (0.f == b._5) && (a._6 == 0.f); }
bool32 operator == (_e3e1 a, _e3e1 b) { return (a._6 == b._6); }
bool32 operator == (_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f == b._0) && (0.f == b._4) && (0.f == b._5) && (a._6 == b._6); }
bool32 operator == (_e3e1 a, _e1e2e3 b) { return (a._6 == 0.f) && (0.f == b._7); }
bool32 operator == (_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f == b._0) && (0.f == b._1) && (0.f == b._2) && (0.f == b._3) && (0.f == b._4) && (0.f == b._5) && (a._6 == b._6) && (0.f == b._7); }

bool32 operator == (_e0_e1e2_e2e3_e3e1 a, float32 b) { return (a._0 == b) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == 0.f); }
bool32 operator == (_e0_e1e2_e2e3_e3e1 a, _e1 b) { return (a._0 == 0.f) && (0.f == b._1) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == 0.f); }
bool32 operator == (_e0_e1e2_e2e3_e3e1 a, _e2 b) { return (a._0 == 0.f) && (0.f == b._2) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == 0.f); }
bool32 operator == (_e0_e1e2_e2e3_e3e1 a, _e3 b) { return (a._0 == 0.f) && (0.f == b._3) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == 0.f); }
bool32 operator == (_e0_e1e2_e2e3_e3e1 a, _e1_e2_e3 b) { return (a._0 == 0.f) && (0.f == b._1) && (0.f == b._2) && (0.f == b._3) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == 0.f); }
bool32 operator == (_e0_e1e2_e2e3_e3e1 a, _e1e2 b) { return (a._0 == 0.f) && (a._4 == b._4) && (a._5 == 0.f) && (a._6 == 0.f); }
bool32 operator == (_e0_e1e2_e2e3_e3e1 a, _e2e3 b) { return (a._0 == 0.f) && (a._4 == 0.f) && (a._5 == b._5) && (a._6 == 0.f); }
bool32 operator == (_e0_e1e2_e2e3_e3e1 a, _e3e1 b) { return (a._0 == 0.f) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == b._6); }
bool32 operator == (_e0_e1e2_e2e3_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { return (a._0 == b._0) && (a._4 == b._4) && (a._5 == b._5) && (a._6 == b._6); }
bool32 operator == (_e0_e1e2_e2e3_e3e1 a, _e1e2e3 b) { return (a._0 == 0.f) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == 0.f) && (0.f == b._7); }
bool32 operator == (_e0_e1e2_e2e3_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (a._0 == b._0) && (0.f == b._1) && (0.f == b._2) && (0.f == b._3) && (a._4 == b._4) && (a._5 == b._5) && (a._6 == b._6) && (0.f == b._7); }

bool32 operator == (_e1e2e3 a, float32 b) { return (0.f == b) && (a._7 == 0.f); }
bool32 operator == (_e1e2e3 a, _e1 b) { return (0.f == b._1) && (a._7 == 0.f); }
bool32 operator == (_e1e2e3 a, _e2 b) { return (0.f == b._2) && (a._7 == 0.f); }
bool32 operator == (_e1e2e3 a, _e3 b) { return (0.f == b._3) && (a._7 == 0.f); }
bool32 operator == (_e1e2e3 a, _e1_e2_e3 b) { return (0.f == b._1) && (0.f == b._2) && (0.f == b._3) && (a._7 == 0.f); }
bool32 operator == (_e1e2e3 a, _e1e2 b) { return (0.f == b._4) && (a._7 == 0.f); }
bool32 operator == (_e1e2e3 a, _e2e3 b) { return (0.f == b._5) && (a._7 == 0.f); }
bool32 operator == (_e1e2e3 a, _e3e1 b) { return (0.f == b._6) && (a._7 == 0.f); }
bool32 operator == (_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f == b._0) && (0.f == b._4) && (0.f == b._5) && (0.f == b._6) && (a._7 == 0.f); }
bool32 operator == (_e1e2e3 a, _e1e2e3 b) { return (a._7 == b._7); }
bool32 operator == (_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f == b._0) && (0.f == b._1) && (0.f == b._2) && (0.f == b._3) && (0.f == b._4) && (0.f == b._5) && (0.f == b._6) && (a._7 == b._7); }

bool32 operator == (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, float32 b) { return (a._0 == b) && (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == 0.f) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == 0.f) && (a._7 == 0.f); }
bool32 operator == (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1 b) { return (a._0 == 0.f) && (a._1 == b._1) && (a._2 == 0.f) && (a._3 == 0.f) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == 0.f) && (a._7 == 0.f); }
bool32 operator == (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2 b) { return (a._0 == 0.f) && (a._1 == 0.f) && (a._2 == b._2) && (a._3 == 0.f) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == 0.f) && (a._7 == 0.f); }
bool32 operator == (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3 b) { return (a._0 == 0.f) && (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == b._3) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == 0.f) && (a._7 == 0.f); }
bool32 operator == (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1_e2_e3 b) { return (a._0 == 0.f) && (a._1 == b._1) && (a._2 == b._2) && (a._3 == b._3) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == 0.f) && (a._7 == 0.f); }
bool32 operator == (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2 b) { return (a._0 == 0.f) && (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == 0.f) && (a._4 == b._4) && (a._5 == 0.f) && (a._6 == 0.f) && (a._7 == 0.f); }
bool32 operator == (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2e3 b) { return (a._0 == 0.f) && (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == 0.f) && (a._4 == 0.f) && (a._5 == b._5) && (a._6 == 0.f) && (a._7 == 0.f); }
bool32 operator == (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3e1 b) { return (a._0 == 0.f) && (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == 0.f) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == b._6) && (a._7 == 0.f); }
bool32 operator == (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { return (a._0 == b._0) && (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == 0.f) && (a._4 == b._4) && (a._5 == b._5) && (a._6 == b._6) && (a._7 == 0.f); }
bool32 operator == (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2e3 b) { return (a._0 == 0.f) && (a._1 == 0.f) && (a._2 == 0.f) && (a._3 == 0.f) && (a._4 == 0.f) && (a._5 == 0.f) && (a._6 == 0.f) && (a._7 == b._7); }
bool32 operator == (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (a._0 == b._0) && (a._1 == b._1) && (a._2 == b._2) && (a._3 == b._3) && (a._4 == b._4) && (a._5 == b._5) && (a._6 == b._6) && (a._7 == b._7); }

bool32 operator != (float32 a, _e1 b) { return (a != 0.f) || (0.f != b._1); }
bool32 operator != (float32 a, _e2 b) { return (a != 0.f) || (0.f != b._2); }
bool32 operator != (float32 a, _e3 b) { return (a != 0.f) || (0.f != b._3); }
bool32 operator != (float32 a, _e1_e2_e3 b) { return (a != 0.f) || (0.f != b._1) || (0.f != b._2) || (0.f != b._3); }
bool32 operator != (float32 a, _e1e2 b) { return (a != 0.f) || (0.f != b._4); }
bool32 operator != (float32 a, _e2e3 b) { return (a != 0.f) || (0.f != b._5); }
bool32 operator != (float32 a, _e3e1 b) { return (a != 0.f) || (0.f != b._6); }
bool32 operator != (float32 a, _e0_e1e2_e2e3_e3e1 b) { return (a != b._0) || (0.f != b._4) || (0.f != b._5) || (0.f != b._6); }
bool32 operator != (float32 a, _e1e2e3 b) { return (a != 0.f) || (0.f != b._7); }
bool32 operator != (float32 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (a != b._0) || (0.f != b._1) || (0.f != b._2) || (0.f != b._3) || (0.f != b._4) || (0.f != b._5) || (0.f != b._6) || (0.f != b._7); }

bool32 operator != (_e1 a, float32 b) { return (0.f != b) || (a._1 != 0.f); }
bool32 operator != (_e1 a, _e1 b) { return (a._1 != b._1); }
bool32 operator != (_e1 a, _e2 b) { return (a._1 != 0.f) || (0.f != b._2); }
bool32 operator != (_e1 a, _e3 b) { return (a._1 != 0.f) || (0.f != b._3); }
bool32 operator != (_e1 a, _e1_e2_e3 b) { return (a._1 != b._1) || (0.f != b._2) || (0.f != b._3); }
bool32 operator != (_e1 a, _e1e2 b) { return (a._1 != 0.f) || (0.f != b._4); }
bool32 operator != (_e1 a, _e2e3 b) { return (a._1 != 0.f) || (0.f != b._5); }
bool32 operator != (_e1 a, _e3e1 b) { return (a._1 != 0.f) || (0.f != b._6); }
bool32 operator != (_e1 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f != b._0) || (a._1 != 0.f) || (0.f != b._4) || (0.f != b._5) || (0.f != b._6); }
bool32 operator != (_e1 a, _e1e2e3 b) { return (a._1 != 0.f) || (0.f != b._7); }
bool32 operator != (_e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f != b._0) || (a._1 != b._1) || (0.f != b._2) || (0.f != b._3) || (0.f != b._4) || (0.f != b._5) || (0.f != b._6) || (0.f != b._7); }

bool32 operator != (_e2 a, float32 b) { return (0.f != b) || (a._2 != 0.f); }
bool32 operator != (_e2 a, _e1 b) { return (0.f != b._1) || (a._2 != 0.f); }
bool32 operator != (_e2 a, _e2 b) { return (a._2 != b._2); }
bool32 operator != (_e2 a, _e3 b) { return (a._2 != 0.f) || (0.f != b._3); }
bool32 operator != (_e2 a, _e1_e2_e3 b) { return (0.f != b._1) || (a._2 != b._2) || (0.f != b._3); }
bool32 operator != (_e2 a, _e1e2 b) { return (a._2 != 0.f) || (0.f != b._4); }
bool32 operator != (_e2 a, _e2e3 b) { return (a._2 != 0.f) || (0.f != b._5); }
bool32 operator != (_e2 a, _e3e1 b) { return (a._2 != 0.f) || (0.f != b._6); }
bool32 operator != (_e2 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f != b._0) || (a._2 != 0.f) || (0.f != b._4) || (0.f != b._5) || (0.f != b._6); }
bool32 operator != (_e2 a, _e1e2e3 b) { return (a._2 != 0.f) || (0.f != b._7); }
bool32 operator != (_e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f != b._0) || (0.f != b._1) || (a._2 != b._2) || (0.f != b._3) || (0.f != b._4) || (0.f != b._5) || (0.f != b._6) || (0.f != b._7); }

bool32 operator != (_e3 a, float32 b) { return (0.f != b) || (a._3 != 0.f); }
bool32 operator != (_e3 a, _e1 b) { return (0.f != b._1) || (a._3 != 0.f); }
bool32 operator != (_e3 a, _e2 b) { return (0.f != b._2) || (a._3 != 0.f); }
bool32 operator != (_e3 a, _e3 b) { return (a._3 != b._3); }
bool32 operator != (_e3 a, _e1_e2_e3 b) { return (0.f != b._1) || (0.f != b._2) || (a._3 != b._3); }
bool32 operator != (_e3 a, _e1e2 b) { return (a._3 != 0.f) || (0.f != b._4); }
bool32 operator != (_e3 a, _e2e3 b) { return (a._3 != 0.f) || (0.f != b._5); }
bool32 operator != (_e3 a, _e3e1 b) { return (a._3 != 0.f) || (0.f != b._6); }
bool32 operator != (_e3 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f != b._0) || (a._3 != 0.f) || (0.f != b._4) || (0.f != b._5) || (0.f != b._6); }
bool32 operator != (_e3 a, _e1e2e3 b) { return (a._3 != 0.f) || (0.f != b._7); }
bool32 operator != (_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f != b._0) || (0.f != b._1) || (0.f != b._2) || (a._3 != b._3) || (0.f != b._4) || (0.f != b._5) || (0.f != b._6) || (0.f != b._7); }

bool32 operator != (_e1_e2_e3 a, float32 b) { return (0.f != b) || (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != 0.f); }
bool32 operator != (_e1_e2_e3 a, _e1 b) { return (a._1 != b._1) || (a._2 != 0.f) || (a._3 != 0.f); }
bool32 operator != (_e1_e2_e3 a, _e2 b) { return (a._1 != 0.f) || (a._2 != b._2) || (a._3 != 0.f); }
bool32 operator != (_e1_e2_e3 a, _e3 b) { return (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != b._3); }
bool32 operator != (_e1_e2_e3 a, _e1_e2_e3 b) { return (a._1 != b._1) || (a._2 != b._2) || (a._3 != b._3); }
bool32 operator != (_e1_e2_e3 a, _e1e2 b) { return (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != 0.f) || (0.f != b._4); }
bool32 operator != (_e1_e2_e3 a, _e2e3 b) { return (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != 0.f) || (0.f != b._5); }
bool32 operator != (_e1_e2_e3 a, _e3e1 b) { return (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != 0.f) || (0.f != b._6); }
bool32 operator != (_e1_e2_e3 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f != b._0) || (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != 0.f) || (0.f != b._4) || (0.f != b._5) || (0.f != b._6); }
bool32 operator != (_e1_e2_e3 a, _e1e2e3 b) { return (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != 0.f) || (0.f != b._7); }
bool32 operator != (_e1_e2_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f != b._0) || (a._1 != b._1) || (a._2 != b._2) || (a._3 != b._3) || (0.f != b._4) || (0.f != b._5) || (0.f != b._6) || (0.f != b._7); }

bool32 operator != (_e1e2 a, float32 b) { return (0.f != b) || (a._4 != 0.f); }
bool32 operator != (_e1e2 a, _e1 b) { return (0.f != b._1) || (a._4 != 0.f); }
bool32 operator != (_e1e2 a, _e2 b) { return (0.f != b._2) || (a._4 != 0.f); }
bool32 operator != (_e1e2 a, _e3 b) { return (0.f != b._3) || (a._4 != 0.f); }
bool32 operator != (_e1e2 a, _e1_e2_e3 b) { return (0.f != b._1) || (0.f != b._2) || (0.f != b._3) || (a._4 != 0.f); }
bool32 operator != (_e1e2 a, _e1e2 b) { return (a._4 != b._4); }
bool32 operator != (_e1e2 a, _e2e3 b) { return (a._4 != 0.f) || (0.f != b._5); }
bool32 operator != (_e1e2 a, _e3e1 b) { return (a._4 != 0.f) || (0.f != b._6); }
bool32 operator != (_e1e2 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f != b._0) || (a._4 != b._4) || (0.f != b._5) || (0.f != b._6); }
bool32 operator != (_e1e2 a, _e1e2e3 b) { return (a._4 != 0.f) || (0.f != b._7); }
bool32 operator != (_e1e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f != b._0) || (0.f != b._1) || (0.f != b._2) || (0.f != b._3) || (a._4 != b._4) || (0.f != b._5) || (0.f != b._6) || (0.f != b._7); }

bool32 operator != (_e2e3 a, float32 b) { return (0.f != b) || (a._5 != 0.f); }
bool32 operator != (_e2e3 a, _e1 b) { return (0.f != b._1) || (a._5 != 0.f); }
bool32 operator != (_e2e3 a, _e2 b) { return (0.f != b._2) || (a._5 != 0.f); }
bool32 operator != (_e2e3 a, _e3 b) { return (0.f != b._3) || (a._5 != 0.f); }
bool32 operator != (_e2e3 a, _e1_e2_e3 b) { return (0.f != b._1) || (0.f != b._2) || (0.f != b._3) || (a._5 != 0.f); }
bool32 operator != (_e2e3 a, _e1e2 b) { return (0.f != b._4) || (a._5 != 0.f); }
bool32 operator != (_e2e3 a, _e2e3 b) { return (a._5 != b._5); }
bool32 operator != (_e2e3 a, _e3e1 b) { return (a._5 != 0.f) || (0.f != b._6); }
bool32 operator != (_e2e3 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f != b._0) || (0.f != b._4) || (a._5 != b._5) || (0.f != b._6); }
bool32 operator != (_e2e3 a, _e1e2e3 b) { return (a._5 != 0.f) || (0.f != b._7); }
bool32 operator != (_e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f != b._0) || (0.f != b._1) || (0.f != b._2) || (0.f != b._3) || (0.f != b._4) || (a._5 != b._5) || (0.f != b._6) || (0.f != b._7); }

bool32 operator != (_e3e1 a, float32 b) { return (0.f != b) || (a._6 != 0.f); }
bool32 operator != (_e3e1 a, _e1 b) { return (0.f != b._1) || (a._6 != 0.f); }
bool32 operator != (_e3e1 a, _e2 b) { return (0.f != b._2) || (a._6 != 0.f); }
bool32 operator != (_e3e1 a, _e3 b) { return (0.f != b._3) || (a._6 != 0.f); }
bool32 operator != (_e3e1 a, _e1_e2_e3 b) { return (0.f != b._1) || (0.f != b._2) || (0.f != b._3) || (a._6 != 0.f); }
bool32 operator != (_e3e1 a, _e1e2 b) { return (0.f != b._4) || (a._6 != 0.f); }
bool32 operator != (_e3e1 a, _e2e3 b) { return (0.f != b._5) || (a._6 != 0.f); }
bool32 operator != (_e3e1 a, _e3e1 b) { return (a._6 != b._6); }
bool32 operator != (_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f != b._0) || (0.f != b._4) || (0.f != b._5) || (a._6 != b._6); }
bool32 operator != (_e3e1 a, _e1e2e3 b) { return (a._6 != 0.f) || (0.f != b._7); }
bool32 operator != (_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f != b._0) || (0.f != b._1) || (0.f != b._2) || (0.f != b._3) || (0.f != b._4) || (0.f != b._5) || (a._6 != b._6) || (0.f != b._7); }

bool32 operator != (_e0_e1e2_e2e3_e3e1 a, float32 b) { return (a._0 != b) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != 0.f); }
bool32 operator != (_e0_e1e2_e2e3_e3e1 a, _e1 b) { return (a._0 != 0.f) || (0.f != b._1) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != 0.f); }
bool32 operator != (_e0_e1e2_e2e3_e3e1 a, _e2 b) { return (a._0 != 0.f) || (0.f != b._2) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != 0.f); }
bool32 operator != (_e0_e1e2_e2e3_e3e1 a, _e3 b) { return (a._0 != 0.f) || (0.f != b._3) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != 0.f); }
bool32 operator != (_e0_e1e2_e2e3_e3e1 a, _e1_e2_e3 b) { return (a._0 != 0.f) || (0.f != b._1) || (0.f != b._2) || (0.f != b._3) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != 0.f); }
bool32 operator != (_e0_e1e2_e2e3_e3e1 a, _e1e2 b) { return (a._0 != 0.f) || (a._4 != b._4) || (a._5 != 0.f) || (a._6 != 0.f); }
bool32 operator != (_e0_e1e2_e2e3_e3e1 a, _e2e3 b) { return (a._0 != 0.f) || (a._4 != 0.f) || (a._5 != b._5) || (a._6 != 0.f); }
bool32 operator != (_e0_e1e2_e2e3_e3e1 a, _e3e1 b) { return (a._0 != 0.f) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != b._6); }
bool32 operator != (_e0_e1e2_e2e3_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { return (a._0 != b._0) || (a._4 != b._4) || (a._5 != b._5) || (a._6 != b._6); }
bool32 operator != (_e0_e1e2_e2e3_e3e1 a, _e1e2e3 b) { return (a._0 != 0.f) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != 0.f) || (0.f != b._7); }
bool32 operator != (_e0_e1e2_e2e3_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (a._0 != b._0) || (0.f != b._1) || (0.f != b._2) || (0.f != b._3) || (a._4 != b._4) || (a._5 != b._5) || (a._6 != b._6) || (0.f != b._7); }

bool32 operator != (_e1e2e3 a, float32 b) { return (0.f != b) || (a._7 != 0.f); }
bool32 operator != (_e1e2e3 a, _e1 b) { return (0.f != b._1) || (a._7 != 0.f); }
bool32 operator != (_e1e2e3 a, _e2 b) { return (0.f != b._2) || (a._7 != 0.f); }
bool32 operator != (_e1e2e3 a, _e3 b) { return (0.f != b._3) || (a._7 != 0.f); }
bool32 operator != (_e1e2e3 a, _e1_e2_e3 b) { return (0.f != b._1) || (0.f != b._2) || (0.f != b._3) || (a._7 != 0.f); }
bool32 operator != (_e1e2e3 a, _e1e2 b) { return (0.f != b._4) || (a._7 != 0.f); }
bool32 operator != (_e1e2e3 a, _e2e3 b) { return (0.f != b._5) || (a._7 != 0.f); }
bool32 operator != (_e1e2e3 a, _e3e1 b) { return (0.f != b._6) || (a._7 != 0.f); }
bool32 operator != (_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { return (0.f != b._0) || (0.f != b._4) || (0.f != b._5) || (0.f != b._6) || (a._7 != 0.f); }
bool32 operator != (_e1e2e3 a, _e1e2e3 b) { return (a._7 != b._7); }
bool32 operator != (_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (0.f != b._0) || (0.f != b._1) || (0.f != b._2) || (0.f != b._3) || (0.f != b._4) || (0.f != b._5) || (0.f != b._6) || (a._7 != b._7); }

bool32 operator != (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, float32 b) { return (a._0 != b) || (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != 0.f) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != 0.f) || (a._7 != 0.f); }
bool32 operator != (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1 b) { return (a._0 != 0.f) || (a._1 != b._1) || (a._2 != 0.f) || (a._3 != 0.f) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != 0.f) || (a._7 != 0.f); }
bool32 operator != (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2 b) { return (a._0 != 0.f) || (a._1 != 0.f) || (a._2 != b._2) || (a._3 != 0.f) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != 0.f) || (a._7 != 0.f); }
bool32 operator != (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3 b) { return (a._0 != 0.f) || (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != b._3) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != 0.f) || (a._7 != 0.f); }
bool32 operator != (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1_e2_e3 b) { return (a._0 != 0.f) || (a._1 != b._1) || (a._2 != b._2) || (a._3 != b._3) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != 0.f) || (a._7 != 0.f); }
bool32 operator != (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2 b) { return (a._0 != 0.f) || (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != 0.f) || (a._4 != b._4) || (a._5 != 0.f) || (a._6 != 0.f) || (a._7 != 0.f); }
bool32 operator != (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2e3 b) { return (a._0 != 0.f) || (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != 0.f) || (a._4 != 0.f) || (a._5 != b._5) || (a._6 != 0.f) || (a._7 != 0.f); }
bool32 operator != (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3e1 b) { return (a._0 != 0.f) || (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != 0.f) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != b._6) || (a._7 != 0.f); }
bool32 operator != (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { return (a._0 != b._0) || (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != 0.f) || (a._4 != b._4) || (a._5 != b._5) || (a._6 != b._6) || (a._7 != 0.f); }
bool32 operator != (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2e3 b) { return (a._0 != 0.f) || (a._1 != 0.f) || (a._2 != 0.f) || (a._3 != 0.f) || (a._4 != 0.f) || (a._5 != 0.f) || (a._6 != 0.f) || (a._7 != b._7); }
bool32 operator != (_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return (a._0 != b._0) || (a._1 != b._1) || (a._2 != b._2) || (a._3 != b._3) || (a._4 != b._4) || (a._5 != b._5) || (a._6 != b._6) || (a._7 != b._7); }

bool32 equal(float32 a, _e1 b) { return ::math::equal(a, 0.f) && ::math::equal(0.f, b._1); }
bool32 equal(float32 a, _e2 b) { return ::math::equal(a, 0.f) && ::math::equal(0.f, b._2); }
bool32 equal(float32 a, _e3 b) { return ::math::equal(a, 0.f) && ::math::equal(0.f, b._3); }
bool32 equal(float32 a, _e1_e2_e3 b) { return ::math::equal(a, 0.f) && ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3); }
bool32 equal(float32 a, _e1e2 b) { return ::math::equal(a, 0.f) && ::math::equal(0.f, b._4); }
bool32 equal(float32 a, _e2e3 b) { return ::math::equal(a, 0.f) && ::math::equal(0.f, b._5); }
bool32 equal(float32 a, _e3e1 b) { return ::math::equal(a, 0.f) && ::math::equal(0.f, b._6); }
bool32 equal(float32 a, _e0_e1e2_e2e3_e3e1 b) { return ::math::equal(a, b._0) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6); }
bool32 equal(float32 a, _e1e2e3 b) { return ::math::equal(a, 0.f) && ::math::equal(0.f, b._7); }
bool32 equal(float32 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return ::math::equal(a, b._0) && ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6) && ::math::equal(0.f, b._7); }

bool32 equal(_e1 a, float32 b) { return ::math::equal(0.f, b) && ::math::equal(a._1, 0.f); }
bool32 equal(_e1 a, _e1 b) { return ::math::equal(a._1, b._1); }
bool32 equal(_e1 a, _e2 b) { return ::math::equal(a._1, 0.f) && ::math::equal(0.f, b._2); }
bool32 equal(_e1 a, _e3 b) { return ::math::equal(a._1, 0.f) && ::math::equal(0.f, b._3); }
bool32 equal(_e1 a, _e1_e2_e3 b) { return ::math::equal(a._1, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3); }
bool32 equal(_e1 a, _e1e2 b) { return ::math::equal(a._1, 0.f) && ::math::equal(0.f, b._4); }
bool32 equal(_e1 a, _e2e3 b) { return ::math::equal(a._1, 0.f) && ::math::equal(0.f, b._5); }
bool32 equal(_e1 a, _e3e1 b) { return ::math::equal(a._1, 0.f) && ::math::equal(0.f, b._6); }
bool32 equal(_e1 a, _e0_e1e2_e2e3_e3e1 b) { return ::math::equal(0.f, b._0) && ::math::equal(a._1, 0.f) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6); }
bool32 equal(_e1 a, _e1e2e3 b) { return ::math::equal(a._1, 0.f) && ::math::equal(0.f, b._7); }
bool32 equal(_e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return ::math::equal(0.f, b._0) && ::math::equal(a._1, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6) && ::math::equal(0.f, b._7); }

bool32 equal(_e2 a, float32 b) { return ::math::equal(0.f, b) && ::math::equal(a._2, 0.f); }
bool32 equal(_e2 a, _e1 b) { return ::math::equal(0.f, b._1) && ::math::equal(a._2, 0.f); }
bool32 equal(_e2 a, _e2 b) { return ::math::equal(a._2, b._2); }
bool32 equal(_e2 a, _e3 b) { return ::math::equal(a._2, 0.f) && ::math::equal(0.f, b._3); }
bool32 equal(_e2 a, _e1_e2_e3 b) { return ::math::equal(0.f, b._1) && ::math::equal(a._2, b._2) && ::math::equal(0.f, b._3); }
bool32 equal(_e2 a, _e1e2 b) { return ::math::equal(a._2, 0.f) && ::math::equal(0.f, b._4); }
bool32 equal(_e2 a, _e2e3 b) { return ::math::equal(a._2, 0.f) && ::math::equal(0.f, b._5); }
bool32 equal(_e2 a, _e3e1 b) { return ::math::equal(a._2, 0.f) && ::math::equal(0.f, b._6); }
bool32 equal(_e2 a, _e0_e1e2_e2e3_e3e1 b) { return ::math::equal(0.f, b._0) && ::math::equal(a._2, 0.f) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6); }
bool32 equal(_e2 a, _e1e2e3 b) { return ::math::equal(a._2, 0.f) && ::math::equal(0.f, b._7); }
bool32 equal(_e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return ::math::equal(0.f, b._0) && ::math::equal(0.f, b._1) && ::math::equal(a._2, b._2) && ::math::equal(0.f, b._3) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6) && ::math::equal(0.f, b._7); }

bool32 equal(_e3 a, float32 b) { return ::math::equal(0.f, b) && ::math::equal(a._3, 0.f); }
bool32 equal(_e3 a, _e1 b) { return ::math::equal(0.f, b._1) && ::math::equal(a._3, 0.f); }
bool32 equal(_e3 a, _e2 b) { return ::math::equal(0.f, b._2) && ::math::equal(a._3, 0.f); }
bool32 equal(_e3 a, _e3 b) { return ::math::equal(a._3, b._3); }
bool32 equal(_e3 a, _e1_e2_e3 b) { return ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(a._3, b._3); }
bool32 equal(_e3 a, _e1e2 b) { return ::math::equal(a._3, 0.f) && ::math::equal(0.f, b._4); }
bool32 equal(_e3 a, _e2e3 b) { return ::math::equal(a._3, 0.f) && ::math::equal(0.f, b._5); }
bool32 equal(_e3 a, _e3e1 b) { return ::math::equal(a._3, 0.f) && ::math::equal(0.f, b._6); }
bool32 equal(_e3 a, _e0_e1e2_e2e3_e3e1 b) { return ::math::equal(0.f, b._0) && ::math::equal(a._3, 0.f) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6); }
bool32 equal(_e3 a, _e1e2e3 b) { return ::math::equal(a._3, 0.f) && ::math::equal(0.f, b._7); }
bool32 equal(_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return ::math::equal(0.f, b._0) && ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(a._3, b._3) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6) && ::math::equal(0.f, b._7); }

bool32 equal(_e1_e2_e3 a, float32 b) { return ::math::equal(0.f, b) && ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f); }
bool32 equal(_e1_e2_e3 a, _e1 b) { return ::math::equal(a._1, b._1) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f); }
bool32 equal(_e1_e2_e3 a, _e2 b) { return ::math::equal(a._1, 0.f) && ::math::equal(a._2, b._2) && ::math::equal(a._3, 0.f); }
bool32 equal(_e1_e2_e3 a, _e3 b) { return ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, b._3); }
bool32 equal(_e1_e2_e3 a, _e1_e2_e3 b) { return ::math::equal(a._1, b._1) && ::math::equal(a._2, b._2) && ::math::equal(a._3, b._3); }
bool32 equal(_e1_e2_e3 a, _e1e2 b) { return ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f) && ::math::equal(0.f, b._4); }
bool32 equal(_e1_e2_e3 a, _e2e3 b) { return ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f) && ::math::equal(0.f, b._5); }
bool32 equal(_e1_e2_e3 a, _e3e1 b) { return ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f) && ::math::equal(0.f, b._6); }
bool32 equal(_e1_e2_e3 a, _e0_e1e2_e2e3_e3e1 b) { return ::math::equal(0.f, b._0) && ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6); }
bool32 equal(_e1_e2_e3 a, _e1e2e3 b) { return ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f) && ::math::equal(0.f, b._7); }
bool32 equal(_e1_e2_e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return ::math::equal(0.f, b._0) && ::math::equal(a._1, b._1) && ::math::equal(a._2, b._2) && ::math::equal(a._3, b._3) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6) && ::math::equal(0.f, b._7); }

bool32 equal(_e1e2 a, float32 b) { return ::math::equal(0.f, b) && ::math::equal(a._4, 0.f); }
bool32 equal(_e1e2 a, _e1 b) { return ::math::equal(0.f, b._1) && ::math::equal(a._4, 0.f); }
bool32 equal(_e1e2 a, _e2 b) { return ::math::equal(0.f, b._2) && ::math::equal(a._4, 0.f); }
bool32 equal(_e1e2 a, _e3 b) { return ::math::equal(0.f, b._3) && ::math::equal(a._4, 0.f); }
bool32 equal(_e1e2 a, _e1_e2_e3 b) { return ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3) && ::math::equal(a._4, 0.f); }
bool32 equal(_e1e2 a, _e1e2 b) { return ::math::equal(a._4, b._4); }
bool32 equal(_e1e2 a, _e2e3 b) { return ::math::equal(a._4, 0.f) && ::math::equal(0.f, b._5); }
bool32 equal(_e1e2 a, _e3e1 b) { return ::math::equal(a._4, 0.f) && ::math::equal(0.f, b._6); }
bool32 equal(_e1e2 a, _e0_e1e2_e2e3_e3e1 b) { return ::math::equal(0.f, b._0) && ::math::equal(a._4, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6); }
bool32 equal(_e1e2 a, _e1e2e3 b) { return ::math::equal(a._4, 0.f) && ::math::equal(0.f, b._7); }
bool32 equal(_e1e2 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return ::math::equal(0.f, b._0) && ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3) && ::math::equal(a._4, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6) && ::math::equal(0.f, b._7); }

bool32 equal(_e2e3 a, float32 b) { return ::math::equal(0.f, b) && ::math::equal(a._5, 0.f); }
bool32 equal(_e2e3 a, _e1 b) { return ::math::equal(0.f, b._1) && ::math::equal(a._5, 0.f); }
bool32 equal(_e2e3 a, _e2 b) { return ::math::equal(0.f, b._2) && ::math::equal(a._5, 0.f); }
bool32 equal(_e2e3 a, _e3 b) { return ::math::equal(0.f, b._3) && ::math::equal(a._5, 0.f); }
bool32 equal(_e2e3 a, _e1_e2_e3 b) { return ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3) && ::math::equal(a._5, 0.f); }
bool32 equal(_e2e3 a, _e1e2 b) { return ::math::equal(0.f, b._4) && ::math::equal(a._5, 0.f); }
bool32 equal(_e2e3 a, _e2e3 b) { return ::math::equal(a._5, b._5); }
bool32 equal(_e2e3 a, _e3e1 b) { return ::math::equal(a._5, 0.f) && ::math::equal(0.f, b._6); }
bool32 equal(_e2e3 a, _e0_e1e2_e2e3_e3e1 b) { return ::math::equal(0.f, b._0) && ::math::equal(0.f, b._4) && ::math::equal(a._5, b._5) && ::math::equal(0.f, b._6); }
bool32 equal(_e2e3 a, _e1e2e3 b) { return ::math::equal(a._5, 0.f) && ::math::equal(0.f, b._7); }
bool32 equal(_e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return ::math::equal(0.f, b._0) && ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3) && ::math::equal(0.f, b._4) && ::math::equal(a._5, b._5) && ::math::equal(0.f, b._6) && ::math::equal(0.f, b._7); }

bool32 equal(_e3e1 a, float32 b) { return ::math::equal(0.f, b) && ::math::equal(a._6, 0.f); }
bool32 equal(_e3e1 a, _e1 b) { return ::math::equal(0.f, b._1) && ::math::equal(a._6, 0.f); }
bool32 equal(_e3e1 a, _e2 b) { return ::math::equal(0.f, b._2) && ::math::equal(a._6, 0.f); }
bool32 equal(_e3e1 a, _e3 b) { return ::math::equal(0.f, b._3) && ::math::equal(a._6, 0.f); }
bool32 equal(_e3e1 a, _e1_e2_e3 b) { return ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3) && ::math::equal(a._6, 0.f); }
bool32 equal(_e3e1 a, _e1e2 b) { return ::math::equal(0.f, b._4) && ::math::equal(a._6, 0.f); }
bool32 equal(_e3e1 a, _e2e3 b) { return ::math::equal(0.f, b._5) && ::math::equal(a._6, 0.f); }
bool32 equal(_e3e1 a, _e3e1 b) { return ::math::equal(a._6, b._6); }
bool32 equal(_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { return ::math::equal(0.f, b._0) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(a._6, b._6); }
bool32 equal(_e3e1 a, _e1e2e3 b) { return ::math::equal(a._6, 0.f) && ::math::equal(0.f, b._7); }
bool32 equal(_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return ::math::equal(0.f, b._0) && ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(a._6, b._6) && ::math::equal(0.f, b._7); }

bool32 equal(_e0_e1e2_e2e3_e3e1 a, float32 b) { return ::math::equal(a._0, b) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f); }
bool32 equal(_e0_e1e2_e2e3_e3e1 a, _e1 b) { return ::math::equal(a._0, 0.f) && ::math::equal(0.f, b._1) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f); }
bool32 equal(_e0_e1e2_e2e3_e3e1 a, _e2 b) { return ::math::equal(a._0, 0.f) && ::math::equal(0.f, b._2) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f); }
bool32 equal(_e0_e1e2_e2e3_e3e1 a, _e3 b) { return ::math::equal(a._0, 0.f) && ::math::equal(0.f, b._3) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f); }
bool32 equal(_e0_e1e2_e2e3_e3e1 a, _e1_e2_e3 b) { return ::math::equal(a._0, 0.f) && ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f); }
bool32 equal(_e0_e1e2_e2e3_e3e1 a, _e1e2 b) { return ::math::equal(a._0, 0.f) && ::math::equal(a._4, b._4) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f); }
bool32 equal(_e0_e1e2_e2e3_e3e1 a, _e2e3 b) { return ::math::equal(a._0, 0.f) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, b._5) && ::math::equal(a._6, 0.f); }
bool32 equal(_e0_e1e2_e2e3_e3e1 a, _e3e1 b) { return ::math::equal(a._0, 0.f) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, b._6); }
bool32 equal(_e0_e1e2_e2e3_e3e1 a, _e0_e1e2_e2e3_e3e1 b) { return ::math::equal(a._0, b._0) && ::math::equal(a._4, b._4) && ::math::equal(a._5, b._5) && ::math::equal(a._6, b._6); }
bool32 equal(_e0_e1e2_e2e3_e3e1 a, _e1e2e3 b) { return ::math::equal(a._0, 0.f) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f) && ::math::equal(0.f, b._7); }
bool32 equal(_e0_e1e2_e2e3_e3e1 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return ::math::equal(a._0, b._0) && ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3) && ::math::equal(a._4, b._4) && ::math::equal(a._5, b._5) && ::math::equal(a._6, b._6) && ::math::equal(0.f, b._7); }

bool32 equal(_e1e2e3 a, float32 b) { return ::math::equal(0.f, b) && ::math::equal(a._7, 0.f); }
bool32 equal(_e1e2e3 a, _e1 b) { return ::math::equal(0.f, b._1) && ::math::equal(a._7, 0.f); }
bool32 equal(_e1e2e3 a, _e2 b) { return ::math::equal(0.f, b._2) && ::math::equal(a._7, 0.f); }
bool32 equal(_e1e2e3 a, _e3 b) { return ::math::equal(0.f, b._3) && ::math::equal(a._7, 0.f); }
bool32 equal(_e1e2e3 a, _e1_e2_e3 b) { return ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3) && ::math::equal(a._7, 0.f); }
bool32 equal(_e1e2e3 a, _e1e2 b) { return ::math::equal(0.f, b._4) && ::math::equal(a._7, 0.f); }
bool32 equal(_e1e2e3 a, _e2e3 b) { return ::math::equal(0.f, b._5) && ::math::equal(a._7, 0.f); }
bool32 equal(_e1e2e3 a, _e3e1 b) { return ::math::equal(0.f, b._6) && ::math::equal(a._7, 0.f); }
bool32 equal(_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { return ::math::equal(0.f, b._0) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6) && ::math::equal(a._7, 0.f); }
bool32 equal(_e1e2e3 a, _e1e2e3 b) { return ::math::equal(a._7, b._7); }
bool32 equal(_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return ::math::equal(0.f, b._0) && ::math::equal(0.f, b._1) && ::math::equal(0.f, b._2) && ::math::equal(0.f, b._3) && ::math::equal(0.f, b._4) && ::math::equal(0.f, b._5) && ::math::equal(0.f, b._6) && ::math::equal(a._7, b._7); }

bool32 equal(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, float32 b) { return ::math::equal(a._0, b) && ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f) && ::math::equal(a._7, 0.f); }
bool32 equal(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1 b) { return ::math::equal(a._0, 0.f) && ::math::equal(a._1, b._1) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f) && ::math::equal(a._7, 0.f); }
bool32 equal(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2 b) { return ::math::equal(a._0, 0.f) && ::math::equal(a._1, 0.f) && ::math::equal(a._2, b._2) && ::math::equal(a._3, 0.f) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f) && ::math::equal(a._7, 0.f); }
bool32 equal(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3 b) { return ::math::equal(a._0, 0.f) && ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, b._3) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f) && ::math::equal(a._7, 0.f); }
bool32 equal(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1_e2_e3 b) { return ::math::equal(a._0, 0.f) && ::math::equal(a._1, b._1) && ::math::equal(a._2, b._2) && ::math::equal(a._3, b._3) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f) && ::math::equal(a._7, 0.f); }
bool32 equal(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2 b) { return ::math::equal(a._0, 0.f) && ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f) && ::math::equal(a._4, b._4) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f) && ::math::equal(a._7, 0.f); }
bool32 equal(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e2e3 b) { return ::math::equal(a._0, 0.f) && ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, b._5) && ::math::equal(a._6, 0.f) && ::math::equal(a._7, 0.f); }
bool32 equal(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e3e1 b) { return ::math::equal(a._0, 0.f) && ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, b._6) && ::math::equal(a._7, 0.f); }
bool32 equal(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1e2_e2e3_e3e1 b) { return ::math::equal(a._0, b._0) && ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f) && ::math::equal(a._4, b._4) && ::math::equal(a._5, b._5) && ::math::equal(a._6, b._6) && ::math::equal(a._7, 0.f); }
bool32 equal(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e1e2e3 b) { return ::math::equal(a._0, 0.f) && ::math::equal(a._1, 0.f) && ::math::equal(a._2, 0.f) && ::math::equal(a._3, 0.f) && ::math::equal(a._4, 0.f) && ::math::equal(a._5, 0.f) && ::math::equal(a._6, 0.f) && ::math::equal(a._7, b._7); }
bool32 equal(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a, _e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 b) { return ::math::equal(a._0, b._0) && ::math::equal(a._1, b._1) && ::math::equal(a._2, b._2) && ::math::equal(a._3, b._3) && ::math::equal(a._4, b._4) && ::math::equal(a._5, b._5) && ::math::equal(a._6, b._6) && ::math::equal(a._7, b._7); }

float32 length_squared(_e1 a) { float32 r = inner(a, conjugated(a)); return r; }
float32 length_squared(_e2 a) { float32 r = inner(a, conjugated(a)); return r; }
float32 length_squared(_e3 a) { float32 r = inner(a, conjugated(a)); return r; }
float32 length_squared(_e1_e2_e3 a) { float32 r = inner(a, conjugated(a)); return r; }
float32 length_squared(_e1e2 a) { float32 r = inner(a, conjugated(a)); return r; }
float32 length_squared(_e2e3 a) { float32 r = inner(a, conjugated(a)); return r; }
float32 length_squared(_e3e1 a) { float32 r = inner(a, conjugated(a)); return r; }
float32 length_squared(_e0_e1e2_e2e3_e3e1 a) { float32 r = inner(a, conjugated(a)); return r; }
float32 length_squared(_e1e2e3 a) { float32 r = inner(a, conjugated(a)); return r; }
float32 length_squared(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a) { float32 r = inner(a, conjugated(a)); return r; }

float32 length(_e1 a) { return ::math::absolute(a._1); }
float32 length(_e2 a) { return ::math::absolute(a._2); }
float32 length(_e3 a) { return ::math::absolute(a._3); }
float32 length(_e1_e2_e3 a) { return square_root(length_squared(a)); }
float32 length(_e1e2 a) { return ::math::absolute(a._4); }
float32 length(_e2e3 a) { return ::math::absolute(a._5); }
float32 length(_e3e1 a) { return ::math::absolute(a._6); }
float32 length(_e0_e1e2_e2e3_e3e1 a) { return square_root(length_squared(a)); }
float32 length(_e1e2e3 a) { return ::math::absolute(a._7); }
float32 length(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a) { return square_root(length_squared(a)); }

void normalize(_e1& a) { float32 norm = length(a); a._1 /= norm; }
void normalize(_e2& a) { float32 norm = length(a); a._2 /= norm; }
void normalize(_e3& a) { float32 norm = length(a); a._3 /= norm; }
void normalize(_e1_e2_e3& a) { float32 norm = length(a); a._1 /= norm; a._2 /= norm; a._3 /= norm; }
void normalize(_e1e2& a) { float32 norm = length(a); a._4 /= norm; }
void normalize(_e2e3& a) { float32 norm = length(a); a._5 /= norm; }
void normalize(_e3e1& a) { float32 norm = length(a); a._6 /= norm; }
void normalize(_e0_e1e2_e2e3_e3e1& a) { float32 norm = length(a); a._0 /= norm; a._4 /= norm; a._5 /= norm; a._6 /= norm; }
void normalize(_e1e2e3& a) { float32 norm = length(a); a._7 /= norm; }
void normalize(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3& a) { float32 norm = length(a); a._0 /= norm; a._1 /= norm; a._2 /= norm; a._3 /= norm; a._4 /= norm; a._5 /= norm; a._6 /= norm; a._7 /= norm; }

_e1 normalized(_e1 a) { normalize(a); return a; }
_e2 normalized(_e2 a) { normalize(a); return a; }
_e3 normalized(_e3 a) { normalize(a); return a; }
_e1_e2_e3 normalized(_e1_e2_e3 a) { normalize(a); return a; }
_e1e2 normalized(_e1e2 a) { normalize(a); return a; }
_e2e3 normalized(_e2e3 a) { normalize(a); return a; }
_e3e1 normalized(_e3e1 a) { normalize(a); return a; }
_e0_e1e2_e2e3_e3e1 normalized(_e0_e1e2_e2e3_e3e1 a) { normalize(a); return a; }
_e1e2e3 normalized(_e1e2e3 a) { normalize(a); return a; }
_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 normalized(_e0_e1_e2_e3_e1e2_e2e3_e3e1_e1e2e3 a) { normalize(a); return a; }

