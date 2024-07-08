/** gccDppConsole.cpp */

// gccDppConsole.cpp : Defines the entry point for the console application.
#include <iostream>
using namespace std; 
#include "ConsoleHelper.h"
#include "stringex.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define CLEAR_TERM "cls"
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#define _getch getchar
#define CLEAR_TERM "clear"
#endif



CConsoleHelper chdpp;					// DPP communications functions
bool bRunSpectrumTest = false;			// run spectrum test
bool bRunConfigurationTest = false;		// run configuration test
bool bHaveStatusResponse = false;		// have status response
bool bHaveConfigFromHW = false;			// have configuration from hardware

//extern "C" {
// connect to default dpp
//		CConsoleHelper::LibUsb_Connect_Default_DPP	// LibUsb connect to default DPP
void ConnectToDefaultDPP()
{
	cout << endl;
	cout << "Running DPP LibUsb tests from console..." << endl;
	cout << endl;
	cout << "\tConnecting to default LibUsb device..." << endl;
	if (chdpp.LibUsb_Connect_Default_DPP()) {
		cout << "\t\tLibUsb DPP device connected." << endl;
		cout << "\t\tLibUsb DPP devices present: "  << chdpp.LibUsb_NumDevices << endl;
	} else {
		cout << "\t\tLibUsb DPP device not connected." << endl;
		cout << "\t\tNo LibUsb DPP device present." << endl;
	}
}

// Get DPP Status
//		CConsoleHelper::LibUsb_isConnected						// check if DPP is connected
//		CConsoleHelper::LibUsb_SendCommand(XMTPT_SEND_STATUS)	// request status
//		CConsoleHelper::LibUsb_ReceiveData()					// parse the status
//		CConsoleHelper::DppStatusString							// display status string
void GetDppStatus()
{
	if (chdpp.LibUsb_isConnected) { // send and receive status
		cout << endl;
		cout << "\tRequesting Status..." << endl;
		if (chdpp.LibUsb_SendCommand(XMTPT_SEND_STATUS_MX2)) {	// request status
			cout << "\t\tStatus sent." << endl;
			cout << "\t\tReceiving status..." << endl;
			if (chdpp.LibUsb_ReceiveData()) {
				cout << "\t\t\tStatus received..." << endl;
				cout << chdpp.DppStatusString << endl;
				// bRunSpectrumTest = false; // set these back to true later on!! EDITS REQUIRED
				// bHaveStatusResponse = false; // set to true
				// bRunConfigurationTest = false; // set to true
			} else {
				cout << "\t\tError receiving status." << endl;
			}
		} else {
			cout << "\t\tError sending status." << endl;
		}
	} else {
		cout << "Device Not Connected" << endl;
	}
}


// Get Interlock Status
//		CConsoleHelper::LibUsb_isConnected						// check if DPP is connected
//		CConsoleHelper::LibUsb_SendCommand(XMTPT_SEND_TUBE_ILOCK_TABLE_MX2)	// request interlock status
//		CConsoleHelper::LibUsb_ReceiveData()					// parse the interlock status
//		CConsoleHelper::DppStatusString							// display interlock status string
void GetInterlockStatus()
{
	if (chdpp.LibUsb_isConnected) { // send and receive status
		cout << endl;
		cout << "\tRequesting Status..." << endl;
		if (chdpp.LibUsb_SendCommand(XMTPT_SEND_TUBE_ILOCK_TABLE_MX2)) {	// request status
			cout << "\t\tInterlock Status sent." << endl;
			cout << "\t\tReceiving Interlock Status..." << endl;
			if (chdpp.LibUsb_ReceiveData()) {
				cout << "\t\t\tInterlock Status received..." << endl;
				cout << chdpp.strTubeInterlockTable << endl;
				//cout << chdpp.DppStatusString << endl;
			} else {
				cout << "\t\tError receiving Interlock Status." << endl;
			}
		} else {
			cout << "\t\tError sending status." << endl;
		}
	}
}


void ReadHVCfg()
{
	cout << endl;
	cout << "\tRequesting HV and I Configuration..." << endl;
	// float sngHV;
	// float sngI;
	chdpp.ReadbackMX2_HVandI();
	if (chdpp.LibUsb_ReceiveData()) {
		cout << "\t\t\tReceiving Voltage and Current..." << endl;
		cout << "\t\t\tVoltage: " << chdpp.strHV << endl;
		cout << "\t\t\tCurrent: " << chdpp.strI << endl;
		//cout << chdpp.DppStatusString << endl;
	} else {
		cout << "\t\tError receiving Voltage and Current Status." << endl;
	}
}


