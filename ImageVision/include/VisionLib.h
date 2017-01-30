
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the VISIONLIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// VISIONLIB_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#ifdef VISIONLIB_EXPORTS
#define VISIONLIB_API __declspec(dllexport)
#else
#define VISIONLIB_API __declspec(dllimport)
#endif

#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>
using namespace std;

const double  VLPI      = 3.1415926535897932384626433832795028841971;
const double  RADIAN	= 180.0/VLPI;
const double  ARADIAN	= VLPI/180.0;
const int     BD_G8		= 8;
const int     BD_I32	= 32;
//============================================================================
//  Error Codes
//============================================================================
#define ERR_SUCCESS                                0 // No error
#define ERR_UNEXPECTED                             -1074396157 // Unexpected error of VisionLib
#define ERR_SYSTEM_ERROR                           -1074396160 // System error
#define ERR_OUT_OF_MEMORY                          -1074396159 // Not enough memory for requested operation
#define ERR_MEMORY_ERROR                           -1074396158 // Memory error
#define ERR_INVALID_CONTRAST_THRESHOLD             -1074396125 // Invalid contrast threshold. The threshold value must be greater than 0
#define ERR_INVALID_IMAGE_TYPE                     -1074396080 // Invalid image type
#define ERR_INCOMP_TYPE                            -1074396077 // Incompatible image type
#define ERR_INCOMP_SIZE                            -1074396074 // Incompatible image size
#define ERR_IMAGES_NOT_DIFF                        -1074396067 // The source image and destination image must be different
#define ERR_INVALID_BIT_DEPTH                      -1074395721 // VisionLib does not support the bit depth you supplied for the image you supplied
#define ERR_TEMPLATE_EMPTY                         -1074395320 // The template image is empty
#define ERR_FILE_ARGERR                            -1074395997 // Invalid parameter
#define ERR_BAD_INDEX                              -1074395840 // Invalid handle table index
#define ERR_INVALID_SEARCHWND                      -1074396156 // Invalid search window,maybe outside current image border
#define ERR_BAD_ROI                                -1074395720 // Invalid ROI
#define ERR_BAD_ROI_BOX                            -1074395719 // Invalid ROI global rectangle
#define ERR_NULL_POINTER                           -1074395269 // Null pointer
#define ERR_INVALID_HANDLE                         -1074396079 // Invalid handle
#define ERR_INVALID_COLUMN_STEP                    -1074395604 // Invalid column step. Valid range is 1 to 25
#define ERR_INVALID_ROW_STEP                       -1074395603 // Invalid row step. Valid range is 1 to 25
#define ERR_INVALID_INITIAL_SAMPLE_SIZE            -1074396153 // Invalid initial sample size. Valid range is 10 to 120
#define ERR_INVALID_FINAL_SAMPLE_SIZE              -1074396152 // Invalid final sample size. Valid range is 120 to 300
#define ERR_INVALID_INITIAL_ANGULAR_ACCURACY       -1074396151 // Invalid initial angular accuracy. Valid range is 3,6,9
#define ERR_INVALID_FINAL_ANGULAR_ACCURACY         -1074396150 // Invalid final angular accuracy. Valid range is 1
#define ERR_INVALID_SUBPIXEL_SAMPLE_SIZE           -1074396149 // Invalid subpixel sample size. Valid range is 0(Not support yet)
#define ERR_INVALID_MAX_ANGULAR                    -1074396148 // Invalid max angular. Valid range is 0 to 360
#define ERR_INVALID_MIN_ANGULAR                    -1074396147 // Invalid min angular. Valid range is 0 to 359
#define ERR_FILE_PERMISSION                        -1074395992 // File access denied
#define ERR_FILE_NOT_FOUND                         -1074395995 // File not found
#define ERR_FILE_READ                              -1074395989 // Unable to read data
#define ERR_FILE_WRITE                             -1074395988 // Unable to write data
#define ERR_FILE_OPERATION                         -1074395985 // Invalid file operation
#define ERR_INVALID_MIN_MATCH_SCORE                -1074395356 // Invalid minimum match score. Acceptable values range from 0 to 1000
#define ERR_INVALID_LENGTH                         -1074395702 // The length of the edge detection line must be greater than 3
#define ERR_INVALID_EDGE_THRESHOLD                 -1074395601 // Invalid edge threshold. Valid values range from 1 to 360
#define ERR_INVALID_STEEPNESS                      -1074395361 // Invalid steepness
#define ERR_INSF_POINTS                            -1074395318 // You supplied an insufficient number of points to perform this operation
#define ERR_OPERATION_FAILED                       -1074396110 // Operation failed
#define ERR_INVALID_MORPHOLOGYMETHOD               -1074395321 // VisionLib does not support the MorphologyMethod value you supplied
#define ERR_DIV_BY_ZERO                            -1074395270 // Cannot divide by zero
#define ERR_LINEAR_COEFF                           -1074395268 // The linear equations are not independent
#define ERR_INVALID_CURVE_EXTRACTION_MODE          -1074395620 // Invalid curve extraction mode
#define ERR_LINES_PARALLEL                         -1074395276 // You specified parallel lines for the meter ROI
#define ERR_INVALID_POINTSYMBOL                    -1074396066 // Invalid point symbol
#define ERR_POINTS_ARE_COLLINEAR                   -1074395292 // Do not supply collinear points for this operation
#define ERR_NO_DEST_IMAGE                          -1074395773 // You must provide a destination image
#define ERR_NOT_IMAGE                              -1074396120 // Not an image
#define ERR_INCOMP_ANGULARRANGE                    -1074396140 // Incompatible angular range, the maximum angle must greater than minimum angle
#define ERR_OCR_INVALID_SOURCEIMAGE                -1074395563 // Not a valid source image to built character set against
#define ERR_OCR_INVALID_ARG                        -1074396116 // One or more arguments are invalid
#define ERR_OCR_INVALID_NUMBER_OF_CHARACTERS       -1074395561 // The number of characters in the character value must match the number of objects in the image
#define ERR_OCR_CHARACTER_CANNOT_BE_EMPTYSTRING    -1074395577 // The character value must not be an empty string
#define ERR_OUT_OF_RANGE                           -1074395717 // Moved out of range
#define ERR_OPERATION_NOT_SUPPORT                  -1074396107 // Operation not supported
#define ERR_INCOMP_TEMPLATE_SIZE                   -1074395757 // Incompatible template size
#define ERR_BAD_IMAGE                              -1074395166 // Image data corrupted
#define ERR_INVALID_IMAGE_SIZE                     -1074396137 // Invalid image size
#define ERR_INVALID_BORDER_SIZE                    -1074395330 // Invalid border size. Acceptable values range from 0 to 50
#define ERR_INVALID_SCALE                          -1074395768 // Scale must be greater than zero
#define ERR_INVALID_SCALINGMODE                    -1074395327 // VisionLib does not support the ScalingMode value you supplied
//============================================================================
// Interface Forward Declaration
//============================================================================
class  CPyraTmp;
struct IImage;

//============================================================================
//  Typedefed Types
//============================================================================
typedef int THRESHOLD;
typedef int INDEX;
typedef CPyraTmp PyraTmp;
typedef pair<double, double> SinCosPair;

#ifdef UNICODE
#define tstring wstring
#else
#define tstring	string
#endif

//============================================================================
//  Enumerated Types
//============================================================================
// Defines the type of an image
enum ImageType {
	VL_IMAGE_U8      = 0, // The image type is 8-bit unsigned integer grayscale
	VL_IMAGE_I16     = 1, // The image type is 16-bit signed integer grayscale
	VL_IMAGE_SGL     = 2, // The image type is 32-bit floating-point grayscale
	VL_IMAGE_COMPLEX = 3, // The image type is complex
	VL_IMAGE_RGB     = 4, // The image type is RGB color
	VL_IMAGE_HSL     = 5, // The image type is HSL color
	VL_IMAGE_RGB_U64 = 6, // The image type is 64-bit unsigned RGB color
	VL_IMAGE_TYPE_SIZE_GUARD = 0xFFFFFFFF // Reserved
};

// The direction the function follows to search for edges along the search lines
enum RakeDirection {
	VL_LEFT_TO_RIGHT = 0,			// The function searches from the left side of the search area to the right side of the search area
	VL_RIGHT_TO_LEFT = 1,			// The function searches from the right side of the search area to the left side of the search area
	VL_TOP_TO_BOTTOM = 2,			// The function searches from the top side of the search area to the bottom side of the search area
	VL_BOTTOM_TO_TOP = 3,			// The function searches from the bottom side of the search area to the top side of the search area
	VL_RAKE_DIRECTION_ROTATED = 4,	// The function searches from the specified direction in the rotated rectangle
	VL_RAKE_DIRECTION_SIZE_GUARD = 0xFFFFFFFF	// Reserved
};

// The weighting function used for geometric elements fitting
enum WeightFunction {
	VL_NONE_WEIGHT		= 0,	// Don't use weight in fitting process
	VL_HUBER_WEIGHT		= 1,	// Weight function was proposed by Huber
	VL_TUKEY_WEIGHT		= 2,	// Weight function was proposed by Tukey
	VL_RANSAC_WEIGHT	= 3,	// Random sample consensus paradigm: A paradigm for model fitting
	VL_WEIGHT_FUNCTION_SIZE_GUARD = 0xFFFFFFFF	// Reserved
};

// The direction the function follows to search for edges along the search lines
enum SpokeDirection {
	VL_OUTSIDE_TO_INSIDE = 0,	// The function searches from the outside of the search area to the inside of the search area
	VL_INSIDE_TO_OUTSIDE = 1,	// The function searches from the inside of the search area to the outside of the search area
	VL_SPOKE_DIRECTION_SIZE_GUARD = 0xFFFFFFFF	// Reserved
};

// Specifies the method to use when looking for the pattern in the image
enum MatchingMode {
	VL_MATCH_SHIFT_INVARIANT	= 1,	// Searches for occurrences of the template image anywhere in the searchRect, assuming that the pattern is not rotated more than plus or minus 4 degrees
	VL_MATCH_ROTATION_INVARIANT = 2,	// Searches for occurrences of the pattern in the image with no restriction on the rotation of the pattern
	VL_MATCHING_MODE_SIZE_GUARD = 0xFFFFFFFF	// Reserved
};

