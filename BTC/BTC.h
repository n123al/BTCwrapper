// BTC.h

#pragma once

#include "BTCHelper.h"
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

namespace BTC {

	typedef  IntPtr (WINAPI* newSkeleton)(int* start, int* End , array<IntPtr>^ mseleton,int mskeletonid);
	typedef  IntPtr (WINAPI* newVoice)(int* start, int* End , char *Voice ,int mskeletonid);
	typedef  void (WINAPI* Release)(void);
	typedef  bool (WINAPI* addGesture)(BTCGestureNative* myGesture, byte replace);
	typedef  bool (WINAPI* addPose)(array<IntPtr>^ mseleton, array<byte>^ Relations, array<double>^ Accuracy, char * PoseId, char * value, byte replace);
	typedef  bool (WINAPI* updatePose)(array<byte>^ Relations, array<double>^ Accuracy, char * PoseId, char * value);
	typedef  BTCPoseNative* (WINAPI* getPose)(char * PoseID);
	typedef  BTCGestureNative* (WINAPI* getGesture)(char * GestureID);
	typedef  bool (WINAPI* loadPosesList)(char* Path, byte newList,char* Key);
	typedef  bool (WINAPI* loadGesturesList)(char* Path, byte newList,char* Key);
	typedef  bool (WINAPI* savePosesList)(char* Path,char* Key);
	typedef  bool (WINAPI* saveGesturesList)(char* Path,char* Key);
	typedef  IntPtr (WINAPI* getGestureList) (int * NumOfGesture);
	typedef  IntPtr (WINAPI* getPoseList) (int * NumOfPose);
	typedef  bool (WINAPI* deleteGesture)(char * GestureID);
	typedef  bool (WINAPI* deletePose)(char * PoseID);

