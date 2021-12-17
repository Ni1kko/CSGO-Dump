const int WM_PUBLIC         = 1;
const int WM_PRIVATE        = 2;

 // Number of crypted sections
const int  NUMBER_OF_CRYPTED_SECTIONS       = 16;

  // Crypt hash types, possible hash values for functions:
  // EP_CryptHashBuffer
  // EP_CryptHashStringA
  // EP_CryptHashStringW
  // EP_CryptHashFileA
  // EP_CryptHashFileW
const int HASH_XOR32        = 0;
const int HASH_MD2          = 1;
const int HASH_MD5          = 2;
const int HASH_RipeMD160    = 3;
const int HASH_SH1          = 4;
const int HASH_SHA224       = 5;
const int HASH_SHA256       = 6;
const int HASH_SHA384       = 7;
const int HASH_SHA512       = 8;

 // Return values of EP_RegLoadKey
const int LOADKEY_SUCCEEDED = 0;
const int LOADKEY_REGINFONOTFOUND = 1;
const int LOADKEY_NAMEBUFFERTOOSMALL = 2;
const int LOADKEY_KEYBUFFERTOOSMALL = 3;

 // Return values of EP_RegKeyStatus
const int KEY_STATUS_DOESNOTEXIST                         = 0;
const int KEY_STATUS_VALID                                = 1;
const int KEY_STATUS_INVALID                              = 2;
const int KEY_STATUS_STOLEN                               = 3;
const int KEY_STATUS_DATEEXPIRED                          = 4;
const int KEY_STATUS_WITHOUTHARDWARELOCK                  = 5;
const int KEY_STATUS_WITHOUTEXPIRATIONDATE                = 6;
const int KEY_STATUS_WITHOUTREGISTERAFTERDATE             = 7;
const int KEY_STATUS_WITHOUTREGISTERBEFOREDATE            = 8;
const int KEY_STATUS_WITHOUTEXECUTIONSLIMIT               = 9;
const int KEY_STATUS_WITHOUTDAYSLIMIT                     = 10;
const int KEY_STATUS_WITHOUTRUNTIMELIMIT                  = 11;
const int KEY_STATUS_WITHOUTGLOBALTIMELIMIT               = 12;
const int KEY_STATUS_WITHOUTCOUNTRYLOCK                   = 13;
const int KEY_STATUS_COUNTRYINVALID                       = 14;
const int KEY_STATUS_REGISTERAFTERFAILED                  = 15;
const int KEY_STATUS_REGISTERBEFOREFAILED                 = 16;
const int KEY_STATUS_EXECUTIONSEXPIRED                    = 17;
const int KEY_STATUS_DAYSEXPIRED                          = 18;
const int KEY_STATUS_RUNTIMEEXPIRED                       = 19;
const int KEY_STATUS_GLOBALTIMEEXPIRED                    = 20;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_VOLUMESERIAL = 21;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_VOLUMENAME   = 22;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_COMPUTERNAME = 23;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_CPU          = 24;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_MOTHERBOARD  = 25;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_WINDOWSKEY   = 26;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_HDDSERIAL    = 27;
const int KEY_STATUS_HARDWARECHANGESEXCEEDED_USERNAME     = 28;
const int KEY_STATUS_HARDWAREINVALID                      = 29;

  // Counties code
