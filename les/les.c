/*
 * Copyright (C) 2015 Wiky L
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */
#include <stdio.h>
#include <stdlib.h>
#include "les-sph.h"

typedef struct {
    PyObject *lessph;
    PyObject *les;
} ModuleData;

static int m_init(void);
static void m_finalize(void);
static int m_accept(SphSocket *socket);
static int m_recv(SphSocket *socket,const uint8_t *data, unsigned int len);

static JacModule mod = {
    m_init,
    m_finalize,
    m_accept,
    m_recv
};

JACQUES_MODULE(mod);

static int m_init(void) {
    printf("m_init\n");
    Py_Initialize();

    PyObject *lessph = PyInit_lessph();
    PyObject *name = PyUnicode_FromString("mod.app");
    PyObject *les = PyImport_Import(name);
    Py_DECREF(name);

    if(!les) {
        PyErr_Print();
        Py_Finalize();
        return -1;
    }
    ModuleData *data=(ModuleData*)malloc(sizeof(ModuleData));
    data->les = les;
    data->lessph = lessph;

    mod.user_data = data;
    return 0;
}

static void m_finalize(void) {
    printf("m_finalize!\n");
    ModuleData *data=(ModuleData*)mod.user_data;
    Py_DECREF(data->les);
    Py_DECREF(data->lessph);
    Py_Finalize();
}

static int m_accept(SphSocket *socket) {
    ModuleData *data=(ModuleData*)mod.user_data;
    PyObject* func = PyObject_GetAttrString(data->les, (char*)"accept");
    if(func && PyCallable_Check(func)) {
        PyObject *args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, py_sph_socket_new(socket));
        PyObject *ret = PyObject_CallObject(func, args);
        if (ret != NULL) {
            Py_DECREF(ret);
        } else {
            PyErr_Print();
        }
        Py_DECREF(args);
    } else {
        PyErr_Print();
    }
    Py_DECREF(func);
    return 0;
}

static int m_recv(SphSocket *socket,const uint8_t *data, unsigned int len) {
    PyObject* func = PyObject_GetAttrString(((ModuleData*)mod.user_data)->les, (char*)"recv");
    if(func && PyCallable_Check(func)) {
        PyObject *args = PyTuple_New(2);
        PyTuple_SetItem(args, 0, py_sph_socket_new(socket));
        PyTuple_SetItem(args, 1, Py_BuildValue("s#", (char*)data, len));
        PyObject *ret = PyObject_CallObject(func, args);
        if (ret != NULL) {
            Py_DECREF(ret);
        } else {
            PyErr_Print();
        }
        Py_DECREF(args);
    } else {
        PyErr_Print();
    }
    Py_DECREF(func);
    return 0;
}
