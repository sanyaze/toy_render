#include "math_tools.h"
#include "log_debug.h"

int main()
{
    Matrix3f m1, m2;
    for (unsigned int i = 0; i < m1.row; i++) {
        for (unsigned int j = 0; j < m1.col; j++) {
            m1.data[i][j] = i + j;
            m2.data[i][j] = i * j;
        }
    }
    
    Matrix3f m3 = Eye3();
    show(m3);

    Vector3f v1(1.0f, 2.0f, 3.0f);
    Vector3f v2(1.0f, 1.0f, 1.0f);

    show(m1);
    show(m2);
    show(v1);
    show(v2);
    
    LOG_DEBUG("vector trans multiply:\n");
    show(TransMultiply(v1, v2));
    LOG_DEBUG("vector dot multiply:\n");
    show(v1 * v2);
    LOG_DEBUG("Mod: %f\n", v1.Mod());
    
    LOG_DEBUG("matrix multiply: \n");
    show(m1 * m3);
    LOG_DEBUG("mat * vec: \n");
    show(m1 * v1);
    LOG_DEBUG("matrix's det: %f\n", m1.Det());
    return 0;
}