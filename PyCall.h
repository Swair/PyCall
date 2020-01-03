#ifndef __PYCALL_H
#define __PYCALL_H

#include <stdio.h>
#include <complex>
#include <string>
#include <vector>
#include <stdlib.h>
#include <Python.h>


class PyCall
{
    public:
        PyCall() 
            : pModule_(nullptr)
        {
            Py_Initialize();

            if(!Py_IsInitialized())
            {
                printf("Python init failed!\n");
                return;
            }

            PyRun_SimpleString("import sys");
            PyRun_SimpleString("sys.path.append('./')");

            pModule_ = PyImport_ImportModule("cpy");
            if(!pModule_)
            {
                printf("Load py_add.py failed!\n");
                return;
            }
            pDict_ = PyModule_GetDict(pModule_);
            if(!pDict_)
            {
                printf("Can't find dict in py_add!\n");
                return;
            }

            getFunc(&funcPlot_, "plot");
            getFunc(&functPlot_, "tplot");
            getFunc(&funcShow_, "show");
            getFunc(&funcSubplot_, "subplot");

        }

        ~PyCall()
        {
            if(pModule_)
                Py_DECREF(pModule_);

            Py_Finalize();
        }

        int getFunc(PyObject** pyObject, const std::string& funcName)
        {
            *pyObject = PyDict_GetItemString(pDict_, funcName.c_str());
            if(!*pyObject || !PyCallable_Check(*pyObject))
            {
                printf("Can't find function %s!\n", funcName.c_str());
                return -1;
            }
            return 0;
        }

        void show()
        {
            PyObject* args = nullptr;
            PyObject_CallObject(funcShow_ ,args);
        }

        int plot(const std::vector<double>& x, const std::vector<double>& y, const std::string& op)
        {
            PyObject* pList1 = PyList_New(x.size());
            PyObject* pList2 = PyList_New(y.size());

            for(int i = 0; i < x.size(); i++)
                PyList_SetItem(pList1, i, PyFloat_FromDouble(x[i]));

            for(int i = 0; i < y.size(); i++)
                PyList_SetItem(pList2, i, PyFloat_FromDouble(y[i]));

            PyObject* args = PyTuple_New(3);
            PyTuple_SetItem(args,0,pList1);
            PyTuple_SetItem(args,1,pList2);
            PyTuple_SetItem(args,2,Py_BuildValue("s",op.c_str()));

            if(!funcPlot_)
            {
                printf("plot null ptr\n");
                return -1;
            }

            PyObject* pReturn = PyObject_CallObject(funcPlot_,args);

            if(args)
                Py_DECREF(args);

            return 0;
        }

        int plot(const std::vector<double>& x)
        {
            return plot(x, "r");
        }

        int plot(const std::vector<std::complex<double> >& x)
        {
            std::vector<double> f;
            for(int i = 0; i < x.size(); i ++)
            {
                f.push_back(x[i].real());
            }
            return plot(f);
        }

        int plot(const std::vector<double>& x, const std::string& op)
        {
            std::vector<double> k;

            for(int i = 0; i < x.size(); i++)
            {
                k.push_back(i);
            }

            return plot(k, x, op);
        }

        int plot(const std::vector<std::complex<double> >& x, const std::string& op)
        {
            std::vector<double> f;
            for(int i = 0; i < x.size(); i ++)
            {
                f.push_back(x[i].real());
            }
            return plot(f, op);
        }


        void subplot(int v)
        {
            PyObject* args = PyTuple_New(1);
            PyTuple_SetItem(args,0,Py_BuildValue("i",v));

            if(!funcPlot_)
            {
                printf("plot null ptr\n");
                return;
            }

            PyObject_CallObject(funcSubplot_, args);

            if(args)
                Py_DECREF(args);
        }

    private:
        PyObject* pModule_;
        PyObject* pDict_;
        PyObject* funcPlot_;
        PyObject* functPlot_;
        PyObject* funcSubplot_;
        PyObject* funcShow_;
};


#endif