// Defines the method a function uses for binary thresholding
enum ThresholdMethod {
	VL_THRESH_CLUSTERING	= 0,	// The function uses a method that sorts the histogram of the image within a discrete number of classes corresponding to the number of phases perceived in an image
	VL_THRESH_ENTROPY		= 1,	// The function uses a method that is best for detecting particles that are present in minuscule proportions on the image
	VL_THRESH_METRIC		= 2,	// The function uses a method that is well-suited for images in which classes are not too disproportionate
	VL_THRESH_MOMENTS		= 3,	// The function uses a method that is suited for images that have poor contrast
	VL_THRESH_INTERCLASS	= 4,	// The function uses a method that is well-suited for images in which classes have well separated pixel value distributions
	VL_THRESHOLD_METHOD_SIZE_GUARD = 0xFFFFFFFF	// Reserved
};

// The morphological transformation the function applies
enum MorphologyMethod {
	VL_AUTOM		= 0,	// The function uses a transformation that generates simpler particles that contain fewer details
	VL_CLOSE		= 1,	// The function uses a transformation that fills tiny holes and smooths boundaries
	VL_DILATE		= 2,	// The function uses a transformation that eliminates tiny holes isolated in particles and expands the contour of the particles according to the template defined by the structuring element
	VL_ERODE		= 3,	// The function uses a transformation that eliminates pixels isolated in the background and erodes the contour of particles according to the template defined by the structuring element
	VL_GRADIENT		= 4,	// The function uses a transformation that leaves only the pixels that would be added by the dilation process or eliminated by the erosion process
	VL_GRADIENTOUT	= 5,	// The function uses a transformation that leaves only the pixels that would be added by the dilation process
	VL_GRADIENTIN	= 6,	// The function uses a transformation that leaves only the pixels that would be eliminated by the erosion process
	VL_HITMISS		= 7,	// The function uses a transformation that extracts each pixel located in a neighborhood exactly matching the template defined by the structuring element
	VL_OPEN			= 8,	// The function uses a transformation that removes small particles and smooths boundaries
	VL_PCLOSE		= 9,	// The function uses a transformation that fills tiny holes and smooths the inner contour of particles according to the template defined by the structuring element
	VL_POPEN		= 10,	// The function uses a transformation that removes small particles and smooths the contour of particles according to the template defined by the structuring element
	VL_THICK		= 11,	// The function uses a transformation that adds to an image those pixels located in a neighborhood that matches a template specified by the structuring element
	VL_THIN			= 12,	// The function uses a transformation that eliminates pixels that are located in a neighborhood matching a template specified by the structuring element
	VL_MORPHOLOGY_METHOD_SIZE_GUARD = 0xFFFFFFFF // Reserved
};

// Specifies the method the function uses to identify the locations of the curves in the image
enum ExtractionMode {
	VL_NORMAL_IMAGE					= 0,	// Specifies that the function makes no assumptions about the uniformity of objects in the image or the image background
	VL_UNIFORM_REGIONS				= 1,	// Specifies that the function assumes that either the objects in the image or the image background consists of uniform pixel values
	VL_EXTRACTION_MODE_GRADIENTIN	= 2,	// Specifies that the function use morphological gradient in method
	VL_EXTRACTION_MODE_GRADIENTOUT	= 3,	// Specifies that the function use morphological gradient out method
	VL_EXTRACTION_MODE_SIZE_GUARD = 0xFFFFFFFF	// Reserved
};

// Specifies the method geometric match function uses when looking for the pattern in the image
enum GeometricMatchingMode {
	VL_GEOMETRIC_MATCH_SHIFT_INVARIANT = 0,            // Searches for occurrences of the pattern in the image, assuming that the pattern is not rotated more than plus or minus 5 degrees
	VL_GEOMETRIC_MATCH_ROTATION_INVARIANT = 1,         // Searches for occurrences of the pattern in the image with reduced restriction on the rotation of the pattern
	VL_GEOMETRIC_MATCH_SCALE_INVARIANT = 2,            // Searches for occurrences of the pattern in the image with reduced restriction on the size of the pattern
	VL_GEOMETRIC_MATCH_OCCLUSION_INVARIANT = 3,        // Searches for occurrences of the pattern in the image, allowing for a specified percentage of the pattern to be occluded
	VL_GEOMETRIC_MATCHING_MODE_SIZE_GUARD = 0xFFFFFFFF // Reserved
};

// Indicates the type of feature to follow
enum FeatureType {
    VL_NOT_FOUND_FEATURE                   = 0,   // Specifies the feature is not found
    VL_CIRCLE_FEATURE                      = 1,   // Specifies the feature is a circle
    VL_ELLIPSE_FEATURE                     = 2,   // Specifies the feature is an ellipse
    VL_CONST_CURVE_FEATURE                 = 3,   // Specifies the features is a constant curve
    VL_RECTANGLE_FEATURE                   = 4,   // Specifies the feature is a rectangle
    VL_LEG_FEATURE                         = 5,   // Specifies the feature is a leg
    VL_CORNER_FEATURE                      = 6,   // Specifies the feature is a corner
    VL_PARALLEL_LINE_PAIR_FEATURE          = 7,   // Specifies the feature is a parallel line pair
    VL_PAIR_OF_PARALLEL_LINE_PAIRS_FEATURE = 8,   // Specifies the feature is a pair of parallel line pairs
    VL_LINE_FEATURE                        = 9,   // Specifies the feature is a line
    VL_CLOSED_CURVE_FEATURE                = 10   // Specifies the feature is a closed curve
};

// Indicates the type of geometric primitives
enum PrimitiveType {
	VL_LINE_PRIMITIVE			= 0,	// Specifies the geometric primitive is a line
	VL_CIRCLE_PRIMITIVE			= 1,	// Specifies the geometric primitive is a circle
	VL_ELLIPSE_PRIMITIVE		= 2,	// Specifies the geometric primitive is a ellipse
	VL_CIRCLE_ARC_PRIMITIVE		= 3,	// Specifies the geometric primitive is a circle arc
	VL_ELLIPSE_ARC_PRIMITIVE	= 4,	// Specifies the geometric primitive is a ellipse arc
	VL_FREE_LINE_PRIMITIVE		= 5,	// Specifies the geometric primitive is a free line
	VL_GEO_PRIMITIVE_TYPE_SIZE_GUARD = 0xFFFFFFFF // Reserved
};

// Defines the edges for which the function looks
enum EdgeProcess {
    VL_FIRST                   = 0,           // The function looks for the first edge
    VL_FIRST_AND_LAST          = 1,           // The function looks for the first and last edge
    VL_ALL                     = 2,           // The function looks for all edges
    VL_EDGE_PROCESS_SIZE_GUARD = 0xFFFFFFFF	// Reserved
};

// Defines the interpolation method used by a function
enum InterpolationMethod {
    VL_ZERO_ORDER                      = 0,           // The function uses an interpolation method that interpolates new pixel values using the nearest valid neighboring pixel
    VL_BILINEAR                        = 1,           // The function uses an interpolation method that interpolates new pixel values using a bidirectional average of the neighboring pixels
    VL_QUADRATIC                       = 2,           // The function uses an interpolation method that interpolates new pixel values using a quadratic approximating polynomial
    VL_CUBIC_SPLINE                    = 3,           // The function uses an interpolation method that interpolates new pixel values by fitting them to a cubic spline curve, where the curve is based on known pixel values from the image
    VL_INTERPOLATION_METHOD_SIZE_GUARD = 0xFFFFFFFF	// Reserved
};

// The polarity of an edge
enum PolarityType {
    VL_EDGE_RISING              = 1,	// The edge is a rising edge
    VL_EDGE_FALLING             = -1,	// The edge is a falling edge
    VL_POLARITY_TYPE_SIZE_GUARD = 0xFFFFFFFF	// Reserved
};

// Gaussian kernels
enum GaussKernel {
	VL_GAUSS_3M3_1             = 0,             // Gaussian 3X3, [1 2 1]
	VL_GAUSS_3M3_2             = 1,             // Gaussian 3X3, [1 4 1]
	VL_GAUSS_5M5_1             = 10,            // Gaussian 5X5, [1 2 4 2 1]
	VL_GAUSS_5M5_2             = 11,            // Gaussian 5X5, [1 4 6 4 1]
	VL_GAUSS_7M7_1             = 20,            // Gaussian 7X7, [1 6 15 20 15 6 1]
	VL_GAUSS_KERNEL_SIZE_GUARD = 0xFFFFFFFF	// Reserved
};

// The method by which to calculate the threshold that Train and ReadText use to analyze an image
enum ThresholdMode {
    VL_FIXED_RANGE        = 0,  // Performs thresholding using the values you provide in the lowThreshold and highThreshold elements of OCRProcessingOptions
    VL_COMPUTED_UNIFORM   = 1,  // Calculates a single threshold value for the entire ROI
    VL_COMPUTED_LINEAR    = 2,  // Calculates a value on the left side of the ROI, calculates a value on the right side of the ROI, and linearly fills the middle values from left to right
    VL_COMPUTED_NONLINEAR = 3   // Divides the ROI into the number of blocks specified by the blockCount element of OCRProcessingOptions and calculates a threshold value for each block
};

// OCV learning indexes
enum OCVIndexes {
	VL_OCVINDEX_AREA           = 0x0001, // Pattern area
	VL_OCVINDEX_ASPECTRATIO    = 0x0002, // Pattern aspect ratio
	VL_OCVINDEX_BRIGHTNESS     = 0x0004, // Pattern brightness
	VL_OCVINDEX_CONTRAST       = 0x0008, // Pattern contrast
	VL_OCVINDEX_ABSPOSITION    = 0x0010, // Pattern absolute position
	VL_OCVINDEX_RELPOSITION    = 0x0020, // Pattern relative position
	VL_OCVINDEX_NORMALIZEDCORR = 0x0040, // Normalized correlation
	VL_OCVINDEX_ALL            = VL_OCVINDEX_AREA | VL_OCVINDEX_ASPECTRATIO | VL_OCVINDEX_BRIGHTNESS | VL_OCVINDEX_CONTRAST | VL_OCVINDEX_ABSPOSITION | VL_OCVINDEX_RELPOSITION,
	VL_OCV_INDEXES_SIZE_GUARD  = 0xFFFFFFFF	// Reserved
};