// void ReadHVandI()
// {
// 	if (chdpp.LibUsb_isConnected) { // send and receive status
// 		cout << endl;
// 		cout << "\tRequesting HV and I..." << endl;
// 		chdpp.ReadbackMX2_HVandI();
// 		cout << "\t\tRequested." << endl;
// 		if (chdpp.LibUsb_ReceiveData()) {
// 			cout << "\t\t\tInterlock Status received..." << endl;
// 			cout << "\t\t\t\tHighVoltage"<< chdpp.strHV << endl;
// 			//cout << chdpp.DppStatusString << endl;
// 		} else {
// 			cout << "\t\tError receiving Interlock Status." << endl;
// 		}
// 	}
// }

void TurnHVOn()
{
	cout << "\t\t\tTurning Tube On Now" << endl;
	//CMSecTimer tmr;
	string stringHV;
	string stringI;
	//string strStatus;

	stringHV = "15";
	stringI = "15";
	
	//strStatus = FloatToString(sngHV,1) + "kV : " + FloatToString(sngI,1) + "uA";
	if (chdpp.LibUsb_isConnected) { // send and receive status
		chdpp.SendMX2_HVandI(stringHV, stringI);
		if (chdpp.LibUsb_ReceiveData()) {
			cout << "\t\t\tReceiving Acknowledgement Packet..." << endl;
			
		} else {
			cout << "\t\tError receiving Acknowledgement Packet . . ." << endl;
		}

		//bValueChanged = chdpp.ReadbackMX2_HVandI(&sngHV, &sngI);

		// m_pParentMX2->SendMX2_HVandI(strHV, strI);
		// tmr.msTimer(100);
		// bValueChanged = m_pParentMX2->ReadbackMX2_HVandI(&sngHV, &sngI);

		// strStatus = FloatToString(sngHV,1) + "kV : " + FloatToString(sngI,1) + "uA";
		// strStatus = FloatToString(sngHV, 1) + "kV" + " : " + FloatToString(sngI, 1) + "uA";
		// if (bValueChanged) {
		//     strHV = FloatToString(sngHV, 1);
		// 	DisplayDouble(IDC_SETHIGHVOLTAGECONTROLEDIT, sngHV, 1);
		//     strI = FloatToString(sngI, 1);
		// 	DisplayDouble(IDC_SETCURRENTCONTROLEDIT, sngI, 1);
		// }
	} else {
		cout << "Device not connected" << endl;
	}
}


void TurnHVOff()
{
	cout << "\t\t\tTurning Tube Off Now" << endl;
	float sngHV;
	float sngI;
	string stringHV;
	string stringI;
	bool bValueChanged;

	sngHV = 0.0;
	sngI = 0.0;
	stringHV = "0.0";
	stringI = "0.0";
	if (chdpp.LibUsb_isConnected) { // send and receive status
		chdpp.SendMX2_HVandI(stringHV, stringI);

		if (chdpp.LibUsb_ReceiveData()) {
			cout << "\t\t\tReceiving Acknowledgement Packet..." << endl;
		} else {
			cout << "\t\tError receiving Acknowledgement Packet . . ." << endl;
		}
	}
}


// void TurnHVOffOld()
// {
// 	if (chdpp.LibUsb_isConnected) { // send and receive status
// 		cout << endl;
// 		cout << "\tTurning Off..." << endl;

// 		string strHV = "0.0";
// 		string strI = "0.0";
// 		double dblHV;
// 		double dblI;
// 		string strCmd;
// 		stringex strfn;

// 		dblHV = atof(strHV.c_str());
// 		dblI = atof(strI.c_str());
// 		//strCmd = "HVSE=" + Trim(Val(strHV)) + ";CUSE=" + Trim(Val(strI)) + ";";
// 		strCmd = "HVSE=";
// 		strCmd += strfn.Format("%0.1f;", dblHV);
// 		strCmd += "CUSE=";
// 		strCmd += strfn.Format("%0.1f;", dblI);


// 		unsigned char DataOut[514];
// 		int MaxDataLen=514;
// 		int idxCh;
// 		int DataLen;
// 		DataLen = (int)strDataIn.length();
// 		if (DataLen > 0) {
// 			for(idxCh=0; idxCh < MaxDataLen; idxCh++) {
// 				if (idxCh < DataLen) {
// 					DataOut[idxCh] = (unsigned char)strDataIn[idxCh];
// 				} else {
// 					DataOut[idxCh] = 0;
// 				}
// 			}

