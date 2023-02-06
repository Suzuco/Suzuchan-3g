//
// Created by suzuco on 1/27/23.
//

#include <new>
#include "shimpy.h"

shimpy::shimpy(const std::string & path, const std::string & method_name)
{
    this->module_string = PyUnicode_FromString(path.c_str());
    this->module_obj = PyImport_Import(module_string);
    if (!module_obj) goto err;
    this->module_dict = PyModule_GetDict(module_obj);
    if (!module_dict) goto err;
    this->method_obj = PyDict_GetItemString(module_dict, method_name.c_str());
    if (! method_obj) goto err;
    return;
err:
    PyErr_Print();
    throw std::bad_alloc();
}

std::string shimpy::call(const std::string & args)
{
    PyObject * argstr_obj = PyUnicode_FromString(args.c_str());
    PyObject * args_obj = PyTuple_New(1);
    PyTuple_SetItem(args_obj, 0, argstr_obj);
    PyObject * py_result = PyObject_CallObject(method_obj, args_obj);
    Py_ssize_t sz_result;
    const char * data = PyUnicode_AsUTF8AndSize(py_result, & sz_result);
    return {data, static_cast<size_t>(sz_result)};
}
