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

#include <Python.h>
#include <sph.h>

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

PyMODINIT_FUNC PyInit_sph(void) {
    PyObject *m = PyModule_Create(&SphModule);
    return m;
}

