//
// Created by suzuco on 1/27/23.
// Shim for Python method calls
// しんぱいしています。
//

#ifndef SUZUCHAN_3G_SHIMPY_H
#define SUZUCHAN_3G_SHIMPY_H

#include <string>
#include <Python.h>

class shimpy {
private:
    PyObject * module_string, * module_obj, * module_dict, * method_obj;

public:
    shimpy(const std::string & path, const std::string & method_name);
    std::string call(const std::string & args);
};


#endif //SUZUCHAN_3G_SHIMPY_H
