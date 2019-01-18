#include <cstdlib>

#if !defined( _MSC_VER)
#define EXPORTED_FN __attribute__((visibility("default")))
#else
#define EXPORTED_FN __declspec(dllexport)
#define snprintf sprintf_s
#define strdup _strdup
#endif

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

    white()
    {
        r = g = b = a = 255;
    }
};

int writeByte(char* vbuf, int cursor, unsigned char value) {
    vbuf[cursor] = value; cursor ++;
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

int vertex_color(char* vbuf, int cursor, color color) {
    cursor = writeByte(vbuf, cursor, color.r);
    cursor = writeByte(vbuf, cursor, color.g);
    cursor = writeByte(vbuf, cursor, color.b);
    cursor = writeByte(vbuf, cursor, color.a);
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

int vertex_texcoord(char* vbuf, int cursor, float u, float v) {
    cursor = writeFloat(vbuf, cursor, u);
    cursor = writeFloat(vbuf, cursor, v);
    return cursor;
}

extern "C" {

    double makebuffer(char* buffer) {
        int cursor = 0;
        color _red(1, 0, 0, .5);

        cursor = vertex_color(buffer, cursor, _red);
        cursor = vertex_position_3d(buffer, cursor, 10, 20, 333333.35);
        cursor = vertex_normal(buffer, cursor, 4, 3, 2);
        cursor = vertex_texcoord(buffer, cursor, 0.0115, 0.0115);

        cursor = vertex_color(buffer, cursor, _red);
        cursor = vertex_position_3d(buffer, cursor, 50, 60, 9999934234.35);
        cursor = vertex_normal(buffer, cursor, 6, 5, 4);
        cursor = vertex_texcoord(buffer, cursor, 0.000115, 0.000115);

        return 1;
    }
}