const int CN_AFGHANISTAN = 114;
const int CN_ALBANIA = 1;
const int CN_ALGERIA = 2;
const int CN_ARGENTINA = 3;
const int CN_ARMENIA = 4;
const int CN_AUSTRALIA = 5;
const int CN_AUSTRIA = 6;
const int CN_AZERBAIJAN = 7;
const int CN_BAHRAIN = 8;
const int CN_BANGLADESH = 115;
const int CN_BELARUS = 9;
const int CN_BELGIUM = 10;
const int CN_BELIZE = 11;
const int CN_BOLIVIA = 116;
const int CN_BOSNIA = 117;
const int CN_BRAZIL = 13;
const int CN_BRUNEI = 14;
const int CN_BULGARIA = 15;
const int CN_CAMBODIA = 16;
const int CN_CANADA = 17;
const int CN_CARRIBEAN = 118;
const int CN_CHILE = 20;
const int CN_CHINA = 21;
const int CN_COLOMBIA = 22;
const int CN_COSTARICA = 23;
const int CN_CROATIA = 24;
const int CN_CZECH = 25;
const int CN_DENMARK = 26;
const int CN_DOMINICAN = 27;
const int CN_ECUADOR = 28;
const int CN_EGYPT = 29;
const int CN_ELSALVADOR = 30;
const int CN_ESTONIA = 31;
const int CN_ETHIOPIA = 119;
const int CN_FAROE = 32;
const int CN_FINLAND = 33;
const int CN_FRANCE = 34;
const int CN_GEORGIA = 35;
const int CN_GERMANY = 36;
const int CN_GREECE = 37;
const int CN_GREENLAND = 120;
const int CN_GUATEMALA = 38;
const int CN_HONDURAS = 39;
const int CN_HONGKONG = 40;
const int CN_HUNGARU = 41;
const int CN_ICELAND = 42;
const int CN_INDIA = 43;
const int CN_INDONESIA = 44;
const int CN_IRAN = 45;
const int CN_IRAQ = 46;
const int CN_IRELAND = 47;
const int CN_ISRAEL = 48;
const int CN_ITALY = 49;
const int CN_JAMAICA = 50;
const int CN_JAPAN = 51;
const int CN_JORDAN = 52;
const int CN_KAZAKHSTAN = 53;
const int CN_KENYA = 54;
const int CN_KOREA = 56;
const int CN_KUWAIT = 57;
const int CN_KYRGYZSTAN = 58;
const int CN_LAOS = 121;
const int CN_LATVIA = 59;
const int CN_LEBANON = 60;
const int CN_LIBYAN = 122;
const int CN_LIECHTENSTEIN = 62;
const int CN_LITHUANIA = 63;
const int CN_LUXEMBOURG = 64;
const int CN_MACAO = 65;
const int CN_MACEDONIA = 66;
const int CN_MALAYSIA = 67;
const int CN_MALDIVES = 123;
const int CN_MALTA = 124;
const int CN_MEXOCI = 68;
const int CN_MONACO = 70;
const int CN_MONGOLIA = 71;
const int CN_MONTENEGRO = 125;
const int CN_MOROCCO = 72;
const int CN_NEPAL = 126;
const int CN_NETHERLANDS = 73;
const int CN_NEWZEALAND = 74;
const int CN_NICARAGUA = 75;
const int CN_NIGERIA = 127;
const int CN_NORWAY = 76;
const int CN_OMAN = 77;
const int CN_PAKISTAN = 78;
const int CN_PANAMA = 79;
const int CN_PARAGUAY = 80;
const int CN_PERY = 81;
const int CN_PHILIPPINES = 82;
const int CN_POLAND = 83;
const int CN_PORTUGAL = 84;
const int CN_PUERTORICO = 85;
const int CN_QATAR = 86;
const int CN_ROMANIA = 87;
const int CN_RUSSIA = 88;
const int CN_RWANDA = 128;
const int CN_SAUDIARABIA = 89;
const int CN_SENEGAL = 129;
const int CN_SERBIA = 130;
const int CN_SERBIAMONTENEGRO = 90;
const int CN_SINGAROPE = 91;
const int CN_SLOVAKIA = 92;
const int CN_SLOVENIA = 93;
const int CN_SOUTHAFRICA = 94;
const int CN_SPAIN = 95;
const int CN_SRILANKA = 131;
const int CN_SWEDEN = 96;
const int CN_SWITZERLAND = 97;
const int CN_SYRIAN = 132;
const int CN_TAIWAN = 98;
const int CN_TAJIKISTAN = 99;
const int CN_THAILAND = 100;
const int CN_TRINIDADTOBAGO = 101;
const int CN_TUNISIA = 102;
const int CN_TURKEY = 103;
const int CN_TURKMENISTAN = 133;
const int CN_UKRAINE = 104;
const int CN_UAE = 105;
const int CN_UNITEDKINGDOM = 106;
const int CN_USA = 107;
const int CN_URUGUAY = 108;
const int CN_UZBEKISTAN = 109;
const int CN_VENEZUELA = 110;
const int CN_VIETNAM = 111;
const int CN_YEMEN = 112;
const int CN_ZIMBABWE = 113;

