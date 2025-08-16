#include "lua_bindings.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <math.h>

// =============================================
// Vector3 Implementation
// =============================================

int lua_vector3_new(lua_State* L) {
    double x = luaL_checknumber(L, 1);
    double y = luaL_checknumber(L, 2);
    double z = luaL_checknumber(L, 3);
    
    Vector3* v = lua_newuserdata(L, sizeof(Vector3));
    v->x = x;
    v->y = y;
    v->z = z;
    
    luaL_getmetatable(L, "Vector3");
    lua_setmetatable(L, -2);
    
    return 1;
}

int lua_vector3_dot(lua_State* L) {
    Vector3* a = luaL_checkudata(L, 1, "Vector3");
    Vector3* b = luaL_checkudata(L, 2, "Vector3");
    
    lua_pushnumber(L, a->x * b->x + a->y * b->y + a->z * b->z);
    return 1;
}

int lua_vector3_cross(lua_State* L) {
    Vector3* a = luaL_checkudata(L, 1, "Vector3");
    Vector3* b = luaL_checkudata(L, 2, "Vector3");
    
    Vector3* result = lua_newuserdata(L, sizeof(Vector3));
    result->x = a->y * b->z - a->z * b->y;
    result->y = a->z * b->x - a->x * b->z;
    result->z = a->x * b->y - a->y * b->x;
    
    luaL_getmetatable(L, "Vector3");
    lua_setmetatable(L, -2);
    
    return 1;
}

int lua_vector3_magnitude(lua_State* L) {
    Vector3* v = luaL_checkudata(L, 1, "Vector3");
    lua_pushnumber(L, sqrt(v->x * v->x + v->y * v->y + v->z * v->z));
    return 1;
}

int lua_vector3_unit(lua_State* L) {
    Vector3* v = luaL_checkudata(L, 1, "Vector3");
    double mag = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    
    Vector3* result = lua_newuserdata(L, sizeof(Vector3));
    if (mag > 0) {
        result->x = v->x / mag;
        result->y = v->y / mag;
        result->z = v->z / mag;
    } else {
        result->x = 0;
        result->y = 0;
        result->z = 0;
    }
    
    luaL_getmetatable(L, "Vector3");
    lua_setmetatable(L, -2);
    
    return 1;
}

int lua_vector3_add(lua_State* L) {
    Vector3* a = luaL_checkudata(L, 1, "Vector3");
    Vector3* b = luaL_checkudata(L, 2, "Vector3");
    
    Vector3* result = lua_newuserdata(L, sizeof(Vector3));
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
    
    luaL_getmetatable(L, "Vector3");
    lua_setmetatable(L, -2);
    
    return 1;
}

int lua_vector3_sub(lua_State* L) {
    Vector3* a = luaL_checkudata(L, 1, "Vector3");
    Vector3* b = luaL_checkudata(L, 2, "Vector3");
    
    Vector3* result = lua_newuserdata(L, sizeof(Vector3));
    result->x = a->x - b->x;
    result->y = a->y - b->y;
    result->z = a->z - b->z;
    
    luaL_getmetatable(L, "Vector3");
    lua_setmetatable(L, -2);
    
    return 1;
}

int lua_vector3_mul(lua_State* L) {
    Vector3* v = luaL_checkudata(L, 1, "Vector3");
    double scalar = luaL_checknumber(L, 2);
    
    Vector3* result = lua_newuserdata(L, sizeof(Vector3));
    result->x = v->x * scalar;
    result->y = v->y * scalar;
    result->z = v->z * scalar;
    
    luaL_getmetatable(L, "Vector3");
    lua_setmetatable(L, -2);
    
    return 1;
}

int lua_vector3_div(lua_State* L) {
    Vector3* v = luaL_checkudata(L, 1, "Vector3");
    double scalar = luaL_checknumber(L, 2);
    
    Vector3* result = lua_newuserdata(L, sizeof(Vector3));
    result->x = v->x / scalar;
    result->y = v->y / scalar;
    result->z = v->z / scalar;
    
    luaL_getmetatable(L, "Vector3");
    lua_setmetatable(L, -2);
    
    return 1;
}

int lua_vector3_eq(lua_State* L) {
    Vector3* a = luaL_checkudata(L, 1, "Vector3");
    Vector3* b = luaL_checkudata(L, 2, "Vector3");
    
    lua_pushboolean(L, 
        a->x == b->x && 
        a->y == b->y && 
        a->z == b->z
    );
    return 1;
}