// 			BOOL HaveBuffer;
// 			BOOL SentPkt;

// 			HaveBuffer = SndCmd.DP5_CMD_Data(DP5Proto.BufferOUT, XMTPT_TEXT_CONFIGURATION_MX2, DataOut);
// 			if (HaveBuffer) {
// 				SentPkt = DppWinUSB.SendPacketUSB(DP5Proto.BufferOUT, DppWinUSB.devInfo, DP5Proto.PacketIn);
// 				if (SentPkt) {
// 					RemCallParsePacket(DP5Proto.PacketIn);
// 					} else {
// 					MiniX2.STATUS_MNX.bUSBError = true;
// 				}
// 			}
// 		}
// 	}
// }




// Read Full DPP Configuration From Hardware			// request status before sending/receiving configurations
//		CONFIG_OPTIONS									// holds configuration command options
//		CConsoleHelper::CreateConfigOptions				// creates configuration options from last status read
//      CConsoleHelper::ClearConfigReadFormatFlags();	// clear configuration format flags, for cfg readback
//      CConsoleHelper::CfgReadBack = true;				// requesting general readback format
//		CConsoleHelper::LibUsb_SendCommand_Config		// send command with options
//		CConsoleHelper::LibUsb_ReceiveData()			// parse the configuration
//		CConsoleHelper::HwCfgReady						// config is ready
void ReadDppConfigurationFromHardware(bool bDisplayCfg)
{
	CONFIG_OPTIONS CfgOptions;
	if (bHaveStatusResponse && bRunConfigurationTest) {
		//test configuration functions
		// Set options for XMTPT_FULL_READ_CONFIG_PACKET
		chdpp.CreateConfigOptions(&CfgOptions, "", chdpp.DP5Stat, false);
		cout << endl;
		cout << "\tRequesting Full Configuration..." << endl;
		chdpp.ClearConfigReadFormatFlags();	// clear all flags, set flags only for specific readback properties
		//chdpp.DisplayCfg = false;	// DisplayCfg format overrides general readback format
		chdpp.CfgReadBack = true;	// requesting general readback format
		if (chdpp.LibUsb_SendCommand_Config(XMTPT_FULL_READ_CONFIG_PACKET, CfgOptions)) {	// request full configuration
			if (chdpp.LibUsb_ReceiveData()) {
				if (chdpp.HwCfgReady) {		// config is ready
					bHaveConfigFromHW = true;
					if (bDisplayCfg) {
						cout << "\t\t\tConfiguration Length: " << (unsigned int)chdpp.HwCfgDP5.length() << endl;
						cout << "\t================================================================" << endl;
						cout << chdpp.HwCfgDP5 << endl;
						cout << "\t================================================================" << endl;
						cout << "\t\t\tScroll up to see configuration settings." << endl;
						cout << "\t================================================================" << endl;
					} else {
						cout << "\t\tFull configuration received." << endl;
					}
				}
			}
		}
	}
}



// Display Preset Settings
//		CConsoleHelper::strPresetCmd	// preset mode
//		CConsoleHelper::strPresetVal	// preset setting
void DisplayPresets()
{
	if (bHaveConfigFromHW) {
		cout << "\t\t\tPreset Mode: " << chdpp.strPresetCmd << endl;
		cout << "\t\t\tPreset Settings: " << chdpp.strPresetVal << endl;
	}
}

// Display Preset Settings
//		CONFIG_OPTIONS								// holds configuration command options
//		CConsoleHelper::CreateConfigOptions			// creates configuration options from last status read
//		CConsoleHelper::HwCfgDP5Out					// preset setting
//		CConsoleHelper::LibUsb_SendCommand_Config	// send command with options
void SendPresetAcquisitionTime(string strPRET)
{
	CONFIG_OPTIONS CfgOptions;
	cout << "\tSetting Preset Acquisition Time..." << strPRET << endl;
	chdpp.CreateConfigOptions(&CfgOptions, "", chdpp.DP5Stat, false);
	CfgOptions.HwCfgDP5Out = strPRET;
	// send PresetAcquisitionTime string, bypass any filters, read back the mode and settings
	if (chdpp.LibUsb_SendCommand_Config(XMTPT_SEND_CONFIG_PACKET_EX, CfgOptions)) {
		ReadDppConfigurationFromHardware(false);	// read setting back
		DisplayPresets();							// display new presets
	} else {
		cout << "\t\tPreset Acquisition Time NOT SET" << strPRET << endl;
	}
}

