#include "object.hpp"
#include "gc.hpp"
#include "Parser.h"
#include "vm.hpp"

ObjectClosure::ObjectClosure(VM* oVM, ObjectFunction *func)
{
    type = OBJ_CLOSURE;
    function = func;
    upvalueCount = func->upValueCount;
    func->module = oVM->currentModule;
    upValues = oVM->gc.NewArray<ObjectUpvalue*>(func->upValueCount);
    for (int i = 0; i < func->upValueCount; i++) {
        upValues[i] = NULL;
    }
}