int lua_vector3_tostring(lua_State* L) {
    Vector3* v = luaL_checkudata(L, 1, "Vector3");
    lua_pushfstring(L, "%f, %f, %f", v->x, v->y, v->z);
    return 1;
}

int lua_vector3_lerp(lua_State* L) {
    Vector3* a = luaL_checkudata(L, 1, "Vector3");
    Vector3* b = luaL_checkudata(L, 2, "Vector3");
    double alpha = luaL_checknumber(L, 3);
    
    Vector3* result = lua_newuserdata(L, sizeof(Vector3));
    result->x = a->x + (b->x - a->x) * alpha;
    result->y = a->y + (b->y - a->y) * alpha;
    result->z = a->z + (b->z - a->z) * alpha;
    
    luaL_getmetatable(L, "Vector3");
    lua_setmetatable(L, -2);
    
    return 1;
}

// =============================================
// CFrame Implementation
// =============================================

int lua_cframe_new(lua_State* L) {
    // Constructor variants:
    // CFrame.new()
    // CFrame.new(pos)
    // CFrame.new(pos, lookAt)
    // CFrame.new(x, y, z)
    // CFrame.new(x, y, z, qx, qy, qz, qw)
    
    CFrame* cf = lua_newuserdata(L, sizeof(CFrame));
    
    if (lua_gettop(L) == 0) {
        // Empty constructor
        *cf = CFrame_identity();
    }
    else if (lua_isuserdata(L, 1) && luaL_checkudata(L, 1, "Vector3")) {
        Vector3* pos = luaL_checkudata(L, 1, "Vector3");
        
        if (lua_gettop(L) >= 2 && lua_isuserdata(L, 2) && luaL_checkudata(L, 2, "Vector3")) {
            // Position + lookAt constructor
            Vector3* lookAt = luaL_checkudata(L, 2, "Vector3");
            *cf = CFrame_lookAt(*pos, *lookAt);
        } else {
            // Position only constructor
            cf->position = *pos;
            cf->right = (Vector3){1, 0, 0};
            cf->up = (Vector3){0, 1, 0};
            cf->back = (Vector3){0, 0, 1};
        }
    }
    else if (lua_isnumber(L, 1)) {
        // Numeric constructor
        double x = luaL_checknumber(L, 1);
        double y = luaL_checknumber(L, 2);
        double z = luaL_checknumber(L, 3);
        
        if (lua_gettop(L) >= 7) {
            // Full orientation constructor
            double qx = luaL_checknumber(L, 4);
            double qy = luaL_checknumber(L, 5);
            double qz = luaL_checknumber(L, 6);
            double qw = luaL_checknumber(L, 7);
            *cf = CFrame_fromQuaternion((Vector3){x, y, z}, qx, qy, qz, qw);
        } else {
            // Position only
            cf->position = (Vector3){x, y, z};
            cf->right = (Vector3){1, 0, 0};
            cf->up = (Vector3){0, 1, 0};
            cf->back = (Vector3){0, 0, 1};
        }
    }
    
    luaL_getmetatable(L, "CFrame");
    lua_setmetatable(L, -2);
    return 1;
}

int lua_cframe_inverse(lua_State* L) {
    CFrame* cf = luaL_checkudata(L, 1, "CFrame");
    CFrame inv = CFrame_inverse(*cf);
    
    CFrame* result = lua_newuserdata(L, sizeof(CFrame));
    *result = inv;
    
    luaL_getmetatable(L, "CFrame");
    lua_setmetatable(L, -2);
    return 1;
}

int lua_cframe_toWorldSpace(lua_State* L) {
    CFrame* cf = luaL_checkudata(L, 1, "CFrame");
    CFrame* other = luaL_checkudata(L, 2, "CFrame");
    
    CFrame* result = lua_newuserdata(L, sizeof(CFrame));
    *result = CFrame_multiply(*cf, *other);
    
    luaL_getmetatable(L, "CFrame");
    lua_setmetatable(L, -2);
    return 1;
}

int lua_cframe_toObjectSpace(lua_State* L) {
    CFrame* cf = luaL_checkudata(L, 1, "CFrame");
    CFrame* other = luaL_checkudata(L, 2, "CFrame");
    
    CFrame inv = CFrame_inverse(*cf);
    CFrame* result = lua_newuserdata(L, sizeof(CFrame));
    *result = CFrame_multiply(inv, *other);
    
    luaL_getmetatable(L, "CFrame");
    lua_setmetatable(L, -2);
    return 1;
}

