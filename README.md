# AAGEShaderEditor

This template was created using `cpp_init.py` https://github.com/ZackMason/cpp_init

A simple shader editor using AAGE.

Right now only 2D screen shaders are supported, the ability to load textures, models, and animations will be added later.

Press R to reload the shader and uniforms

# How To Build 

```
mkdir build
cd build
conan install .. -s build_type=Release
cmake ..
cmake --build . --config Release
```

follow conan instructions if this is your first time building AAGE, you probably need to add --build=missing
