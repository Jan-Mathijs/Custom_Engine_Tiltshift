#pragma once
#include <utility>

#define INPUT SIN::Input::GetSingleton()

#ifdef PLATFORM_WIN64
/* Printable keys */
#define SIN_KEY_SPACE              32
#define SIN_KEY_APOSTROPHE         39  /* ' */
#define SIN_KEY_COMMA              44  /* , */
#define SIN_KEY_MINUS              45  /* - */
#define SIN_KEY_PERIOD             46  /* . */
#define SIN_KEY_SLASH              47  /* / */
#define SIN_KEY_0                  48
#define SIN_KEY_1                  49
#define SIN_KEY_2                  50
#define SIN_KEY_3                  51
#define SIN_KEY_4                  52
#define SIN_KEY_5                  53
#define SIN_KEY_6                  54
#define SIN_KEY_7                  55
#define SIN_KEY_8                  56
#define SIN_KEY_9                  57
#define SIN_KEY_SEMICOLON          59  /* ; */
#define SIN_KEY_EQUAL              61  /* = */
#define SIN_KEY_A                  65
#define SIN_KEY_B                  66
#define SIN_KEY_C                  67
#define SIN_KEY_D                  68
#define SIN_KEY_E                  69
#define SIN_KEY_F                  70
#define SIN_KEY_G                  71
#define SIN_KEY_H                  72
#define SIN_KEY_I                  73
#define SIN_KEY_J                  74
#define SIN_KEY_K                  75
#define SIN_KEY_L                  76
#define SIN_KEY_M                  77
#define SIN_KEY_N                  78
#define SIN_KEY_O                  79
#define SIN_KEY_P                  80
#define SIN_KEY_Q                  81
#define SIN_KEY_R                  82
#define SIN_KEY_S                  83
#define SIN_KEY_T                  84
#define SIN_KEY_U                  85
#define SIN_KEY_V                  86
#define SIN_KEY_W                  87
#define SIN_KEY_X                  88
#define SIN_KEY_Y                  89
#define SIN_KEY_Z                  90
#define SIN_KEY_LEFT_BRACKET       91  /* [ */
#define SIN_KEY_BACKSLASH          92  /* \ */
#define SIN_KEY_RIGHT_BRACKET      93  /* ] */
#define SIN_KEY_GRAVE_ACCENT       96  /* ` */
#define SIN_KEY_WORLD_1            161 /* non-US #1 */
#define SIN_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define SIN_KEY_ESCAPE             256
#define SIN_KEY_ENTER              257
#define SIN_KEY_TAB                258
#define SIN_KEY_BACKSPACE          259
#define SIN_KEY_INSERT             260
#define SIN_KEY_DELETE             261
#define SIN_KEY_RIGHT              262
#define SIN_KEY_LEFT               263
#define SIN_KEY_DOWN               264
#define SIN_KEY_UP                 265
#define SIN_KEY_PAGE_UP            266
#define SIN_KEY_PAGE_DOWN          267
#define SIN_KEY_HOME               268
#define SIN_KEY_END                269
#define SIN_KEY_CAPS_LOCK          280
#define SIN_KEY_SCROLL_LOCK        281
#define SIN_KEY_NUM_LOCK           282
#define SIN_KEY_PRINT_SCREEN       283
#define SIN_KEY_PAUSE              284
#define SIN_KEY_F1                 290
#define SIN_KEY_F2                 291
#define SIN_KEY_F3                 292
#define SIN_KEY_F4                 293
#define SIN_KEY_F5                 294
#define SIN_KEY_F6                 295
#define SIN_KEY_F7                 296
#define SIN_KEY_F8                 297
#define SIN_KEY_F9                 298
#define SIN_KEY_F10                299
#define SIN_KEY_F11                300
#define SIN_KEY_F12                301
#define SIN_KEY_F13                302
#define SIN_KEY_F14                303
#define SIN_KEY_F15                304
#define SIN_KEY_F16                305
#define SIN_KEY_F17                306
#define SIN_KEY_F18                307
#define SIN_KEY_F19                308
#define SIN_KEY_F20                309
#define SIN_KEY_F21                310
#define SIN_KEY_F22                311
#define SIN_KEY_F23                312
#define SIN_KEY_F24                313
#define SIN_KEY_F25                314
#define SIN_KEY_KP_0               320
#define SIN_KEY_KP_1               321
#define SIN_KEY_KP_2               322
#define SIN_KEY_KP_3               323
#define SIN_KEY_KP_4               324
#define SIN_KEY_KP_5               325
#define SIN_KEY_KP_6               326
#define SIN_KEY_KP_7               327
#define SIN_KEY_KP_8               328
#define SIN_KEY_KP_9               329
#define SIN_KEY_KP_DECIMAL         330
#define SIN_KEY_KP_DIVIDE          331
#define SIN_KEY_KP_MULTIPLY        332
#define SIN_KEY_KP_SUBTRACT        333
#define SIN_KEY_KP_ADD             334
#define SIN_KEY_KP_ENTER           335
#define SIN_KEY_KP_EQUAL           336
#define SIN_KEY_LEFT_SHIFT         340
#define SIN_KEY_LEFT_CONTROL       341
#define SIN_KEY_LEFT_ALT           342
#define SIN_KEY_LEFT_SUPER         343
#define SIN_KEY_RIGHT_SHIFT        344
#define SIN_KEY_RIGHT_CONTROL      345
#define SIN_KEY_RIGHT_ALT          346
#define SIN_KEY_RIGHT_SUPER        347
#define SIN_KEY_MENU               348

