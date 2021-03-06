#pragma once

#include <stdint.h>

#define BLINK_VECTOR_SIZE 64
#define BLINK_OK 0
#define BLINK_TRUE 1
#define BLINK_FALSE 0

#define BLINK_STD_UUID_CHORD_SCALE "860166f7-e839-448b-bd3b-a5bccbfe3ac1"
#define BLINK_STD_UUID_ENVELOPE_AMP "273e7c30-404b-4db6-ba97-20f33d49fe51"
#define BLINK_STD_UUID_ENVELOPE_FILTER_FREQUENCY "91181212-7072-41a9-9d11-3a265301a9a3"
#define BLINK_STD_UUID_ENVELOPE_FILTER_RESONANCE "4436fc1c-ae51-4580-b1fa-24b9c41425e3"
#define BLINK_STD_UUID_ENVELOPE_FORMANT "7b72dbef-e36d-4dce-958b-b0fa498ae41e"
#define BLINK_STD_UUID_ENVELOPE_MIX "6441d97c-37c9-4670-9049-d22fac68b023"
#define BLINK_STD_UUID_ENVELOPE_NOISE_AMOUNT "29d5ecb5-cb5d-4f19-afd3-835dd805682a"
#define BLINK_STD_UUID_ENVELOPE_NOISE_COLOR "30100123-7343-4386-9ed2-f913b9e1e571"
#define BLINK_STD_UUID_ENVELOPE_PAN "9c312a2c-a1b4-4a8d-ab68-07ea157c4574"
#define BLINK_STD_UUID_ENVELOPE_PITCH "ca2529db-e7bd-4019-9a07-22aee24526d1"
#define BLINK_STD_UUID_ENVELOPE_SPEED "02f68738-f54a-4f35-947b-c30e73896aa4"
#define BLINK_STD_UUID_OPTION_NOISE_MODE "e426cc55-306d-4561-99bc-003bb7707a93"
#define BLINK_STD_UUID_SLIDER_AMP "a6ae4ad0-2965-448c-ab04-ee378e0c4ab5"
#define BLINK_STD_UUID_SLIDER_NOISE_WIDTH "84e18fd3-03f1-49c2-a713-12e7e24dc03f"
#define BLINK_STD_UUID_SLIDER_PAN "b5bf03f3-17e2-4546-8cc2-e29790ea02a2"
#define BLINK_STD_UUID_SLIDER_PITCH "00859eeb-ce9e-43cd-9994-bff881a9d32d"
#define BLINK_STD_UUID_SLIDER_SAMPLE_OFFSET "88373752-7656-4d0e-8da2-a18c05af0106"
#define BLINK_STD_UUID_SLIDER_SPEED "04293c38-3a64-42b2-80f0-43a4f8190ba7"
#define BLINK_STD_UUID_TOGGLE_LOOP "dfa36d24-3c41-4a13-9b57-dc0116ef19f7"
#define BLINK_STD_UUID_TOGGLE_REVERSE "e7cacaf8-4afc-4e81-83de-50620fed4b13"

#define BLINK_STD_CATEGORY_FILTERS "Filters"
#define BLINK_STD_CATEGORY_DESTRUCTION "Destruction"

typedef uint8_t blink_ChannelCount;
typedef uint64_t blink_FrameCount;
typedef uint32_t blink_ParamCount;
typedef uint32_t blink_Index;
typedef int32_t blink_ID;
typedef const char* blink_UUID;
typedef uint32_t blink_SR;
typedef uint8_t blink_BitDepth;
typedef double blink_Position;
typedef int32_t blink_IntPosition;
typedef int8_t blink_Bool;
typedef uint32_t blink_Scale;

enum blink_ChannelMode
{
	blink_ChannelMode_Left = 0,
	blink_ChannelMode_Right = 1,
	blink_ChannelMode_Stereo = 2,
};

