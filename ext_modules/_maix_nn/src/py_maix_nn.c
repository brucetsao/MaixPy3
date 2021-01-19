
#include "py_maix_nn.h"

#include "string.h"
#include "libmaix_nn.h"


static PyObject* _maix_nn_help() {
    return PyUnicode_FromString(_maix_nn_doc);
}

PyDoc_STRVAR(_maix_nn_load_doc, "load model, returen a nn.Model object\n");
static PyObject *_maix_nn_load(PyObject *self, PyObject *args, PyObject *kw_args)
{
    /* parse args */
    static char *kwlist[] = {"model_path", "opt", NULL};

    int flags = 0;
    PyObject *model_path = NULL;
    PyObject *opt        = NULL;
    PyObject *key        = NULL;
    char *buf;
    Py_ssize_t buflen, readlen, recvlen = 0;

    /* Get the buffer's memory */
    if (!PyArg_ParseTupleAndKeywords(args, kw_args, "|OO:load", kwlist,
                                     &model_path, &opt))
    {
        return NULL;
    }
    if(!model_path || !opt)
    {
        PyErr_SetString(PyExc_ValueError, "nee mode_path and opt args");
        return NULL;
    }
    /* new object */
    PyObject* model_obj = PyMaix_NN_Model_Type.tp_new(&PyMaix_NN_Model_Type, PyList_New(0), PyDict_New());
    if(!model_obj)
    {
        PyErr_NoMemory();
        return NULL;
    }
    /* init model object */

    PyObject *call_args = Py_BuildValue("(O)", model_path);
    PyObject *call_keywords = PyDict_New();
    PyDict_SetItemString(call_keywords, "opt", opt);

    PyObject* ret = PyObject_Call(PyObject_GetAttrString(model_obj, "__init__"), call_args, call_keywords);
    Py_DECREF(call_args);
    Py_DECREF(call_keywords);
    if( ret == NULL)
    {
        return NULL;
    }

    return model_obj;
}

static PyMethodDef _maix_nn_methods[] = {
    {"help", (PyCFunction)_maix_nn_help, METH_NOARGS, _maix_nn_doc},
    {"load", (PyCFunction)_maix_nn_load, METH_VARARGS | METH_KEYWORDS, _maix_nn_load_doc},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef _maix_nn_module = {
    PyModuleDef_HEAD_INIT,
    _MAIX_NN_NAME_,         /* Module name */
    _maix_nn_doc,	/* Module _maixMethods */
    -1,			    /* size of per-interpreter state of the module, size of per-interpreter state of the module,*/
    _maix_nn_methods,
};

PyMODINIT_FUNC PyInit__maix_nn(void)
{

    PyObject *module;

    module = PyModule_Create(&_maix_nn_module);
    PyObject *version = PyUnicode_FromString(_MAIX_NN_VERSION_);


    /* Set module version */
    PyObject *dict = PyModule_GetDict(module);
    PyDict_SetItemString(dict, "__version__", version);
    Py_DECREF(version);

    /* Add class*/
    // PyModule_AddObject(module, "Model", SpamError);

    /* init object(default value) */
    if (PyType_Ready(&PyMaix_NN_Model_Type) < 0) {
        return NULL;
    }

    return module;
}
