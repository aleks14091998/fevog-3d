#ifndef _VALK_ENG_H
#define _VALK_ENG_H

#include"renderer/GeometryBuffer.h"
#include"renderer/Shader.h"
#include"renderer/Material.h"

#include"iobject.h"
#include<vector>

#if defined(_WIN32) || defined(_WIN64)

#include <Windows.h>

#define PLATFORM_WINDOWS
#define VALK_USE_COM

#ifdef _WIN64
#	pragma message("Target platform is WIN64.")
#else //_WIN64
#	pragma message("Target platform is WIN32.")
#endif

#else//_WIN32 or _WIN64
#	error Unknown platform.

#endif//_WIN32 or _WIN64

namespace valk {

	// signed //
	typedef short int				int16;
	typedef long int				int32;
	typedef signed char				int8;
	typedef wchar_t					wchar;

	// unsigned //
	typedef unsigned int			uint;
	typedef unsigned short int		uint16;
	typedef unsigned long int		uint32;
	typedef unsigned char			uint8;
	typedef uint8					uchar;

#if defined(PLATFORM_WINDOWS)
	typedef __int64					int64;
	typedef unsigned __int64		uint64;

	typedef HWND				WindowHandle;

#else//PLATFORM_WINDOWS

#define CALLBACK
	typedef signed long long		int64;
	typedef unsigned long long		uint64;

	struct GUID
	{
		ui32	Data1;
		ui16	Data2;
		ui16	Data3;
		uint8	Data4[8];
	};

	typedef int32 HRESULT;

	/** Return value of HRESULT type. Method returns S_OK when all of its routines were done succesfully. */
#define S_OK			0
#define S_FALSE			1
#define E_FAIL			0x80004005
#define E_INVALIDARG	0x80070057
#define E_ABORT			0x80004004

#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)

#endif//PLATFORM_WINDOWS

	class VALK_Base
#if defined(PLATFORM_WINDOWS) && defined(VALK_USE_COM)
		: public IUnknown
#endif
	{
	public:
		virtual HRESULT CALLBACK GetGUID(GUID &guid) = 0;
	};

#if defined(STRUCT_ALIGNMENT_1) && defined(PLATFORM_WINDOWS)
#pragma pack( push, 1 )
#endif

	enum E_WINDOW_MESSAGE_TYPE
	{
		WMT_UNKNOWN = 0,/**< This type will be returned if there is no proper conversion from platform specific window event to engine window message. Message parameters have platform specific usage. \note For Windows pParam3 of TWinMessage structure will point to the MSG structure. */
		WMT_REDRAW,		/**< Message indicates that window must redraw itself. Message parameters are not used. */
		WMT_PRESENT,	/**< Message indicates that window is ready and now will be displayed to the user for the first time. Message parameters are not used. */
		WMT_CLOSE,		/**< Message indicates that window is ready to be destroyed and now will disappear from the screen. Message parameters are not used. */
		WMT_CREATE,		/**< Message indicates that window is being created. Message parameters are not used. */
		WMT_DESTROY,	/**< Message indicates that window is ready to be destroyed. Message parameters are not used. */
		WMT_RELEASED,	/**< Message indicates that window was destroyed and released successfully. Message parameters are not used. */
		WMT_ACTIVATED,	/**< Message indicates that window became a foreground window and get user input focus. Message parameters are not used. */
		WMT_DEACTIVATED,/**< Message indicates that window became a background window and lost user input focus. Message parameters are not used. */
		WMT_MINIMIZED,	/**< Message indicates that window was minimized. Message parameter ui32Param1 stores width of the window and ui32Param2 stores height. \note Under Windows pParam3 points to RECT structure with window size. */
		WMT_RESTORED,	/**< Message indicates that window was restored to its normal state. Message parameter ui32Param1 stores width of the window and ui32Param2 stores height. \note Under Windows pParam3 points to RECT structure with window size. */
		WMT_MOVE,		/**< Message indicates that window is being moved. Message parameter ui32Param1 stores x coordinate of upper left window corner and ui32Param2 stores y coordinate. \note Under Windows pParam3 points to RECT structure with window size. */
		WMT_SIZE,		/**< Message indicates that window is being sized. Message parameter ui32Param1 stores width of the window and ui32Param2 stores height. \note Under Windows pParam3 points to RECT structure with window size. */
		WMT_KEY_UP,		/**< Message indicates that the user has released some keyboard key. Message parameter ui32Param1 is the engine code of the key being released. \see E_KEYBOARD_KEY_CODES */
		WMT_KEY_DOWN,	/**< Message indicates that the user has pressed some keyboard key. Message parameter ui32Param1 is the engine code of the key being pressed. \see E_KEYBOARD_KEY_CODES */
		WMT_ENTER_CHAR, /**< Message indicates that the user has pressed some keyboard key. Message parameter ui32Param1 is the ASCII code of the key being pressed. */
		WMT_MOUSE_MOVE, /**< Message indicates that cursor is being moved within the window. Message parameter ui32Param1 stores x coordinate of cursor and ui32Param1 stores y coordinate. */
		WMT_MOUSE_DOWN, /**< Message indicates that the user has pressed mouse button. Message parameter ui32Param1 indicates what button was pressed: 0 - Left, 1 - Right, 2 - Middle */
		WMT_MOUSE_UP,	/**< Message indicates that the user has released mouse button. Message parameter ui32Param1 indicates what button was released: 0 - Left, 1 - Right, 2 - Middle */
		WMT_MOUSE_WHEEL /**< Message indicates that the user has rolled mouse wheel. Message parameter pParam3 points to integer (int) with mouse wheel delta value. */
	};

