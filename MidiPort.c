#include <MIDI.h>#define					ZeroValue			(long)0#define					ZeroPointer			(long)0#define					ZeroHandle			(long)0#define					ManagerMemory		0x0400#define					AppIconType			'ICN#'#define					AppIconID			128#define					ManagerPortID		'QTgm'#define					ManagerInputID		'QTim'#define					ManagerPortName		"\pQuickMIDI"#define					ManagerTypeName		"\pIn"#define					PortResType			'PORt'#define					PortResID			128//GlobalRoutinesOSErr					SetupMIDIPort		(void);void					ResetMIDIPort		(void);void					LoadPortConnection	(void);void					SavePortConnection	(void);//ExternRoutinesextern	pascal	short	ReadRoutine			(MIDIPacketPtr,long);extern			void	CopyStringByteX		(register unsigned char*,register unsigned char*);//StaticMemoriesstatic			short	MIDIPortRefNum;static			Handle	AppIcon;OSErr SetupMIDIPort(void){	MIDIPortParams		ParamMIDI;	OSErr				Result;		Result = noErr;		if((AppIcon = Get1Resource(AppIconType,AppIconID)) != ZeroHandle){		DetachResource(AppIcon);		HLockHi(AppIcon);		if((Result = MIDISignIn(ManagerPortID,ZeroValue,AppIcon,ManagerPortName)) == noErr){			ParamMIDI.portID = ManagerInputID;			ParamMIDI.portType = midiPortTypeInput;			ParamMIDI.timeBase = 0;			ParamMIDI.offsetTime = 0;			ParamMIDI.readHook = (MIDIReadHookProcPtr)&ReadRoutine;			ParamMIDI.refCon = SetCurrentA5();			ParamMIDI.initClock.syncType = 0;			ParamMIDI.initClock.curTime = 0;			ParamMIDI.initClock.format = 0;			CopyStringByteX(ManagerTypeName,ParamMIDI.name);			if((Result = MIDIAddPort(ManagerPortID,ManagerMemory,&MIDIPortRefNum,&ParamMIDI)) != noErr) MIDISignOut(ManagerPortID);		}		if(Result != noErr){			HUnlock(AppIcon);			DisposeHandle(AppIcon);		}	}	else Result = ResError();	return(Result);}/*SetupMIDIPort*/void ResetMIDIPort(void){	MIDIRemovePort(MIDIPortRefNum);	MIDISignOut(ManagerPortID);	HUnlock(AppIcon);	DisposeHandle(AppIcon);	return;}/*ResetMIDIPort*/void LoadPortConnection(void){	MIDIPortInfoHdl		PortInfo;	short				ID;	if((PortInfo = (MIDIPortInfoHdl)Get1Resource(PortResType,PortResID)) != ZeroHandle){		for(ID=0;ID<(*PortInfo)->numConnects;ID++){			MIDIConnectData(ManagerPortID,ManagerInputID,(*PortInfo)->cList[ID].clientID,(*PortInfo)->cList[ID].portID);		}		ReleaseResource((Handle)PortInfo);	}		return;}/*LoadPortConnection*/void SavePortConnection(void){	MIDIPortInfoHdl		PortInfo;	Handle				Rsrc;		if((PortInfo = MIDIGetPortInfo(ManagerPortID,ManagerInputID)) != ZeroHandle){		if((Rsrc = Get1Resource(PortResType,PortResID)) == ZeroHandle){			Rsrc = NewHandleClear(0);			AddResource(Rsrc,PortResType,PortResID,"\p");		}		SetHandleSize(Rsrc,GetHandleSize((Handle)PortInfo));		HLock(Rsrc);		HLock((Handle)PortInfo);		BlockMove(*PortInfo,*Rsrc,GetHandleSize((Handle)PortInfo));		HUnlock((Handle)PortInfo);		HUnlock(Rsrc);		ChangedResource(Rsrc);		ReleaseResource(Rsrc);		DisposeHandle((Handle)PortInfo);	}	return;}/*SavePortConnection*/