// OCV verifying results
enum OCVDiagnostics {
	VL_OCVDIAGNOSTIC_OK            = 0x0001, // Pattern verified successfully
	VL_OCVDIAGNOSTIC_NOTFOUND      = 0x0002, // Pattern not found
	VL_OCVDIAGNOSTIC_MISMATCH      = 0x0004, // Wrong pattern
	VL_OCVDIAGNOSTIC_MISSHAPE      = 0x0008, // Pattern shape out of tolerance
	VL_OCVDIAGNOSTIC_OVERPRINTING  = 0x0010, // Too much inking on a pattern
	VL_OCVDIAGNOSTIC_UNDERPRINTING = 0x0020, // Too little inking on a pattern
	VL_OCVDIAGNOSTIC_BADCONTRAST   = 0x0040, // Pattern global contrast out of range
	VL_OCV_DIAGNOSTICS_SIZE_GUARD  = 0xFFFFFFFF	// Reserved
};

// The similarity measure between two feature
enum SimilarityMeasure {
	VL_SIMILARITY_MEASURE_SAD          = 0, // Sum of absolute differences
	VL_SIMILARITY_MEASURE_SSD          = 1, // Sum of squared differences
	VL_SIMILARITY_MEASURE_NCC          = 2, // Normalized crosscorrelation
	VL_SIMILARITY_MEASURE_SIZE_GUARD  = 0xFFFFFFFF // Reserved
};

// Specifies the invariance mode the function uses when learning the pattern
enum LearningMode {
	VL_LEARN_ALL                  = 0, // The function extracts information for shift- and rotation-invariant matching
	VL_LEARN_SHIFT_INFORMATION    = 1, // The function extracts information for shift-invariant matching
	VL_LEARN_ROTATION_INFORMATION = 2, // The function extracts information for rotation-invariant matching
	VL_LEARNING_MODE_SIZE_GUARD   = 0xFFFFFFFF // Reserved 
};

// Specifies how the features of the image are used during the search phase
enum SearchStrategy {
	VL_CONSERVATIVE               = 0, // Instructs the pattern matching algorithm to use the largest possible amount of information from the image at the expense of slowing down the speed of the algorithm
	VL_BALANCED                   = 1, // Instructs the pattern matching algorithm to balance the amount of information from the image it uses with the speed of the algorithm
	VL_AGGRESSIVE                 = 2, // Instructs the pattern matching algorithm to use a lower amount of information from the image, which allows the algorithm to run quickly but at the expense of accuracy
	VL_VERY_AGGRESSIVE			  = 3, // Instructs the pattern matching algorithm to use the smallest possible amount of information from the image, which allows the algorithm to run at the highest speed possible but at the expense of accuracy
	VL_SEARCH_STRATEGY_SIZE_GUARD = 0xFFFFFFFF // Reserved
};

// Template structure form
enum StructureForm {
	VL_STRUCTURE_FORM_SINGLE     = 0, // The created template component only hold single template
	VL_STRUCTURE_FORM_MULTIPLE   = 1, // The created template component can hold multiple templates
	VL_STRUCTURE_FORM_SIZE_GUARD = 0xFFFFFFFF // Reserved  
};

// The method by which a function modifies the border
enum BorderMethod {
	VL_BORDER_MIRROR            = 0, // Symmetrically copies pixel values from the image into the border
	VL_BORDER_COPY              = 1, // Copies the value of the pixel closest to the edge of the image into the border
	VL_BORDER_CLEAR             = 2, // Sets all pixels in the border to 0
	VL_BORDER_METHOD_SIZE_GUARD = 0xFFFFFFFF // Reserved
};

// The axis over which to flip an image
enum FlipAxis {
	VL_HORIZONTAL_AXIS      = 0, // Flips the image over the central horizontal axis
	VL_VERTICAL_AXIS        = 1, // Flips the image over the central vertical axis
	VL_CENTER_AXIS          = 2, // Flips the image over both the central vertical and horizontal axes
	VL_DIAG_L_TO_R_AXIS     = 3, // Flips the image over an axis from the upper left corner to lower right corner
	VL_DIAG_R_TO_L_AXIS     = 4, // Flips the image over an axis from the upper right corner to lower left corner
	VL_FLIP_AXIS_SIZE_GUARD = 0xFFFFFFFF // Reserved 
};

// The scaling mode for the function
enum ScalingMode {
	VL_SCALE_LARGER       = 0, // The function duplicates pixels to make the image larger
	VL_SCALE_SMALLER      = 1, // The function subsamples pixels to make the image smaller
	VL_SCALING_MODE_SIZE_GUARD = 0xFFFFFFFF // Reserved 
};

// Specifies the orientation of a resulting rectangular image relative to an annulus
enum RectOrientation {
	VL_BASE_INSIDE  = 0, // Specifies that the base of the rectangular image lies along the inside edge of the annulus
	VL_BASE_OUTSIDE = 1, // Specifies that the base of the rectangular image lies along the outside edge of the annulus
	VL_TEXT_ORIENTATION_SIZE_GUARD = 0xFFFFFFFF // Reserved 
};

// The unit of measure for the image
enum CalibrationUnit {
	VL_UNDEFINED   = 0,  // The image does not have a defined unit of measurement
	VL_ANGSTROM    = 1,  // The unit of measure for the image is angstroms
	VL_MICROMETER  = 2,  // The unit of measure for the image is micrometers
	VL_MILLIMETER  = 3,  // The unit of measure for the image is millimeters
	VL_CENTIMETER  = 4,  // The unit of measure for the image is centimeters
	VL_METER       = 5,  // The unit of measure for the image is meters
	VL_KILOMETER   = 6,  // The unit of measure for the image is kilometers
	VL_MICROINCH   = 7,  // The unit of measure for the image is microinches
	VL_INCH        = 8,  // The unit of measure for the image is inches
	VL_FOOT        = 9,  // The unit of measure for the image is feet
	VL_NAUTICMILE  = 10, // The unit of measure for the image is nautical miles
	VL_GROUNDMILE  = 11, // The unit of measure for the image is ground miles
	VL_STEP        = 12, // The unit of measure for the image is steps
	VL_CALIBRATION_UNIT_SIZE_GUARD = 0xFFFFFFFF // Reserved 
};

// Image File Format Constants 
enum ImageFormat {
	VL_BMP			= 0,	// Indicates the Microsoft Windows Bitmap (BMP) format
	VL_EMF			= 1,	// Indicates the Enhanced Metafile (EMF) format
	VL_EXIF			= 2,	// Indicates the Exif (Exchangeable Image File) format
	VL_GIF			= 3,	// Indicates the Graphics Interchange Format (GIF) format
	VL_JPEG			= 4,	// Indicates the JPEG format
	VL_PNG			= 5,	// Indicates the Portable Network Graphics (PNG) format
	VL_TIFF			= 6,	// Indicates the Tagged Image File Format (TIFF) format
	VL_WMF			= 7,	// Indicates the Windows Metafile Format (WMF) format
	VL_UNDETERMINED	= 8,	// Indicates that Windows GDI+ is unable to determine the format
	VL_IMAGE_FORMAT_SIZE_GUARD = 0xFFFFFFFF // Reserved 
};

// Palette type to use when displaying a monochrome image in an image window
enum PaletteType {
	VL_PALETTE_GRAY			= 0,	//	The function uses a palette that has a gradual gray-level variation from black to white
	VL_PALETTE_BINARY		= 1,	//	The function uses a palette of 16 cycles of 16 different colors that is useful with binary images
	VL_PALETTE_GRADIENT		= 2,	//	The function uses a palette that has a gradation from red to white with a prominent range of light blue in the upper value range
	VL_PALETTE_RAINBOW		= 3,	//	The function uses a palette that has a gradation from blue to red with a prominent range of greens in the middle value range
	VL_PALETTE_TEMPERATURE	= 4,	//	The function uses a palette that has a gradation from light brown to dark brown	
	VL_PALETTE_USER			= 5,	//	The function uses a palette defined by the user
	VL_PALETTE_TYPE_SIZE_GUARD = 0xFFFFFFFF // Reserved
};

//============================================================================
//  Forward Declare Data Structures
//============================================================================
typedef struct _Annulus                 Annulus;
typedef struct _BestCircle              BestCircle;
typedef struct _BestLine                BestLine;
typedef struct _CircleDescriptor        CircleDescriptor;
typedef struct _CircleMatch             CircleMatch;
typedef struct _CircularEdgeReport      CircularEdgeReport;
typedef struct _CurveOptions            CurveOptions;
typedef struct _EdgePoint               EdgePoint;
typedef struct _FindEdgeOptions         FindEdgeOptions;
typedef struct _FitCircleOptions        FitCircleOptions;
typedef struct _FitLineOptions          FitLineOptions;
typedef struct _HaltonPoint             HaltonPoint;
typedef struct _HaltonSamples           HaltonSamples;
typedef struct _Image                   Image;
typedef struct _LearnPatternOptions     LearnPatternOptions;
typedef struct _LineEquation            LineEquation;
typedef struct _MatchPatternOptions     MatchPatternOptions;
typedef struct _PatternMatch            PatternMatch;
typedef struct _RangeFloat              RangeFloat;
typedef struct _RotationAngleRange      RotationAngleRange;
typedef struct _ShapeDetectionOptions   ShapeDetectionOptions;
typedef struct _StraightEdgeReport      StraightEdgeReport;
typedef struct _StructuringElement      StructuringElement;

//============================================================================
// Data Structures
//============================================================================
// Image structure.
typedef struct _Image {
	PBYTE pBits;		//	Image first pixel address
	int   nWidth;		//	Image width in pixels
	int   nHeight;		//	Image height in pixels
	int   nRowLength;	//	Image row length in number of bytes
	int   nBitDepth;	//	Image bit depth, valid value is 8 or BD_G8
}Image,*PImage;

// Describes how to calculate the best fit line.
typedef struct _FitLineOptions {
    WeightFunction  weight;			// Specifies the weight function used in line fitting process
	float           minScore;		// Specifies the required quality of the fitted line
	float           pixelRadius;	// Specifies the neighborhood pixel relationship for the initial subset of points being used
	int             numRefinements;	// Specifies the number of refinement iterations you allow the function to perform on the initial subset of points
	BOOL            bHorizontal;	// Specifies the expected line orientation
}FitLineOptions,*PFitLineOptions;

// Describes how to calculate the best fit circle.
typedef struct _FitCircleOptions {
	WeightFunction  weight;			// Specifies the weight function used in circle fitting process
	BOOL            rejectOutliers;	// Whether to use every given point or only a subset of the points to fit the circle
	float           pixelRadius;	// The acceptable distance, in pixels, that a point determined to belong to the circle can be from the circumference of the circle
	float           minScore;		// Specifies the required quality of the fitted circle
	int             maxIterations;	// Specifies the number of refinement iterations you allow the function to perform on the initial subset of points
}FitCircleOptions,*PFitCircleOptions;