	struct TWinMessage
	{
		E_WINDOW_MESSAGE_TYPE
			uiMsgType;	/**< Window message type identifier. */
		uint32	ui32Param1;	/**< Message first parametr. */
		uint32	ui32Param2;	/**< Message second parametr. */
		void	*pParam3;	/**< Message third parametr. Points to specific message data. */

		TWinMessage() :uiMsgType(WMT_UNKNOWN), ui32Param1(0), ui32Param2(0), pParam3(NULL) {}
		TWinMessage(E_WINDOW_MESSAGE_TYPE msg, uint32 param1 = 0, uint32 param2 = 0, void *param3 = NULL) :uiMsgType(msg), ui32Param1(param1), ui32Param2(param2), pParam3(param3) {}
	};

#if defined(STRUCT_ALIGNMENT_1) && defined(PLATFORM_WINDOWS)
#pragma pack(pop)
#endif

	enum E_ENGINE_INIT_FLAGS
	{
		EIF_DEFAULT = 0x00000000,	/**< Use default settings. */
		EIF_FULL_SCREEN = 0x00000001,	/**< Engine will run in full screen mode. */
		EIF_NATIVE_RESOLUTION = 0x00000002,	/**< If EIF_FULL_SCREEN flag is set, engine will set native system screen resolution. */
		EIF_NO_LOGGING = 0x00000004	/**< Means that engine logging will be turned off. */
	};

	enum E_ENGINE_PROCEDURE_TYPE
	{
		EPT_PROCESS = 0,	/**< Procedure is called periodically(like on timer event). Interval of calling is set on engine initialization. In this procedure you should do any application computes. \see IEngineCore::InitializeEngine */
		EPT_RENDER,			/**< Procedure is called when engine decides to draw new frame. In this procedure you can call any rendering routines. */
		EPT_INIT,			/**< Procedure is called before engine will start its main loop. In this procedure you should load all resources needed by your application. */
		EPT_DESTRPY			/**< Procedure is called before engine quits. In this procedure you should release all resources and free memmory. */
	};

	enum E_RENDER_API
	{
		ERA_OGL43,
		ERA_DX9,
		ERA_DX11
	};

	static const GUID IID_Engine =
	{ 0x111bb884, 0x2ba6, 0x4e84, { 0x95, 0xa5, 0x5e, 0x47, 0x0, 0x30, 0x9c, 0xba } };

	class IInput;
	class IRenderAPI;
	class IWindowManager;

