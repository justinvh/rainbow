#include <rainbow/file.hpp>
#include <rainbow/scripting/stdlib.hpp>
#include <rainbow/scripting/logging.hpp>
#include <rainbow/display/gui.hpp>
#include <rainbow/state.hpp>

using namespace rb;
using namespace v8;

// Add your function here.
void wrap(Handle<ObjectTemplate>& tmpl) {
    tmpl->Set(String::New("Gui"), FunctionTemplate::New(Gui::wrap));
    tmpl->Set(String::New("Logging"), FunctionTemplate::New(Logging::wrap));
}

namespace {
Persistent<ObjectTemplate> stdlib_tmpl;
}

Handle<ObjectTemplate> scripting::stdlib()
{
    if (stdlib_tmpl.IsEmpty()) {
        stdlib_tmpl = Persistent<ObjectTemplate>::New(ObjectTemplate::New());
        wrap(stdlib_tmpl);
    }
    return stdlib_tmpl;
}

bool scripting::load(Engine_state* state, const std::string& file)
{
    Contents content = read(file);
    if (!content.valid)
        return false;

    HandleScope handle_scope;
    Handle<ObjectTemplate> global = scripting::stdlib();
    global->SetInternalFieldCount(1);

    // Assign global stuff
    {
        Persistent<Context> context = Context::New(0, global);
        Handle<Object> proxy = context->Global();
        Handle<Object> proxy_as_global = proxy->GetPrototype().As<Object>();
        proxy_as_global->SetPointerInInternalField(0, state);
        Context::Scope context_scope(context);
        {
            auto source = String::New(content.data.c_str());
            auto script = Script::Compile(source);
            script->Run();
        }
        context.Dispose();
    }

    return true;
}

