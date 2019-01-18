dll_makebuffer = external_define("test.dll", "makebuffer", dll_cdecl, ty_real, 1, ty_string);

buffer = buffer_create(3, buffer_fast, 1);
external_call(dll_makebuffer, buffer_get_address(buffer));
