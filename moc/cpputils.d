// TODO: improve
// Adapted from: https://wiki.dlang.org/Memory_Management#Explicit_Class_Instance_Allocation

module moc.cpputils;

T* cppNew(T, Args...) (Args args)
{
    import core.stdc.stdlib : malloc;

    // get size of aggregate instance in bytes
    static if ( is(T == class) )
        auto size = __traits(classInstanceSize, T);
    else static if ( is(T == struct) )
        auto size = T.sizeof;
    else
        static assert(false);

    // allocate memory for the object
    auto memory = malloc(size)[0..size];
    if(!memory) {
        import core.exception : onOutOfMemoryError;
        onOutOfMemoryError();
    }

    // call T's constructor and emplace instance on newly allocated memory
    auto result = cast(T*) memory.ptr;
    result.__ctor(args);
    return result;
}

void cppDelete(T)(T obj)
{
    import core.stdc.stdlib : free;

    // calls obj's destructor
    obj.__dtor();

    // free memory occupied by object
    free(cast(void*)obj);
}
