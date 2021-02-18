
namespace hooks {

	extern WNDPROC wndproc_original;
	extern HWND window;

	void initialize();
	void shutdown();

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	using create_move_fn = bool(__thiscall*)(i_client_mode*, float, c_usercmd*);
	using paint_traverse_fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
	using present_fn = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
	using reset_fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

	bool __stdcall create_move(float frame_time, c_usercmd* user_cmd);

	void __stdcall paint_traverse(unsigned int panel, bool force_repaint, bool allow_force);

	LRESULT __stdcall wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	long __stdcall present(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region);
	long __stdcall reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters);
}