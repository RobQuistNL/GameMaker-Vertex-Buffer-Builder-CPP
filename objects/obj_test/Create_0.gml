dll_makebuffer = external_define("test.dll", "makebuffer", dll_cdecl, ty_real, 1, ty_string);

vbuf_size = 0;
vertex_format_begin();
vertex_format_add_color(); vbuf_size += 4; //4 bytes (RGBA)
vertex_format_add_position_3d(); vbuf_size += 3 * 4; //4 bytes (F32) * 3
vertex_format_add_normal(); vbuf_size += 3 * 4; //4 bytes (F32) * 3
vertex_format_add_texcoord(); vbuf_size += 2 * 4; //4 bytes (F32) * 4
vbuf_format = vertex_format_end();

vbuf = vertex_create_buffer();
vertex_begin(vbuf, vbuf_format);
vertex_color(vbuf, c_red, 0.5);
vertex_position_3d(vbuf, 10, 20, 333333.35);
vertex_normal(vbuf, 4, 3, 2);
vertex_texcoord(vbuf, 0.0115, 0.0115);

vertex_color(vbuf, c_red, 0.5);
vertex_position_3d(vbuf, 50, 60, 9999934234.35);
vertex_normal(vbuf, 6, 5, 4);
vertex_texcoord(vbuf, 0.000115, 0.000115);

vertex_end(vbuf);

show_debug_message("vbuf size: " + string(vertex_get_buffer_size(vbuf)) + " (calc: "+string(vbuf_size)+")");

buffer_from_vbuffer = buffer_create_from_vertex_buffer(vbuf, buffer_fast, 1);
show_debug_message("actual buffer size: " + string(buffer_get_size(buffer_from_vbuffer)));

dll_buffer = buffer_create(vbuf_size*5, buffer_fast, 1);
external_call(dll_makebuffer, buffer_get_address(dll_buffer));

for (var i = 0; i < buffer_get_size(buffer_from_vbuffer); i++) {
	var gmbuf = buffer_read(buffer_from_vbuffer, buffer_u8);
	var dllbuf = buffer_read(dll_buffer, buffer_u8);
	var extrstr = "";
	if (gmbuf != dllbuf) {
		extrstr = "MISMATCH";
	}
	show_debug_message("A: " + string(gmbuf) + "\t/ " + string(dllbuf) + "\t" + extrstr);
}