// Return results of the functions EP_RegEncryptRegistrationInformation and
// EP_RegDecryptRegistrationInformation
const int REG_CRYPT_OK                       = 0;
const int REG_CRYPT_INVALID                  = 1;
const int REG_CRYPT_INSUFFICIENT_BUFFER      = 2;
const int REG_CRYPT_INSUFFICIENT_NAME_BUFFER = 3;
const int REG_CRYPT_INSUFFICIENT_KEY_BUFFER  = 4;

#pragma pack(push, 4)

typedef struct _TKeyCountries
{
  char* Name;
  int Code;
} TKeyCountries, *PKeyCountries;

const TKeyCountries KEY_COUNTRIES[127] =
{
  xorstr_("Afghanistan")               , CN_AFGHANISTAN      ,
  xorstr_("Albania")                   , CN_ALBANIA          ,
  xorstr_("Algeria")                   , CN_ALGERIA          ,
  xorstr_("Argentina")                 , CN_ARGENTINA        ,
  xorstr_("Armenia")                   , CN_ARMENIA          ,
  xorstr_("Australia")                 , CN_AUSTRALIA        ,
  xorstr_("Austria")                   , CN_AUSTRIA          ,
  xorstr_("Azerbaijan")                , CN_AZERBAIJAN       ,
  xorstr_("Bahrain")                   , CN_BAHRAIN          ,
  xorstr_("Bangladesh")                , CN_BANGLADESH       ,
  xorstr_("Belarus")                   , CN_BELARUS          ,
  xorstr_("Belgium")                   , CN_BELGIUM          ,
  xorstr_("Belize")                    , CN_BELIZE           ,
  xorstr_("Bolivia")                   , CN_BOLIVIA          ,
  xorstr_("Bosnia and Herzegovina")    , CN_BOSNIA           ,
  xorstr_("Brazil")                    , CN_BRAZIL           ,
  xorstr_("Brunei Darussalam")         , CN_BRUNEI           ,
  xorstr_("Bulgaria")                  , CN_BULGARIA         ,
  xorstr_("Cambodia")                  , CN_CAMBODIA         ,
  xorstr_("Canada")                    , CN_CANADA           ,
  xorstr_("Caribbean")                 , CN_CARRIBEAN        ,
  xorstr_("Chile")                     , CN_CHILE            ,
  xorstr_("China")                     , CN_CHINA            ,
  xorstr_("Colombia")                  , CN_COLOMBIA         ,
  xorstr_("Costa Rica")                , CN_COSTARICA        ,
  xorstr_("Croatia")                   , CN_CROATIA          ,
  xorstr_("Czech Republic")            , CN_CZECH            ,
  xorstr_("Denmark")                   , CN_DENMARK          ,
  xorstr_("Dominican Republic")        , CN_DOMINICAN        ,
  xorstr_("Ecuador")                   , CN_ECUADOR          ,
  xorstr_("Egypt")                     , CN_EGYPT            ,
  xorstr_("El Salvador")               , CN_ELSALVADOR       ,
  xorstr_("Estonia")                   , CN_ESTONIA          ,
  xorstr_("Ethiopia")                  , CN_ETHIOPIA         ,
  xorstr_("Faroe Islands")             , CN_FAROE            ,
  xorstr_("Finland")                   , CN_FINLAND          ,
  xorstr_("France")                    , CN_FRANCE           ,
  xorstr_("Georgia")                   , CN_GEORGIA          ,
  xorstr_("Germany")                   , CN_GERMANY          ,
  xorstr_("Greece")                    , CN_GREECE           ,
  xorstr_("Greenland")                 , CN_GREENLAND        ,
  xorstr_("Guatemala")                 , CN_GUATEMALA        ,
  xorstr_("Honduras")                  , CN_HONDURAS         ,
  xorstr_("Hong Kong")                 , CN_HONGKONG         ,
  xorstr_("Hungary")                   , CN_HUNGARU          ,
  xorstr_("Iceland")                   , CN_ICELAND          ,
  xorstr_("India")                     , CN_INDIA            ,
  xorstr_("Indonesia")                 , CN_INDONESIA        ,
  xorstr_("Iran")                      , CN_IRAN             ,
  xorstr_("Iraq")                      , CN_IRAQ             ,
  xorstr_("Ireland")                   , CN_IRELAND          ,
  xorstr_("Israel")                    , CN_ISRAEL           ,
  xorstr_("Italy")                     , CN_ITALY            ,
  xorstr_("Jamaica")                   , CN_JAMAICA          ,
  xorstr_("Japan")                     , CN_JAPAN            ,
  xorstr_("Jordan")                    , CN_JORDAN           ,
  xorstr_("Kazakhstan")                , CN_KAZAKHSTAN       ,
  xorstr_("Kenya")                     , CN_KENYA            ,
  xorstr_("Korea")                     , CN_KOREA            ,
  xorstr_("Kuwait")                    , CN_KUWAIT           ,
  xorstr_("Kyrgyzstan")                , CN_KYRGYZSTAN       ,
  xorstr_("Laos")                      , CN_LAOS             ,
  xorstr_("Latvia")                    , CN_LATVIA           ,
  xorstr_("Lebanon")                   , CN_LEBANON          ,
  xorstr_("Libyan")                    , CN_LIBYAN           ,
  xorstr_("Liechtenstein")             , CN_LIECHTENSTEIN    ,
  xorstr_("Lithuania")                 , CN_LITHUANIA        ,
  xorstr_("Luxembourg")                , CN_LUXEMBOURG       ,
  xorstr_("Macao")                     , CN_MACAO            ,
  xorstr_("Macedonia")                 , CN_MACEDONIA        ,
  xorstr_("Malaysia")                  , CN_MALAYSIA         ,
  xorstr_("Maldives")                  , CN_MALDIVES         ,
  xorstr_("Malta")                     , CN_MALTA            ,
  xorstr_("Mexico")                    , CN_MEXOCI           ,
  xorstr_("Monaco")                    , CN_MONACO           ,
  xorstr_("Mongolia")                  , CN_MONGOLIA         ,
  xorstr_("Montenegro")                , CN_MONTENEGRO       ,
  xorstr_("Morocco")                   , CN_MOROCCO          ,
  xorstr_("Nepal")                     , CN_NEPAL            ,
  xorstr_("Netherlands")               , CN_NETHERLANDS      ,
  xorstr_("New Zealand")               , CN_NEWZEALAND       ,
  xorstr_("Nicaragua")                 , CN_NICARAGUA        ,
  xorstr_("Nigeria")                   , CN_NIGERIA          ,
  xorstr_("Norway")                    , CN_NORWAY           ,
  xorstr_("Oman")                      , CN_OMAN             ,
  xorstr_("Pakistan")                  , CN_PAKISTAN         ,
  xorstr_("Panama")                    , CN_PANAMA           ,
  xorstr_("Paraguay")                  , CN_PARAGUAY         ,
  xorstr_("Peru")                      , CN_PERY             ,
  xorstr_("Philippines")               , CN_PHILIPPINES      ,
  xorstr_("Poland")                    , CN_POLAND           ,
  xorstr_("Portugal")                  , CN_PORTUGAL         ,
  xorstr_("Puerto Rico")               , CN_PUERTORICO       ,
  xorstr_("Qatar")                     , CN_QATAR            ,
  xorstr_("Romania")                   , CN_ROMANIA          ,
  xorstr_("Russia")                    , CN_RUSSIA           ,
  xorstr_("Rwanda")                    , CN_RWANDA           ,
  xorstr_("Saudi Arabia")              , CN_SAUDIARABIA      ,
  xorstr_("Senegal")                   , CN_SENEGAL          ,
  xorstr_("Serbia")                    , CN_SERBIA           ,
  xorstr_("Serbia and Montenegro")     , CN_SERBIAMONTENEGRO ,
  xorstr_("Singapore")                 , CN_SINGAROPE        ,
  xorstr_("Slovakia")                  , CN_SLOVAKIA         ,
  xorstr_("Slovenia")                  , CN_SLOVENIA         ,
  xorstr_("South Africa")              , CN_SOUTHAFRICA      ,
  xorstr_("Spain")                     , CN_SPAIN            ,
  xorstr_("Sri Lanka")                 , CN_SRILANKA         ,
  xorstr_("Sweden")                    , CN_SWEDEN           ,
  xorstr_("Switzerland")               , CN_SWITZERLAND      ,
  xorstr_("Syrian")                    , CN_SYRIAN           ,
  xorstr_("Taiwan")                    , CN_TAIWAN           ,
  xorstr_("Tajikistan")                , CN_TAJIKISTAN       ,
  xorstr_("Thailand")                  , CN_THAILAND         ,
  xorstr_("Trinidad and Tobago")       , CN_TRINIDADTOBAGO   ,
  xorstr_("Tunisia")                   , CN_TUNISIA          ,
  xorstr_("Turkey")                    , CN_TURKEY           ,
  xorstr_("Turkmenistan")              , CN_TURKMENISTAN     ,
  xorstr_("Ukraine")                   , CN_UKRAINE          ,
  xorstr_("United Arab Emirates")      , CN_UAE              ,
  xorstr_("United Kingdom")            , CN_UNITEDKINGDOM    ,
  xorstr_("United States")             , CN_USA              ,
  xorstr_("Uruguay")                   , CN_URUGUAY          ,
  xorstr_("Uzbekistan")                , CN_UZBEKISTAN       ,
  xorstr_("Venezuela")                 , CN_VENEZUELA        ,
  xorstr_("Viet Nam")                  , CN_VIETNAM          ,
  xorstr_("Yemen")                     , CN_YEMEN            ,
  xorstr_("Zimbabwe")                  , CN_ZIMBABWE
};

