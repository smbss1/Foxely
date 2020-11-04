#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string.h>

#include "library/library.h"
#include "foxely.h"

ObjectAbstractType foxely_file_type =
{
    "core/file"
};

Value openNative(VM* oVM, int argCount, Value* args)
{
    Fox_FixArity(oVM, argCount, 2);
	Fox_PanicIfNot(oVM, Fox_IsString(args[0]), "Expected string value");
	Fox_PanicIfNot(oVM, Fox_IsString(args[1]), "Expected string value");
	
    FILE* fp = fopen(Fox_ValueToCString(args[0]), Fox_ValueToCString(args[1]));
    if (fp)
    {
        Value instance = Fox_DefineInstanceOfCStruct(oVM, "File", fp);
        return instance;
    }
	Fox_RuntimeError(oVM, "'%s' doesn't exist.", Fox_ValueToCString(args[0]));
    return NIL_VAL;
}

Value readNative(VM* oVM, int argCount, Value* args)
{
    Fox_Arity(oVM, argCount, 0, 1);
    FILE* fp = (FILE *) Fox_GetInstanceCStruct(args[-1]);

    if (argCount == 0)
    {
        size_t len = 0;

        fseek(fp, 0, SEEK_END);
        len = ftell(fp);
        rewind(fp);

        char fcontent[len];
        size_t lBytes = fread(fcontent, 1, len, fp);

        if (lBytes < 0)
            return NIL_VAL;

        fcontent[len] = 0;

        return Fox_StringToValue(oVM, fcontent);
    }
    else
    {
        Fox_PanicIfNot(oVM, Fox_IsNumber(args[0]), "Invalid type, expected number type");
        int size = Fox_ValueToNumber(args[0]);
        char chunk[size];

        if (fgets(chunk, sizeof(chunk), fp) != NULL)
            return Fox_StringToValue(oVM, chunk);
    }
    return NIL_VAL;
}

Value readLineNative(VM* oVM, int argCount, Value* args)
{
    Fox_FixArity(oVM, argCount, 0);
    FILE* fp = (FILE *) Fox_GetInstanceCStruct(args[-1]);
    size_t len = 0;
    char* line = NULL;
    ssize_t read;
    
    read = getline(&line, &len, fp);
    if (read != -1) {
        Value lineValue = Fox_StringToValue(oVM, line);
        if (line)
            free(line);
        return lineValue;
    }
    return NIL_VAL;
}

Value writeNative(VM* oVM, int argCount, Value* args)
{
    Fox_FixArity(oVM, argCount, 1);
    FILE* fp = (FILE *) Fox_GetInstanceCStruct(args[-1]);

    Fox_PanicIfNot(oVM, Fox_IsString(args[0]), "Expected string value in write function");
    fputs(Fox_ValueToCString(args[0]), fp);
    return NIL_VAL;
}

Value closeNative(VM* oVM, int argCount, Value* args)
{
    Fox_FixArity(oVM, argCount, 0);
    FILE* fp = (FILE *) Fox_GetInstanceCStruct(args[-1]);
	if (fp)
        fclose(fp);
    return NIL_VAL;
}

NativeMethods methods =
{
    std::make_pair<std::string, NativeFn>("open", openNative),
};

NativeMethods fileMethods =
{
    std::make_pair<std::string, NativeFn>("write", writeNative),
    std::make_pair<std::string, NativeFn>("read", readNative),
    std::make_pair<std::string, NativeFn>("readline", readLineNative),
    std::make_pair<std::string, NativeFn>("close", closeNative),
};

void DefineIOModule(VM* oVM)
{
    oVM->DefineModule("io");
    oVM->DefineClass("io", "io", methods);
    oVM->DefineClass("io", "File", fileMethods);
}