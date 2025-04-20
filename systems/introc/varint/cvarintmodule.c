#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *cvarint_encode(PyObject *self, PyObject *args) {
    unsigned long long n;
    if (!PyArg_ParseTuple(args, "K", &n)) {
        return NULL;
    }

    char bytes[11] =  {};
    int lower_7, msb;

    for (int i = 0; n > 0; i++) {
        lower_7 = n & 0x7f; // same as % 128
        n >>= 7;
        msb = (n > 0) ? 0x80 : 0;
        bytes[i] = msb + lower_7;
    }

    return PyBytes_FromStringAndSize(bytes, strlen(bytes));
}

static PyObject *cvarint_decode(PyObject *self, PyObject *args) {
    const unsigned char *bytes_ptr;
    Py_ssize_t bytes_len;
    if (!PyArg_ParseTuple(args, "y#", &bytes_ptr, &bytes_len)) {
        return NULL;
    }

    unsigned long long n = 0;

    for (int i = bytes_len - 1; i >= 0; i--) {
        n <<= 7;
        n += bytes_ptr[i] & 0x7f; // same as % 128
    }

    return PyLong_FromUnsignedLongLong(n);
}

static PyMethodDef CVarintMethods[] = {
    {"encode", cvarint_encode, METH_VARARGS, "Encode an integer as varint."},
    {"decode", cvarint_decode, METH_VARARGS,
     "Decode varint bytes to an integer."},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef cvarintmodule = {
    PyModuleDef_HEAD_INIT, "cvarint",
    "A C implementation of protobuf varint encoding", -1, CVarintMethods};

PyMODINIT_FUNC PyInit_cvarint(void) { return PyModule_Create(&cvarintmodule); }