int lua_cframe_pointToWorldSpace(lua_State* L) {
    CFrame* cf = luaL_checkudata(L, 1, "CFrame");
    Vector3* point = luaL_checkudata(L, 2, "Vector3");
    
    Vector3* result = lua_newuserdata(L, sizeof(Vector3));
    *result = CFrame_pointToWorldSpace(*cf, *point);
    
    luaL_getmetatable(L, "Vector3");
    lua_setmetatable(L, -2);
    return 1;
}

int lua_cframe_pointToObjectSpace(lua_State* L) {
    CFrame* cf = luaL_checkudata(L, 1, "CFrame");
    Vector3* point = luaL_checkudata(L, 2, "Vector3");
    
    Vector3* result = lua_newuserdata(L, sizeof(Vector3));
    *result = CFrame_pointToObjectSpace(*cf, *point);
    
    luaL_getmetatable(L, "Vector3");
    lua_setmetatable(L, -2);
    return 1;
}

int lua_cframe_components(lua_State* L) {
    CFrame* cf = luaL_checkudata(L, 1, "CFrame");
    
    lua_pushnumber(L, cf->position.x);
    lua_pushnumber(L, cf->position.y);
    lua_pushnumber(L, cf->position.z);
    
    lua_pushnumber(L, cf->right.x);
    lua_pushnumber(L, cf->up.x);
    lua_pushnumber(L, cf->back.x);
    
    lua_pushnumber(L, cf->right.y);
    lua_pushnumber(L, cf->up.y);
    lua_pushnumber(L, cf->back.y);
    
    lua_pushnumber(L, cf->right.z);
    lua_pushnumber(L, cf->up.z);
    lua_pushnumber(L, cf->back.z);
    
    return 12;
}

int lua_cframe_tostring(lua_State* L) {
    CFrame* cf = luaL_checkudata(L, 1, "CFrame");
    lua_pushfstring(L, "CFrame(%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f)",
        cf->position.x, cf->position.y, cf->position.z,
        cf->right.x, cf->right.y, cf->right.z,
        cf->up.x, cf->up.y, cf->up.z,
        cf->back.x, cf->back.y, cf->back.z);
    return 1;
}

// =============================================
// Instance Implementation
// =============================================

int lua_instance_new(lua_State* L) {
    const char* className = luaL_checkstring(L, 1);
    
    Instance* inst = InstanceNew(className);
    lua_pushlightuserdata(L, inst);
    
    luaL_getmetatable(L, "Instance");
    lua_setmetatable(L, -2);
    
    return 1;
}

int lua_instance_get_children(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    
    lua_createtable(L, inst->ChildrenCount, 0);
    for (int i = 0; i < inst->ChildrenCount; i++) {
        lua_pushlightuserdata(L, inst->Children[i]);
        luaL_getmetatable(L, "Instance");
        lua_setmetatable(L, -2);
        lua_rawseti(L, -2, i + 1);
    }
    
    return 1;
}

int lua_instance_find_first_child(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    const char* name = luaL_checkstring(L, 2);
    bool recursive = lua_toboolean(L, 3);
    
    Instance* result = InstanceFindFirstChild(inst, name, recursive);
    if (result) {
        lua_pushlightuserdata(L, result);
        luaL_getmetatable(L, "Instance");
        lua_setmetatable(L, -2);
    } else {
        lua_pushnil(L);
    }
    
    return 1;
}

int lua_instance_destroy(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    InstanceDestroy(inst);
    return 0;
}

int lua_instance_clone(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    Instance* clone = InstanceClone(inst);
    
    lua_pushlightuserdata(L, clone);
    luaL_getmetatable(L, "Instance");
    lua_setmetatable(L, -2);
    return 1;
}

int lua_instance_get_property(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    const char* name = luaL_checkstring(L, 2);
    
    Variant* prop = InstanceGetProperty(inst, name);
    if (prop) {
        PushVariant(L, prop);
    } else {
        lua_pushnil(L);
    }
    
    return 1;
}

int lua_instance_set_property(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    const char* name = luaL_checkstring(L, 2);
    
    Variant value;
    GetVariant(L, 3, &value);
    InstanceSetProperty(inst, name, &value);
    return 0;
}

int lua_instance_get_attribute(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    const char* name = luaL_checkstring(L, 2);
    
    Variant* attr = InstanceGetAttribute(inst, name);
    if (attr) {
        PushVariant(L, attr);
    } else {
        lua_pushnil(L);
    }
    
    return 1;
}

