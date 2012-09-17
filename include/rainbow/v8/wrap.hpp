#ifndef RAINBOW_V8_WRAP_HPP
#define RAINBOW_V8_WRAP_HPP

#include <v8.h>
#include <rainbow/v8/easy.hpp>

namespace rb {
inline
void generic_object_template(const char* name,
    v8::Handle<v8::FunctionTemplate>& function_template,
    Easy_accessor* accessor_list)
{
    if (!function_template.IsEmpty()) return;

    v8::Local<v8::FunctionTemplate> tmpl = v8::FunctionTemplate::New();
    tmpl->SetClassName(v8::String::New(name));
    v8::Local<v8::ObjectTemplate> self = tmpl->InstanceTemplate();
    self->SetInternalFieldCount(1);
    v8::Local<v8::ObjectTemplate> proto = tmpl->PrototypeTemplate();
    for (int i = 0; ; i++) {
        const Easy_accessor& e = accessor_list[i];
        if (e.name == nullptr) break;
        proto->SetAccessor(v8::String::NewSymbol(e.name), e.getter, e.setter);
    }
    function_template = v8::Persistent<v8::FunctionTemplate>::New(tmpl);
}

template<class T>
v8::Handle<v8::Value> generic_wrap(T* external_instance,
    const v8::Handle<v8::FunctionTemplate>& function_template)
{
    if (function_template.IsEmpty()) {
        // do something
    }

    v8::HandleScope handle_scope;
    v8::Handle<v8::Function> ctor = function_template->GetFunction();
    v8::Persistent<v8::Object> obj = 
        v8::Persistent<v8::Object>::New(ctor->NewInstance());
    obj.MakeWeak(external_instance, T::cleanup);
    obj.MarkIndependent();
    obj->SetInternalField(0, v8::External::New((void*)external_instance));
    external_instance->handle = handle_scope.Close(obj);
    return external_instance->handle;
}
}

#endif // RAINBOW_V8_WRAP_HPP