// Describes how you want the function to search for edges and the information the function overlays to the image.
typedef struct _FindEdgeOptions {
	int  threshold;			// Specifies the threshold value for the contrast of the edge
	int  width;				// The number of pixels that the function averages to find the contrast at either side of the edge
	int  steepness;			// The span, in pixels, of the slope of the edge projected along the path specified by the input points
	int  subsamplingRatio;	// Specifies the number of pixels that separate two consecutive search lines
	BOOL showSearchArea;	// If TRUE, the function overlays the search area on the image
	BOOL showSearchLines;	// If TRUE, the function overlays the search lines used to locate the edges on the image
	BOOL showEdgesFound;	// If TRUE, the function overlays the locations of the edges found on the image
	BOOL showResult;		// If TRUE, the function overlays the hit lines to the object and the edge used to generate the hit line on the result image
}FindEdgeOptions,*PFindEdgeOptions;

// Edge point structure.
typedef struct _EdgePoint {
	POINTFLOAT point;		// Edge point position
	float      distance;	// The distance between edge point and expected line, circle etc
	float      weight;		// Weight value
} EdgePoint,*PEdgePoint;

// Information describing a calculated straight edge
typedef struct _StraightEdgeReport {
	POINTFLOAT  start;			// The coordinates location of the start of the calculated edge
	POINTFLOAT  end;			// The coordinates location of the end of the calculated edge
	float       meanDistance;	// The mean deviation distance from edge points to straight line
	float       alpha;			// The pitch angle
	float       straightness;	// The straightness of the calculated edge, which is equal to the least-square error of the fitted line to the entire set of coordinates. If the function does not detect any edges or the edges do not fit to a valid line, it sets straightness to 0
	POINTFLOAT *coordinates;	// An array of detected edge coordinates the function used to calculate the location of the straight edge
	int         numCoordinates;	// The number of detected edge coordinates
}StraightEdgeReport,*PStraightEdgeReport;

// Information describing a calculated circular edge
typedef struct _CircularEdgeReport {
	POINTFLOAT  center;			// The center of the circle that best fits the circular edge
	float       radius;			// The radius of the circle that best fits the circular edge
	float       roundness;		// The roundness of the calculated circular edge
	float       meanDistance;	// The mean deviation distance from edge points to circle
	POINTFLOAT *coordinates;	// An array of points indicating the location of the detected edge
	int         numCoordinates; // The number of detected edge coordinates
}CircularEdgeReport,*PCircularEdgeReport;

// Defines the location and size of an annulus
typedef struct _Annulus{
	POINT  center;		// The coordinate location of the center of the annulus
	int    innerRadius;	// The internal radius of the annulus
	int    outerRadius;	// The external radius of the annulus
	double startAngle;	// The start angle, in degrees, of the annulus
	double endAngle;	// The end angle, in degrees, of the annulus
}Annulus,*PAnnulus;

// Describes how the algorithm learns the pattern
typedef struct _LearnPatternOptions {
	BOOL bLearnForRotation;			// The function extracts information for rotation-invariant matching
	int  initialStepSize;			// The largest number of image pixels to shift the sample across the inspection image during the initial phase of shift-invariant matching
	int  initialSampleSize;			// Specifies the number of template pixels that you want to include in a sample for the initial phase of shift-invariant matching
	int  finalSampleSize;			// Specifies the number of template pixels you want to add to initialSampleSize for the final phase of shift-invariant matching
	int  initialAngularAccuracy;	// Sets the angle accuracy, in degrees, to use during the initial phase of rotation-invariant matching
    int  finalAngularAccuracy;		// Sets the angle accuracy, in degrees, to use during the final phase of the rotation-invariant matching
	int  subpixelSampleSize;		// Specifies the number of template pixels that you want to include in a sample for the subpixel phase of rotation-invariant matching
	int  maxAngular;				// The highest amount of rotation, in degrees, a valid pattern can have
	int  minAngular;				// The lowest amount of rotation, in degrees, a valid pattern can have
}LearnPatternOptions,*PLearnPatternOptions;

// Describes how the algorithm learns during shift-invariant matching
typedef struct _LearnPatternAdvancedShiftOptions {
	int   initialStepSize;          // The largest number of image pixels to shift the sample across the inspection image during the initial phase of shift-invariant matching
	int   initialSampleSize;		// Specifies the number of template pixels that you want to include in a sample for the initial phase of shift-invariant matching
	float initialSampleSizeFactor;  // Specifies the size of the sample for the initial phase of shift-invariant matching as a percent of the template size, in pixels
	int   finalSampleSize;			// Specifies the number of template pixels you want to add to initialSampleSize for the final phase of shift-invariant matching
	float finalSampleSizeFactor;	// Specifies the size of the sample for the final phase of shift-invariant matching as a percent of the edge points in the template, in pixels
	int   subpixelSampleSize;		// Specifies the number of template pixels that you want to include in a sample for the subpixel phase of shift-invariant matching
	float subpixelSampleSizeFactor;	// Specifies the size of the sample for the subpixel phase of shift-invariant matching as a percent of the template size, in pixels
} LearnPatternAdvancedShiftOptions, *PLearnPatternAdvancedShiftOptions;

// Describes how the algorithm learns during rotation-invariant matching
typedef struct _LearnPatternAdvancedRotationOptions {
	SearchStrategy searchStrategySupport;	// Specifies the aggressiveness of the rotation search strategy available during the matching phase
	int   initialStepSize;					// The largest number of image pixels to shift the sample across the inspection image during the initial phase of matching
	int   initialSampleSize;				// Specifies the number of template pixels that you want to include in a sample for the initial phase of rotation-invariant matching
	float initialSampleSizeFactor;			// Specifies the size of the sample for the initial phase of rotation-invariant matching as a percent of the template size, in pixels
	int   initialAngularAccuracy;			// Sets the angle accuracy, in degrees, to use during the initial phase of rotation-invariant matching
	int   finalSampleSize;					// Specifies the number of template pixels you want to add to initialSampleSize for the final phase of rotation-invariant matching
	float finalSampleSizeFactor;			// Specifies the size of the sample for the final phase of rotation-invariant matching as a percent of the edge points in the template, in pixels
	int   finalAngularAccuracy;				// Sets the angle accuracy, in degrees, to use during the final phase of the rotation-invariant matching
	int   subpixelSampleSize;				// Specifies the number of template pixels that you want to include in a sample for the subpixel phase of rotation-invariant matching
	float subpixelSampleSizeFactor;			// Specifies the size of the sample for the subpixel phase of rotation-invariant matching as a percent of the template size, in pixels
} LearnPatternAdvancedRotationOptions, *PLearnPatternAdvancedRotationOptions;

// Describes how the algorithm learns the pattern
typedef struct _LearnPatternAdvancedOptions {
	SimilarityMeasure					 measure;			// Similarity measure used by matching phase
	LearningMode						 learningMode;		// The mode in which the Learn method of interface ITemplate learns the template image
	LearnPatternAdvancedShiftOptions    *shiftOptions;		// Use this element to control the behavior of Learn method of interface ITemplate during the shift-invariant learning phase
	LearnPatternAdvancedRotationOptions *rotationOptions;	// Use this element to control the behavior of Learn method of interface ITemplate during the rotation-invariant learning phase
} LearnPatternAdvancedOptions, *PLearnPatternAdvancedOptions;

// Halton point structure
typedef struct _HaltonPoint {
    int x;				// Sampling point x coordinate
	int y;				// Sampling point y coordinate
	int nStableSize;	// Sampling point stable region size
} HaltonPoint,*PHaltonPoint;

// Halton samples
typedef struct _HaltonSamples {
    DWORD       hsSize;					// Struct size
	int         hsWidth;				// Samples window width
	int         hsHeight;				// Samples window height
	int         hsSamplePointsCount;	// Sample points count
	HaltonPoint hsHaltonPoints[1];		// Sample points
} HaltonSamples,*PHaltonSamples;

// Specifies an allowed range of rotation for a pattern
typedef struct _RotationAngleRange {
	int lower; // The lowest amount of rotation, in degrees, a valid pattern can have
	int upper; // The highest amount of rotation, in degrees, a valid pattern can have
} RotationAngleRange, *PRotationAngleRange;

// Describes how you want the function to search for the template image
typedef struct _MatchPatternOptions {
	DWORD               size;							// Struct size
	MatchingMode        mode;							// Specifies the method to use when looking for the pattern in the image
	int                 minContrast;					// Specifies the minimum contrast expected in the image
	BOOL                subpixelAccuracy;				// Set this element to TRUE to return areas in the image that match the pattern area with subpixel accuracy
	RotationAngleRange *angleRanges;					// An array of angle ranges, in degrees, where each range specifies how much you expect the pattern to be rotated in the image. To decrease the search time, limit the degrees of rotation in which you expect to find the template image. Set this element to NULL to allow all angles
	int                 numRanges;						// Number of angle ranges in the angleRanges array
	int                 numMatchesRequested;			// Number of valid matches expected
	float               minMatchScore;					// The minimum score a match can have for the function to consider the match valid
	int                 subpixelIterations;				// Defines the maximum number of incremental improvements used to refine matching using subpixel information.
	double              subpixelTolerance;				// Defines the maximum amount of change, in pixels, between consecutive incremental improvements in the match position that you want to trigger the end of the refinement process
	int                 initialMatchListLength;			// Specifies the maximum size of the match list. The match list contains the regions in the inspection image that have the highest probability of containing a match
	int                 matchListReductionFactor;		// Specifies the reduction of the match list as matches are refined
	int                 initialStepSize;				// Specifies the number of pixels to shift the sample across the inspection image during the initial phase of shift-invariant matching. The default is 0, which uses the initialStepSize stored in the template. If the step size is not an odd integer, the algorithm uses the default value
	int                 intermediateAngularAccuracy;	// Specifies the accuracy to use during the intermediate phase of rotation-invariant matching
} MatchPatternOptions, *PMatchPatternOptions;