	class Engine : public VALK_Base
	{
	public:
		virtual HRESULT CALLBACK InitEngine(uint uiResX, uint uiResY, const char* pcApplicationName, E_ENGINE_INIT_FLAGS eInitFlags = EIF_DEFAULT) = 0;
		virtual HRESULT CALLBACK SetProcessInterval(uint uiProcessInterval) = 0;
		virtual HRESULT CALLBACK AddProcedure(E_ENGINE_PROCEDURE_TYPE eProcType, void (CALLBACK *proc)(void *parametr), void *parametr = NULL) = 0;
		virtual HRESULT CALLBACK RemoveProcedure(E_ENGINE_PROCEDURE_TYPE eProcType, void (CALLBACK *proc)(void *parametr), void *parametr = NULL) = 0;
		virtual HRESULT CALLBACK QuitEngine() = 0;
		virtual	HRESULT CALLBACK AddToLog(const char *pcTxt, bool bError = false) = 0;
		virtual	HRESULT CALLBACK GetWinManager(IWindowManager *&wm) = 0;
		virtual	HRESULT CALLBACK GetRender(IRenderAPI *&pRender) = 0;
		virtual HRESULT CALLBACK InitRenderSystem(E_RENDER_API RenderType) = 0;
	};

	namespace render {
		struct ResourceList {
			struct transform {
				float translate[3];
				float rotateAngle;
				float scale[3];
				int rotate[3];
			} transform;
			IGeometryBuffer* Mesh;
			IShader* Shader;
			Material* Material;
		};

		struct CameraParameters {
			float Position[3];
			float CenterPoint[3];
			float UpperSide[3];
		};
	}

	// {20367A9B-901D-474D-9FAF-2BC3F933FF67}
	static const GUID IID_IRenderAPI =
	{ 0x20367a9b, 0x901d, 0x474d, { 0x9f, 0xaf, 0x2b, 0xc3, 0xf9, 0x33, 0xff, 0x67 } };

	class IRenderAPI : public VALK_Base {
	public:
		virtual void Init() = 0;
		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void Draw(render::ResourceList *list) = 0;
		virtual void DrawCamera(render::CameraParameters *list) = 0;
	};

	// {389B69F5-25E3-4D06-B2E1-8DA97A2313EC}
	static const GUID IID_IPhysics =
	{ 0x389b69f5, 0x25e3, 0x4d06, { 0xb2, 0xe1, 0x8d, 0xa9, 0x7a, 0x23, 0x13, 0xec } };

	class IPhysics : public VALK_Base {
	public:

	};

	struct MouseStates {
		int iX;
		int iY;
		int iDeltaX;
		int	iDeltaY;
		int	iDeltaWheel;
		bool bLeftButton;
		bool bRightButton;
		bool bMiddleButton;
	};

	enum E_INPUT_CONFIGURATION_FLAGS {
		ICF_DEFAULT = 0x00000000,
		ICF_EXCLUSIVE = 0x00000001,
		ICF_HIDE_CURSOR = 0x00000002
	};

	enum E_KEYBOARD_KEY_CODES;

	// {FF52ADBD-BB08-4F42-A834-D394CA2BFDB2}
	static const GUID IID_IInput =
	{ 0xff52adbd, 0xbb08, 0x4f42, { 0xa8, 0x34, 0xd3, 0x94, 0xca, 0x2b, 0xfd, 0xb2 } };

	class IInput : public VALK_Base {
	public:
		virtual HRESULT CALLBACK Configure(E_INPUT_CONFIGURATION_FLAGS flag = ICF_DEFAULT) = 0;
		virtual HRESULT CALLBACK GetMouseStates(MouseStates &stMouseStates) const = 0;
		virtual HRESULT CALLBACK GetKeyState(E_KEYBOARD_KEY_CODES keyCode, bool &state) const = 0;
		virtual HRESULT CALLBACK BeginTextInput(char *buffer, uint bufferSize) = 0;
		virtual HRESULT CALLBACK EndTextInput() = 0;
		virtual void CALLBACK _SetCursorPos(bool toCenter, int x = 0, int y = 0) = 0;
		virtual void CALLBACK _GetWindowSize(int &width, int &height) = 0;
		virtual void CALLBACK _HideCursor() = 0;
		virtual void CALLBACK _ShowCursor() = 0;
	};

	// {9CF022D4-25C7-4DBD-B2BA-F75B5367F1B3}
	static const GUID IID_IWindowManager =
	{ 0x9cf022d4, 0x25c7, 0x4dbd, { 0xb2, 0xba, 0xf7, 0x5b, 0x53, 0x67, 0xf1, 0xb3 } };

	class IWindowManager : public VALK_Base {
	public:
		virtual HRESULT GetInput(IInput *&input) = 0;
		virtual void UpdateObjectList(std::vector<IObject*> objList) = 0;
	};

