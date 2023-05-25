#pragma once

#include <string>
#include <vector>

// Forward declaration of the implementation class
class cAssimpHelper_Imp;

class __declspec(dllexport) cMesh
{
	cMesh() {};
	GetNumColorChannels () const
		Get the number of vertex color channels the mesh contains.
		unsigned int 	GetNumUVChannels () const
		Get the number of UV channels the mesh contains.
		bool 	HasBones () const
		Check whether the mesh contains bones.
		bool 	HasFaces () const
		Check whether the mesh contains faces.
		bool 	HasNormals () const
		Check whether the mesh contains normal vectors.
		bool 	HasPositions () const
		Check whether the mesh contains positions.
		bool 	HasTangentsAndBitangents () const
		Check whether the mesh contains tangent and bitangent vectors It is not possible that it contains tangents and no bitangents (or the other way round).
		bool 	HasTextureCoords (unsigned int pIndex) const
		Check whether the mesh contains a texture coordinate set.
		bool 	HasVertexColors (unsigned int pIndex) const
		Check whether the mesh contains a vertex color set.

};

class __declspec(dllexport) cAssimpHelper
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

	cAssimpHelper();
	~cAssimpHelper();

	// Uses the default sPostProcessFlags settings
	bool Load3DModelFile(std::string filename);
	bool Load3DModelFile(std::string filename, sPostProcessFlags postProcessOptions);
	void SetBasePath(std::string basepath_no_end_slash);

	std::string getLastError(bool bAndClearErrors = true);

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
	cAssimpHelper_Imp* m_pImp;
};