typedef struct TWMContent {

    int   WMType;
    char* Name;
    int   NameLen;
    char* Text;
    int   TextLen;
    char* FileName;
    int   FileNameLen;
    char* AFile;
    int   AFileLen;

} TWMContent, *PWMContent;

typedef struct TKeyInformation {
	BOOL Stolen;               // {out} is key stolen
    DWORD CreationYear;        // {out} key creation year
    DWORD CreationMonth;       // {out} key creation month
    DWORD CreationDay;         // {out} key creation day
    BOOL UseKeyExpiration;     // {out} has key expiration date?
    DWORD ExpirationYear;      // {out} key expiration year
    DWORD ExpirationMonth;     // {out} key expiration month
    DWORD ExpirationDay;       // {out} key expiration day
    BOOL UseHardwareLocking;   // {out} hardware locked key
    BOOL UseExecutionsLimit;   // {out} limit key by executions?
    DWORD ExecutionsCount;     // {out} number of executions
    BOOL UseDaysLimit;         // {out} limit key by days?
    DWORD DaysCount;           // {out} number of days
    BOOL UseRunTimeLimit;      // {out} limit key by run time?
    DWORD RunTimeMinutes;      // {out} run time minutes
    BOOL UseGlobalTimeLimit;   // {out} limit key by global time?
    DWORD GlobalTimeMinutes;   // {out} global time minutes
    BOOL UseCountyLimit;       // {out} limit key by country?
    DWORD CountryCode;         // {out} country code
    BOOL UseRegisterAfter;     // {out} register key after date?
    DWORD RegisterAfterYear;   // {out} register after year
    DWORD RegisterAfterMonth;  // {out} register after month
    DWORD RegisterAfterDay;    // {out} register after day
    BOOL UseRegisterBefore;    // {out} register key before date?
    DWORD RegisterBeforeYear;  // {out} register before year
    DWORD RegisterBeforeMonth; // {out} register before month
    DWORD RegisterBeforeDay;   // {out} register before day
    BOOL EncryptedSections[NUMBER_OF_CRYPTED_SECTIONS]; // {out} Crypted sections
} TKeyInformation, *PKeyInformation;

