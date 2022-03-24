#include "DirectTX.cpp"

const int LENGTH = 700, WIDTH  = 600;

struct Vector
{
    double lx_, ly_, lz_;

    Vector (double x, double y, double z);

    Vector operator ! ();
    Vector operator * (double k);
    Vector operator + (Vector N);
    Vector operator ^ (Vector N);
    Vector operator - (Vector N);
};

void   FonDraw            ();
void   Sphere             (int r, Vector Light, Vector L_Color);
double Clamp              (double k, int BorderUp, int BorderDown);
double DistanceFromCentre (double x, double y);
double kDraw              (Vector L, Vector N, double k);
void   ControlofSource    (Vector* Light, Vector* L_Color);
double StepenN            (double a, int N);

void RotateZ (double alfa, Vector* Light);
void RotateY (double alfa, Vector* Light);
void RotateX (double alfa, Vector* Light);

int main ()
{
    txTextCursor (false);
    DirectTXCreateWindow (LENGTH, WIDTH, "Освещение сферы");

    int r = 100;

    double L_X = 100, L_Y = 400, L_Z = 10;
    double L_R = 1.0, L_G = 0.5, L_B = 0;

    Vector Light   (L_X, L_Y, L_Z);
    Vector L_Color (L_R, L_G, L_B);

    //txMessageBox ("Выполненно Котиками, совместно с ТыЖ Программистом.\nПод песни непревзойденного Кристофера Ли!");

    while (!GetAsyncKeyState (VK_ESCAPE))
    {
        DirectTXBegin();

        FonDraw ();
        Sphere (r, Light, L_Color);

        DirectTXFlush();

        DirectTXEnd ();

        txSleep (0);

        ControlofSource (&Light, &L_Color);
    }

    return 0;
}

void Sphere(int r, Vector Light, Vector L_Color)
{
    Vector ViewPosition (0, 0, 2*r);
    Vector K_Color = L_Color;

    for (double x = -r; x < r; x += 1)
    for (double y = -r; y < r; y += 1)
    {
        if ((r * r) >  DistanceFromCentre (x, y))
        {
            Vector Point (x, y, sqrt (r*r - x*x - y*y));
            Vector V = ! (ViewPosition - Point);

            double cosk = kDraw (!(Point - Light), !Point, 0);

            Vector Lr = (!Point*2 - !Light) * cosk;

            cosk *= 255;
            cosk = Clamp (cosk, 255, 0);

            //double cosb = kDraw (!Lr, !V, 0);

            //cosb *= 255;
            //cosb = Clamp (cosb, 255, 0);

            //cosb = StepenN (cosb, 17);

            K_Color = L_Color * (cosk);

            //printf ("%lg ", cosb);

            COLORREF color = (RGB (K_Color.lx_, K_Color.ly_, K_Color.lz_));

            DirectTXPutPixel (LENGTH/2 + x, WIDTH/2 + y, color);
        }
    }
}

Vector::Vector (double x, double y, double z):
    lx_     (x),
    ly_     (y),
    lz_     (z)
    {}

Vector Vector::operator ! ()
{
    double length_ = sqrt (lx_*lx_ + ly_*ly_ + lz_*lz_);

    Vector Ret (lx_ / length_, ly_ / length_, lz_ / length_);

    return Ret;
}

Vector Vector::operator + (Vector N)
{
    Vector Ret (N.lx_ + lx_, N.ly_ + ly_, N.lz_ + lz_);

    return Ret;
}

Vector Vector::operator * (double k)
{
    Vector Ret (lx_*k, ly_*k, lz_*k);

    return Ret;
}

Vector Vector::operator - (Vector N)
{
    Vector Ret (lx_ - N.lx_, ly_ - N.ly_, lz_ - N.lz_);

    return Ret;
}

Vector Vector::operator ^ (Vector N)
{
    Vector Ret (N.lx_ * lx_, N.ly_ * ly_, N.lz_ * lz_);

    return Ret;
}

double kDraw (Vector L, Vector N, double k)
{
    k = L.lx_*N.lx_ + L.ly_*N.ly_ + L.lz_*N.lz_;

    return k;
}

double DistanceFromCentre (double x, double y)
{
    double ret = (x * x) + (y * y);
    return ret;
}

double StepenN (double a, int N)
{
    double b = a;
    for (int i = 1; i < N; i ++) a *= b;

    return a;
}

void ControlofSource (Vector* Light, Vector* L_Color)
{
    if (GetAsyncKeyState ('Q')) RotateX ( 0.1, Light);
    if (GetAsyncKeyState ('A')) RotateX (-0.1, Light);
    if (GetAsyncKeyState ('W')) RotateY ( 0.1, Light);
    if (GetAsyncKeyState ('S')) RotateY (-0.1, Light);
    if (GetAsyncKeyState ('E')) RotateZ ( 0.1, Light);
    if (GetAsyncKeyState ('D')) RotateZ (-0.1, Light);

    RotateX (0.05, Light);
    RotateY (0.05, Light);
    RotateZ (0.05, Light);

    if (GetAsyncKeyState ('R') && L_Color -> lx_ < 1) L_Color -> lx_ += 0.02;
    if (GetAsyncKeyState ('T') && L_Color -> lx_ > 0) L_Color -> lx_ -= 0.02;
    if (GetAsyncKeyState ('G') && L_Color -> ly_ < 1) L_Color -> ly_ += 0.02;
    if (GetAsyncKeyState ('H') && L_Color -> ly_ > 0) L_Color -> ly_ -= 0.02;
    if (GetAsyncKeyState ('B') && L_Color -> lz_ < 1) L_Color -> lz_ += 0.02;
    if (GetAsyncKeyState ('N') && L_Color -> lz_ > 0) L_Color -> lz_ -= 0.02;
}

void FonDraw ()
{
    DirectTXSetColor      (RGB (120, 120, 120));
    DirectTXSetFillColor  (RGB (120, 120, 120));
    DirectTXDrawRectangle (0, 0, LENGTH, WIDTH);
}

double Clamp (double k, int BorderUp, int BorderDown)
{
    if (k > BorderUp  ) k = BorderUp;
    if (k < BorderDown) k = BorderDown;

    return k;
}

void RotateZ (double alfa, Vector* Light)
{
    double x = Light -> lx_;
    double y = Light -> ly_;
    Light -> lx_ = x * cos (alfa) - y * sin (alfa);
    Light -> ly_ = x * sin (alfa) + y * cos (alfa);
    Light -> lz_ = Light -> lz_;
}

void RotateY (double alfa, Vector* Light)
{
    double x = Light -> lx_;
    double z = Light -> lz_;
    Light -> lx_ =   x * cos (alfa) + z * sin (alfa);
    Light -> ly_ = Light -> ly_;
    Light -> lz_ = - x * sin (alfa) + z * cos (alfa);
}

void RotateX (double alfa, Vector* Light)
{
    double y = Light -> ly_;
    double z = Light -> lz_;
    Light -> lx_ = Light -> lx_;
    Light -> ly_ = y * cos (alfa) - z * sin (alfa);
    Light -> lz_ = y * sin (alfa) + z * cos (alfa);
}
