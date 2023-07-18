_e0_e1_e2_e1e2 operator + (_e1 a, float32 x) { _e0_e1_e2_e1e2 r; r._0 = x; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; return r; }
_e0_e1_e2_e1e2 operator + (float32 x, _e1 a) { _e0_e1_e2_e1e2 r; r._0 = x; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; return r; }
_e0_e1_e2_e1e2 operator + (_e2 a, float32 x) { _e0_e1_e2_e1e2 r; r._0 = x; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; return r; }
_e0_e1_e2_e1e2 operator + (float32 x, _e2 a) { _e0_e1_e2_e1e2 r; r._0 = x; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; return r; }
_e0_e1_e2_e1e2 operator + (_e1_e2 a, float32 x) { _e0_e1_e2_e1e2 r; r._0 = x; r._1 = a._1; r._2 = a._2; r._3 = 0.f; return r; }
_e0_e1_e2_e1e2 operator + (float32 x, _e1_e2 a) { _e0_e1_e2_e1e2 r; r._0 = x; r._1 = a._1; r._2 = a._2; r._3 = 0.f; return r; }
_e0_e1e2 operator + (_e1e2 a, float32 x) { _e0_e1e2 r; r._0 = x; r._3 = a._3; return r; }
_e0_e1e2 operator + (float32 x, _e1e2 a) { _e0_e1e2 r; r._0 = x; r._3 = a._3; return r; }
_e0_e1e2 operator + (_e0_e1e2 a, float32 x) { _e0_e1e2 r; r._0 = a._0 + x; r._3 = a._3; return r; }
_e0_e1e2 operator + (float32 x, _e0_e1e2 a) { _e0_e1e2 r; r._0 = a._0 + x; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, float32 x) { _e0_e1_e2_e1e2 r; r._0 = a._0 + x; r._1 = a._1; r._2 = a._2; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator + (float32 x, _e0_e1_e2_e1e2 a) { _e0_e1_e2_e1e2 r; r._0 = a._0 + x; r._1 = a._1; r._2 = a._2; r._3 = a._3; return r; }

_e0_e1_e2_e1e2 operator - (_e1 a, float32 x) { _e0_e1_e2_e1e2 r; r._0 = -x; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; return r; }
_e0_e1_e2_e1e2 operator - (float32 x, _e1 a) { _e0_e1_e2_e1e2 r; r._0 = -x; r._1 = a._1; r._2 = 0.f; r._3 = 0.f; return r; }
_e0_e1_e2_e1e2 operator - (_e2 a, float32 x) { _e0_e1_e2_e1e2 r; r._0 = -x; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; return r; }
_e0_e1_e2_e1e2 operator - (float32 x, _e2 a) { _e0_e1_e2_e1e2 r; r._0 = -x; r._1 = 0.f; r._2 = a._2; r._3 = 0.f; return r; }
_e0_e1_e2_e1e2 operator - (_e1_e2 a, float32 x) { _e0_e1_e2_e1e2 r; r._0 = -x; r._1 = a._1; r._2 = a._2; r._3 = 0.f; return r; }
_e0_e1_e2_e1e2 operator - (float32 x, _e1_e2 a) { _e0_e1_e2_e1e2 r; r._0 = -x; r._1 = a._1; r._2 = a._2; r._3 = 0.f; return r; }
_e0_e1e2 operator - (_e1e2 a, float32 x) { _e0_e1e2 r; r._0 = -x; r._3 = a._3; return r; }
_e0_e1e2 operator - (float32 x, _e1e2 a) { _e0_e1e2 r; r._0 = -x; r._3 = a._3; return r; }
_e0_e1e2 operator - (_e0_e1e2 a, float32 x) { _e0_e1e2 r; r._0 = a._0 - x; r._3 = a._3; return r; }
_e0_e1e2 operator - (float32 x, _e0_e1e2 a) { _e0_e1e2 r; r._0 = a._0 - x; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, float32 x) { _e0_e1_e2_e1e2 r; r._0 = a._0 - x; r._1 = a._1; r._2 = a._2; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator - (float32 x, _e0_e1_e2_e1e2 a) { _e0_e1_e2_e1e2 r; r._0 = a._0 - x; r._1 = a._1; r._2 = a._2; r._3 = a._3; return r; }

_e1 operator * (_e1 a, float32 x) { _e1 r; r._1 = x * a._1; return r; }
_e1 operator * (float32 x, _e1 a) { _e1 r; r._1 = x * a._1; return r; }
_e2 operator * (_e2 a, float32 x) { _e2 r; r._2 = x * a._2; return r; }
_e2 operator * (float32 x, _e2 a) { _e2 r; r._2 = x * a._2; return r; }
_e1_e2 operator * (_e1_e2 a, float32 x) { _e1_e2 r; r._1 = x * a._1; r._2 = x * a._2; return r; }
_e1_e2 operator * (float32 x, _e1_e2 a) { _e1_e2 r; r._1 = x * a._1; r._2 = x * a._2; return r; }
_e1e2 operator * (_e1e2 a, float32 x) { _e1e2 r; r._3 = x * a._3; return r; }
_e1e2 operator * (float32 x, _e1e2 a) { _e1e2 r; r._3 = x * a._3; return r; }
_e0_e1e2 operator * (_e0_e1e2 a, float32 x) { _e0_e1e2 r; r._0 = x * a._0; r._3 = x * a._3; return r; }
_e0_e1e2 operator * (float32 x, _e0_e1e2 a) { _e0_e1e2 r; r._0 = x * a._0; r._3 = x * a._3; return r; }
_e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, float32 x) { _e0_e1_e2_e1e2 r; r._0 = x * a._0; r._1 = x * a._1; r._2 = x * a._2; r._3 = x * a._3; return r; }
_e0_e1_e2_e1e2 operator * (float32 x, _e0_e1_e2_e1e2 a) { _e0_e1_e2_e1e2 r; r._0 = x * a._0; r._1 = x * a._1; r._2 = x * a._2; r._3 = x * a._3; return r; }

_e1 operator / (_e1 a, float32 x) { _e1 r; r._1 = x / a._1; return r; }
_e2 operator / (_e2 a, float32 x) { _e2 r; r._2 = x / a._2; return r; }
_e1_e2 operator / (_e1_e2 a, float32 x) { _e1_e2 r; r._1 = x / a._1; r._2 = x / a._2; return r; }
_e1e2 operator / (_e1e2 a, float32 x) { _e1e2 r; r._3 = x / a._3; return r; }
_e0_e1e2 operator / (_e0_e1e2 a, float32 x) { _e0_e1e2 r; r._0 = x / a._0; r._3 = x / a._3; return r; }
_e0_e1_e2_e1e2 operator / (_e0_e1_e2_e1e2 a, float32 x) { _e0_e1_e2_e1e2 r; r._0 = x / a._0; r._1 = x / a._1; r._2 = x / a._2; r._3 = x / a._3; return r; }

_e1 operator - (_e1 a) { _e1 r; r._1 = -a._1; return r; }
_e2 operator - (_e2 a) { _e2 r; r._2 = -a._2; return r; }
_e1_e2 operator - (_e1_e2 a) { _e1_e2 r; r._1 = -a._1; r._2 = -a._2; return r; }
_e1e2 operator - (_e1e2 a) { _e1e2 r; r._3 = -a._3; return r; }
_e0_e1e2 operator - (_e0_e1e2 a) { _e0_e1e2 r; r._0 = -a._0; r._3 = -a._3; return r; }
_e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a) { _e0_e1_e2_e1e2 r; r._0 = -a._0; r._1 = -a._1; r._2 = -a._2; r._3 = -a._3; return r; }