// Information describing a matched pattern
typedef struct _PatternMatch {
	POINTFLOAT position;	// The location of the center of the match
	float      rotation;	// The rotation of the match relative to the template image, in degrees
	float      scale;		// The size of the match relative to the size of the template image, expressed as a percentage
	float      score;		// The accuracy of the match. A score of 1,000 indicates a perfect match, and a score of 0 indicates no match
	POINTFLOAT corner[4];	// An array of four points describing the rectangle surrounding the template image
}PatternMatch, *PPatternMatch;

// Describes how a function identifies curves in an image
typedef struct _CurveOptions {
	ExtractionMode extractionMode;		// Specifies the method the function uses to identify curves in the image
	BOOL           onlyClosed;			// Set this element to TRUE to specify that the function should only identify closed curves in the image. Set this element to FALSE to specify that the function should identify both open and closed curves in the image
	BOOL           subpixelAccuracy;	// Set this element to TRUE to specify that the function identifies the location of curves with subpixel accuracy by interpolating between points to find the crossing of threshold
	int            filterSize;			// Specifies the width of the edge filter the function uses to identify curves in the image
	int            minLength;			// Specifies the length, in pixels, of the smallest curve the function will extract
	int            maxLength;			// Specifies the length, in pixels, of the longest curve the function will extract
	int            rowStepSize;			// Specifies the distance, in the y direction, between lines the function inspects for curve seed points
	int            columnStepSize;		// Specifies the distance, in the x direction, between columns the function inspects for curve seed points
	int            maxEndPointGap;		// Specifies the maximum gap, in pixels, between the endpoints of a curve that the function identifies as a closed curve
	THRESHOLD      lowerThreshold;		// Specifies the minimum contrast a edge point must have in order to be a part of a curve
	THRESHOLD      upperThreshold;		// Specifies the minimum contrast a seed point must have in order to begin a curve
}CurveOptions, *PCurveOptions;

// The size and contents of a structuring element specify which pixels a morphological
// operation takes into account when determining the new value of the pixel being processed 
typedef struct _StructuringElement {
	int  matrixCols;	// Number of columns in the matrix
	int  matrixRows;	// Number of rows in the matrix
	BOOL hexa;			// Set this element to TRUE if you specify a hexagonal structuring element in kernel
	int  *pKernel;		// The values of the structuring element
}StructuringElement,*PStructuringElement;

// Describes a circle that best fits a set of points
typedef struct _BestCircle {
	POINTFLOAT center;			// The coordinate location of the center of the circle
	float      radius;			// The radius of the circle
	float      area;			// The area of the circle
	float      perimeter;		// The length of the perimeter of the circle
	float      meanDistance;	// Represents the least square error of the fitted circle to the entire set of points
	float      maxError;		// The largest deviation distance from edge to circle
	float      score;			// The accuracy of the fitting. A score of 1,000 indicates a perfect fitting, and a score of 0 indicates fitting failed completely
	BOOL       valid;			// This element is TRUE if the function achieved the minimum score within the number of allowed refinement iterations and FALSE if the function did not achieve the minimum score
	int       *pointsUsed;		// An array of the indexes for the points array indicating which points the function used to fit the circle
	int        numPointsUsed;	// The number of points the function used to fit the circle
}BestCircle, *PBestCircle;

// Defines the three coefficients of the equation in the normal form (ax+by+c=0) of a line
typedef struct _LineEquation {
	double a;	// The a coefficient of the line equation
	double b;	// The b coefficient of the line equation
	double c;	// The c coefficient of the line equation
}LineEquation, *PLineEquation;

// Describes a line that best fits a set of points
typedef struct _BestLine {
	POINTFLOAT   start;			// The coordinate location of the start of the line
	POINTFLOAT   end;			// The coordinate location of the end of the line
	LineEquation equation;		// Defines the three coefficients of the equation of the best fit line
	BOOL         valid;			// This element is TRUE if the function achieved the minimum score within the number of allowed refinement iterations and FALSE if the function did not achieve the minimum score
	float        alpha;			// The pitch angle of the line
	float        meanDistance;	// Represents the least square error of the fitted line to the entire set of points
	float        score;			// The accuracy of the fitting. A score of 1,000 indicates a perfect fitting, and a score of 0 indicates fitting failed completely
	int         *pointsUsed;    // An array of the indexes for the points array indicating which points the function used to fit the line
	int          numPointsUsed; // The number of points the function used to fit the line
}BestLine, *PBestLine;

// Describes the circles the function searches for
typedef struct _CircleDescriptor {
	float minRadius; // Specifies the minimum radius of a circle the function will return
	float maxRadius; // Specifies the maximum radius of a circle the function will return
}CircleDescriptor, *PCircleDescriptor;

// Float range
typedef struct _RangeFloat {
	float minValue; // The minimum value of the range
	float maxValue; // The maximum value of the range
}RangeFloat, *PRangeFloat;

// Specifies the requirements for shapes that the function detects
typedef struct _ShapeDetectionOptions {
	unsigned int mode;            // Specifies the method used when looking for the shape in the image. Combine values from the GeometricMatchingMode enumeration to specify the value of this element
	RangeFloat  *angleRanges;     // An array of angle ranges, in degrees, where each range specifies how much you expect the shape to be rotated in the image. To decrease the search time, limit the degrees of rotation in which you expect to find the shape in the image. Set this element to NULL to allow all angles. This function ignores this range if mode does not include VL_GEOMETRIC_MATCH_ROTATION_INVARIANT
	int          numAngleRanges;  // The size of the orientationRanges array
	RangeFloat   scaleRange;      // A range that specifies the sizes of the shapes you expect to be in the image, expressed as a ratio percentage representing the size of the pattern in the image divided by size of the original pattern multiplied by 100. This function ignores this range if mode does not include VL_GEOMETRIC_MATCH_SCALE_INVARIANT
	float        minMatchScore;   // The minimum score a match can have for the function to consider the match valid. Acceptable values range from 0 to 1,000
}ShapeDetectionOptions, *PShapeDetectionOptions;

// Information describing a matched circle
typedef struct _CircleMatch {
	POINTFLOAT center;	// The location of the center of the matched circle
	float      radius;	// The radius of the matched circle
	float      score;	// The score of the matched circle
}CircleMatch, *PCircleMatch;

// A circle feature
typedef struct _CircleFeature {
    POINTFLOAT position; // The location of the center of the circle
    float      radius;   // The radius of the circle
} CircleFeature, *PCircleFeature;


// An ellipse feature
typedef struct _EllipseFeature {
    POINTFLOAT position;    // The location of the center of the ellipse
    float      rotation;    // The orientation of the semi-major axis of the ellipse with respect to the horizontal
    float      minorRadius; // The length of the semi-minor axis of the ellipse
    float      majorRadius; // The length of the semi-major axis of the ellipse
} EllipseFeature, *PEllipseFeature;

// A constant curve feature
typedef struct _ConstCurveFeature {
    POINTFLOAT position;   // The center of the circle that this constant curve lies upon
    float      radius;     // The radius of the circle that this constant curve lies upon
    float      startAngle; // When traveling along the constant curve from one endpoint to the next in a counterclockwise manner, this is the angular component of the vector originating at the center of the constant curve and pointing towards the first endpoint of the constant curve
    float      endAngle;   // When traveling along the constant curve from one endpoint to the next in a counterclockwise manner, this is the angular component of the vector originating at the center of the constant curve and pointing towards the second endpoint of the constant curve
} ConstCurveFeature, *PConstCurveFeature;

// A rectangle feature
typedef struct _RectangleFeature {
    POINTFLOAT position;  // The center of the rectangle
    POINTFLOAT corner[4]; // The four corners of the rectangle
    float      rotation;  // The orientation of the rectangle with respect to the horizontal
    float      width;     // The width of the rectangle
    float      height;    // The height of the rectangle
} RectangleFeature, *PRectangleFeature;

// A leg feature
typedef struct _LegFeature {
    POINTFLOAT position;  // The location of the leg feature
    POINTFLOAT corner[4]; // The four corners of the leg feature
    float      rotation;  // The orientation of the leg with respect to the horizontal
    float      width;     // The width of the leg
    float      height;    // The height of the leg
} LegFeature, *PLegFeature;

// A corner feature
typedef struct _CornerFeature {
    POINTFLOAT position;      // The location of the corner feature
    float      rotation;      // The angular component of the vector bisecting the corner from position
    float      enclosedAngle; // The measure of the enclosed angle of the corner
    int        isVirtual;     // This element is TRUE if the corner is virtual and FALSE if the corner is not virtual. A virtual corner is a corner that would be created if two non-intersecting lines are extended until they intersect
} CornerFeature, *PCornerFeature;

// A parallel line pair feature
typedef struct _ParallelLinePairFeature {
    POINTFLOAT firstStartPoint;  // The starting point of the first line of the pair
    POINTFLOAT firstEndPoint;    // The ending point of the first line of the pair
    POINTFLOAT secondStartPoint; // The starting point of the second line of the pair
    POINTFLOAT secondEndPoint;   // The ending point of the second line of the pair
    float      rotation;         // The orientation of the feature with respect to the horizontal
    float      distance;         // The distance from the first line to the second line
} ParallelLinePairFeature, *PParallelLinePairFeature;

// A pair of parallel line pairs feature
typedef struct _PairOfParallelLinePairsFeature {
    ParallelLinePairFeature firstParallelLinePair;  // The first parallel line pair
    ParallelLinePairFeature secondParallelLinePair; // The second parallel line pair
    float                   rotation;               // The orientation of the feature with respect to the horizontal
    float                   distance;               // The distance from the midline of the first parallel line pair to the midline of the second parallel line pair
} PairOfParallelLinePairsFeature, *PPairOfParallelLinePairsFeature;

// A line feature
typedef struct _LineFeature {
    POINTFLOAT startPoint; // The starting point of the line
    POINTFLOAT endPoint;   // The ending point of the line
    float      length;     // The length of the line measured in pixels from the start point to the end point
    float      rotation;   // The orientation of the line with respect to the horizontal
} LineFeature, *PLineFeature;

// A closed curve feature
typedef struct _ClosedCurveFeature {
    POINTFLOAT position;  // The center of the closed curve feature
    float      arcLength; // The arc length of the closed curve feature
} ClosedCurveFeature, *PClosedCurveFeature;