enum blink_StdIcon
{
	blink_StdIcon_None = 0,
	blink_StdIcon_Amp = 1,
	blink_StdIcon_Pan = 2,
	blink_StdIcon_Pitch = 3,
	blink_StdIcon_SampleOffset = 4,
	blink_StdIcon_Speed = 5,
	blink_StdIcon_Loop = 6,
	blink_StdIcon_Reverse = 7,
	blink_StdIcon_PianoRoll = 8,
};

// Plugins define their own error codes. Blockhead will probably just ignore
// any errors but this might be useful for future proofing
typedef int blink_Error;

// Some shared error codes. Plugin-specific error codes should be > 0
enum blink_StdError
{
	blink_StdError_None = 0,
	blink_StdError_AlreadyInitialized = -1,
	blink_StdError_NotInitialized = -2,
	blink_StdError_NotImplemented = -3,
};

typedef struct
{
	blink_IntPosition x;
	float y;
} blink_EnvelopePointPosition;

typedef struct
{
	blink_EnvelopePointPosition position;

	// Not currently used.
	float curve;
} blink_EnvelopePoint;

typedef struct
{
	blink_Index count;
	blink_EnvelopePoint* points;
} blink_EnvelopePoints;

typedef struct
{
	float min;
	float max;
} blink_Range;

typedef struct
{
	int min;
	int max;
} blink_IntRange;

enum blink_ParameterType
{
	blink_ParameterType_Chord,
	blink_ParameterType_Envelope,
	blink_ParameterType_Option,
	blink_ParameterType_Slider,
	blink_ParameterType_IntSlider,
	blink_ParameterType_Toggle,
};

typedef struct
{
	blink_IntPosition position;
	blink_Scale scale;
} blink_ChordBlock;

typedef struct
{
	blink_Index count;
	blink_ChordBlock* blocks;
} blink_ChordBlocks;

// Manipulators
//
// This will be a future Blockhead feature, not sure how it is going
// to look. Plugins don't need to worry about it yet.
typedef struct
{
	blink_UUID uuid;
	blink_ID local_id;
	const char* name;
} blink_ManipulatorTarget;

typedef struct
{
	blink_ID local_id;
	float value;
	float glide_in;
	float glide_out;
} blink_ManipulatorPoint;

typedef struct
{
	blink_Index count;
	blink_ManipulatorPoint* points;
} blink_ManipulatorData;

typedef struct
{
	blink_ParameterType type;
	blink_ChordBlocks blocks;
} blink_ChordData;

typedef struct
{
	blink_ParameterType type;
	blink_EnvelopePoints points;
	blink_Index* options;
	float min;
	float max;
} blink_EnvelopeData;

typedef struct
{
	blink_ParameterType type;
	blink_Index index;
} blink_OptionData;

typedef struct
{
	blink_ParameterType type;
	float value;
} blink_SliderData;

typedef struct
{
	blink_ParameterType type;
	int value;
} blink_IntSliderData;

typedef struct
{
	blink_ParameterType type;
	blink_Bool value;
} blink_ToggleData;

union blink_ParameterData
{
	blink_ParameterType type;
	blink_ChordData chord;
	blink_EnvelopeData envelope;
	blink_OptionData option;
	blink_SliderData slider;
	blink_IntSliderData int_slider;
	blink_ToggleData toggle;
};

typedef float (*blink_Curve)(void* proc_data, float value);
typedef float (*blink_InverseCurve)(void* proc_data, float value);
typedef float (*blink_Stepify)(void* proc_data, float value);
typedef float (*blink_SnapValue)(void* proc_data, float value, float step_size, float snap_amount);
typedef float (*blink_Constrain)(void* proc_data, float value);
typedef float (*blink_Drag)(void* proc_data, float start_value, int amount, bool precise);
typedef float (*blink_Increment)(void* proc_data, float value, bool precise);
typedef float (*blink_Decrement)(void* proc_data, float value, bool precise);
typedef const char* (*blink_DisplayValue)(void* proc_data, float value);
typedef bool (*blink_FromString)(void* proc_data, const char* str, float* value);
typedef int (*blink_IntConstrain)(void* proc_data, int value);
typedef int (*blink_IntDrag)(void* proc_data, int start_value, int amount, bool precise);
typedef int (*blink_IntIncrement)(void* proc_data, int value, bool precise);
typedef int (*blink_IntDecrement)(void* proc_data, int value, bool precise);
typedef const char* (*blink_IntDisplayValue)(void* proc_data, int value);
typedef bool (*blink_IntFromString)(void* proc_data, const char* str, int* value);