#pragma pack(pop)

extern xorstr_("C")
{
// Registration API
BOOL __declspec(dllimport) __stdcall EP_RegSaveKey( const char* Name, const char* Key );
BOOL __declspec(dllimport) __stdcall EP_RegSaveKeyA( const char* Name, const char* Key );
BOOL __declspec(dllimport) __stdcall EP_RegSaveKeyW( const wchar_t* Name, const wchar_t* Key );
BOOL __declspec(dllimport) __stdcall EP_RegLoadKey( char** Name, char** Key );
BOOL __declspec(dllimport) __stdcall EP_RegLoadKeyA( char** Name, char** Key );
BOOL __declspec(dllimport) __stdcall EP_RegLoadKeyW( wchar_t** Name, wchar_t** Key );
int __declspec(dllimport) __stdcall EP_RegLoadKeyEx( const char* Name, int* NameLen, const char* Key, int* KeyLen);
BOOL __declspec(dllimport) __stdcall EP_RegCheckKey( const char* Name, const char* Key );
BOOL __declspec(dllimport) __stdcall EP_RegCheckKeyA( const char* Name, const char* Key );
BOOL __declspec(dllimport) __stdcall EP_RegCheckKeyW( const wchar_t* Name, const wchar_t* Key );
BOOL __declspec(dllimport) __stdcall EP_RegDeleteKey();
BOOL __declspec(dllimport) __stdcall EP_RegLoadAndCheckKey();
BOOL __declspec(dllimport) __stdcall EP_RegCheckAndSaveKey( const char* Name, const char* Key );
BOOL __declspec(dllimport) __stdcall EP_RegCheckAndSaveKeyA( const char* Name, const char* Key );
BOOL __declspec(dllimport) __stdcall EP_RegCheckAndSaveKeyW( const wchar_t* Name, const wchar_t* Key );
LPCTSTR __declspec(dllimport) __stdcall EP_RegHardwareID();
LPCTSTR __declspec(dllimport) __stdcall EP_RegHardwareIDA();
LPCWSTR __declspec(dllimport) __stdcall EP_RegHardwareIDW();
BOOL __declspec(dllimport) __stdcall EP_RegKeyCreationDate( int* Year, int* Month, int* Day );
BOOL __declspec(dllimport) __stdcall EP_RegKeyExpirationDate( int* Year, int* Month, int* Day );
int __declspec(dllimport) __stdcall EP_RegKeyCreationDateEx();
int __declspec(dllimport) __stdcall EP_RegKeyExpirationDateEx();
BOOL __declspec(dllimport) __stdcall EP_RegKeyExecutions( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_RegKeyExecutionsTotal();
int __declspec(dllimport) __stdcall EP_RegKeyExecutionsLeft();
BOOL __declspec(dllimport) __stdcall EP_RegKeyDays( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_RegKeyDaysTotal();
int __declspec(dllimport) __stdcall EP_RegKeyDaysLeft();
BOOL __declspec(dllimport) __stdcall EP_RegKeyRuntime( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_RegKeyRuntimeTotal();
int __declspec(dllimport) __stdcall EP_RegKeyRuntimeLeft();
BOOL __declspec(dllimport) __stdcall EP_RegKeyGlobalTime( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_RegKeyGlobalTimeTotal();
int __declspec(dllimport) __stdcall EP_RegKeyGlobalTimeLeft();
BOOL __declspec(dllimport) __stdcall EP_RegKeyRegisterAfterDate( int* Year, int* Month, int* Day );
int __declspec(dllimport) __stdcall EP_RegKeyRegisterAfterDateEx();
BOOL __declspec(dllimport) __stdcall EP_RegKeyRegisterBeforeDate( int* Year, int* Month, int* Day );
int __declspec(dllimport) __stdcall EP_RegKeyRegisterBeforeDateEx();
BOOL __declspec(dllimport) __stdcall EP_RegKeyInformation(const char* AName, const char* AKey, PKeyInformation AKeyInfo);
BOOL __declspec(dllimport) __stdcall EP_RegKeyInformationA(const char* AName, const char* AKey, PKeyInformation AKeyInfo);
BOOL __declspec(dllimport) __stdcall EP_RegKeyInformationW(const wchar_t* AName, const wchar_t* AKey, PKeyInformation AKeyInfo);
int __declspec(dllimport) __stdcall EP_RegKeyStatus();
void __declspec(dllimport) __stdcall EP_RegShowDialog();
int __declspec(dllimport) __stdcall EP_RegEncryptRegistrationInformation(const byte* ABuffer, int* ABufferSize, const byte* ARegName, int ARegNameLen, const byte* ARegKey, int ARegKeyLen);
int __declspec(dllimport) __stdcall EP_RegDecryptRegistrationInformation(const byte* ABuffer, int ABufferSize, const byte* ARegName, int* ARegNameLen, const byte* ARegKey, int* ARegKeyLen);

// Enigma Online Activation API
void __declspec(dllimport) __stdcall EP_ActivationShowDialog();

// Trial API
BOOL __declspec(dllimport) __stdcall EP_TrialExecutions( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_TrialExecutionsTotal();
int __declspec(dllimport) __stdcall EP_TrialExecutionsLeft();
BOOL __declspec(dllimport) __stdcall EP_TrialDays( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_TrialDaysTotal();
int __declspec(dllimport) __stdcall EP_TrialDaysLeft();
BOOL __declspec(dllimport) __stdcall EP_TrialExpirationDate( int* Year, int* Month, int* Day );
int __declspec(dllimport) __stdcall EP_TrialExpirationDateEx();
BOOL __declspec(dllimport) __stdcall EP_TrialDateTillDate( int* StartYear, int* StartMonth, int* StartDay, int* EndYear, int* EndMonth, int* EndDay );
int __declspec(dllimport) __stdcall EP_TrialDateTillDateStartEx();
int __declspec(dllimport) __stdcall EP_TrialDateTillDateEndEx();
BOOL __declspec(dllimport) __stdcall EP_TrialExecutionTime( int* Total, int* Left );
int __declspec(dllimport) __stdcall EP_TrialExecutionTimeTotal();
int __declspec(dllimport) __stdcall EP_TrialExecutionTimeLeft();
int __declspec(dllimport) __stdcall EP_TrialClockReversedDays();

// Misc API
int __declspec(dllimport) __stdcall EP_MiscGetWatermark( int ID, PWMContent WM );
int __declspec(dllimport) __stdcall EP_MiscCountryCode();

// Protection API
int __declspec(dllimport) __stdcall EP_ProtectedStringByID( int ID, const char* Str, int Len);
int __declspec(dllimport) __stdcall EP_ProtectedStringByKey( const char* Key, const char* Str, int Len);

// Crypt API
int __declspec(dllimport) __stdcall EP_CryptHashBuffer( int Hash, const byte* Buffer, int Size, const byte* Digest);
int __declspec(dllimport) __stdcall EP_CryptHashFileA( int Hash, const char* FileName, const byte* Digest);
int __declspec(dllimport) __stdcall EP_CryptHashFileW( int Hash, const wchar_t* FileName, const byte* Digest);
int __declspec(dllimport) __stdcall EP_CryptHashStringA( int Hash, const char* Str, const byte* Digest);
int __declspec(dllimport) __stdcall EP_CryptHashStringW( int Hash, const wchar_t* Str, const byte* Digest);
void __declspec(dllimport) __stdcall EP_CryptEncryptBuffer( const byte* Buffer, int Size, const char* Key);
void __declspec(dllimport) __stdcall EP_CryptEncryptBufferEx( const byte* InBuffer, const byte* OutBuffer, int Size, const byte* Key, int KeySize);
void __declspec(dllimport) __stdcall EP_CryptDecryptBuffer( const byte* Buffer, int Size, const char* Key);
void __declspec(dllimport) __stdcall EP_CryptDecryptBufferEx( const byte* InBuffer, const byte* OutBuffer, int Size, const byte* Key, int KeySize);

// Checkup API
BOOL __declspec(dllimport) __stdcall EP_CheckupCopies( int* Total, int* Current);
int __declspec(dllimport) __stdcall EP_CheckupCopiesTotal();
int __declspec(dllimport) __stdcall EP_CheckupCopiesCurrent();
BOOL __declspec(dllimport) __stdcall EP_CheckupIsProtected();
BOOL __declspec(dllimport) __stdcall EP_CheckupIsEnigmaOk();
BOOL __declspec(dllimport) __stdcall EP_CheckupVirtualizationTools();

// Enigma API
int __declspec(dllimport) __stdcall EP_EnigmaVersion();

// Splash screen
int __declspec(dllimport) __stdcall EP_SplashScreenShow();
void __declspec(dllimport) __stdcall EP_SplashScreenHide();
}

