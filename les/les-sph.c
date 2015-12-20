/*
 * Copyright (C) 2015 Wiky L
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */

#include "les-sph.h"


static void Socket_dealloc(SocketObject* self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *Socket_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    SocketObject *self;

    self = (SocketObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->socket=NULL;
    }

    return (PyObject *)self;
}

static int Socket_init(SocketObject *self, PyObject *args, PyObject *kwds) {
    PyErr_SetString(PyExc_RuntimeError, "You cannot create a SphSocket manually");
    return -1;
}

static PyObject *socket_prepare_data(SocketObject* self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"data", NULL};
    char *data=NULL;
    int len;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s#", kwlist, &data, &len)) {
        return NULL;
    }
    sph_socket_prepare_data(self->socket, data, len);
    return Py_BuildValue("");
}

static PyMethodDef Socket_methods[] = {
    {
        "prepare_data", (PyCFunction)socket_prepare_data, METH_VARARGS|METH_KEYWORDS,
        "prepare data for sending"
    },
    {NULL}  /* Sentinel */
};


static PyTypeObject SocketType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "lessph.Socket",             /* tp_name */
    sizeof(SocketObject),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Socket_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
    Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "Socket objects",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Socket_methods,             /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Socket_init,      /* tp_init */
    0,                         /* tp_alloc */
    Socket_new,                /* tp_new */
};

static PyMethodDef SphMethods[] = {
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef SphModule = {
    PyModuleDef_HEAD_INIT,
    "sph",
    NULL,
    -1,
    SphMethods
};

PyMODINIT_FUNC PyInit_lessph(void) {
    PyObject *m;

    SocketType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&SocketType) < 0)
        return NULL;

    if((m = PyModule_Create(&SphModule))==NULL) {
        return NULL;
    }

    Py_INCREF(&SocketType);
    PyModule_AddObject(m, "Socket", (PyObject *)&SocketType);
    return m;
}

PyObject *py_sph_socket_new(SphSocket *socket) {
    PyObject *obj = Socket_new(&SocketType, NULL, NULL);

    ((SocketObject*)obj)->socket=socket;
    return obj;
}