// A union of pointers to geometric feature types
typedef union GeometricFeature_union {
    CircleFeature                  *circle;                  // A pointer to a CircleFeature
    EllipseFeature                 *ellipse;                 // A pointer to an EllipseFeature
    ConstCurveFeature              *constCurve;              // A pointer to a ConstCurveFeature
    RectangleFeature               *rectangle;               // A pointer to a RectangleFeature
    LegFeature                     *leg;                     // A pointer to a LegFeature
    CornerFeature                  *corner;                  // A pointer to a CornerFeature
    ParallelLinePairFeature        *parallelLinePair;        // A pointer to a ParallelLinePairFeature
    PairOfParallelLinePairsFeature *pairOfParallelLinePairs; // A pointer to a PairOfParallelLinePairsFeature
    LineFeature                    *line;                    // A pointer to a LineFeature
    ClosedCurveFeature             *closedCurve;             // A pointer to a ClosedCurveFeature
} GeometricFeature, *PGeometricFeature;

// A structure describing a generic geometric matching feature
typedef struct _FeatureData {
	FeatureType        type;             // An enumeration representing the type of the feature
	vector<POINTFLOAT> contourPoints;    // A set of points describing the contour of the feature
	int                numContourPoints; // The number of points in the contourPoints vector 
	float              error;            // The maximum deviation of the contour points to this feature
	GeometricFeature   feature;          // The feature data specific to this type of feature
} FeatureData, *PFeatureData;

// Information about a curve
typedef struct _Curve {
	vector<POINTFLOAT> points;              // The points on the curve
	int                numPoints;           // The number of points in the curve
	BOOL               closed;              // This element is TRUE if the curve is closed and FALSE if the curve is open
	float              curveLength;         // The length of the curve
	float              minEdgeStrength;     // The lowest edge strength detected on the curve
	float              maxEdgeStrength;     // The highest edge strength detected on the curve
	float              averageEdgeStrength; // The average of all edge strengths detected on the curve
}Curve, *PCurve;

// Geometric primitives specified by index and coordinates
typedef struct _GeoPrimitive {
	PrimitiveType type;		// Geometric primitive type
	INDEX         start;	// Start index
	INDEX         end;		// End index
	float         error;	// Max error 
}GeoPrimitive, *PGeoPrimitive;

// Defines the location, size, and rotation of a rotated rectangle
typedef struct _RotatedRect {
	POINTFLOAT topLeft;     // Location of the top left vertex of the rectangle after rotation
	POINTFLOAT bottomLeft;  // Location of the bottom left vertex of the rectangle after rotation
	POINTFLOAT bottomRight; // Location of the bottom right vertex of the rectangle after rotation
	POINTFLOAT topRight;    // Location of the top right vertex of the rectangle after rotation
	FLOAT      width;       // Rectangle width
	FLOAT      height;		// Rectangle height
	double     angle;		// The rotation, in degrees, of the rectangle
}RotatedRect, *PRotatedRect;

// Describes how you want the function to find edges
typedef struct _EdgeOptions {
    THRESHOLD           threshold;         // Specifies the threshold value for the contrast of the edge
    int                 width;             // The number of pixels that the function averages to find the contrast at either side of the edge
    int                 steepness;         // The span, in pixels, of the slope of the edge projected along the path specified by the input points
    InterpolationMethod subpixelType;      // The method for interpolating
    int                 subpixelDivisions; // The number of samples the function obtains from a pixel
}EdgeOptions, *PEdgeOptions;

// Information about an edge
typedef struct _EdgeReport {
    double       location;   // The location of the edge from the start point
    FLOAT        contrast;   // The contrast at the edge
    PolarityType polarity;   // The polarity of the edge
    FLOAT        reserved;   // This element is reserved
    POINTFLOAT   coordinate; // The coordinates of the edge
}EdgeReport, *PEdgeReport;

// Information about a particle in an image
typedef struct _ParticleReport {
	int   label;			// The lable of this particle
    int   area;             // The number of pixels in the particle
    float calibratedArea;   // The size of the particle, calibrated to the calibration information of the image
    float perimeter;        // The length of the perimeter, calibrated to the calibration information of the image
    int   numHoles;         // The number of holes in the particle
    int   areaOfHoles;      // The total surface area, in pixels, of all the holes in a particle
    float perimeterOfHoles; // The length of the perimeter of all the holes in the particle calibrated to the calibration information of the image
    RECT  boundingBox;      // The smallest rectangle that encloses the particle
    int   sigmaX;           // The sum of the particle pixels on the x-axis
    int   sigmaY;           // The sum of the particle pixels on the y-axis
    int   sigmaXX;          // The sum of the particle pixels on the x-axis, squared
    int   sigmaYY;          // The sum of the particle pixels on the y-axis, squared
    int   sigmaXY;          // The sum of the particle pixels on the x-axis and y-axis
    int   longestLength;    // The length of the longest horizontal line segment
    POINT longestPoint;     // The location of the leftmost pixel of the longest segment in the particle
    int   projectionX;      // The length of the particle when projected onto the x-axis
    int   projectionY;      // The length of the particle when projected onto the y-axis
    BOOL  connect8;         // This element is TRUE if the function used connectivity-8 to determine if particles are touching
} ParticleReport, *PParticleReport;

// Configures how VisionLib processes the image before training or reading characters
typedef struct _OCRProcessingOptions {
    ThresholdMode   mode;                       // The thresholding mode
	ThresholdMethod thresholdMethod;            // The thresholding method
    int             lowThreshold;               // The low threshold value when you set mode to VL_FIXED_RANGE
    int             highThreshold;              // The high threshold value when you set mode to VL_FIXED_RANGE
    int             blockCount;                 // The number of blocks for threshold calculation algorithms that require blocks
    BOOL            fastThreshold;              // Set this element to TRUE to use a faster, less accurate threshold calculation algorithm
    BOOL            biModalCalculation;         // Set this element to TRUE to calculate both the low and high threshold values when using the fast thresholding method
    BOOL            darkCharacters;             // Set this element to TRUE to read or train dark characters on a light background
    BOOL            removeParticlesTouchingROI; // Set this element to TRUE to remove the particles touching the ROI
} OCRProcessingOptions, *POCRProcessingOptions;

// Character size and spacing constraints you want to use during the training or reading process
typedef struct _OCRSpacingOptions {
    int minCharSpacing;              // The minimum number of pixels that must be between two characters for VisionLib to train or read the characters separately
    int minCharSize;                 // The minimum number of pixels required for an object to be a potentially identifiable character
    int maxCharSize;                 // The maximum number of pixels required for an object to be a potentially identifiable character
    int maxHorizontalElementSpacing; // The maximum horizontal spacing, in pixels, allowed between character elements to train or read the character elements as a single character
    int maxVerticalElementSpacing;   // The maximum vertical element spacing in pixels
    int minBoundingRectWidth;        // The minimum possible width, in pixels, for a character bounding rectangle
    int maxBoundingRectWidth;        // The maximum possible width, in pixels, for a character bounding rectangle
    int minBoundingRectHeight;       // The minimum possible height, in pixels, for a character bounding rectangle
    int maxBoundingRectHeight;       // The maximum possible height, in pixels, for a character bounding rectangle
    int autoSplit;                   // Set this element to TRUE to automatically adjust the location of the character bounding rectangle when characters overlap vertically
} OCRSpacingOptions, *POCRSpacingOptions;

// VisionLib OCR configuration settings you want to use during the reading process
typedef struct _ReadTextOptions {
	char substitutionChar; // The character to substitute for objects that the function cannot match with any of the trained characters
	int  acceptanceLevel;  // The minimum acceptance level at which an object is considered a trained character
	int  aspectRatio;      // The maximum aspect ratio variance percentage for valid characters
} ReadTextOptions, *PReadTextOptions;

// Contains information about a character
typedef struct _CharReport {
	char       character; // The character value
	POINTFLOAT corner[4]; // An array of four points that describes the rectangle that surrounds the character
	int        score;     // The similarity of the character and the reference character for the character class
}CharReport, *PCharReport;

// VisionLib OCV learning settings you want to use during the learning process
typedef struct _OCVLearningOptions {
	UINT indexesMask; // OCV learning indexes
} OCVLearningOptions, *POCVLearningOptions;

// VisionLib OCV learning settings you want to use during the learning process
typedef struct _OCVVerifyingOptions {
	UINT indexesMask;            // Logical combination (bitwise OR) of the desired verifying indexes, as define by enum OCVIndexes
	int  acceptanceLevel;        // The minimum acceptance level at which an pattern is considered verified
	int  areaTolerance;          // Pattern area tolerance
	int  aspectRatioTolerance;	 // Pattern aspect ratio tolerance
	int  brightnessTolerance;    // Pattern brightness tolerance
	int  contrastTolerance;      // Pattern contrast tolerance
	int  absPositionTolerance;   // Pattern absolute position tolerance
	int  reaPositionTolerance;   // Pattern relative position tolerance
	int  acceptanceCorrScore;    // Minimum normalized correlation score
} OCVVerifyingOptions, *POCVVerifyingOptions;

// Contains information about a pattern
typedef struct _PattReport {
	int        pattern;     // The pattern value
	POINTFLOAT corner[4];   // An array of four points that describes the rectangle that surrounds the pattern
	int        score;       // The mean similarity of the pattern and the reference pattern
	UINT	   diagnostics; // Logical combination (bitwise OR) of the defects found, as define by enum OCVDiagnostics
} PattReport, *PPattReport;

// The information needed to describe a color in the RGB (Red, Green, and Blue) color space
typedef struct _RGBValue {
	unsigned char B;     // The blue value of the color
	unsigned char G;     // The green value of the color
	unsigned char R;     // The red value of the color
	unsigned char alpha; // The alpha value of the color, which represents extra information about a color image, such as gamma correction
} RGBValue, *PRGBValue;

// The information needed to describe a color in the HSL (Hue, Saturation, and Luminance) color space
typedef struct _HSLValue {
	unsigned char L;     // The color luminance
	unsigned char S;     // The color saturation
	unsigned char H;     // The color hue
	unsigned char alpha; // The alpha value of the color, which represents extra information about a color image, such as gamma correction
} HSLValue, *PHSLValue;

// A complex value
typedef struct _Complex {
	float r; // The real part of the value
	float i; // The imaginary part of the value
} Complex, *PComplex;

// The information needed to describe color in the RGB (Red, Green, Blue) color space where each channel has 16 bits
typedef struct _RGBU64Value {
	unsigned short B;     // The blue value of the color
	unsigned short G;     // The green value of the color
	unsigned short R;     // The red value of the color
	unsigned short alpha; // The alpha value of the color, which represents extra information about a color image, such as gamma correction
} RGBU64Value, *PRGBU64Value;

