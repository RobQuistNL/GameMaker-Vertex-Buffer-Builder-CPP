#include <cstdlib>

#if !defined( _MSC_VER)
#define EXPORTED_FN __attribute__((visibility("default")))
#else
#define EXPORTED_FN __declspec(dllexport)
#define snprintf sprintf_s
#define strdup _strdup
#endif

double clamp(float in, double min, double max)
{
    if (in > max) { return max; }
    if (in < min) { return min; }
    return in;
}
/*
class tangent {
    float e[6];
public:
    tangent(float a = 0, float b = 0, float c = 0, float d = 0, float e = 0, float f = 0)  : e[0](a), e[1](b), e[2](c), e[3](d), e[4](e), e[5](f) { }
    float& operator(int i) { return e[i]; }
    float  operator(int i) const { return e[i]; }
    static int const size;

    tangent& operator += (tangent const& b) {
       e[0] += b[0]; e[1] += b[1]; e[2] += b[2];
       return *this;
    }
};*/
/*
vertex_triangle(
				terrainmesh,
				(xx  )*s, (yy  )*s, tl,
				(xx+1)*s, (yy  )*s, tr,
				(xx  )*s, (yy+1)*s, bl,
				col,
				[tx1, ty1, tx2, ty1, tx1, ty2]
			);
			vertex_triangle(
				terrainmesh,
				(xx  )*s, (yy+1)*s, bl,
				(xx+1)*s, (yy  )*s, tr,
				(xx+1)*s, (yy+1)*s, br,
				col,
				[tx1, ty2, tx2, ty1, tx2, ty2]
			);
*/

class color {
    public:
    unsigned char r, g, b, a;

    color()
    {
        r = g = b = a =0;
    }

    color(float f_r, float f_g, float f_b, float f_a)
    {
        r = f_r * 255;
        g = f_g * 255;
        b = f_b * 255;
        a = f_a * 255;
    }
/*
    color(unsigned char f_r, unsigned char f_g, unsigned char f_b, unsigned char f_a)
    {
        r = f_r;
        g = f_g;
        b = f_b;
        a = f_a;
    }
*/
    white()
    {
        r = g = b = a = 255;
    }
};

int packFloat(void *buf, float x) {
    unsigned char *b = (unsigned char *)buf;
    unsigned char *p = (unsigned char *) &x;
#if defined (_M_IX86) || (defined (CPU_FAMILY) && (CPU_FAMILY == I80X86))
    b[0] = p[3];
    b[1] = p[2];
    b[2] = p[1];
    b[3] = p[0];
#else
    b[0] = p[0];
    b[1] = p[1];
    b[2] = p[2];
    b[3] = p[3];
#endif
    return 4;
}

int writeByte(char* vbuf, int cursor, unsigned char value) {
    vbuf[cursor] = value; cursor ++;
    return cursor;
}

int writeLong(char* vbuf, int cursor, long value) {
    vbuf[cursor] = (value >> 24) & 0xFF; cursor ++;
    vbuf[cursor] = (value >> 16) & 0xFF; cursor ++;
    vbuf[cursor] = (value >> 8) & 0xFF; cursor ++;
    vbuf[cursor] = value & 0xFF; cursor ++;

    return cursor;
}

int writeFloat(char* vbuf, int cursor, float fvalue) {

    unsigned char const * p = reinterpret_cast<unsigned char const *>(&fvalue);

    for (std::size_t i = 0; i != sizeof(float); ++i)
    {
        vbuf[cursor] = p[i]; cursor ++;
    }

    return cursor;
}

int vertex_position_3d(char* vbuf, int cursor, float x, float y, float z) {
    cursor = writeFloat(vbuf, cursor, x);
    cursor = writeFloat(vbuf, cursor, y);
    cursor = writeFloat(vbuf, cursor, z);
    return cursor;
}

int vertex_normal(char* vbuf, int cursor, float x, float y, float z) {
    cursor = writeFloat(vbuf, cursor, x);
    cursor = writeFloat(vbuf, cursor, y);
    cursor = writeFloat(vbuf, cursor, z);
    return cursor;
}

