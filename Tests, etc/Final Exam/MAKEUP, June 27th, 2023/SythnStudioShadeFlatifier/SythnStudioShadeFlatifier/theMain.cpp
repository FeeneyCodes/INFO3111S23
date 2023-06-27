#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "Ply_File_Loader/CPlyFile5nt.h"
#include "Ply_File_Loader/CStringHelper.h"

int main(int argc, char* argv[])
{
	// Get the list of files.
	// Ignore the 1st command line param
	std::vector<std::string> vecFileList;
	for (int index = 1; index != argc; index++ )
	{
		// F:\z_FS\3111_3D\2023\Local_SAM\CPXX (Bonus)\PLY\SM_Env_Basement_Ceiling_01.ply
		// C:\z_FS\3111_3D\2023\Local_SAM\CPXX (Bonus)\PLY\SM_Env_Basement_Ceiling_01.ply
		// 
		// Is this a ply file?
		std::string curParam(argv[index]);
//		std::cout << curParam << std::endl;
		std::string extension = curParam.substr(curParam.length() - 3, 3);
		std::transform(extension.begin(), extension.end(), extension.begin(), ::toupper);
//		std::cout << extension << std::endl;
		if ( extension == "PLY" )
		{
//			std::cout << "It's a ply file" << std::endl;
			std::string fileNoExtension = curParam.substr(0, curParam.length() - 4);
			vecFileList.push_back(fileNoExtension);
		}
	}

	if ( vecFileList.empty() )
	{
		std::cout << "I didn't see any ply files." << std::endl;
		return -1;
	}





	for (std::string modelName : vecFileList)
	{
		CPlyFile5nt* pPlyLoader = new CPlyFile5nt();

		std::wstring wFileToLoad = CStringHelper::ASCIIToUnicodeQnD(modelName);
		wFileToLoad.append(L".ply");

		std::wstring wErrors;
		if ( ! pPlyLoader->OpenPLYFile2(wFileToLoad, wErrors) )
		{
			std::wcout << "Error: Can't load >" << wFileToLoad << "< because: " << wErrors << std::endl;
			// 
			continue;
		}
		
		std::wcout << "Loaded " << wFileToLoad << "< : ";
		std::wcout << pPlyLoader->GetNumberOfVerticies() << " vertices, " << pPlyLoader->GetNumberOfElements() << " elements" << std::endl;

		// Now flatten it

		std::vector<PlyVertex> vecVerts;
		std::vector<PlyElement> vecTris;

		// Copy the triangles into the vecVerts;
		unsigned int vertexIndex = 0;
		for ( unsigned int triIndex = 0; triIndex != pPlyLoader->GetNumberOfElements(); triIndex++ )
		{
			PlyElement tempTri = pPlyLoader->getElement_at(triIndex);

			PlyVertex triVerts[3];
			triVerts[0] = pPlyLoader->getVertex_at(tempTri.vertex_index_1);
			triVerts[1] = pPlyLoader->getVertex_at(tempTri.vertex_index_2);
			triVerts[2] = pPlyLoader->getVertex_at(tempTri.vertex_index_3);

			// Recalculate the normals so it's flat
			CVector3f vert1 = triVerts[1].xyz - triVerts[0].xyz;
			CVector3f vert2 = triVerts[2].xyz - triVerts[0].xyz;
			CVector3f flatNormal = CVector3f::CrossProduct(vert1, vert2);

			triVerts[0].nx = triVerts[1].nx = triVerts[2].nx = flatNormal.x;
			triVerts[0].ny = triVerts[1].ny = triVerts[2].ny = flatNormal.y;
			triVerts[0].nz = triVerts[1].nz = triVerts[2].nz = flatNormal.z;


			// Push these back
			vecVerts.push_back(triVerts[0]);
			vecVerts.push_back(triVerts[1]);
			vecVerts.push_back(triVerts[2]);

			PlyElement tempTriFlat;
			tempTriFlat.vertex_index_1 = vertexIndex;
			tempTriFlat.vertex_index_2 = vertexIndex + 1;
			tempTriFlat.vertex_index_3 = vertexIndex + 2;

			vertexIndex += 3;

			vecTris.push_back(tempTriFlat);
		}

		CPlyFile5nt::sSaveFileParams fileSaveParams;
		fileSaveParams.vecComments.push_back(L"INFO-3111 Flatenator generated");
		fileSaveParams.vecComments.push_back(L"TextureFile Dungeons_Texture_01.png");

		{// XYZ only
			std::wstring wFileToSaveXYZ = CStringHelper::ASCIIToUnicodeQnD(modelName);
			wFileToSaveXYZ.append(L"_flatshaded_xyz.ply");

			fileSaveParams.bIncludeNormals = false;
			fileSaveParams.bInculdeRGBA = false;
			fileSaveParams.bIncludeUVs = false;

			if ( pPlyLoader->SavePlyFileASCII(wFileToSaveXYZ, true, fileSaveParams) )
			{
				std::wcout << wFileToSaveXYZ << " saved OK" << std::endl;
			}
		}

		{// XYZ + normal only
			std::wstring wFileToSaveXYZ = CStringHelper::ASCIIToUnicodeQnD(modelName);
			wFileToSaveXYZ.append(L"_flatshaded_xyz_n.ply");

			fileSaveParams.bIncludeNormals = true;
			fileSaveParams.bInculdeRGBA = false;
			fileSaveParams.bIncludeUVs = false;

			if (pPlyLoader->SavePlyFileASCII(wFileToSaveXYZ, true, fileSaveParams))
			{
				std::wcout << wFileToSaveXYZ << " saved OK" << std::endl;
			}
		}

		{// XYZ + normal + uv only
			std::wstring wFileToSaveXYZ = CStringHelper::ASCIIToUnicodeQnD(modelName);
			wFileToSaveXYZ.append(L"_flatshaded_xyz_n_uv.ply");

			fileSaveParams.bIncludeNormals = true;
			fileSaveParams.bInculdeRGBA = false;
			fileSaveParams.bIncludeUVs = true;

			if (pPlyLoader->SavePlyFileASCII(wFileToSaveXYZ, true, fileSaveParams))
			{
				std::wcout << wFileToSaveXYZ << " saved OK" << std::endl;
			}
		}

		{// XYZ + normal + RGBA + uv only
			std::wstring wFileToSaveXYZ = CStringHelper::ASCIIToUnicodeQnD(modelName);
			wFileToSaveXYZ.append(L"_flatshaded_xyz_n_rgba_uv.ply");

			fileSaveParams.bIncludeNormals = true;
			fileSaveParams.bInculdeRGBA = true;
			fileSaveParams.bIncludeUVs = true;

			if (pPlyLoader->SavePlyFileASCII(wFileToSaveXYZ, true, fileSaveParams))
			{
				std::wcout << wFileToSaveXYZ << " saved OK" << std::endl;
			}
		}

		delete pPlyLoader;

	}//for (...vecFileList)




	system("pause");

	return 0;
}