// The information necessary to describe a particular type of pixel
typedef struct _PixelValue {
	float       grayscale; // A grayscale pixel value. Use this member for images of type VL_IMAGE_U8, VL_IMAGE_I16, or VL_IMAGE_SGL
	RGBValue    rgb;       // A RGB pixel value. Use this member for images of type VL_IMAGE_RGB
	HSLValue    hsl;       // A HSL pixel value. Use this member for images of type VL_IMAGE_HSL
	Complex     complex;   // A complex pixel value. Use this member for images of type VL_IMAGE_COMPLEX
	RGBU64Value rgbu64;    // An unsigned 64-bit RGB pixel value. Use this member for images of type VL_IMAGE_RGB_U64
} PixelValue, *PPixelValue;

// Information about an image
typedef struct _ImageInfo {
	CalibrationUnit imageUnit;      // If you set calibration information with SetCalibrationInfo(), imageUnit is the calibration unit
	float           stepX;          // If you set calibration information with SetCalibrationInfo(), stepX is the distance in the calibration unit between two pixels in the x direction
	float           stepY;          // If you set calibration information with SetCalibrationInfo(), stepY is the distance in the calibration unit between two pixels in the y direction
	ImageType       imageType;      // The type of the image
	int             xRes;           // The number of columns in the image
	int             yRes;           // The number of rows in the image
	int             xOffset;        // If you set mask offset information with SetMaskOffset(), xOffset is the offset of the mask origin in the x direction
	int             yOffset;        // If you set mask offset information with SetMaskOffset(), yOffset is the offset of the mask origin in the y direction
	int             border;         // The number of border pixels around the image
	int             pixelsPerLine;  // The number of pixels stored for each line of the image. This value may be larger than xRes
	void*           reserved0;      // This element is reserved
	void*           reserved1;      // This element is reserved
	void*           imageStart;     // A pointer to pixel (0,0)
}ImageInfo, *PImageInfo;

// Information about an ellipse
typedef struct _EllipseParameter {
	POINTFLOAT	center;				// The coordinate location of the center of the ellipse
	POINTFLOAT	majorAxisStart;		// The coordinate location of the start of the major axis of the ellipse
	POINTFLOAT	majorAxisEnd;		// The coordinate location of the end of the major axis of the ellipse
	POINTFLOAT	minorAxisStart;		// The coordinate location of the start of the minor axis of the ellipse
	POINTFLOAT	minorAxisEnd;		// The coordinate location of the end of the minor axis of the ellipse
	float		orientation;		// The orientation of ellipse
}EllipseParameter, *PEllipseParameter;

//============================================================================
//  Data Structures Declare
//============================================================================
typedef vector<float>          FloatArray;
typedef vector<EdgePoint>      Edge;
typedef vector<Curve>          Curves;
typedef vector<PrimitiveType>  GeoTypes;
typedef vector<GeoPrimitive>   Primitives;
typedef vector<ParticleReport> Particles;
typedef vector<POINTFLOAT>     Contour;
typedef vector<POINTFLOAT>     PointsSet;

//============================================================================
// Export classes
//============================================================================
// This class is exported from the VisionLib.dll
class VISIONLIB_API CVisionLib {
public:
	CVisionLib(void);
	// TODO: add your methods here
};

//============================================================================
//  Interface IPersistency
//============================================================================
struct IPersistency {
	// Reads a object and the appropriate VisionLib structures from the file specified by fileName
	virtual BOOL Load(LPCTSTR lpszFileName) = 0;
	// Stores a object and the values of the appropriate VisionLib structures in the file specified by fileName
	virtual BOOL Save(LPCTSTR lpszFileName) = 0;
	// Cleans up resources associated with images, regions of interest (ROIs), arrays, and reports that you no longer need. After you dispose of something, you can no longer use it 
	virtual void Dispose() = 0; 
};

//============================================================================
//  Interface IEnumObject
//============================================================================
struct IEnumObject {
	virtual BOOL IsEOF()    const = 0;
	virtual int  GetCount() const = 0;
	virtual void MoveNext()       = 0;
	virtual void MoveFirst()      = 0;
	virtual void Dispose()        = 0;
};

//============================================================================
//  OCR Interface ICharObject
//============================================================================
struct ICharObject : public IEnumObject {
	virtual void GetBoundingBox(RECT &boundingBox) const = 0;
};

//============================================================================
//  OCR Interface IEnumChar
//============================================================================
struct IEnumChar : public IEnumObject {
	virtual void GetCharValue(char &value)           const = 0;
	virtual void GetCharBitmap(HBITMAP &hCharBitmap) const = 0;
};

//============================================================================
//  OCR Interface ICharReport
//============================================================================
struct ICharReport : public IEnumObject {
	virtual void GetCharReport(CharReport &report) const = 0;
};

//============================================================================
//  OCR Interface ICharSet
//============================================================================
struct ICharSet : public IPersistency {
	virtual BOOL Attach(const Image &mother, const RECT rect) = 0;
	virtual void Detach() = 0;
	virtual BOOL PutOptions(const OCRProcessingOptions &processingOptions, const OCRSpacingOptions &spacingOptions, const ReadTextOptions &readTextOptions) = 0;
	virtual void GetOptions(OCRProcessingOptions &processingOptions, OCRSpacingOptions &spacingOptions, ReadTextOptions &readTextOptions) = 0;
	virtual ICharObject *Segment() = 0;
	virtual BOOL AddChars(const char *pCharValue, const int numChars) = 0;
	virtual BOOL Train() = 0;
	virtual ICharReport *ReadText(const Image &image, const RECT roi) = 0;
	virtual void Empty() = 0;
};

//============================================================================
//  OCV Interface IPatternObject
//============================================================================
struct IPatternObject : public IEnumObject {
	virtual void GetBoundingBox(RECT &boundingBox) const = 0;
};

//============================================================================
//  OCV Interface IEnumPattern
//============================================================================
struct IEnumPattern : public IEnumObject {
	virtual void GetPattValue(char &value)           const = 0;
	virtual void GetPattBitmap(HBITMAP &hCharBitmap) const = 0;
	virtual void GetTextBox(RECT &textBox)           const = 0;
};

//============================================================================
//  OCV Interface IPatternReport
//============================================================================
struct IPatternReport : public IEnumObject {
	virtual void GetPattReport(PattReport &report) const = 0;
	virtual void GetTextBox(RECT &textBox)         const = 0;
};

//============================================================================
//  OCV Interface IObjectSet
//============================================================================
struct IObjectSet : public  IPersistency {
	virtual BOOL Attach(const Image &mother, const RECT rect) = 0;
	virtual void Detach() = 0;
	virtual BOOL PutProcessingOptions(const OCRProcessingOptions &processingOptions) = 0;
	virtual BOOL PutSpacingOptions(const OCRSpacingOptions &spacingOptions) = 0;
	virtual BOOL PutVerifyingOptions(const OCVVerifyingOptions &verifyingOptions) = 0;
	virtual void GetProcessingOptions(OCRProcessingOptions &processingOptions) = 0;
	virtual void GetSpacingOptions(OCRSpacingOptions &spacingOptions) = 0;
	virtual void GetVerifyingOptions(OCVVerifyingOptions &verifyingOptions) = 0;
	virtual IPatternObject *Segment() = 0;
	virtual IEnumPattern *LearnObjects(const OCVLearningOptions &learningOptions, const char *pObjectValue, const int numObjects) = 0;
	virtual IPatternReport *VerifyObjects(const Image &image, const RECT roi) = 0;
};

//============================================================================
//  Template Matching Interface IPatternMatch
//============================================================================
struct IPatternMatch : public IEnumObject {
	virtual void GetPatternMatch(PatternMatch &match) const = 0;
	virtual void GetOffset(SIZE &offset) const = 0;
	virtual void GetID(UINT &id) const = 0;
};

//============================================================================
//  Template Matching Interface ITemplate
//============================================================================
struct ITemplate : public IPersistency {
	virtual BOOL Attach(const Image &mother, const RECT rect) = 0;
	virtual void Detach() = 0;
	virtual BOOL PutLearningOptions(const LearnPatternAdvancedOptions &options) = 0;
	virtual void GetLearningOptions(LearnPatternAdvancedOptions &options) const = 0;
	virtual BOOL PutMatchingOptions(const MatchPatternOptions &options) = 0;
	virtual void GetMatchingOptions(MatchPatternOptions &options) const = 0;
	virtual BOOL Learn() = 0;
	virtual BOOL LearnAndAdd(const UINT id) = 0;
	virtual void GetCount(int &nCount) const = 0;
	virtual void Clear() = 0;
	virtual IPatternMatch *Match(const Image &image, const RECT &searchRect) = 0;
};

//============================================================================
//  Image Border Interface IBorder
//============================================================================
struct IBorder {
	// Sets the border size of an image. This operation preserves image pixels
	virtual BOOL SetBorderSize(const int size) = 0;
	// Returns the border size of the given image
	virtual BOOL GetBorderSize(int &borderSize) const = 0;
	// Modifies the border of an image
	virtual BOOL FillBorder(const BorderMethod method) = 0;
};

//============================================================================
//  Image Information Interface IImageInfo
//============================================================================
struct IImageInfo {
	// Returns the bit depth of the image
	virtual BOOL GetBitDepth(unsigned int &bitDepth) const = 0;
	// Returns the number of bytes that a single pixel occupies in the given image
	virtual BOOL GetBytesPerPixel(int &byteCount) const = 0;
	// Returns the size, border, type, calibration, and memory layout of an image
	virtual BOOL GetImageInfo(ImageInfo &info) const = 0;
	// Returns the size of a given image
	virtual BOOL GetImageSize(int &width, int &height) const = 0;
	// Returns the type of the given image
	virtual BOOL GetImageType(ImageType &type) const = 0;
	// Returns the address of a given pixel in an image. If the requested pixel location is outside of the image, the function fails and returns NULL
	virtual void *GetPixelAddress(const POINT &pixel) const = 0;
	// Tests to see if the supplied image is empty
	virtual BOOL IsImageEmpty(BOOL &empty) const = 0;
};