// Mouse
#define SIN_MOUSE_BUTTON_1         0
#define SIN_MOUSE_BUTTON_2         1
#define SIN_MOUSE_BUTTON_3         2
#define SIN_MOUSE_BUTTON_4         3
#define SIN_MOUSE_BUTTON_5         4
#define SIN_MOUSE_BUTTON_6         5
#define SIN_MOUSE_BUTTON_7         6
#define SIN_MOUSE_BUTTON_8         7
#define SIN_MOUSE_BUTTON_LAST      SIN_MOUSE_BUTTON_8
#define SIN_MOUSE_BUTTON_LEFT      SIN_MOUSE_BUTTON_1
#define SIN_MOUSE_BUTTON_RIGHT     SIN_MOUSE_BUTTON_2
#define SIN_MOUSE_BUTTON_MIDDLE    SIN_MOUSE_BUTTON_3

// Joystick
#define SIN_JOYSTICK_1             0
#define SIN_JOYSTICK_2             1
#define SIN_JOYSTICK_3             2
#define SIN_JOYSTICK_4             3
#define SIN_JOYSTICK_5             4
#define SIN_JOYSTICK_6             5
#define SIN_JOYSTICK_7             6
#define SIN_JOYSTICK_8             7
#define SIN_JOYSTICK_9             8
#define SIN_JOYSTICK_10            9
#define SIN_JOYSTICK_11            10
#define SIN_JOYSTICK_12            11
#define SIN_JOYSTICK_13            12
#define SIN_JOYSTICK_14            13
#define SIN_JOYSTICK_15            14
#define SIN_JOYSTICK_16            15
#define SIN_JOYSTICK_LAST          SIN_JOYSTICK_16

// Controller
#define SIN_GAMEPAD_BUTTON_A               0
#define SIN_GAMEPAD_BUTTON_B               1
#define SIN_GAMEPAD_BUTTON_X               2
#define SIN_GAMEPAD_BUTTON_Y               3
#define SIN_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define SIN_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define SIN_GAMEPAD_BUTTON_BACK            6
#define SIN_GAMEPAD_BUTTON_START           7
#define SIN_GAMEPAD_BUTTON_GUIDE           8
#define SIN_GAMEPAD_BUTTON_LEFT_THUMB      9
#define SIN_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define SIN_GAMEPAD_BUTTON_DPAD_UP         11
#define SIN_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define SIN_GAMEPAD_BUTTON_DPAD_DOWN       13
#define SIN_GAMEPAD_BUTTON_DPAD_LEFT       14
#define SIN_GAMEPAD_BUTTON_LAST            SIN_GAMEPAD_BUTTON_DPAD_LEFT

#define SIN_GAMEPAD_BUTTON_CROSS       SIN_GAMEPAD_BUTTON_A
#define SIN_GAMEPAD_BUTTON_CIRCLE      SIN_GAMEPAD_BUTTON_B
#define SIN_GAMEPAD_BUTTON_SQUARE      SIN_GAMEPAD_BUTTON_X
#define SIN_GAMEPAD_BUTTON_TRIANGLE    SIN_GAMEPAD_BUTTON_Y

#define SIN_GAMEPAD_AXIS_LEFT_X        0
#define SIN_GAMEPAD_AXIS_LEFT_Y        1
#define SIN_GAMEPAD_AXIS_RIGHT_X       2
#define SIN_GAMEPAD_AXIS_RIGHT_Y       3
#define SIN_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define SIN_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define SIN_GAMEPAD_AXIS_LAST          SIN_GAMEPAD_AXIS_RIGHT_TRIGGER
#endif

namespace SIN
{	
	class Window;
	class Input
	{
	protected:

		const Window* m_Window;
	
	public:

		void SetWindow(const Window* a_Window);
		static Input& GetSingleton();
		virtual void Initialize() = 0;
		virtual void Clear() = 0;
		
		static bool IsKeyPressed(int a_Keycode)						{ return s_Input->ImplIsKeyPressed(a_Keycode); }
		static bool IsMouseButtonPressed(int a_MouseButton)			{ return s_Input->ImplMouseButtonPressed(a_MouseButton); }
		static std::pair<float, float> GetMousePosition()			{ return s_Input->ImplGetMousePosition(); }
		static float GetScroll()									{ return s_Input->ImplGetScroll(); }
		static bool GetControllerButton(int a_Button)				{ return s_Input->ImplGetControllerButton(a_Button); }
		static float GetTriggerAxis(int a_Trigger)					{ return s_Input->ImplGetTriggerAxis(a_Trigger); }
		static std::pair<float, float> GetStickAxis(int a_Stick)	{ return s_Input->ImplGetStickAxis(a_Stick); }
	
	protected:

		Input();
		virtual ~Input();

		[[nodiscard]] virtual bool ImplIsKeyPressed(int a_Keycode) const = 0;
		[[nodiscard]] virtual bool ImplMouseButtonPressed(int a_MouseButton) const = 0;
		[[nodiscard]] virtual std::pair<float, float> ImplGetMousePosition() const = 0;
		[[nodiscard]] virtual float ImplGetScroll() const = 0;
		[[nodiscard]] virtual bool ImplGetControllerButton(int a_Button) const = 0;
		[[nodiscard]] virtual float ImplGetTriggerAxis(int a_Trigger) const = 0;
		[[nodiscard]] virtual std::pair<float, float> ImplGetStickAxis(int a_Trigger) const = 0;

	private:

		static Input* s_Input;
	};
}



