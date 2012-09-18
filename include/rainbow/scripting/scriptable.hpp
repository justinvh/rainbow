#ifndef RAINBOW_SCRIPTING_SCRIPTABLE_HPP
#define RAINBOW_SCRIPTING_SCRIPTABLE_HPP

#include <rainbow/v8.hpp>
#include <cassert>

namespace rb {

template <class T>
class Scriptable {
public:
    static v8::Handle<v8::Value> wrap(const v8::Arguments& args)
    {
        generic_object_template(T::name(), 
            T::function_tmpl(), T::accessors(), T::functions());
        return generic_wrap(new T, T::function_tmpl());
    }

    static void cleanup(v8::Persistent<v8::Value> handle, void* obj)
    {
        T* self = static_cast<T*>(obj);
        assert(handle == self->handle);
        assert(handle.IsNearDeath());
        handle.Dispose();
        delete self;
    }

public:
    v8::Handle<v8::Value> handle;
};

}

#endif // RAINBOW_SCRIPTING_SCRIPTABLE_HPP