int lua_instance_set_attribute(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    const char* name = luaL_checkstring(L, 2);
    
    Variant value;
    GetVariant(L, 3, &value);
    InstanceSetAttribute(inst, name, &value);
    return 0;
}

int lua_instance_get_parent(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    
    if (inst->Parent) {
        lua_pushlightuserdata(L, inst->Parent);
        luaL_getmetatable(L, "Instance");
        lua_setmetatable(L, -2);
    } else {
        lua_pushnil(L);
    }
    
    return 1;
}

int lua_instance_set_parent(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    
    if (lua_isnil(L, 2)) {
        InstanceSetParent(inst, NULL);
    } else {
        Instance* parent = luaL_checkudata(L, 2, "Instance");
        InstanceSetParent(inst, parent);
    }
    
    return 0;
}

int lua_instance_get_class_name(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    lua_pushstring(L, inst->ClassName);
    return 1;
}

int lua_instance_get_name(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    lua_pushstring(L, inst->Name);
    return 1;
}

int lua_instance_set_name(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    const char* name = luaL_checkstring(L, 2);
    strncpy(inst->Name, name, MAX_INSTANCE_NAME_LENGTH);
    return 0;
}

int lua_instance_is_a(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    const char* className = luaL_checkstring(L, 2);
    lua_pushboolean(L, InstanceIsA(inst, className));
    return 1;
}

int lua_instance_find_service(lua_State* L) {
    const char* serviceName = luaL_checkstring(L, 1);
    Instance* service = DataModelFindService(serviceName);
    
    if (service) {
        lua_pushlightuserdata(L, service);
        luaL_getmetatable(L, "Instance");
        lua_setmetatable(L, -2);
    } else {
        lua_pushnil(L);
    }
    
    return 1;
}

int lua_instance_get_service(lua_State* L) {
    const char* serviceName = luaL_checkstring(L, 1);
    Instance* service = DataModelGetService(serviceName);
    
    lua_pushlightuserdata(L, service);
    luaL_getmetatable(L, "Instance");
    lua_setmetatable(L, -2);
    return 1;
}

int lua_instance_get_descendants(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    
    int count = 0;
    Instance** descendants = InstanceGetDescendants(inst, &count);
    
    lua_createtable(L, count, 0);
    for (int i = 0; i < count; i++) {
        lua_pushlightuserdata(L, descendants[i]);
        luaL_getmetatable(L, "Instance");
        lua_setmetatable(L, -2);
        lua_rawseti(L, -2, i + 1);
    }
    
    free(descendants);
    return 1;
}

int lua_instance_get_ancestors(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    
    int count = 0;
    Instance** ancestors = InstanceGetAncestors(inst, &count);
    
    lua_createtable(L, count, 0);
    for (int i = 0; i < count; i++) {
        lua_pushlightuserdata(L, ancestors[i]);
        luaL_getmetatable(L, "Instance");
        lua_setmetatable(L, -2);
        lua_rawseti(L, -2, i + 1);
    }
    
    free(ancestors);
    return 1;
}

int lua_instance_tostring(lua_State* L) {
    Instance* inst = luaL_checkudata(L, 1, "Instance");
    lua_pushfstring(L, "%s (%s)", inst->Name, inst->ClassName);
    return 1;
}

// =============================================
// Signal Implementation
// =============================================

int lua_signal_connect(lua_State* L) {
    RBXScriptSignal* signal = luaL_checkudata(L, 1, "RBXScriptSignal");
    luaL_checktype(L, 2, LUA_TFUNCTION);
    
    // Create connection
    RBXScriptConnection* conn = SignalConnect(signal, L, 2);
    
    // Push connection object
    lua_pushlightuserdata(L, conn);
    luaL_getmetatable(L, "RBXScriptConnection");
    lua_setmetatable(L, -2);
    
    return 1;
}

int lua_signal_wait(lua_State* L) {
    RBXScriptSignal* signal = luaL_checkudata(L, 1, "RBXScriptSignal");
    
    // Implement signal wait using coroutines
    if (lua_pushthread(L)) {
        // We're in a coroutine
        SignalWait(signal, L);
        return lua_yield(L, 0);
    } else {
        luaL_error(L, "Signal:wait() can only be called from a coroutine");
        return 0;
    }
}

int lua_connection_disconnect(lua_State* L) {
    RBXScriptConnection* conn = luaL_checkudata(L, 1, "RBXScriptConnection");
    SignalDisconnect(conn);
    return 0;
}

// =============================================
// Registration
// =============================================