// Acquire Spectrum
//		CConsoleHelper::LibUsb_SendCommand(XMTPT_DISABLE_MCA_MCS)		//disable for data/status clear
//		CConsoleHelper::LibUsb_SendCommand(XMTPT_SEND_CLEAR_SPECTRUM_STATUS)  //clear spectrum/status
//		CConsoleHelper::LibUsb_SendCommand(XMTPT_ENABLE_MCA_MCS);		// enabling MCA for spectrum acquisition
//		CConsoleHelper::LibUsb_SendCommand(XMTPT_SEND_SPECTRUM_STATUS)) // request spectrum+status
//		CConsoleHelper::LibUsb_ReceiveData()							// process spectrum and data
//		CConsoleHelper::ConsoleGraph()	(low resolution display)		// graph data on console with status
//		CConsoleHelper::LibUsb_SendCommand(XMTPT_DISABLE_MCA_MCS)		// disable mca after acquisition
void AcquireSpectrum()
{
	int MaxMCA = 11;
	bool bDisableMCA;

	//bRunSpectrumTest = false;		// disable test
	if (bRunSpectrumTest) {
		cout << "\tRunning spectrum test..." << endl;
		cout << "\t\tDisabling MCA for spectrum data/status clear." << endl;
		chdpp.LibUsb_SendCommand(XMTPT_DISABLE_MCA_MCS);
		Sleep(1000);
		cout << "\t\tClearing spectrum data/status." << endl;
		chdpp.LibUsb_SendCommand(XMTPT_SEND_CLEAR_SPECTRUM_STATUS);
		Sleep(1000);
		cout << "\t\tEnabling MCA for spectrum data acquisition with status ." << endl;
		chdpp.LibUsb_SendCommand(XMTPT_ENABLE_MCA_MCS);
		Sleep(1000);
		for(int idxSpectrum=0;idxSpectrum<MaxMCA;idxSpectrum++) {
			//cout << "\t\tAcquiring spectrum data set " << (idxSpectrum+1) << " of " << MaxMCA << endl;
			if (chdpp.LibUsb_SendCommand(XMTPT_SEND_SPECTRUM_STATUS)) {	// request spectrum+status
				if (chdpp.LibUsb_ReceiveData()) {
					bDisableMCA = true;				// we are aquiring data, disable mca when done
					system(CLEAR_TERM);
					chdpp.ConsoleGraph(chdpp.DP5Proto.SPECTRUM.DATA,chdpp.DP5Proto.SPECTRUM.CHANNELS,true,chdpp.DppStatusString);
					Sleep(2000);
				}
			} else {
				cout << "\t\tProblem acquiring spectrum." << endl;
				break;
			}
		}
		if (bDisableMCA) {
			//system("Pause");
			//cout << "\t\tSpectrum acquisition with status done. Disabling MCA." << endl;
			chdpp.LibUsb_SendCommand(XMTPT_DISABLE_MCA_MCS);
			Sleep(1000);
		}
	}
}

// Read Configuration File
//		CConsoleHelper::SndCmd.GetDP5CfgStr("PX5_Console_Test.txt");
void ReadConfigFile()
{
	std::string strCfg;
	strCfg = chdpp.SndCmd.AsciiCmdUtil.GetDP5CfgStr("PX5_Console_Test.txt");
	cout << "\t\t\tConfiguration Length: " << (unsigned int)strCfg.length() << endl;
	cout << "\t================================================================" << endl;
	cout << strCfg << endl;
	cout << "\t================================================================" << endl;
}

//Following is an example of loading a configuration from file 
//then sending the configuration to the DPP device.
//	SendConfigFileToDpp("NaI_detector_cfg.txt");    // calls SendCommandString
//	AcquireSpectrum();
//
bool SendCommandString(string strCMD) {
	CONFIG_OPTIONS CfgOptions;
	chdpp.CreateConfigOptions(&CfgOptions, "", chdpp.DP5Stat, false);
	CfgOptions.HwCfgDP5Out = strCMD;
	// send ASCII command string, bypass any filters, read back the mode and settings
	if (chdpp.LibUsb_SendCommand_Config(XMTPT_SEND_CONFIG_PACKET_EX, CfgOptions)) {
		// command sent
	} else {
		cout << "\t\tASCII Command String NOT SENT" << strCMD << endl;
		return false;
	}
	return true;
}

