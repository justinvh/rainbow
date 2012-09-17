#ifndef V8_EASY_HH
#define V8_EASY_HH

#include <v8.h>

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
    v8::Handle<v8::Value> (*getter)(v8::Local<v8::String> name, 
                                    const v8::AccessorInfo& info);
    void (*setter)(v8::Local<v8::String> name, 
                   v8::Local<v8::Value> value,
                   const v8::AccessorInfo& info);
};

#define easy_accessor_map(klass, method) \
    {#method, \
     klass::easy_getter_function(method), \
     klass::easy_setter_function(method)}

#endif // V8_EASY_HH