int vertex_color(char* vbuf, int cursor, color color) {
    cursor = writeByte(vbuf, cursor, color.r);
    cursor = writeByte(vbuf, cursor, color.g);
    cursor = writeByte(vbuf, cursor, color.b);
    cursor = writeByte(vbuf, cursor, color.a);
    return cursor;
}

int vertex_texcoord(char* vbuf, int cursor, float u, float v) {
    cursor = writeFloat(vbuf, cursor, u);
    cursor = writeFloat(vbuf, cursor, v);
    return cursor;
}
/*
int vertex_triangle(char* vbuf, int cursor, float x1, float y1, float z1, float x2, float y2, float z2,  float x3, float y3, float z3, float u0, float v0, float u1, float v1, float u2, float v2, int color) {
    unsigned char found[4];

    cursor = vertex_position_3d(vbuf, cursor, x1, y1, z1);
    cursor = vertex_normal(vbuf, cursor, found[2], found[3], found[4]);
    cursor = vertex_color(vbuf, cursor, color::color());
    cursor = vertex_texcoord(vbuf, cursor, u0, v0);
    cursor = vertex_color(vbuf, cursor, (new color).white());

    cursor = vertex_position_3d(vbuf, cursor, x2, y2, z2);
    cursor = vertex_normal(vbuf, cursor, found[2], found[3], found[4]);
    cursor = vertex_color(vbuf, cursor, color::white());
    cursor = vertex_texcoord(vbuf, cursor, u1, v1);
    cursor = vertex_color(vbuf, cursor, color::white());

    cursor = vertex_position_3d(vbuf, cursor, x3, y3, z3);
    cursor = vertex_normal(vbuf, cursor, found[2], found[3], found[4]);
    cursor = vertex_color(vbuf, cursor, color::white());
    cursor = vertex_texcoord(vbuf, cursor, u2, v2);
    cursor = vertex_color(vbuf, cursor, color::white());

    return cursor;
}
*/

extern "C" {

    double chunkSize, ter_octaves, ter_persistance, ter_scale, temp_octaves, temp_persistance, temp_scale, moist_octaves, moist_persistance, moist_scale;

    double makebuffer(char* buffer) {
        int cursor = 0;
        color _red(1, 0, 0, .5);
        color _green(0, 1, 0, .5);
        color _blue(0, 0, 1, .5);
        color _white(1, 1, 1, 1);
        color _black(0, 0, 0, 0);

        cursor = vertex_color(buffer, cursor, _red);
        cursor = vertex_color(buffer, cursor, _green);
        cursor = vertex_color(buffer, cursor, _blue);
        cursor = vertex_color(buffer, cursor, _black);
        cursor = vertex_color(buffer, cursor, _white);
        return 1;
    }

    double vertex_buffer(char* vbuf, double lod) {
        int c = 0;

        for (int yy = 0; yy <= chunkSize/lod; yy++) {
            for (int xx = 0; xx <= chunkSize/lod; xx++) {
                //int noiseX = xx*lod + (chunkX*chunkSize) + 1000;
                //int noiseY = yy*lod + (chunkY*chunkSize) + 4000;
                float s = 32;
                //c = vertex_triangle(vbuf, c, xx*s, yy*s, 10, (xx+1)*s, yy*s, 10, xx*s, (yy+1)*s, 10, 0, 0, 1, 1, 1, 1, 255);
                //c = vertex_triangle(vbuf, c, xx*s, (yy+1)*s, 10, (xx+1)*s, yy*s, 10, (xx+1)*s, (yy+1)*s, 10, 0, 0, 1, 1, 1, 1, 255);

                /*
                vertex_triangle(
				terrainmesh,
				(xx  )*s, (yy  )*s, tl,
				(xx+1)*s, (yy  )*s, tr,
				(xx  )*s, (yy+1)*s, bl,
				col,
				[tx1, ty1, tx2, ty1, tx1, ty2]
			);
			vertex_triangle(
				terrainmesh,
				(xx  )*s, (yy+1)*s, bl,
				(xx+1)*s, (yy  )*s, tr,
				(xx+1)*s, (yy+1)*s, br,
				col,
				[tx1, ty2, tx2, ty1, tx2, ty2]
			);
                */

                //buffer[c] = 255 * (1 + noise.noise(noiseX*moist_scale, noiseY*moist_scale))/2;
                //c++;
            }
        }

        return true;
    }


}
