dll_makebuffer = external_define("test.dll", "makebuffer", dll_cdecl, ty_real, 1, ty_string);

vbuf_size = 0;
vertex_format_begin();
vertex_format_add_color(); vbuf_size += 4; //4 bytes (RGBA)
vbuf_format = vertex_format_end();

vbuf = vertex_create_buffer();
vertex_begin(vbuf, vbuf_format);
vertex_color(vbuf, c_red, 0.5);
vertex_color(vbuf, c_lime, 0.5);
vertex_color(vbuf, c_blue, 0.5);
vertex_color(vbuf, c_black, 0);
vertex_color(vbuf, c_white, 1);
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
	//show_debug_message("D: " + string(buffer_read(buffer_from_vbuffer, buffer_u8)));	
}


//for (var i = 0; i < buffer_get_size(dll_buffer); i++) {
//	show_debug_message("D: " + );	
//}

//show_debug_message("COPI vbuf size: " + string(vertex_get_buffer_size(vbuf)));

/*
vertex_format_begin();
vertex_format_add_position_3d();
vbuf_format = vertex_format_end();

vbuf = vertex_create_buffer();
vertex_begin(vbuf, vbuf_format);
vertex_position_3d(vbuf, 10, 30, 50.5);
vertex_end(vbuf);
