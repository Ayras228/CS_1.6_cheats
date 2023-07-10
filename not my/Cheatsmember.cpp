//#include <stdafx.h>
//#ifndef PARSING_TEST_BUILD
//#include "EngineLoader.h"
//#include <engine_launcher_api.h>
//#include <tier1/strtools.h>
//#include "EngineHooks.h"
//#include "SteamPatcher.h"
//#include <DynPatcher/CSectionData.h>
//#include <tier0/ThreadUtils.h>
//#include "SteamAPI_impl/CServerQuery.h"
//#include "SteamAPI_impl/CBaseServerListRequest.h"
//
//
//CEngineLoader::CEngineLoader()
//{
//#ifdef _WIN32
//    Registry = 0;
//#endif
//
//    g_EngineData = new EnginePatcherData_t;
//    memset(g_EngineData, 0, sizeof(EnginePatcherData_t));
//    g_EngineModule = CreateDynPatcher();
//
//}
//
//CEngineLoader::~CEngineLoader()
//{
//#ifdef _WIN32
//    if (Registry)
//    {
//        ReleaseInstancedRegistry(Registry);
//    }
//#endif
//    delete g_EngineData;
//    delete g_EngineModule;
//}
//
//BOOL CEngineLoader::Init()
//{
//
//#ifdef _WIN32
//    Registry = InstanceRegistry("Half-Life\\Settings\\");
//    AssertFatalMsg(Registry, _T("Can't init registry!"));
//    if (!Registry)
//    {
//        return FALSE;
//    }
//
//    if (CommandLine()->CheckParm("-soft") || CommandLine()->CheckParm("-software"))
//    {
//        Registry->WriteString("EngineDLL", "sw.dll");
//        IsSoftware = TRUE;
//    }
//    else if (CommandLine()->CheckParm("-gl") || CommandLine()->CheckParm("-d3d"))
//    {
//        Registry->WriteString("EngineDLL", "hw.dll");
//        IsSoftware = FALSE;
//    }
//
//
//    if (Registry->ReadInt("CrashInitializingVideoMode", FALSE))
//    {
//        Registry->WriteInt("CrashInitializingVideoMode", FALSE);
//
//        if (!strcmp(Registry->ReadString("EngineDLL", "hw.dll"), "hw.dll"))
//        {
//            if (Registry->ReadInt("EngineD3D", FALSE))
//            {
//                Registry->WriteInt("EngineD3D", FALSE);
//                if (MessageBoxA(NULL, "The game has detected that the previous attempt to start in D3D video mode failed.\nThe game will now run attempt to run in openGL mode.", "Video mode change failure", MB_OKCANCEL | MB_ICONWARNING) != IDOK)
//                    return FALSE;
//            }
//            else
//            {
//                Registry->WriteString("EngineDLL", "sw.dll");
//                if (MessageBoxA(NULL, "The game has detected that the previous attempt to start in openGL video mode failed.\nThe game will now run in software mode.", "Video mode change failure", MB_OKCANCEL | MB_ICONWARNING) != IDOK)
//                    return FALSE;
//
//                IsSoftware = TRUE;
//            }
//        }
//        else
//        {
//            if (MessageBoxA(NULL, "The game has detected that the previous attempt to start in software video mode failed.\nThe game will now run in openGL mode.", "Video mode change failure", MB_OKCANCEL | MB_ICONWARNING) != IDOK)
//                return FALSE;
//            Registry->WriteString("EngineDLL", "hw.dll");
//            Registry->WriteInt("EngineD3D", FALSE);
//            IsSoftware = FALSE;
//        }
//
//        Registry->WriteInt("ScreenWidth", 640);
//        Registry->WriteInt("ScreenHeight", 480);
//        Registry->WriteInt("ScreenBPP", 16);
//    }
//
//
//    strcpy(engineDLL, Registry->ReadString("EngineDLL", "hw.dll"));
//    if (!strcmp(engineDLL, "hw.dll"))
//    {
//        IsSoftware = FALSE;
//    }
//    else
//    {
//        IsSoftware = TRUE;
//    }
//#else
//    strcpy(engineDLL, "hw.so");
//    IsSoftware = TRUE;
//#endif
//    BOOL ParseOK = TRUE;
//
//    if (!g_EngineModule->Init(engineDLL, bTrue))
//    {
//        Msg(_T("Unable to load %s\n"), engineDLL);
//        return FALSE;
//    }
//    else
//    {
//        DbgMsg(_T("Load %s:OK\nChecking:\n"), engineDLL);
//        ParseOK = InitEngineData(g_EngineModule, g_EngineData);
//    }
//
//    if (!ParseOK)
//        return FALSE;
//    return TRUE;
//}
//
//BOOL CEngineLoader::Run()
//{
//    char szNewCommandParams[2048];
//    char szBaseDir[512];
//    char szCmdLine[512];
//    int CmdLineLen = 0;
//
//    szNewCommandParams[0] = 0;
//    BOOL ShouldRestart = FALSE;
//    BOOL ShouldSwitchMode = FALSE;
//    BOOL ShouldRunInSafeMode = FALSE;
//    BOOL InFailState = FALSE;
//    CreateInterfaceFn EngineFactory = NULL;
//    IEngineAPI* EngineAPI = NULL;
//    EngRunResult_t RunResult = ENGINE_NOTQUITTING;
//
//    char ErrorMsg[2048];
//    uint32_t ErrorMsgLen = 0;
//    int Instance = 0;
//    EngineFactory = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(reinterpret_cast<HMODULE>(g_EngineModule->GetDllHandler()), CREATEINTERFACE_PROCNAME));
//    if (!EngineFactory)
//    {
//        ErrorMsgLen += V_snprintf(&ErrorMsg[ErrorMsgLen], sizeof(ErrorMsg) - 1 - ErrorMsgLen, "Unable to find \"%s\" function in \"%s\"\n. The game will now run in %s mode.", CREATEINTERFACE_PROCNAME, engineDLL, IsSoftware ? "OpenGL" : "Software");
//        InFailState = TRUE;
//        ShouldSwitchMode = TRUE;
//    }
//    else
//    {
//        if (NULL == (EngineAPI = reinterpret_cast<IEngineAPI*>(EngineFactory(VENGINE_LAUNCHER_API_VERSION, NULL))))
//        {
//            ErrorMsgLen += V_snprintf(&ErrorMsg[ErrorMsgLen], sizeof(ErrorMsg) - 1 - ErrorMsgLen, "Unable to create IEngineAPI interface in \"%s\"\n. The game will now run in %s mode.", engineDLL, IsSoftware ? "OpenGL" : "Software");
//            InFailState = TRUE;
//            ShouldSwitchMode = TRUE;
//        }
//        else
//        {
//            PreRun();
//            if (!CommandLine()->CheckParm("-game"))
//            {
//                CommandLine()->AppendParm("-game", "cstrike");
//            }
//            strcpy(szBaseDir, CBaseNotification::GetBaseDir());
//            szCmdLine[0] = 0;
//            for (int i = 0; i < CommandLine()->ParmCount(); i++)
//            {
//                strcat(szCmdLine, CommandLine()->GetParm(i));
//                strcat(szCmdLine, " ");
//            }
//            //strcpy(szCmdLine,CommandLine()->GetCmdLine());      
//            Msg("BaseDir=%s\n,CmdLine=%s\n", szBaseDir, szCmdLine);
//            RunResult = EngineAPI->Run(&Instance, szBaseDir, szCmdLine, szNewCommandParams, NULL, NULL);
//            Msg("RunResult=%i,Instance=%i\n", RunResult, Instance);
//            PostRun();
//        }
//        switch (RunResult)
//        {
//        case ENGINE_RESTART:
//        {
//            ShouldRestart = TRUE;
//            break;
//        }
//        case ENGINE_FAILURE:
//        {
//
//            ErrorMsgLen += V_snprintf(&ErrorMsg[ErrorMsgLen], sizeof(ErrorMsg) - 1 - ErrorMsgLen, "The specified video mode is not supported.\nThe game will now run in %s mode.", IsSoftware ? "OpenGL" : "Software");
//            ShouldRestart = TRUE;
//            ShouldRunInSafeMode = TRUE;
//            ShouldSwitchMode = TRUE;
//            InFailState = TRUE;
//            break;
//        }
//        case ENGINE_NOTQUITTING:
//        {
//            ShouldRestart = FALSE;
//            break;
//        }
//        default:
//        {
//            ShouldRestart = FALSE;
//        }
//        }
//    }
//
//    if (InFailState)
//    {
//#ifdef _WIN32
//        if (MessageBoxA(NULL, ErrorMsg, "Engine failure", MB_OKCANCEL | MB_ICONWARNING) != IDOK)
//        {
//#endif
//            ShouldRestart = FALSE;
//#ifdef _WIN32
//        }
//        else
//        {
//            ShouldRestart = TRUE;
//            if (ShouldRunInSafeMode)
//            {
//                Registry->WriteInt("ScreenWidth", 640);
//                Registry->WriteInt("ScreenHeight", 480);
//                Registry->WriteInt("ScreenBPP", 16);
//            }
//            if (ShouldSwitchMode)
//            {
//                if (IsSoftware)
//                {
//                    Registry->WriteString("EngineDLL", "hw.dll");
//                    Registry->WriteInt("EngineD3D", FALSE);
//                }
//                else
//                {
//                    Registry->WriteString("EngineDLL", "sw.dll");
//                }
//            }
//        }
//#endif
//    }
//
//    if (ShouldRestart)
//    {
//        CommandLine()->RemoveParm("-sw");
//        CommandLine()->RemoveParm("-startwindowed");
//        CommandLine()->RemoveParm("-windowed");
//        CommandLine()->RemoveParm("-window");
//        CommandLine()->RemoveParm("-full");
//        CommandLine()->RemoveParm("-fullscreen");
//        CommandLine()->RemoveParm("-soft");
//        CommandLine()->RemoveParm("-software");
//        CommandLine()->RemoveParm("-gl");
//        CommandLine()->RemoveParm("-d3d");
//        CommandLine()->RemoveParm("-w");
//        CommandLine()->RemoveParm("-width");
//        CommandLine()->RemoveParm("-h");
//        CommandLine()->RemoveParm("-height");
//        CommandLine()->RemoveParm("+connect");
//        CommandLine()->ParmValue("-novid", 0);
//        if (szNewCommandParams[0])
//        {
//            if (Q_strstr(szNewCommandParams, "-game"))
//                CommandLine()->RemoveParm("-game");
//
//            if (Q_strstr(szNewCommandParams, "+load"))
//                CommandLine()->RemoveParm("+load");
//
//            CommandLine()->AppendParm(szNewCommandParams, 0);
//        }
//    }
//    Msg("ShouldRestart=%i\n", ShouldRestart);
//    return ShouldRestart;
//}
//
//BOOL CEngineLoader::PreRun()
//{
//    g_SteamPatcher.Init();
//    g_EngineModule->HookFunctionCall(g_EngineData->Host_Frame, Host_Frame_Hook);
//    return TRUE;
//}
//
//
//
//void CEngineLoader::PostRun()
//{
//    g_ServerQueryList.RemoveQueries();
//    g_ServerQueryList.Terminate();
//    g_EngineModule->UnsetHooks();
//    g_SteamPatcher.Close();
//    g_ServerListCallbacks.OnExit();
//
//}
//#endif // PARSING_TEST_BUILD