	enum E_KEYBOARD_KEY_CODES
	{
		KEY_ESCAPE = 0x01,
		KEY_TAB = 0x0F,
		KEY_GRAVE = 0x29,
		KEY_CAPSLOCK = 0x3A,
		KEY_BACKSPACE = 0x0E,
		KEY_RETURN = 0x1C,
		KEY_SPACE = 0x39,
		KEY_SLASH = 0x35,
		KEY_BACKSLASH = 0x2B,

		KEY_SYSRQ = 0xB7,
		KEY_SCROLL = 0x46,
		KEY_PAUSE = 0xC5,

		KEY_INSERT = 0xD2,
		KEY_DELETE = 0xD3,
		KEY_HOME = 0xC7,
		KEY_END = 0xCF,
		KEY_PGUP = 0xC9,
		KEY_PGDN = 0xD1,

		KEY_LSHIFT = 0x2A,
		KEY_RSHIFT = 0x36,
		KEY_LALT = 0x38,
		KEY_RALT = 0xB8,
		KEY_LWIN_OR_CMD = 0xDB,
		KEY_RWIN_OR_CMD = 0xDC,
		KEY_LCONTROL = 0x1D,
		KEY_RCONTROL = 0x9D,

		KEY_UP = 0xC8,
		KEY_RIGHT = 0xCD,
		KEY_LEFT = 0xCB,
		KEY_DOWN = 0xD0,

		KEY_1 = 0x02,
		KEY_2 = 0x03,
		KEY_3 = 0x04,
		KEY_4 = 0x05,
		KEY_5 = 0x06,
		KEY_6 = 0x07,
		KEY_7 = 0x08,
		KEY_8 = 0x09,
		KEY_9 = 0x0A,
		KEY_0 = 0x0B,

		KEY_F1 = 0x3B,
		KEY_F2 = 0x3C,
		KEY_F3 = 0x3D,
		KEY_F4 = 0x3E,
		KEY_F5 = 0x3F,
		KEY_F6 = 0x40,
		KEY_F7 = 0x41,
		KEY_F8 = 0x42,
		KEY_F9 = 0x43,
		KEY_F10 = 0x44,
		KEY_F11 = 0x57,
		KEY_F12 = 0x58,

		KEY_Q = 0x10,
		KEY_W = 0x11,
		KEY_E = 0x12,
		KEY_R = 0x13,
		KEY_T = 0x14,
		KEY_Y = 0x15,
		KEY_U = 0x16,
		KEY_I = 0x17,
		KEY_O = 0x18,
		KEY_P = 0x19,
		KEY_A = 0x1E,
		KEY_S = 0x1F,
		KEY_D = 0x20,
		KEY_F = 0x21,
		KEY_G = 0x22,
		KEY_H = 0x23,
		KEY_J = 0x24,
		KEY_K = 0x25,
		KEY_L = 0x26,
		KEY_Z = 0x2C,
		KEY_X = 0x2D,
		KEY_C = 0x2E,
		KEY_V = 0x2F,
		KEY_B = 0x30,
		KEY_N = 0x31,
		KEY_M = 0x32,

		KEY_MINUS = 0x0C,
		KEY_PLUS = 0x0D,
		KEY_LBRACKET = 0x1A,
		KEY_RBRACKET = 0x1B,

		KEY_SEMICOLON = 0x27,
		KEY_APOSTROPHE = 0x28,

		KEY_COMMA = 0x33,
		KEY_PERIOD = 0x34,

		KEY_NUMPAD0 = 0x52,
		KEY_NUMPAD1 = 0x4F,
		KEY_NUMPAD2 = 0x50,
		KEY_NUMPAD3 = 0x51,
		KEY_NUMPAD4 = 0x4B,
		KEY_NUMPAD5 = 0x4C,
		KEY_NUMPAD6 = 0x4D,
		KEY_NUMPAD7 = 0x47,
		KEY_NUMPAD8 = 0x48,
		KEY_NUMPAD9 = 0x49,
		KEY_NUMPADPERIOD = 0x53,
		KEY_NUMPADENTER = 0x9C,
		KEY_NUMPADSTAR = 0x37,
		KEY_NUMPADPLUS = 0x4E,
		KEY_NUMPADMINUS = 0x4A,
		KEY_NUMPADSLASH = 0xB5,
		KEY_NUMLOCK = 0x45,
	};
}

extern bool GetEngine(valk::Engine *&engine);
extern void DestroyEngine();

#endif // _VALK_ENG_H