_e1 operator + (_e1 a, _e1 b) { _e1 r; r._1 = a._1 + b._1; return r; }
_e1_e2 operator + (_e1 a, _e2 b) { _e1_e2 r; r._1 = a._1; r._2 = b._2; return r; }
_e1_e2 operator + (_e1 a, _e1_e2 b) { _e1_e2 r; r._1 = a._1 + b._1; r._2 = b._2; return r; }
_e0_e1_e2_e1e2 operator + (_e1 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = a._1; r._2 = 0.f; r._3 = b._3; return r; }
_e0_e1_e2_e1e2 operator + (_e1 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = b._0; r._1 = a._1; r._2 = 0.f; r._3 = b._3; return r; }
_e0_e1_e2_e1e2 operator + (_e1 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = b._0; r._1 = a._1 + b._1; r._2 = b._2; r._3 = b._3; return r; }

_e1_e2 operator + (_e2 a, _e1 b) { _e1_e2 r; r._1 = b._1; r._2 = a._2; return r; }
_e2 operator + (_e2 a, _e2 b) { _e2 r; r._2 = a._2 + b._2; return r; }
_e1_e2 operator + (_e2 a, _e1_e2 b) { _e1_e2 r; r._1 = b._1; r._2 = a._2 + b._2; return r; }
_e0_e1_e2_e1e2 operator + (_e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 = b._3; return r; }
_e0_e1_e2_e1e2 operator + (_e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = b._0; r._1 = 0.f; r._2 = a._2; r._3 = b._3; return r; }
_e0_e1_e2_e1e2 operator + (_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = b._0; r._1 = b._1; r._2 = a._2 + b._2; r._3 = b._3; return r; }