	public ref class BTCWraper
	{
	protected:
		String^ path;
		HINSTANCE dllHandle;
		BOOL freeResult, runTimeLinkSuccess;
		//BTC functions
		newSkeleton addNewSkeleton;
		newVoice addNewVoice;
		addPose addNewPose;
		updatePose updateExPose;
		addGesture addNewGesture;
		getGesture getExGesture;
		getPose getExPose;
		Release btcRelease;
		loadPosesList LoadPosesListFun;
		savePosesList SavePosesListFun;
		loadGesturesList LoadGesturesListFun;
		saveGesturesList SaveGesturesListFun;
		getGestureList GetGestureListFun;
		getPoseList GetPoseListFun;
		deleteGesture DeleteGestureFun;
		deletePose DeletePoseFun;

	public:
		BTCWraper(void){
			//GetBTCHelperPtr GetBTCHelper;


			runTimeLinkSuccess = false;
			if (NULL == dllHandle) dllHandle = LoadLibrary( L"libBeTheController.dll");

			if (NULL != dllHandle) {
				addNewSkeleton = (newSkeleton)GetProcAddress(dllHandle, "NewSkeleton");
				addNewVoice = (newVoice)GetProcAddress(dllHandle, "NewVoice");
				btcRelease =(Release)GetProcAddress(dllHandle, "Release");
				addNewGesture =(addGesture)GetProcAddress(dllHandle, "AddGesture");
				getExGesture = (getGesture)GetProcAddress(dllHandle, "GetGesture");
				getExPose =(getPose)GetProcAddress(dllHandle, "GetPose");
				addNewPose =(addPose)GetProcAddress(dllHandle, "AddPose");
				updateExPose =(updatePose)GetProcAddress(dllHandle, "UpdatePose");
				LoadPosesListFun =(loadPosesList)GetProcAddress(dllHandle, "LoadPosesList");
				LoadGesturesListFun =(loadGesturesList)GetProcAddress(dllHandle, "LoadGesturesList");
				SavePosesListFun =(savePosesList)GetProcAddress(dllHandle, "SavePosesList");
				SaveGesturesListFun =(saveGesturesList)GetProcAddress(dllHandle, "SaveGesturesList");
				GetGestureListFun = (getGestureList)GetProcAddress(dllHandle, "GetGestureList");
				GetPoseListFun = (getPoseList)GetProcAddress(dllHandle, "GetPoseList");
				DeleteGestureFun = (deleteGesture)GetProcAddress(dllHandle, "DeleteGesture");
				DeletePoseFun = (deletePose)GetProcAddress(dllHandle, "DeletePose");

				runTimeLinkSuccess = (NULL != DeleteGestureFun) && (NULL != addNewVoice) && (NULL != GetPoseListFun) && (NULL != GetGestureListFun) && (NULL != addNewSkeleton) && (NULL != btcRelease) && (NULL != addNewGesture)&& (NULL != LoadGesturesListFun)&& (NULL != LoadPosesListFun)&& (NULL != SavePosesListFun)&& (NULL != SaveGesturesListFun);
			}
		}

		~BTCWraper(){
			if (NULL != dllHandle) {
				if(runTimeLinkSuccess) btcRelease();
				freeResult = FreeLibrary(dllHandle);  
				runTimeLinkSuccess = false;
				dllHandle = NULL;
			}
		}
#pragma region WrapedFunction
		//Wrap class add new Gesture
		bool AddGesture(BTCGesture^ myGesture, bool replace ){
			if (runTimeLinkSuccess){
				BTCGestureNative* tmp=  new BTCGestureNative(myGesture);
				byte rep =replace;
				bool rt =addNewGesture(tmp,rep);
				tmp->~BTCGestureNative();
				return rt;
			}
			return false;
		}

		//Wrap function DeleteGesture
		bool DeleteGesture(String^ GestureID){	
			if (runTimeLinkSuccess){
			char* id =(char *)(void *)Marshal::StringToHGlobalAnsi(GestureID);
			return DeleteGestureFun(id);
			}
		}

		//Wrap class get Gesture
		BTCGesture^ GetGesture(String^ GestureID){
			if (runTimeLinkSuccess){
				char* id =(char *)(void *)Marshal::StringToHGlobalAnsi(GestureID);
				BTCGestureNative* temp = getExGesture(id);
				Marshal::FreeHGlobal((IntPtr) id);
				if(temp==NULL) return nullptr;
				BTCGesture^ result = gcnew BTCGesture();

				result->value = Marshal::PtrToStringAnsi((IntPtr)temp->value);
				result->ID= Marshal::PtrToStringAnsi((IntPtr)temp->ID);
				result->StartPose= Marshal::PtrToStringAnsi((IntPtr)temp->StartPose);
				result->EndPose= Marshal::PtrToStringAnsi((IntPtr)temp->EndPose);
				result->Voice= Marshal::PtrToStringAnsi((IntPtr)temp->Voice);

				result->EndPoseHold = *((double^)(Marshal::PtrToStructure(temp->EndPoseHold,double::typeid)));
				result->FoundEnd= *((byte^)(Marshal::PtrToStructure(temp->FoundEnd,byte::typeid)));
				result->FoundStart= *((byte^)(Marshal::PtrToStructure(temp->FoundStart,byte::typeid)));
				result->FoundVoice= *((byte^)(Marshal::PtrToStructure(temp->FoundVoice,byte::typeid)));
				result->MaxDuration= *((float^)(Marshal::PtrToStructure(temp->MaxDuration,float::typeid)));
				return result;
			}
			return nullptr;
		}

		//Wrap function PoseGesture
		bool DeletePose(String^ PoseID){	
			if (runTimeLinkSuccess){
			char* id =(char *)(void *)Marshal::StringToHGlobalAnsi(PoseID);
			return DeletePoseFun(id);
			}
		}

		//wrap function get Pose
		BTCPose^ GetPose(String^ PoseID){
			if (runTimeLinkSuccess){
				char* id =(char *)(void *)Marshal::StringToHGlobalAnsi(PoseID);
				BTCPoseNative* result = getExPose(id);
				Marshal::FreeHGlobal((IntPtr) id);
				if(result==NULL) return nullptr;
				BTCPose^ FPose =gcnew BTCPose();
				FPose->ID =Marshal::PtrToStringAnsi((IntPtr)result->ID);
				FPose->value =Marshal::PtrToStringAnsi((IntPtr)result->value);
				array<IntPtr>^ accuracy = gcnew array<IntPtr>((int)RelationsID::RelationsCount);
				array<IntPtr>^ relations = gcnew array<IntPtr>((int)RelationsID::RelationsCount);
				Marshal::Copy(result->Accuracy,accuracy,0,(int)RelationsID::RelationsCount);
				Marshal::Copy(result->Relations,relations,0,(int)RelationsID::RelationsCount);
				for (int i = 0; i <(int)RelationsID::RelationsCount; i++)
				{
					FPose->Accuracy[i] =*((double^)(Marshal::PtrToStructure(accuracy[i],double::typeid)));
					FPose->Relations[i] =*((BOOLEAN^)(Marshal::PtrToStructure(relations[i],BOOLEAN::typeid)));
				}
				return FPose;
			}
			return nullptr;
		}

		//wrap function Update Pose
		bool UpdatePose(BTCPose^ ppose){
		return UpdatePose(ppose->Relations ,ppose->Accuracy,ppose->ID,ppose->value);
		}
		
		bool UpdatePose(array<bool>^ Relations, array<double>^ Accuracy, String^ PoseID,String^ Value ){
			if (runTimeLinkSuccess){	
				char* id =(char *)(void *)Marshal::StringToHGlobalAnsi(PoseID);
				char* value =(char *)(void *)Marshal::StringToHGlobalAnsi(Value);
				array<double>^ accuracy = gcnew array<double>((int)RelationsID::RelationsCount);
				for(int i=0;i<(int)RelationsID::RelationsCount;i++)accuracy[i]=Accuracy[i];
				array<byte>^ relations= gcnew array<byte>((int)RelationsID::RelationsCount);
				for(int i=0;i<(int)RelationsID::RelationsCount;i++)relations[i]=Relations[i];
				bool result = updateExPose(relations,accuracy,id,value);
				Marshal::FreeHGlobal((IntPtr) id);
				Marshal::FreeHGlobal((IntPtr) value);
				delete[] relations;
				delete[] accuracy;
				return result;
			}
			return false;
		}

		//wrap function add Pose
		bool AddPose(array<Vector3f^>^skeleton ,BTCPose^ ppose,bool replace ){
		return AddPose(skeleton,ppose->Relations ,ppose->Accuracy,ppose->ID,ppose->value,replace);
		}

		bool AddPose(array<Vector3f^>^ skeleton,array<bool>^ Relations, array<double>^ Accuracy, String^ PoseID,String^ Value, bool replace ){
			if (runTimeLinkSuccess){	
				array<IntPtr>^ mskeleton = gcnew array<IntPtr>((int)JointID::JointCount);
				for(int i =(int)JointID::HipCenter;i<(int)JointID::JointCount;i++)
				{
					mskeleton[i] =Marshal::AllocHGlobal(Marshal::SizeOf(Vector3f::typeid));
					Marshal::StructureToPtr(skeleton[i],mskeleton[i],false);
				}
				char* id =(char *)(void *)Marshal::StringToHGlobalAnsi(PoseID);
				char* value =(char *)(void *)Marshal::StringToHGlobalAnsi(Value);
				array<double>^ accuracy = gcnew array<double>((int)RelationsID::RelationsCount);
				for(int i=0;i<(int)RelationsID::RelationsCount;i++)accuracy[i]=Accuracy[i];
				array<byte>^ relations= gcnew array<byte>((int)RelationsID::RelationsCount);
				for(int i=0;i<(int)RelationsID::RelationsCount;i++)relations[i]=Relations[i];
				byte rep =replace;
				bool result = addNewPose(mskeleton,relations,accuracy,id,value,rep);
				Marshal::FreeHGlobal((IntPtr) id);
				Marshal::FreeHGlobal((IntPtr) value);
				delete[] relations;
				delete[] accuracy;
				return result;
			}
			return false;
		}

		//Wrap class add new skeleton
		BTCResults^ NewSkeleton(array<Vector3f^>^ skeleton,int skeletonID ){

			BTCResults^ res = gcnew BTCResults();

			if (runTimeLinkSuccess){
				int newe,ende;

				array<IntPtr>^ mskeleton = gcnew array<IntPtr>((int)JointID::JointCount);

				for(int i =(int)JointID::HipCenter;i<(int)JointID::JointCount;i++){
					mskeleton[i] =Marshal::AllocHGlobal(Marshal::SizeOf(Vector3f::typeid));
					Marshal::StructureToPtr(skeleton[i],mskeleton[i],false);
				}
				IntPtr ddd = addNewSkeleton(&newe,&ende,mskeleton,skeletonID);

				array<IntPtr>^result =gcnew array<IntPtr>(2);


				Marshal::Copy(ddd, result, 0, 2);

				if(result[0] != IntPtr::Zero){
					array<IntPtr>^Nevents = gcnew array<IntPtr>(newe);
					Marshal::Copy(result[0], Nevents, 0, newe);
					for (int i = 0; i < newe; i++)
					{
						KinectEvent^ obj = dynamic_cast<KinectEvent^>(Marshal::PtrToStructure(Nevents[i], KinectEvent::typeid));
						res->NewEvents->Add(obj);
					}
					delete[] Nevents;
				}
				if(result[1] != IntPtr::Zero){
					array<IntPtr>^Eevents = gcnew array<IntPtr>(ende);
					Marshal::Copy(result[1], Eevents, 0, ende);
					for (int i = 0; i < ende; i++)
					{
						KinectEvent^ obj = dynamic_cast<KinectEvent^>(Marshal::PtrToStructure(Eevents[i], KinectEvent::typeid));
						res->EndEvents->Add(obj);
					}
					delete[] Eevents;
				}
				for(int i =(int)JointID::HipCenter;i<(int)JointID::JointCount;i++) Marshal::FreeHGlobal(mskeleton[i]);
				delete[] mskeleton;
				delete[] result;
			}
			//	res->NewEvents=nullptr;
			//  res->EndEvents=nullptr;
			return res;
		}


		//Wrap Class add new Voice
		BTCResults^ NewVoice(String^ Voice, int skeletonID ){

			BTCResults^ res = gcnew BTCResults();
			char* voice =(char *)(void *)Marshal::StringToHGlobalAnsi(Voice);

			if (runTimeLinkSuccess){
				int newe,ende;

				IntPtr ddd = addNewVoice(&newe,&ende,voice,skeletonID);

				array<IntPtr>^result =gcnew array<IntPtr>(2);


				Marshal::Copy(ddd, result, 0, 2);

				if(result[0] != IntPtr::Zero){
					array<IntPtr>^Nevents = gcnew array<IntPtr>(newe);
					Marshal::Copy(result[0], Nevents, 0, newe);
					for (int i = 0; i < newe; i++)
					{
						KinectEvent^ obj = dynamic_cast<KinectEvent^>(Marshal::PtrToStructure(Nevents[i], KinectEvent::typeid));
						res->NewEvents->Add(obj);
					}
					delete[] Nevents;
				}
				if(result[1] != IntPtr::Zero){
					array<IntPtr>^Eevents = gcnew array<IntPtr>(ende);
					Marshal::Copy(result[1], Eevents, 0, ende);
					for (int i = 0; i < ende; i++)
					{
						KinectEvent^ obj = dynamic_cast<KinectEvent^>(Marshal::PtrToStructure(Eevents[i], KinectEvent::typeid));
						res->EndEvents->Add(obj);
					}
					delete[] Eevents;
				}
				Marshal::FreeHGlobal((IntPtr) voice);
				delete[] result;
			}
			//	res->NewEvents=nullptr;
			//  res->EndEvents=nullptr;
			return res;
		}

		//Wrap function LoadPosesList
		bool LoadPosesList(String^ Path, bool newList, String^ Key){
			bool result = false;
			if (runTimeLinkSuccess){	
				char * path = (char *)(void *)Marshal::StringToHGlobalAnsi(Path);
				char * key = (char *)(void *)Marshal::StringToHGlobalAnsi(Key);
				byte newlist = newList;
				result = LoadPosesListFun(path,newlist,key);
			}
			return result;
		}

		//Wrap function LoadGesturesList
		bool LoadGesturesList(String^ Path, bool newList, String^ Key){
			bool result = false;
			if (runTimeLinkSuccess){	
				char * path = (char *)(void *)Marshal::StringToHGlobalAnsi(Path);
				char * key = (char *)(void *)Marshal::StringToHGlobalAnsi(Key);
				byte newlist = newList;
				result = LoadGesturesListFun(path,newlist,key);
			}
			return result;
		}

		//Wrap function SavePosesList
		bool SavePosesList(String^ Path, String^ Key){
			bool result = false;
			if (runTimeLinkSuccess){	
				char * path = (char *)(void *)Marshal::StringToHGlobalAnsi(Path);
				char * key = (char *)(void *)Marshal::StringToHGlobalAnsi(Key);
				result = SavePosesListFun(path,key);
			}
			return result;
		}

		//Wrap function SaveGesturesList
		bool SaveGesturesList(String^ Path, String^ Key){
			bool result = false;
			if (runTimeLinkSuccess){	
				char * path = (char *)(void *)Marshal::StringToHGlobalAnsi(Path);
				char * key = (char *)(void *)Marshal::StringToHGlobalAnsi(Key);
				result = SaveGesturesListFun(path,key);
			}
			return result;
		}

		//Wrap get Gesture list
		List<String^>^ GetGestureList(){
			int NGesture; 
			List<String^>^ temp = gcnew List<String^>();
			IntPtr ddd = GetGestureListFun(&NGesture);
			array<IntPtr>^ result = gcnew array<IntPtr>(NGesture);
			Marshal::Copy(ddd, result, 0, NGesture);
			if(result != nullptr){
				for (int i = 0; i < NGesture; i++)
				{
					String^ obj =  Marshal::PtrToStringAnsi(result[i]);
					temp->Add(obj);
				}
				delete[] result;
			}
			return temp;
		}

		//Wrap get Gesture list
		List<String^>^ GetPoseList(){
			int NPose; 
			List<String^>^ temp = gcnew List<String^>();
			IntPtr ddd = GetPoseListFun(&NPose);
			array<IntPtr>^ result = gcnew array<IntPtr>(NPose);
			Marshal::Copy(ddd, result, 0, NPose);
			if(result != nullptr){
				for (int i = 0; i < NPose; i++)
				{
					String^ obj =  Marshal::PtrToStringAnsi(result[i]);
					temp->Add(obj);
				}
				delete[] result;
			}
			return temp;
		}

#pragma endregion
	};
}