static const luaL_Reg vector3_methods[] = {
    {"Dot", lua_vector3_dot},
    {"Cross", lua_vector3_cross},
    {"Magnitude", lua_vector3_magnitude},
    {"Unit", lua_vector3_unit},
    {"Lerp", lua_vector3_lerp},
    {"__add", lua_vector3_add},
    {"__sub", lua_vector3_sub},
    {"__mul", lua_vector3_mul},
    {"__div", lua_vector3_div},
    {"__eq", lua_vector3_eq},
    {"__tostring", lua_vector3_tostring},
    {NULL, NULL}
};

static const luaL_Reg cframe_methods[] = {
    {"inverse", lua_cframe_inverse},
    {"toWorldSpace", lua_cframe_toWorldSpace},
    {"toObjectSpace", lua_cframe_toObjectSpace},
    {"pointToWorldSpace", lua_cframe_pointToWorldSpace},
    {"pointToObjectSpace", lua_cframe_pointToObjectSpace},
    {"components", lua_cframe_components},
    {"__mul", lua_cframe_toWorldSpace},
    {"__tostring", lua_cframe_tostring},
    {NULL, NULL}
};

static const luaL_Reg instance_methods[] = {
    {"GetChildren", lua_instance_get_children},
    {"FindFirstChild", lua_instance_find_first_child},
    {"Destroy", lua_instance_destroy},
    {"Clone", lua_instance_clone},
    {"GetProperty", lua_instance_get_property},
    {"SetProperty", lua_instance_set_property},
    {"GetAttribute", lua_instance_get_attribute},
    {"SetAttribute", lua_instance_set_attribute},
    {"GetParent", lua_instance_get_parent},
    {"SetParent", lua_instance_set_parent},
    {"GetClassName", lua_instance_get_class_name},
    {"GetName", lua_instance_get_name},
    {"SetName", lua_instance_set_name},
    {"IsA", lua_instance_is_a},
    {"FindService", lua_instance_find_service},
    {"GetService", lua_instance_get_service},
    {"GetDescendants", lua_instance_get_descendants},
    {"GetAncestors", lua_instance_get_ancestors},
    {"__tostring", lua_instance_tostring},
    {NULL, NULL}
};

static const luaL_Reg signal_methods[] = {
    {"Connect", lua_signal_connect},
    {"connect", lua_signal_connect},
    {"Wait", lua_signal_wait},
    {"wait", lua_signal_wait},
    {NULL, NULL}
};

static const luaL_Reg connection_methods[] = {
    {"Disconnect", lua_connection_disconnect},
    {"disconnect", lua_connection_disconnect},
    {NULL, NULL}
};

void RegisterAllLuaBindings(lua_State* L) {
    // Register Vector3
    luaL_newmetatable(L, "Vector3");
    luaL_setfuncs(L, vector3_methods, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    
    // Register CFrame
    luaL_newmetatable(L, "CFrame");
    luaL_setfuncs(L, cframe_methods, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    
    // Register Instance
    luaL_newmetatable(L, "Instance");
    luaL_setfuncs(L, instance_methods, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    
    // Register RBXScriptSignal
    luaL_newmetatable(L, "RBXScriptSignal");
    luaL_setfuncs(L, signal_methods, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    
    // Register RBXScriptConnection
    luaL_newmetatable(L, "RBXScriptConnection");
    luaL_setfuncs(L, connection_methods, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    
    // Register global functions
    lua_register(L, "Vector3", lua_vector3_new);
    lua_register(L, "CFrame", lua_cframe_new);
    lua_register(L, "Instance", lua_instance_new);
    
    // Create game table
    lua_newtable(L);
    lua_setglobal(L, "game");
    
    // Register services
    lua_getglobal(L, "game");
    
    // Workspace
    lua_pushstring(L, "Workspace");
    lua_pushlightuserdata(L, GetWorkspace());
    luaL_getmetatable(L, "Instance");
    lua_setmetatable(L, -2);
    lua_settable(L, -3);
    
    // Players
    lua_pushstring(L, "Players");
    lua_pushlightuserdata(L, GetPlayersService());
    luaL_getmetatable(L, "Instance");
    lua_setmetatable(L, -2);
    lua_settable(L, -3);
    
    // Lighting
    lua_pushstring(L, "Lighting");
    lua_pushlightuserdata(L, GetLightingService());
    luaL_getmetatable(L, "Instance");
    lua_setmetatable(L, -2);
    lua_settable(L, -3);
    
    // ... register all other services
    
    lua_pop(L, 1); // pop game table
}