_e1_e2 operator + (_e1_e2 a, _e1 b) { _e1_e2 r; r._1 = a._1 + b._1; r._2 = a._2; return r; }
_e1_e2 operator + (_e1_e2 a, _e2 b) { _e1_e2 r; r._1 = a._1; r._2 = a._2 + b._2; return r; }
_e1_e2 operator + (_e1_e2 a, _e1_e2 b) { _e1_e2 r; r._1 = a._1 + b._1; r._2 = a._2 + b._2; return r; }
_e0_e1_e2_e1e2 operator + (_e1_e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = a._1; r._2 = a._2; r._3 = b._3; return r; }
_e0_e1_e2_e1e2 operator + (_e1_e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = b._0; r._1 = a._1; r._2 = a._2; r._3 = b._3; return r; }
_e0_e1_e2_e1e2 operator + (_e1_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = b._0; r._1 = a._1 + b._1; r._2 = a._2 + b._2; r._3 = b._3; return r; }

_e0_e1_e2_e1e2 operator + (_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = b._1; r._2 = 0.f; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator + (_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = 0.f; r._2 = b._2; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator + (_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = b._1; r._2 = b._2; r._3 = a._3; return r; }
_e1e2 operator + (_e1e2 a, _e1e2 b) { _e1e2 r; r._3 = a._3 + b._3; return r; }
_e0_e1e2 operator + (_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = b._0; r._3 = a._3 + b._3; return r; }
_e0_e1_e2_e1e2 operator + (_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = b._0; r._1 = b._1; r._2 = b._2; r._3 = a._3 + b._3; return r; }

_e0_e1_e2_e1e2 operator + (_e0_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = b._1; r._2 = 0.f; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator + (_e0_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = 0.f; r._2 = b._2; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator + (_e0_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = b._1; r._2 = b._2; r._3 = a._3; return r; }
_e0_e1e2 operator + (_e0_e1e2 a, _e1e2 b) { _e0_e1e2 r; r._0 = a._0; r._3 = a._3 + b._3; return r; }
_e0_e1e2 operator + (_e0_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = a._0 + b._0; r._3 = a._3 + b._3; return r; }
_e0_e1_e2_e1e2 operator + (_e0_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0 + b._0; r._1 = b._1; r._2 = b._2; r._3 = a._3 + b._3; return r; }

_e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = a._1 + b._1; r._2 = a._2; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = a._1; r._2 = a._2 + b._2; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = a._1 + b._1; r._2 = a._2 + b._2; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = a._3 + b._3; return r; }
_e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0 + b._0; r._1 = a._1; r._2 = a._2; r._3 = a._3 + b._3; return r; }
_e0_e1_e2_e1e2 operator + (_e0_e1_e2_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0 + b._0; r._1 = a._1 + b._1; r._2 = a._2 + b._2; r._3 = a._3 + b._3; return r; }

_e1 operator - (_e1 a, _e1 b) { _e1 r; r._1 = a._1 - b._1; return r; }
_e1_e2 operator - (_e1 a, _e2 b) { _e1_e2 r; r._1 = a._1; r._2 = -b._2; return r; }
_e1_e2 operator - (_e1 a, _e1_e2 b) { _e1_e2 r; r._1 = a._1 - b._1; r._2 = -b._2; return r; }
_e0_e1_e2_e1e2 operator - (_e1 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = a._1; r._2 = 0.f; r._3 = -b._3; return r; }
_e0_e1_e2_e1e2 operator - (_e1 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = a._1; r._2 = 0.f; r._3 = -b._3; return r; }
_e0_e1_e2_e1e2 operator - (_e1 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = a._1 - b._1; r._2 = -b._2; r._3 = -b._3; return r; }

_e1_e2 operator - (_e2 a, _e1 b) { _e1_e2 r; r._1 = -b._1; r._2 = a._2; return r; }
_e2 operator - (_e2 a, _e2 b) { _e2 r; r._2 = a._2 - b._2; return r; }
_e1_e2 operator - (_e2 a, _e1_e2 b) { _e1_e2 r; r._1 = -b._1; r._2 = a._2 - b._2; return r; }
_e0_e1_e2_e1e2 operator - (_e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = 0.f; r._2 = a._2; r._3 = -b._3; return r; }
_e0_e1_e2_e1e2 operator - (_e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = 0.f; r._2 = a._2; r._3 = -b._3; return r; }
_e0_e1_e2_e1e2 operator - (_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = -b._1; r._2 = a._2 - b._2; r._3 = -b._3; return r; }

_e1_e2 operator - (_e1_e2 a, _e1 b) { _e1_e2 r; r._1 = a._1 - b._1; r._2 = a._2; return r; }
_e1_e2 operator - (_e1_e2 a, _e2 b) { _e1_e2 r; r._1 = a._1; r._2 = a._2 - b._2; return r; }
_e1_e2 operator - (_e1_e2 a, _e1_e2 b) { _e1_e2 r; r._1 = a._1 - b._1; r._2 = a._2 - b._2; return r; }
_e0_e1_e2_e1e2 operator - (_e1_e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = a._1; r._2 = a._2; r._3 = -b._3; return r; }
_e0_e1_e2_e1e2 operator - (_e1_e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = a._1; r._2 = a._2; r._3 = -b._3; return r; }
_e0_e1_e2_e1e2 operator - (_e1_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = a._1 - b._1; r._2 = a._2 - b._2; r._3 = -b._3; return r; }

_e0_e1_e2_e1e2 operator - (_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = -b._1; r._2 = 0.f; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator - (_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = 0.f;  r._2 = -b._2; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator - (_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = 0.f; r._1 = -b._1; r._2 = -b._2; r._3 = a._3; return r; }
_e1e2 operator - (_e1e2 a, _e1e2 b) { _e1e2 r; r._3 = a._3 - b._3; return r; }
_e0_e1e2 operator - (_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = -b._0; r._3 = a._3 - b._3; return r; }
_e0_e1_e2_e1e2 operator - (_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = -b._0; r._1 = -b._1; r._2 = -b._2; r._3 = a._3 - b._3; return r; }

_e0_e1_e2_e1e2 operator - (_e0_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = -b._1; r._2 = 0.f; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator - (_e0_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = 0.f; r._2 = -b._2; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator - (_e0_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = -b._1; r._2 = -b._2; r._3 = a._3; return r; }
_e0_e1e2 operator - (_e0_e1e2 a, _e1e2 b) { _e0_e1e2 r; r._0 = a._0; r._3 = a._3 - b._3; return r; }
_e0_e1e2 operator - (_e0_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = a._0 - b._0; r._3 = a._3 - b._3; return r; }
_e0_e1_e2_e1e2 operator - (_e0_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0 - b._0; r._1 = -b._1; r._2 = -b._2; r._3 = a._3 - b._3; return r; }

_e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = a._1 - b._1; r._2 = a._2; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = a._1; r._2 = a._2 - b._2; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = a._1 - b._1; r._2 = a._2 - b._2; r._3 = a._3; return r; }
_e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0; r._1 = a._1; r._2 = a._2; r._3 = a._3 - b._3; return r; }
_e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0 - b._0; r._1 = a._1; r._2 = a._2; r._3 = a._3 - b._3; return r; }
_e0_e1_e2_e1e2 operator - (_e0_e1_e2_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r; r._0 = a._0 - b._0; r._1 = a._1 - b._1; r._2 = a._2 - b._2; r._3 = a._3 - b._3; return r; }

float32 inner(float32 a, float32 b) { return a * b; }
float32 inner(float32 a, _e1 b) { return 0.f; }
float32 inner(float32 a, _e2 b) { return 0.f; }
float32 inner(float32 a, _e1_e2 b) { return 0.f; }
float32 inner(float32 a, _e1e2 b) { return 0.f; }
float32 inner(float32 a, _e0_e1e2 b) { return a * b._0; }
float32 inner(float32 a, _e0_e1_e2_e1e2 b) { return a * b._0; }

float32 inner(_e1 a, float32 b) { return 0.f; }
float32 inner(_e1 a, _e1 b) { return a._1 * b._1; }
float32 inner(_e1 a, _e2 b) { return 0.f; }
float32 inner(_e1 a, _e1_e2 b) { return a._1 * b._1; }
float32 inner(_e1 a, _e1e2 b) { return 0.f; }
float32 inner(_e1 a, _e0_e1e2 b) { return 0.f; }
float32 inner(_e1 a, _e0_e1_e2_e1e2 b) { return a._1 * b._1; }

float32 inner(_e2 a, float32 b) { return 0.f; }
float32 inner(_e2 a, _e1 b) { return 0.f; }
float32 inner(_e2 a, _e2 b) { return a._2 * b._2; }
float32 inner(_e2 a, _e1_e2 b) { return a._2 * b._2; }
float32 inner(_e2 a, _e1e2 b) { return 0.f; }
float32 inner(_e2 a, _e0_e1e2 b) { return 0.f; }
float32 inner(_e2 a, _e0_e1_e2_e1e2 b) { return a._2 * b._2; }

float32 inner(_e1_e2 a, float32 b) { return 0.f; }
float32 inner(_e1_e2 a, _e1 b) { return a._1 * b._1; }
float32 inner(_e1_e2 a, _e2 b) { return a._2 * b._2; }
float32 inner(_e1_e2 a, _e1_e2 b) { return (a._1 * b._1) + (a._2 * b._2); }
float32 inner(_e1_e2 a, _e1e2 b) { return 0.f; }
float32 inner(_e1_e2 a, _e0_e1e2 b) { return 0.f; }
float32 inner(_e1_e2 a, _e0_e1_e2_e1e2 b) { return (a._1 * b._1) + (a._2 * b._2); }

float32 inner(_e1e2 a, float32 b) { return 0.f; }
float32 inner(_e1e2 a, _e1 b) { return 0.f; }
float32 inner(_e1e2 a, _e2 b) { return 0.f; }
float32 inner(_e1e2 a, _e1_e2 b) { return 0.f; }
float32 inner(_e1e2 a, _e1e2 b) { return -(a._3 * b._3); }
float32 inner(_e1e2 a, _e0_e1e2 b) { return -(a._3 * b._3); }
float32 inner(_e1e2 a, _e0_e1_e2_e1e2 b) { return -(a._3 * b._3); }

float32 inner(_e0_e1e2 a, float32 b) { return (a._0 * b); }
float32 inner(_e0_e1e2 a, _e1 b) { return 0.f; }
float32 inner(_e0_e1e2 a, _e2 b) { return 0.f; }
float32 inner(_e0_e1e2 a, _e1_e2 b) { return 0.f; }
float32 inner(_e0_e1e2 a, _e1e2 b) { return -(a._3 * b._3); }
float32 inner(_e0_e1e2 a, _e0_e1e2 b) { return (a._0 * b._0) - (a._3 * b._3); }
float32 inner(_e0_e1e2 a, _e0_e1_e2_e1e2 b) { return (a._0 * b._0) - (a._3 * b._3); }

float32 inner(_e0_e1_e2_e1e2 a, float32 b) { return (a._0 * b); }
float32 inner(_e0_e1_e2_e1e2 a, _e1 b) { return (a._1 * b._1); }
float32 inner(_e0_e1_e2_e1e2 a, _e2 b) { return (a._2 * b._2); }
float32 inner(_e0_e1_e2_e1e2 a, _e1_e2 b) { return (a._1 * b._1) + (a._2 * b._2); }
float32 inner(_e0_e1_e2_e1e2 a, _e1e2 b) { return -(a._3 * b._3); }
float32 inner(_e0_e1_e2_e1e2 a, _e0_e1e2 b) { return (a._0 * b._0) - (a._3 * b._3); }
float32 inner(_e0_e1_e2_e1e2 a, _e0_e1_e2_e1e2 b) { return (a._0 * b._0) + (a._1 * b._1) + (a._2 * b._2) - (a._3 * b._3); }

float32 outer(float32 a, float32 b) { return (a * b); }
_e0_e1_e2_e1e2 outer(float32 a, _e1 b) { _e0_e1_e2_e1e2 r = {}; r._1 = a * b._1; return r; }
_e0_e1_e2_e1e2 outer(float32 a, _e2 b) { _e0_e1_e2_e1e2 r = {}; r._2 = a * b._2; return r; }
_e0_e1_e2_e1e2 outer(float32 a, _e1_e2 b) { _e0_e1_e2_e1e2 r = {}; r._1 = a * b._1; r._2 = a * b._2; return r; }
_e0_e1e2 outer(float32 a, _e1e2 b) { _e0_e1e2 r = {}; r._3 = a * b._3; return r; }
_e0_e1e2 outer(float32 a, _e0_e1e2 b) { _e0_e1e2 r = {}; r._0 = a * b._0; r._3 = a * b._3; return r; }
_e0_e1_e2_e1e2 outer(float32 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._0 = a * b._0; r._1 = a * b._1; r._2 = a * b._2; r._3 = a * b._3; return r; }

_e0_e1_e2_e1e2 outer(_e1 a, float32 b) { _e0_e1_e2_e1e2 r = {}; r._1 = a._1 * b; return r; }
float32 outer(_e1 a, _e1 b) { return 0.f; }
_e0_e1e2 outer(_e1 a, _e2 b) { _e0_e1e2 r = {}; r._3 = a._1 * b._2; return r; }
_e0_e1e2 outer(_e1 a, _e1_e2 b) { _e0_e1e2 r = {}; r._3 = a._1 * b._2; return r; }
float32 outer(_e1 a, _e1e2 b) { return 0.f; }
_e0_e1_e2_e1e2 outer(_e1 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._1 = a._1 * b._0; return r; }
_e0_e1_e2_e1e2 outer(_e1 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._1 = a._1 * b._0; r._3 = a._1 * b._2; return r; }

_e0_e1_e2_e1e2 outer(_e2 a, float32 b) { _e0_e1_e2_e1e2 r = {}; r._2 += a._2 * b; return r; }
_e0_e1e2 outer(_e2 a, _e1 b) { _e0_e1e2 r = {}; r._3 -= a._2 * b._1; return r; }
float32 outer(_e2 a, _e2 b) { return 0.f; }
_e0_e1e2 outer(_e2 a, _e1_e2 b) { _e0_e1e2 r = {}; r._3 -= a._2 * b._1; return r; }
float32 outer(_e2 a, _e1e2 b) { return 0.f; }
_e0_e1_e2_e1e2 outer(_e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._2 += a._2 * b._0; return r; }
_e0_e1_e2_e1e2 outer(_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._2 += a._2 * b._0; r._3 -= a._2 * b._1; return r; }

_e0_e1_e2_e1e2 outer(_e1_e2 a, float32 b) { _e0_e1_e2_e1e2 r = {}; r._1 += a._1 * b; r._2 += a._2 * b; return r; }
_e0_e1e2 outer(_e1_e2 a, _e1 b) { _e0_e1e2 r = {}; r._3 -= a._2 * b._1; return r; }
_e0_e1e2 outer(_e1_e2 a, _e2 b) { _e0_e1e2 r = {}; r._3 += a._1 * b._2; return r; }
_e0_e1e2 outer(_e1_e2 a, _e1_e2 b) { _e0_e1e2 r = {}; r._3 += a._1 * b._2; r._3 -= a._2 * b._1; return r; }
float32 outer(_e1_e2 a, _e1e2 b) { return 0.f; }
_e0_e1_e2_e1e2 outer(_e1_e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._1 += a._1 * b._0; r._2 += a._2 * b._0; return r; }
_e0_e1_e2_e1e2 outer(_e1_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._1 += a._1 * b._0; r._3 += a._1 * b._2; r._2 += a._2 * b._0; r._3 -= a._2 * b._1; return r; }

_e0_e1e2 outer(_e1e2 a, float32 b) { _e0_e1e2 r = {}; r._3 += a._3 * b; return r; }
float32 outer(_e1e2 a, _e1 b) { return 0.f; }
float32 outer(_e1e2 a, _e2 b) { return 0.f; }
float32 outer(_e1e2 a, _e1_e2 b) { return 0.f; }
float32 outer(_e1e2 a, _e1e2 b) { return 0.f; }
_e0_e1e2 outer(_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r = {}; r._3 += a._3 * b._0; return r; }
_e0_e1e2 outer(_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1e2 r = {}; r._3 += a._3 * b._0; return r; }

_e0_e1e2 outer(_e0_e1e2 a, float32 b) { _e0_e1e2 r = {}; r._0 += a._0 * b; r._3 += a._3 * b; return r; }
_e0_e1_e2_e1e2 outer(_e0_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r = {}; r._1 += a._0 * b._1; return r; }
_e0_e1_e2_e1e2 outer(_e0_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r = {}; r._2 += a._0 * b._2; return r; }
_e0_e1_e2_e1e2 outer(_e0_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r = {}; r._1 += a._0 * b._1; r._2 += a._0 * b._2; return r; }
_e0_e1e2 outer(_e0_e1e2 a, _e1e2 b) { _e0_e1e2 r = {}; r._3 += a._0 * b._3; return r; }
_e0_e1e2 outer(_e0_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r = {}; r._0 += a._0 * b._0; r._3 += a._0 * b._3; r._3 += a._3 * b._0; return r; }
_e0_e1_e2_e1e2 outer(_e0_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._0 += a._0 * b._0; r._1 += a._0 * b._1; r._2 += a._0 * b._2; r._3 += a._0 * b._3; r._3 += a._3 * b._0; return r; }

_e0_e1_e2_e1e2 outer(_e0_e1_e2_e1e2 a, float32 b) { _e0_e1_e2_e1e2 r = {}; r._0 += a._0 * b; r._1 += a._1 * b; r._2 += a._2 * b; r._3 += a._3 * b; return r; }
_e0_e1_e2_e1e2 outer(_e0_e1_e2_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r = {}; r._1 += a._0 * b._1; r._3 -= a._2 * b._1; return r; }
_e0_e1_e2_e1e2 outer(_e0_e1_e2_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r = {}; r._2 += a._0 * b._2; r._3 += a._1 * b._2; return r; }
_e0_e1_e2_e1e2 outer(_e0_e1_e2_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r = {}; r._1 += a._0 * b._1; r._2 += a._0 * b._2; r._3 += a._1 * b._2; r._3 -= a._2 * b._1; return r; }
_e0_e1e2 outer(_e0_e1_e2_e1e2 a, _e1e2 b) { _e0_e1e2 r = {}; r._3 += a._0 * b._3; return r; }
_e0_e1_e2_e1e2 outer(_e0_e1_e2_e1e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._0 += a._0 * b._0; r._3 += a._0 * b._3; r._1 += a._1 * b._0; r._2 += a._2 * b._0; r._3 += a._3 * b._0; return r; }
_e0_e1_e2_e1e2 outer(_e0_e1_e2_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._0 += a._0 * b._0; r._1 += a._0 * b._1; r._2 += a._0 * b._2; r._3 += a._0 * b._3; r._1 += a._1 * b._0; r._3 += a._1 * b._2; r._2 += a._2 * b._0; r._3 -= a._2 * b._1; r._3 += a._3 * b._0; return r; }

float32 operator * (_e1 a, _e1 b) { return (a._1 * b._1); }
_e1e2 operator * (_e1 a, _e2 b) { _e1e2 r = {}; r._3 += a._1 * b._2; return r; }
_e0_e1e2 operator * (_e1 a, _e1_e2 b) { _e0_e1e2 r = {}; r._0 += a._1 * b._1; r._3 += a._1 * b._2; return r; }
_e2 operator * (_e1 a, _e1e2 b) { _e2 r = {}; r._2 += a._1 * b._3; return r; }
_e1_e2 operator * (_e1 a, _e0_e1e2 b) { _e1_e2 r = {}; r._1 += a._1 * b._0; r._2 += a._1 * b._3; return r; }
_e0_e1_e2_e1e2 operator * (_e1 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._1 += a._1 * b._0; r._0 += a._1 * b._1; r._3 += a._1 * b._2; r._2 += a._1 * b._3; return r; }

_e1e2 operator * (_e2 a, _e1 b) { _e1e2 r = {}; r._3 -= a._2 * b._1; return r; }
float32 operator * (_e2 a, _e2 b) { return (a._2 * b._2); }
_e0_e1e2 operator * (_e2 a, _e1_e2 b) { _e0_e1e2 r = {}; r._3 -= a._2 * b._1; r._0 += a._2 * b._2; return r; }
_e1 operator * (_e2 a, _e1e2 b) { _e1 r = {}; r._1 -= a._2 * b._3; return r; }
_e1_e2 operator * (_e2 a, _e0_e1e2 b) { _e1_e2 r = {}; r._2 += a._2 * b._0; r._1 -= a._2 * b._3; return r; }
_e0_e1_e2_e1e2 operator * (_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._2 += a._2 * b._0; r._3 -= a._2 * b._1; r._0 += a._2 * b._2; r._1 -= a._2 * b._3; return r; }

_e0_e1e2 operator * (_e1_e2 a, _e1 b) { _e0_e1e2 r = {}; r._0 += a._1 * b._1; r._3 -= a._2 * b._1; return r; }
_e0_e1e2 operator * (_e1_e2 a, _e2 b) { _e0_e1e2 r = {}; r._3 += a._1 * b._2; r._0 += a._2 * b._2; return r; }
_e0_e1e2 operator * (_e1_e2 a, _e1_e2 b) { _e0_e1e2 r = {}; r._0 += a._1 * b._1; r._3 += a._1 * b._2; r._3 -= a._2 * b._1; r._0 += a._2 * b._2; return r; }
_e1_e2 operator * (_e1_e2 a, _e1e2 b) { _e1_e2 r = {}; r._2 += a._1 * b._3; r._1 -= a._2 * b._3; return r; }
_e1_e2 operator * (_e1_e2 a, _e0_e1e2 b) { _e1_e2 r = {}; r._1 += a._1 * b._0; r._2 += a._1 * b._3; r._2 += a._2 * b._0; r._1 -= a._2 * b._3; return r; }
_e0_e1_e2_e1e2 operator * (_e1_e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._1 += a._1 * b._0; r._0 += a._1 * b._1; r._3 += a._1 * b._2; r._2 += a._1 * b._3; r._2 += a._2 * b._0; r._3 -= a._2 * b._1; r._0 += a._2 * b._2; r._1 -= a._2 * b._3; return r; }

_e2 operator * (_e1e2 a, _e1 b) { _e2 r = {}; r._2 -= a._3 * b._1; return r; }
_e1 operator * (_e1e2 a, _e2 b) { _e1 r = {}; r._1 += a._3 * b._2; return r; }
_e1_e2 operator * (_e1e2 a, _e1_e2 b) { _e1_e2 r = {}; r._2 -= a._3 * b._1; r._1 += a._3 * b._2; return r; }
float32 operator * (_e1e2 a, _e1e2 b) { return -(a._3 * b._3); }
_e0_e1e2 operator * (_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r = {}; r._3 += a._3 * b._0; r._0 -= a._3 * b._3; return r; }
_e0_e1_e2_e1e2 operator * (_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._3 += a._3 * b._0; r._2 -= a._3 * b._1; r._1 += a._3 * b._2; r._0 -= a._3 * b._3; return r; }

_e1_e2 operator * (_e0_e1e2 a, _e1 b) { _e1_e2 r = {}; r._1 += a._0 * b._1; r._2 -= a._3 * b._1; return r; }
_e1_e2 operator * (_e0_e1e2 a, _e2 b) { _e1_e2 r = {}; r._2 += a._0 * b._2; r._1 += a._3 * b._2; return r; }
_e1_e2 operator * (_e0_e1e2 a, _e1_e2 b) { _e1_e2 r = {}; r._1 += a._0 * b._1; r._2 += a._0 * b._2; r._2 -= a._3 * b._1; r._1 += a._3 * b._2; return r; }
_e0_e1e2 operator * (_e0_e1e2 a, _e1e2 b) { _e0_e1e2 r = {}; r._3 += a._0 * b._3; r._0 -= a._3 * b._3; return r; }
_e0_e1e2 operator * (_e0_e1e2 a, _e0_e1e2 b) { _e0_e1e2 r; r._0 = (a._0 * b._0) - (a._3 * b._3); r._3 = (a._0 * b._3) + (a._3 * b._0); return r; }
_e0_e1_e2_e1e2 operator * (_e0_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._0 += a._0 * b._0; r._1 += a._0 * b._1; r._2 += a._0 * b._2; r._3 += a._0 * b._3; r._3 += a._3 * b._0; r._2 -= a._3 * b._1; r._1 += a._3 * b._2; r._0 -= a._3 * b._3; return r; }

_e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, _e1 b) { _e0_e1_e2_e1e2 r = {}; r._1 += a._0 * b._1; r._0 += a._1 * b._1; r._3 -= a._2 * b._1; r._2 -= a._3 * b._1; return r; }
_e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, _e2 b) { _e0_e1_e2_e1e2 r = {}; r._2 += a._0 * b._2; r._3 += a._1 * b._2; r._0 += a._2 * b._2; r._1 += a._3 * b._2; return r; }
_e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, _e1_e2 b) { _e0_e1_e2_e1e2 r = {}; r._1 += a._0 * b._1; r._2 += a._0 * b._2; r._0 += a._1 * b._1; r._3 += a._1 * b._2; r._3 -= a._2 * b._1; r._0 += a._2 * b._2; r._2 -= a._3 * b._1; r._1 += a._3 * b._2; return r; }
_e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, _e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._3 += a._0 * b._3; r._2 += a._1 * b._3; r._1 -= a._2 * b._3; r._0 -= a._3 * b._3; return r; }
_e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, _e0_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._0 += a._0 * b._0; r._3 += a._0 * b._3; r._1 += a._1 * b._0; r._2 += a._1 * b._3; r._2 += a._2 * b._0; r._1 -= a._2 * b._3; r._3 += a._3 * b._0; r._0 -= a._3 * b._3; return r; }
_e0_e1_e2_e1e2 operator * (_e0_e1_e2_e1e2 a, _e0_e1_e2_e1e2 b) { _e0_e1_e2_e1e2 r = {}; r._0 += a._0 * b._0; r._1 += a._0 * b._1; r._2 += a._0 * b._2; r._3 += a._0 * b._3; r._1 += a._1 * b._0; r._0 += a._1 * b._1; r._3 += a._1 * b._2; r._2 += a._1 * b._3; r._2 += a._2 * b._0; r._3 -= a._2 * b._1; r._0 += a._2 * b._2; r._1 -= a._2 * b._3; r._3 += a._3 * b._0; r._2 -= a._3 * b._1; r._1 += a._3 * b._2; r._0 -= a._3 * b._3; return r; }