//============================================================================
//  Image Manipulation Interface IImageManipu
//============================================================================
struct IImageManipu {
	// Sets the size of an image. The original pixels are not transferred to the new image. The new image will contain uninitialized pixels
	virtual BOOL SetImageSize(const int width, const int height) = 0;
	// Copies an area of one image into another image
	virtual BOOL Crop(IImage *pIDest ,const RECT &rect, const POINT &destLoc) const = 0;
	// Copies the source image to the destination image, including the border size and vision information
	virtual BOOL Duplicate(IImage *pIDest) const = 0;
	// Flips an image over an axis
	virtual BOOL Flip(IImage *pIDest, const FlipAxis axis) = 0;
	// Copies the source image to the destination image in the following manner: If a pixel in the mask has a value of 0, the function sets the corresponding source pixel to 0. Otherwise, the function copies the corresponding source pixel to the destination image
	virtual BOOL Mask(IImage *pIDest, const IImage* pIMask) = 0;
	// Resizes an image to a given resolution
	virtual BOOL Resample(IImage *pIDest, const int newWidth, const int newHeight, const InterpolationMethod method, const RECT &rect) const = 0;
	// Rotates an image counterclockwise
	virtual BOOL Rotate(IImage *pIDest, const POINTFLOAT pivot, const float angle, const PixelValue &fill, const InterpolationMethod method, const BOOL maintainSize) = 0;
	// Scales an image or area of an image. The source image and destination image must be the same image type. This function makes an image larger by duplicating pixels, and it makes an image smaller by subsampling pixels. For more sophisticated scaling techniques, use Resample()
	virtual BOOL Scale(IImage *pIDest, const int xScale, const int yScale, const ScalingMode scaleMode, const RECT &rect) const = 0;
	// Shifts an image
	virtual BOOL Shift(IImage *pIDest, const float shiftX, const float shiftY, const PixelValue &fill) = 0;
	// Transposes an image
	virtual BOOL Transpose(IImage *pIDest) const = 0;
	// This function unwraps an annulus from an image into a rectangular image
	virtual BOOL UnwrapImage(IImage *pIDest, const Annulus &annulus, const RectOrientation orientation, const InterpolationMethod method) const = 0;
};

//============================================================================
//  Image Management Interface IImage
//============================================================================
struct IImage : public IPersistency, public IImageInfo, public IImageManipu, public IBorder {
	// Sets each pixel in an image to a specified value
	virtual BOOL FillImage(const PixelValue &value, const IImage *pIMask) = 0;
	// Sets the pixels of an image to the values in a given array. This function resizes the image to the size of the source array
	virtual BOOL ArrayToImage(const void* array, int numCols, int numRows) = 0;
	// Sets the color palette to use when displaying a monochrome image in an image window
	virtual BOOL SetPalette(const PaletteType type, const RGBQUAD* palette, const int numColors) = 0;
	// Displays an image in an window
	virtual BOOL Display(HDC hdcDest) const = 0;
	// For more efficient screen displays
	virtual BOOL CreateDDB(const HWND hwnd, const HPALETTE hPalette, HBITMAP &hDDB) const = 0;
};

//============================================================================
//  Globals
//============================================================================
extern VISIONLIB_API int nVisionLib;

//============================================================================
//  Dummy functions
//============================================================================
VISIONLIB_API int fnVisionLib(void);

//============================================================================
//  Error Management functions
//============================================================================
VISIONLIB_API int          vlClearError(void);
VISIONLIB_API const char*  vlGetErrorText(int errorCode);
VISIONLIB_API int          vlGetLastError(void);
VISIONLIB_API int          vlSetError(int errorCode);

//============================================================================
//  Image Analysis functions
//============================================================================
VISIONLIB_API BOOL vlExtractCurves(const Image &image, const RECT& search, const CurveOptions* pCurveOptions, Curves& result);
VISIONLIB_API BOOL vlSegmentCurve(const Curve &curve, const GeoTypes &types, Primitives &primitives);

//============================================================================
//  Pattern Matching functions
//============================================================================
VISIONLIB_API BOOL vlDetectCircles(const Image &image, const CircleDescriptor &circleDescriptor, const CurveOptions *pCurveOptions, const ShapeDetectionOptions *pShapeDetectionOptions, const RECT &roi, CircleMatch *const pCircles, int *const pNumCircles);
VISIONLIB_API PyraTmp *vlCreatePattern(const Image &mother, const RECT rect);
VISIONLIB_API PyraTmp *vlLoadPattern(LPCTSTR lpszFileName);
VISIONLIB_API BOOL vlSavePattern(const PyraTmp *pTemplate, LPCTSTR lpszFileName);
VISIONLIB_API void vlDestroyPattern(PyraTmp *pTemplate);
VISIONLIB_API BOOL vlLearnPattern(PyraTmp *pTemplate, const LearningMode lm, const LearnPatternOptions &lo);
VISIONLIB_API BOOL vlMatchPattern(const Image &image, const PyraTmp *pTemplate, const MatchPatternOptions& options, const RECT& searchRect, PatternMatch* pPatternMatch, int* numMatches);
VISIONLIB_API ITemplate *vlCreateTemplate(const StructureForm form);

//============================================================================
//  Threshold functions
//============================================================================
VISIONLIB_API BOOL vlAutoThreshold(const Image &source, Image &dest, const RECT& roi, const ThresholdMethod method, const BOOL darkBackground=TRUE, int *pThreshold=NULL);
VISIONLIB_API BOOL vlThreshold(const Image &source, Image &dest, const RECT& roi, const int threshold, const BOOL darkBackground=TRUE);

//============================================================================
//  Morphology functions
//============================================================================
VISIONLIB_API BOOL vlMorphology(const Image &source, Image &dest, const MorphologyMethod method, const RECT& roi, const StructuringElement* const pStructuringElement);
VISIONLIB_API BOOL vlLabel(const Image &source, Image &dest, const RECT& roi, const BOOL connectivity8, int* particleCount);
VISIONLIB_API BOOL vlSimpleConvexHull(const Contour &vertices, Contour &hull);
VISIONLIB_API BOOL vlConvexHull(const PointsSet &vertices, Contour &hull);

//============================================================================
//  Locate Edges Functions
//============================================================================
VISIONLIB_API BOOL vlExtractLinearEdge(const FloatArray& lineProfile, const int threshold, const int steepness, FLOAT *const pEdgePoint);

//============================================================================
//  Analytic Geometry functions
//============================================================================
VISIONLIB_API BOOL vlFitCircle(Edge &edgePoints, const FitCircleOptions *pOptions, BestCircle *const pReport);
VISIONLIB_API BOOL vlFitCircle(const Curve &curve, BestCircle *const pReport);
VISIONLIB_API BOOL vlFitLine(Edge &edgePoints, const FitLineOptions *pOptions, BestLine *const pReport);
VISIONLIB_API BOOL vlGetIntersection(const POINTFLOAT &start1, const POINTFLOAT &end1, const POINTFLOAT &start2, const POINTFLOAT &end2, POINTFLOAT &intersection);
VISIONLIB_API BOOL vlGetAngle(const POINTFLOAT &start1, const POINTFLOAT &end1, const POINTFLOAT &start2, const POINTFLOAT &end2, float &angle);
VISIONLIB_API BOOL vlGetPerpendicularLine(const POINTFLOAT &refLineStart, const POINTFLOAT &refLineEnd, const POINTFLOAT &point, POINTFLOAT &perpLineEnd);
VISIONLIB_API BOOL vlGetPerpendicularDistance(const POINTFLOAT &refLineStart, const POINTFLOAT &refLineEnd, const POINTFLOAT &point, double &distance);

//============================================================================
//  Locate Edges functions
//============================================================================
VISIONLIB_API BOOL vlFindCircularEdge(const Image &image, const Annulus& searchArea, const SpokeDirection direction, const FindEdgeOptions *pFindOptions, const FitCircleOptions *pFitOptions, CircularEdgeReport *pReport);
VISIONLIB_API BOOL vlFindEdge(const Image &image, const RECT &searchRect, const RakeDirection direction, const FindEdgeOptions *pFindOptions, const FitLineOptions *pFitOptions, StraightEdgeReport *pReport);
VISIONLIB_API BOOL vlFindEdge(const Image &image, const RotatedRect &searchRect, const FindEdgeOptions *pFindOptions, const FitLineOptions *pFitOptions, StraightEdgeReport* pReport);

//============================================================================
//  Spatial Filters functions
//============================================================================
VISIONLIB_API BOOL vlMeanFilter(const Image &source, Image &dest, const int width, const int height, const RECT &roi, const Image *pMask);
VISIONLIB_API BOOL vlMedianFilter(const Image &source, Image &dest, const int width, const int height, const RECT &roi, const Image *pMask);
VISIONLIB_API BOOL vlGaussFilter(const Image &source, Image &dest, const GaussKernel kernel, const RECT &roi, const Image *pMask);

//============================================================================
//  Image Management functions
//============================================================================
VISIONLIB_API BOOL   vlMakeImage(const HBITMAP hBitmap, Image &image);
VISIONLIB_API void   vlMakeRotatedRect(const FLOAT left, const FLOAT top, const FLOAT width, const FLOAT height, const double angle, RotatedRect &rotatedRect);
VISIONLIB_API BOOL   vlMirrorHor(const Image &source, Image *pDest);
VISIONLIB_API IImage *vlCreateImage(const ImageType &type);
VISIONLIB_API IImage *vlLoadImage(LPCTSTR lpszFileName);

//============================================================================
//  Utility
//============================================================================
VISIONLIB_API BOOL vlNormalize(double *pVector, const int nCount , int *pContrast , int *pMean);
VISIONLIB_API BOOL vlNormalize(float *pVector, const int nCount, int*pContrast, int *pMean);
VISIONLIB_API BOOL vlNormalize(FloatArray::iterator start, const int nCount, int *pContrast, int *pMean);
VISIONLIB_API BOOL vlFindPadCenter(const Image &image, const RECT &searchRect, POINTFLOAT &center);
VISIONLIB_API BOOL vlFindPadEllipseParam(const Image &image, const RECT &searchRect, long lEdgeLv,
										long padStableHeight, EllipseParameter &ellipse);
// Find pad ellipse parameter
// Return FALSE on failure, TRUE if success
// image: image data structure
// searchRect: search region
// lEdgeLv: minimum edge level
// padStableHeight: Stable pad height
// *dCenterX, *dCenterY: [output] Ellipse center point
// ellipse: Infomation about the found ellipse

//============================================================================
//  OCR&OCV
//============================================================================
VISIONLIB_API ICharSet *vlCreateCharSet();
VISIONLIB_API IObjectSet *vlCreateObjectSet();