#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include "RA8875.h"
#include "DisplayDefs.h"

// OK, these key assignments are a bit odd looking. The value are mapped to the character set of the RA8875
// internal font. The selected characters shown various symbols - left arrow for the <bs>, up arrow for <shift>
// and so on. They then have to be mapped to legitimate characters as needed.

#define KYBD_SYM_ENTER  0x1C   ///< This is a symbol in the RA8875 character set that looks most like <enter>, so it used to detect enter
#define KYBD_SYM_ESCAPE 0x15   ///< This is a symbol in the RA8875 character set that is used to show and detect escape
#define KYBD_SYM_BS     0x1B   ///< This is a symbol in the RA8875 character set that is used to show and detect backspace
#define KYBD_SYM_SHIFT  0x18   ///< This is a symbol in the RA8875 character set that is used to show and detect shift
#define KYBD_SYM_TAB    0x1A   ///< This is a symbol in the RA8875 character set that is used to show and detect tab (which translates to <space>


/// A QWERTY keypad, intended to be coupled with the touchscreen, provides
/// on-screen data entry.
///
/// This keypad takes over the bottom of the screen to show an onscreen keyboard.
/// It shows a prompt, and allows basic text entry, including backspace. It does
/// not allow more complex editing.
///
/// Since it takes over a significant chunk of the screen, the parent application
/// must be prepared to redraw the underlying data when the keyboard is no longer
/// in use. Alternately, the parent application may choose to select an
/// alternate display layer, and then activate the keypad.
///
/// keyboard definition structure, used to define a keyboard.
typedef struct
{
	loc_t x;        ///< left edge of keypad. typically 0 to +n. negative is translated to zero.
	loc_t y;        ///< top edge of keypad. typically 0 to +n, and more typically in the bottom half of the screen height.
	                ///  negative number allows keypad to size from the bottom of the screen upward.
	dim_t width;    ///< width of keypad. be sure that the keys can fit. zero is translated to screen-width.
	dim_t height;   ///< height of keypad. be sure the number of rows will fit, based on the desired key height.
	                ///  zero number allows keypad to size from the bottom of the screen upward.
	int rows;       ///< number of rows to show.
	int cols;       ///< number of columns (keys) in the longest row

	const char * keydef1; ///< pointer to the base keyboard data stream
	const char * keydef2; ///< pointer to an alternate keyboard data stream (e.g. after <shift> is activated)

} keyboard_t;


