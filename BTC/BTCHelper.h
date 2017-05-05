
#ifndef BTCHELPER_H
#define	BTCHELPER_H

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;


namespace BTC {

public enum  class KEvents {
            Pose = 0,
            Gesture = 1,
            Spech = 2,
        };

public  enum class RelationsID
       {
            ChestFrontSlope = 0,
            ForearmShouldersLeft = 1,
            ArmForearmLeft = 2,
            ArmChestLeft = 3,
            ChestRotation = 4,
            ChestSideSlope = 5,
            ThingAbdomenLeft = 6,
            ThighLegLeft = 7,
            ThighRotationLeft = 8,
            ArmShoulderLeft = 9,
            LegAbdomenLeft = 10,
            ForearmShouldersRight = 11,
            ArmForearmRight = 12,
            ArmChestRight = 13,
            ArmShoulderRight = 14,
            ThingAbdomenRight = 15,
            ThighLegRight = 16,
            ThighRotationRight = 17,
            LegAbdomenRight = 18,
            RelationsCount = 19,
        };

public enum class JointID
       {
            HipCenter = 0,
            Spine = 1,
            ShoulderCenter = 2,
            Head = 3,
            ShoulderLeft = 4,
            ElbowLeft = 5,
            WristLeft = 6,
            HandLeft = 7,
            ShoulderRight = 8,
            ElbowRight = 9,
            WristRight = 10,
            HandRight = 11,
            HipLeft = 12,
            KneeLeft = 13,
            AnkleLeft = 14,
            FootLeft = 15,
            HipRight = 16,
            KneeRight = 17,
            AnkleRight = 18,
            FootRight = 19,
            JointCount = 20,
        };

[StructLayout(LayoutKind::Sequential, Pack=8)]
public ref struct Vector3f {
 public:
	float x,y,z;
    };

[StructLayout(LayoutKind::Sequential, Pack=8)]
public ref struct BTCPose{
        String^ ID;
        String^ value;
        array<bool>^ Relations;
        array<double>^ Accuracy;
		BTCPose(){
		Relations = gcnew array<bool>((int)RelationsID::RelationsCount);
		Accuracy = gcnew array<double>((int)RelationsID::RelationsCount);
		}
		~BTCPose(){
		delete[] Relations;
		delete[] Accuracy;
		}
};

private class BTCPoseNative{
public:
        char * ID;
        char * value;
		IntPtr Relations;
        IntPtr Accuracy;
};

public ref struct BTCGesture
    { 
	String^ value;
	String^ ID;
    String^ StartPose;
    bool FoundStart;
    String^ EndPose;
    double EndPoseHold;
    bool FoundEnd;
    String^ Voice;
    bool FoundVoice;
    float MaxDuration;
	BTCGesture(){
		StartPose= String::Empty;
		EndPose= String::Empty;
		Voice= String::Empty;
		value = String::Empty;
		ID = String::Empty;
	}
    };

private class BTCGestureNative
    { 
	//fields
	public:
	char * ID;
	char * value;
    char * StartPose;
    IntPtr FoundStart;
    char * EndPose;
    IntPtr EndPoseHold;
	IntPtr FoundEnd;
    char* Voice;
    IntPtr FoundVoice;
    IntPtr MaxDuration;

	//Constractor
	BTCGestureNative(BTCGesture^ tmp){
	value = (char *)(void *)Marshal::StringToHGlobalAnsi(tmp->value);
	ID = (char *)(void *)Marshal::StringToHGlobalAnsi(tmp->ID);
	StartPose = (char *)(void *)Marshal::StringToHGlobalAnsi(tmp->StartPose);
	EndPose = (char *)(void *)Marshal::StringToHGlobalAnsi(tmp->EndPose);
	Voice = (char *)(void *)Marshal::StringToHGlobalAnsi(tmp->Voice);
	EndPoseHold =  Marshal::AllocHGlobal(Marshal::SizeOf(double::typeid));
	Marshal::StructureToPtr(tmp->EndPoseHold,(IntPtr)EndPoseHold,false);
	FoundStart =  Marshal::AllocHGlobal(Marshal::SizeOf(bool::typeid));
	Marshal::StructureToPtr(tmp->FoundStart,(IntPtr)FoundStart,false);
	FoundEnd =  Marshal::AllocHGlobal(Marshal::SizeOf(bool::typeid));
	Marshal::StructureToPtr(tmp->FoundEnd,(IntPtr)FoundEnd,false);
	FoundVoice =  Marshal::AllocHGlobal(Marshal::SizeOf(bool::typeid));
	Marshal::StructureToPtr(tmp->FoundVoice,(IntPtr)FoundVoice,false);
	MaxDuration = Marshal::AllocHGlobal(Marshal::SizeOf(float::typeid));
	Marshal::StructureToPtr(tmp->MaxDuration,(IntPtr)MaxDuration,false);
	}

	//Distractor
	~BTCGestureNative(){
		
		Marshal::FreeHGlobal((IntPtr) value);
		Marshal::FreeHGlobal((IntPtr) ID);
		Marshal::FreeHGlobal((IntPtr) StartPose);
		Marshal::FreeHGlobal((IntPtr) EndPose);
		Marshal::FreeHGlobal((IntPtr) Voice);

		Marshal::FreeHGlobal( EndPoseHold);
		Marshal::FreeHGlobal(FoundStart);
		Marshal::FreeHGlobal(FoundEnd);
		Marshal::FreeHGlobal(FoundVoice);
		Marshal::FreeHGlobal(MaxDuration);
	}
};

[StructLayout(LayoutKind::Sequential, Pack=8)]
public ref struct KinectEvent
{
        KEvents Type;
        String^ Value;
        String^ ID;
}; 

public ref struct BTCResults
{
public:
	List<KinectEvent^>^ NewEvents;
	List<KinectEvent^>^ EndEvents;
	BTCResults(){
	NewEvents= gcnew List<KinectEvent^>();
	EndEvents= gcnew List<KinectEvent^>();
	}
	~BTCResults(){
	for(int i=0;i<NewEvents->Count;i++)delete NewEvents[i];
	delete[]NewEvents;
		for(int i=0;i<EndEvents->Count;i++)delete EndEvents[i];
	delete[]EndEvents;
	}
};

}
#endif