std::string ShortenCfgCmds(std::string strCfgIn) {
	std::string strCfg("");
	strCfg = strCfgIn;
	long lCfgLen=0;						//ASCII Configuration Command String Length
	lCfgLen = (long)strCfg.length();
	if (lCfgLen > 0) {		
		strCfg = chdpp.SndCmd.AsciiCmdUtil.ReplaceCmdText(strCfg, "US;", ";");
		strCfg = chdpp.SndCmd.AsciiCmdUtil.ReplaceCmdText(strCfg, "OFF;", "OF;");
		strCfg = chdpp.SndCmd.AsciiCmdUtil.ReplaceCmdText(strCfg, "RISING;", "RI;");
		strCfg = chdpp.SndCmd.AsciiCmdUtil.ReplaceCmdText(strCfg, "FALLING;", "FA;");
	}
	return strCfg;
}

// run GetDppStatus(); first to get PC5_PRESENT, DppType
// Includes Configuration Oversize Fix 20141224
bool SendConfigFileToDpp(string strFilename){
	std::string strCfg;
	long lCfgLen=0;						//ASCII Configuration Command String Length
	bool bCommandSent=false;
	bool isPC5Present=false;
	int DppType=0;
	int idxSplitCfg=0;					//Configuration split position, only if necessary
	bool bSplitCfg=false;				//Configuration split flag
	std::string strSplitCfg("");		//Configuration split string second buffer
	bool isDP5_RevDxGains;
	unsigned char DPP_ECO;

	isPC5Present = chdpp.DP5Stat.m_DP5_Status.PC5_PRESENT;
	DppType = chdpp.DP5Stat.m_DP5_Status.DEVICE_ID;
	isDP5_RevDxGains = chdpp.DP5Stat.m_DP5_Status.isDP5_RevDxGains;
	DPP_ECO = chdpp.DP5Stat.m_DP5_Status.DPP_ECO;

	strCfg = chdpp.SndCmd.AsciiCmdUtil.GetDP5CfgStr(strFilename);
	strCfg = chdpp.SndCmd.AsciiCmdUtil.RemoveCmdByDeviceType(strCfg,isPC5Present,DppType,isDP5_RevDxGains,DPP_ECO);
	lCfgLen = (long)strCfg.length();
	if ((lCfgLen > 0) && (lCfgLen <= 512)) {		// command length ok
		cout << "\t\t\tConfiguration Length: " << lCfgLen << endl;
	} else if (lCfgLen > 512) {	// configuration too large, needs fix
		cout << "\t\t\tConfiguration Length (Will Shorten): " << lCfgLen << endl;
		strCfg = ShortenCfgCmds(strCfg);
		lCfgLen = (long)strCfg.length();
		if (lCfgLen > 512) {	// configuration still too large, split config
			cout << "\t\t\tConfiguration Length (Will Split): " << lCfgLen << endl;
			bSplitCfg = true;
			idxSplitCfg = chdpp.SndCmd.AsciiCmdUtil.GetCmdChunk(strCfg);
			cout << "\t\t\tConfiguration Split at: " << idxSplitCfg << endl;
			strSplitCfg = strCfg.substr(idxSplitCfg);
			strCfg = strCfg.substr(0, idxSplitCfg);
		}
	} else {
		cout << "\t\t\tConfiguration Length Error: " << lCfgLen << endl;
		return false;
	}
	bCommandSent = SendCommandString(strCfg);
	if (bSplitCfg) {
		// Sleep(40);			// may need delay here
		bCommandSent = SendCommandString(strSplitCfg);
	}
	return bCommandSent;
}

// Close Connection
//		CConsoleHelper::LibUsb_isConnected			// LibUsb DPP connection indicator
//		CConsoleHelper::LibUsb_Close_Connection()	// close connection
void CloseConnection()
{
	if (chdpp.LibUsb_isConnected) { // send and receive status
		cout << endl;
		cout << "\tClosing connection to default LibUsb device..." << endl;
		chdpp.LibUsb_Close_Connection();
		cout << "\t\tDPP device connection closed." << endl;
	}
}

// Helper functions for saving spectrum files
void SaveSpectrumConfig()
{
	string strSpectrumConfig;
	chdpp.Dp5CmdList = chdpp.MakeDp5CmdList();	// ascii text command list for adding comments
	strSpectrumConfig = chdpp.CreateSpectrumConfig(chdpp.HwCfgDP5);	// append configuration comments
	chdpp.sfInfo.strSpectrumConfig = strSpectrumConfig;
}