typedef struct
{
	// RA8875 &ra;

	const uint8_t *restore_font; 		// save user font
	const uint8_t *user_font; 			// keyboard restore user font
	int restore_hScale; 			    // save scale
	int restore_vScale;
	int user_font_scale; 				// keyboard font scale
	const keyboard_t *kbd;
	char enter_key;
	char esc_key;
	bool shift;
	char *pStart;
	char *pNext;
	int bufSize;

	point_t userText;
	color_t fore;
	color_t back;

	/// Select a keyboard to use.
	///
	/// This selects the internally defined keyboard (when NULL) or a user defined
	/// keyboard.
	///
	/// @param[in] keyboard is an optional pointer to a @ref keyboard_t data structure,
	///             NULL is the default.
	/// @param[in] enter_key is an optional character used to complete the entry,
	///             KYBD_SYM_ENTER is the default.
	/// @param[in] esc_key is an optional character used to abandon the entry,
	///             KYBD_SYM_ESCAPE is the default.
	/// @returns true if it was accepted.
	/// @returns false if it could not be accepted (no known reason to return false at this time).
	///
 	// bool *SetKeyboard( const keyboard_t * kbd = NULL, char enter_key = KYBD_SYM_ENTER, char esc_key = KYBD_SYM_ESCAPE );
 	// ( bool )( *SetKeyboard )( const keyboard_t *kbd, char enter_key, char esc_key );
    bool ( *SetKeyboard )( const keyboard_t *kbd, char enter_key, char esc_key );

	/// Set Keyboard Font.
	/// This selects the font, or the font size, to be used on the keyboard.
	///
	/// If the font is set, that font will be used and the fontscale will be ignored.\\
	/// If the font is NULL, the display defined font will be used, and if that is not
	/// set the internal font will be used.\\
	/// If the internal font is being used, the fontscale parameter can be set to
	/// choose a scale factor, where values 1, 2, 3, and 4 are permitted.
	///
	/// @param[in] font is an optional pointer to a user font, NULL is the default.
	/// @param[in] fontscale is an optional parameter to scale the built-in font, if
	///             a user specified font was not used.
	/// @returns true if it was accepted.
	/// @returns false if it could not be accepted (no known reason to return false at this time).
	///
	// bool *SetKeyboardFont(const uint8_t * font = NULL, int fontscale = 0);
	bool ( *SetKeyboardFont )( const uint8_t *font, int fontscale );

	/// Get a handle to the keyboard currently in use.
	///
	/// This returns a handle to the currently active keyboard. If you first SelectKeyboard(NULL),
	/// and then call GetKeyboard(), it will return a handle to the built-in keyboard.
	///
	/// This API might be used if you want to use the built-in keyboard definition, but
	/// resize it meet your needs.
	///
	/// @returns a handle to the currently active keyboard.
	///
    const keyboard_t* ( *GetKeyboard )( void );

	/// Get a string of text entered on the onscreen keypad.
	///
	/// text entry is terminated by <enter> or <esc>.
	///
	/// @param[in,out] buffer is the provided buffer into which the string is written.
	/// @param[in] size is the size of the buffer.
	/// @param[in] prompt is the optional text to prompt the user. It defaults
	///             to no prompt.
	/// @param[in] initFromBuffer causes initial text from buffer to seed the data entry.
	///             If this is true and if the strlen(buffer) < size, it will be shown.
	/// @param[in] mask is the optional character, if non-zero, that is used to obscure
	///             the text entry - for password purposes. It defaults to not hidden.
	/// @param[in] autoclose is an optional parameter to erase the keyboard when done.
	///             It defaults to true. Erase is accomplished by filling with the
	///             'back' color that instantiated the Keypad.
	/// @returns true if text was entered.
	/// @returns false if text was not entered (e.g. <esc> pressed).
	///
	/*
	bool *GetString( char * buffer, size_t size,
						 const char * prompt = NULL,
    					 bool initFromBuffer = false,
    					 char mask = 0,
    					 bool autoclose = true );
    					 */
    bool ( *GetString )( char * buffer, unsigned char size,
                            const char * prompt,
                            bool initFromBuffer,
                            char mask,
                            bool autoclose );

	/// Erase the area where the keypad panel was drawn
	///
	/// This API is usually not necessary, since the parent application will generally
	/// redraw the screen after getting the text information. This might be more useful
	/// if the Keypad is placed onto an alternate layer.
	///
	/// @param[in] fillcolor is the color with which to fill the keyboard area.
	///
	// void *Erase( color_t fillcolor = Black );
	void ( *Erase )( color_t );

}_KEYPAD;

/// Instantiator the Keypad object.
///
/// Shows a keypad like this:
/// @code
/// Prompt: [Input text field here.......]
/// `123456...
///  qwertyui...
///   asdfgh...
///    zxcvb....
///           [ space   ]         000/020
/// @endcode
/// The bottom right corner shows how many characters have been entered and how
/// many are permitted.
///
/// @param[in] _ra is a reference to the hosting applications RA8875 display
///            object. This grants Keypad access to the display.
/// @param[in] fore is the foreground color which is for the keys, prompt, text
/// @param[in] back is the background color.
///
/// void Keypad_CreateObj( color_t fore = Blue, color_t back = White );
extern _KEYPAD* Keypad_CreateObj( color_t fore, color_t back );


#endif