float32 conjugate(float32 a) { return a; }
_e1 conjugate(_e1 a) { return a; }
_e2 conjugate(_e2 a) { return a; }
_e1_e2 conjugate(_e1_e2 a) { return a; }
_e1e2 conjugate(_e1e2 a) { a._3 = -a._3; return a; }
_e0_e1e2 conjugate(_e0_e1e2 a) { a._3 = -a._3; return a; }
_e0_e1_e2_e1e2 conjugate(_e0_e1_e2_e1e2 a) { a._3 = -a._3; return a; }

template <typename T>
float32 length_squared(T t) { return inner(t, conjugate(t)); }

float32 length(float32 a) { return a; }
float32 length(_e1 a) { return a._1; }
float32 length(_e2 a) { return a._2; }
float32 length(_e1_e2 a) { return math::square_root(length_squared(a)); }
float32 length(_e1e2 a) { return a._3; }
float32 length(_e0_e1e2 a) { return math::square_root(length_squared(a)); }
float32 length(_e0_e1_e2_e1e2 a) { return math::square_root(length_squared(a)); }

void normalize(float32& a) { a = 1.f; }
void normalize(_e1& a) { a._1 = 1.f; }
void normalize(_e2& a) { a._2 = 1.f; }
void normalize(_e1_e2& a) { float32 norm = length(a); if (not math::near_zero(norm)) { a._1 /= norm; a._2 /= norm; } }
void normalize(_e1e2& a) {a._3 = 1.f; }
void normalize(_e0_e1e2& a) { float32 norm = length(a); if (not math::near_zero(norm)) { a._0 /= norm; a._3 /= norm; } }
void normalize(_e0_e1_e2_e1e2& a) { float32 norm = length(a); if (not math::near_zero(norm)) { a._0 /= norm; a._1 /= norm; a._2 /= norm; a._3 /= norm; } }

float32 normalized(float32 a) { normalize(a); return a; }
_e1 normalized(_e1 a) { normalize(a); return a; }
_e2 normalized(_e2 a) { normalize(a); return a; }
_e1_e2 normalized(_e1_e2 a) { normalize(a); return a; }
_e1e2 normalized(_e1e2 a) { normalize(a); return a; }
_e0_e1e2 normalized(_e0_e1e2 a) { normalize(a); return a; }
_e0_e1_e2_e1e2 normalized(_e0_e1_e2_e1e2 a) { normalize(a); return a; }