// Saving spectrum file
void SaveSpectrumFile()
{
	string strSpectrum;											// holds final spectrum file
	chdpp.sfInfo.strSpectrumStatus = chdpp.DppStatusString;		// save last status after acquisition
	chdpp.sfInfo.m_iNumChan = chdpp.mcaCH;						// number channels in spectrum
	chdpp.sfInfo.SerialNumber = chdpp.DP5Stat.m_DP5_Status.SerialNumber;	// dpp serial number
	chdpp.sfInfo.strDescription = "Amptek Spectrum File";					// description
	chdpp.sfInfo.strTag = "TestTag";										// tag
	// create spectrum file, save file to string
	strSpectrum = chdpp.CreateMCAData(chdpp.DP5Proto.SPECTRUM.DATA,chdpp.sfInfo,chdpp.DP5Stat.m_DP5_Status);
	chdpp.SaveSpectrumStringToFile(strSpectrum);	// save spectrum file string to file
}

int main(int argc, char* argv[])
{
	//system(CLEAR_TERM);
	ConnectToDefaultDPP();
	cout << "Press the Enter key to continue . . .";
	_getch();

	if(!chdpp.LibUsb_isConnected) { return 1; }

	//system(CLEAR_TERM);
	chdpp.DP5Stat.m_DP5_Status.SerialNumber = 0;
	GetDppStatus();
	cout << "Press the Enter key to continue . . .";
	_getch();

	if (chdpp.DP5Stat.STATUS_MNX.SN == 0) { return 1; }

	GetInterlockStatus();
	cout << "Press the Enter key to continue . . .";
	_getch();

	/// Read the voltage and current on the tube.
	// ReadHVCfg();
	// cout << "Press the Enter Key to continue . . .";
	// _getch();
	int userInput;



	while (true) {
		cout << "1 - Status, 2 - TurnHVOff, 3 - TurnHVOn, 4 - Disconnect USB" << endl;
		cin >> userInput;

		if (userInput == 0) {
			break;
		}

		switch (userInput) {
			case 1:
				GetDppStatus();
				break;
			case 2:
				TurnHVOff();
				break;
			case 3:
				TurnHVOn();
				break;
			case 4:
				CloseConnection();
				return 0;
				break;
			default:
				cout << "Invalid Input" << endl;
		}
	}


	bool bReadHVCfg;
	bReadHVCfg = false;

	if (bReadHVCfg) 
	{
		/// Turn the tube on to 15 kV and 15 uA
		TurnHVOn();
		cout << "Press the Enter key to continue . . .";
		_getch();

		GetDppStatus();
		cout << "Press the Enter key to continue . . .";
		_getch();
		

		// ReadHVCfg();
		// cout << "Press the Enter Key to continue . . .";
		// _getch();


		/// Turn the tube off (set to 0 kV and 0 uA)
		TurnHVOff();
		cout << "Press the Enter key to continue . . .";
		_getch();



		GetDppStatus();
		cout << "Press the Enter key to continue . . .";
		_getch();

		

		// ReadHVCfg();
		// cout << "Press the Enter Key to continue . . .";
		// _getch();
	}
	

	//////	system("cls");
	//////	SendConfigFileToDpp("PX5_Console_Test.txt");    // calls SendCommandString
	//////	system("Pause");


	//system(CLEAR_TERM);
	//ReadDppConfigurationFromHardware(true);
	//cout << "Press the Enter key to continue . . .";
	//_getch(); 

	//system(CLEAR_TERM);
	//DisplayPresets();
	//cout << "Press the Enter key to continue . . .";
	//_getch(); 

	//system(CLEAR_TERM);
	//SendPresetAcquisitionTime("PRET=20;");
	//SaveSpectrumConfig();
	//cout << "Press the Enter key to continue . . .";
	//_getch(); 

	//system(CLEAR_TERM);
	//AcquireSpectrum();
	//SaveSpectrumFile();
	//cout << "Press the Enter key to continue . . .";
	//_getch(); 

	//system(CLEAR_TERM);
	//SendPresetAcquisitionTime("PRET=OFF;");
	//cout << "Press the Enter key to continue . . .";
	//_getch(); 

	//system(CLEAR_TERM);
	//ReadConfigFile();
	//cout << "Press the Enter key to continue . . .";
	//_getch(); 

	//system(CLEAR_TERM);
	CloseConnection();
	cout << "Press the Enter key to continue . . .";
	_getch(); 

	return 0;
}


//}