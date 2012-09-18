#ifndef V8_EASY_HH
#define V8_EASY_HH

#include <v8.h>
#include <vector>

/*
Generically unwraps a handle to a V8 object of a specified class.
If the object does not have an internal field count then a
nullptr will be returned.
*/
template <class T>
T* unwrap(v8::Handle<v8::Object> object)
{
    if (!object->InternalFieldCount()) {
        return NULL;
    }

    auto wrapped = v8::Local<v8::External>::Cast(object->GetInternalField(0));

    if (wrapped.IsEmpty()) {
        return NULL;
    }

    return static_cast<T*>(wrapped->Value());
}

/*
Performs a unwrap for Diamond-structured classes.
*/
#define diamond_unwrap(T, S) dynamic_cast<T*>(unwrap<Element>(S));

/*
Returns the internal pointer of the current v8 context and casts it to the
appropriate object.

Why can't we just reference Global()?
http://code.google.com/p/v8/source/browse/trunk/test/cctest/test-api.cc#1470
*/
template <class T>
T* unwrap_global_pointer(int index)
{

    v8::Handle<v8::Object> proxy = v8::Context::GetCurrent()->Global();
    v8::Handle<v8::Object> global = proxy->GetPrototype().As<v8::Object>();
    void* p = global->GetPointerFromInternalField(index);
    return reinterpret_cast<T*>(p);
}

#define easy_getter_function(method) jsgen_getter_##method
#define easy_getter_name(method) jsgen_getter_name_##method
#define easy_getter(method) \
    static v8::Handle<v8::Value> easy_getter_function(method) (\
            v8::Local<v8::String> name, const v8::AccessorInfo& info);\
    static const char* easy_getter_name(method) () { return #method; }
#define easy_getter_def(klass, method) \
    v8::Handle<v8::Value> klass::easy_getter_function(method) (\
            v8::Local<v8::String> name, const v8::AccessorInfo& info)

#define easy_setter_function(method) jsgen_setter_##method
#define easy_setter_name(method) jsgen_setter_name_##method
#define easy_setter(method) \
    static void easy_setter_function(method) (\
            v8::Local<v8::String> name, v8::Local<v8::Value> value,\
            const v8::AccessorInfo& info);\
    static const char* easy_setter_name(method) () \
    {return #method;}
#define easy_setter_def(klass, method) \
    void klass::easy_setter_function(method) (\
            v8::Local<v8::String> name, v8::Local<v8::Value> value,\
            const v8::AccessorInfo& info)


#define easy_accessor(method) \
    easy_getter(method); \
    easy_setter(method);

#define easy_function(method) \
    static v8::Handle<v8::Value> jsgen_function_##method (\
            const v8::Arguments & args);

struct Easy_accessor {
    const char* name;
    v8::Handle<v8::Value> (*getter)(v8::Local<v8::String>, 
                                       const v8::AccessorInfo&);
    void (*setter)(v8::Local<v8::String>, 
                   v8::Local<v8::Value>, 
                   const v8::AccessorInfo&);
};

struct Easy_function {
    const char* name;
    v8::Handle<v8::Value>(*caller)(const v8::Arguments&);
};

typedef std::vector<Easy_accessor> Accessors;
typedef std::vector<Easy_function> Functions;

#define easy_accessor_map(klass, method) \
    {#method, \
     klass::easy_getter_function(method), \
     klass::easy_setter_function(method)}

#endif // V8_EASY_HH