typedef struct
{
	float default_value;

	void* proc_data;

	blink_DisplayValue display_value;
	blink_FromString from_string;
	blink_Constrain constrain;
	blink_Drag drag;
	blink_Increment increment;
	blink_Decrement decrement;
	blink_Stepify stepify;
} blink_Slider;

typedef struct
{
	int default_value;

	void* proc_data;

	blink_IntDisplayValue display_value;
	blink_IntFromString from_string;
	blink_IntConstrain constrain;
	blink_IntDrag drag;
	blink_IntIncrement increment;
	blink_IntDecrement decrement;
} blink_IntSlider;

typedef struct
{
	// Step size is also a range with a value that
	// can be configured by the user
	blink_Slider step_size;
	float default_snap_amount;
} blink_EnvelopeSnapSettings;

enum blink_ChordFlags
{
	blink_ChordFlags_None                               = 1 << 0,
	blink_ChordFlags_AlwaysShowButtonWhenGroupIsVisible = 1 << 1, // Will not appear in modulator tree
	blink_ChordFlags_DefaultActive                      = 1 << 2,
	blink_ChordFlags_DefaultDisabled                    = 1 << 3,
	blink_ChordFlags_IconOnly                           = 1 << 4, // Only show icon in button
	blink_ChordFlags_MovesDisplay                       = 1 << 5, // Editing should trigger a visual update
};

enum blink_EnvelopeFlags
{
	blink_EnvelopeFlags_None                               = 1 << 0,
	blink_EnvelopeFlags_AlwaysShowButtonWhenGroupIsVisible = 1 << 1, // Will not appear in modulator tree
	blink_EnvelopeFlags_DefaultActive                      = 1 << 2,
	blink_EnvelopeFlags_DefaultDisabled                    = 1 << 3,
	blink_EnvelopeFlags_DefaultAlwaysVisible               = 1 << 4,
	blink_EnvelopeFlags_NoGridLabels                       = 1 << 5,
	blink_EnvelopeFlags_IconOnly                           = 1 << 6, // Only show icon in button
	blink_EnvelopeFlags_MovesDisplay                       = 1 << 7, // Editing should trigger a visual update
};

enum blink_SliderFlags
{
	blink_SliderFlags_None          = 1 << 0,
	blink_SliderFlags_MovesDisplay  = 1 << 1, // Editing should trigger a visual update
	blink_SliderFlags_NonGlobal     = 1 << 2, // Do not create a global control for this slider (can be used
	                                          // to create sliders which are only visible when an envelope is
											  // selected)
};

enum blink_ToggleFlags
{
	blink_ToggleFlags_None              = 1 << 0,
	blink_ToggleFlags_ShowButton        = 1 << 1,
	blink_ToggleFlags_ShowInContextMenu = 1 << 2,
	blink_ToggleFlags_MovesDisplay      = 1 << 3, // Editing should trigger a visual update
};

//
// Option parameter
// Will be displayed in Blockhead as a drop-down menu or radio buttons or something
//
typedef const char* (*blink_Option_GetText)(void* proc_data, blink_Index index);

typedef struct
{
	enum blink_ParameterType parameter_type; // blink_ParameterType_Option
	blink_Index max_index;
	blink_Index default_index;

	void* proc_data;

	// Returns the display text for the option index
	blink_Option_GetText get_text;
} blink_Option;

struct blink_Parameter_;

//
// Envelope parameter
// Can be manipulated in Blockhead using the envelope editor
//
typedef bool (*blink_GetGridLine)(void* proc_data, int index, float* out);
typedef bool (*blink_GetStepLine)(void* proc_data, int index, float step_size, float* out);
typedef float (*blink_EnvelopeSearch)(void* proc_data, const blink_EnvelopeData* data, float block_position);
typedef blink_Index (*blink_GetOption)(void* proc_data, blink_Index index);
typedef blink_Index (*blink_GetSlider)(void* proc_data, blink_Index index);

