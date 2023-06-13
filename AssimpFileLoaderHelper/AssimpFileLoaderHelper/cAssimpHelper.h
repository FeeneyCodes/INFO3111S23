#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// Forward declaration of the implementation class
class cFileLoader_Imp;

namespace AssimpHelper
{
	struct __declspec(dllexport) sVec3
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};
	struct __declspec(dllexport) sColourRGB
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
	};
	struct __declspec(dllexport) sColourRGBA
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 0.0f;
	};

	class __declspec(dllexport) cString
	{
	public:
		cString();
		cString(const char* const c_str);
		cString(const cString &copyString);				// Copy constructor
		cString& operator=(const cString &copyString);	// Copy assignment 
		cString operator+(const cString& concatString);	// concatenation
		~cString();
		unsigned int getLength();
		const char* c_str(void) const;
	private:
		void m_reset(void);
		char* m_data = NULL;
		unsigned int m_numCharacters = 0;
	};

	class __declspec(dllexport) cFace
	{
	public:
		cFace() {};
		unsigned int number_of_indices = 0;
		unsigned int* indicies = NULL;
	};

	class __declspec(dllexport) cMesh
	{
	public:
		cMesh() {};
		cString name;
		sVec3* vertices = NULL;
		sVec3* normals = NULL;
		sVec3* tangents = NULL;
		sVec3* bitangents = NULL;
		sColourRGBA* coloursRGBA = NULL;
		sVec3* texture_coordinates = NULL;
		unsigned int number_of_vertices = 0;

		cFace* faces = NULL;
		unsigned int number_of_faces = 0;

		enum ePrimitiveType
		{
			UNKNOWN_OR_UNDEFINED,		// 0?
			POINT,				// aiPrimitiveType_POINT = 0x1,
			LINE,				// aiPrimitiveType_LINE = 0x2,
			TRIANGLE,			// aiPrimitiveType_TRIANGLE = 0x4,
			POLYGON,			// aiPrimitiveType_POLYGON = 0x8,
			NGONE				// aiPrimitiveType_NGONEncodingFlag = 0x10,
		};
		ePrimitiveType primitive_type;
		cString getPrimitiveTypeString(void);

		unsigned int GetNumColorChannels();	// Get the number of vertex color channels the mesh contains.
		unsigned int GetNumUVChannels();	// Get the number of UV channels the mesh contains.
		bool HasBones();					// Check whether the mesh contains bones.
		bool HasFaces();					// Check whether the mesh contains faces.
		bool HasNormals();					// Check whether the mesh contains normal vectors.
		bool HasPositions();				// Check whether the mesh contains positions.
		bool HasTangentsAndBitangents();	// Check whether the mesh contains tangent and bitangent vectors 
											// It is not possible that it contains tangents and no bitangents (or the other way round).
		bool HasTextureCoords(unsigned int pIndex);		// Check whether the mesh contains a texture coordinate set.
		bool HasVertexColors (unsigned int pIndex);		// Check whether the mesh contains a vertex color set.
	};//cMesh

	class __declspec(dllexport) cCamera
	{
	public:
		float aspect_ratio = 0.0f;				// Screen aspect ratio.
		float near_clipping_plane = 0.0f;
		float far_clipping_plane = 0.0f;
		float half_horizontal_FOV = 0.0f;		// Half horizontal field of view angle, in radians.
		sVec3 target_or_at_vector;
		sVec3 position_or_eye_vector;
		sVec3 up_vector;
		cString name;
	};


	class __declspec(dllexport) cLight
	{
	public:
		float spotlight_inner_cone_angle;		// Spot light inner cone angle
		float spotlight_outer_cone_angle;
		float attenuation_constant;
		float attenuation_linear;
		float attenuation_quadratic;
		sVec3 colour_ambient_RGB;
		sVec3 colour_diffuse_RGB;
		sVec3 colour_specular_RGB;
		sVec3 position;
		sVec3 direction;
		cString name;
		enum eLightType
		{
			UNKNOWN_OR_UNDEFINED,			// aiLightSource_UNDEFINED = 0x0
			DIRECTIONAL_LIGHT,				// aiLightSource_DIRECTIONAL = 0x1
			POINT_LIGHT,					// aiLightSource_POINT = 0x2
			SPOT_LIGHT						// aiLightSource_SPOT = 0x3
		};
		eLightType lightType;
	};

	class __declspec(dllexport) cMaterialProperty
	{
	public:
		cMaterialProperty() {};
		cString key;
		unsigned int semantic = 0;
		unsigned int index = 0;
		unsigned int data_length = 0;
		enum ePropertyTypeInfo
		{
			// TO DO: Complete this
			AI_MATKEY_NAME			// #define AI_MATKEY_NAME "?mat.name", 0, 0
//			#define AI_MATKEY_TWOSIDED "$mat.twosided", 0, 0
//#define AI_MATKEY_SHADING_MODEL "$mat.shadingm", 0, 0
//#define AI_MATKEY_ENABLE_WIREFRAME "$mat.wireframe", 0, 0
//#define AI_MATKEY_BLEND_FUNC "$mat.blend", 0, 0
//#define AI_MATKEY_OPACITY "$mat.opacity", 0, 0
//#define AI_MATKEY_TRANSPARENCYFACTOR "$mat.transparencyfactor", 0, 0
//#define AI_MATKEY_BUMPSCALING "$mat.bumpscaling", 0, 0
//#define AI_MATKEY_SHININESS "$mat.shininess", 0, 0
//#define AI_MATKEY_REFLECTIVITY "$mat.reflectivity", 0, 0
//#define AI_MATKEY_SHININESS_STRENGTH "$mat.shinpercent", 0, 0
//#define AI_MATKEY_REFRACTI "$mat.refracti", 0, 0
//#define AI_MATKEY_COLOR_DIFFUSE "$clr.diffuse", 0, 0
//#define AI_MATKEY_COLOR_AMBIENT "$clr.ambient", 0, 0
//#define AI_MATKEY_COLOR_SPECULAR "$clr.specular", 0, 0
//#define AI_MATKEY_COLOR_EMISSIVE "$clr.emissive", 0, 0
//#define AI_MATKEY_COLOR_TRANSPARENT "$clr.transparent", 0, 0
//#define AI_MATKEY_COLOR_REFLECTIVE "$clr.reflective", 0, 0
//#define AI_MATKEY_GLOBAL_BACKGROUND_IMAGE "?bg.global", 0, 0
//#define AI_MATKEY_GLOBAL_SHADERLANG "?sh.lang", 0, 0
//#define AI_MATKEY_SHADER_VERTEX "?sh.vs", 0, 0
//#define AI_MATKEY_SHADER_FRAGMENT "?sh.fs", 0, 0
//#define AI_MATKEY_SHADER_GEO "?sh.gs", 0, 0
//#define AI_MATKEY_SHADER_TESSELATION "?sh.ts", 0, 0
//#define AI_MATKEY_SHADER_PRIMITIVE "?sh.ps", 0, 0
//#define AI_MATKEY_SHADER_COMPUTE "?sh.cs", 0, 0
		};
		ePropertyTypeInfo property_type;
		char* data = NULL;
	};

	class __declspec(dllexport) cMaterial
	{
	public:
		cMaterial() {};
	};

	class __declspec(dllexport) cScene
	{
	public:
		cScene();

		struct sSceneFlags
		{
			bool incomplete = false;			// #define AI_SCENE_FLAGS_INCOMPLETE			0x1
			bool validated = false;				// #define AI_SCENE_FLAGS_VALIDATED				0x2
			bool validation_warning = false;	// #define AI_SCENE_FLAGS_VALIDATION_WARNING	0x4
			bool non_verbose_format = false;	// define AI_SCENE_FLAGS_NON_VERBOSE_FORMAT		0x8
			bool terrain = false;				// #define AI_SCENE_FLAGS_TERRAIN				0x10
			bool allow_shared = false;			// #define AI_SCENE_FLAGS_ALLOW_SHARED			0x20
			void DecodeSceneFlags(unsigned int flags);
		};
		sSceneFlags scene_flags;

	};//cScene

	class __declspec(dllexport) cFileLoader
	{
	public:

		// These match the post process flags 
		// https://assimp.sourceforge.net/lib_html/postprocess_8h.html#a64795260b95f5a4b3f3dc1be4f52e410a6afb4fee42eca4482674859196cb8685
		struct sPostProcessFlags
		{
			bool bProcess_CalcTangentSpace = true;
			// aiProcess_CalcTangentSpace: Calculates the tangents and bitangents for the imported meshes.
			//                             Does nothing if a mesh does not have normals. 
			bool bProcess_JoinIdenticalVertices = true;
			// aiProcess_JoinIdenticalVertices: Identifies and joins identical vertex data sets within all imported meshes.
			bool bProcess_MakeLeftHanded = false;
			// aiProcess_MakeLeftHanded: Converts all the imported data to a left-handed coordinate space.
			bool bProcess_Triangulate = true;
			// aiProcess_Triangulate: Triangulates all faces of all meshes.
			bool bProcess_RemoveComponent = false;
			// aiProcess_RemoveComponent: Removes some parts of the data structure (animations, materials, light sources, cameras, textures, vertex components).
			bool bProcess_GenNormals = true;
			// aiProcess_GenNormals: Generates normals for all faces of all meshes.
			//                             (is ignored if normals are already there at the time this flag is evaluated.)
			//                             (can *NOT* be used with aiProcess_GenSmoothNormals)
			bool bProcess_GenSmoothNormals= false;
			// aiProcess_GenSmoothNormals: Generates smooth normals for all vertices in the mesh.
			//                             (...is ignored if normals are already there at the time this flag is evaluated.)
			//                             (can *NOT* be used with aiProcess_GenNormals)
			bool bProcess_SplitLargeMeshes = false;
			// aiProcess_SplitLargeMeshes: 	Splits large meshes into smaller sub - meshes.
			bool bProcess_PreTransformVertices = false;
			// aiProcess_PreTransformVertices: Removes the node graph and pre-transforms all vertices with the local transformation matrices of their nodes.
			bool bProcess_LimitBoneWeights = false;
			// aiProcess_LimitBoneWeights: Limits the number of bones simultaneously affecting a single vertex to a maximum value.
			bool bProcess_ValidateDataStructure = false;
			// aiProcess_ValidateDataStructure: Validates the imported scene data structure. This makes sure that all indices are valid, all animations 
			//                                  and bones are linked correctly, all material references are correct .. etc.
			//                                  It is recommended that you capture Assimp's log output if you use this flag, so you can 
			//                                  easily find out what's wrong if a file fails the validation. The validator is quite 
			//                                  strict and will find all inconsistencies in the data structure...
			bool bProcess_ImproveCacheLocality = false;
			// aiProcess_ImproveCacheLocality: Reorders triangles for better vertex cache locality.
			bool bProcess_RemoveRedundantMaterials = false;
			// aiProcess_RemoveRedundantMaterials: Searches for redundant or unreferenced materials and removes them.
			bool bProcess_FixInfacingNormals= false;
			// aiProcess_FixInfacingNormals: This step tries to determine which meshes have normal vectors that are facing inwards 
			//                               and inverts them. The algorithm is simple but effective : the bounding box of all 
			//                               vertices + their normals is compared against the volume of the bounding box of all 
			//                               vertices without their normals.This works well for most objects, problems might occur 
			//                               with planar surfaces.However, the step tries to filter such cases.The step inverts 
			//                               all in-facing normals.Generally it is recommended to enable this step, although 
			//                               the result is not always correct.
			bool bProcess_SortByPType = false;
			// aiProcess_SortByPType: This step splits meshes with more than one primitive type in homogeneous sub-meshes.
			//                        The step is executed after the triangulation step.
			bool bProcess_FindDegenerates = false;
			// aiProcess_FindDegenerates: This step searches all meshes for degenerate primitives and converts them to proper lines or points.
			bool bProcess_FindInvalidData = true;
			// aiProcess_FindInvalidData: This step searches all meshes for invalid data, such as zeroed normal vectors or invalid UV coords 
			//                            and removes or fixes them. This is intended to get rid of some common exporter errors.
			//                            This is especially useful for normals. If they are invalid, and the step recognizes this, 
			//                            they will be removed and can later be recomputed, i.e.by the aiProcess_GenSmoothNormals flag.
			bool bProcess_GenUVCoords = false;
			// aiProcess_GenUVCoords: This step converts non-UV mappings (such as spherical or cylindrical mapping) to proper texture coordinate channels.
			bool bProcess_TransformUVCoords = false;
			// aiProcess_TransformUVCoords: This step applies per-texture UV transformations and bakes them into stand-alone vtexture coordinate channels.
			bool bProcess_FindInstances = false;
			// aiProcess_FindInstances: This step searches for duplicate meshes and replaces them with references to the first mesh.
			bool bProcess_OptimizeMeshes = false;
			// aiProcess_OptimizeMeshes: A postprocessing step to reduce the number of meshes.
			bool bProcess_OptimizeGraph = false;
			// aiProcess_OptimizeGraph: A postprocessing step to optimize the scene hierarchy. 
			//                          Nodes without animations, bones, lights or cameras assigned are collapsed and joined.
			//                          Use this flag with caution.
			bool bProcess_FlipUVs = false;
			// aiProcess_FlipUVs: This step flips all UV coordinates along the y-axis and adjusts material settings and bitangents accordingly.
			bool bProcess_FlipWindingOrder = false;
			// aiProcess_FlipWindingOrder: This step adjusts the output face winding order to be CW.
			bool bProcess_SplitByBoneCount = false;
			// aiProcess_SplitByBoneCount: This step splits meshes with many bones into sub-meshes so that each su-bmesh has fewer or as many bones as a given limit.
			bool bProcess_Debone = false;
			// aiProcess_Debone:: This step removes bones losslessly or according to some threshold.
		};

		cFileLoader();
		~cFileLoader();

		// Uses the default sPostProcessFlags settings
		bool Load3DModelFile(std::string filename);
		bool Load3DModelFile(std::string filename, sPostProcessFlags postProcessOptions);
		void SetBasePath(std::string basepath_no_end_slash);

		cString getLastError(bool bAndClearErrors = true);

	//	void GetVertexPosistionsXYZ(std::vector< glm::vec3 >& vecPositions);
	//	void GetVertexNormalsXYZ(std::vector< glm::vec3 >& vecPositions);
	//	void GetVertexColorusRGBA(std::vector< glm::vec3 >& vecPositions);
	//	void GetVertexTextureCoords(std::vector< glm::vec3 >& vecPositions);

	//	void LetsDoThis(std::string message);
	private:
		// The actual implementation class:
		// Note that this HAS to be a pointer.
		// If it wasn't, then the compiler would need to know the size, 
		//	and we'd have to include the actual header.
		// (Note we AREN'T doing this - we are only doing a forward declaration)
		// It's OK with the forward declaration because we're using a pointer, 
		//	and it knows how large a pointer is (it's just an int).
		cFileLoader_Imp* m_pImp;
	};

};//namespace ah