typedef struct
{
	enum blink_ParameterType parameter_type; // blink_ParameterType_Envelope

	void* proc_data;

	float default_value;
	int flags; // blink_EnvelopeFlags

	int sliders_count;
	int options_count;

	blink_Slider value_slider;
	blink_Slider min;
	blink_Slider max;
	blink_EnvelopeSnapSettings snap_settings;

	blink_DisplayValue display_value;
	blink_FromString from_string;
	blink_EnvelopeSearch search;
	blink_GetGridLine get_gridline;
	blink_GetStepLine get_stepline;
	blink_Stepify stepify;
	blink_SnapValue snap_value;
	blink_GetOption get_option;
	blink_GetSlider get_slider;
} blink_Envelope;

//
// Chord parameter
// Can be manipulated in Blockhead using the chord/scale/harmonics editor thing
//

typedef struct
{
	enum blink_ParameterType parameter_type; // blink_ParameterType_Chord
	blink_StdIcon icon;
	int flags; // blink_ChordFlags
} blink_Chord;

//
// Slider parameter
// Will be displayed in Blockhead as a slider or spinbox control
//

typedef struct
{
	enum blink_ParameterType parameter_type; // blink_ParameterType_Slider
	blink_StdIcon icon;
	int flags; // blink_SliderFlags
	blink_Slider slider;
} blink_SliderParameter;

typedef struct
{
	enum blink_ParameterType parameter_type; // blink_ParameterType_IntSlider
	blink_StdIcon icon;
	int flags; // blink_SliderFlags
	blink_IntSlider slider;
} blink_IntSliderParameter;

//
// Toggle parameter
// On/off value
//

typedef struct
{
	enum blink_ParameterType parameter_type; // blink_ParameterType_Toggle
	blink_Bool default_value;
	blink_StdIcon icon;
	int flags; // blink_ToggleFlags
} blink_Toggle;

//
// Generic Parameter
//
union blink_ParameterObject
{
	enum blink_ParameterType type;
	blink_Chord chord;
	blink_Envelope envelope;
	blink_Option option;
	blink_SliderParameter slider;
	blink_IntSliderParameter int_slider;
	blink_Toggle toggle;
};

typedef struct blink_Parameter_
{
	// Generators can share parameter UUIDs to allow the user to switch back and forth
	// between different generators without losing modulation data (for example the
	// "Amp", "Pan" and "Pitch" envelopes are shared between Classic and Fudge
	// generators.)
	blink_UUID uuid;

	// < 0 if the parameter does not belong to a group
	blink_ID group_index;

	// User-friendly parameter name
	const char* name;

	// Long description of the parameter. Can be null
	const char* long_desc;

	union blink_ParameterObject parameter;
} blink_Parameter;

typedef struct
{
	const char* name;
} blink_Group;

#ifdef BLINK_EXPORT

#ifdef _WIN32
# define EXPORTED  __declspec( dllexport )
#else
# define EXPORTED
#endif

extern "C"
{
	EXPORTED blink_UUID blink_get_plugin_uuid();
	EXPORTED const char* blink_get_plugin_name();
	EXPORTED const char* blink_get_plugin_category();
	EXPORTED const char* blink_get_plugin_version();
	EXPORTED blink_Error blink_init();
	EXPORTED blink_Error blink_terminate();
	EXPORTED blink_Error blink_stream_init(blink_SR SR);
	EXPORTED int blink_get_num_groups();
	EXPORTED int blink_get_num_parameters();
	EXPORTED blink_Group blink_get_group(blink_Index index);
	EXPORTED blink_Parameter blink_get_parameter(blink_Index index);
	EXPORTED blink_Parameter blink_get_parameter_by_uuid(blink_UUID id);

	// Returned buffer remains valid until the next call to get_error_string or
	// until the generator is destroyed
	EXPORTED const char* blink_get_error_string(blink_Error error);
}

#endif
